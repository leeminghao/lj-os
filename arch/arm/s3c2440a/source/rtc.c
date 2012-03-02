/*
 *=============================================================================================
 *                                              LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：rtc.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现与实时时钟有关的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/16
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=============================================================================================
 */
 
 #include "rtc.h"
 
/*
 *============================================================================================
 * 函数名称：RtcInit
 * 功能描述：实时时钟初始化函数
 * 传    参：void
 * 返 回 值：void
 *============================================================================================
 */
 void
 RtcInit( void )
 {
 	S32 year, month, day, dayOfWeek, hour, minute, second;
 	
 	year		= 2010;
 	month		= 11;
 	day			= 16;
 	dayOfWeek	= 2;
 	hour		= 15;
 	minute		= 39;
 	second		= 20;
 	
 	R_RTCCON	= 1;									/* RTC使能                            */
 	
 	R_BCDYEAR	= (U8)TO_BCD( year % 100 );				/* 年                                 */
 	R_BCDMON	= (U8)TO_BCD( month );					/* 月                                 */
 	R_BCDDATE	= (U8)TO_BCD( day );					/* 日                                 */
 	R_BCDDAY	= (U8)TO_BCD( dayOfWeek );				/* 星期                               */
 	R_BCDHOUR	= (U8)TO_BCD( hour );					/* 时                                 */
 	R_BCDMIN	= (U8)TO_BCD( minute );					/* 分                                 */
 	R_BCDSEC	= (U8)TO_BCD( second );					/* 秒                                 */
 	
 	R_RTCCON   &= ~1;
 }