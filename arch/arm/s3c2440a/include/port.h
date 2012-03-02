/*
 *============================================================================================
 *                                           LJ_RTOS
 *                                     The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：port.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明I/O端口的初始化函数
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
 
 #ifndef __PORT_H
 #define __PORT_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "regaddr.h"
 
 void PortInit( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __PORT_H */