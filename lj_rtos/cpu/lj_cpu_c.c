/*
 *===============================================================================================
 *                                           LJ_RTOS
 *                                     The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_cpu_c.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现LJ_RTOS与CPU有关的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *===============================================================================================
 */
 
 #include <lj_cpu.h>
 
/*
 *===============================================================================================
 * 函数名称：LJTaskStkInit
 * 功能描述：初始化任务堆栈
 *
 * 传    参：task  (void (*)( void *pd ))：指向任务代码的指针
 *           pdata (void *)：              任务开始执行时，传递给任务的参数的指针
 *           ptos  (LJ_STK *)：            分配给任务的堆栈的栈顶指针
 *
 * 返 回 值：stk (LJ_STK *)
 *===============================================================================================
 */
 LJ_STK *
 LJTaskStkInit( void (*task)( void *pd ), void *pdata, LJ_STK *ptos, INT16U opt )
 {
 	LJ_STK	*stk;
 	
 	opt			= opt;
 	stk			= ptos;
 	
 	*(stk)		= (LJ_STK)task;					/* 入口点                                        */
 	*(--stk)	= (INT32U)0;					/* LR											 */
 	*(--stk)	= (INT32U)0;					/* R12                                           */
 	*(--stk)	= (INT32U)0;					/* R11                                           */
 	*(--stk)	= (INT32U)0;					/* R10                                           */
 	*(--stk)	= (INT32U)0;					/* R9                                            */
 	*(--stk)	= (INT32U)0;					/* R8                                            */
 	*(--stk)	= (INT32U)0;					/* R7                                            */
 	*(--stk)	= (INT32U)0;					/* R6                                            */
 	*(--stk)	= (INT32U)0;					/* R5                                            */
 	*(--stk)	= (INT32U)0;					/* R4                                            */
 	*(--stk)	= (INT32U)0;					/* R3                                            */
 	*(--stk)	= (INT32U)0;					/* R2                                            */
 	*(--stk)	= (INT32U)0;					/* R1                                            */
 	*(--stk)	= (INT32U)pdata;				/* R0                                            */
 	*(--stk)	= (INT32U)0x00000013;			/* CPSR( SVC 模式，允许IRQ和FIQ中断)             */
 	
 	return stk;
 }
 