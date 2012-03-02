/*
 *==============================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_rtos.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明LJ_RTOS使用的常量和函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==============================================================================================================
 */
 
 #ifndef __LJ_RTOS_H
 #define __LJ_RTOS_H
 
 #ifdef __cplusplus
 extern "C"
 #endif
 
 #include <lj_cfg.h>											/* 与用户应用程序配置有关的头文件               */
 #include <lj_cpu.h>											/* 与CPU和编译器有关的头文件                    */
 #include <lj_err.h>											/* LJ_RTOS错误码声明函数                        */
 
 #include <string.h>
 											
 
/*==================================================版本号======================================================*/

 #define LJ_VERSION					0							/* 内核版本号 = 0.0 * 100                       */
 
/*=================================================杂项声明=====================================================*/

 #ifndef LJ_FALSE
 #define LJ_FALSE					0
 #endif
 
 #ifndef LJ_TRUE
 #define LJ_TRUE					1
 #endif
 
 #define LJ_ASCII_NUL				0
 
 #ifndef LJ_GLOBAL
 #define LJ_EXT		extern
 #else
 #define LJ_EXT
 #endif
 
 #define LJ_PRIO_SELF				0xff						/* 声明自身优先级                               */
 
 #define LJ_RDY_TBL_SIZE	(LJ_LOWEST_PRIO / 8 + 1)			/* 就绪任务的组数，根据用户自行配置的           */
 																/* 最小优先级任务算出，可节省空间的使用         */
 #define LJ_EVENT_TBL_SIZE	(LJ_LOWEST_PRIO / 8 + 1)			/* 等待事件任务的组数                           */
 															
 #define LJ_N_SYS_TASKS		1									/* LJ_RTOS中的系统任务个数                      */
 
 #define LJ_IDLE_PRIO		(LJ_LOWEST_PRIO)					/* 空闲任务的优先级(系统任务)                   */
 
 #define LJ_TASK_IDLE_ID	65535								/* 空闲任务的ID                                 */
 
 #define LJ_EVENT_EN ((LJ_SEM_EN > 0) || LJ_MUTEX_EN > 0 || (LJ_MBOX_EN > 0) || ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)))
 
/*=================================================任务状态=====================================================*/

 #define LJ_STAT_RDY			0x00							/* 就绪态                                       */
 #define LJ_STAT_SEM			0x01							/* 等待一个信号量                               */
 #define LJ_STAT_MUTEX			0x02							/* 等待一个互斥信号量                           */
 #define LJ_STAT_MBOX			0x04							/* 等待一个消息邮箱                             */
 #define LJ_STAT_Q				0x08							/* 等待消息队列中的一则消息                     */
 #define LJ_STAT_SUSPEND		0x10							/* 任务被挂起                                   */
 
/*=================================================事件类型=====================================================*/
 
 #define LJ_EVENT_TYPE_UNUSED	0								/* 未使用                                       */
 #define LJ_EVENT_TYPE_SEM		1								/* 信号量                                       */
 #define LJ_EVENT_TYPE_MUTEX	2								/* 互斥型信号量                                 */
 #define LJ_EVENT_TYPE_MBOX		3								/* 消息邮箱                                     */
 #define LJ_EVENT_TYPE_Q		4								/* 消息队列                                     */
 
/*===============================================删除事件选项===================================================*/

 #define LJ_DEL_NO_PEND			0								/* 没有任务等待事件                             */
 #define LJ_DEL_ALWAYS			1								/* 所有等待该事件的任务都进入就绪态             */
 
/*
 *===============================================================================================================
 *                                              LJ???Post()的选项
 *
 * 下列定义的选项用于这两个函数：LJMboxPost() 和 LJQPost()
 *===============================================================================================================
 */
 #define LJ_POST_OPT_NONE		0x00							/* 向等待任务列表中的HPT任务传递消息(FIFO)      */
 #define LJ_POST_OPT_BROADCAST	0x01							/* 向等待任务列表中的所有任务广播消息           */
 #define LJ_POST_OPT_FRONT		0x02							/* 向等待任务列表中的HPT任务传递消息(LIFO)      */
 
/*=================================================任务选项=====================================================*/

 #define LJ_TASK_OPT_STK_CHK	0x0001							/* 允许任务堆栈检查                             */
 #define LJ_TASK_OPT_STK_CLR	0x0002							/* 清空任务堆栈                                 */
 
/*================================================事件控制块====================================================*/

 #if (LJ_EVENT_EN > 0) && (LJ_MAX_EVENTS > 0)
 
 typedef struct lj_event
 {
 	INT8U	 ljEventType;										/* 事件类型                                     */
 	INT8U	 ljEventGrp;										/* 等待任务所在的组                             */
 	INT16U	 ljEventCnt;										/* 信号量计数器                                 */
 	void	*ljEventPtr;										/* 指向消息或者消息队列的指针                   */
 	INT8U	 ljEventTbl[LJ_EVENT_TBL_SIZE];						/* 等待任务列表                                 */
 	
 } LJ_EVENT;
 
 #endif
 
/*================================================信号量数据====================================================*/

 #if LJ_SEM_EN > 0
 
 typedef struct lj_sem_data
 {
 	INT16U	ljCnt;												/* 信号量计数器                                 */
 	INT8U	ljEventGrp;											/* 等待任务所在的组                             */
 	INT8U	ljEventTbl[LJ_EVENT_TBL_SIZE];						/* 等待任务列表                                 */
 	
 } LJ_SEM_DATA;
 
 #endif
 
/*=============================================互斥型信号量数据=================================================*/

 #if LJ_MUTEX_EN > 0
 
 typedef struct lj_mutex_data
 {
 	INT8U	ljEventTbl[LJ_EVENT_TBL_SIZE];						/* 等待任务列表                                 */
 	INT8U	ljEventGrp;											/* 等待任务所在的组                             */
 	INT8U	ljValue;											/* 互斥型信号量值(0 = 已被使用, 1 = 可使用的)   */
 	INT8U	ljOwnerPrio;										/* 占用互斥量的任务优先级，0xff表示没有任务占用 */
 	INT8U	ljMutexPIP;											/* 优先级继承优先级，略高于最高HPT任务的优先级  */
 	
 } LJ_MUTEX_DATA;
 
 #endif
 
/*===============================================消息邮箱数据===================================================*/

 #if LJ_MBOX_EN > 0
 
 typedef struct lj_mbox_data
 {
 	void	*ljMsg;												/* 指向消息邮箱中消息的指针                     */
 	INT8U	 ljEventTbl[LJ_EVENT_TBL_SIZE];						/* 等待任务列表                                 */
 	INT8U	 ljEventGrp;										/* 等待任务所在的组                             */
 	
 } LJ_MBOX_DATA;
 
 #endif
 
/*===============================================消息队列数据===================================================*/

 #if LJ_Q_EN > 0
 
 typedef struct lj_q											/* 消息队列控制块                               */
 {
 	struct	lj_q	 *ljQPtr;									/* 在空闲队列控制块链表中指向下一个队列控制块   */
 	void			**ljQStart;									/* 指向消息队列的指针数组的起始地址的指针       */
 	void			**ljQEnd;									/* 指向消息队列结束单元的下一个地址的指针       */
 	void			**ljQIn;									/* 指向消息队列中插入下一条消息的位置的指针     */
 	void			**ljQOut;									/* 指向消息队列中下一个取出消息的位置的指针     */
 	INT16U			  ljQSize;									/* 消息队列中可容纳的总的消息数                 */
 	INT16U			  ljQEntries;								/* 消息队列中当前的消息数                       */
 	
 } LJ_Q;
 
 typedef struct lj_q_data
 {
 	void			 *ljMsg;									/* 指向消息队列中下一个取出消息的位置的指针     */
 	INT16U			  ljNMsgs;									/* 消息队列中的消息数目                         */
 	INT16U			  ljQSize;									/* 消息队列的总容量                             */
 	INT8U			  ljEventTbl[LJ_EVENT_TBL_SIZE];			/* 等待任务列表                                 */
 	INT8U			  ljEventGrp;								/* 等待任务所在的组                             */
 	
 } LJ_Q_DATA;
 
 #endif
 
/*===============================================任务堆栈数据===================================================*/

 #if LJ_TASK_CREATE_EXT_EN > 0
 
 typedef struct lj_stk_data
 {
 	INT32U		ljFree;											/* 堆栈已经被占用的字节数                       */
 	INT32U		ljUsed;											/* 堆栈空闲字节数                               */
 	
 } LJ_STK_DATA;
 
 #endif 
 
 
/*================================================任务控制块====================================================*/

 typedef struct lj_tcb											/* 任务控制块                                   */
 {
 	LJ_STK			*ljTcbStkTop;								/* 指向当前任务堆栈栈顶的指针                   */
 	
 #if LJ_TASK_CREATE_EXT_EN > 0
 	void			*ljTcbExtPtr;								/* 指向用户定义的任务控制块扩展                 */
 	LJ_STK			*ljTcbStkBottom;							/* 指向任务堆栈栈底的指针                       */
 	INT32U			 ljTcbStkSize;								/* 存有栈中可容纳的指针元数目                   */
 	INT16U			 ljTcbOpt;									/* 选择项：LJ_TASK_OPT_STK_CHK(任务堆栈检验)    */
 																/*         LJ_TASK_OPT_STK_CLR(清空任务堆栈)    */
 	INT16U			 ljTcbId;									/* 用于存储任务的识别码                         */
 #endif
 	
 	struct lj_tcb	*ljTcbNext;									/* ljTcbNext和ljTcbPrev用于任务控制块           */
 	struct lj_tcb	*ljTcbPrev; 								/* LJ_TCB双向链表的前后链接                     */
 	
 #if LJ_EVENT_EN
 	LJ_EVENT		*ljTcbEventPtr;								/* 指向事件控制块的指针                         */
 #endif
 
 #if ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)) || (LJ_MBOX_EN > 0)
 	void			*ljTcbMsg;									/* 指向传递给任务的消息的指针                   */
 #endif
 	
 	INT16U			 ljTcbDly;									/* 用于任务延时的变量                           */
 	INT8U			 ljTcbStat;									/* 当前任务的状态                               */
 	INT8U			 ljTcbPrio;									/* 当前任务的优先级：min = 63, max = 0          */
 	
 	INT8U			 ljTcbX;									/* 以下4个值用于加速任务进入就绪态              */
 	INT8U			 ljTcbY;									/* 或进入等待事件发生状态的过程的               */
 	INT8U			 ljTcbBitX;									/* 这些值在任务建立时算好的，避免在运行         */
 	INT8U			 ljTcbBitY;									/* 中计算这些值                                 */
 	
 #if LJ_TASK_DEL_EN > 0
 	BOOLEAN			 ljTcbDelReq;								/* 用于表示该任务是否须删除的布尔变量           */
 #endif
 	
 } LJ_TCB;

/*==============================================内存分区数据====================================================*/

 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 typedef struct lj_mem											/* 内存控制块                                   */
 {
 	void			*ljMemAddr;									/* 指向内存分区起始地址的指针                   */
 	void			*ljMemFreeList;								/* 指向下一个空余内存控制块                     */
 	INT32U			 ljMemBlkSize;								/* 内存分区中内存块的大小                       */
 	INT32U			 ljMemNBlks;								/* 内存分区中总的内存块数量                     */
 	INT32U			 ljMemNFree;								/* 内存分区中当前可以获得的空余内存块的数量     */
 	
 } LJ_MEM;
 
 typedef struct lj_mem_data
 {
 	void			*ljAddr;									/* 指向内存分区首地址的指针                     */
 	void			*ljFreeList;								/* 指向空余内存块链表首地址的指针               */
 	INT32U			 ljBlkSize;									/* 每个内存块所含的字节数                       */
 	INT32U			 ljNBlks;									/* 内存分区总的内存块数                         */
 	INT32U			 ljNFree;									/* 空余内存块数                                 */
 	INT32U			 ljNUsed;									/* 正在使用的内存块数                           */
 	
 } LJ_MEM_DATA;
 
 #endif
 
/*================================================全局变量======================================================*/
 
 LJ_EXT INT32U				 gLJCtxSwCtr;						/* 任务切换次数计数器                           */
 LJ_EXT INT8U				 gLJIntNesting;						/* 中断嵌套层数                                 */
 LJ_EXT INT8U				 gLJIntExitY;
 LJ_EXT INT8U				 gLJLockNesting;					/* 任务锁定层数                                 */
 
 LJ_EXT INT8U				 gLJPrioCur;						/* 当前任务优先级                               */
 LJ_EXT INT8U				 gLJPrioHighRdy;					/* 最高优先级任务优先级                         */
 
 LJ_EXT INT8U				 gLJRdyGrp;							/* 以下两个变量为就绪表                         */
 LJ_EXT INT8U				 gLJRdyTbl[LJ_RDY_TBL_SIZE];
 
 LJ_EXT BOOLEAN				 gLJRunning;						/* 标记LJ_RTOS是否在运行                        */
 
 LJ_EXT INT8U				 gLJTaskCtr;						/* 已经创建的任务的个数                         */
 
 LJ_EXT LJ_TCB				*gLJTcbCur;							/* 指向当前运行中的任务的任务控制块指针         */
 LJ_EXT LJ_TCB				*gLJTcbFreeList;					/* 指向空闲任务控制块列表的指针                 */
 LJ_EXT LJ_TCB				*gLJTcbHighRdy;						/* 指向最高优先级任务控制块的指针               */
 LJ_EXT LJ_TCB				*gLJTcbList;						/* 指向任务控制块列表双链表的指针               */
 LJ_EXT LJ_TCB				*gLJTcbPrioTbl[LJ_LOWEST_PRIO + 1];	/* 已创建任务控制块指针数组                     */
 LJ_EXT LJ_TCB				 gLJTcbTbl[LJ_MAX_TASKS + LJ_N_SYS_TASKS];	/* 任务控制块数组                       */
 
 #if (LJ_EVENT_EN > 0) && (LJ_MAX_EVENTS > 0)
 LJ_EXT LJ_EVENT			*gLJEventFreeList;					/* 指向空余事件控制块链表的指针                 */
 LJ_EXT LJ_EVENT			 gLJEventTbl[LJ_MAX_EVENTS];		/* 空余事件控制块数组                           */
 #endif
 
 #if (LJ_Q_EN > 0) && (LJ_MAX_QS > 0)
 LJ_EXT	LJ_Q				*gLJQFreeList;						/* 指向空闲队列控制块链表                       */
 LJ_EXT	LJ_Q	 			 gLJQTbl[LJ_MAX_QS];				/* 消息队列控制块数组                           */
 #endif
 
 #if (LJ_MEM_EN > 0) && (LJ_MAX_MEM_PART > 0)
 LJ_EXT	LJ_MEM				*gLJMemFreeList;					/* 指向空余内存控制块链表                       */
 LJ_EXT	LJ_MEM				 gLJMemTbl[LJ_MAX_MEM_PART];		/* 内存分区控制块数组                           */
 #endif
 
 #if LJ_TIME_GET_SET_EN > 0
 LJ_EXT volatile INT32U		 gLJTime;							/* 当前系统时间(用系统时钟节拍数表示)           */
 #endif
 
 LJ_EXT LJ_STK				 gLJTaskIdleStk[LJ_TASK_IDLE_STK_SIZE];		/* 空闲任务堆栈                         */
 LJ_EXT volatile INT32U		 gLJIdleCtr;						/* 空闲任务计数器                               */
 
 extern INT8U const			 gLJMapTbl[];						/* 优先级->位掩码  查表                         */
 extern INT8U const			 gLJUnMapTbl[];						/* 优先级->索引    查表                         */
 
 
/*============================================任务管理函数原型==================================================*/

 #if LJ_TASK_CREATE_EN > 0
 INT8U   LJTaskCreate( void (*task)( void *pd ), void *pdata, LJ_STK *ptos, INT8U prio );
 #endif
 
 #if LJ_TASK_CREATE_EXT_EN > 0
 INT8U	 LJTaskCreateExt( void		(*task)( void *pd ),
 						  void		 *pdata,
 						  LJ_STK	 *ptos,
 						  INT8U		  prio,
 						  INT16U	  id,
 						  LJ_STK	 *pbos,
 						  INT32U	  stkSize,
 						  void		 *pext,
 						  INT16U	  opt );
 #endif
 
 #if LJ_TASK_CHANGE_PRIO_EN > 0
 INT8U	 LJTaskChangePrio( INT8U oldPrio, INT8U newPrio );
 #endif

 #if LJ_TASK_CREATE_EXT_EN > 0
 INT8U	 LJTaskStkChk( INT8U prio, LJ_STK_DATA *pdata );
 #endif
 
 #if LJ_TASK_DEL_EN > 0
 INT8U	 LJTaskDel( INT8U prio );
 INT8U	 LJTaskDelReq( INT8U prio );
 #endif
 
 #if LJ_TASK_SUSPEND_EN > 0
 INT8U	 LJTaskSuspend( INT8U prio );
 INT8U	 LJTaskResume( INT8U prio );
 #endif
 
 #if LJ_TASK_QUERY_EN > 0
 INT8U	 LJTaskQuery( INT8U prio, LJ_TCB *pdata );
 #endif
 
/*================================================时间管理======================================================*/

 void    LJTimeTick( void );
 void	 LJTimeDly( INT16U ticks );
 
 #if LJ_TIME_DLY_HMSM_EN > 0
 INT8U	 LJTimeDlyHMSM( INT8U hours, INT8U minutes, INT8U seconds, INT16U milli );
 #endif
 
 #if LJ_TIME_DLY_RESUME_EN > 0
 INT8U	 LJTimeDlyResume( INT8U prio );
 #endif
 
 #if LJ_TIME_GET_SET_EN > 0
 INT32U	 LJTimeGet( void );
 void	 LJTimeSet( INT32U ticks );
 #endif

/*===============================================信号量管理=====================================================*/

 #if LJ_SEM_EN > 0
 
 LJ_EVENT	*LJSemCreate( INT16U cnt );
 
 void		 LJSemPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 INT8U		 LJSemPost( LJ_EVENT *pevent );
 
 #if LJ_SEM_DEL_EN > 0
 LJ_EVENT	*LJSemDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_SEM_ACCEPT_EN > 0
 INT16U		 LJSemAccept( LJ_EVENT *pevent );
 #endif
 
 #if LJ_SEM_QUERY_EN > 0
 INT8U		 LJSemQuery( LJ_EVENT *pevent, LJ_SEM_DATA *pdata );
 #endif
 
 #endif
 
/*============================================互斥型信号量管理==================================================*/
 
 #if LJ_MUTEX_EN > 0
 
 LJ_EVENT	*LJMutexCreate( INT8U prio, INT8U *err );
 
 void		 LJMutexPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 INT8U		 LJMutexPost( LJ_EVENT *pevent );
 
 #if LJ_MUTEX_DEL_EN > 0
 LJ_EVENT	*LJMutexDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_MUTEX_ACCEPT_EN > 0
 INT8U		 LJMutexAccept( LJ_EVENT *pevent, INT8U *err );
 #endif
 
 #if LJ_MUTEX_QUERY_EN > 0
 INT8U		 LJMutexQuery( LJ_EVENT *pevent, LJ_MUTEX_DATA *pdata );
 #endif
 
 #endif
 
/*==============================================消息邮箱管理====================================================*/

 #if LJ_MBOX_EN > 0
 
 LJ_EVENT	*LJMboxCreate( void *msg );
 
 void		*LJMboxPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 #if LJ_MBOX_POST_EN > 0
 INT8U		 LJMboxPost( LJ_EVENT *pevent, void *msg, INT8U opt );
 #endif
 
 #if LJ_MBOX_DEL_EN > 0
 LJ_EVENT	*LJMboxDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_MBOX_ACCEPT_EN > 0
 void		*LJMboxAccept( LJ_EVENT *pevent );
 #endif
 
 #if LJ_MBOX_QUERY_EN > 0
 INT8U		 LJMboxQuery( LJ_EVENT *pevent, LJ_MBOX_DATA *pdata );
 #endif
 
 #endif
 
/*==============================================消息队列管理====================================================*/

 #if LJ_Q_EN > 0
 
 LJ_EVENT	*LJQCreate( void **start, INT16U size );
 
 void		*LJQPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 #if LJ_Q_POST_EN > 0
 INT8U		 LJQPost( LJ_EVENT *pevent, void *msg, INT8U opt );
 #endif
 
 #if LJ_Q_DEL_EN > 0
 LJ_EVENT	*LJQDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_Q_ACCEPT_EN > 0
 void		*LJQAccept( LJ_EVENT *pevent );
 #endif
 
 #if LJ_Q_FLUSH_EN > 0
 INT8U		 LJQFlush( LJ_EVENT *pevent );
 #endif
 
 #if LJ_Q_QUERY_EN > 0
 INT8U		 LJQQuery( LJ_EVENT *pevent, LJ_Q_DATA *pdata );
 #endif
 
 #endif
 
/*================================================内存管理======================================================*/

 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 LJ_MEM		*LJMemCreate( void *addr, INT32U nblks, INT32U blkSize, INT8U *err );
 void		*LJMemGet( LJ_MEM *pmem, INT8U *err );
 INT8U		 LJMemPut( LJ_MEM *pmem, void *pblk );
 
 #if LJ_MEM_QUERY_EN > 0
 INT8U		 LJMemQuery( LJ_MEM *pmem, LJ_MEM_DATA *pdata );
 #endif
 
 #endif
 
/*==============================================杂项函数原型====================================================*/

 void    LJInit( void );
 void    LJStart( void );
 
 void	 LJIntEnter( void );
 void	 LJIntExit( void );
 
 #if LJ_SCHED_LOCK_EN > 0
 void	 LJSchedLock( void );
 void	 LJSchedUnlock( void );
 #endif
 
 INT16U	 LJVersion( void );
 
/*==============================================内部函数原型====================================================*/

 INT8U   LJ_TcbInit( INT8U prio, LJ_STK *ptos, LJ_STK *pbos, INT16U id, INT32U stkSize, void *pext, INT16U opt );
 void    LJ_Sched( void );
 void	 LJ_TaskIdle( void *pdata );
 
 #if LJ_TASK_DEL_EN > 0
 void	 LJ_Dummy( void );
 #endif
 
 #if LJ_EVENT_EN > 0
 void	 LJ_EventWaitListInit( LJ_EVENT *pevent );
 INT8U	 LJ_EventTaskRdy( LJ_EVENT *pevent, void *msg, INT8U msk );
 void	 LJ_EventTaskWait( LJ_EVENT *pevent );
 void	 LJ_EventTo( LJ_EVENT *pevent );
 #endif
 
 #if LJ_Q_EN > 0
 void	 LJ_QInit( void );
 #endif
 
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 void	 LJ_MemInit( void );
 #endif
 
/*============================================与CPU有关函数原型=================================================*/

 LJ_STK *LJTaskStkInit( void (*task)( void *pd ), void *pdata, LJ_STK *ptos, INT16U opt );	/* c语言实现        */
 
 void    LJStartHighRdy( void );												/* 汇编语言实现(lj_cpu_asm.s)   */
 void	 LJCtxSw( void );														/* 汇编语言实现(lj_cpu_asm.s)   */
 void	 LJTickIsr( void );														/* 汇编语言实现(lj_cpu_asm.s)   */
 void	 LJIntCtxSw( void );													/* 汇编语言实现(lj_cpu_asm.s)   */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_RTOS_H */