/*
 *=======================================================================================================
 *                                                 LJ_RTOS
 *                                           The Real-Time Kernel
 * 
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_mutex.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现LJ_RTOS互斥型信号量管理
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/22
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=======================================================================================================
 */
 
 #include <lj_rtos.h>
 
 #define LJ_MUTEX_KEEP_LOWER_8	0x00ff
 #define LJ_MUTEX_KEEP_UPPER_8	0xff00
 
 #define LJ_MUTEX_AVAILABLE		0x00ff
 
/*
 *=======================================================================================================
 * 函数名称：LJMutexCreate
 * 功能描述：创建一个互斥型信号量
 * 
 * 传    参：prio  (INT8U)：优先级继承优先级，当一个低优先级的任务占用这个互斥量时，有一个更高优先级的任务
 *                        ：尝试获得这个信号量，这时就把低优先级任务的优先级提升到这个值
 *           err (INT8U *)：指向错误代码的指针
 *                          LJ_NO_ERR         ：如果这个函数调用成功
 *                          LJ_ERR_CREATE_ISR ：如果尝试在中断服务子程序中创建一个互斥量
 *                          LJ_PRIO_EXIST     ：如果指定的PIP已经被一个任务所占用了
 *                          LJ_ERR_PEVENT_NULL：如果没有多余的事件控制块可利用
 *                          LJ_PRIO_INVALID   ：如果指定的优先级继承优先级无效(prio >= LJ_LOWEST_PRIO)
 *
 * 返 回 值：!= (void *)0 ：一个指向事件控制块的指针
 *           == (void *)0 ：如果有错误
 *
 * 注    意：.ljEventCnt的高8位用于保存PIP的值，而低8位在资源无任务占用时的值为0xff,在有任务占用时为
 *           占用mutex的任务优先级
 *=======================================================================================================
 */
 #if LJ_MUTEX_EN > 0
 
 LJ_EVENT *
 LJMutexCreate( INT8U prio, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_EVENT	*pevent;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在ISR中创建互斥型信号量               */
 	{
 		*err = LJ_ERR_CREATE_ISR;
 		return ((LJ_EVENT *)0);
 	}
 #if LJ_ARG_CHK_EN
 	if ( prio >= LJ_LOWEST_PRIO )						/* 确保任务的PIP有效                             */
 	{
 		*err = LJ_PRIO_INVALID;
 		return ((LJ_EVENT *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbPrioTbl[prio] != (LJ_TCB *)0 )			/* 确认PIP没有被任务所占用                       */
 	{
 		*err = LJ_PRIO_EXIST;
 		LJ_EXIT_CRITICAL( );
 		return ((LJ_EVENT *)0);
 	}
 	gLJTcbPrioTbl[prio] = (LJ_TCB *)1;					/* 保留这个优先级，防止使用这个优先级建立别的任务*/
 	pevent				= gLJEventFreeList;				/* 试图从空余事件控制块得到一个事件控制块ECB     */
 	if ( pevent == (LJ_EVENT *)0 )
 	{
 		gLJTcbPrioTbl[prio] = (LJ_TCB *)0;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_ERR_PEVENT_NULL;
 		return (pevent);
 	}
 	gLJEventFreeList = gLJEventFreeList->ljEventPtr;
 	LJ_EXIT_CRITICAL( );
 	pevent->ljEventType	= LJ_EVENT_TYPE_MUTEX;
 	pevent->ljEventCnt	= (prio << 8) | LJ_MUTEX_AVAILABLE;	/* 置MUTEX有效，同时将PIP保存起来            */
 	pevent->ljEventPtr	= (void *)0;
 	LJ_EventWaitListInit( pevent );						/* 初始化等待事件的任务列表                      */
 	*err = LJ_NO_ERR;
 	
 	return (pevent);
 }
 
 #endif
 
/*
 *========================================================================================================
 * 函数名称：LJMutexPend
 * 功能描述：等待一个互斥型信号量
 *
 * 传    参：pevent (LJ_EVENT *)：指向一个互斥型信号量ECB的指针
 *           timeout    (INT16U)：等待超时的时间(时钟节拍数)
 *           err       (INT8U *)：指向错误码的指针：
 *                                LJ_NO_ERR         ：如果函数调用成功
 *                                LJ_TIMEOUT        ：如果任务在指定的时间内还没有等到互斥型信号量
 *                                LJ_ERR_EVENT_TYPE ：如果传递的不是一个互斥型信号量类型的指针
 *                                LJ_ERR_PEVENT_NULL：'pevent'是一个空指针
 *                                LJ_ERR_PEND_ISR   ：如果是在中断服务子程序中调用此函数
 *
 * 返 回 值：void
 *========================================================================================================
 */
 #if LJ_MUTEX_EN > 0
 
 void
 LJMutexPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	INT8U		 pip;
 	INT8U		 mprio;
 	BOOLEAN		 rdy;
 	LJ_TCB		*ptcb;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是中断服务子程序调用该函数               */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return;
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保'pevent'指针不是空指针'NULL'                */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return;
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )	/* 确保'pevent'指向的是互斥型ECB指针               */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return;
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	/* 
 	 * 如果.ljEventCnt的低8位等于0xff(等于LJ_MUTEX_AVAILABLE), 这个mutex有效。LJMutexPend()将mutex赋予调用
 	 * 该函数的任务，且置.ljEventCnt的低8位等于该任务的优先级，并将.ljEventPtr指向调用该函数的任务的任务控
 	 * 制块TCB, 然后返回.
 	 */
 	if ( (INT8U)(pevent->ljEventCnt & LJ_MUTEX_KEEP_LOWER_8) == LJ_MUTEX_AVAILABLE )
 	{
 		pevent->ljEventCnt &= LJ_MUTEX_KEEP_UPPER_8;		
 		pevent->ljEventCnt |= gLJTcbCur->ljTcbPrio;
 		pevent->ljEventPtr  = (void *)gLJTcbCur;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		return;
 	}
 	
 	/*
 	 * 如果mutex被另外的任务占用，那么调用任务应进入休眠态，直到占用mutex的任务释放了mutex, 或者等待超时
 	 * 如果timeout != 0, 则LJMutexPend()将任务挂起直到timeout的超时时间到
 	 * 如果timeout == 0, 则任务将无限期地等待mutex
 	 */
 	pip		= (INT8U)(pevent->ljEventCnt >> 8);			/* 提取mutex中的PIP                               */
 	mprio	= (INT8U)(pevent->ljEventCnt & LJ_MUTEX_KEEP_LOWER_8);	/* 提取占用mutex的任务的优先级        */
 	ptcb	= (LJ_TCB *)(pevent->ljEventPtr);			/* 提取指向占用mutex任务的任务控制块指针          */
 	
 	/*
 	 * 如果占用mutex任务的优先级比调用LJMutexPend()函数的任务的优先级低，占用mutex的任务的优先级就被提升到
 	 * mutex的优先级继承优先级pip
 	 */
 	if ( ptcb->ljTcbPrio != pip && mprio > gLJTcbCur->ljTcbPrio )
 	{
 		if ( (gLJRdyTbl[ptcb->ljTcbY] & ptcb->ljTcbBitX) != 0x00 )	/* 确认占用mutex的任务是否进入就绪态  */
 		{
 			if ( (gLJRdyTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0x00 )
 			{
 				gLJRdyGrp &= ~ptcb->ljTcbBitY;
 			}
 			
 			rdy = LJ_TRUE;
 		}
 		else
 		{
 			rdy = LJ_FALSE;
 		}
 		ptcb->ljTcbPrio	= pip;
	 	ptcb->ljTcbY	= ptcb->ljTcbPrio >> 3;
	 	ptcb->ljTcbBitY	= gLJMapTbl[ptcb->ljTcbY];
	 	ptcb->ljTcbX	= ptcb->ljTcbPrio & 0x07;
	 	ptcb->ljTcbBitX	= gLJMapTbl[ptcb->ljTcbX];
	 	if ( rdy == LJ_TRUE )
	 	{
	 		gLJRdyGrp				|= ptcb->ljTcbBitY;
	 		gLJRdyTbl[ptcb->ljTcbY] |= ptcb->ljTcbBitX;
	 	}
	 	gLJTcbPrioTbl[pip]	= (LJ_TCB *)ptcb;
 	}
 	gLJTcbCur->ljTcbStat |= LJ_STAT_MUTEX;
 	gLJTcbCur->ljTcbDly   = timeout;
 	LJ_EventTaskWait( pevent );								/* 使当前调用LJMutexPend()的任务进入休眠态    */
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbCur->ljTcbStat & LJ_STAT_MUTEX )
 	{
 		LJ_EventTo( pevent );								/* 由于等待超时而唤醒任务                     */
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_TIMEOUT;
 		return;
 	}
 	gLJTcbCur->ljTcbEventPtr = (LJ_EVENT *)0;
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_NO_ERR;
 }
 
 #endif
 
/*
 *========================================================================================================
 * 函数名称：LJMutexPost
 * 功能描述：释放一个互斥型信号量
 *
 * 传    参：pevent (LJ_EVENT *)    ：指向互斥型信号量ECB的指针
 *
 * 返 回 值：LJ_NO_ERR              ：如果函数调用成功
 *           LJ_ERR_EVENT_TYPE      ：如果传递的不是一个指向互斥型信号量ECB的指针
 *           LJ_ERR_PEVENT_NULL     ：如果传递的'pevent'是NULL指针
 *           LJ_ERR_POST_ISR        ：如果该函数是在中断服务子程序中调用的
 *           LJ_ERR_NOT_MUTEX_OWNER ：如果调用该函数的任务不是占用指定互斥型信号量的任务
 *========================================================================================================
 */
 #if LJ_MUTEX_EN > 0
 
 INT8U
 LJMutexPost( LJ_EVENT *pevent )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	INT8U		 pip;
 	INT8U		 prio;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是中断服务子程序中调用该函数             */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保'pevent'不是NULL指针                       */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pip		= (INT8U)(pevent->ljEventCnt >> 8);
 	prio	= (INT8U)(pevent->ljEventCnt & LJ_MUTEX_KEEP_LOWER_8);
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_ERR_EVENT_TYPE);
 	}
 	if ( gLJTcbCur->ljTcbPrio != pip && 				/* 确认释放mutex的任务确实是占用mutex的任务       */
 	     gLJTcbCur->ljTcbPrio != prio )	
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_ERR_NOT_MUTEX_OWNER);
 	}
 #endif
 	if ( gLJTcbCur->ljTcbPrio == pip )					/* 查看占用mutex的任务的优先级是否已经升到了PIP   */
 	{
 		/* 
 		 * 将调用本函数的任务从任务就绪表中PIP位置上删去，放回到任务就绪表原来的优先级位置上
 		 */
 		if ( (gLJRdyTbl[gLJTcbCur->ljTcbY] &= ~gLJTcbCur->ljTcbBitX) == 0 )	
 		{
 			gLJRdyGrp &= ~gLJTcbCur->ljTcbBitY;
 		}
 		gLJTcbCur->ljTcbPrio			= prio;
 		gLJTcbCur->ljTcbY				= prio >> 3;
 		gLJTcbCur->ljTcbBitY			= gLJMapTbl[gLJTcbCur->ljTcbY];
 		gLJTcbCur->ljTcbX				= prio & 0x07;
 		gLJTcbCur->ljTcbBitX			= gLJMapTbl[gLJTcbCur->ljTcbX];
 		gLJRdyGrp			  	       |= gLJTcbCur->ljTcbBitY;
 		gLJRdyTbl[gLJTcbCur->ljTcbY]   |= gLJTcbCur->ljTcbBitX;
 		gLJTcbPrioTbl[prio]				= (LJ_TCB *)gLJTcbCur;
 	}
 	gLJTcbPrioTbl[pip] = (LJ_TCB *)1;
 	if ( pevent->ljEventGrp != 0x00 )					/* 查看是否有正在等待mutex的任务                  */
 	{
 		prio				= LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_MUTEX );
 		pevent->ljEventCnt &= LJ_MUTEX_KEEP_UPPER_8;
 		pevent->ljEventCnt |= prio;						/* 新的占用mutex任务的优先级被保存在mutex的ECB中  */
 		pevent->ljEventPtr  = gLJTcbPrioTbl[prio];
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );									/* 调用LJ_Sched()查看这个进入就绪态的任务是否是当 */
 		return (LJ_NO_ERR);								/* 前就绪态任务中优先级最高的                     */
 	}
 	pevent->ljEventCnt	|= LJ_MUTEX_KEEP_LOWER_8;		/* 如果没有等待mutex的任务则将.ljEventCnt的低8位  */
 														/* 置为0xff, 表明mutex有效，立即可用              */
 	pevent->ljEventPtr	 = (void *)0;
 	LJ_EXIT_CRITICAL( );
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *=========================================================================================================
 * 函数名称：LJMutexDel
 * 功能描述：删除一个互斥型信号量
 *
 * 传    参：pevent (LJ_EVENT *)：指向一个互斥型信号量ECB的指针
 *           opt         (INT8U)：删除选项
 *           err       (INT8U *)：指向错误代码的指针
 *                                LJ_NO_ERR          ：如果该函数调用成功
 *                                LJ_ERR_DEL_ISR     ：如果是在中断服务子程序中调用该函数
 *                                LJ_ERR_INVALID_OPT ：如果指定的删除选项不合法
 *                                LJ_ERR_TASK_WAITING：如果有任务等待互斥型信号量
 *                                LJ_ERR_EVENT_TYPE  ：如果传入的指针不是指向互斥型信号量ECB的指针
 *                                LJ_ERR_PEVENT_NULL ：如果传入的指针是NULL指针
 *
 * 返 回 值：pevent            ：如果出现上述的错误
 *           (LJ_EVENT *)0     ：成功删除互斥型信号量
 *========================================================================================================
 */
 #if LJ_MUTEX_EN > 0 && LJ_MUTEX_DEL_EN > 0
 
 LJ_EVENT *
 LJMutexDel( LJ_EVENT *pevent, INT8U opt, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	BOOLEAN		 tasksWaiting;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在终端服务子程序中调用该函数           */
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
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )	/* 确保'pevent'是指向互斥型信号量ECB的指针        */
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
 		case LJ_DEL_NO_PEND:							/* 没有任务等待这个mutex时，将ECB标明为未使用，并 */
 			if ( tasksWaiting == LJ_FALSE )				/* 送回到空余事件控制块列表中                     */
 			{
 				pevent->ljEventType = LJ_EVENT_TYPE_UNUSED;
 				pevent->ljEventPtr  = gLJEventFreeList;
 				gLJEventFreeList	= pevent;
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
 			
 		case LJ_DEL_ALWAYS:
 			while ( pevent->ljEventGrp != 0x00 )
 			{
 				LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_MUTEX );
 			}
 			pevent->ljEventType = LJ_EVENT_TYPE_UNUSED;
 			pevent->ljEventPtr  = gLJEventFreeList;
 			gLJEventFreeList	= pevent;
 			LJ_EXIT_CRITICAL( );
 			if ( tasksWaiting == LJ_TRUE )				/* 只有当有任务在等待这个mutex时，才调用调度函数 */
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
 *========================================================================================================
 * 函数名称：LJMutexAccept
 * 功能描述：无等待的获取互斥型信号量(任务不挂起)
 *
 * 传    参：pevent (LJ_EVENT *)：指向互斥型信号量ECB的指针
 *           err       (INT8U *)：指向错误代码的指针
 *                                LJ_NO_ERR         ：如果该函数调用成功
 *                                LJ_ERR_EVENT_TYPE ：如果'pevent'不是指向一个互斥型信号量ECB的指针
 *                                LJ_ERR_PEVENT_NULL：如果'pevent'为NULL指针
 *                                LJ_ERR_PEND_ISR   ：如果该函数是在中断服务子程序中调用
 *
 * 返 回 值：== 1               ：如果有互斥型信号量可利用
 *           == 0               ：a) 如果没有互斥型信号量可利用
 *                                b) 如果'pevent'不是指向互斥型信号量ECB的指针
 *                                c) 如果该函数是在中断服务子程序中被调用的
 *========================================================================================================
 */
 #if LJ_MUTEX_EN > 0 && LJ_MUTEX_ACCEPT_EN > 0
 
 INT8U
 LJMutexAccept( LJ_EVENT *pevent, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 	if ( gLJIntNesting > 0 )							/* 确保不是在中断服务子程序中调用该函数           */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return (0);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保'pevent'不是NULL指针                       */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return (0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )	/* 确保'pevent'是指向互斥型信号量ECB的指针        */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return (0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( (pevent->ljEventCnt & LJ_MUTEX_KEEP_LOWER_8) == LJ_MUTEX_AVAILABLE )
 	{
 		pevent->ljEventCnt &= LJ_MUTEX_KEEP_UPPER_8;
 		pevent->ljEventCnt |= gLJTcbCur->ljTcbPrio;
 		pevent->ljEventPtr  = (void *)gLJTcbCur;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		return (1);
 	}
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_NO_ERR;
 	return (0);
 }
 
 #endif
 
/*
 *========================================================================================================
 * 函数名称：LJMutexQuery
 * 功能描述：获取互斥型信号量的当前状态
 *
 * 传    参：pevent     (LJ_EVENT *)：指向互斥型信号量ECB的指针
 *           pdata (LJ_MUTEX_DATA *)：互斥型信号量数据LJ_MUTEX_DATA的指针
 *
 * 返 回 值：LJ_NO_ERR              ：如果调用该函数成功
 *           LJ_ERR_QUERY_ISR       ：如果是在中断服务子程序中调用该函数的
 *           LJ_ERR_PEVENT_NULL     ：如果'pevent'是NULL指针
 *           LJ_ERR_EVENT_TYPE      ：如果'pevent'不是指向互斥型信号量ECB的指针
 *========================================================================================================
 */
 #if LJ_MUTEX_EN > 0 && LJ_MUTEX_QUERY_EN > 0
 
 INT8U
 LJMutexQuery( LJ_EVENT *pevent, LJ_MUTEX_DATA *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	INT8U		*psrc;
 	INT8U		*pdest;
 	INT8U		 i;
 	
 	if ( gLJIntNesting > 0 )							/* 确保不是在中断服务子程序中调用该函数           */
 	{
 		return (LJ_ERR_QUERY_ISR);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* 确保'pevent'不是NULL指针                       */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pdata->ljMutexPIP	= (INT8U)(pevent->ljEventCnt >> 8);
 	pdata->ljOwnerPrio	= (INT8U)(pevent->ljEventCnt & LJ_MUTEX_KEEP_LOWER_8);
 	if ( pdata->ljOwnerPrio == 0xff )
 	{
 		pdata->ljValue = 1;
 	}
 	else
 	{
 		pdata->ljValue = 0;
 	}
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