/*
 *==========================================================================================
 * 										LJ_RTOS
 *								  The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 * 
 * �ļ����ƣ�regaddr.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ�����s3c2440a�ļĴ�����ַ
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/10
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *==========================================================================================
 */
 
 #ifndef __REGADDR_H
 #define __REGADDR_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 
/*=======================================ʱ�Ӽ���Դ����===================================*/

 #define R_LOCKTIME		(*(volatile U32 *)0x4c000000)		/* ����ʱ������Ĵ���         */
 #define R_MPLLCON		(*(volatile U32 *)0x4c000004)		/* MPLL���üĴ���             */
 #define R_UPLLCON		(*(volatile U32 *)0x4c000008)		/* UPLL���üĴ���             */
 #define R_CLKCON		(*(volatile U32 *)0x4c00000c)		/* ʱ�ӿ��ƼĴ���             */
 #define R_CLKSLOW		(*(volatile U32 *)0x4c000010)		/* ʱ�Ӽ������ƼĴ���         */
 #define R_CLKDIVN		(*(volatile U32 *)0x4c000014)		/* ʱ�ӷ�Ƶ�����ƼĴ���       */
 #define R_CAMDIVN		(*(volatile U32 *)0x4c000018)		/* ����ͷʱ�ӷ�Ƶ���Ĵ���     */
 
/*=======================================�жϿ��ƼĴ���===================================*/

 #define R_SRCPND		(*(volatile U32 *)0x4a000000)		/* Դδ���Ĵ���               */
 #define R_INTMOD		(*(volatile U32 *)0x4a000004)		/* �ж�ģʽ�Ĵ���             */
 #define R_INTMSK		(*(volatile U32 *)0x4a000008)		/* �ж����μĴ���             */
 #define R_PRIORITY		(*(volatile U32 *)0x4a00000c)		/* ���ȼ��Ĵ���               */
 #define R_INTPND		(*(volatile U32 *)0x4a000010)		/* �ж�δ���Ĵ���             */
 #define R_INTOFFSET	(*(volatile U32 *)0x4a000014)		/* �ж�ƫ�ƼĴ���             */
 #define R_SUBSRCPND	(*(volatile U32 *)0x4a000018)		/* ���ж�Դδ���Ĵ���         */
 #define R_SUBINTMSK	(*(volatile U32 *)0x4a00001c)		/* ���ж����μĴ���           */
 
/*=========================================���Ź���ʱ��===================================*/

 #define R_WTCON		(*(volatile U32 *)0x53000000)		/* ���Ź����ƼĴ���           */
 #define R_WTDAT		(*(volatile U32 *)0x53000004)		/* ���Ź����������Ĵ���       */
 #define R_WTCNT		(*(volatile U32 *)0x53000008)		/* ���Ź������Ĵ���           */
 
/*===========================================Timer========================================*/

 #define R_TCFG0		(*(volatile U32 *)0x51000000)		/* ��ʱ�����üĴ���0          */
 #define R_TCFG1		(*(volatile U32 *)0x51000004)		/* ��ʱ�����üĴ���1          */
 #define R_TCON			(*(volatile U32 *)0x51000008)		/* ��ʱ�����ƼĴ���           */
 #define R_TCNTB0		(*(volatile U32 *)0x5100000c)		/* ��ʱ��0��������Ĵ���      */
 #define R_TCMPB0		(*(volatile U32 *)0x51000010)		/* ��ʱ��0�Ƚϻ���Ĵ���      */
 #define R_TCNTO0		(*(volatile U32 *)0x51000014)		/* ��ʱ��0�����۲�Ĵ���      */
 #define R_TCNTB1		(*(volatile U32 *)0x51000018)		/* ��ʱ��1��������Ĵ���      */
 #define R_TCMPB1		(*(volatile U32 *)0x5100001c)		/* ��ʱ��1�Ƚϻ���Ĵ���      */
 #define R_TCNTO1		(*(volatile U32 *)0x51000020)		/* ��ʱ��1�����۲�Ĵ���      */
 #define R_TCNTB2		(*(volatile U32 *)0x51000024)		/* ��ʱ��2��������Ĵ���      */
 #define R_TCMPB2		(*(volatile U32 *)0x51000028)		/* ��ʱ��2�Ƚϻ���Ĵ���      */
 #define R_TCNTO2		(*(volatile U32 *)0x5100002c)		/* ��ʱ��2�����۲�Ĵ���      */
 #define R_TCNTB3		(*(volatile U32 *)0x51000030)		/* ��ʱ��3��������Ĵ���      */
 #define R_TCMPB3		(*(volatile U32 *)0x51000034)		/* ��ʱ��3�Ƚϻ���Ĵ���      */
 #define R_TCNTO3		(*(volatile U32 *)0x51000038)		/* ��ʱ��3�����۲�Ĵ���      */
 #define R_TCNTB4		(*(volatile U32 *)0x5100003c)		/* ��ʱ��4��������Ĵ���      */
 #define R_TCNTO4		(*(volatile U32 *)0x51000040)		/* ��ʱ��4�����۲�Ĵ���      */
 
/*===========================================RTC==========================================*/
 
 #ifdef BIG_ENDIAN
 #define R_RTCCON		(*(volatile U8 *)0x57000043)		/* RTC���ƼĴ���              */
 #define R_TICNT		(*(volatile U8 *)0x57000047)		/* ʱ��Ƭ������               */
 #define R_RTCALM		(*(volatile U8 *)0x57000053)		/* �������ƼĴ���             */
 #define R_ALMSEC		(*(volatile U8 *)0x57000057)		/* ���������ݼĴ���           */
 #define R_ALMMIN		(*(volatile U8 *)0x5700005b)		/* ���������ݼĴ���           */
 #define R_ALMHOUR		(*(volatile U8 *)0x5700005f)		/* ����ʱ���ݼĴ���           */
 #define R_ALMDATE		(*(volatile U8 *)0x57000063)		/* ���������ݼĴ���           */
 #define R_ALMMON		(*(volatile U8 *)0x57000067)		/* ���������ݼĴ���           */
 #define R_ALMYEAR		(*(volatile U8 *)0x5700006b)		/* ���������ݼĴ���           */
 #define R_RTCRST		(*(volatile U8 *)0x5700006f)		/* ѭ����λ�Ĵ���             */
 #define R_BCDSEC		(*(volatile U8 *)0x57000073)		/* �����ݼĴ���               */
 #define R_BCDMIN		(*(volatile U8 *)0x57000077)		/* �����ݼĴ���               */
 #define R_BCDHOUR		(*(volatile U8 *)0x5700007b)		/* ʱ���ݼĴ���               */
 #define R_BCDDATE		(*(volatile U8 *)0x5700007f)		/* �����ݼĴ���               */
 #define R_BCDDAY		(*(volatile U8 *)0x57000083)		/* �������ݼĴ���             */
 #define R_BCDMON		(*(volatile U8 *)0x57000087)		/* �����ݼĴ���               */
 #define R_BCDYEAR		(*(volatile U8 *)0x5700008b)		/* �����ݼĴ���               */
 #else
 #define R_RTCCON		(*(volatile U8 *)0x57000040)		/* RTC���ƼĴ���              */
 #define R_TICNT		(*(volatile U8 *)0x57000044)		/* ʱ��Ƭ������               */
 #define R_RTCALM		(*(volatile U8 *)0x57000050)		/* �������ƼĴ���             */
 #define R_ALMSEC		(*(volatile U8 *)0x57000054)		/* ���������ݼĴ���           */
 #define R_ALMMIN		(*(volatile U8 *)0x57000058)		/* ���������ݼĴ���           */
 #define R_ALMHOUR		(*(volatile U8 *)0x5700005c)		/* ����ʱ���ݼĴ���           */
 #define R_ALMDATE		(*(volatile U8 *)0x57000060)		/* ���������ݼĴ���           */
 #define R_ALMMON		(*(volatile U8 *)0x57000064)		/* ���������ݼĴ���           */
 #define R_ALMYEAR		(*(volatile U8 *)0x57000068)		/* ���������ݼĴ���           */
 #define R_RTCRST		(*(volatile U8 *)0x5700006c)		/* ѭ����λ�Ĵ���             */
 #define R_BCDSEC		(*(volatile U8 *)0x57000070)		/* �����ݼĴ���               */
 #define R_BCDMIN		(*(volatile U8 *)0x57000074)		/* �����ݼĴ���               */
 #define R_BCDHOUR		(*(volatile U8 *)0x57000078)		/* ʱ���ݼĴ���               */
 #define R_BCDDATE		(*(volatile U8 *)0x5700007c)		/* �����ݼĴ���               */
 #define R_BCDDAY		(*(volatile U8 *)0x57000080)		/* �������ݼĴ���             */
 #define R_BCDMON		(*(volatile U8 *)0x57000084)		/* �����ݼĴ���               */
 #define R_BCDYEAR		(*(volatile U8 *)0x57000088)		/* �����ݼĴ���               */
 #endif
 
/*===========================================UART=========================================*/
 
 #define R_ULCON0		(*(volatile U32 *)0x50000000)		/* UART0��·���ƼĴ���        */
 #define R_UCON0		(*(volatile U32 *)0x50000004)		/* UART0���ƼĴ���            */
 #define R_UFCON0		(*(volatile U32 *)0x50000008)		/* UART0 FIFO ���ƼĴ���      */
 #define R_UMCON0		(*(volatile U32 *)0x5000000c)		/* UART0 MODEM ���ƼĴ���     */
 #define R_UTRSTAT0		(*(volatile U32 *)0x50000010)		/* UART0 ����/����״̬�Ĵ���  */
 #define R_UERSTAT0		(*(volatile U32 *)0x50000014)		/* UART0 ����״̬�Ĵ���       */
 #define R_UFSTAT0		(*(volatile U32 *)0x50000018)		/* UART0 FIFO ״̬�Ĵ���      */
 #define R_UMSTAT0		(*(volatile U32 *)0x5000001c)		/* UART0 MODEM ״̬�Ĵ���     */
 #define R_UBRDIV0		(*(volatile U32 *)0x50000028)		/* UART0 �����ʳ����Ĵ���     */
 
 #define R_ULCON1		(*(volatile U32 *)0x50004000)		/* UART1��·���ƼĴ���        */
 #define R_UCON1		(*(volatile U32 *)0x50004004)		/* UART1���ƼĴ���            */
 #define R_UFCON1		(*(volatile U32 *)0x50004008)		/* UART1 FIFO ���ƼĴ���      */
 #define R_UMCON1		(*(volatile U32 *)0x5000400c)		/* UART1 MODEM ���ƼĴ���     */
 #define R_UTRSTAT1		(*(volatile U32 *)0x50004010)		/* UART1 ����/����״̬�Ĵ���  */
 #define R_UERSTAT1		(*(volatile U32 *)0x50004014)		/* UART1 ����״̬�Ĵ���       */
 #define R_UFSTAT1		(*(volatile U32 *)0x50004018)		/* UART1 FIFO ״̬�Ĵ���      */
 #define R_UMSTAT1		(*(volatile U32 *)0x5000401c)		/* UART1 MODEM ״̬�Ĵ���     */
 #define R_UBRDIV1		(*(volatile U32 *)0x50004028)		/* UART1 �����ʳ����Ĵ���     */
 
 #define R_ULCON2		(*(volatile U32 *)0x50008000)		/* UART2��·���ƼĴ���        */
 #define R_UCON2		(*(volatile U32 *)0x50008004)		/* UART2���ƼĴ���            */
 #define R_UFCON2		(*(volatile U32 *)0x50008008)		/* UART2 FIFO ���ƼĴ���      */
 #define R_UMCON2		(*(volatile U32 *)0x5000800c)		/* UART2 MODEM ���ƼĴ���     */
 #define R_UTRSTAT2		(*(volatile U32 *)0x50008010)		/* UART2 ����/����״̬�Ĵ���  */
 #define R_UERSTAT2		(*(volatile U32 *)0x50008014)		/* UART2 ����״̬�Ĵ���       */
 #define R_UFSTAT2		(*(volatile U32 *)0x50008018)		/* UART2 FIFO ״̬�Ĵ���      */
 #define R_UMSTAT2		(*(volatile U32 *)0x5000801c)		/* UART2 MODEM ״̬�Ĵ���     */
 #define R_UBRDIV2		(*(volatile U32 *)0x50008028)		/* UART2 �����ʳ����Ĵ���     */
 
 #ifdef BIG_ENDIAN
 #define WRITE_UTXH0( ch )		(*(volatile U8 *)0x50000023) = (U8)( ch )
 #define READ_URXH0				(*(volatile U8 *)0x50000027)
 #define WRITE_UTXH1( ch )		(*(volatile U8 *)0x50004023) = (U8)( ch )
 #define READ_URXH1				(*(volatile U8 *)0x50004027)
 #define WRITE_UTXH2( ch )		(*(volatile U8 *)0x50008023) = (U8)( ch )
 #define READ_URXH2				(*(volatile U8 *)0x50008027)
 
 #define R_UTXH0		(*(volatile U8 *)0x50000023)		/* UART0 ���ͻ���Ĵ���       */
 #define R_URXH0		(*(volatile U8 *)0x50000027)		/* UART0 ���ܻ���Ĵ���       */
 #define R_UTXH1		(*(volatile U8 *)0x50004023)		/* UART1 ���ͻ���Ĵ���       */
 #define R_URXH1		(*(volatile U8 *)0x50004027)		/* UART1 ���ܻ���Ĵ���       */
 #define R_UTXH2		(*(volatile U8 *)0x50008023)		/* UART2 ���ͻ���Ĵ���       */
 #define R_URXH2		(*(volatile U8 *)0x50008027)		/* UART2 ���ܻ���Ĵ���       */
 #else
 #define WRITE_UTXH0( ch )		(*(volatile U8 *)0x50000020) = (U8)( ch )
 #define READ_URXH0				(*(volatile U8 *)0x50000024)
 #define WRITE_UTXH1( ch )		(*(volatile U8 *)0x50004020) = (U8)( ch )
 #define READ_URXH1				(*(volatile U8 *)0x50004024)
 #define WRITE_UTXH2( ch )		(*(volatile U8 *)0x50008020) = (U8)( ch )
 #define READ_URXH2				(*(volatile U8 *)0x50008024)
 
 #define R_UTXH0		(*(volatile U8 *)0x50000020)		/* UART0 ���ͻ���Ĵ���       */
 #define R_URXH0		(*(volatile U8 *)0x50000024)		/* UART0 ���ܻ���Ĵ���       */
 #define R_UTXH1		(*(volatile U8 *)0x50004020)		/* UART1 ���ͻ���Ĵ���       */
 #define R_URXH1		(*(volatile U8 *)0x50004024)		/* UART1 ���ܻ���Ĵ���       */
 #define R_UTXH2		(*(volatile U8 *)0x50008020)		/* UART2 ���ͻ���Ĵ���       */
 #define R_URXH2		(*(volatile U8 *)0x50008024)		/* UART2 ���ܻ���Ĵ���       */
 #endif
 
/*===============================================I/O======================================*/
 
 #define R_GPACON		(*(volatile U32 *)0x56000000)		/* �˿�A���ƼĴ���            */
 #define R_GPADAT		(*(volatile U32 *)0x56000004)		/* �˿�A���ݼĴ���            */
 
 #define R_GPBCON		(*(volatile U32 *)0x56000010)		/* �˿�B���ƼĴ���            */
 #define R_GPBDAT		(*(volatile U32 *)0x56000014)		/* �˿�B���ݼĴ���            */
 #define R_GPBUP		(*(volatile U32 *)0x56000018)		/* �˿�B�������üĴ���        */
 
 #define R_GPCCON		(*(volatile U32 *)0x56000020)		/* �˿�C���ƼĴ���            */
 #define R_GPCDAT		(*(volatile U32 *)0x56000024)		/* �˿�C���ݼĴ���            */
 #define R_GPCUP		(*(volatile U32 *)0x56000028)		/* �˿�C�������üĴ���        */
 
 #define R_GPDCON		(*(volatile U32 *)0x56000030)		/* �˿�D���ƼĴ���            */
 #define R_GPDDAT		(*(volatile U32 *)0x56000034)		/* �˿�D���ݼĴ���            */
 #define R_GPDUP		(*(volatile U32 *)0x56000038)		/* �˿�D�������üĴ���        */
 
 #define R_GPECON		(*(volatile U32 *)0x56000040)		/* �˿�E���ƼĴ���            */
 #define R_GPEDAT		(*(volatile U32 *)0x56000044)		/* �˿�E���ݼĴ���            */
 #define R_GPEUP		(*(volatile U32 *)0x56000048)		/* �˿�E�������üĴ���        */
 
 #define R_GPFCON		(*(volatile U32 *)0x56000050)		/* �˿�F���ƼĴ���            */
 #define R_GPFDAT		(*(volatile U32 *)0x56000054)		/* �˿�F���ݼĴ���            */
 #define R_GPFUP		(*(volatile U32 *)0x56000058)		/* �˿�F�������üĴ���        */
 
 #define R_GPGCON		(*(volatile U32 *)0x56000060)		/* �˿�G���ƼĴ���            */
 #define R_GPGDAT		(*(volatile U32 *)0x56000064)		/* �˿�G���ݼĴ���            */
 #define R_GPGUP		(*(volatile U32 *)0x56000068)		/* �˿�G�������üĴ���        */
 
 #define R_GPHCON		(*(volatile U32 *)0x56000070)		/* �˿�H���ƼĴ���            */
 #define R_GPHDAT		(*(volatile U32 *)0x56000074)		/* �˿�H���ݼĴ���            */
 #define R_GPHUP		(*(volatile U32 *)0x56000078)		/* �˿�H�������üĴ���        */
 
 #define R_GPJCON		(*(volatile U32 *)0x560000d0)		/* �˿�J���ƼĴ���            */
 #define R_GPJDAT		(*(volatile U32 *)0x560000d4)		/* �˿�J���ݼĴ���            */
 #define R_GPJUP		(*(volatile U32 *)0x560000d8)		/* �˿�J�������üĴ���        */
 
/*============================================LCD=========================================*/
 
 #define R_LCDCON1		(*(volatile U32 *)0x4d000000)		/* LCD���ƼĴ���1             */ 
 #define R_LCDCON2		(*(volatile U32 *)0x4d000004)		/* LCD���ƼĴ���2             */
 #define R_LCDCON3		(*(volatile U32 *)0x4d000008)		/* LCD���ƼĴ���3             */
 #define R_LCDCON4		(*(volatile U32 *)0x4d00000c)		/* LCD���ƼĴ���4             */
 #define R_LCDCON5		(*(volatile U32 *)0x4d000010)		/* LCD���ƼĴ���5             */
 #define R_LCDSADDR1	(*(volatile U32 *)0x4d000014)		/* ֡������ʼ��ַ�Ĵ���1      */
 #define R_LCDSADDR2	(*(volatile U32 *)0x4d000018)		/* ֡������ʼ��ַ�Ĵ���2      */
 #define R_LCDSADDR3	(*(volatile U32 *)0x4d00001c)		/* ֡������ʼ��ַ�Ĵ���3      */
 #define R_REDLUT		(*(volatile U32 *)0x4d000020)		/* ��ɫ��Ĵ���               */
 #define R_GREENLUT		(*(volatile U32 *)0x4d000024)		/* ��ɫ��Ĵ���               */
 #define R_BLUELUT		(*(volatile U32 *)0x4d000028)		/* ��ɫ��Ĵ���               */
 #define R_DITHMODE		(*(volatile U32 *)0x4d00004c)		/* ����ģʽ�Ĵ���             */
 #define R_TPAL			(*(volatile U32 *)0x4d000050)		/* ��ʱ��ɫ��Ĵ���           */
 #define R_LCDINTPND	(*(volatile U32 *)0x4d000054)		/* LCD�ж�δ���Ĵ���          */
 #define R_LCDSRCPND	(*(volatile U32 *)0x4d000058)		/* LCDԴδ���Ĵ���            */
 #define R_LCDINTMSK	(*(volatile U32 *)0x4d00005c)		/* LCD�ж����μĴ���          */
 #define R_TCONSEL		(*(volatile U32 *)0x4d000060)		/* LPC3600���ƼĴ���	      */
 
/*=========================================�˿������Ĵ���=================================*/
 
 #define R_MISCCR		(*(volatile U32 *)0x56000080)		/* ����ƼĴ���               */
 #define R_DCLKCON		(*(volatile U32 *)0x56000084)		/* DCLK�ⲿԴʱ�ӼĴ���       */
 #define R_EXTINT0		(*(volatile U32 *)0x56000088)		/* �ⲿ�жϿ��ƼĴ���0        */
 #define R_EXTINT1		(*(volatile U32 *)0x5600008c)		/* �ⲿ�жϿ��ƼĴ���1        */
 #define R_EXTINT2		(*(volatile U32 *)0x56000090)		/* �ⲿ�жϿ��ƼĴ���2        */
 #define R_EINTFLT0		(*(volatile U32 *)0x56000094)		/* �ⲿ�жϹ��˼Ĵ���0(����)  */
 #define R_EINTFLT1		(*(volatile U32 *)0x56000098)		/* �ⲿ�жϹ��˼Ĵ���1(����)  */
 #define R_EINTFLT2		(*(volatile U32 *)0x5600009c)		/* �ⲿ�жϹ��˼Ĵ���2        */
 #define R_EINTFLT3		(*(volatile U32 *)0x560000a0)		/* �ⲿ�жϹ��˼Ĵ���3        */
 #define R_EINTMASK		(*(volatile U32 *)0x560000a4)		/* �ⲿ�ж����μĴ���         */
 #define R_EINTPEND		(*(volatile U32 *)0x560000a8)		/* �ⲿ�ж�δ���Ĵ���         */
 #define R_GSTATUS0		(*(volatile U32 *)0x560000ac)		/* ͨ��״̬�Ĵ���0            */
 #define R_GSTATUS1		(*(volatile U32 *)0x560000b0)		/* ͨ��״̬�Ĵ���1            */
 #define R_GSTATUS2		(*(volatile U32 *)0x560000b4)		/* ͨ��״̬�Ĵ���2            */
 #define R_GSTATUS3		(*(volatile U32 *)0x560000b8)		/* ͨ��״̬�Ĵ���3            */
 #define R_GSTATUS4		(*(volatile U32 *)0x560000bc)		/* ͨ��״̬�Ĵ���4            */
 #define R_MSLCON		(*(volatile U32 *)0c560000cc)		/* �ڴ�˯�߿��ƼĴ���         */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __REGADDR_H */