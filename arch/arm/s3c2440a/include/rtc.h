/*
 *============================================================================================
 *                                           LJ_RTOS
 *                                    The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：rtc.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明与RTC有关的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/16
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *============================================================================================
 */
 
 #ifndef __RTC_H
 #define __RTC_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 #include "regaddr.h"
 
 #define TO_BCD( n )	((((n) / 10) << 4) | ((n) % 10))
 #define FROM_BCD( n )	((((n) >> 4) * 10) + ((n) & 0xf))
 
 void RtcInit( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __RTC_H */