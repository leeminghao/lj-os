;===============================================================================================
;											LJ_RTOS
;									The Real-Time Kernel
;
; (c) Copyright 2010, ������
; All Rights Reserved
;
; �ļ����ƣ�boot.s
; �ļ���ʶ�����Դ�ļ�
; ժ    Ҫ���ϵ��ʹӱ��ļ���ʼ���У�ʵ�ֻ�����ʼ�������nor��nand�������жϣ���ɰѳ���
;           ���˵�SDRAM�У��ڰ�����ɺ���ת��Main����ִ�С���Ҫ��ɹ��ܰ�����
;          (1) �����쳣�жϵ��������
;          (2) �����ж�������
;          (3) ΪARMÿ������ģʽ���ö�ջ
;          (4) ��ʼ��MPLLʱ��
;          (5) ��ʼ��MMU(�ڴ����Ԫ)
;          (6) ��ʼ���洢��������
;          (7) �رտ��Ź����ж�
;          (8) �ж�IRQ�жϵ��ж����
;          (9) ��RW�ε����ݴ�flash������SDRAM����ʼ��ZI��Ϊ0
;          (10)��ת��Ӧ�ó���
;
; ��ǰ�汾��0.0
; ��    �ߣ�������
; ������ڣ�2010/11/8
;
; ȡ���汾��
; ��    �ߣ�
; ������ڣ�
;================================================================================================

		GET	clkcfg.inc		; ϵͳʱ�ӵ�����
		GET memcfg.inc		; �洢��������
		GET regaddr.inc		; �Ĵ�����ַ������
		
BIT_SELFREFRESH		EQU		(0x1 << 22)			; ������ˢ��λ

; Ԥ����7�й���ģʽ
USER_MODE	EQU		0x10	; �û�ģʽ
FIQ_MODE	EQU		0x11	; �����ж�ģʽ
IRQ_MODE	EQU		0x12	; �ⲿ�ж�ģʽ
SVC_MODE	EQU		0x13	; ��Ȩģʽ
ABORT_MODE	EQU		0x17	; �쳣�ж�ģʽ
UNDEF_MODE	EQU		0x1b	; δ����ģʽ
MODE_MASK	EQU		0x1f	; ϵͳģʽ(ģʽ����)

NOINT		EQU		0xc0	; ����IRQ/FIQ�ж�

; ����7�д�����ģʽ�Ķ�ջ��ʼ��ַ�������û�ģʽ��ϵͳģʽ����һ��ջ�ռ�
; STACK_BASEADDRESSS = 0x33ff8000
USER_STACK	EQU		(STACK_BASEADDRESS - 0x3800)	;0x33ff4800
SVC_STACK	EQU		(STACK_BASEADDRESS - 0x2800)	;0x33ff5800
UNDEF_STACK	EQU		(STACK_BASEADDRESS - 0x2400)	;0x33ff5c00
ABORT_STACK	EQU		(STACK_BASEADDRESS - 0x2000)	;0x33ff6000
IRQ_STACK	EQU		(STACK_BASEADDRESS - 0x1000)	;0x33ff7000
FIQ_STACK	EQU		(STACK_BASEADDRESS - 0x0)		;0x33ff8000

; �ж��ǲ���Thumbָ��
	GBLL	THUMBCODE
	[ {CONFIG} = 16		; CONFIGΪ����������ñ���
THUMBCODE	SETL	{TRUE}
		CODE32
	|
THUMBCODE	SETL	{FALSE}
	]
	
; �궨��MOV_PC_LR, ���ã������ӳ��򷵻�
		MACRO
	MOV_PC_LR
	[ THUMBCODE
	 bx lr
	|
	 mov pc, lr
	]
	MEND
		
; ���س��򣬰��жϷ��������׵�ַװ�ص�PC�У��ڳ����ĩβ������һ�������������
; ��Ӧ���жϷ����ӳ����ַ���������ж�ģʽ��ʹ��"���س���"��ִ���жϷ������
		MACRO
$HandlerLabel	HANDLER	$HandleLabel

$HandlerLabel
	sub		sp, sp, #4			; ���㷵�ص�ַ
	stmfd	sp!, {r0}			; ��r0�е�ֵѹ��ջ��
	ldr		r0, =$HandleLabel	; ��Handle***��ַ����r0
	ldr		r0, [r0]			; ��ȡ�жϷ�������ַ����r0
	str		r0, [sp, #4]		; ���жϷ�������ַѹ��ջ��
	ldmfd	sp!, {r0, pc}		; ��ת���жϷ�����򲢻ظ�r0��ԭֵ
	MEND
	
; ������������ɵı�����RO, RW, ZI�������ζ�������flash�У���RW, ZI�εĵ�ַ�϶�����
; ��������ʱ�ı������洢��λ�ã�������ǵĳ����ʼ��ʱӦ�ð�flash�е�RW, ZI�ο�����
; SDRAM��Ӧ��λ�ã���Щ������ͨ��ADS�Ĺ������������趨��RO Base��RW Base�趨�ģ���
; ���ɱ������ű������ӳ��������
	IMPORT		|Image$$RO$$Base|		; RO������ʱ��ʼ��ַ
	IMPORT		|Image$$RO$$Limit|		; RO������ʱ������ַ
	IMPORT		|Image$$RW$$Base|		; RW�ε�����ʱ��ʼ��ַ
	IMPORT		|Image$$ZI$$Base|		; ZI�ε�����ʱ��ʼ��ַ
	IMPORT		|Image$$ZI$$Limit|		; ZI�ε�����ʱ������ַ
	
	;IMPORT		MmuSetAsyncBusMode		; ���������첽����ģʽ�����ⲿ����
	;IMPORT		MmuSetFastBusMode		; �������ÿ�������ģʽ�����ⲿ����
	
	IMPORT		Main
	IMPORT		LJCpuIrqIsr				; �������ϵͳ�жϴ�����
	EXPORT		HandleEINT0
	EXPORT		EnterPWDN
	EXPORT		__ENTRY
	
	AREA	Init, CODE, READONLY
	
	ENTRY	; �������
	
__ENTRY		; ��Ҫ����MMU
ResetEntry	; ��λ�����

; ��0x0�����쳣�ж��Ǹ�λ�쳣�жϣ����ϵ��ִ�еĵ�һ��ָ�����ENDIAN_CHANGE����
; ����Ƿ�Ҫ��С��ģʽ�ı�Ϊ���ģʽ����Ϊ��������ʼģʽ��С��ģʽ�����Ҫ�ô��ģ
; ʽ����Ҫ���ȰѸñ�������ΪTRUE������ΪFALSE
; ����ENTRY_BUS_WIDTH�����������߿�ȣ���Ϊ16λ��8Ϊ�������ʾ32λ����ʱ��С��ģʽ
; �£����ݵĺ����ǲ�ͬ��
; ����Ҫ���ǵ���˺�С��ģʽ���Լ����߿�ȣ���˴˴����Ƹ��ӣ���ʵֻ��һ����תָ��
; ��Ϊ���ģʽʱ����ת��ChangeBigEndian��������������ת��ResetHandler������

	ASSERT	:DEF:ENDIAN_CHANGE
	
	[ ENDIAN_CHANGE
	
		ASSERT	:DEF:ENTRY_BUS_WIDTH
		
		[ ENTRY_BUS_WIDTH = 32
		; ��ת��ChangeBigEndian(ChangeBigEndian��0x24��)����˸���ָ��Ļ�����Ϊ��
		; 0xea000007�����Ը�����ڸô�(0x0)ֱͬ�ӷ���0xea000007����(����DCD 0xea000007)
		; ������ͬ
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
	b	HandlerUndef	; δ�����쳣�ж�
	b	HandlerSWI		; ����쳣�ж�
	b	HandlerPabort	; ָ��Ԥȡ�쳣�ж�
	b	HandlerDabort	; ���ݷ����쳣�ж�
	b	.				; ��������ת�������ַ������������ѭ��
	b	HandlerIRQ		; �ⲿ�ж�����
	b	HandlerFIQ		; �����ж�����
	; ����Ϊһ���ж�������
	
; ��ת��EnterPWDN�������Դ���������������ģʽ����c���Գ����б�����
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
	
	; �˺�ϵͳ�����Զ�ʶ�����ͬ���߿���µĴ��ģʽ������Ժ��������Ϊ����ָ����
	DCD	0xffffffff
	DCD	0xffffffff
	DCD	0xffffffff
	DCD	0xffffffff
	b 	ResetHandler
		
; ��ϵͳ�����쳣�жϺ��ɴ����0x0~0x1c�����ж�������ַ�е���תָ����ת���˴���Ӧ
; ��λ�ã��������ȶ���õĺ궨���ٴ���ת����Ӧ���жϷ��������
HandlerFIQ		HANDLER		HandleFIQ
HandlerIRQ		HANDLER		HandleIRQ
HandlerUndef	HANDLER		HandleUndef
HandlerSWI		HANDLER		HandleSWI
HandlerDabort	HANDLER		HandleDabort
HandlerPabort	HANDLER		HandlePabort

; ����������ڴ���������жϣ���������������жϵ��׷����������жϣ�Ȼ����ת����Ӧ
; ���жϷ�������У������˵�����ǻ���INTOFFSET�Ĵ�����Ӧ��λΪ1��Ȼ��ͨ�������
; ����Ӧ���ж���ڵ�ַ
IsrIRQ
	sub		sp, sp, #4			; ��ջ��Ԥ��4�ֽڿռ䣬�Ա㱣���ж���ڵ�ַ
	stmfd	sp!, {r8-r9}		; ����r8,r9��ֵ
	ldr		r9, =INTOFFSET		; ��INTOFFSET�Ĵ�����ַװ��r9��
	ldr		r9, [r9]			; ��ȡINTOFFSET�Ĵ���������
	ldr		r8, =HandleEINT0	; �õ��ж�������Ļ�ַ
	add		r8, r8, r9, lsl #2	; �õ��жϷ���������ڵ�ַ
	ldr		r8, [r8]
	str		r8, [sp, #8]		; ���жϷ��������ڵ�ַװ��ջ��
	
	ldmfd	sp!, {r8-r9, pc}	; �ָ�r8, r9�Ĵ���ֵ������ת����Ӧ�жϷ��������
	
	LTORG
	
; ϵͳ��λ����0x0������תָ���ת���ô�����ִ��ϵͳ��ʼ����������λ������ARM
; �������ϵ���һ����ִ�е��쳣����ʱϵͳ���ڹ���ģʽ
; ��λ����������������Ҫ����
; 1.�رտ��Ź���ʱ�������������ж�
; 2.����ϵͳʱ��
; 3.�����ڴ������
; 4.����ÿ�ִ�����ģʽ�µĶ�ջָ��
; 5.��ʼ������������
; 6.��ת������ϵͳ���
ResetHandler
	
	; �رտ��Ź�
	ldr		r0, =WTCON
	ldr		r1, =0x0
	str		r1, [r0]
	
	; ���������жϺ����ж�
	ldr		r0, =INTMSK
	ldr		r1, =0xffffffff
	str		r1, [r0]
	
	ldr		r0, =SUBINTMSK
	ldr		r1, =0x7fff
	str		r1, [r0]
	
; �����ú�PLLʱ��ϵͳ��Ϊ��ʱPLL��û�ȶ���������ʱ����PLL��ʱ�ӣ������ⲿ������
; ʱ�ӣ���PLL����������LOCKTIME����ΪPLL�Ѿ��ȶ��ˣ���ʹ��PLL��ϵͳ�ṩʱ��
; s3c2440aҪ��PLL����ʱ�����300us������(1 / 12M) * N > 300us, ����NΪU_LTIME��
; M_LTIME��������N > 12M * 300us = 3600����LOCKTIME=0x0fff0fff�㹻�ˣ���16λΪ
; MPLL����16λΪUPLL
	ldr		r0, =LOCKTIME
	ldr		r1, =0xffffff
	str		r1, [r0]

	[ PLL_ON_START
		ldr		r0, =CLKDIVN
		ldr		r1, =CLKDIV_VAL
		str		r1, [r0]
		
		[ CLKDIV_VAL > 1	; ����Ϊ�첽����ģʽ
		 mrc	p15, 0, r0, c1, c0, 0
		 orr	r0, r0, #0xc0000000
		 mcr	p15, 0, r0, c1, c0, 0
		|					; ����Ϊ��������ģʽ
		 mrc	p15, 0, r0, c1, c0, 0
		 bic	r0, r0, #0xc0000000
		 mcr	p15, 0, r0, c1, c0, 0	
		]
		
; ����ʱ�ӣ�s3c2440aҪ��������UPLL������MPLL��֮����7 NOP
		
		ldr		r0, =UPLLCON	; ����UPLLΪ48MHz, ����Ͳ���ʹ��USB�ӿ���
		ldr		r1, =((U_MDIV << 12) + (U_PDIV << 4) + U_SDIV)
		str		r1, [r0]
		
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		
		ldr		r0, =MPLLCON	; ����MPLL, FCLK = 304MHz
		ldr		r1, =((M_MDIV << 12) + (M_PDIV << 4) + M_SDIV)
		str		r1, [r0]
	]
	
; ��SLEEPģʽ�����ѣ�����RESET���ű������������ҲҪ��0x0����ʼִ�У�
; �ڴ˴�Ҫ�ж��Ƿ�����SLEEPģʽ��������ĸ�λ
	ldr		r1, =GSTATUS2
	ldr		r0, [r1]
	tst		r0, #0x2
	bne		WAKEUP_SLEEP
	
; ����һ�������Ѹ�λ�����ʼ���ַ��ţ����԰������浽GSTATUS3�У������õ�ַ���
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

	bl		InitStacks		; ��ʼ����ջ
	
; ��װ�ⲿ�жϴ������
	ldr		r0, =HandleIRQ
	ldr		r1, =LJCpuIrqIsr
	str		r1, [r0]

;	bl LedTest	
; �ж��Ǵ�Nor����Nand����
	ldr		r0, =BWSCON
	ldr		r0, [r0]
	ands	r0, r0, #6		; OM[1:0] != 0, ��Nor Flash����
	bne		CopyProcBeg		; ���ö�ȡNand Flash
	adr		r0, ResetEntry
	cmp		r0, #0			; �ٱȽ�����Ƿ�Ϊ0��ַ����������������˷�����
	bne		CopyProcBeg		; �÷����������Ҳ��Ҫ��Nand Flash�������磬JTAG����ʱʱֱ�����ص��ڴ�������
	
NandBootBeg					; ��δ�����ɴ�nand�����뵽SDRAM
	bl		ClearSdram
	mov		r5, #NFCONF
	ldr		r0, =(7 << 12) | (7 << 8) | (7 << 4)
	str		r0, [r5]
	ldr		r0, =(0<<13) | (0<<12) | (0<<10) | (0<<9) | (0<<8) | (1<<6) | (1<<5) | (1<<4) | (1<<1) | (1<<0)
	str		r0, [r5, #4]
	
	bl		ReadNandID			; ��ȡNand��ID�ţ����������r5��
	mov		r6, #0				; r6���ֵ0
	ldr		r0, =0xecda			; ������Nand ID��.
	cmp		r5, r0				; ������бȽ�
	beq		%F1					; ��ȵĻ���������һ����Ŵ�
	mov		r6, #1				; ����Ⱦ�������һ����Ŵ�
1
	bl		ReadNandStatus		; ��ȡNand��״̬���������r1��
	
	mov		r8, #0				; r8���ֵ0������Ϊҳ��
; ע�⣬������ʹ�õĵ���ldrαָ������������õ�adrαָ��,�����ص���ResetEntry
; �ľ��Ե�ַ��Ҳ��������������Ram�еĵ�ַ, ����������|Image$$RO$$Base|һ��
	ldr		r9, =ResetEntry
2
	ands	r0, r8, #0x3f		; ��r8Ϊ0x3f(64)����������eq��Ч��ne��Ч;��ÿ����(64ҳ)���м��
	bne		%F3
	mov		r0, r8				; r8->r0
	bl		CheckBadBlk			; ���Nand�Ļ���
	cmp		r0, #0				; �Ƚ�r0��0
	addne	r8, r8, #64			; ���ڻ���Ļ��������������
	bne		%F4
3
	mov		r0, r8
	mov		r1, r9
	bl		ReadNandPage
	add		r9, r9, #2048		; ÿҳ���ֽ���
	add		r8, r8, #1
4
	cmp		r8, #131072			; 128KB
	bcc		%B2
	
	mov		r5, #NFCONF
	ldr		r0, [r5, #4]
	bic		r0, r0, #1
	str		r0, [r5, #4]
	
	ldr		pc, =CopyProcBeg	; �˴���ת���ڴ�ռ�
; ���Ǵ�Nand�������򿽱������Ѿ���NandBootBeg����ɣ�����ֱ����ת��Main
; ���Ǵ�Nor��������RO��RW���ֿ������ڴ棬Ȼ����ת���ڴ�����(Ҳ����NOR�����У�ֻ���ٶ�����)
; ע������Nor��ֱ�����У����RO/BASE��Ϊ0������RW/BASE,������RO����
CopyProcBeg
	adr		r0, ResetEntry		; ��Ե�ַװ�أ�����Ե�ַ�޹أ���ָ��Flash��RO���Ŀ�ʼ��ַ
	ldr		r2, BaseOfRom		; ���Ե�ַװ�أ���RO��������RAM��Ŀ�ʼ��ַ
	cmp		r0, r2				; ����ǵ�һ������R0 != r2
	ldreq	r0, TopOfRom		; ��Flash������ʱ����Ҫ���д����Ǩ
	beq		InitRam
	
; �����������Դ�����Nor Flashʱ�Ŀ�������������Ϊ��
; �Ѵ�ResetEntry��TopOfRom - BaseOfRom��С�����ݿ�����BaseOfRom
	ldr		r3, TopOfRom
0
	ldmia	r0!, {r4-r7}		; ��������Flash�е�RO��ȫ�����Ƶ�RAM��
	stmia	r2!, {r4-r7}
	cmp		r2, r3
	bcc		%B0
	
	sub		r2, r2, r3			; r2 = BaseOfROM - TopOfRom = -���볤��
	sub		r0, r0, r2			; r0 = ResetEntry - (-)���볤�� = ResetEntry + ���볤��
	
InitRam
	ldr		r2, BaseOfBss		; װ����ֵRWBase, ָ��RAM��RW���׵�ַ
	ldr		r3, BaseOfZero		; װ����ֵZIBase, ָ��RAM��ZI���׵�ַ��Ҳ��RW��β��ַ
0
	cmp		r2, r3				; ������ʱӦ��RW, ZI�ض�λ��SDRAM��
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
	 b	.		; ��ת��Main���ɹ������
	]
	
	[ THUMBCODE
	 orr	lr, pc, #1
	 bx		lr
	 CODE	16
	 bl		Main
	 b		.
	]
	
ClearSdram			; ���Sdram
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
	
InitStacks					; ��ʼ����ջ
; stmfd, ldmfd�����ڽ�����ջ�����ʹ��
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

; ��Ϊϵͳģʽ���û�ģʽ����һ��ջ�ռ䣬��˲������ظ������û�ģʽ��ջ��ϵͳ��λ������
; ��SVCģʽ�����Ҹ���ģʽ�µ�lr��ͬ�������Ҫ�Ӹú������أ���Ҫ�����л���SVCģʽ����ʹ��
; lr, �����Ϳ�����ȷ������
	mov		pc, lr

; Nand Flash��������
ReadNandID
	mov		r7, #NFCONF
	ldr		r0, [r7, #4]
	bic		r0, r0, #0x2
	str		r0, [r7, #4]
	
	mov		r0, #0x90		; 0x90��ȡNand ID����
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
	
	mov		r0, #0x70		; 0x70��ȡNand Status
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
	mov		r2, r0			; r2 = CLKCON, CLKCON[3]��Ϊ1����ʾתΪ�˵���ģʽ
	tst		r0, #0x8
	bne		ENTER_SLEEP
	
ENTER_STOP					; ����PWDN���������SLEEPģʽ�����IDLEģʽ
	ldr		r0, =REFRESH
	ldr		r3, [r0]
	mov		r1, r3
	orr		r1, r1, #BIT_SELFREFRESH
	str		r1, [r0]
	
	mov		r1, #16
0
	subs	r1, r1, #1
	bne		%B0
	
	ldr		r0, =CLKCON		; ����ֹͣģʽ
	str		r2, [r0]		; �õ�2λ������IDLEģʽ
	
	; �ȴ�һ��ʱ��
	mov		r1, #32
0
	subs	r1, r1, #1
	bne		%B0
	
	; ȡ��SDRAM��ˢ��ģʽ
	ldr		r0, =REFRESH
	str		r3, [r0]
	
	MOV_PC_LR
	
ENTER_SLEEP
	ldr		r0, =REFRESH
	ldr		r1, [r0]
	orr		r1, r1, #BIT_SELFREFRESH
	str		r1, [r0]
	
	; �ȴ�һ��ʱ��
	mov		r1, #16
0
	subs	r1, r1, #1
	bne		%B0
	
	; �ڽ���SLEEPģʽ֮ǰ���ñ�Ҫ��ʱ��
	ldr		r1, =MISCCR
	ldr		r0, [r1]
	orr		r0, r0, #(7 << 17)
	str		r0, [r1]
	
	ldr		r0, =CLKCON
	str		r2, [r0]
	
	b		.

; ��SLEPPģʽ�±�����
WAKEUP_SLEEP
	ldr		r1, =MISCCR
	ldr		r0, [r1]
	bic		r0, r0, #(7 << 17)
	str		r0, [r1]
	
	; �����ڴ���ƼĴ���
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
	
	; GSTATUS3��������SLEEPģʽ���Ѻ�ĵ�ַ
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
;�ȴ�ֱ��ʱ���ȶ�
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
;�ȴ�ʱ�ӱ���ȶ�
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
	
	ALIGN		;ʹ������Ĵ��밴һ���������
	
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
