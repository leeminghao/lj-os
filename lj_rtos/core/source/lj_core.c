/*
 *==================================================================================================
 *                                          LJ_RTOS
 *                                    The Real-Time Kernel
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_core.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于定义LJ_RTOS的核心代码
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==================================================================================================
 */
 
 #define LJ_GLOBAL									/* 在本文件中对全局变量进行定义                 */
 #include <lj_rtos.h>
 
/*
 *==================================================================================================
 * 使任务进入就绪态的掩码表
 *==================================================================================================
 */
 INT8U  const  gLJMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
 
/*
 *==================================================================================================
 * 找出进入就绪态的最高优先级任务的掩码表
 *==================================================================================================
 */
 INT8U  const  gLJUnMapTbl[] = 
 {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 ~ 0x0F                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 ~ 0x1F                            */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 ~ 0x2F                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 ~ 0x3F                            */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 ~ 0x4F                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 ~ 0x5F                            */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 ~ 0x6F                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 ~ 0x7F                            */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 ~ 0x8F                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 ~ 0x9F                            */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 ~ 0xAF                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 ~ 0xBF                            */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 ~ 0xCF                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 ~ 0xDF                            */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 ~ 0xEF                            */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 ~ 0xFF                            */
 };
 
/*========================================LJ_RTOS函数原型===========================================*/

 static void LJ_InitMisc( void );
 static void LJ_InitRdyList( void );
 static void LJ_InitTcbList( void );
 static void LJ_InitTaskIdle( void );
 static void LJ_InitEventList( void );

/*
 *==================================================================================================
 * 函数名称：LJInit
 * 功能描述：初始化LJ_RTOS
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 void
 LJInit( void )
 {
 	LJ_InitMisc( );										/* 初始化全局变量                           */
 	
 	LJ_InitRdyList( );									/* 初始化就绪表                             */
 	LJ_InitTcbList( );									/* 初始化空闲任务控制块链表                 */
 	
 	LJ_InitTaskIdle( );									/* 初始化空闲任务                           */
 
 	LJ_InitEventList( );								/* 初始化空余事件控制块链表                 */
 	
 #if (LJ_Q_EN > 0) && (LJ_MAX_QS > 0)
 	LJ_QInit( );										/* 初始化空闲队列控制块链表                 */
 #endif
 
 #if (LJ_MEM_EN > 0) && (LJ_MAX_MEM_PART > 0)
 	LJ_MemInit( );										/* 初始化空余内存控制块链表                 */
 #endif
 }
 
/*
 *==================================================================================================
 * 函数名称：LJStart
 * 功能描述：开始多任务
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 void
 LJStart( void )
 {
 	INT8U	y;
 	INT8U	x;
 	
 	if ( gLJRunning == LJ_FALSE )
 	{
 														/* 查找就绪表中优先级最高的任务            	*/
 		y				= gLJUnMapTbl[gLJRdyGrp];		/* 查gLJRdyGrp中右起第几位首先为1           */
 		x				= gLJUnMapTbl[gLJRdyTbl[y]];	/* 再去gLJRdyTbl[y]中查找右起第几位首先为1  */
 		gLJPrioHighRdy	= (INT8U)((y << 3) + x);		/* 计算出优先级最高任务的优先级             */
 		gLJPrioCur		= gLJPrioHighRdy;
 		gLJTcbHighRdy	= gLJTcbPrioTbl[gLJPrioHighRdy];
 		gLJTcbCur		= gLJTcbHighRdy;
 		
 		LJStartHighRdy( );
 	}
 }
 
/*
 *==================================================================================================
 * 函数名称：LJIntEnter
 * 功能描述：通知LJ_RTOS中断服务子程序ISR开始
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 void
 LJIntEnter( void )
 {
 	if ( gLJRunning == LJ_TRUE )
 	{
 		if ( gLJIntNesting < 255 )
 		{
 			gLJIntNesting++;
 		}
 	}
 }
 
/*
 *==================================================================================================
 * 函数名称：LJIntExit
 * 功能描述：通知LJ_RTOS离开中断服务子程序ISR
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 void
 LJIntExit( void )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR		 cpuSR;
 #endif
 
 	LJ_ENTER_CRITICAL( );
 	if ( gLJRunning == LJ_TRUE )
 	{
 		if ( gLJIntNesting > 0 )
 		{
 			gLJIntNesting--;
 		}
 		
 		if ( (gLJIntNesting == 0) && (gLJLockNesting == 0) )
 		{
 			gLJIntExitY				= gLJUnMapTbl[gLJRdyGrp];	/* 查找就绪表中优先级最高的任务      */
 			gLJPrioHighRdy			= (INT8U)((gLJIntExitY << 3) + gLJUnMapTbl[gLJRdyTbl[gLJIntExitY]]);
 			
	 		if ( gLJPrioHighRdy != gLJPrioCur )
	 		{
	 			gLJTcbHighRdy = gLJTcbPrioTbl[gLJPrioHighRdy];
	 			gLJCtxSwCtr++;
	 			LJIntCtxSw( );						/* 通过调用LJIntCtxSw(), 在ISR中执行任务切换功能 */
	 		}
 		}
 	}
 	LJ_EXIT_CRITICAL( );
 }
 
/*
 *==================================================================================================
 * 函数名称：LJSchedLock
 * 功能描述：给调度器上锁
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 #if LJ_SCHED_LOCK_EN > 0
 
 void
 LJSchedLock( void )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	
 	if ( gLJRunning == LJ_TRUE )
 	{
 		LJ_ENTER_CRITICAL( );
 		if ( gLJLockNesting < 255 )
 		{
 			gLJLockNesting++;
 		}
 		LJ_EXIT_CRITICAL( );
 	}
 }
 
 #endif
 
/*
 *==================================================================================================
 * 函数名称：LJSchedUnlock
 * 功能描述：给调度器开锁
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 #if LJ_SCHED_LOCK_EN > 0
 
 void
 LJSchedUnlock( void )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 	if ( gLJRunning == LJ_TRUE )
 	{
 		LJ_ENTER_CRITICAL( );
 		
 		if ( gLJLockNesting > 0 )
 		{
 			gLJLockNesting--;
 			
 			if ( (gLJLockNesting == 0) && (gLJIntNesting == 0) )
 			{
 				LJ_EXIT_CRITICAL( );
 				LJ_Sched( );
 			}
 			else
 			{
 				LJ_EXIT_CRITICAL( );
 			}
 		}
 		else
 		{
 			LJ_EXIT_CRITICAL( );
 		}
 	}
 }
 
 #endif
 
/*
 *==================================================================================================
 * 函数名称：LJVersion
 * 功能描述：获取当前LJ_RTOS内核的版本号
 * 传    参：void
 * 返 回 值：LJ_VERSION (INT16U)：LJ_RTOS版本号 * 100
 *==================================================================================================
 */
 INT16U
 LJVersion( void )
 {
 	return (LJ_VERSION);
 }
 
/*
 *==================================================================================================
 * 函数名称：LJTimeTick
 * 功能描述：时钟节拍函数的节拍服务
 * 传    参：void
 * 返 回 值：void
 *==================================================================================================
 */
 void
 LJTimeTick( void )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_TCB		*ptcb;
 	
 #if LJ_TIME_GET_SET_EN > 0
 	LJ_ENTER_CRITICAL( );
 	gLJTime++;										/* 计算从系统上电以来的系统时钟节拍数           */
 	LJ_EXIT_CRITICAL( );
 #endif
 	
 	if ( gLJRunning == LJ_TRUE )
 	{
 		ptcb = gLJTcbList;
 													/* LJTimeTick()从gLJTcbList开始，沿着LJ_TCB链表 */
 		while ( ptcb->ljTcbPrio != LJ_IDLE_PRIO )	/* 做，一直做到空闲任务                         */
 		{
 			LJ_ENTER_CRITICAL( );
 			if ( ptcb->ljTcbDly != 0 )
 			{	
 				if ( --ptcb->ljTcbDly == 0 )
 				{
 					if ( (ptcb->ljTcbStat & LJ_STAT_SUSPEND) == 0x00 )
 					{
 						gLJRdyGrp				|= ptcb->ljTcbBitY;
 						gLJRdyTbl[ptcb->ljTcbY]	|= ptcb->ljTcbBitX;
 					}
 					else				/* 确实被挂起函数LJTaskSuspend()挂起的任务则不会进入就绪态  */
 					{
 						ptcb->ljTcbDly			 = 1;
 					}
 				}
 			}
 			ptcb = ptcb->ljTcbNext;
 			LJ_EXIT_CRITICAL( );
 		}
 	}
 }
 
/*
 *===================================================================================================
 * 函数名称：LJ_Sched
 * 功能描述：任务调度器
 * 传    参：void
 * 返 回 值：void
 *===================================================================================================
 */
 void
 LJ_Sched( void )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	cpuSR;
 #endif
 	INT8U		y;
 	
 	LJ_ENTER_CRITICAL( );
 	/* 
 	 * 如果调用来自中断服务子程序或者调用了一次给任务调度上锁函数，
 	 * 任务调度函数LJ_Sched将退出不做任务调度
 	 */
 	if ( (gLJIntNesting == 0) && (gLJLockNesting == 0) )
 	{
 		y				= gLJUnMapTbl[gLJRdyGrp];			/* 找出就绪态中优先级最高的任务         */
 		gLJPrioHighRdy	= (INT8U)((y << 3) + gLJUnMapTbl[gLJRdyTbl[y]]);
 		if ( gLJPrioHighRdy != gLJPrioCur )					/* 检查最高优先级任务是否是当前任务     */
 		{
 			gLJTcbHighRdy = gLJTcbPrioTbl[gLJPrioHighRdy];
 			gLJCtxSwCtr++;
 			LJ_TASK_SW( );									/* 宏调用，完成实际上的任务切换         */
 		}
 	}
 	LJ_EXIT_CRITICAL( );	
 }
 
/*
 *===================================================================================================
 * 函数名称：LJ_TaskIdle
 * 功能描述：空闲任务，系统在没有其他任务进入就绪态是投入运行
 * 传    参：pdata	(void *)：任务参数
 * 返 回 值：void
 *===================================================================================================
 */
 void
 LJ_TaskIdle( void *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	cpuSR;
 #endif	
 	
 	pdata		= pdata;
 	while ( 1 )
 	{
 		LJ_ENTER_CRITICAL( );
 		gLJIdleCtr++;
 		LJ_EXIT_CRITICAL( );
 	}
 }
 
/*
 *===================================================================================================
 * 函数名称：LJ_InitMisc
 * 功能描述：初始化LJ_RTOS中定义的全局变量
 * 传    参：void
 * 返 回 值：static void
 *===================================================================================================
 */
 static void
 LJ_InitMisc( void )
 {
 #if LJ_TIME_GET_SET_EN > 0
 	gLJTime			= 0;
 #endif
 
 	gLJIntNesting	= 0;
 	gLJLockNesting	= 0;
 	
 	gLJTaskCtr		= 0;
 	
 	gLJRunning		= LJ_FALSE;
 	
 	gLJCtxSwCtr		= 0;
 	gLJIdleCtr		= 0;
 }
 
/*
 *====================================================================================================
 * 函数名称：LJ_InitRdyList
 * 功能描述：初始化LJ_RTOS中的就绪表
 * 传    参：void
 * 返 回 值：static void
 *====================================================================================================
 */
 static void
 LJ_InitRdyList( void )
 {
 	INT16U		 i;
 	INT8U		*pRdyTbl;
 	
 	gLJRdyGrp		= 0x00;
 	pRdyTbl			= &gLJRdyTbl[0];
 	
 	for ( i = 0; i < LJ_RDY_TBL_SIZE; i++ )
 	{
 		*pRdyTbl++ = 0x00;
 	}
 	
 	gLJPrioCur		= 0;
 	gLJPrioHighRdy	= 0;
 	
 	gLJTcbHighRdy	= (LJ_TCB *)0;
 	gLJTcbCur		= (LJ_TCB *)0;
 }
 
/*
 *====================================================================================================
 * 函数名称：LJ_InitTcbList
 * 功能描述：初始化LJ_RTOS中的空闲任务控制块列表
 * 传    参：void
 * 返 回 值：static void
 *====================================================================================================
 */
 static void
 LJ_InitTcbList( void )
 {
 	INT8U		 i;
 	LJ_TCB		*ptcb1;
 	LJ_TCB		*ptcb2;
 	
 	gLJTcbList		= (LJ_TCB *)0;
 	for ( i = 0; i < (LJ_LOWEST_PRIO + 1); i++ )
 	{
 		gLJTcbPrioTbl[i] = (LJ_TCB *)0;
 	}
 	
 	ptcb1	= &gLJTcbTbl[0];
 	ptcb2	= &gLJTcbTbl[1];
 	for ( i = 0; i < (LJ_MAX_TASKS + LJ_N_SYS_TASKS - 1); i++ )
 	{
 		ptcb1->ljTcbNext = ptcb2;
 		ptcb1++;
 		ptcb2++;
 	}
 	ptcb1->ljTcbNext = (LJ_TCB *)0;
 	gLJTcbFreeList   = &gLJTcbTbl[0];
 }
 
/*
 *=====================================================================================================
 * 函数名称：LJ_InitTaskIdle
 * 功能描述：初始化空闲任务
 * 传    参：void
 * 返 回 值：static void
 *=====================================================================================================
 */
 static void
 LJ_InitTaskIdle( void )
 {
 #if LJ_TASK_CREATE_EXT_EN > 0
 	#if LJ_STK_GROWTH == 1
 	(void)LJTaskCreateExt( LJ_TaskIdle,
 	                       (void *)0,
 	                       &gLJTaskIdleStk[LJ_TASK_IDLE_STK_SIZE - 1],
 	                       LJ_IDLE_PRIO,
 	                       LJ_TASK_IDLE_ID,
 	                       &gLJTaskIdleStk[0],
 	                       LJ_TASK_IDLE_STK_SIZE,
 	                       (void *)0,
 	                       LJ_TASK_OPT_STK_CHK | LJ_TASK_OPT_STK_CLR );
 	#else
 	(void)LJTaskCreateExt( LJ_TaskIdle,
 	                       (void *)0,
 	                       &gLJTaskIdleStk[0],
 	                       LJ_IDLE_PRIO,
 	                       LJ_TASK_IDLE_ID,
 	                       &gLJTaskIdleStk[LJ_TASK_IDLE_STK_SIZE - 1],
 	                       LJ_TASK_IDLE_STK_SIZE,
 	                       (void *)0,
 	                       LJ_TASK_OPT_STK_CHK | LJ_TASK_OPT_STK_CLR );
 	#endif
 #else
 	#if LJ_STK_GROWTH == 1
 	(void)LJTaskCreate( LJ_TaskIdle,
 	                    (void *)0,
 	                    &gLJTaskIdleStk[LJ_TASK_IDLE_STK_SIZE - 1],
 	                    LJ_IDLE_PRIO );
 	#else
 	(void)LJTaskCreate( LJ_TaskIdle,
 	                    (void *)0,
 	                    &gLJTaskIdleStk[0],
 	                    LJ_IDLE_PRIO );
 	#endif
 #endif	
 }
 
/*
 *=====================================================================================================
 * 函数名称：LJ_InitEventList
 * 功能描述：初始化空余事件控制块链表
 * 传    参：void
 * 返 回 值：static void
 *=====================================================================================================
 */
 static void
 LJ_InitEventList( void )
 {
 #if (LJ_EVENT_EN > 0) && (LJ_MAX_EVENTS > 0)
 #if (LJ_MAX_EVENTS > 1)
 	INT16U		 i;
 	LJ_EVENT	*pevent1;
 	LJ_EVENT	*pevent2;
 	
 	pevent1 = &gLJEventTbl[0];
 	pevent2 = &gLJEventTbl[1];
 	for ( i = 0; i < (LJ_MAX_EVENTS - 1); i++ )
 	{
 		pevent1->ljEventType = LJ_EVENT_TYPE_UNUSED;
 		pevent1->ljEventPtr  = pevent2;
 		pevent1++;
 		pevent2++;
 	}
 	pevent1->ljEventType = LJ_EVENT_TYPE_UNUSED;
 	pevent1->ljEventPtr  = (LJ_EVENT *)0;
 	gLJEventFreeList     = &gLJEventTbl[0];
 #else
 	gLJEventFreeList     = &gLJEventTbl[0];
 	gLJEventFreeList     = LJ_EVENT_TYPE_UNUSED;
 	gLJEventFreeList     = (LJ_EVENT *)0;
 #endif
 #endif
 }
 
/*
 *========================================================================================================
 * 函数名称：LJ_TcbInit
 * 功能描述：初始化任务控制块
 *
 * 传    参：prio		(INT8U)：任务的优先级
 *           ptos	 (LJ_STK *)：指向任务栈栈顶的指针
 *           pbos	 (LJ_STK *)：指向任务栈栈底的指针
 *           id	  	   (INT16U)：任务标志符
 *           stkSize   (INT32U)：堆栈的容量
 *           pext	   (void *)：LJ_TCB中的扩展指针
 *           opt       (INT16U)：LJ_TCB的选择项
 *
 * 返 回 值：LJ_NO_ERR		   ：如果函数调用成功
 *           LJ_NO_MORE_TCB    ：如果没有更多的空闲任务控制块被分配，任务就不能被创建
 *
 * 注    意：这个函数LJ_RTOS的内部函数，应用程序不能调用该函数
 *========================================================================================================
 */
 INT8U
 LJ_TcbInit( INT8U prio, LJ_STK *ptos, LJ_STK *pbos, INT16U id, INT32U stkSize, void *pext, INT16U opt )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_TCB		*ptcb;
 	
 	LJ_ENTER_CRITICAL( );
 	ptcb = gLJTcbFreeList;							/* 试图从LJ_TCB缓冲池中得到一个任务控制块            */
 	if ( ptcb != (LJ_TCB *)0 )						/* 如果缓冲池中有空余的LJ_TCB,这个LJ_TCB就被初始化了 */
 	{
 		gLJTcbFreeList		= ptcb->ljTcbNext;
 		LJ_EXIT_CRITICAL( );
 		ptcb->ljTcbStkTop	= ptos;
 		ptcb->ljTcbPrio		= prio;
 		ptcb->ljTcbStat		= LJ_STAT_RDY;
 		ptcb->ljTcbDly		= 0;
 #if LJ_TASK_CREATE_EXT_EN > 0
 		ptcb->ljTcbExtPtr	= pext;
 		ptcb->ljTcbStkSize	= stkSize;
 		ptcb->ljTcbStkBottom= pbos;
 		ptcb->ljTcbOpt		= opt;
 		ptcb->ljTcbId		= id;
 #else
 		pext				= pext;
 		stkSize				= stkSize;
 		pbos				= pbos;
 		opt					= opt;
 		id					= id;
 #endif
 
 #if LJ_TASK_DEL_EN > 0
 		ptcb->ljTcbDelReq	= LJ_NO_ERR;
 #endif
 
 		ptcb->ljTcbY		= prio >> 3;
 		ptcb->ljTcbBitY		= gLJMapTbl[ptcb->ljTcbY];
 		ptcb->ljTcbX		= prio & 0x07;
 		ptcb->ljTcbBitX		= gLJMapTbl[ptcb->ljTcbX];
 		
 #if LJ_EVENT_EN > 0
 		ptcb->ljTcbEventPtr = (LJ_EVENT *)0;
 #endif
 
 #if ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)) || (LJ_MBOX_EN > 0)
 		ptcb->ljTcbMsg		= (void *)0;
 #endif
 
 		LJ_ENTER_CRITICAL( );
 		gLJTcbPrioTbl[prio]	= ptcb;
 		ptcb->ljTcbNext		= gLJTcbList;			/* 把LJ_TCB插入到已经建立任务的双向链表中            */
 		ptcb->ljTcbPrev		= (LJ_TCB *)0;
 		if ( gLJTcbList != (LJ_TCB *)0 )
 		{
 			gLJTcbList->ljTcbPrev = ptcb;
 		}
 		gLJTcbList			= ptcb;
 		
 		gLJRdyGrp		   		|= ptcb->ljTcbBitY;	/* 使任务进入就绪态                                 */
 		gLJRdyTbl[ptcb->ljTcbY] |= ptcb->ljTcbBitX;
 		
 		LJ_EXIT_CRITICAL( );
 		
 		return (LJ_NO_ERR);	
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_MORE_TCB);
 }
 
/*
 *=======================================================================================================
 * 函数名称：LJ_Dummy
 * 功能描述：不进行任何的实质性操作，用于LJTaskDel()中在终端开着的情况下至少执行一条指令
 * 传    参：void
 * 返 回 值：void
 *=======================================================================================================
 */
 #if LJ_TASK_DEL_EN > 0
 
 void
 LJ_Dummy( void )
 {
 }
 
 #endif
 
/*
 *=======================================================================================================
 * 函数名称：LJ_EventTaskRdy
 * 功能描述：当某个事件发生时，将等待该事件任务列表中的最高优先级任务置于就绪态
 *
 * 传    参：pevent (LJ_EVENT *)：指向事件控制块的指针
 *           msg        (void *)：指向消息的指针
 *           msk         (INT8U)：清除任务控制块状态位的掩码
 *
 * 返 回 值：prio        (INT8U)：返回就绪任务的优先级
 *=======================================================================================================
 */
 #if LJ_EVENT_EN > 0
 
 INT8U
 LJ_EventTaskRdy( LJ_EVENT *pevent, void *msg, INT8U msk )
 {
 	LJ_TCB	*ptcb;
 	INT8U	 x;
 	INT8U	 y;
 	INT8U	 bitX;
 	INT8U	 bitY;
 	INT8U	 prio;
 	
 	y		= gLJUnMapTbl[pevent->ljEventGrp];			/* 确定HPT任务在gLJEventTbl[]中的字节索引         */
 	bitY	= gLJMapTbl[y];								/* 利用该索引得到HPT任务在gLJEventGrp中的位屏蔽码 */
 	x		= gLJUnMapTbl[pevent->ljEventTbl[y]];		/* 判断HPT任务在gLJEventTbl[]中的相应位的位置     */
 	bitX	= gLJMapTbl[x];								/* 得到HPT任务在gLJEventTbl[]中相应的位屏蔽码     */
 	prio	= (INT8U)((y << 3) + x);					/* 算出要进入就绪态的最高优先级任务的优先级       */
 	if ( (pevent->ljEventTbl[y] &= ~bitX) == 0x00 )		/* 从等待任务列表中删除HPT任务                    */
 	{
 		pevent->ljEventGrp &= ~bitY;
 	}
 	ptcb				= gLJTcbPrioTbl[prio];			/* 找到指向该任务的任务控制块TCB的指针            */
 	ptcb->ljTcbDly		= 0;
 	ptcb->ljTcbEventPtr = (LJ_EVENT *)0;
 #if ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)) || (LJ_MBOX_EN > 0)
 	ptcb->ljTcbMsg		= msg;							/* 如果是等待消息则将相应的消息作为参数传递给它   */
 #else
 	msg					= msg;
 #endif
 	ptcb->ljTcbStat	   &= ~msk;
 	if ( ptcb->ljTcbStat == LJ_STAT_RDY )
 	{
 		gLJRdyGrp		|= bitY;
 		gLJRdyTbl[y]	|= bitX;
 	}
 	
 	return (prio);
 }
 
 #endif  
 
/*
 *=======================================================================================================
 * 函数名称：LJ_EventWaitListInit
 * 功能描述：初始化一个事件控制块的等待任务列表
 * 传    参：pEvnet (LJ_EVENT *)：指向事件控制块的指针
 * 返 回 值：void
 *=======================================================================================================
 */
 #if LJ_EVENT_EN > 0
 
 void
 LJ_EventWaitListInit( LJ_EVENT *pevent )
 {
 	INT8U	*ptbl;
 	INT8U	 i;
 	
 	pevent->ljEventGrp = 0x00;
 	ptbl			   = &pevent->ljEventTbl[0];
 	for ( i = 0; i < LJ_EVENT_TBL_SIZE; i++ )
 	{
 		*ptbl++ = 0x00;
 	}
 }
 
 #endif
 
/*
 *======================================================================================================
 * 函数名称：LJ_EventTaskWait
 * 功能描述：使一个任务进入等待状态,使当前任务从就绪表中脱离，并放到相应的事件控制块的等待任务列表中
 * 传    参：pevent (LJ_EVENT *)：指向事件控制块的指针
 * 返 回 值：void
 *======================================================================================================
 */
 #if LJ_EVENT_EN > 0
 
 void
 LJ_EventTaskWait( LJ_EVENT *pevent )
 {
 	gLJTcbCur->ljTcbEventPtr = pevent;
 	
 	if ( (gLJRdyTbl[gLJTcbCur->ljTcbY] &= ~gLJTcbCur->ljTcbBitX) == 0x00 )
 	{
 		gLJRdyGrp &= ~gLJTcbCur->ljTcbBitY;
 	}
 	
 	pevent->ljEventTbl[gLJTcbCur->ljTcbY] |= gLJTcbCur->ljTcbBitX;
 	pevent->ljEventGrp                    |= gLJTcbCur->ljTcbBitY;
 }
 
 #endif
 
/*
 *=====================================================================================================
 * 函数名称：LJ_EventTo
 * 功能描述：由于等待超时而将任务置为就绪态
 * 传    参：pevent (LJ_EVENT *)：指向事件控制块的指针
 * 返 回 值：void
 *=====================================================================================================
 */
 
 #if LJ_EVENT_EN > 0
 
 void
 LJ_EventTo( LJ_EVENT *pevent )
 {
 	if ( (pevent->ljEventTbl[gLJTcbCur->ljTcbY] &= ~gLJTcbCur->ljTcbBitX) == 0x00 )
 	{
 		pevent->ljEventGrp &= ~gLJTcbCur->ljTcbBitY;
 	}
 	
 	gLJTcbCur->ljTcbStat		= LJ_STAT_RDY;
 	gLJTcbCur->ljTcbEventPtr	= (LJ_EVENT *)0;
 }
 
 #endif