/*
 *===========================================================================================================
 *                                                  LJ_RTOS
 *                                            The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_sem.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现信号量管理函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/22
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==========================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *==========================================================================================================
 * 函数名称：LJSemCreate
 * 功能描述：创建一个信号量
 *
 * 传    参：cnt (INT16U)：信号量初始值
 *           1.如果信号量表示一个或者多个事件发生的话，那么该信号量的初始值通常赋为0
 *           2.如果信号量用于对共享资源的访问，那么该信号量的初始值应赋为1(二值信号量)
 *           3.如果信号量用来表示允许任务访问n个相同的资源，那么该信号量的初始值应赋为n,把信号量作为一个
 *             可计数的信号量使用
 *
 * 返 回 值：!= (void *)0：一个指向事件控制块的指针
 *           == (void *)0：如果没有事件控制块可用
 *==========================================================================================================
 */
 #if LJ_SEM_EN > 0
 
 LJ_EVENT *
 LJSemCreate( INT16U cnt )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_EVENT	*pevent;
 	
 	if ( gLJIntNesting > 0 )						/* 确保LJSemCreate()函数不是被中断服务子程序所调用      */
 	{
 		return ((LJ_EVENT *)0);
 	}
 	LJ_ENTER_CRITICAL( );
 	pevent = gLJEventFreeList;						/* 试图从空余事件控制块链表中获得一个事件控制块         */
 	if ( gLJEventFreeList != (LJ_EVENT *)0 )
 	{
 		gLJEventFreeList = gLJEventFreeList->ljEventPtr;
 	}
 	LJ_EXIT_CRITICAL( );
 	if ( pevent != (LJ_EVENT *)0 )
 	{
 		pevent->ljEventType = LJ_EVENT_TYPE_SEM;
 		pevent->ljEventCnt  = cnt;
 		pevent->ljEventPtr  = (void *)0;
 		
 		LJ_EventWaitListInit( pevent );
 	}
 	
 	return pevent;
 }
 
 #endif
 
/*
 *========================================================================================================
 * 函数名称：LJSemPend
 * 功能描述：等待一个信号量
 *
 * 传    参：pevent (LJ_EVENT *)：一个指向事件控制块的指针，在这表示信号量
 *           timeout    (INT16U)：等待超时时间(时钟节拍)，如果 != 0, 则任务等待指定的事件，如果 == 0, 则
 *                                任务将一直等待下去
 *           err       (INT8U *)：指向错误信息的指针，可能的错误信息为：
 *                                LJ_NO_ERR         ：如果函数调用成功
 *                                LJ_TIMEOUT        ：如果任务等待超时
 *                                LJ_ERR_EVENT_TYPE ：如果没用传递一个指向信号量的指针
 *                                LJ_ERR_PEND_ISR   ：如果在ISR中调用此函数
 *                                LJ_ERR_PEVENT_NULL：如果'pevent'是一个NULL指针
 *
 * 返 回 值：void
 *========================================================================================================
 */
 #if LJ_SEM_EN > 0
 
 void
 LJSemPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 	if ( gLJIntNesting > 0 )							/* 确保不是中断服务子程序调用该函数               */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return;
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保pevent不为空                               */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return;
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )		/* 确保pevent指向由LJSemCreate()创建的ECB块       */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return;
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventCnt > 0 )						/* 如果信号量本来就是有效的                        */
 	{
 		pevent->ljEventCnt--;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		return;
 	}
 	gLJTcbCur->ljTcbStat |= LJ_STAT_SEM;				/* 若信号量无效，则是任务进入等待信号量状态        */
 	gLJTcbCur->ljTcbDly   = timeout;
 	LJ_EventTaskWait( pevent );
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );						/* 由于得不到信号量，调用任务调度函数让下一个优先级更高的任务运行  */
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbCur->ljTcbStat & LJ_STAT_SEM )			/* 在调用LJ_Sched()函数之后检查任务状态            */
 	{
 		LJ_EventTo( pevent );			/* 等待超时，任务被LJTimeTick()函数置为就绪态的并不是因为LJSemPost */
 		LJ_EXIT_CRITICAL( );			/* 发出的信号量唤醒，故调用LJ_EventTo()将因等待超时的任务从等待任务*/
 		*err = LJ_TIMEOUT;				/* 列表中删除，并返回一个"超时错误"代码                            */
 	}
 	gLJTcbCur->ljTcbEventPtr	= (LJ_EVENT *)0;		/* LJSemPost()已经发出信号量，将指向信号量ECB的指针*/
 	LJ_EXIT_CRITICAL( );								/* 从该任务的任务控制块中删除                      */
 	*err = LJ_NO_ERR;
 }
 
 #endif
 
/*
 *==========================================================================================================
 * 函数名称：LJSemPost
 * 功能描述：发出一个信号量
 *
 * 传    参：pevent (LJ_EVENT *)：指向由LJSemCreate()创建的ECB控制块指针
 *
 * 返 回 值：LJ_NO_ERR          ：如果函数调用成功
 *           LJ_SEM_OVF         ：如果信号量值超过了16位无符号整数的范围
 *           LJ_ERR_EVENT_TYPE  ：如果传递的不是一个指向信号量ECB的指针
 *           LJ_ERR_PEVENT_NULL ：如果传递的'pevent'是一个NULL指针
 *==========================================================================================================
 */
 #if LJ_SEM_EN > 0
 
 INT8U
 LJSemPost( LJ_EVENT *pevent )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保事件控制块指针不为空                         */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventGrp != 0x00 )					/* 检查是否有任务在等待该信号量                     */
 	{
 		LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_SEM );	/* 把优先级最高的任务从等待列表中去除           */
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );									/* 检查从等待列表中删除的任务是否是系统中的HPT任务	*/ 
 		return (LJ_NO_ERR);
 	}
 	if ( pevent->ljEventCnt < 65535 )
 	{
 		pevent->ljEventCnt++;
 		LJ_EXIT_CRITICAL( );
 		return (LJ_NO_ERR);
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_SEM_OVF);
 }
 
 #endif
 
/*
 *===========================================================================================================
 * 函数名称：LJSemDel
 * 功能描述：删除一个信号量
 * 
 * 传    参：pevent (LJ_EVENT *)：指向事件控制块的指针
 *           opt         (INT8U)：删除信号量选项
 *                                LJ_DEL_NO_PEND     ：没有任务等待信号量
 *                                LJ_DEL_ALWAYS      ：所有等待该信号量的任务都进入就绪态
 *           err       (INT8U *)：指向错误码的指针
 *                                LJ_NO_ERR          ：如果该函数调用成功
 *                                LJ_ERR_DEL_ISR     ：如果尝试在ISR中删除信号量
 *                                LJ_ERR_INVALID_OPT ：如果不是指定的删除选项
 *                                LJ_ERR_TASK_WAITING：如果有任务等待信号量,但指定了LJ_DEL_NO_PEND选项
 *                                LJ_ERR_EVENT_TYPE  ：如果传递的事件指针不是指向信号量ECB
 *                                LJ_ERR_PEVENT_NULL ：如果'pevent'是NULL指针
 *
 * 返 回 值：pevent (LJ_EVENT *)：如果出现上述错误
 *           (LJ_EVENT *)0      ：如果信号量被成功删除
 *===========================================================================================================
 */
 #if LJ_SEM_EN > 0 && LJ_SEM_DEL_EN > 0
 
 LJ_EVENT *
 LJSemDel( LJ_EVENT *pevent, INT8U opt, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	BOOLEAN		 tasksWaiting;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在终端服务子程序中删除信号量               */
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
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )
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
 		case LJ_DEL_NO_PEND:							/* 表示并没有任务在等待该信号量                      */
 			if ( tasksWaiting == LJ_FALSE )
 			{
 				pevent->ljEventType = LJ_EVENT_TYPE_UNUSED;
 				pevent->ljEventPtr  = gLJEventFreeList; /* 将信号量ECB放回到空闲事件链表中                   */
 				gLJEventFreeList    = pevent;
 				LJ_EXIT_CRITICAL( );
 				
 				return ((LJ_EVENT *)0);
 			}
 			else										/* 如果指定了LJ_DEL_NO_PEND选项，但有任务在等待则返回 */
 			{
 				LJ_EXIT_CRITICAL( );
 				*err = LJ_ERR_TASK_WAITING;
 				
 				return (pevent);
 			}
 			
 		case LJ_DEL_ALWAYS:
 			while ( pevent->ljEventGrp != 0x00 )		/* 将所有等待信号量的任务都处于就绪态                */
 			{
 				LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_SEM );
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
 *===========================================================================================================
 * 函数名称：LJSemAccept
 * 功能描述：无等待的请求一个信号
 *
 * 传    参：pevent	(LJ_EVENT *)：指向事件控制块的指针
 *
 * 返 回 值：>  0               ：如果有信号量资源可利用，将信号量值简单的减1后返回
 *           == 0               ：如果没有信号量可利用或者参数错误简单的返回，而不等待
 *===========================================================================================================
 */
 #if LJ_SEM_EN > 0 && LJ_SEM_ACCEPT_EN > 0
 
 INT16U
 LJSemAccept( LJ_EVENT *pevent )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	INT16U		 cnt;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		return (0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )
 	{
 		return (0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	cnt	= pevent->ljEventCnt;
 	if ( cnt > 0 )
 	{
 		pevent->ljEventCnt--;
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (cnt);
 }
 
 #endif
 
/*
 *===========================================================================================================
 * 函数名称：LJSemQuery
 * 功能描述：查询一个信号量的当前状态
 *
 * 传    参：pevent    (LJ_EVENT *)：指向事件控制块的指针
 *           pdata	(LJ_SEM_DATA *)：指向用于记录信号量信息的指针
 *
 * 返 回 值：LJ_NO_ERR			   ：如果该函数调用成功
 *           LJ_ERR_EVENT_TYPE     ：如果'pevent'未指向一个信号量类型的事件指针
 *           LJ_ERR_PEVENT_NULL    ：如果'pevent'是一个NULL指针
 *===========================================================================================================
 */
 #if LJ_SEM_EN > 0 && LJ_SEM_QUERY_EN > 0
 
 INT8U
 LJSemQuery( LJ_EVENT *pevent, LJ_SEM_DATA *pdata )
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
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )
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
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_ERR);
 }
 
 #endif