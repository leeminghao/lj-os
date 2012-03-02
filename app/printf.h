/*
 *=================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：printf.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明声明输出任务的函数(测试用)
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/24
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
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