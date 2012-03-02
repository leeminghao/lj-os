/*
 *=======================================================================================================
 *                                               LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_queue.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现LJ_RTOS消息队列的管理
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/23
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=======================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *=======================================================================================================
 * 函数名称：LJ_QInit
 * 功能描述：初始化空闲队列控制块链表
 * 传    参：void
 * 返 回 值：void
 *=======================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0
 
 void
 LJ_QInit( void )
 {
 #if LJ_MAX_QS == 1
 	gLJQFreeList		 = &gLJQTbl[0];
 	gLJQFreeList->ljQPtr = (LJ_Q *)0;
 #endif
 
 #if LJ_MAX_QS >= 2
 	INT16U	 i;
 	LJ_Q	*pq1;
 	LJ_Q	*pq2;
 	
 	pq1 = &gLJQTbl[0];
 	pq2 = &gLJQTbl[1];
 	for ( i = 0; i < (LJ_MAX_QS - 1); i++ )
 	{
 		pq1->ljQPtr = pq2;
 		pq1++;
 		pq2++;
 	}
 	pq1->ljQPtr  = (LJ_Q *)0;
 	gLJQFreeList = &gLJQTbl[0];
 #endif
 }
 
 #endif
 
/*
 *=======================================================================================================
 * 函数名称：LJQCreate
 * 功能描述：建立一个消息队列
 *
 * 传    参：start (void **) ：指向消息数组的指针 void *messageStorage[size]
 *           size   (INT16U) ：指向消息数组的大小
 *
 * 返 回 值：!= (LJ_EVENT *)0：指向消息队列ECB的指针
 *           == (LJ_EVENT *)0：如果没有可使用的事件控制块
 *=======================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0
 
 LJ_EVENT *
 LJQCreate( void **start, INT16U size )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_EVENT	*pevent;
 	LJ_Q		*pq;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在中断服务子程序中调用此函数          */
 	{
 		return ((LJ_EVENT *)0);
 	}
 	LJ_ENTER_CRITICAL( );
 	pevent = gLJEventFreeList;							/* 试图从空余事件控制块ECB链表中取得一个ECB      */
 	if ( gLJEventFreeList != (LJ_EVENT *)0 )
 	{
 		gLJEventFreeList = gLJEventFreeList->ljEventPtr;
 	}
 	LJ_EXIT_CRITICAL( );
 	if ( pevent != (LJ_EVENT *)0 )
 	{
 		LJ_ENTER_CRITICAL( );
 		pq = gLJQFreeList;								/* 试图从空闲队列控制块链表中得到一个队列控制块  */
 		if ( pq != (LJ_Q *)0 )
 		{
 			gLJQFreeList		= gLJQFreeList->ljQPtr;
 			LJ_EXIT_CRITICAL( );
 			pq->ljQStart		= start;
 			pq->ljQEnd			= &start[size];
 			pq->ljQIn			= start;
 			pq->ljQOut			= start;
 			pq->ljQSize			= size;
 			pq->ljQEntries		= 0;
 			pevent->ljEventType = LJ_EVENT_TYPE_Q;
 			pevent->ljEventCnt	= 0;
 			pevent->ljEventPtr	= pq;
 			LJ_EventWaitListInit( pevent );
 		}
 		else
 		{
 			pevent->ljEventPtr = (void *)gLJEventFreeList;
 			gLJEventFreeList   = pevent;
 			LJ_EXIT_CRITICAL( );
 			pevent = (LJ_EVENT *)0;
 		}
 	}
 	
 	return (pevent);
 }
 
 #endif
 
/*
 *==========================================================================================================
 * 函数名称：LJQPend
 * 功能描述：等待消息队列中的消息
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息队列ECB的指针
 *           timeout    (INT16U)：等待超时时间
 *           err       (INT8U *)：指向错误代码的指针：
 *                                LJ_NO_ERR         ：如果函数调用成功
 *                                LJ_TIMEOUT        ：如果函数调用超时
 *                                LJ_ERR_EVENT_TYPE ：如果'pevent'不是指向一个消息队列ECB的指针
 *                                LJ_ERR_PEVENT_NULL：如果'pevent'是NULL指针
 *                                LJ_ERR_PEND_ISR   ：如果是在中断服务子程序中调用该函数
 *
 * 返 回 值：!= (void *)0      ：指向接收到的消息
 *           == (void *)0      ：a)如果没有接收到消息
 *                               b)如果'pevent'是一个NULL指针
 *                               c)如果'pevent'不是指向消息队列ECB的指针
 *==========================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0
 
 void *
 LJQPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	void		*msg;
 	LJ_Q		*pq;
 	
 	if ( gLJIntNesting > 0 )								/* 确保不是在中断服务子程序中调用该函数          */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return ((void *)0);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )							/* 确保'pevent'不是空指针                        */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return ((void *)0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )			/* 确保'pevent'指向的是消息队列ECB的指针         */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return ((void *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pq = (LJ_Q *)pevent->ljEventPtr;
 	if ( pq->ljQEntries > 0 )								/* 判断消息队列中是否还存在消息                  */
 	{
 		msg = *pq->ljQOut++;
 		pq->ljQEntries--;
 		if ( pq->ljQOut == pq->ljQEnd )
 		{
 			pq->ljQOut = pq->ljQStart;
 		}
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		
 		return (msg);
 	}
 	gLJTcbCur->ljTcbStat |= LJ_STAT_Q;
 	gLJTcbCur->ljTcbDly   = timeout;
 	LJ_EventTaskWait( pevent );								/* 没有消息则等待                                */
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );
 	LJ_ENTER_CRITICAL( );
 	msg = gLJTcbCur->ljTcbMsg;
 	if ( msg != (void *)0 )									/* 等到了消息                                    */
 	{
 		gLJTcbCur->ljTcbMsg			= (void *)0;
 		gLJTcbCur->ljTcbStat		= LJ_STAT_RDY;
 		gLJTcbCur->ljTcbEventPtr	= (LJ_EVENT *)0;
 		LJ_EXIT_CRITICAL( );
 		*err						= LJ_NO_ERR;
 		return (msg);
 	}
 	LJ_EventTo( pevent );									/* 因为等待超时而将任务置于就绪态                */
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_TIMEOUT;
 	
 	return ((void *)0);
 }
 
 #endif
 
/*
 *============================================================================================================
 * 函数名称：LJQPost
 * 功能描述：向消息队列发送一则消息
 *
 * 传    参：pevent (LJ_EVENT *) ：指向消息队列ECB
 *           msg        (void *) ：指向将要发送消息的指针
 *           opt         (INT8U) ：发送消息选项
 *                                 LJ_POST_OPT_NONE     ：发送消息给单个任务(FIFO)
 *                                 LJ_POST_OPT_BROADCAST：发送消息给所有等待消息队列中消息的任务
 *                                 LJ_POST_OPT_FRONT    ：发送消息个单个任务(LIFO)
 *
 * 返 回 值：LJ_NO_ERR           ：如果函数调用成功
 *           LJ_Q_FULL           ：如果消息队列已满
 *           LJ_ERR_EVENT_TYPE   ：如果'pevent'不是一个指向消息队列ECB的指针
 *           LJ_ERR_PEVENT_NULL  ：如果'pevent'是一个NULL指针
 *           LJ_ERR_POST_NULL_PTR：如果发送的消息是空消息
 *============================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0 && LJ_Q_POST_EN > 0
 
 INT8U
 LJQPost( LJ_EVENT *pevent, void *msg, INT8U opt )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_Q		*pq;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )							/* 确保'pevent'不是NULL指针                       */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( msg == (void *)0 )									/* 确保发送的不是空消息                           */
 	{
 		return (LJ_ERR_POST_NULL_PTR);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )			/* 确保'pevent'指向的是消息队列ECB的指针          */
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventGrp != 0x00 )						/* 检查是否有任务在等待消息队列中的消息           */
 	{
 		if ( (opt & LJ_POST_OPT_BROADCAST) != 0x00 )		/* 检查是不是广播方式                             */
 		{
 			while ( pevent->ljEventGrp != 0x00 )
 			{
 				LJ_EventTaskRdy( pevent, msg, LJ_STAT_Q );
 			}
 		}
 		else
 		{
 			LJ_EventTaskRdy( pevent, msg, LJ_STAT_Q );		/* 如果不是广播的方式，则只有等待列表中的HPT任务进入就绪态 */
 		}
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );										/* 检查该任务是不是最高优先级任务                 */
 		return (LJ_NO_ERR);
 	}
 	pq = (LJ_Q *)pevent->ljEventPtr;
 	if ( pq->ljQEntries >= pq->ljQSize )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_Q_FULL);
 	}
 	if ( (opt & LJ_POST_OPT_FRONT) != 0x00 )				/* LIFO                                           */
 	{
 		if ( pq->ljQOut == pq->ljQStart )
 		{
 			pq->ljQOut = pq->ljQEnd;
 		}
 		pq->ljQOut--;
 		*pq->ljQOut = msg;
 	}
 	else													/* FIFO                                           */
 	{
 		*pq->ljQIn++ = msg;
 		if ( pq->ljQIn == pq->ljQEnd )
 		{
 			pq->ljQIn = pq->ljQStart;
 		}
 	}
 	pq->ljQEntries++;
 	LJ_EXIT_CRITICAL( );
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *============================================================================================================
 * 函数名称：LJQDel
 * 功能描述：删除一个消息队列
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息队列ECB的指针
 *           opt         (INT8U)：删除选项
 *                                LJ_DEL_NO_PEND     ：删除消息队列如果没有任务等待
 *                                LJ_DEL_ALWAYS      ：删除消息队列即便有任务等待
 *           err       (INT8U *)：指向错误代码的指针
 *                                LJ_NO_ERR          ：如果调用函数成功
 *                                LJ_ERR_DEL_ISR     ：如果在中断服务子程序中调用该函数
 *                                LJ_ERR_INVALID_OPT ：如果删除选项无效
 *                                LJ_ERR_TASK_WAITING：如果有任务在等待消息队列
 *                                LJ_ERR_EVENT_TYPE  ：如果'pevent'不是指向消息队列ECB的指针
 *                                LJ_ERR_PEVENT_NULL ：如果'pevent'是NULL指针
 *
 * 返 回 值：pevent (LJ_EVENT *)：出现上述错误
 *                 (LJ_EVENT *)0：如果删除消息队列成功
 *============================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0 && LJ_Q_DEL_EN > 0
 
 LJ_EVENT *
 LJQDel( LJ_EVENT *pevent, INT8U opt, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	BOOLEAN		 tasksWaiting;
 	LJ_Q		*pq;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在中断服务子程序中调用此函数               */
 	{
 		*err = LJ_ERR_DEL_ISR;
 		return (pevent);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return (pevent);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return (pevent);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventGrp != 0x00 )
 	{
 		tasksWaiting = LJ_TRUE;
 	}
 	else
 	{
 		tasksWaiting = LJ_FALSE;
 	}
 	switch ( opt )
 	{
 		case LJ_DEL_NO_PEND:							/* 如果没有任务等待消息队列中消息，直接删除消息队列  */
 			if ( tasksWaiting == LJ_FALSE )
 			{
 				pq					= pevent->ljEventPtr;
 				pq->ljQPtr			= gLJQFreeList;
 				gLJQFreeList		= pq;
 				pevent->ljEventType = LJ_EVENT_TYPE_UNUSED;
 				pevent->ljEventPtr  = gLJEventFreeList;
 				gLJEventFreeList    = pevent;
 				LJ_EXIT_CRITICAL( );
 				*err = LJ_NO_ERR;
 				return ((LJ_EVENT *)0);
 			}
 			else
 			{
 				LJ_EXIT_CRITICAL( );
 				*err = LJ_ERR_TASK_WAITING;
 				return (pevent);
 			}
 			
 		case LJ_DEL_ALWAYS:								/* 总是删除消息队列，并将等待任务列表中的任务置于就绪态 */
 			while ( pevent->ljEventGrp != 0x00 )
 			{
 				LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_Q );
 			}
 			pq					= pevent->ljEventPtr;
 			pq->ljQPtr			= gLJQFreeList;
 			gLJQFreeList		= pq;
 			pevent->ljEventType	= LJ_EVENT_TYPE_UNUSED;
 			pevent->ljEventPtr	= gLJEventFreeList;
 			gLJEventFreeList    = pevent;
 			LJ_EXIT_CRITICAL( );
 			if ( tasksWaiting == LJ_TRUE )
 			{
 				LJ_Sched( );
 			}
 			*err = LJ_NO_ERR;
 			return ((LJ_EVENT *)0);
 			
 		default:
 			LJ_EXIT_CRITICAL( );
 			*err = LJ_ERR_INVALID_OPT;
 			return (pevent);
 	}
 }
 
 #endif
 
/*
 *=================================================================================================================
 * 函数名称：LJQAccept
 * 功能描述：无等待的从消息队列中获得消息
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息队列ECB的指针
 *
 * 返 回 值：!= (void *)0       ：如果消息队列中有消息可利用
 *           == (void *)0       ：1)如果消息队列为空
 *                                2)如果'pevent'为NULL指针
 *                                3)如果'pevent'不是指向消息队列ECB的指针
 *=================================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0 && LJ_Q_ACCEPT_EN > 0
 
 void *
 LJQAccept( LJ_EVENT *pevent )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	void		*msg;
 	LJ_Q		*pq;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		return ((void *)0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )
 	{
 		return ((void *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pq = (LJ_Q *)pevent->ljEventPtr;
 	if ( pq->ljQEntries > 0 )
 	{
 		msg = *pq->ljQOut++;
 		pq->ljQEntries--;
 		if ( pq->ljQOut == pq->ljQEnd )
 		{
 			pq->ljQOut = pq->ljQStart;
 		}
 	}
 	else
 	{
 		msg = (void *)0;
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (msg);
 }
 
 #endif
 
/*
 *==================================================================================================================
 * 函数名称：LJQFlush
 * 功能描述：清空消息队列
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息队列ECB控制块指针
 *
 * 返 回 值：LJ_NO_ERR          ：如果调用该函数成功
 *           LJ_ERR_EVENT_TYPE  ：如果'pevent'不是指向消息队列ECB的指针
 *           LJ_ERR_PEVENT_NULL ：如果'pevent'为NULL指针
 *==================================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0 && LJ_Q_FLUSH_EN > 0
 
 INT8U
 LJQFlush( LJ_EVENT *pevent )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_Q		*pq;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pq				= (LJ_Q *)pevent->ljEventPtr;
 	pq->ljQIn   	= pq->ljQStart;
 	pq->ljQOut  	= pq->ljQStart;
 	pq->ljQEntries	= 0;
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *=================================================================================================================
 * 函数名称：LJQQuery
 * 功能描述：获取消息队列的状态
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息队列ECB的指针
 *           pdata (LJ_Q_DATA *)：指向消息队列数据控制块的指针
 *
 * 返 回 值：LJ_NO_ERR          ：如果该函数调用成功
 *           LJ_ERR_EVENT_TYPE  ：如果'pevent'不是指向消息队列ECB的指针
 *           LJ_ERR_PEVENT_NULL ：如果'pevent'是NULL指针
 *=================================================================================================================
 */
 #if LJ_Q_EN > 0 && LJ_MAX_QS > 0 && LJ_Q_QUERY_EN > 0
 
 INT8U
 LJQQuery( LJ_EVENT *pevent, LJ_Q_DATA *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_Q		*pq;
 	INT8U		*psrc;
 	INT8U		*pdest;
 	INT8U		 i;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pdata->ljEventGrp = pevent->ljEventGrp;
 	psrc              = &pevent->ljEventTbl[0];
 	pdest             = &pdata->ljEventTbl[0];
 	for ( i = 0; i < LJ_EVENT_TBL_SIZE; i++ )
 	{
 		*pdest++ = *psrc++;
 	}
 	pq = (LJ_Q *)pevent->ljEventPtr;
 	if ( pq->ljQEntries > 0 )
 	{
 		pdata->ljMsg = *pq->ljQOut;
 	}
 	else
 	{
 		pdata->ljMsg = (void *)0;
 	}
 	pdata->ljNMsgs = pq->ljQEntries;
 	pdata->ljQSize = pq->ljQSize;
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_ERR);
 }
 
 #endif