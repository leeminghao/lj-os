/*
 *============================================================================================
 *											LJ_RTOS
 *									  The Real-Time Kernel 
 * 
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：uart.h
 * 文件标识：c语言源文件
 * 摘    要：本文件声明目标板与宿主机通过串口进行通信的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/11
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==============================================================================================
 */
 
 #ifndef __UART_H
 #define __UART_H
 
 #ifdef __cpulsplus
 extern "C"
 {
 #endif
 
 #include "option.h"
 #include "define.h"
 #include "regaddr.h"
 #include "timer.h"
 #if !USE_MAIN
 #include <stdarg.h>
 #include <stdio.h>
 #endif
 
 #define STR_LEN		256						/* 字符串长度                                   */
 
 void UartInit( U32 pclk, U32 baud );
 void UartSelect( U32 port );
 void UartTxEmpty( U32 port );
 
 char UartGetCh( void );
 char UartGetChAccept( void );
 void UartGetString( char *pstr );
 
 void UartSendByte( char data );
 void UartSendString( char *pstr );
 
 #if !USE_MAIN
 void UartPrintf( const char *fmt, ... );
 #endif
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __UART_H */