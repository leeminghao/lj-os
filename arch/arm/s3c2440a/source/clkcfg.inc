;===================================================================================
; 											  LJ_RTOS
;										The Real-Time Kernel
;
; (c) Copyright 2010, ������
; All Rights Reserved
;
; �ļ����ƣ�clkcfg.inc
; �ļ���ʶ�������ļ�(ͬc�����е�ͷ�ļ�)
; ժ    Ҫ�����ļ�����s3c2440aʱ������
;
; ��ǰ�汾��0.0
; ��    �ߣ�������
; ������ڣ�2010/11/8
;
; ȡ���汾��
; ��    �ߣ�
; ������ڣ�
;====================================================================================

; ������ʼ��ַ����
STACK_BASEADDRESS		EQU		0x33ff8000			; ��ջ��ʼ��ַ����
MMUTT_STARTADDRESS		EQU		0x33ff8000			; ����ַ-��ַ�Ĵ����ĳ�ʼֵ
ISR_STARTADDRESS		EQU		0x33ffff00			; �ж���ʼ��ַ

	GBLL	USE_MAIN			; �����ж��Ƿ�ʹ��main����
USE_MAIN		SETL	{FALSE}

	GBLL	PLL_ON_START		; �������໷ʹ���ж�
PLL_ON_START	SETL	{TRUE}

	GBLL	ENDIAN_CHANGE
ENDIAN_CHANGE	SETL	{FALSE}	; �����ж��Ƿ�Ϊ���ģʽ

	GBLA	ENTRY_BUS_WIDTH		; �����ж�������߿���
ENTRY_BUS_WIDTH SETA	16

	GBLA	BUSWIDTH			; �����ж����߿���
BUSWIDTH		SETA	32

	GBLA	UCLK
UCLK		SETA		48000000	; ����USB��ʱ��Ƶ��

	GBLA	XTAL_SEL				; ����ѡ����Ƶ��
	GBLA	FCLK					; �ں�ʱ��Ƶ��

; ѡ����Ƶ��
XTAL_SEL	SETA		12000000
;XTAL_SEL	SETA		16934400

; ѡ���ں�ʱ��Ƶ��
FCLK		SETA		304000000

; ѡ��ʱ�ӷ�Ƶ��(FCLK : HCLK : PCLK)
CLKDIV_VAL	EQU			7			; 1 : 3 : 6

; ����ʱ�����ã�����оƬ�ֲ��������
 [ XTAL_SEL = 12000000
	
	[ FCLK = 271500000
M_MDIV	EQU	173
M_PDIV	EQU	2
M_SDIV	EQU	2
	]
	
	[ FCLK = 304000000
M_MDIV	EQU	68
M_PDIV	EQU	1
M_SDIV	EQU	1
	]
	
	[ UCLK = 48000000
U_MDIV	EQU	56
U_PDIV	EQU	2
U_SDIV	EQU	2
	]
	
	[ UCLK = 96000000
U_MDIV	EQU	56
U_PDIV	EQU	2
U_SDIV	EQU	1
	]
	
 |	; XTAL_SEL = 16.9344HZ
	
	[ FCLK = 296352000
M_MDIV	EQU	97
M_PDIV	EQU	1
M_SDIV	EQU	2
	]
	
	[ FCLK = 541900800
M_MDIV	EQU	120
M_PDIV	EQU	2
M_SDIV	EQU	1
	]
	
	[ UCLK = 48000000
M_MDIV	EQU	60
M_PDIV	EQU	4
M_SDIV	EQU	2
	]
	
	[ UCLK = 96000000
M_MDIV	EQU	60
M_PDIV	EQU	4
M_SDIV	EQU	1
	]
 ]

	END