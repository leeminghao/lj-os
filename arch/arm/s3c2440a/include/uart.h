/*
 *============================================================================================
 *											LJ_RTOS
 *									  The Real-Time Kernel 
 * 
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�uart.h
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����Ŀ�����������ͨ�����ڽ���ͨ�ŵĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/11
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
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
 
 #define STR_LEN		256						/* �ַ�������                                   */
 
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