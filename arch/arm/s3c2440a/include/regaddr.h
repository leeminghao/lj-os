/*
 *==========================================================================================
 * 										LJ_RTOS
 *								  The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 * 
 * 文件名称：regaddr.h
 * 文件标识：c语言头文件
 * 摘    要：本文件声明s3c2440a的寄存器地址
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/10
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==========================================================================================
 */
 
 #ifndef __REGADDR_H
 #define __REGADDR_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 
/*=======================================时钟及电源管理===================================*/

 #define R_LOCKTIME		(*(volatile U32 *)0x4c000000)		/* 锁定时间计数寄存器         */
 #define R_MPLLCON		(*(volatile U32 *)0x4c000004)		/* MPLL配置寄存器             */
 #define R_UPLLCON		(*(volatile U32 *)0x4c000008)		/* UPLL配置寄存器             */
 #define R_CLKCON		(*(volatile U32 *)0x4c00000c)		/* 时钟控制寄存器             */
 #define R_CLKSLOW		(*(volatile U32 *)0x4c000010)		/* 时钟减慢控制寄存器         */
 #define R_CLKDIVN		(*(volatile U32 *)0x4c000014)		/* 时钟分频器控制寄存器       */
 #define R_CAMDIVN		(*(volatile U32 *)0x4c000018)		/* 摄像头时钟分频器寄存器     */
 
/*=======================================中断控制寄存器===================================*/

 #define R_SRCPND		(*(volatile U32 *)0x4a000000)		/* 源未决寄存器               */
 #define R_INTMOD		(*(volatile U32 *)0x4a000004)		/* 中断模式寄存器             */
 #define R_INTMSK		(*(volatile U32 *)0x4a000008)		/* 中断屏蔽寄存器             */
 #define R_PRIORITY		(*(volatile U32 *)0x4a00000c)		/* 优先级寄存器               */
 #define R_INTPND		(*(volatile U32 *)0x4a000010)		/* 中断未决寄存器             */
 #define R_INTOFFSET	(*(volatile U32 *)0x4a000014)		/* 中断偏移寄存器             */
 #define R_SUBSRCPND	(*(volatile U32 *)0x4a000018)		/* 子中断源未决寄存器         */
 #define R_SUBINTMSK	(*(volatile U32 *)0x4a00001c)		/* 子中断屏蔽寄存器           */
 
/*=========================================看门狗定时器===================================*/

 #define R_WTCON		(*(volatile U32 *)0x53000000)		/* 看门狗控制寄存器           */
 #define R_WTDAT		(*(volatile U32 *)0x53000004)		/* 看门狗计数常数寄存器       */
 #define R_WTCNT		(*(volatile U32 *)0x53000008)		/* 看门狗计数寄存器           */
 
/*===========================================Timer========================================*/

 #define R_TCFG0		(*(volatile U32 *)0x51000000)		/* 定时器配置寄存器0          */
 #define R_TCFG1		(*(volatile U32 *)0x51000004)		/* 定时器配置寄存器1          */
 #define R_TCON			(*(volatile U32 *)0x51000008)		/* 定时器控制寄存器           */
 #define R_TCNTB0		(*(volatile U32 *)0x5100000c)		/* 定时器0计数缓冲寄存器      */
 #define R_TCMPB0		(*(volatile U32 *)0x51000010)		/* 定时器0比较缓冲寄存器      */
 #define R_TCNTO0		(*(volatile U32 *)0x51000014)		/* 定时器0计数观察寄存器      */
 #define R_TCNTB1		(*(volatile U32 *)0x51000018)		/* 定时器1计数缓冲寄存器      */
 #define R_TCMPB1		(*(volatile U32 *)0x5100001c)		/* 定时器1比较缓冲寄存器      */
 #define R_TCNTO1		(*(volatile U32 *)0x51000020)		/* 定时器1计数观察寄存器      */
 #define R_TCNTB2		(*(volatile U32 *)0x51000024)		/* 定时器2计数缓冲寄存器      */
 #define R_TCMPB2		(*(volatile U32 *)0x51000028)		/* 定时器2比较缓冲寄存器      */
 #define R_TCNTO2		(*(volatile U32 *)0x5100002c)		/* 定时器2计数观察寄存器      */
 #define R_TCNTB3		(*(volatile U32 *)0x51000030)		/* 定时器3计数缓冲寄存器      */
 #define R_TCMPB3		(*(volatile U32 *)0x51000034)		/* 定时器3比较缓冲寄存器      */
 #define R_TCNTO3		(*(volatile U32 *)0x51000038)		/* 定时器3计数观察寄存器      */
 #define R_TCNTB4		(*(volatile U32 *)0x5100003c)		/* 定时器4计数缓冲寄存器      */
 #define R_TCNTO4		(*(volatile U32 *)0x51000040)		/* 定时器4计数观察寄存器      */
 
/*===========================================RTC==========================================*/
 
 #ifdef BIG_ENDIAN
 #define R_RTCCON		(*(volatile U8 *)0x57000043)		/* RTC控制寄存器              */
 #define R_TICNT		(*(volatile U8 *)0x57000047)		/* 时间片计数器               */
 #define R_RTCALM		(*(volatile U8 *)0x57000053)		/* 报警控制寄存器             */
 #define R_ALMSEC		(*(volatile U8 *)0x57000057)		/* 报警秒数据寄存器           */
 #define R_ALMMIN		(*(volatile U8 *)0x5700005b)		/* 报警分数据寄存器           */
 #define R_ALMHOUR		(*(volatile U8 *)0x5700005f)		/* 报警时数据寄存器           */
 #define R_ALMDATE		(*(volatile U8 *)0x57000063)		/* 报警日数据寄存器           */
 #define R_ALMMON		(*(volatile U8 *)0x57000067)		/* 报警月数据寄存器           */
 #define R_ALMYEAR		(*(volatile U8 *)0x5700006b)		/* 报警年数据寄存器           */
 #define R_RTCRST		(*(volatile U8 *)0x5700006f)		/* 循环复位寄存器             */
 #define R_BCDSEC		(*(volatile U8 *)0x57000073)		/* 秒数据寄存器               */
 #define R_BCDMIN		(*(volatile U8 *)0x57000077)		/* 分数据寄存器               */
 #define R_BCDHOUR		(*(volatile U8 *)0x5700007b)		/* 时数据寄存器               */
 #define R_BCDDATE		(*(volatile U8 *)0x5700007f)		/* 日数据寄存器               */
 #define R_BCDDAY		(*(volatile U8 *)0x57000083)		/* 星期数据寄存器             */
 #define R_BCDMON		(*(volatile U8 *)0x57000087)		/* 月数据寄存器               */
 #define R_BCDYEAR		(*(volatile U8 *)0x5700008b)		/* 年数据寄存器               */
 #else
 #define R_RTCCON		(*(volatile U8 *)0x57000040)		/* RTC控制寄存器              */
 #define R_TICNT		(*(volatile U8 *)0x57000044)		/* 时间片计数器               */
 #define R_RTCALM		(*(volatile U8 *)0x57000050)		/* 报警控制寄存器             */
 #define R_ALMSEC		(*(volatile U8 *)0x57000054)		/* 报警秒数据寄存器           */
 #define R_ALMMIN		(*(volatile U8 *)0x57000058)		/* 报警分数据寄存器           */
 #define R_ALMHOUR		(*(volatile U8 *)0x5700005c)		/* 报警时数据寄存器           */
 #define R_ALMDATE		(*(volatile U8 *)0x57000060)		/* 报警日数据寄存器           */
 #define R_ALMMON		(*(volatile U8 *)0x57000064)		/* 报警月数据寄存器           */
 #define R_ALMYEAR		(*(volatile U8 *)0x57000068)		/* 报警年数据寄存器           */
 #define R_RTCRST		(*(volatile U8 *)0x5700006c)		/* 循环复位寄存器             */
 #define R_BCDSEC		(*(volatile U8 *)0x57000070)		/* 秒数据寄存器               */
 #define R_BCDMIN		(*(volatile U8 *)0x57000074)		/* 分数据寄存器               */
 #define R_BCDHOUR		(*(volatile U8 *)0x57000078)		/* 时数据寄存器               */
 #define R_BCDDATE		(*(volatile U8 *)0x5700007c)		/* 日数据寄存器               */
 #define R_BCDDAY		(*(volatile U8 *)0x57000080)		/* 星期数据寄存器             */
 #define R_BCDMON		(*(volatile U8 *)0x57000084)		/* 月数据寄存器               */
 #define R_BCDYEAR		(*(volatile U8 *)0x57000088)		/* 年数据寄存器               */
 #endif
 
/*===========================================UART=========================================*/
 
 #define R_ULCON0		(*(volatile U32 *)0x50000000)		/* UART0线路控制寄存器        */
 #define R_UCON0		(*(volatile U32 *)0x50000004)		/* UART0控制寄存器            */
 #define R_UFCON0		(*(volatile U32 *)0x50000008)		/* UART0 FIFO 控制寄存器      */
 #define R_UMCON0		(*(volatile U32 *)0x5000000c)		/* UART0 MODEM 控制寄存器     */
 #define R_UTRSTAT0		(*(volatile U32 *)0x50000010)		/* UART0 发送/接受状态寄存器  */
 #define R_UERSTAT0		(*(volatile U32 *)0x50000014)		/* UART0 错误状态寄存器       */
 #define R_UFSTAT0		(*(volatile U32 *)0x50000018)		/* UART0 FIFO 状态寄存器      */
 #define R_UMSTAT0		(*(volatile U32 *)0x5000001c)		/* UART0 MODEM 状态寄存器     */
 #define R_UBRDIV0		(*(volatile U32 *)0x50000028)		/* UART0 波特率除数寄存器     */
 
 #define R_ULCON1		(*(volatile U32 *)0x50004000)		/* UART1线路控制寄存器        */
 #define R_UCON1		(*(volatile U32 *)0x50004004)		/* UART1控制寄存器            */
 #define R_UFCON1		(*(volatile U32 *)0x50004008)		/* UART1 FIFO 控制寄存器      */
 #define R_UMCON1		(*(volatile U32 *)0x5000400c)		/* UART1 MODEM 控制寄存器     */
 #define R_UTRSTAT1		(*(volatile U32 *)0x50004010)		/* UART1 发送/接受状态寄存器  */
 #define R_UERSTAT1		(*(volatile U32 *)0x50004014)		/* UART1 错误状态寄存器       */
 #define R_UFSTAT1		(*(volatile U32 *)0x50004018)		/* UART1 FIFO 状态寄存器      */
 #define R_UMSTAT1		(*(volatile U32 *)0x5000401c)		/* UART1 MODEM 状态寄存器     */
 #define R_UBRDIV1		(*(volatile U32 *)0x50004028)		/* UART1 波特率除数寄存器     */
 
 #define R_ULCON2		(*(volatile U32 *)0x50008000)		/* UART2线路控制寄存器        */
 #define R_UCON2		(*(volatile U32 *)0x50008004)		/* UART2控制寄存器            */
 #define R_UFCON2		(*(volatile U32 *)0x50008008)		/* UART2 FIFO 控制寄存器      */
 #define R_UMCON2		(*(volatile U32 *)0x5000800c)		/* UART2 MODEM 控制寄存器     */
 #define R_UTRSTAT2		(*(volatile U32 *)0x50008010)		/* UART2 发送/接受状态寄存器  */
 #define R_UERSTAT2		(*(volatile U32 *)0x50008014)		/* UART2 错误状态寄存器       */
 #define R_UFSTAT2		(*(volatile U32 *)0x50008018)		/* UART2 FIFO 状态寄存器      */
 #define R_UMSTAT2		(*(volatile U32 *)0x5000801c)		/* UART2 MODEM 状态寄存器     */
 #define R_UBRDIV2		(*(volatile U32 *)0x50008028)		/* UART2 波特率除数寄存器     */
 
 #ifdef BIG_ENDIAN
 #define WRITE_UTXH0( ch )		(*(volatile U8 *)0x50000023) = (U8)( ch )
 #define READ_URXH0				(*(volatile U8 *)0x50000027)
 #define WRITE_UTXH1( ch )		(*(volatile U8 *)0x50004023) = (U8)( ch )
 #define READ_URXH1				(*(volatile U8 *)0x50004027)
 #define WRITE_UTXH2( ch )		(*(volatile U8 *)0x50008023) = (U8)( ch )
 #define READ_URXH2				(*(volatile U8 *)0x50008027)
 
 #define R_UTXH0		(*(volatile U8 *)0x50000023)		/* UART0 发送缓冲寄存器       */
 #define R_URXH0		(*(volatile U8 *)0x50000027)		/* UART0 接受缓冲寄存器       */
 #define R_UTXH1		(*(volatile U8 *)0x50004023)		/* UART1 发送缓冲寄存器       */
 #define R_URXH1		(*(volatile U8 *)0x50004027)		/* UART1 接受缓冲寄存器       */
 #define R_UTXH2		(*(volatile U8 *)0x50008023)		/* UART2 发送缓冲寄存器       */
 #define R_URXH2		(*(volatile U8 *)0x50008027)		/* UART2 接受缓冲寄存器       */
 #else
 #define WRITE_UTXH0( ch )		(*(volatile U8 *)0x50000020) = (U8)( ch )
 #define READ_URXH0				(*(volatile U8 *)0x50000024)
 #define WRITE_UTXH1( ch )		(*(volatile U8 *)0x50004020) = (U8)( ch )
 #define READ_URXH1				(*(volatile U8 *)0x50004024)
 #define WRITE_UTXH2( ch )		(*(volatile U8 *)0x50008020) = (U8)( ch )
 #define READ_URXH2				(*(volatile U8 *)0x50008024)
 
 #define R_UTXH0		(*(volatile U8 *)0x50000020)		/* UART0 发送缓冲寄存器       */
 #define R_URXH0		(*(volatile U8 *)0x50000024)		/* UART0 接受缓冲寄存器       */
 #define R_UTXH1		(*(volatile U8 *)0x50004020)		/* UART1 发送缓冲寄存器       */
 #define R_URXH1		(*(volatile U8 *)0x50004024)		/* UART1 接受缓冲寄存器       */
 #define R_UTXH2		(*(volatile U8 *)0x50008020)		/* UART2 发送缓冲寄存器       */
 #define R_URXH2		(*(volatile U8 *)0x50008024)		/* UART2 接受缓冲寄存器       */
 #endif
 
/*===============================================I/O======================================*/
 
 #define R_GPACON		(*(volatile U32 *)0x56000000)		/* 端口A控制寄存器            */
 #define R_GPADAT		(*(volatile U32 *)0x56000004)		/* 端口A数据寄存器            */
 
 #define R_GPBCON		(*(volatile U32 *)0x56000010)		/* 端口B控制寄存器            */
 #define R_GPBDAT		(*(volatile U32 *)0x56000014)		/* 端口B数据寄存器            */
 #define R_GPBUP		(*(volatile U32 *)0x56000018)		/* 端口B上拉设置寄存器        */
 
 #define R_GPCCON		(*(volatile U32 *)0x56000020)		/* 端口C控制寄存器            */
 #define R_GPCDAT		(*(volatile U32 *)0x56000024)		/* 端口C数据寄存器            */
 #define R_GPCUP		(*(volatile U32 *)0x56000028)		/* 端口C上拉设置寄存器        */
 
 #define R_GPDCON		(*(volatile U32 *)0x56000030)		/* 端口D控制寄存器            */
 #define R_GPDDAT		(*(volatile U32 *)0x56000034)		/* 端口D数据寄存器            */
 #define R_GPDUP		(*(volatile U32 *)0x56000038)		/* 端口D上拉设置寄存器        */
 
 #define R_GPECON		(*(volatile U32 *)0x56000040)		/* 端口E控制寄存器            */
 #define R_GPEDAT		(*(volatile U32 *)0x56000044)		/* 端口E数据寄存器            */
 #define R_GPEUP		(*(volatile U32 *)0x56000048)		/* 端口E上拉设置寄存器        */
 
 #define R_GPFCON		(*(volatile U32 *)0x56000050)		/* 端口F控制寄存器            */
 #define R_GPFDAT		(*(volatile U32 *)0x56000054)		/* 端口F数据寄存器            */
 #define R_GPFUP		(*(volatile U32 *)0x56000058)		/* 端口F上拉设置寄存器        */
 
 #define R_GPGCON		(*(volatile U32 *)0x56000060)		/* 端口G控制寄存器            */
 #define R_GPGDAT		(*(volatile U32 *)0x56000064)		/* 端口G数据寄存器            */
 #define R_GPGUP		(*(volatile U32 *)0x56000068)		/* 端口G上拉设置寄存器        */
 
 #define R_GPHCON		(*(volatile U32 *)0x56000070)		/* 端口H控制寄存器            */
 #define R_GPHDAT		(*(volatile U32 *)0x56000074)		/* 端口H数据寄存器            */
 #define R_GPHUP		(*(volatile U32 *)0x56000078)		/* 端口H上拉设置寄存器        */
 
 #define R_GPJCON		(*(volatile U32 *)0x560000d0)		/* 端口J控制寄存器            */
 #define R_GPJDAT		(*(volatile U32 *)0x560000d4)		/* 端口J数据寄存器            */
 #define R_GPJUP		(*(volatile U32 *)0x560000d8)		/* 端口J上拉设置寄存器        */
 
/*============================================LCD=========================================*/
 
 #define R_LCDCON1		(*(volatile U32 *)0x4d000000)		/* LCD控制寄存器1             */ 
 #define R_LCDCON2		(*(volatile U32 *)0x4d000004)		/* LCD控制寄存器2             */
 #define R_LCDCON3		(*(volatile U32 *)0x4d000008)		/* LCD控制寄存器3             */
 #define R_LCDCON4		(*(volatile U32 *)0x4d00000c)		/* LCD控制寄存器4             */
 #define R_LCDCON5		(*(volatile U32 *)0x4d000010)		/* LCD控制寄存器5             */
 #define R_LCDSADDR1	(*(volatile U32 *)0x4d000014)		/* 帧缓冲起始地址寄存器1      */
 #define R_LCDSADDR2	(*(volatile U32 *)0x4d000018)		/* 帧缓冲起始地址寄存器2      */
 #define R_LCDSADDR3	(*(volatile U32 *)0x4d00001c)		/* 帧缓冲起始地址寄存器3      */
 #define R_REDLUT		(*(volatile U32 *)0x4d000020)		/* 红色表寄存器               */
 #define R_GREENLUT		(*(volatile U32 *)0x4d000024)		/* 绿色表寄存器               */
 #define R_BLUELUT		(*(volatile U32 *)0x4d000028)		/* 蓝色表寄存器               */
 #define R_DITHMODE		(*(volatile U32 *)0x4d00004c)		/* 抖动模式寄存器             */
 #define R_TPAL			(*(volatile U32 *)0x4d000050)		/* 临时调色板寄存器           */
 #define R_LCDINTPND	(*(volatile U32 *)0x4d000054)		/* LCD中断未决寄存器          */
 #define R_LCDSRCPND	(*(volatile U32 *)0x4d000058)		/* LCD源未决寄存器            */
 #define R_LCDINTMSK	(*(volatile U32 *)0x4d00005c)		/* LCD中断屏蔽寄存器          */
 #define R_TCONSEL		(*(volatile U32 *)0x4d000060)		/* LPC3600控制寄存器	      */
 
/*=========================================端口其它寄存器=================================*/
 
 #define R_MISCCR		(*(volatile U32 *)0x56000080)		/* 多控制寄存器               */
 #define R_DCLKCON		(*(volatile U32 *)0x56000084)		/* DCLK外部源时钟寄存器       */
 #define R_EXTINT0		(*(volatile U32 *)0x56000088)		/* 外部中断控制寄存器0        */
 #define R_EXTINT1		(*(volatile U32 *)0x5600008c)		/* 外部中断控制寄存器1        */
 #define R_EXTINT2		(*(volatile U32 *)0x56000090)		/* 外部中断控制寄存器2        */
 #define R_EINTFLT0		(*(volatile U32 *)0x56000094)		/* 外部中断过滤寄存器0(保留)  */
 #define R_EINTFLT1		(*(volatile U32 *)0x56000098)		/* 外部中断过滤寄存器1(保留)  */
 #define R_EINTFLT2		(*(volatile U32 *)0x5600009c)		/* 外部中断过滤寄存器2        */
 #define R_EINTFLT3		(*(volatile U32 *)0x560000a0)		/* 外部中断过滤寄存器3        */
 #define R_EINTMASK		(*(volatile U32 *)0x560000a4)		/* 外部中断屏蔽寄存器         */
 #define R_EINTPEND		(*(volatile U32 *)0x560000a8)		/* 外部中断未决寄存器         */
 #define R_GSTATUS0		(*(volatile U32 *)0x560000ac)		/* 通用状态寄存器0            */
 #define R_GSTATUS1		(*(volatile U32 *)0x560000b0)		/* 通用状态寄存器1            */
 #define R_GSTATUS2		(*(volatile U32 *)0x560000b4)		/* 通用状态寄存器2            */
 #define R_GSTATUS3		(*(volatile U32 *)0x560000b8)		/* 通用状态寄存器3            */
 #define R_GSTATUS4		(*(volatile U32 *)0x560000bc)		/* 通用状态寄存器4            */
 #define R_MSLCON		(*(volatile U32 *)0c560000cc)		/* 内存睡眠控制寄存器         */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __REGADDR_H */