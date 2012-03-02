;=============================================================================================
;										LJ_RTOS
;								The Real-Time Kernel
;
; (c) Copyright 2010, ������
; All Rights Reserved
;
; �ļ����ƣ�memcfg.inc
; �ļ���ʶ�������ļ�(ͬc�����е�ͷ�ļ�)
; ժ    Ҫ�����ļ�����s3c2440a���ڴ������
;
; ��ǰ�汾��0.0
; ��    �ߣ�������
; ������ڣ�2010/11/8
;
; ȡ���汾��
; ��    �ߣ�
; ������ڣ�
;=============================================================================================

; BWSCON--���߿��Ⱥ�WAIT���ƼĴ���
DW8		EQU		(0x0)
DW16	EQU		(0x1)
DW32	EQU		(0x2)
WAIT	EQU		(0x1 << 2)
UBLB	EQU		(0x1 << 3)

; ���ݲ�ͬ�����߿��Ƚ��в�ͬ����, δ����B0_BWSCON, ����ΪBANK0��Ϊ����ROM, �ڷ���
; ǰ����ȷ�����߿��ȣ���OM[0:1]����
	ASSERT	:DEF:BUSWIDTH
	[ BUSWIDTH = 16
	
B1_BWSCON	EQU	(DW16)
B2_BWSCON	EQU	(DW16)
B3_BWSCON	EQU	(DW16)
B4_BWSCON	EQU	(DW16)
B5_BWSCON	EQU	(DW16)
B6_BWSCON	EQU	(DW16)
B7_BWSCON	EQU	(DW16)

	|

B1_BWSCON	EQU	(DW32)	
B2_BWSCON	EQU	(DW16)
B3_BWSCON	EQU	(0xd)
B4_BWSCON	EQU	(DW16)
B5_BWSCON	EQU	(DW16)
B6_BWSCON	EQU	(DW32)
B7_BWSCON	EQU	(DW32)
	]

; TACS ��ַ��Ч�󣬵ȴ�CS
; TCOS CS��Ч�󣬵ȴ�OE
; TACC ����ʱ��
; TCOH OE��Ч�󣬱���CS
; TAH CS��Ч�󣬱��ֵ�ַ
; TACP Pageģʽ�µķ���ʱ��
; TPMC Pageģʽ����
; MT �ڴ�����
; TRCD RAS��CAS����ʱ(��ַ����ʱ��)
; SCAN �е�ַλ��
; TRP RASԤ���ʱ��
; TSRC ��������ʱ��
; TRC = TSRC + TRP ������ʱ��

; BANKCON0
B0_TACS		EQU		0x0		;0clk
B0_TCOS		EQU		0x0		;0clk
B0_TACC		EQU		0x7		;14clk
B0_TCOH		EQU		0x0		;0clk
B0_TAH		EQU		0x0		;0clk
B0_TACP		EQU		0x0		;0clk
B0_PMC		EQU		0x0		;����

; BANKCON1
B1_TACS		EQU		0x0		;0clk
B1_TCOS		EQU		0x0		;0clk
B1_TACC		EQU		0x7		;14clk
B1_TCOH		EQU		0x0		;0clk
B1_TAH		EQU		0x0		;0clk
B1_TACP		EQU		0x0		;0clk
B1_PMC		EQU		0x0		;����

; BANKCON2
B2_TACS		EQU		0x0		;0clk
B2_TCOS		EQU		0x0		;0clk
B2_TACC		EQU		0x7		;14clk
B2_TCOH		EQU		0x0		;0clk
B2_TAH		EQU		0x0		;0clk
B2_TACP		EQU		0x0		;0clk
B2_PMC		EQU		0x0		;����

; BANKCON3
B3_TACS		EQU		0x1		;1clk
B3_TCOS		EQU		0x3		;4clk
B3_TACC		EQU		0x7		;14clk
B3_TCOH		EQU		0x2		;2clk
B3_TAH		EQU		0x1		;1clk
B3_TACP		EQU		0x3		;6clk
B3_PMC		EQU		0x0		;����

; BANKCON4
B4_TACS		EQU		0x0		;0clk
B4_TCOS		EQU		0x0		;0clk
B4_TACC		EQU		0x7		;14clk
B4_TCOH		EQU		0x0		;0clk
B4_TAH		EQU		0x0		;0clk
B4_TACP		EQU		0x0		;0clk
B4_PMC		EQU		0x0		;����

; BANKCON5
B5_TACS		EQU		0x0		;0clk
B5_TCOS		EQU		0x0		;0clk
B5_TACC		EQU		0x7		;14clk
B5_TCOH		EQU		0x0		;0clk
B5_TAH		EQU		0x0		;0clk
B5_TACP		EQU		0x0		;0clk
B5_PMC		EQU		0x0		;����

; BANKCON6
B6_MT		EQU		0x3		;SDRAM
B6_TRCD		EQU		0x2		;4clk
B6_SCAN		EQU		0x1		;9bit

; BANKCON7
B7_MT		EQU		0x3		;SDRAM
B7_TRCD		EQU		0x2		;4clk
B7_SCAN		EQU		0x1		;9bit

; REFRESH
REFRESH_EN		EQU		0x0		;ˢ��ʹ��
REFRESH_MODE	EQU		0x0		;ȷ��ˢ��ģʽ/�Զ�ˢ��
TRP				EQU		0x2		;4clk
TRC				EQU		0x2		;6clk

TCHR			EQU		0x2		;3clk
REFRESH_CNT		EQU		1296	;ˢ�¼���ֵ = 2048 + 1 - HCLK * ˢ������

	END


