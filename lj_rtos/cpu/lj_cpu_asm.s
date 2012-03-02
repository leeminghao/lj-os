;===============================================================================================
;                                     		 LJ_RTOS
;                                      The Real-Time Kernel
;
; (c) Copyright 2010, 李明浩
; All Rights Reserved
;
; 文件名称：lj_cpu_asm.s
; 文件标识：汇编语言源文件
; 摘    要：本文件是LJ_RTOS的核心部分，其主要实现以下几个函数
;			1.LJCpuIrqIsr   ：在启动代码中用于第二次查找中断向量
;           2.LJStartHighRdy：用于使就绪态中最高优先级任务运行，只在LJStart()函数中使用
;           3.LJCtxSw       ：用于任务级任务切换
;           4.LJIntCtxSw    ：用于中断级任务切换
;           5.LJTickIsr     ：定时器中断处理函数
;
; 当前版本：0.0
; 作    者：李明浩
; 完成日期：2010/11/17
;
; 取代版本：
; 作    者：
; 完成日期：
;===============================================================================================

SRCPND			EQU		0x4a000000		; 源未决寄存器
INTPND			EQU		0x4a000010		; 中断未决寄存器
EINTPEND		EQU		0x560000a8		; 外部中断未决寄存器
INTOFFSET		EQU		0x4a000014		; 中断偏移寄存器

GPBCON			EQU		0x56000010
GPBDAT			EQU		0x56000014
GPBUP			EQU		0x56000018
	
USER_MODE		EQU		0x10
FIQ_MODE		EQU		0x11
IRQ_MODE		EQU		0x12
SVC_MODE		EQU		0x13
ABORT_MODE		EQU		0x17
UNDEF_MODE		EQU		0x1b
MODE_MASK		EQU		0x1f
NOINT			EQU		0xc0

	IMPORT		gLJRunning
	IMPORT		gLJTcbCur
	IMPORT		gLJTcbHighRdy
	IMPORT		gLJPrioCur
	IMPORT		gLJPrioHighRdy
	IMPORT		gLJIntNesting
	
	IMPORT		LJIntEnter
	IMPORT		LJIntExit
	IMPORT		LJTimeTick
	IMPORT		HandleEINT0
	
	EXPORT		LJCpuIrqIsr

	EXPORT		LJStartHighRdy
	EXPORT		LJCtxSw
	EXPORT		LJTickIsr
	EXPORT		LJIntCtxSw
	
	EXPORT		LJCpuSaveSR
	EXPORT		LJCpuRestoreSR
	
	AREA		LJArm, CODE, READONLY
	
LJCpuSaveSR					; 保存cpsr的值, 关中断
	mrs		r0, cpsr		; 将寄存器cpsr的值读到寄存器r0中
	orr		r1, r0, #0xc0	
	msr		cpsr_cxsf, r1	; 关闭寄存器cpsr中I, F位
	mrs		r1, cpsr		; 确认已经禁止cpsr中I, F中断禁止位
	and		r1, r1, #0xc0
	cmp		r1, #0xc0
	bne		LJCpuSaveSR		; 再次尝试禁止cpsr中I, F中断禁止位
	mov		pc, lr
	
LJCpuRestoreSR				; 恢复cpsr的值，开中断
	msr		cpsr_cxsf, r0
	mov		pc, lr

; 外部中断处理函数

LJCpuIrqIsr
	; 进入IRQ模式
	stmfd	sp!, {r1-r3}
	
	; r1 = sp, r2 = pc, r3 = spsr
	mov		r1, sp
	add		sp, sp, #12						; 将sp_irq指向IRQ堆栈栈顶
	; 当IRQ中断发生时，处理器将(pc - 4)保存到lr中, 故返回时，要将(lr - 4)
	sub		r2, lr, #4
	
	mrs		r3, spsr
	
	msr		cpsr_cxsf, #SVC_MODE | NOINT	; 进入特权模式
	
	stmfd	sp!, {r2}						; push pc
	stmfd	sp!, {r4-r12, lr}
	
	ldmfd	r1!, {r4-r6}					; 将r1-r3从IRQ堆栈中出栈
	stmfd	sp!, {r4-r6}					; 将r1-r3进栈到SVC堆栈
	stmfd	sp!, {r0}						; 将r0进栈到SVC堆栈
	stmfd	sp!, {r3}						; 将cpsr进栈到SVC堆栈
	
	ldr		r0, =gLJIntNesting
	ldrb	r1, [r0]
	add		r1, r1, #1
	strb	r1, [r0]
	
	cmp		r1, #1
	bne		%F1
	
	ldr		r4, =gLJTcbCur
	ldr		r5, [r4]
	str		sp, [r5]
	
1
	msr		cpsr_c, #IRQ_MODE | NOINT	; 进入IRQ模式
	
	ldr		r0, =INTOFFSET
	ldr		r0, [r0]
	
	ldr		r1, IrqIsrVect
	mov		lr, pc
	ldr		pc, [r1, r0, lsl #2]			; 调用对应外部中断的中断服务子程序
	
	msr		cpsr_c, #SVC_MODE | NOINT
	bl		LJIntExit
	
	ldmfd	sp!, {r4}						; pop cpsr
	msr		spsr_cxsf, r4
	ldmfd	sp!, {r0-r12, lr, pc}^
	
IrqIsrVect	DCD		HandleEINT0
	
; void LJStartHighRdy( void )：运行最高优先级任务
; 堆栈的结构如下所示：
; 								Entry Point( Task Name )		(High memory)
; 								LR( R14 )
;							 	R12
; 								R11
; 								R10
; 								R9
; 								R8
; 								R7
; 								R6
; 								R5
; 								R4
; 								R3
; 								R2
; 								R1
; 								R0 (argument)
; 	gLJTcbHighRdy->ljTcbStkTop->CPSR							(Low Memory)

LJStartHighRdy
	msr		cpsr_cxsf, #SVC_MODE | NOINT		; 切换到特权模式，并且禁止IRQ & FIQ
	
	ldr		r0, =gLJRunning						; 将gLJRunning置为1，即：表示有任务正在运行
	mov		r1, #1
	strb	r1, [r0]
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r0, [r0]
	ldr		sp, [r0]
	
	ldmfd	sp!, {r0}
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12, lr, pc}^
	
; void LJCtxSw( void )：完成任务级的任务切换

LJCtxSw

	; 保存处理器寄存器
	
	stmfd	sp!, {lr}			; pc
	stmfd	sp!, {r0-r12, lr}	; r0-r12 lr
	mrs		r0, cpsr			; push cpsr
	stmfd	sp!, {r0}
	
	; 在当前运行任务的任务控制块中保存当前任务的堆栈指针
	; gLJTcbCur->ljTcbStkTop = sp
	
	ldr		r0, =gLJTcbCur
	ldr		r0, [r0]
	str		sp, [r0]
	
	; 将指向当前任务的指针指向要恢复运行的任务，即：新任务取代了当前任务
	; gLJTcbCur = gLJTcbHighRdy
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r1, =gLJTcbCur
	ldr		r0, [r0]
	str		r0, [r1]
	
	; 将新任务的优先级复制给当前任务的优先级
	; gLJPrioCur = gLJPrioHighRdy
	
	ldr		r0, =gLJPrioHighRdy
	ldr		r1, =gLJPrioCur
	ldrb	r0, [r0]
	strb	r0, [r1]
	
	; 从新任务的任务控制块中得到新任务的堆栈指针
	; sp = gLJTcbHighRdy->ljTcbStkTop
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r0, [r0]
	ldr		sp, [r0]
	
	; 恢复将要运行任务的CPU寄存器
	ldmfd	sp!, {r0}		; pop cpsr
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12, lr, pc}^
	
; void LJIntCtxSw( void )：中断级任务切换

LJIntCtxSw
	
	; gLJTcbCur = gLJTcbHighRdy
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r1, =gLJTcbCur
	ldr		r0, [r0]
	str		r0, [r1]
	
	; gLJPrioCur = gLJPrioHighRdy
	
	ldr		r0, =gLJPrioHighRdy
	ldr		r1, =gLJPrioCur
	ldrb	r0, [r0]
	strb	r0, [r1]
	
	; sp = gLJTcbHighRdy->ljTcbStkTop
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r0, [r0]
	ldr		sp, [r0]
	
	; 进行任务切换，切换到新的最高优先级任务执行
	ldmfd	sp!, {r0}		; pop cpsr
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12, lr, pc}^
	
; void LJTickIsr( void )：注意：必须在调用LJStart后，启动时钟节拍中断

LJTickIsr
	mov		r5, lr		; pc
	
	mov		r1, #1
	mov		r1, r1, lsl #10
	
	ldr		r0, =SRCPND
	ldr		r2, [r0]
	orr		r1, r1, r2
	str		r1, [r0]
	
	ldr		r0, =INTPND
	ldr		r1, [r0]
	str		r1, [r0]
	
	bl		LJTimeTick
	
	mov		pc, r5			; RETURN
	
LedTest
	ldr		r0, =GPBCON
	ldr		r1, =0x1dd7fc		; GPB5, GPB6, GPB8, GPB10设置为输出
	str		r1, [r0]
	
	ldr		r0, =GPBDAT
	ldr		r1, =((1 << 5) | (1 << 6) | (1 << 8) | (1 << 10))	; 使LED全灭
;	orr		r1, r1, #0xffe		; 关闭蜂鸣器
	str		r1, [r0]
	
	ldr		r2, =GPBUP
	ldr		r3, =0x00
	str		r3, [r2]
	
	ldr		r1, =(~(1 << 5));
	str		r1, [r0]
	;ldr		r1, =(~(1 << 10))
	;str		r1, [r0]
	
	mov		pc, lr
	
	END
	