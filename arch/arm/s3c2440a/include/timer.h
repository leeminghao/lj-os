/*
 *=============================================================================================
 * 												LJ_RTOS
 * 										  The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：time.h
 * 文件标识：c语言头文件
 * 摘    要：本文件声明与定时部件有关的函数
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
 
 #ifndef __TIME_H
 #define __TIME_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 #include "option.h"
 #include "regaddr.h"
 #include <lj_cfg.h>
 
 void Delay( U32 time );
 void Timer0Init( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __TIME_H */