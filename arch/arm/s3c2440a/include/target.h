/*
 *===============================================================================================
 *                                         LJ_RTOS
 *                                   The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：target.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明初始化目标板的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/18
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *===============================================================================================
 */
 
 #ifndef __TARGET_H
 #define __TARGET_H
 
 #ifdef __cplusplus
 extern "C"
 { 
 #endif
 
 #include "define.h"
 #include "irq.h"
 #include "option.h"
 #include "regaddr.h"
 #include "timer.h"
 #include "uart.h"
 #include "clock.h"
 #include "mmu.h"
 #include "lcd.h"
 #include "rtc.h"
 #include "port.h"
 
 void TargetInit( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __TARGET_H */