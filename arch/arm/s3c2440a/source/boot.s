;===============================================================================================
;											LJ_RTOS
;									The Real-Time Kernel
;
; (c) Copyright 2010, 李明浩
; All Rights Reserved
;
; 文件名称：boot.s
; 文件标识：汇编源文件
; 摘    要：上电后就从本文件开始运行，实现基本初始化，完成nor和nand启动的判断，完成把程序
;           搬运到SDRAM中，在搬运完成后跳转到Main函数执行。主要完成功能包括：
;          (1) 建立异常中断的入口向量
;          (2) 建立中断向量表
;          (3) 为ARM每种运行模式设置堆栈
;          (4) 初始化MPLL时钟
;          (5) 初始化MMU(内存管理单元)
;          (6) 初始化存储器控制器
;          (7) 关闭看门狗、中断
;          (8) 判断IRQ中断的中断入口
;          (9) 将RW段的内容从flash拷贝到SDRAM，初始化ZI段为0
;          (10)跳转到应用程序
;
; 当前版本：0.0
; 作    者：李明浩
; 完成日期：2010/11/8
;
; 取代版本：
; 作    者：
; 完成日期：
;================================================================================================

		GET	clkcfg.inc		; 系统时钟的配置
		GET memcfg.inc		; 存储器的配置
		GET regaddr.inc		; 寄存器地址的声明
		
BIT_SELFREFRESH		EQU		(0x1 << 22)			; 声明自刷新位

; 预定义7中工作模式
USER_MODE	EQU		0x10	; 用户模式
FIQ_MODE	EQU		0x11	; 快速中断模式
IRQ_MODE	EQU		0x12	; 外部中断模式
SVC_MODE	EQU		0x13	; 特权模式
ABORT_MODE	EQU		0x17	; 异常中断模式
UNDEF_MODE	EQU		0x1b	; 未定义模式
MODE_MASK	EQU		0x1f	; 系统模式(模式掩码)

NOINT		EQU		0xc0	; 禁用IRQ/FIQ中断

; 定义7中处理器模式的堆栈起始地址，其中用户模式和系统模式共用一个栈空间
; STACK_BASEADDRESSS = 0x33ff8000
USER_STACK	EQU		(STACK_BASEADDRESS - 0x3800)	;0x33ff4800
SVC_STACK	EQU		(STACK_BASEADDRESS - 0x2800)	;0x33ff5800
UNDEF_STACK	EQU		(STACK_BASEADDRESS - 0x2400)	;0x33ff5c00
ABORT_STACK	EQU		(STACK_BASEADDRESS - 0x2000)	;0x33ff6000
IRQ_STACK	EQU		(STACK_BASEADDRESS - 0x1000)	;0x33ff7000
FIQ_STACK	EQU		(STACK_BASEADDRESS - 0x0)		;0x33ff8000

; 判断是不是Thumb指令
	GBLL	THUMBCODE
	[ {CONFIG} = 16		; CONFIG为汇编器的内置变量
THUMBCODE	SETL	{TRUE}
		CODE32
	|
THUMBCODE	SETL	{FALSE}
	]
	
; 宏定义MOV_PC_LR, 作用：用于子程序返回
		MACRO
	MOV_PC_LR
	[ THUMBCODE
	 bx lr
	|
	 mov pc, lr
	]
	MEND
		
; 加载程序，把中断服务程序的首地址装载到PC中，在程序的末尾定义了一个数据区，存放
; 相应的中断服务子程序地址，在向量中断模式下使用"加载程序"来执行中断服务程序
		MACRO
$HandlerLabel	HANDLER	$HandleLabel

$HandlerLabel
	sub		sp, sp, #4			; 计算返回地址
	stmfd	sp!, {r0}			; 将r0中的值压入栈中
	ldr		r0, =$HandleLabel	; 把Handle***地址放入r0
	ldr		r0, [r0]			; 读取中断服务程序地址放入r0
	str		r0, [sp, #4]		; 将中断服务程序地址压入栈中
	ldmfd	sp!, {r0, pc}		; 跳转到中断服务程序并回复r0的原值
	MEND
	
; 引入编译器生成的变量，RO, RW, ZI这三个段都保存在flash中，但RW, ZI段的地址肯定不是
; 程序运行时的变量所存储的位置，因此我们的程序初始化时应该吧flash中的RW, ZI段拷贝到
; SDRAM对应的位置，这些变量是通过ADS的工程设置里面设定的RO Base和RW Base设定的，最
; 终由编译器脚本和链接程序导入程序
	IMPORT		|Image$$RO$$Base|		; RO段运行时开始地址
	IMPORT		|Image$$RO$$Limit|		; RO段运行时结束地址
	IMPORT		|Image$$RW$$Base|		; RW段的运行时开始地址
	IMPORT		|Image$$ZI$$Base|		; ZI段的运行时开始地址
	IMPORT		|Image$$ZI$$Limit|		; ZI段的运行时结束地址
	
	;IMPORT		MmuSetAsyncBusMode		; 引入设置异步总线模式函数外部变量
	;IMPORT		MmuSetFastBusMode		; 引入设置快速总线模式函数外部变量
	
	IMPORT		Main
	IMPORT		LJCpuIrqIsr				; 引入操作系统中断处理函数
	EXPORT		HandleEINT0
	EXPORT		EnterPWDN
	EXPORT		__ENTRY
	
	AREA	Init, CODE, READONLY
	
	ENTRY	; 程序入口
	
__ENTRY		; 主要用于MMU
ResetEntry	; 复位后入口

; 在0x0处的异常中断是复位异常中断，是上电后执行的第一条指令，变量ENDIAN_CHANGE用于
; 标记是否要从小端模式改变为大端模式，因为编译器初始模式是小端模式，如果要用大端模
; 式，就要事先把该变量设置为TRUE，否则为FALSE
; 变量ENTRY_BUS_WIDTH用于设置总线宽度，因为16位和8为宽度来表示32位数据时，小端模式
; 下，数据的含义是不同的
; 由于要考虑到大端和小端模式，以及总线宽度，因此此处看似复杂，其实只是一条跳转指令
; 当为大端模式时：跳转到ChangeBigEndian函数处，否则跳转到ResetHandler函数处

	ASSERT	:DEF:ENDIAN_CHANGE
	
	[ ENDIAN_CHANGE
	
		ASSERT	:DEF:ENTRY_BUS_WIDTH
		
		[ ENTRY_BUS_WIDTH = 32
		; 跳转到ChangeBigEndian(ChangeBigEndian在0x24处)，因此该条指令的机器码为：
		; 0xea000007，所以该语句在该处(0x0)同直接放入0xea000007数据(即：DCD 0xea000007)
		; 作用相同
		 b	ChangeBigEndian				; <=>DCD 0xea000007
		]
		
		[ ENTRY_BUS_WIDTH = 16
		 andeq	r14, r7, r0, lsl #20	; <=>DCD 0x0007ea00
		]
		
		[ ENTRY_BUS_WIDTH = 8
		 streq	r0, [r0, -r10, ror #1]	; <=>DCD 0x070000ea
		]
	|
		b	ResetHandler
	]
	b	HandlerUndef	; 未定义异常中断
	b	HandlerSWI		; 软件异常中断
	b	HandlerPabort	; 指令预取异常中断
	b	HandlerDabort	; 数据访问异常中断
	b	.				; 保留，跳转到自身地址处，即进入死循环
	b	HandlerIRQ		; 外部中断请求
	b	HandlerFIQ		; 快速中断请求
	; 以上为一级中断向量表
	
; 跳转到EnterPWDN，处理电源管理的其它非正常模式，在c语言程序中被调用
	b	EnterPWDN		; @0x20
	
; @0x24
ChangeBigEndian

	[ ENTRY_BUS_WIDTH = 32
		DCD	0xee110f10		; <=>mrc p15, 0, r0, c1, c0, 0
		DCD	0xe3800080		; <=>orr r0, r0, #0x80
		DCD	0xee010f10		; <=>mcr p15, 0, r0, c1, c0, 0
	]
	
	[ ENTRY_BUS_WIDTH = 16
		DCD	0x0f10ee11
		DCD	0x0080e380
		DCD	0x0f10ee01
	]
	
	[ ENTRY_BUS_WIDTH = 8
		DCD	0x100f11ee
		DCD	0x800080e3
		DCD	0x100f01ee
	]
	
	; 此后系统就能自动识别出不同总线宽度下的大端模式，因此以后就无需人为调整指令了
	DCD	0xffffffff
	DCD	0xffffffff
	DCD	0xffffffff
	DCD	0xffffffff
	b 	ResetHandler
		
; 当系统进入异常中断后，由存放在0x0~0x1c处的中断向量地址中的跳转指令跳转到此处相应
; 的位置，并由事先定义好的宏定义再次跳转到相应的中断服务程序中
HandlerFIQ		HANDLER		HandleFIQ
HandlerIRQ		HANDLER		HandleIRQ
HandlerUndef	HANDLER		HandleUndef
HandlerSWI		HANDLER		HandleSWI
HandlerDabort	HANDLER		HandleDabort
HandlerPabort	HANDLER		HandlePabort

; 下面代码用于处理非向量中断，即由软件程序来判断到底发生了哪种中断，然后跳转到相应
; 的中断服务程序中，具体的说，就是会置INTOFFSET寄存器相应的位为1，然后通过查表，找
; 到相应的中断入口地址
IsrIRQ
	sub		sp, sp, #4			; 在栈中预留4字节空间，以便保存中断入口地址
	stmfd	sp!, {r8-r9}		; 保存r8,r9的值
	ldr		r9, =INTOFFSET		; 把INTOFFSET寄存器地址装入r9中
	ldr		r9, [r9]			; 读取INTOFFSET寄存器的内容
	ldr		r8, =HandleEINT0	; 得到中断向量表的基址
	add		r8, r8, r9, lsl #2	; 得到中断服务程序的入口地址
	ldr		r8, [r8]
	str		r8, [sp, #8]		; 将中断服务程序入口地址装入栈中
	
	ldmfd	sp!, {r8-r9, pc}	; 恢复r8, r9寄存器值，并跳转到相应中断服务程序中
	
	LTORG
	
; 系统复位后，由0x0处的跳转指令，跳转到该处真正执行系统初始化工作，复位向量是ARM
; 处理器上电后第一个被执行的异常，此时系统处于管理模式
; 复位向量有以下六件事要做：
; 1.关闭看门狗定时器和屏蔽所有中断
; 2.配置系统时钟
; 3.配置内存控制器
; 4.配置每种处理器模式下的堆栈指针
; 5.初始化镜像运行域
; 6.跳转到操作系统入口
ResetHandler
	
	; 关闭看门狗
	ldr		r0, =WTCON
	ldr		r1, =0x0
	str		r1, [r0]
	
	; 屏蔽所有中断和子中断
	ldr		r0, =INTMSK
	ldr		r1, =0xffffffff
	str		r1, [r0]
	
	ldr		r0, =SUBINTMSK
	ldr		r1, =0x7fff
	str		r1, [r0]
	
; 刚设置好PLL时，系统认为这时PLL还没稳定，所以这时不用PLL的时钟，而用外部晶振做
; 时钟，将PLL锁定，过了LOCKTIME后认为PLL已经稳定了，才使用PLL给系统提供时钟
; s3c2440a要求PLL锁定时间大于300us，即：(1 / 12M) * N > 300us, 其中N为U_LTIME或
; M_LTIME，所以有N > 12M * 300us = 3600设置LOCKTIME=0x0fff0fff足够了，低16位为
; MPLL，高16位为UPLL
	ldr		r0, =LOCKTIME
	ldr		r1, =0xffffff
	str		r1, [r0]

	[ PLL_ON_START
		ldr		r0, =CLKDIVN
		ldr		r1, =CLKDIV_VAL
		str		r1, [r0]
		
		[ CLKDIV_VAL > 1	; 设置为异步总线模式
		 mrc	p15, 0, r0, c1, c0, 0
		 orr	r0, r0, #0xc0000000
		 mcr	p15, 0, r0, c1, c0, 0
		|					; 设置为快速总线模式
		 mrc	p15, 0, r0, c1, c0, 0
		 bic	r0, r0, #0xc0000000
		 mcr	p15, 0, r0, c1, c0, 0	
		]
		
; 配置时钟：s3c2440a要求先配置UPLL后配置MPLL且之间间隔7 NOP
		
		ldr		r0, =UPLLCON	; 配置UPLL为48MHz, 否则就不能使用USB接口了
		ldr		r1, =((U_MDIV << 12) + (U_PDIV << 4) + U_SDIV)
		str		r1, [r0]
		
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		
		ldr		r0, =MPLLCON	; 配置MPLL, FCLK = 304MHz
		ldr		r1, =((M_MDIV << 12) + (M_PDIV << 4) + M_SDIV)
		str		r1, [r0]
	]
	
; 从SLEEP模式被唤醒，类似RESET引脚被触发，因此它也要从0x0处开始执行，
; 在此处要判断是否是由SLEEP模式唤醒引起的复位
	ldr		r1, =GSTATUS2
	ldr		r0, [r1]
	tst		r0, #0x2
	bne		WAKEUP_SLEEP
	
; 设置一个被唤醒复位后的起始点地址标号，可以把它保存到GSTATUS3中，导出该地址标号
	EXPORT	StartPointAfterSleepWakeUp
StartPointAfterSleepWakeUp
	adrl	r0, SMRDATA
	ldr		r1, =BWSCON
	add		r2, r0, #52
0
	ldr		r3, [r0], #4
	str		r3, [r1], #4
	cmp		r2, r0
	bne		%B0

	bl		InitStacks		; 初始化堆栈
	
; 安装外部中断处理程序
	ldr		r0, =HandleIRQ
	ldr		r1, =LJCpuIrqIsr
	str		r1, [r0]

;	bl LedTest	
; 判断是从Nor还是Nand启动
	ldr		r0, =BWSCON
	ldr		r0, [r0]
	ands	r0, r0, #6		; OM[1:0] != 0, 从Nor Flash启动
	bne		CopyProcBeg		; 不用读取Nand Flash
	adr		r0, ResetEntry
	cmp		r0, #0			; 再比较入口是否为0地址处，如果不是则用了仿真器
	bne		CopyProcBeg		; 用仿真器的情况也不要用Nand Flash启动，如，JTAG调试时时直接下载到内存中运行
	
NandBootBeg					; 这段代码完成从nand读代码到SDRAM
	bl		ClearSdram
	mov		r5, #NFCONF
	ldr		r0, =(7 << 12) | (7 << 8) | (7 << 4)
	str		r0, [r5]
	ldr		r0, =(0<<13) | (0<<12) | (0<<10) | (0<<9) | (0<<8) | (1<<6) | (1<<5) | (1<<4) | (1<<1) | (1<<0)
	str		r0, [r5, #4]
	
	bl		ReadNandID			; 读取Nand的ID号，结果保存在r5里
	mov		r6, #0				; r6设初值0
	ldr		r0, =0xecda			; 期望的Nand ID号.
	cmp		r5, r0				; 这里进行比较
	beq		%F1					; 相等的话就跳到下一个标号处
	mov		r6, #1				; 不相等就跳到下一个标号处
1
	bl		ReadNandStatus		; 读取Nand的状态，结果放在r1中
	
	mov		r8, #0				; r8设初值0，意义为页号
; 注意，在这里使用的的是ldr伪指令，而不是上面用的adr伪指令,它加载的是ResetEntry
; 的绝对地址，也就是我们期望的Ram中的地址, 在这里它和|Image$$RO$$Base|一样
	ldr		r9, =ResetEntry
2
	ands	r0, r8, #0x3f		; 凡r8为0x3f(64)的整数倍，eq有效，ne无效;对每个块(64页)进行检错
	bne		%F3
	mov		r0, r8				; r8->r0
	bl		CheckBadBlk			; 检查Nand的坏块
	cmp		r0, #0				; 比较r0和0
	addne	r8, r8, #64			; 存在坏块的话就跳过这个坏块
	bne		%F4
3
	mov		r0, r8
	mov		r1, r9
	bl		ReadNandPage
	add		r9, r9, #2048		; 每页的字节数
	add		r8, r8, #1
4
	cmp		r8, #131072			; 128KB
	bcc		%B2
	
	mov		r5, #NFCONF
	ldr		r0, [r5, #4]
	bic		r0, r0, #1
	str		r0, [r5, #4]
	
	ldr		pc, =CopyProcBeg	; 此处跳转到内存空间
; 若是从Nand启动，则拷贝工作已经在NandBootBeg中完成，所以直接跳转到Main
; 若是从Nor启动，则将RO和RW部分拷贝到内存，然后跳转到内存运行(也可在NOR中运行，只是速度稍慢)
; 注：若在Nor中直接运行，需把RO/BASE改为0并定义RW/BASE,会跳过RO拷贝
CopyProcBeg
	adr		r0, ResetEntry		; 相对地址装载，与绝对地址无关，即指向Flash中RO区的开始地址
	ldr		r2, BaseOfRom		; 绝对地址装载，即RO区拷贝到RAM后的开始地址
	cmp		r0, r2				; 如果是第一次启动R0 != r2
	ldreq	r0, TopOfRom		; 在Flash中运行时，不要进行代码搬迁
	beq		InitRam
	
; 下面这个是针对代码在Nor Flash时的拷贝方法，功能为：
; 把从ResetEntry起，TopOfRom - BaseOfRom大小的数据拷贝到BaseOfRom
	ldr		r3, TopOfRom
0
	ldmia	r0!, {r4-r7}		; 将保存在Flash中的RO区全部复制到RAM中
	stmia	r2!, {r4-r7}
	cmp		r2, r3
	bcc		%B0
	
	sub		r2, r2, r3			; r2 = BaseOfROM - TopOfRom = -代码长度
	sub		r0, r0, r2			; r0 = ResetEntry - (-)代码长度 = ResetEntry + 代码长度
	
InitRam
	ldr		r2, BaseOfBss		; 装载数值RWBase, 指向RAM中RW区首地址
	ldr		r3, BaseOfZero		; 装载数值ZIBase, 指向RAM中ZI区首地址，也是RW区尾地址
0
	cmp		r2, r3				; 在运行时应将RW, ZI重定位到SDRAM中
	ldrcc	r1, [r0], #4
	strcc	r1, [r2], #4
	bcc		%B0
	
	mov		r0, #0
	ldr		r3, EndOfBss
1
	cmp		r2, r3
	strcc	r0, [r2], #4
	bcc		%B1

	ldr		pc, =%F2
2

	[ :LNOT:THUMBCODE 
	 bl Main
	 b	.		; 跳转到Main不成功则挂起
	]
	
	[ THUMBCODE
	 orr	lr, pc, #1
	 bx		lr
	 CODE	16
	 bl		Main
	 b		.
	]
	
ClearSdram			; 清空Sdram
	mov		r1, #0
	mov		r2, #0
	mov		r3, #0
	mov		r4, #0
	mov		r5, #0
	mov		r6, #0
	mov		r7, #0
	mov		r8, #0
	
	ldr		r9, =0x4000000	; 64MB
	ldr		r0, =0x30000000
0
	stmia	r0!, {r1-r8}
	subs	r9, r9, #32
	bne		%B0
	mov	pc, lr
	
InitStacks					; 初始化堆栈
; stmfd, ldmfd必须在建立堆栈后才能使用
	mrs		r0, cpsr
	bic		r0, r0, #MODE_MASK
	orr		r1, r0, #UNDEF_MODE | NOINT
	msr		cpsr_cxsf, r1
	ldr		sp, =UNDEF_STACK
	
	orr		r1, r0, #ABORT_MODE | NOINT
	msr		cpsr_cxsf, r1
	ldr		sp, =ABORT_STACK
	
	orr		r1, r0, #IRQ_MODE | NOINT
	msr		cpsr_cxsf, r1
	ldr		sp, =IRQ_STACK
	
	orr		r1, r0, #FIQ_MODE | NOINT
	msr		cpsr_cxsf, r1
	ldr		sp, =FIQ_STACK
	
	bic		r0, r0, #MODE_MASK | NOINT
	orr		r1, r0, #SVC_MODE
	msr		cpsr_cxsf, r1
	ldr		sp, =SVC_STACK	

; 因为系统模式和用户模式共用一个栈空间，因此不用再重复设置用户模式堆栈，系统复位后进入的
; 是SVC模式，而且各种模式下的lr不同，因此想要从该函数返回，就要首先切换到SVC模式，再使用
; lr, 这样就可以正确返回了
	mov		pc, lr

; Nand Flash基本操作
ReadNandID
	mov		r7, #NFCONF
	ldr		r0, [r7, #4]
	bic		r0, r0, #0x2
	str		r0, [r7, #4]
	
	mov		r0, #0x90		; 0x90读取Nand ID命令
	strb	r0, [r7, #8]
	mov		r4, #0
	strb	r4, [r7, #0x0c]
1
	ldr		r0, [r7, #0x20]
	tst		r0, #0x1
	beq		%B1
	
	ldrb	r0, [r7, #0x10]
	mov		r0, r0, lsl #8
	ldrb	r1, [r7, #0x10]
	orr		r5, r1, r0
	
	ldr		r0, [r7, #4]
	orr		r0, r0, #0x02
	str		r0, [r7, #4]
	
	mov		pc, lr
	
ReadNandStatus
	mov		r7, #NFCONF
	ldr		r0, [r7, #4]
	bic		r0, r0, #0x2
	str		r0, [r7, #4]
	
	mov		r0, #0x70		; 0x70读取Nand Status
	strb	r0, [r7, #8]
	ldrb	r1, [r7, #0x10]
	
	ldr		r0, [r7, #4]
	orr		r0, r0, #0x02
	str		r0, [r7, #4]
	mov		pc, lr
	
WaitNandBusy
	mov		r1, #NFCONF
	mov		r0, #0x70
	strb	r0, [r1, #8]
1
	ldrb	r0, [r1, #0x10]
	tst		r0, #0x40
	beq		%B1
	
	mov		r0, #0
	strb	r0, [r1, #8]
	mov		pc, lr
	
CheckBadBlk
	mov		r7, lr
	mov		r5, #NFCONF
	
	bic		r0, r0, #0x3f	; addr &= ~0x3f
	ldr		r1, [r5, #4]
	bic		r1, r1, #2
	str		r1, [r5, #4]
	
	mov		r1, #0x00
	strb	r1, [r5, #8]
	mov		r1, #0			
	strb	r1, [r5, #0xc]
	mov		r1, #8
	strb	r1, [r5, #0xc]
	
	strb	r0, [r5, #0xc]	; WrNFAddr( addr )
	mov		r1, r0, lsr #8
	strb	r1, [r5, #0xc]
	cmp		r6, #0
	movne	r1, r0, lsr #16
	strb	r1, [r5, #0xc]
	
	mov		r1, #0x30
	strb	r1, [r5, #8]
	
	mov		r0, #100
1	
	subs	r0, r0, #1
	bne		%B1
2
	ldr		r0, [r5, #0x20]
	tst		r0, #1
	beq		%B2
	
	ldrb	r0, [r5, #10]
	sub		r0, r0, #0xff
	
	ldr		r1, [r5, #4]
	orr		r1, r1, #2
	strb	r1, [r5, #4]
	
	mov		pc, r7
	
ReadNandPage
	mov		r7, lr
	mov		r4, r1
	mov		r5, #NFCONF
	
	ldr		r1, [r5, #4]
	bic		r1, r1, #2
	str		r1, [r5, #4]
	
	mov		r1, #0
	strb	r1, [r5, #8]
	
	strb	r1, [r5, #0xc]	; 1
	strb	r1, [r5, #0xc]	; 2
	strb	r0, [r5, #0xc]	; 3
	mov		r1, r0, lsr #8
	strb	r1, [r5, #0xc]	; 4
	cmp		r6, #0
	movne	r1, r0, lsr #16
	strb	r1, [r5, #0xc]	; 5
	
	mov		r1, #0x30
	strb	r1, [r5, #8]
	
	ldr		r0, [r5, #4]
	orr		r0, r0, #0x10
	str		r0, [r5, #4]
	
	bl		WaitNandBusy
	
	mov		r0, #0
1
	ldrb	r1, [r5, #0x10]
	strb	r1, [r4, r0]
	add		r0, r0, #1
	bic		r0, r0, #0x10000
	cmp		r0, #0x800
	bcc		%B1
	
	ldr		r0, [r5, #4]
	orr		r0, r0, #2
	str		r0, [r5, #4]
	
	mov		pc, r7

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
	
SMRDATA		DATA
	DCD	(0+(B1_BWSCON<<4)+(B2_BWSCON<<8)+(B3_BWSCON<<12)+(B4_BWSCON<<16)+(B5_BWSCON<<20)+(B6_BWSCON<<24)+(B7_BWSCON<<28))
	DCD	((B0_TACS<<13) + (B0_TCOS<<11) + (B0_TACC<<8) + (B0_TCOH<<6) + (B0_TAH<<4) + (B0_TACP<<2) + (B0_PMC))
	DCD	((B1_TACS<<13) + (B1_TCOS<<11) + (B1_TACC<<8) + (B1_TCOH<<6) + (B1_TAH<<4) + (B1_TACP<<2) + (B1_PMC))
	DCD	((B2_TACS<<13) + (B2_TCOS<<11) + (B2_TACC<<8) + (B2_TCOH<<6) + (B2_TAH<<4) + (B2_TACP<<2) + (B2_PMC))
	DCD	((B3_TACS<<13) + (B3_TCOS<<11) + (B3_TACC<<8) + (B3_TCOH<<6) + (B3_TAH<<4) + (B3_TACP<<2) + (B3_PMC))
	DCD	((B4_TACS<<13) + (B4_TCOS<<11) + (B4_TACC<<8) + (B4_TCOH<<6) + (B4_TAH<<4) + (B4_TACP<<2) + (B4_PMC))
	DCD	((B5_TACS<<13) + (B5_TCOS<<11) + (B5_TACC<<8) + (B5_TCOH<<6) + (B5_TAH<<4) + (B5_TACP<<2) + (B5_PMC))
	DCD	((B6_MT << 15) + (B6_TRCD << 2) + (B6_SCAN))
	DCD	((B7_MT << 15) + (B7_TRCD << 2) + (B7_SCAN))
	DCD	((REFRESH_EN << 23) + (REFRESH_MODE << 22) + (TRP << 20) + (TRC << 18) + (TCHR << 16) + (REFRESH_CNT))
	DCD	0x32
	DCD	0x20
	DCD	0x20
	
BaseOfRom	DCD		|Image$$RO$$Base|
TopOfRom	DCD		|Image$$RO$$Limit|
BaseOfBss	DCD		|Image$$RW$$Base|
BaseOfZero	DCD		|Image$$ZI$$Base|
EndOfBss	DCD		|Image$$ZI$$Limit|

	ALIGN
	
; void EnterPWDN( int CLKCON )
EnterPWDN
	bl	LedTest
	mov		r2, r0			; r2 = CLKCON, CLKCON[3]若为1，表示转为了掉电模式
	tst		r0, #0x8
	bne		ENTER_SLEEP
	
ENTER_STOP					; 进入PWDN后如果不是SLEEP模式则进入IDLE模式
	ldr		r0, =REFRESH
	ldr		r3, [r0]
	mov		r1, r3
	orr		r1, r1, #BIT_SELFREFRESH
	str		r1, [r0]
	
	mov		r1, #16
0
	subs	r1, r1, #1
	bne		%B0
	
	ldr		r0, =CLKCON		; 进入停止模式
	str		r2, [r0]		; 置第2位，进入IDLE模式
	
	; 等待一段时间
	mov		r1, #32
0
	subs	r1, r1, #1
	bne		%B0
	
	; 取消SDRAM自刷新模式
	ldr		r0, =REFRESH
	str		r3, [r0]
	
	MOV_PC_LR
	
ENTER_SLEEP
	ldr		r0, =REFRESH
	ldr		r1, [r0]
	orr		r1, r1, #BIT_SELFREFRESH
	str		r1, [r0]
	
	; 等待一段时间
	mov		r1, #16
0
	subs	r1, r1, #1
	bne		%B0
	
	; 在进入SLEEP模式之前配置必要的时钟
	ldr		r1, =MISCCR
	ldr		r0, [r1]
	orr		r0, r0, #(7 << 17)
	str		r0, [r1]
	
	ldr		r0, =CLKCON
	str		r2, [r0]
	
	b		.

; 从SLEPP模式下被唤醒
WAKEUP_SLEEP
	ldr		r1, =MISCCR
	ldr		r0, [r1]
	bic		r0, r0, #(7 << 17)
	str		r0, [r1]
	
	; 配置内存控制寄存器
	ldr		r0, =SMRDATA
	ldr		r1, =BWSCON
	add		r2, r0, #52
0
	ldr		r3, [r0], #4
	str		r3, [r1], #4
	cmp		r2, r0
	bne		%B0
	
	mov		r1, #256
0
	subs	r1, r1, #1
	bne		%B0	
	
	; GSTATUS3存放着想从SLEEP模式唤醒后的地址
	ldr		r1, =GSTATUS3
	ldr		r0, [r1]
	
	mov		pc, r0
	
	LTORG
	
	EXPORT	CLKDIV124
	EXPORT	CLKDIV144
	
CLKDIV124
	ldr		r0, =CLKDIVN
	ldr		r1, =0x3		;1 : 2 : 4
	str		r1, [r0]
;等待直到时钟稳定
	nop
	nop
	nop
	nop
	nop
	
	ldr		r0, =REFRESH
	ldr		r1, [r0]
	bic		r1, r1, #0xff
	bic		r1, r1, #(0x7 << 8)
	orr		r1, r1, #0x470
	str		r1, [r0]
	nop
	nop
	nop
	nop
	nop
	mov		pc, lr
	
CLKDIV144
	ldr		r0, =CLKDIVN
	ldr		r1, =0x4	;0x4 = 1 : 4 : 4
	str		r1, [r0]
;等待时钟变得稳定
	nop
	nop
	nop
	nop
	nop
	
	ldr		r0, =REFRESH
	ldr		r1, [r0]
	bic		r1, r1, #0xff
	bic		r1, r1, #(0x7 << 8)
	orr		r1, r1, #0x630
	str		r1, [r0]
	nop
	nop
	nop
	nop
	nop
	mov		pc, lr
	
	LTORG
	
	ALIGN		;使得下面的代码按一定规则对齐
	
	AREA	RamData, DATA, READWRITE
	
	^		ISR_STARTADDRESS
HandleReset		#	4
HandleUndef		#	4
HandleSWI		#	4
HandlePabort	#	4
HandleDabort	#	4
HandleReserved	#	4
HandleIRQ		#	4
HandleFIQ		#	4
HandleEINT0		#	4
HandleEINT1		#	4
HandleEINT2		#	4
HandleEINT3		#	4
HandleEINT4_7	#	4
HandleEINT8_23	#	4
HandleCAM		#	4
HandleBATFLT	#	4
HandleTICK		#	4
HandleWDT		#	4
HandleTIMER0	#	4
HandleTIMER1	#	4
HandleTIMER2	#	4
HandleTIMER3	#	4
HandleTIMER4	#	4
HandleUART2		#	4
HandleLCD		#	4
HandleDMA0		#	4
HandleDMA1		#	4
HandleDMA2		#	4
HandleDMA3		#	4
HandleMMC		#	4
HandleSPI0		#	4
HandleUART1		#	4
HandleNFCON		#	4
HandleUSBD		#	4
HandleUSBH		#	4
HandleIIC		#	4
HandleUART0		#	4
HandleSPI1		#	4
HandleRTC		#	4
HandleADC		#	4

	END
