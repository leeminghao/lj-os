/*
 *=============================================================================================
 *                                                LJ_RTOS
 *                                          The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_cpu.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明与CPU或编译器有关的类型声明
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
 
 #ifndef __LJ_CPU_H
 #define __LJ_CPU_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 typedef unsigned char    BOOLEAN;					/* 布尔类型                                */
 typedef unsigned char    INT8U;					/* 无符号8位整数                           */
 typedef signed   char    INT8S;					/* 有符号8位整数                           */
 typedef unsigned short   INT16U;					/* 无符号16位整数                          */
 typedef signed   short   INT16S;					/* 有符号16位整数                          */
 typedef unsigned int     INT32U;					/* 无符号32位整数                          */
 typedef signed   int     INT32S;					/* 有符号32位整数                          */
 
 typedef INT32U			  LJ_STK;					/* 设置堆栈宽度为32位                      */
 
 typedef INT32U			  LJ_CPU_SR;				/* 定义CPU状态寄存器宽度为32位             */
 
 
 #define LJ_CRITICAL_METHOD		1					/* 用于选择开关中断的方法                  */
 
 #if LJ_CRITICAL_METHOD == 1
 #define LJ_ENTER_CRITICAL( ) (cpuSR = LJCpuSaveSR( ))	  	  /* 进入临界区，也就是关中断      */
 #define LJ_EXIT_CRITICAL( )   (LJCpuRestoreSR( cpuSR ))	  /* 退出临界区，也就是开中断      */
 #endif
 
 #define LJ_STK_GROWTH		1	/* 1 = 堆栈从高地址向低地址增长, 0 = 堆栈从低地址向高地址增长  */
 
 #define LJ_TASK_SW( )		LJCtxSw( )				/* 任务切换                                */
 
 #if LJ_CRITICAL_METHOD == 1						/* 声明开关中断函数                        */
 LJ_CPU_SR LJCpuSaveSR( void );
 void      LJCpuRestoreSR( LJ_CPU_SR cpuSr );
 #endif
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_CPU_H */