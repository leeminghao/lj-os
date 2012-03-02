/*
 *=====================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_cfg.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于用户对LJ_RTOS自行配置使用
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/17
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=====================================================================================================
 */
 
 #ifndef __LJ_CFG_H
 #define __LJ_CFG_H
 
 #ifdef __cpluspluls
 extern "C"
 {
 #endif
 
 											/*========================任务堆栈大小====================*/
 #define LJ_TASK_IDLE_STK_SIZE		128		/* 空闲任务堆栈大小(指针元数目)                           */
 
 											/*============================杂项========================*/
 #define LJ_LOWEST_PRIO				63		/* 定义LJ_RTOS最低优先级任务，注意：必须 < 64             */
 #define LJ_ARG_CHK_EN				1		/* 用于设定是否允许大多数LJ_RTOS中的函数执行参数检查      */
 											/* 功能：1 = 允许，0 = 不允许                             */
 #define LJ_MAX_TASKS				30		/* 用于定义用户程序中可以使用的最多任务数                 */
 #define LJ_MAX_EVENTS				20		/* 用于定义系统中可分配的事件控制块的最大数目             */
 #define LJ_MAX_QS					25		/* 用于定义系统中可以创建的消息队列的最大数目             */
 #define LJ_MAX_MEM_PART			25		/* 用于定义系统中内存块的最大数据                         */
 #define LJ_SCHED_LOCK_EN			1		/* 控制是否使用LJSchedLock()和LJSchedUnlock()             */
 
 											/*==========================任务管理======================*/
 #define LJ_TASK_CHANGE_PRIO_EN		1		/* 控制是否使用LJ_RTOS中的LJTaskChangePrio()函数          */
 #define LJ_TASK_CREATE_EN			1		/* 控制是否使用LJ_RTOS中的LJTaskCreate()函数              */
 #define LJ_TASK_CREATE_EXT_EN		1		/* 控制是否使用LJ_RTOS中的LJTaskCreateExt()函数           */
 #define LJ_TASK_DEL_EN				1		/* 控制是否使用LJ_RTOS中的LJTaskDel()和LJTaskDelReq()函数 */
 #define LJ_TASK_SUSPEND_EN			1		/* 控制是否使用LJ_RTOS中的LJTaskSuspend()和LJTaskResume() */
 #define LJ_TASK_QUERY_EN			1		/* 控制是否使用LJ_RTOS中的LJTaskQuery()函数               */
 #define LJ_TICKS_PER_SEC			200		/* 设定系统每秒产生的系统时钟节拍数                       */
 
 											/*==========================时间管理======================*/
 #define LJ_TIME_DLY_HMSM_EN		1		/* 控制是否使用LJTimeDlyHMSM()函数                        */
 #define LJ_TIME_DLY_RESUME_EN		1		/* 控制是否使用LJTimeDlyResume()函数                      */
 #define LJ_TIME_GET_SET_EN			1		/* 控制是否使用LJTimeGet()和LJTimeSet()函数               */
 
 											/*===========================信号量=======================*/
 #define LJ_SEM_EN					1		/* 控制是否使用LJ_RTOS中的信号量管理函数                  */
 #define LJ_SEM_ACCEPT_EN			1		/* 控制是否使用LJ_RTOS中的LJSemAccept()函数               */
 #define LJ_SEM_DEL_EN				1		/* 控制是否使用LJ_RTOS中的LJSemDel()函数                  */
 #define LJ_SEM_QUERY_EN			1		/* 控制是否使用LJ_RTOS中的LJSemQuery()函数                */
 
 											/*========================互斥型信号量====================*/
 #define LJ_MUTEX_EN				1		/* 控制是否使用互斥型信号量函数及其相关数据结构           */
 #define LJ_MUTEX_ACCEPT_EN 		1		/* 控制是否使用LJ_RTOS中的LJMutexAccept()函数             */
 #define LJ_MUTEX_DEL_EN 			1		/* 控制是否使用LJ_RTOS中的LJMutexDel()函数                */
 #define LJ_MUTEX_QUERY_EN			1		/* 控制是否使用LJ_RTOS中的LJMutexQuery()函数              */
 
 											/*==========================消息邮箱======================*/
 #define LJ_MBOX_EN					1		/* 控制是否使用LJ_RTOS中的消息邮箱函数及其相关数据结构    */
 #define LJ_MBOX_POST_EN			1		/* 控制是否使用LJ_RTOS中的LJMboxPost()函数                */
 #define LJ_MBOX_DEL_EN				1		/* 控制是否使用LJ_RTOS中的LJMboxDel()函数                 */
 #define LJ_MBOX_ACCEPT_EN			1		/* 控制是否使用LJ_RTOS中的LJMboxAccept()函数              */
 #define LJ_MBOX_QUERY_EN			1		/* 控制是否使用LJ_RTOS中的LJMboxQuery()函数               */
 
 											/*==========================消息队列======================*/
 #define LJ_Q_EN					1		/* 控制是否使用LJ_RTOS中的消息队列函数及其相关的数据结构  */
 #define LJ_Q_POST_EN				1		/* 控制是否使用LJ_RTOS中的LJQPost()函数                   */
 #define LJ_Q_DEL_EN				1		/* 控制是否使用LJ_RTOS中的LJQDel()函数                    */
 #define LJ_Q_ACCEPT_EN				1		/* 控制是否使用LJ_RTOS中的LJQAccept()函数                 */
 #define LJ_Q_FLUSH_EN				1		/* 控制是否使用LJ_RTOS中的LJQFlush()函数                  */
 #define LJ_Q_QUERY_EN              1		/* 控制是否使用LJ_RTOS中的LJQQuery()函数                  */
 
 											/*==========================内存管理======================*/
 #define LJ_MEM_EN					1		/* 控制是否使用LJ_RTOS中的内存块管理函数及其相关数据结构  */
 #define LJ_MEM_QUERY_EN			1		/* 控制是否使用LJ_RTOS中的LJMemQuery()函数                */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_CFG_H */