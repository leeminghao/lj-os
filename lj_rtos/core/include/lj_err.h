/*
 *==================================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_err.h
 * 文件标识：c语言头文件
 * 摘    要：本文件声明LJ_RTOS所使用的错误码和对用户配置错误进行管理
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/24
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==================================================================================================================
 */
 
 #ifndef __LJ_ERR_H
 #define __LJ_ERR_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #define LJ_NO_ERR					0
 
 #define LJ_ERR_EVENT_TYPE			1
 #define LJ_ERR_PEND_ISR			2
 #define LJ_ERR_POST_NULL_PTR		3
 #define LJ_ERR_PEVENT_NULL			4
 #define LJ_ERR_POST_ISR			5
 #define LJ_ERR_QUERY_ISR			6
 #define LJ_ERR_INVALID_OPT			7
 #define LJ_ERR_TASK_WAITING		8
 
 #define LJ_TIMEOUT					10
 #define LJ_TASK_NOT_EXIST			11
 
 #define LJ_MBOX_FULL				20
 
 #define LJ_Q_FULL					30
 
 #define LJ_PRIO_EXIST				40
 #define LJ_PRIO_ERR				41
 #define LJ_PRIO_INVALID			42
 
 #define LJ_SEM_OVF					50
 
 #define LJ_TASK_DEL_ERR			60
 #define LJ_TASK_DEL_IDLE			61
 #define LJ_TASK_DEL_REQ			62
 #define LJ_TASK_DEL_ISR			63
 
 #define LJ_NO_MORE_TCB				70
 
 #define LJ_TIME_NOT_DLY			80
 #define LJ_TIME_INVALID_MINUTES	81
 #define LJ_TIME_INVALID_SECONDS	82
 #define LJ_TIME_INVALID_MILLI		83
 #define LJ_TIME_ZERO_DLY			84
 
 #define LJ_TASK_SUSPEND_PRIO		90
 #define LJ_TASK_SUSPEND_IDLE		91
 
 #define LJ_TASK_RESUME_PRIO		100
 #define LJ_TASK_NOT_SUSPENDED		101
 
 #define LJ_ERR_NOT_MUTEX_OWNER		120
 
 #define LJ_TASK_OPT_ERR			130
 
 #define LJ_ERR_DEL_ISR				140
 #define LJ_ERR_CREATE_ISR			141
 
 #define LJ_MEM_INVALID_PART		110
 #define LJ_MEM_INVALID_BLKS		111
 #define LJ_MEM_INVALID_SIZE		112
 #define LJ_MEM_NO_FREE_BLKS		113
 #define LJ_MEM_FULL				114
 #define LJ_MEM_INVALID_PBLK		115
 #define LJ_MEM_INVALID_PMEM		116
 #define LJ_MEM_INVALID_PDATA		117
 #define LJ_MEM_INVALID_ADDR		118

/*======================================================堆栈大小管理=====================================================*/
 
 #ifndef LJ_TASK_IDLE_STK_SIZE
 #error  "lj_cfg.h, Missing LJ_TASK_IDLE_STK_SIZE: Idle task stack size"
 #endif
 
/*========================================================杂项管理=======================================================*/

 #ifndef LJ_MAX_EVENTS
 #error  "lj_cfg.h, Missing LJ_MAX_EVENTS: Max. number of event control blocks in your application"
 #else
 	#if  LJ_MAX_EVENTS <= 0
 	#error "lj_cfg.h, LJ_MAX_EVENTS must be > 0"
 	#endif
 	
 	#if  LJ_MAX_EVENTS > 255
 	#error "lj_cfg.h, LJ_MAX_EVENTS must be <= 255"
 	#endif
 #endif
 
 #ifndef LJ_LOWEST_PRIO
 #error "lj_cfg.h, Missing LJ_LOWEST_PRIO: Defines the lowest priority that can be assigned"
 #endif
 
 #ifndef LJ_ARG_CHK_EN
 #error "lj_cfg.h, Missing LJ_ARG_CHK_EN: Enable (1) or Disable (0) argument checking"
 #endif
 
 #ifndef LJ_SCHED_LOCK_EN
 #error "lj_cfg.h, Missing LJ_SCHED_LOCK_EN: Include code for LJSchedLock() and LJSchedUnlock()"
 #endif
 
/*========================================================任务管理======================================================*/

 #ifndef LJ_MAX_TASKS 
 #error  "lj_cfg.h, Missing LJ_MAX_TASKS: Max. number of tasks in your application"
 #else
 	#if  LJ_MAX_TASKS < 2
 	#error "lj_cfg.h, LJ_MAX_TASKS must be >= 2"
 	#endif
 	
 	#if  LJ_MAX_TASKS > 63
 	#error "lj_cfg.h, LJ_MAX_TASKS must be <= 63"
 	#endif
 #endif
 
 #ifndef LJ_TASK_CHANGE_PRIO_EN
 #error  "lj_cfg.h, Missing LJ_TASK_CHANGE_PRIO_EN: Include code for LJTaskChangePrio()"
 #endif
 
 #ifndef LJ_TASK_CREATE_EN
 #error  "lj_cfg.h, Missing LJ_TASK_CREATE_EN: Include code for LJTaskCreate()"
 #endif
 
 #ifndef LJ_TASK_CREATE_EXT_EN
 #error  "lj_cfg.h, Missing LJ_TASK_CREATE_EXT_EN: Include code for LJTaskCreateExt()"
 #endif
 
 #ifndef LJ_TASK_DEL_EN
 #error  "lj_cfg.h, Missing LJ_TASK_DEL_EN: Include code for LJTaskDel()"
 #endif
 
 #ifndef LJ_TASK_SUSPEND_EN
 #error  "lj_cfg.h, Missing LJ_TASK_SUSPEND_EN: Include code for LJTaskSuspend() and LJTaskResuem()"
 #endif
 
 #ifndef LJ_TASK_QUERY_EN
 #error  "lj_cfg.h, Missing LJ_TASK_QUERY_EN: Include code for LJTaskQuery()"
 #endif
 
/*========================================================时间管理=====================================================*/

 #ifndef LJ_TICKS_PER_SEC
 #error  "lj_cfg.h, Missing LJ_TICKS_PER_SEC: Set the number of ticks in one second"
 #endif
 
 #ifndef LJ_TIME_DLY_HMSM_EN
 #error  "lj_cfg.h, Missing LJ_TIME_DLY_HMSM_EN: Include code for LJTimeDlyHMSM()"
 #endif
 
 #ifndef LJ_TIME_DLY_RESUME_EN
 #error  "lj_cfg.h, Missing LJ_TIME_DLY_RESUME_EN: Include code for LJTimeDlyResume()"
 #endif
 
 #ifndef LJ_TIME_GET_SET_EN
 #error  "lj_cfg.h, Missing LJ_TIME_GET_SET_EN: Include code for LJTimeGet() and LJTimeSet()"
 #endif
 
/*=======================================================信号量管理====================================================*/
 
 #ifndef LJ_SEM_EN
 #error  "lj_cfg.h, Missing LJ_SEM_EN: Enable (1) or Disable (0) code generation for SEMAPHORES"
 #else
 	#ifndef LJ_SEM_ACCEPT_EN
 	#error  "lj_cfg.h, Missing LJ_SEM_ACCEPT_EN: Include code for LJSemAccept()"
 	#endif
 	
 	#ifndef LJ_SEM_DEL_EN
 	#error  "lj_cfg.h, Missing LJ_SEM_DEL_EN: Include code for LJSemDel()"
 	#endif
 	
 	#ifndef LJ_SEM_QUERY_EN
 	#error  "lj_cfg.h, Missing LJ_SEM_QUERY_EN: Include code for LJSemQuery()"
 	#endif
 #endif
 
/*====================================================互斥型信号量管理=================================================*/
 
 #ifndef LJ_MUTEX_EN
 #error  "lj_cfg.h, Missing LJ_MUTEX_EN: Enable (1) or Disable (0) code generation for MUTEX"
 #else
 	#ifndef LJ_MUTEX_ACCEPT_EN
 	#error  "lj_cfg.h, Missing LJ_MUTEX_ACCEPT_EN: Include code for LJMutexAccept()"
 	#endif
 	
 	#ifndef LJ_MUTEX_DEL_EN
 	#error  "lj_cfg.h, Missing LJ_MUTEX_DEL_EN: Include code for LJMutexDel()"
 	#endif
 	
 	#ifndef LJ_MUTEX_QUERY_EN
 	#error  "lj_cfg.h, Missing LJ_MUTEX_QUERY_EN: Include code for LJMutexQuery()"
 	#endif
 #endif
 
/*========================================================消息邮箱管理==================================================*/
 
 #ifndef LJ_MBOX_EN
 #error  "lj_cfg.h, Missing LJ_MBOX_EN: Enable (1) or Disable (0) code generation for MAILBOXES"
 #else
 	#ifndef LJ_MBOX_ACCEPT_EN
 	#error  "lj_cfg.h, Missing LJ_MBOX_ACCEPT_EN: Include code for LJMboxAccept()"
 	#endif
 	
 	#ifndef LJ_MBOX_DEL_EN
 	#error  "lj_cfg.h, Missing LJ_MBOX_DEL_EN: Include code for LJMboxDel()"
 	#endif
 	
 	#ifndef LJ_MBOX_POST_EN
 	#error  "lj_cfg.h, Missing LJ_MBOX_POST_EN: Include code for LJMboxPost()"
 	#endif
 	
 	#ifndef LJ_MBOX_QUERY_EN
 	#error  "lj_cfg.h, Missing LJ_MBOX_QUERY_EN: Include code for LJMboxQuery()"
 	#endif
 #endif
 
/*=======================================================消息队列管理==================================================*/
 
 #ifndef LJ_Q_EN
 #error  "lj_cfg.h, Missing LJ_Q_EN: Enable (1) or Disable (0) code gerneration for QUEUES"
 #else
 	#ifndef LJ_MAX_QS
 	#error  "lj_cfg.h, Missing LJ_MAX_QS: Max. number of queue control blocks"
 	#else
 		#if LJ_MAX_QS <= 0
 		#error "lj_cfg.h, LJ_MAX_QS must be > 0"
 		#endif
 		
 		#if LJ_MAX_QS > 255
 		#error "lj_cfg.h, LJ_MAX_QS must be <= 255"
 		#endif
 	#endif
 	
 	#ifndef LJ_Q_ACCEPT_EN
 	#error  "lj_cfg.h, Missing LJ_Q_ACCEPT_EN: Include code for LJQAccept()"
 	#endif
 	
 	#ifndef LJ_Q_DEL_EN
 	#error  "lj_cfg.h, Missing LJ_Q_DEL_EN: Include code for LJQDel()"
 	#endif
 	
 	#ifndef LJ_Q_FLUSH_EN
 	#error  "lj_cfg.h, Missing LJ_Q_FLUSH_EN: Include code for LJQFlush()"
 	#endif
 	
 	#ifndef LJ_Q_POST_EN
 	#error  "lj_cfg.h, Missing LJ_Q_POST_EN: Include code for LJQPost()"
 	#endif
 	
 	#ifndef LJ_Q_QUERY_EN
 	#error  "lj_cfg.h, Missing LJ_Q_QUERY_EN: Include code for LJQQuery()"
 	#endif
 #endif
 
 
/*=========================================================内存管理=====================================================*/
 
 #ifndef LJ_MEM_EN
 #error  "lj_cfg.h, Missing LJ_MEM_EN: Enable (1) or Disable (0) code generation for MEMEORY MANAGER"
 #else
 	#ifndef LJ_MAX_MEM_PART
 	#error  "lj_cfg.h, Missing LJ_MAX_MEM_PART: Max. number of memory partitions"
 	#else
 		#if LJ_MAX_MEM_PART <= 0
 		#error "lj_cfg.h, LJ_MAX_MEM_PART must be > 0"
 		#endif
 		
 		#if LJ_MAX_MEM_PART > 255
 		#error "lj_cfg.h, LJ_MAX_MEM_PART must be <= 255"
 		#endif
 	#endif
 	
 	#ifndef LJ_MEM_QUERY_EN
 	#error "lj_cfg.h, Missing LJ_MEM_QUERY_EN: Include code for LJMemQuery()"
 	#endif
 #endif
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* __LJ_ERR_H */ 