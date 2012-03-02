;===============================================================================================
;                                     		 LJ_RTOS
;                                      The Real-Time Kernel
;
; (c) Copyright 2010, ������
; All Rights Reserved
;
; �ļ����ƣ�lj_cpu_asm.s
; �ļ���ʶ���������Դ�ļ�
; ժ    Ҫ�����ļ���LJ_RTOS�ĺ��Ĳ��֣�����Ҫʵ�����¼�������
;			1.LJCpuIrqIsr   �����������������ڵڶ��β����ж�����
;           2.LJStartHighRdy������ʹ����̬��������ȼ��������У�ֻ��LJStart()������ʹ��
;           3.LJCtxSw       ���������������л�
;           4.LJIntCtxSw    �������жϼ������л�
;           5.LJTickIsr     ����ʱ���жϴ�����
;
; ��ǰ�汾��0.0
; ��    �ߣ�������
; ������ڣ�2010/11/17
;
; ȡ���汾��
; ��    �ߣ�
; ������ڣ�
;===============================================================================================

SRCPND			EQU		0x4a000000		; Դδ���Ĵ���
INTPND			EQU		0x4a000010		; �ж�δ���Ĵ���
EINTPEND		EQU		0x560000a8		; �ⲿ�ж�δ���Ĵ���
INTOFFSET		EQU		0x4a000014		; �ж�ƫ�ƼĴ���

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
	
LJCpuSaveSR					; ����cpsr��ֵ, ���ж�
	mrs		r0, cpsr		; ���Ĵ���cpsr��ֵ�����Ĵ���r0��
	orr		r1, r0, #0xc0	
	msr		cpsr_cxsf, r1	; �رռĴ���cpsr��I, Fλ
	mrs		r1, cpsr		; ȷ���Ѿ���ֹcpsr��I, F�жϽ�ֹλ
	and		r1, r1, #0xc0
	cmp		r1, #0xc0
	bne		LJCpuSaveSR		; �ٴγ��Խ�ֹcpsr��I, F�жϽ�ֹλ
	mov		pc, lr
	
LJCpuRestoreSR				; �ָ�cpsr��ֵ�����ж�
	msr		cpsr_cxsf, r0
	mov		pc, lr

; �ⲿ�жϴ�����

LJCpuIrqIsr
	; ����IRQģʽ
	stmfd	sp!, {r1-r3}
	
	; r1 = sp, r2 = pc, r3 = spsr
	mov		r1, sp
	add		sp, sp, #12						; ��sp_irqָ��IRQ��ջջ��
	; ��IRQ�жϷ���ʱ����������(pc - 4)���浽lr��, �ʷ���ʱ��Ҫ��(lr - 4)
	sub		r2, lr, #4
	
	mrs		r3, spsr
	
	msr		cpsr_cxsf, #SVC_MODE | NOINT	; ������Ȩģʽ
	
	stmfd	sp!, {r2}						; push pc
	stmfd	sp!, {r4-r12, lr}
	
	ldmfd	r1!, {r4-r6}					; ��r1-r3��IRQ��ջ�г�ջ
	stmfd	sp!, {r4-r6}					; ��r1-r3��ջ��SVC��ջ
	stmfd	sp!, {r0}						; ��r0��ջ��SVC��ջ
	stmfd	sp!, {r3}						; ��cpsr��ջ��SVC��ջ
	
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
	msr		cpsr_c, #IRQ_MODE | NOINT	; ����IRQģʽ
	
	ldr		r0, =INTOFFSET
	ldr		r0, [r0]
	
	ldr		r1, IrqIsrVect
	mov		lr, pc
	ldr		pc, [r1, r0, lsl #2]			; ���ö�Ӧ�ⲿ�жϵ��жϷ����ӳ���
	
	msr		cpsr_c, #SVC_MODE | NOINT
	bl		LJIntExit
	
	ldmfd	sp!, {r4}						; pop cpsr
	msr		spsr_cxsf, r4
	ldmfd	sp!, {r0-r12, lr, pc}^
	
IrqIsrVect	DCD		HandleEINT0
	
; void LJStartHighRdy( void )������������ȼ�����
; ��ջ�Ľṹ������ʾ��
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
	msr		cpsr_cxsf, #SVC_MODE | NOINT		; �л�����Ȩģʽ�����ҽ�ֹIRQ & FIQ
	
	ldr		r0, =gLJRunning						; ��gLJRunning��Ϊ1��������ʾ��������������
	mov		r1, #1
	strb	r1, [r0]
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r0, [r0]
	ldr		sp, [r0]
	
	ldmfd	sp!, {r0}
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12, lr, pc}^
	
; void LJCtxSw( void )��������񼶵������л�

LJCtxSw

	; ���洦�����Ĵ���
	
	stmfd	sp!, {lr}			; pc
	stmfd	sp!, {r0-r12, lr}	; r0-r12 lr
	mrs		r0, cpsr			; push cpsr
	stmfd	sp!, {r0}
	
	; �ڵ�ǰ���������������ƿ��б��浱ǰ����Ķ�ջָ��
	; gLJTcbCur->ljTcbStkTop = sp
	
	ldr		r0, =gLJTcbCur
	ldr		r0, [r0]
	str		sp, [r0]
	
	; ��ָ��ǰ�����ָ��ָ��Ҫ�ָ����е����񣬼���������ȡ���˵�ǰ����
	; gLJTcbCur = gLJTcbHighRdy
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r1, =gLJTcbCur
	ldr		r0, [r0]
	str		r0, [r1]
	
	; ������������ȼ����Ƹ���ǰ��������ȼ�
	; gLJPrioCur = gLJPrioHighRdy
	
	ldr		r0, =gLJPrioHighRdy
	ldr		r1, =gLJPrioCur
	ldrb	r0, [r0]
	strb	r0, [r1]
	
	; ���������������ƿ��еõ�������Ķ�ջָ��
	; sp = gLJTcbHighRdy->ljTcbStkTop
	
	ldr		r0, =gLJTcbHighRdy
	ldr		r0, [r0]
	ldr		sp, [r0]
	
	; �ָ���Ҫ���������CPU�Ĵ���
	ldmfd	sp!, {r0}		; pop cpsr
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12, lr, pc}^
	
; void LJIntCtxSw( void )���жϼ������л�

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
	
	; ���������л����л����µ�������ȼ�����ִ��
	ldmfd	sp!, {r0}		; pop cpsr
	msr		spsr_cxsf, r0
	ldmfd	sp!, {r0-r12, lr, pc}^
	
; void LJTickIsr( void )��ע�⣺�����ڵ���LJStart������ʱ�ӽ����ж�

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
	ldr		r1, =0x1dd7fc		; GPB5, GPB6, GPB8, GPB10����Ϊ���
	str		r1, [r0]
	
	ldr		r0, =GPBDAT
	ldr		r1, =((1 << 5) | (1 << 6) | (1 << 8) | (1 << 10))	; ʹLEDȫ��
;	orr		r1, r1, #0xffe		; �رշ�����
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
	