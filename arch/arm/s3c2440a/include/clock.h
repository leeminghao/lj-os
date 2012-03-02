/*
 *=============================================================================================
 *                                              LJ_TROS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：clock.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明s3c2440a时钟主频的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/14
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=============================================================================================
 */
 
 #ifndef __CLOCK_H
 #define __CLOCK_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "regaddr.h"
 #include "define.h"
 
 #define ENTER_PWDN( clkcon )		((void (*)(int))0x20)( clkcon )		/* 电源管理            */
 
 void StartPointAfterSleepWakeUp( void );								/* 汇编语言实现        */
 void ChangeClockDivider( U32 hdivnVal, U32 pdivnVal );
 void ChangeUPLLValue( U32 m, U32 p, U32 s );
 void ChangeMPLLValue( U32 m, U32 p, U32 s );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __CLOCK_H */