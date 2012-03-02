/*
 *=========================================================================================================
 *                                                LJ_RTOS
 *                                          The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_mbox.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现LJ_RTOS中的消息邮箱管理
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/23
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *========================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *========================================================================================================
 * 函数名称：LJMboxCreate
 * 功能描述：建立一个消息邮箱
 *
 * 传    参：msg (void *)    ：指向一个投递到消息邮箱中的消息的指针，如果为NULL, 则创建一个空的消息邮箱
 *
 * 返 回 值：!= (LJ_EVENT *)0：返回一个指向消息邮箱ECB的指针
 *           == (LJ_EVENT *)0：如果没有空闲的事件控制块可利用
 *========================================================================================================
 */
 #if LJ_MBOX_EN > 0
 
 LJ_EVENT *
 LJMboxCreate( void *msg )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_EVENT	*pevent;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在中断服务子程序中调用此函数           */
 	{
 		return ((LJ_EVENT *)0);
 	}
 	LJ_ENTER_CRITICAL( );
 	pevent = gLJEventFreeList;
 	if ( gLJEventFreeList != (LJ_EVENT *)0 )
 	{
 		gLJEventFreeList = (LJ_EVENT *)gLJEventFreeList->ljEventPtr;
 	}
 	LJ_EXIT_CRITICAL( );
 	if ( pevent != (LJ_EVENT *)0 )						/* 确保事件控制块可用                             */
 	{
 		pevent->ljEventType = LJ_EVENT_TYPE_MBOX;
 		pevent->ljEventCnt  = 0;
 		pevent->ljEventPtr  = msg;
 		LJ_EventWaitListInit( pevent );					/* 初始化等待任务列表                             */
 	}
 	
 	return (pevent);
 }
 
 #endif
 
/*
 *=========================================================================================================
 * 函数名称：LJMboxPend
 * 功能描述：等待消息邮箱中的消息
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息邮箱ECB的指针
 *           timeout    (INT16U)：等待超时的时间(用时钟节拍表示)
 *           err       (INT8U *)：指向错误代码的指针：
 *                                LJ_NO_ERR         ：如果函数调用成功
 *                                LJ_TIMEOUT        ：如果等待超时
 *                                LJ_ERR_EVENT_TYPE ：如果'pevent'不是指向消息邮箱ECB的指针
 *                                LJ_ERR_PEND_ISR   ：如果该函数是在中断服务子程序中调用的
 *                                LJ_ERR_PEVENT_NULL：如果'pevent'是一个NULL指针
 *
 * 返 回 值：!= (void *)0       ：接收到的消息指针
 *           == (void *)0       ：a)如果没有接收到消息
 *                                b)如果'pevent'是一个NULL指针
 *                                c)如果'pevent'不是指向消息邮箱ECB的指针
 *=========================================================================================================
 */
 #if LJ_MBOX_EN > 0
 
 void *
 LJMboxPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	void		*msg;
 	
 	if ( gLJIntNesting > 0 )						/* 确保不是在中断服务子程序中调用该函数                */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return ((void *)0);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )					/* 确保'pevent'不是一个NULL指针                        */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return ((void *)0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )/* 确保'pevent'是一个指向消息邮箱ECB的指针             */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return ((void *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	msg = pevent->ljEventPtr;
 	if ( msg != (void *)0 )
 	{
 		pevent->ljEventPtr = (void *)0;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		return (msg);
 	}
 	gLJTcbCur->ljTcbStat |= LJ_STAT_MBOX;
 	gLJTcbCur->ljTcbDly   = timeout;
 	LJ_EXIT_CRITICAL( );
 	LJ_EventTaskWait( pevent );
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );
 	msg = gLJTcbCur->ljTcbMsg;						/* 等到了指定的消息                                   */
 	if ( msg != (void *)0 )
 	{
 		gLJTcbCur->ljTcbMsg  	 = (void *)0;
 		gLJTcbCur->ljTcbStat	 = LJ_STAT_RDY;
 		gLJTcbCur->ljTcbEventPtr = (LJ_EVENT *)0;
 		LJ_EXIT_CRITICAL( );
 		*err                     = LJ_NO_ERR;
 		return (msg);
 	}
 	LJ_EventTo( pevent );							/* 等待超时                                           */
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_TIMEOUT;
 	
 	return ((void *)0);
 }
 
 #endif
 
/*
 *=========================================================================================================
 * 函数名称：LJMboxPost
 * 功能描述：向消息邮箱中发送一则消息
 *
 * 传    参：pevent (LJ_EVENT *) ：指向消息邮箱ECB的指针
 *           msg        (void *) ：指向消息的指针
 *           opt         (INT8U) ：消息选项
 *                                 LJ_POST_OPT_NONE     ：发送消息给单个任务(该任务为等待列表中的HPT任务)
 *                                 LJ_POST_OPT_BROADCAST：发送消息给等待列表中所有任务
 *
 * 返 回 值：LJ_NO_ERR           ：如果函数调用成功
 *           LJ_MBOX_FULL        ：如果消息邮箱已经有消息了
 *           LJ_ERR_EVENT_TYPE   ：如果'pevent'不是指向消息邮箱ECB的指针
 *           LJ_ERR_PEVENT_NULL  ：如果'pevent'是一个NULL指针
 *           LJ_ERR_POST_NULL_PTR：如果你发送的是一个空消息
 *=========================================================================================================
 */
 #if LJ_MBOX_EN > 0 && LJ_MBOX_POST_EN > 0
 
 INT8U
 LJMboxPost( LJ_EVENT *pevent, void *msg, INT8U opt )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保'pevent'不是NULL指针                        */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( msg == (void *)0 )								/* 确保不是发送空消息                              */
 	{
 		return (LJ_ERR_POST_NULL_PTR);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventGrp != 0x00 )
 	{
 		if ( (opt & LJ_POST_OPT_BROADCAST) != 0x00 )
 		{
 			while ( pevent->ljEventGrp != 0x00 )
 			{
 				LJ_EventTaskRdy( pevent, msg, LJ_STAT_MBOX );
 			}
 		}
 		else
 		{
 			LJ_EventTaskRdy( pevent, msg, LJ_STAT_MBOX );
 		}
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );									/* 检查该任务是否是系统中优先级最高的就绪态任务   */
 		return (LJ_NO_ERR);
 	}
 	if ( pevent->ljEventPtr != (void *)0 )				/* 如果没有任务等待，那么发送的消息应发在邮箱中   */
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_MBOX_FULL);
 	}
 	pevent->ljEventPtr = msg;
 	LJ_EXIT_CRITICAL( );
 	return (LJ_NO_ERR);
 	
 }
 
 #endif
 
/*
 *========================================================================================================
 * 函数名称：LJMboxDel
 * 功能描述：删除一个邮箱
 * 
 * 传    参：pevent (LJ_EVENT *)：指向消息邮箱ECB的指针
 *           opt         (INT8U)：删除选项
 *           err       (INT8U *)：指向错误代码的指针：
 *                                LJ_NO_ERR          ：如果函数调用成功
 *                                LJ_ERR_DEL_ISR     ：如果是在中断服务子程序中调用该函数
 *                                LJ_ERR_INVALID_OPT ：如果删除选项非法
 *                                LJ_ERR_TASK_WAITING：如果有任务在等待消息邮箱中的消息
 *                                LJ_ERR_EVENT_TYPE  ：如果'pevent'不是指向消息邮箱ECB的指针
 *                                LJ_ERR_PEVENT_NULL ：如果'pevent'是NULL指针
 *
 * 返 回 值：pevent (LJ_EVENT *)：如果出现上述错误
 *                 (LJ_EVENT *)0：如果成功删除消息邮箱
 *========================================================================================================
 */
 #if LJ_MBOX_EN > 0 && LJ_MBOX_DEL_EN > 0
 
 LJ_EVENT *
 LJMboxDel( LJ_EVENT *pevent, INT8U opt, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	BOOLEAN		 tasksWaiting;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在中断服务子程序中调用该函数           */
 	{
 		*err = LJ_ERR_DEL_ISR;
 		return (pevent);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保'pevent'不是NULL指针                       */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return (pevent);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )	/* 确保'pevent'是指向消息邮箱ECB的指针            */
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
 		case LJ_DEL_NO_PEND:							/* 没有等待该邮箱的任务时，删除该消息邮箱         */
 			if ( tasksWaiting == LJ_FALSE )
 			{
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
 			
 		case LJ_DEL_ALWAYS:								/* 将所有等待该消息邮箱的任务都进入就绪态         */
 			while ( pevent->ljEventGrp != 0x00 )
 			{
 				LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_MBOX );
 			}
 			pevent->ljEventType = LJ_EVENT_TYPE_UNUSED;
 			pevent->ljEventPtr  = gLJEventFreeList;
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
 *=========================================================================================================
 * 函数名称：LJMboxAccept
 * 功能描述：无等待地从邮箱中得到一则消息
 *
 * 传    参：pevent (LJ_EVENT *)：指向消息邮箱ECB的指针
 *
 * 返 回 值：!= (void *)0       ：如果消息邮箱中有消息可用，则立即返回里面的消息
 *           == (void *)0       ：a)如果消息邮箱中没有消息可用
 *                                b)如果'pevent'是NULL指针
 *                                c)如果'pevent'不是指向消息邮箱ECB的指针
 *=========================================================================================================
 */
 #if LJ_MBOX_EN > 0 && LJ_MBOX_ACCEPT_EN > 0
 
 void *
 LJMboxAccept( LJ_EVENT *pevent )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	void        *msg;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		return ((void *)0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )
 	{
 		return ((void *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	msg					= pevent->ljEventPtr;
 	pevent->ljEventPtr	= (void *)0;
 	LJ_EXIT_CRITICAL( );
 	
 	return (msg);
 }
 
 #endif
 
/*
 *========================================================================================================
 * 函数名称：LJMboxQuery
 * 功能描述：查询一个邮箱的状态
 * 
 * 传    参：pevent     (LJ_EVENT *)：指向消息邮箱ECB的指针
 *           pdata  (LJ_MBOX_DATA *)：指向消息邮箱数据的指针
 *
 * 返 回 值：LJ_NO_ERR              ：该函数调用成功
 *           LJ_ERR_EVENT_TYPE      ：如果'pevent'不是指向消息邮箱ECB的指针
 *           LJ_ERR_PEVENT_NULL     ：如果'pevent'是一个NULL指针
 *========================================================================================================
 */
 #if LJ_MBOX_EN > 0 && LJ_MBOX_QUERY_EN > 0
 
 INT8U
 LJMboxQuery( LJ_EVENT *pevent, LJ_MBOX_DATA *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	INT8U		*psrc;
 	INT8U		*pdest;
 	INT8U		 i;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pdata->ljEventGrp = pevent->ljEventGrp;
 	psrc			  = &pevent->ljEventTbl[0];
 	pdest			  = &pdata->ljEventTbl[0];
 	for ( i = 0; i < LJ_EVENT_TBL_SIZE; i++ )
 	{
 		*pdest++ = *psrc++;
 	}
 	pdata->ljMsg = pevent->ljEventPtr;
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_ERR);	
 }
 
 #endif