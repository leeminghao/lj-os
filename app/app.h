/*
 *==============================================================================================================
 *                                                  LJ_RTOS
 *                                            The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�app.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ����������û���������
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/24
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *==============================================================================================================
 */
 #ifndef __APP_H
 #define __APP_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
/*==================================================���������ջ��С============================================*/

 #define MAIN_TASK_STK_LENGTH	1024 * 2
 #define TASK0_STK_LENGTH		1024
 #define TASK1_STK_LENGTH		1024 * 2
 #define TASK2_STK_LENGTH		1024 * 2
 #define TASK_UART_STK_LENGTH	1024
 
/*====================================================����������==============================================*/
 
 void MainTask( void *pdata );
 void Task0( void *pdata );
 void Task1( void *pdata );
 void Task2( void *pdata );
 void TaskUart( void *pdata );
 
/*================================================�����������ȼ���С============================================*/
 
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