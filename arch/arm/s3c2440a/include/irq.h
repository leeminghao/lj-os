/*
 *=============================================================================================
 * 											LJ_RTOS
 *									  The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：irq.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明s3c2440a中断的中断处理函数
 * 
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/11
 * 
 * 取代版本：
 * 作    者：
 * 完成日期：
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
 
 
 #define ISR_STARTADDRESS 	0x33ffff00			/* 中断起始地址                                */
 
 /*===========================================异常向量=========================================*/
 #define ISR_RESET		(*(U32 *)(ISR_STARTADDRESS + 0x0))			/* 复位异常                */
 #define ISR_UNDEF		(*(U32 *)(ISR_STARTADDRESS + 0x4))			/* 未定义异常              */
 #define ISR_SWI		(*(U32 *)(ISR_STARTADDRESS + 0x8))			/* 软件异常                */
 #define ISR_PABORT		(*(U32 *)(ISR_STARTADDRESS + 0xc))			/* 指令预取异常            */
 #define ISR_DABORT		(*(U32 *)(ISR_STARTADDRESS + 0x10))			/* 数据异常                */
 #define ISR_RESERVED	(*(U32 *)(ISR_STARTADDRESS + 0x14))			/* 保留                    */
 #define ISR_IRQ		(*(U32 *)(ISR_STARTADDRESS + 0x18))			/* 外部中断异常            */
 #define ISR_FIQ		(*(U32 *)(ISR_STARTADDRESS + 0x1c))			/* 快速中断异常            */
 /*==========================================中断向量==========================================*/
 #define ISR_EINT0		(*(U32 *)(ISR_STARTADDRESS + 0x20))			/* 外部中断0               */
 #define ISR_EINT1		(*(U32 *)(ISR_STARTADDRESS + 0x24))			/* 外部中断1               */
 #define ISR_EINT2		(*(U32 *)(ISR_STARTADDRESS + 0x28))			/* 外部中断2               */
 #define ISR_EINT3		(*(U32 *)(ISR_STARTADDRESS + 0x2c))			/* 外部中断3               */
 #define ISR_EINT4_7	(*(U32 *)(ISR_STARTADDRESS + 0x30))			/* 外部中断4_7             */
 #define ISR_EINT8_23	(*(U32 *)(ISR_STARTADDRESS + 0x34))			/* 外部中断8_23            */
 #define ISR_CAM		(*(U32 *)(ISR_STARTADDRESS + 0x38))			/* 摄像头中断              */
 #define ISR_BAT_FLT	(*(U32 *)(ISR_STARTADDRESS + 0x3c))			/* 电池失效中断            */
 #define ISR_TICK		(*(U32 *)(ISR_STARTADDRESS + 0x40))			/* RTC定时中断             */
 #define ISR_WDT_AC97	(*(U32 *)(ISR_STARTADDRESS + 0x44))			/* 看门狗定时中断          */
 #define ISR_TIMER0		(*(U32 *)(ISR_STARTADDRESS + 0x48))			/* 定时器0中断             */
 #define ISR_TIMER1		(*(U32 *)(ISR_STARTADDRESS + 0x4c))			/* 定时器1中断             */
 #define ISR_TIMER2		(*(U32 *)(ISR_STARTADDRESS + 0x50))			/* 定时器2中断             */
 #define ISR_TIMER3		(*(U32 *)(ISR_STARTADDRESS + 0x54))			/* 定时器3中断             */
 #define ISR_TIMER4		(*(U32 *)(ISR_STARTADDRESS + 0x58))			/* 定时器4中断             */
 #define ISR_UART2		(*(U32 *)(ISR_STARTADDRESS + 0x5c))			/* 串口2中断               */
 #define ISR_LCD		(*(U32 *)(ISR_STARTADDRESS + 0x60))			/* LCD中断                 */
 #define ISR_DMA0		(*(U32 *)(ISR_STARTADDRESS + 0x64))			/* DMA通道0中断            */
 #define ISR_DMA1		(*(U32 *)(ISR_STARTADDRESS + 0x68))			/* DMA通道1中断            */
 #define ISR_DMA2		(*(U32 *)(ISR_STARTADDRESS + 0x6c))			/* DMA通道2中断            */
 #define ISR_DMA3		(*(U32 *)(ISR_STARTADDRESS + 0x70))			/* DMA通道3中断            */
 #define ISR_SDI		(*(U32 *)(ISR_STARTADDRESS + 0x74))			/* SDI中断                 */
 #define ISR_SPI0		(*(U32 *)(ISR_STARTADDRESS + 0x78))			/* SPI0中断                */
 #define ISR_UART1		(*(U32 *)(ISR_STARTADDRESS + 0x7c))			/* 串口1中断               */
 #define ISR_NFCON		(*(U32 *)(ISR_STARTADDRESS + 0x80))			/* Added for 2440a         */
 #define ISR_USBD		(*(U32 *)(ISR_STARTADDRESS + 0x84))			/* USB设备中断             */
 #define ISR_USBH		(*(U32 *)(ISR_STARTADDRESS + 0x88))			/* USB主机中断             */
 #define ISR_IIC		(*(U32 *)(ISR_STARTADDRESS + 0x8c))			/* IIC中断                 */
 #define ISR_UART0		(*(U32 *)(ISR_STARTADDRESS + 0x90))			/* 串口0中断               */
 #define ISR_SPI1		(*(U32 *)(ISR_STARTADDRESS + 0x94))			/* SPI1中断                */
 #define ISR_RTC		(*(U32 *)(ISR_STARTADDRESS + 0x98))			/* RTC中断                 */
 #define ISR_ADC		(*(U32 *)(ISR_STARTADDRESS + 0x9c))			/* ADC结束中断，触摸屏中断 */
 
 /*============================================中断未决位======================================*/
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
 
 #define ENABLE_IRQ( bit )		R_INTMSK &= ~(bit)		/* 允许外部中断                           */
 #define DISABLE_IRQ( bit )		R_INTMSK |=  (bit)		/* 屏蔽外部中断                           */
 #define ENABLE_SUB_IRQ( bit )	R_SUBINTMSK &= ~(bit)	/* 允许外部子中断                         */
 #define DISABLE_SUB_IRQ( bit )	R_SUBINTMSK |=  (bit)	/* 屏蔽外部子中断                         */
 
 void ClearPending( int bit );
 void ClearSubPending( int bit );
 void ClearExtPending( int bit );
 void IsrInit( void );
 void __irq Timer0Isr( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __IRQ_H */