/*
 *=============================================================================================
 * 											LJ_RTOS
 *									  The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�irq.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ���������s3c2440a�жϵ��жϴ�����
 * 
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/11
 * 
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=============================================================================================
 */
 
 #ifndef __IRQ_H
 #define __IRQ_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "regaddr.h"
 #include "timer.h"
 #include "uart.h"
 
 
 #define ISR_STARTADDRESS 	0x33ffff00			/* �ж���ʼ��ַ                                */
 
 /*===========================================�쳣����=========================================*/
 #define ISR_RESET		(*(U32 *)(ISR_STARTADDRESS + 0x0))			/* ��λ�쳣                */
 #define ISR_UNDEF		(*(U32 *)(ISR_STARTADDRESS + 0x4))			/* δ�����쳣              */
 #define ISR_SWI		(*(U32 *)(ISR_STARTADDRESS + 0x8))			/* ����쳣                */
 #define ISR_PABORT		(*(U32 *)(ISR_STARTADDRESS + 0xc))			/* ָ��Ԥȡ�쳣            */
 #define ISR_DABORT		(*(U32 *)(ISR_STARTADDRESS + 0x10))			/* �����쳣                */
 #define ISR_RESERVED	(*(U32 *)(ISR_STARTADDRESS + 0x14))			/* ����                    */
 #define ISR_IRQ		(*(U32 *)(ISR_STARTADDRESS + 0x18))			/* �ⲿ�ж��쳣            */
 #define ISR_FIQ		(*(U32 *)(ISR_STARTADDRESS + 0x1c))			/* �����ж��쳣            */
 /*==========================================�ж�����==========================================*/
 #define ISR_EINT0		(*(U32 *)(ISR_STARTADDRESS + 0x20))			/* �ⲿ�ж�0               */
 #define ISR_EINT1		(*(U32 *)(ISR_STARTADDRESS + 0x24))			/* �ⲿ�ж�1               */
 #define ISR_EINT2		(*(U32 *)(ISR_STARTADDRESS + 0x28))			/* �ⲿ�ж�2               */
 #define ISR_EINT3		(*(U32 *)(ISR_STARTADDRESS + 0x2c))			/* �ⲿ�ж�3               */
 #define ISR_EINT4_7	(*(U32 *)(ISR_STARTADDRESS + 0x30))			/* �ⲿ�ж�4_7             */
 #define ISR_EINT8_23	(*(U32 *)(ISR_STARTADDRESS + 0x34))			/* �ⲿ�ж�8_23            */
 #define ISR_CAM		(*(U32 *)(ISR_STARTADDRESS + 0x38))			/* ����ͷ�ж�              */
 #define ISR_BAT_FLT	(*(U32 *)(ISR_STARTADDRESS + 0x3c))			/* ���ʧЧ�ж�            */
 #define ISR_TICK		(*(U32 *)(ISR_STARTADDRESS + 0x40))			/* RTC��ʱ�ж�             */
 #define ISR_WDT_AC97	(*(U32 *)(ISR_STARTADDRESS + 0x44))			/* ���Ź���ʱ�ж�          */
 #define ISR_TIMER0		(*(U32 *)(ISR_STARTADDRESS + 0x48))			/* ��ʱ��0�ж�             */
 #define ISR_TIMER1		(*(U32 *)(ISR_STARTADDRESS + 0x4c))			/* ��ʱ��1�ж�             */
 #define ISR_TIMER2		(*(U32 *)(ISR_STARTADDRESS + 0x50))			/* ��ʱ��2�ж�             */
 #define ISR_TIMER3		(*(U32 *)(ISR_STARTADDRESS + 0x54))			/* ��ʱ��3�ж�             */
 #define ISR_TIMER4		(*(U32 *)(ISR_STARTADDRESS + 0x58))			/* ��ʱ��4�ж�             */
 #define ISR_UART2		(*(U32 *)(ISR_STARTADDRESS + 0x5c))			/* ����2�ж�               */
 #define ISR_LCD		(*(U32 *)(ISR_STARTADDRESS + 0x60))			/* LCD�ж�                 */
 #define ISR_DMA0		(*(U32 *)(ISR_STARTADDRESS + 0x64))			/* DMAͨ��0�ж�            */
 #define ISR_DMA1		(*(U32 *)(ISR_STARTADDRESS + 0x68))			/* DMAͨ��1�ж�            */
 #define ISR_DMA2		(*(U32 *)(ISR_STARTADDRESS + 0x6c))			/* DMAͨ��2�ж�            */
 #define ISR_DMA3		(*(U32 *)(ISR_STARTADDRESS + 0x70))			/* DMAͨ��3�ж�            */
 #define ISR_SDI		(*(U32 *)(ISR_STARTADDRESS + 0x74))			/* SDI�ж�                 */
 #define ISR_SPI0		(*(U32 *)(ISR_STARTADDRESS + 0x78))			/* SPI0�ж�                */
 #define ISR_UART1		(*(U32 *)(ISR_STARTADDRESS + 0x7c))			/* ����1�ж�               */
 #define ISR_NFCON		(*(U32 *)(ISR_STARTADDRESS + 0x80))			/* Added for 2440a         */
 #define ISR_USBD		(*(U32 *)(ISR_STARTADDRESS + 0x84))			/* USB�豸�ж�             */
 #define ISR_USBH		(*(U32 *)(ISR_STARTADDRESS + 0x88))			/* USB�����ж�             */
 #define ISR_IIC		(*(U32 *)(ISR_STARTADDRESS + 0x8c))			/* IIC�ж�                 */
 #define ISR_UART0		(*(U32 *)(ISR_STARTADDRESS + 0x90))			/* ����0�ж�               */
 #define ISR_SPI1		(*(U32 *)(ISR_STARTADDRESS + 0x94))			/* SPI1�ж�                */
 #define ISR_RTC		(*(U32 *)(ISR_STARTADDRESS + 0x98))			/* RTC�ж�                 */
 #define ISR_ADC		(*(U32 *)(ISR_STARTADDRESS + 0x9c))			/* ADC�����жϣ��������ж� */
 
 /*============================================�ж�δ��λ======================================*/
 #define BIT_EINT0		(0x1)
 #define BIT_EINT1		(0x1 << 1)
 #define BIT_EINT2		(0x1 << 2)
 #define BIT_EINT3		(0x1 << 3)
 #define BIT_EINT4_7	(0x1 << 4)
 #define BIT_EINT8_23	(0x1 << 5)
 #define BIT_CAM		(0x1 << 6)
 #define BIT_BAT_FLT	(0x1 << 7)
 #define BIT_TICK		(0x1 << 8)
 #define BIT_WDT_AC97	(0x1 << 9)
 #define BIT_TIMER0		(0x1 << 10)
 #define BIT_TIMER1		(0x1 << 11)
 #define BIT_TIMER2		(0x1 << 12)
 #define BIT_TIMER3		(0x1 << 13)
 #define BIT_TIMER4		(0x1 << 14)
 #define BIT_UART2		(0x1 << 15)
 #define BIT_LCD		(0x1 << 16)
 #define BIT_DMA0		(0x1 << 17)
 #define BIT_DMA1		(0x1 << 18)
 #define BIT_DMA2		(0x1 << 19)
 #define BIT_DMA3		(0x1 << 20)
 #define BIT_SDI		(0x1 << 21)
 #define BIT_SPI0		(0x1 << 22)
 #define BIT_UART1		(0x1 << 23)
 #define BIT_NFCON		(0x1 << 24)
 #define BIT_USBD		(0x1 << 25)
 #define BIT_USBH		(0x1 << 26)
 #define BIT_IIC		(0x1 << 27)
 #define BIT_UART0		(0x1 << 28)
 #define BIT_SPI1		(0x1 << 29)
 #define BIT_RTC		(0x1 << 30)
 #define BIT_ADC		(0x1 << 31)
 #define BIT_ALLMSK		(0xffffffff)
 
 #define BIT_SUB_AC97	(0x1 << 14)
 #define BIT_SUB_WDT	(0x1 << 13)
 #define BIT_SUB_CAM_P	(0x1 << 12)
 #define BIT_SUB_CAM_C	(0x1 << 11)
 #define BIT_SUB_ADC	(0x1 << 10)
 #define BIT_SUB_TC		(0x1 << 9)
 #define BIT_SUB_ERR2	(0x1 << 8)
 #define BIT_SUB_TXD2	(0x1 << 7)
 #define BIT_SUB_RXD2	(0x1 << 6)
 #define BIT_SUB_ERR1	(0x1 << 5)
 #define BIT_SUB_TXD1	(0x1 << 4)
 #define BIT_SUB_RXD1	(0x1 << 3)
 #define BIT_SUB_ERR0	(0x1 << 2)
 #define BIT_SUB_TXD0	(0x1 << 1)
 #define BIT_SUB_RXD0	(0x1 << 0)
 #define BIT_SUB_ALLMSK	(0x7fff)
 
 #define BIT_EXT_EINT4	(0x1 << 4)
 #define BIT_EXT_EINT5	(0x1 << 5)
 #define BIT_EXT_EINT6	(0x1 << 6)
 #define BIT_EXT_EINT7	(0x1 << 7)
 #define BIT_EXT_EINT8	(0x1 << 8)
 #define BIT_EXT_EINT9	(0x1 << 9)
 #define BIT_EXT_EINT10	(0x1 << 10)
 #define BIT_EXT_EINT11	(0x1 << 11)
 #define BIT_EXT_EINT12	(0x1 << 12)
 #define BIT_EXT_EINT13	(0x1 << 13)
 #define BIT_EXT_EINT14	(0x1 << 14)
 #define BIT_EXT_EINT15	(0x1 << 15)
 #define BIT_EXT_EINT16	(0x1 << 16)
 #define BIT_EXT_EINT17	(0x1 << 17)
 #define BIT_EXT_EINT18 (0x1 << 18)
 #define BIT_EXT_EINT19	(0x1 << 19)
 #define BIT_EXT_EINT20	(0x1 << 20)
 #define BIT_EXT_EINT21	(0x1 << 21)
 #define BIT_EXT_EINT22	(0x1 << 22)
 #define BIT_EXT_EINT23	(0x1 << 23)
 
 #define ENABLE_IRQ( bit )		R_INTMSK &= ~(bit)		/* �����ⲿ�ж�                           */
 #define DISABLE_IRQ( bit )		R_INTMSK |=  (bit)		/* �����ⲿ�ж�                           */
 #define ENABLE_SUB_IRQ( bit )	R_SUBINTMSK &= ~(bit)	/* �����ⲿ���ж�                         */
 #define DISABLE_SUB_IRQ( bit )	R_SUBINTMSK |=  (bit)	/* �����ⲿ���ж�                         */
 
 void ClearPending( int bit );
 void ClearSubPending( int bit );
 void ClearExtPending( int bit );
 void IsrInit( void );
 void __irq Timer0Isr( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __IRQ_H */