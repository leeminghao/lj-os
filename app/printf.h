/*
 *=================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�printf.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ��������������������ĺ���(������)
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/24
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=================================================================================================
 */
 #ifndef __PRINTF_H
 #define __PRINTF_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "app.h"
 #include <lj_rtos.h>
 #include <target.h>
 #include <stdio.h>
 #include <string.h>
 
 void LJPrintfInit( void );
 void LJPrintf( const char *fmt, ... );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif