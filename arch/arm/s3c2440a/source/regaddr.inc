;====================================================================================
;										LJ_RTOS
;								The Real-Time Kernel
; 
; (c) Copyright 2010, ������
; All Rights Reserved
;
; �ļ����ƣ�regaddr.inc
; �ļ���ʶ�������ļ�(�൱��c����ͷ�ļ�)
; ժ    Ҫ�����ļ���Ҫ��������s3c2440a�ļĴ�����ַ
;
; ��ǰ�汾��0.0
; ��    �ߣ�������
; ������ڣ�2010/11/8
;
; ȡ���汾��
; ��    �ߣ�
; ������ڣ�
;====================================================================================

; �ڴ���ƼĴ���
BWSCON		EQU		0x48000000		; ���߿���&�ȴ����ƼĴ���
BANKCON0	EQU		0x48000004		; ROM, ���װ�빤��
BANKCON1	EQU		0x48000008		; Bank1 ���ƼĴ���
BANKCON2	EQU		0x4800000c		; Bank2 ���ƼĴ���
BANKCON3	EQU		0x48000010		; Bank3 ���ƼĴ���
BANKCON4	EQU		0x48000014		; Bank4 ���ƼĴ���
BANKCON5	EQU		0x48000018		; Bank5 ���ƼĴ���
BANKCON6	EQU		0x4800001c		; Bank6 ���ƼĴ���
BANKCON7	EQU		0x48000020		; Bank7 ���ƼĴ���
REFRESH		EQU		0x48000024		; SDRAM ˢ�¿��ƼĴ���
BANKSIZE	EQU		0x48000028		; �洢���С���ƼĴ���
MRSRB6		EQU		0x4800002c		; ģʽ�Ĵ������Ĵ���BANK6
MRSRB7		EQU		0x48000030		; ģʽ�Ĵ������Ĵ���BANK7

; ʱ�Ӽ���Դ����
LOCKTIME	EQU		0x4c000000		; ����ʱ������Ĵ���
MPLLCON		EQU		0x4c000004		; MPLL���üĴ���
UPLLCON		EQU		0x4c000008		; UPLL���üĴ���
CLKCON		EQU		0x4c00000c		; ʱ�ӿ��ƼĴ���
CLKSLOW		EQU		0x4c000010		; ʱ�Ӽ������ƼĴ���
CLKDIVN		EQU		0x4c000014		; ʱ�ӷ�Ƶ�����ƼĴ���

; �жϿ�����
SRCPND		EQU		0x4a000000		; Դδ���Ĵ���
INTMOD		EQU		0x4a000004		; �ж�ģʽ�Ĵ���
INTMSK		EQU		0x4a000008		; �ж����μĴ���
PRIORITY	EQU		0x4a00000c		; ���ȼ��Ĵ���
INTPND		EQU		0x4a000010		; �ж�δ���Ĵ���
INTOFFSET	EQU		0x4a000014		; �ж�ƫ�ƼĴ���
SUBSRCPND	EQU		0x4a000018		; ���ж�Դδ���Ĵ���
SUBINTMSK	EQU		0x4a00001c		; ���ж����μĴ���

; ���Ź���ʱ��
WTCON		EQU		0x53000000		; ���Ź����ƼĴ���
WTDAT		EQU		0x53000004		; ���Ź����������Ĵ���
WTCNT		EQU		0x53000008		; ���Ź��������Ĵ���

; Nand Flash
NFCONF		EQU		0x4e000000		; Nand Flash ���üĴ���
NFCONT		EQU		0x4e000004		; Nand Flash ���ƼĴ���

; I/O Port
GPBCON		EQU		0x56000010
GPBDAT		EQU		0x56000014
GPBUP		EQU		0x56000018

; �˿������Ĵ���
MISCCR		EQU		0x56000080		; ����ƼĴ���
DCLKCON		EQU		0x56000084		; DCLK�ⲿԴʱ�ӼĴ���
EXTINT0		EQU		0x56000088		; �ⲿ�жϿ��ƼĴ���0
EXTINT1		EQU		0x5600008c		; �ⲿ�жϿ��ƼĴ���1
EXTINT2		EQU		0x56000090		; �ⲿ�жϿ��ƼĴ���2
EINTFLT0	EQU		0x56000094		; �ⲿ�жϹ��˼Ĵ���0(����)
EINTFLT1	EQU		0x56000098		; �ⲿ�жϹ��˼Ĵ���1(����)
EINTFLT2	EQU		0x5600009c		; �ⲿ�жϹ��˼Ĵ���2
EINTFLT3	EQU		0x560000a0		; �ⲿ�жϹ��˼Ĵ���3
EINTMASK	EQU		0x560000a4		; �ⲿ�ж����μĴ���
EINTPEND	EQU		0x560000a8		; �ⲿ�ж�δ���Ĵ���
GSTATUS0	EQU		0x560000ac		; ͨ��״̬�Ĵ���0
GSTATUS1	EQU		0x560000b0		; ͨ��״̬�Ĵ���1
GSTATUS2	EQU		0x560000b4		; ͨ��״̬�Ĵ���2
GSTATUS3	EQU		0x560000b8		; ͨ��״̬�Ĵ���3
GSTATUS4	EQU		0x560000bc		; ͨ��״̬�Ĵ���4
MSLCON		EQU		0x560000cc		; �ڴ�˯�߿��ƼĴ���

	END
