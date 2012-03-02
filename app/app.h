/*
 *==============================================================================================================
 *                                                  LJ_RTOS
 *                                            The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：app.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明用户的任务函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/24
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==============================================================================================================
 */
 #ifndef __APP_H
 #define __APP_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
/*==================================================声明任务堆栈大小============================================*/

 #define MAIN_TASK_STK_LENGTH	1024 * 2
 #define TASK0_STK_LENGTH		1024
 #define TASK1_STK_LENGTH		1024 * 2
 #define TASK2_STK_LENGTH		1024 * 2
 #define TASK_UART_STK_LENGTH	1024
 
/*====================================================声明任务函数==============================================*/
 
 void MainTask( void *pdata );
 void Task0( void *pdata );
 void Task1( void *pdata );
 void Task2( void *pdata );
 void TaskUart( void *pdata );
 
/*================================================声明任务优先级大小============================================*/
 
 #define NORMAL_PRIO		3
 #define MAIN_TASK_PRIO		(NORMAL_PRIO)
 #define TASK0_PRIO			(NORMAL_PRIO + 1)
 #define TASK1_PRIO			(NORMAL_PRIO + 2)
 #define TASK2_PRIO			(NORMAL_PRIO + 3)
 #define TASK_UART_PRIO		(NORMAL_PRIO + 4)
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __APP_H */