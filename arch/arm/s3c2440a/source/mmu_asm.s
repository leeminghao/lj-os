;=============================================================================================
;                                          LJ_RTOS
;                                    The Real-Time Kernel
;
; (c) Copyright 2010, ������
; All Rights Reserved
; 
; �ļ����ƣ�mmu_asm.s
; �ļ���ʶ�����Դ�ļ�
; ժ    Ҫ�����ļ�����ʵ��MMU����ĺ������û������ʵ��
;
; ��ǰ�汾��0.0
; ��    �ߣ�������
; ������ڣ�2010/11/15
;
; ȡ���汾��
; ��    �ߣ�
; ������ڣ�
;============================================================================================

NOINT	EQU		0xc0				; �ж�λ

	GBLL	THUMBCODE
	[ {CONFIG} = 16
THUMBCODE SETL	{TRUE}
	  CODE32
	|
THUMBCODE SETL	{FALSE}
	]
	
	MACRO
	MOV_PC_LR
	[ THUMBCODE
	 bx lr
	|
	 mov pc, lr
	]
	MEND
	
	AREA	|C$$code|, CODE, READONLY
	
; CPSR I, F λ
; ע������3���������������ڽ��̴�����Ȩģʽ��

; U32 SetIF( void );
	EXPORT	SetIF
SetIF
	mrs		r0, cpsr
	mov		r1, r0
	orr		r1, r1, #NOINT
	msr		cpsr_cxsf, r1
	MOV_PC_LR
	
; void WrIF( int cpsrValue );
	EXPORT	WrIF
WrIF
	msr		cpsr_cxsf, r0
	MOV_PC_LR
	
; void ClrIF( void )
	EXPORT	ClrIF
ClrIF
	mrs		r0, cpsr
	bic		r0, r0, #NOINT
	msr		cpsr_cxsf, r0
	MOV_PC_LR
	
;===================================================================================
; MMU Cache/TLB/etc on/off functions
;===================================================================================
R1_I		EQU	(1 << 12)
R1_C		EQU	(1 << 2)
R1_A		EQU	(1 << 1)
R1_M		EQU	(1)
R1_iA		EQU	(1 << 31)
R1_nF		EQU	(1 << 30)

; void MMUEnableICache( void )
	EXPORT	MMUEnableICache
MMUEnableICache
	mrc		p15, 0, r0, c1, c0, 0
	orr		r0, r0, #R1_I
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUDisableICache( void )
	EXPORT	MMUDisableICache
MMUDisableICache
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, r0, #R1_I
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUEnableDCache( void )
	EXPORT	MMUEnableDCache
MMUEnableDCache
	mrc		p15, 0, r0, c1, c0, 0
	orr		r0, r0, #R1_C
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUDisableDCache( void )
	EXPORT	MMUDisableDCache
MMUDisableDCache
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, r0, #R1_C
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUEnableAlignFault( void )
	EXPORT	MMUEnableAlignFault
MMUEnableAlignFault
	mrc		p15, 0, r0, c1, c0, 0
	orr		r0, r0, #R1_A
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUDisableAlignFault
	EXPORT	MMUDisableAlignFault
MMUDisableAlignFault
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, r0, #R1_A
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUEnableMMU( void )
	EXPORT	MMUEnableMMU
MMUEnableMMU
	mrc		p15, 0, r0, c1, c0, 0
	orr		r0, r0, #R1_M
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUDisableMMU( void )
	EXPORT	MMUDisableMMU
MMUDisableMMU
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, r0, #R1_M
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUSetFastBusMode
; FCLK : HCLK = 1 : 1
	EXPORT	MMUSetFastBusMode
MMUSetFastBusMode
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, r0, #R1_iA
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; void MMUSetAsyncBusMode
; FCLK : HCLK = 1 : 2
	EXPORT	MMUSetAsyncBusMode
MMUSetAsyncBusMode
	mrc		p15, 0, r0, c1, c0, 0
	orr		r0, r0, #R1_nF
	mcr		p15, 0, r0, c1, c0, 0
	MOV_PC_LR
	
; Set TTBase
; void MMUSetTTBase( U32 base )				; �����ڴ���ҳ�����ַ
	EXPORT	MMUSetTTBase
MMUSetTTBase
	mcr		p15, 0, r0, c2, c0, 0
	MOV_PC_LR
	
; void MMUSetDomain( U32 domain )
	EXPORT	MMUSetDomain
MMUSetDomain
	mcr		p15, 0, r0, c3, c0, 0
	MOV_PC_LR
	
; void MMUInvalidateIDCache( void )			; ʹ��Ч����ͳһCache��ʹ��Ч��������Cache��ָ��Cache
	EXPORT	MMUInvalidateIDCache
MMUInvalidateIDCache
	mcr		p15, 0, r0, c7, c7, 0
	MOV_PC_LR
	
; void MMUInvalidateICache( void )			; ʹ��Ч����ָ��Cache
	EXPORT	MMUInvalidateICache
MMUInvalidateICache
	mcr		p15, 0, r0, c7, c5, 0
	MOV_PC_LR
	
; void MMUInvalidateICacheMVA( U32 mva )	; ʹ��Чָ��Cache�е�ĳ�飺mva (�����ַ)
	EXPORT	MMUInvalidateICacheMVA
MMUInvalidateICacheMVA
	mcr		p15, 0, r0, c7, c5, 1
	MOV_PC_LR
	
; void MMUPrefetchICacheMVA( U32 mva )		; Ԥȡָ��Cache��ĳ�飺mva (�����ַ)
	EXPORT	MMUPrefetchICacheMVA
MMUPrefetchICacheMVA
	mcr		p15, 0, r0, c7, c13, 1
	MOV_PC_LR
	
; void MMUInvalidateDCache( void )			; ʹ��Ч��������Cache
	EXPORT	MMUInvalidateDCache
MMUInvalidateDCache
	mcr		p15, 0, r0, c7, c6, 0
	MOV_PC_LR
	
; void MMUInvalidateDCacheMVA( U32 mva )	; ʹ��Ч����Cache�е�ĳ�飺mva (�����ַ)
	EXPORT	MMUInvalidateDCacheMVA
MMUInvalidateDCacheMVA
	mcr		p15, 0, r0, c7, c6, 1
	MOV_PC_LR
	

;void MMUCleanDCacheMVA(U32 mva)			; �������Cache��ĳ��
   EXPORT MMUCleanDCacheMVA
MMUCleanDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c10,1
   MOV_PC_LR

;void MMUCleanDCacheIndex(U32 index)		; �������Cache��ĳ�飺���/�������
   EXPORT MMUCleanDCacheIndex
MMUCleanDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c10,2
   MOV_PC_LR

;void MMUCleanInvalidateDCacheMVA(U32 mva)	; ��ղ�ʹ��Ч����Cache��ĳ��
   EXPORT MMUCleanInvalidateDCacheMVA
MMUCleanInvalidateDCacheMVA
   ;r0=mva
   mcr p15,0,r0,c7,c14,1
   MOV_PC_LR

;void MMUCleanInvalidateDCacheIndex(U32 index)	; ��ղ�ʹ��Ч����Cache��ĳ��
   EXPORT MMUCleanInvalidateDCacheIndex
MMUCleanInvalidateDCacheIndex
   ;r0=index
   mcr p15,0,r0,c7,c14,2
   MOV_PC_LR

;void MMUWaitForInterrupt(void)					; �ȴ��жϼ���
   EXPORT MMUWaitForInterrupt
MMUWaitForInterrupt
   mcr p15,0,r0,c7,c0,4
   MOV_PC_LR

;===============
; TLB functions
;===============
;voic MMUInvalidateTLB(void)				; ʹ��Ч����ͳһTLB����ʹ��Ч��������TLB��ָ��TLB
   EXPORT MMUInvalidateTLB
MMUInvalidateTLB
   mcr p15,0,r0,c8,c7,0
   MOV_PC_LR

;void MMUInvalidateITLB(void)				; ʹ��Ч����ָ��TLB			
   EXPORT MMUInvalidateITLB
MMUInvalidateITLB
   mcr p15,0,r0,c8,c5,0
   MOV_PC_LR

;void MMUInvalidateITLBMVA(U32 mva)			; ʹ��Чָ��TLB�еĵ�����ַ�任��Ŀ��mva (�����ַ)
   EXPORT MMUInvalidateITLBMVA
MMUInvalidateITLBMVA
   ;ro=mva
   mcr p15,0,r0,c8,c5,1
   MOV_PC_LR

;void MMUInvalidateDTLB(void)				; ʹ��Ч��������TLB
	EXPORT MMUInvalidateDTLB
MMUInvalidateDTLB
	mcr p15,0,r0,c8,c6,0
	MOV_PC_LR

;void MMUInvalidateDTLBMVA(U32 mva)			; ʹ��Ч����TLB�еĵ�����ַ�任��Ŀ
	EXPORT MMUInvalidateDTLBMVA
MMUInvalidateDTLBMVA
	;r0=mva
	mcr p15,0,r0,c8,c6,1
	MOV_PC_LR

;=================
; Cache lock down
;=================
;void MMUSetDCacheLockdownBase(U32 base)
   EXPORT MMUSetDCacheLockdownBase
MMUSetDCacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,0
   MOV_PC_LR

;void MMUSetICacheLockdownBase(U32 base)
   EXPORT MMUSetICacheLockdownBase
MMUSetICacheLockdownBase
   ;r0= victim & lockdown base
   mcr p15,0,r0,c9,c0,1
   MOV_PC_LR

;=================
; TLB lock down
;=================
;void MMUSetDTLBLockdown(U32 baseVictim)
   EXPORT MMUSetDTLBLockdown
MMUSetDTLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,0
   MOV_PC_LR

;void MMUSetITLBLockdown(U32 baseVictim)
   EXPORT MMUSetITLBLockdown
MMUSetITLBLockdown
   ;r0= baseVictim
   mcr p15,0,r0,c10,c0,1
   MOV_PC_LR

;============
; Process ID
;============
;void MMUSetProcessId(U32 pid)
   EXPORT MMUSetProcessId
MMUSetProcessId
   ;r0= pid
   mcr p15,0,r0,c13,c0,0
   MOV_PC_LR

   END

	 