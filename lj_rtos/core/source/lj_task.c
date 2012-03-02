/*
 *================================================================================================================
 *                                           LJ_RTOS
 *                                    The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_task.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现与任务管理有关的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/21
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *================================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *================================================================================================================
 * 函数名称：LJTaskCreate
 * 功能描述：创建一个任务
 * 传    参：task  (void (*)(void *pd))： 指向任务代码的指针
 *           pdata             (void *)： 任务开始执行时，传递给任务的参数指针
 *           ptos            (LJ_STK *)： 是分配给任务的堆栈的栈顶指针
 *           prio               (INT8U)： 分配给任务的优先级 
 *
 * 返 回 值：LJ_NO_ERR                 ： 如果函数调用成功
 *           LJ_PRIO_EXIT              ： 如果任务优先级已经存在
 *           LJ_PRIO_INVALID           ： 如果任务的优先级最大允许的优先级值
 *                                        ( prio >= LJ_LOWEST_PRIO)
 *================================================================================================================
 */
 #if LJ_TASK_CREATE_EN > 0
 
 INT8U
 LJTaskCreate( void (*task)( void *pd ), void *pdata, LJ_STK *ptos, INT8U prio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	void		*psp;
 	INT8U		 err;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio > LJ_LOWEST_PRIO )
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbPrioTbl[prio] == (LJ_TCB *)0 )
 	{
 		gLJTcbPrioTbl[prio] = (LJ_TCB *)1;
 		LJ_EXIT_CRITICAL( );
 		psp = (void *)LJTaskStkInit( task, pdata, ptos, 0 );				/* 建立任务的堆栈                            */
 		err = LJ_TcbInit( prio, psp, (void *)0, 0, 0, (void *)0, 0 );
 		if ( err == LJ_NO_ERR )
 		{
 			LJ_ENTER_CRITICAL( );
 			gLJTaskCtr++;
 			LJ_EXIT_CRITICAL( );
 			if ( gLJRunning == LJ_TRUE )			/* 若LJTaskCreate是在某个任务执行过程中被调用，则任务函数会被调用 */
 			{
 				LJ_Sched( );
 			}
 		}
 		else
 		{
 			LJ_ENTER_CRITICAL( );
 			gLJTcbPrioTbl[prio] = (LJ_TCB *)0;
 			LJ_EXIT_CRITICAL( );
 		}
 		
 		return (err);
 	}
 	
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_PRIO_EXIST);
 }
 
 #endif
 
/*
 *=======================================================================================================================
 * 函数名称：LJTaskCreateExt
 * 功能描述：用于创建一个任务，该任务用户可以对其用户控制块进行扩展
 *
 * 传    参：task  (void (*)(void *pd))：指向任务代码的指针
 *           pdata             (void *)：任务开始执行时，传递给任务的参数指针
 *           ptos            (LJ_STK *)：是分配给任务的堆栈的栈顶指针
 *           prio               (INT8U)：分配给任务的优先级
 *           id				   (INT16U)：为要建立的任务创建一个特殊的标志符
 *           pbos            (LJ_STK *)：指向任务堆栈栈底的指针
 *           stkSize           (INT32U)：指定堆栈的容量
 *           pext              (void *)：指向用户附加的数据域的指针，用来扩展任务的任务控制块
 *           opt               (INT16U)：用来设定选项，指定是否允许堆栈检验
 *
 * 返 回 值：LJ_NO_ERR                 ：如果函数调用成功
 *           LJ_PRIO_EXIT              ：如果任务优先级已经存在
 *           LJ_PRIO_INVALID           ：如果任务的优先级最大允许的优先级值
 *                                       ( prio >= LJ_LOWEST_PRIO)
 *=======================================================================================================================
 */
 #if LJ_TASK_CREATE_EXT_EN > 0
 
 INT8U
 LJTaskCreateExt( void		(*task)( void *pd ),
                  void       *pdata,
                  LJ_STK     *ptos,
                  INT8U       prio,
                  INT16U      id,
                  LJ_STK     *pbos,
                  INT32U      stkSize,
                  void       *pext,
                  INT16U      opt )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_STK		*psp;
 	INT8U		 err;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio > LJ_LOWEST_PRIO )
 	{
 		return LJ_PRIO_INVALID;
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbPrioTbl[prio] == (LJ_TCB *)0 )
 	{
 		gLJTcbPrioTbl[prio] = (LJ_TCB *)1;
 		
 		LJ_EXIT_CRITICAL( );
 		
 		if ( ((opt & LJ_TASK_OPT_STK_CHK) != 0x0000) ||
 		     ((opt & LJ_TASK_OPT_STK_CLR) != 0x0000) )
 		{
 			#if LJ_STK_GROWTH == 1
 			(void)memset( pbos, 0, stkSize * sizeof (LJ_STK) );
 			#else
 			(void)memset( ptos, 0, stkSize * sizeof (LJ_STK) );
 			#endif
 		}
 		
 		psp = (LJ_STK *)LJTaskStkInit( task, pdata, ptos, opt );
 		err = LJ_TcbInit( prio, psp, pbos, id, stkSize, pext, opt );
 		if ( err == LJ_NO_ERR )
 		{
 			LJ_ENTER_CRITICAL( );
 			gLJTaskCtr++;
 			LJ_EXIT_CRITICAL( );
 			if ( gLJRunning == LJ_TRUE )
 			{
 				LJ_Sched( );
 			}
 		}
 		else
 		{
 			LJ_ENTER_CRITICAL( );
 			gLJTcbPrioTbl[prio] = (LJ_TCB *)0;
 			LJ_EXIT_CRITICAL( );
 		}
 		
 		return (err);
 	}
 	
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_PRIO_EXIST);
 }
 
 #endif
 
/*
 *================================================================================================================
 * 函数名称：LJTaskChangePrio
 * 功能描述：用于改变任务的优先级
 *
 * 传    参：oldPrio (INT8U)：任务原先优先级
 *           newPrio (INT8U)：新的任务优先级
 *
 * 返 回 值：LJ_NO_ERR      ：函数调用成功
 *           LJ_PRIO_INVALID：任务优先级无效(prio > LJ_LOWEST_PRIO)
 *           LJ_PRIO_EXIST  ：新的任务优先级已经存在
 *           LJ_PRIO_ERR    ：指定要修改任务的优先级不存在
 *================================================================================================================
 */
 #if LJ_TASK_CHANGE_PRIO_EN > 0
 
 INT8U
 LJTaskChangePrio( INT8U oldPrio, INT8U newPrio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 #if LJ_EVENT_EN > 0
 	LJ_EVENT	*pevent;
 #endif
 
 	LJ_TCB		*ptcb;
 	INT8U		 x;
 	INT8U		 y;
 	INT8U		 bitX;
 	INT8U		 bitY;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( (oldPrio >= LJ_LOWEST_PRIO && oldPrio != LJ_PRIO_SELF) ||
 	      newPrio >= LJ_LOWEST_PRIO )
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbPrioTbl[newPrio] != (LJ_TCB *)0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_PRIO_EXIST);
 	}
 	else
 	{
 		gLJTcbPrioTbl[newPrio] = (LJ_TCB *)1;
 		LJ_EXIT_CRITICAL( );
 		y		= newPrio >> 3;
 		bitY	= gLJMapTbl[y];
 		x		= newPrio & 0x07;
 		bitX	= gLJMapTbl[x];
 		LJ_ENTER_CRITICAL( );
 		if ( oldPrio == LJ_PRIO_SELF )									/* 查看是否修改它自身的优先级                    */
 		{
 			oldPrio = gLJTcbCur->ljTcbPrio;								/* 获取优先级                                    */
 		}
 		ptcb = gLJTcbPrioTbl[oldPrio];
 		if ( ptcb != (LJ_TCB *)0 )										/* 要改变优先级的任务必须存在                    */
 		{
 			gLJTcbPrioTbl[oldPrio] = (LJ_TCB *)0;						/* 移除任务控制块                                */
 			if ( (gLJRdyTbl[ptcb->ljTcbY] & ptcb->ljTcbBitX) != 0x00 )	/* 将任务从就绪表中移除，如果其在就绪表中的话    */
 			{
 				if ( (gLJRdyTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0x00 )
 				{
 					gLJRdyGrp	&= ~ptcb->ljTcbBitY;
 				}
 				gLJRdyGrp		|= bitY;
 				gLJRdyTbl[y]	|= bitX;
#if LJ_EVENT_EN > 0
 			}
 			else
 			{
 				pevent = ptcb->ljTcbEventPtr;							/* 从等待事件任务列表中移除需要修改优先级的任务  */
 				if ( pevent != (LJ_EVENT *)0 )
 				{
 					if ( (pevent->ljEventTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0 )
 					{
 						pevent->ljEventGrp		|= bitY;
 						pevent->ljEventTbl[y]	|= bitX;
 					}
 				}
#endif
 			}
 			
 			gLJTcbPrioTbl[newPrio]	= ptcb;
 			ptcb->ljTcbPrio			= newPrio;
 			ptcb->ljTcbY			= y;
 			ptcb->ljTcbX			= x;
 			ptcb->ljTcbBitY			= bitY;
 			ptcb->ljTcbBitX			= bitX;
 			LJ_EXIT_CRITICAL( );
 			LJ_Sched( );												/* 运行最高优先级任务                             */
 			return (LJ_NO_ERR);
 		}
 		else
 		{
 			gLJTcbPrioTbl[newPrio] = (LJ_TCB *)0;
 			LJ_EXIT_CRITICAL( );
 			return (LJ_PRIO_ERR);
 		}
 	}
 }
 #endif
 
/*
 *========================================================================================================================
 * 函数名称：LJTaskStkChk
 * 功能描述：堆栈检验，用于确定任务实际需要的堆栈空间的大小
 *
 * 传    参：prio          (INT8U)：任务的优先级
 *           pdata (LJ_STK_DATA *)：任务堆栈数据大小
 *
 * 返 回 值：LJ_NO_ERR        ：函数执行成功
 *           LJ_PRIO_INVALID  ：任务优先级超过了允许的优先级(prio > LJ_LOWEST_PRIO)
 *           LJ_TASK_NOT_EXIST：任务不存在
 *           LJ_TASK_OPT_ERR  ：选项错误，即：没有声明LJ_TASK_OPT_STK_CHK当任务被创建时
 *========================================================================================================================
 */
 #if LJ_TASK_CREATE_EXT_EN > 0
 
 INT8U
 LJTaskStkChk( INT8U prio, LJ_STK_DATA *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_TCB		*ptcb;
 	LJ_STK		*pChk;
 	INT32U		 free;
 	INT32U		 size;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio > LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )					/* 检验定义的优先级是否在有效范围内           */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 	pdata->ljFree	= 0;
 	pdata->ljUsed	= 0;
 	LJ_ENTER_CRITICAL( );
 	if ( prio == LJ_PRIO_SELF )
 	{
 		prio = gLJTcbCur->ljTcbPrio;
 	}
 	ptcb = gLJTcbPrioTbl[prio];
 	if ( ptcb == (LJ_TCB *)0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_TASK_NOT_EXIST);
 	}
 	if ( (ptcb->ljTcbOpt & LJ_TASK_OPT_STK_CHK) == 0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_TASK_OPT_ERR);
 	}
 	free = 0;
 	size = ptcb->ljTcbStkSize;
 	pChk = ptcb->ljTcbStkBottom;
 	LJ_EXIT_CRITICAL( );
 #if LJ_STK_GROWTH == 1
 	while ( *pChk++ == (LJ_STK)0 )
 	{
 		free++;
 	}
 #else
 	while ( *pChk-- == (LJ_STK)0 )
 	{
 		free++;
 	}
 #endif
 	pdata->ljFree = free * sizeof (LJ_STK);
 	pdata->ljUsed = (size - free) * sizeof (LJ_STK);
 	
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *=======================================================================================================================
 * 函数名称：LJTaskDel
 * 功能描述：删除任务(是将任务返回并处于休眠状态，并不是说任务代码被删除了)
 *
 * 传    参：prio     (INT8U)：将要删除任务的优先级
 *
 * 返 回 值：LJ_NO_ERR       ：函数调用成功
 *           LJ_TASK_DEL_IDLE：如果要删除的任务是空闲任务
 *           LJ_PRIO_INVALID ：任务的优先级无效
 *           LJ_TASK_DEL_ERR ：将要删除的任务不存在
 *           LJ_TASK_DEL_ISR ：如果尝试在ISR中删除任务
 *======================================================================================================================
 */
 #if LJ_TASK_DEL_EN > 0
 
 INT8U
 LJTaskDel( INT8U prio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 #if LJ_EVENT_EN > 0
 	LJ_EVENT	*pevent;
 #endif
 	LJ_TCB		*ptcb;
 	
 	if ( gLJIntNesting > 0 )									/* 确保不是在终端服务子程序ISR中试图删除一个任务        */
 	{
 		return (LJ_TASK_DEL_ISR);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( prio == LJ_IDLE_PRIO )									/* 确保被删除的任务不是空闲任务                         */
 	{
 		return (LJ_TASK_DEL_IDLE);
 	}
 	if ( prio >= LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* 确保任务优先级是有效地                               */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 
 	LJ_ENTER_CRITICAL( );
 	if ( prio == LJ_PRIO_SELF )									/* 调用该函数的任务可以通过定义参数LJ_PRIO_SELF删除自己 */
 	{
 		prio = gLJTcbCur->ljTcbPrio;
 	}
 	ptcb = gLJTcbPrioTbl[prio];
 	if ( ptcb != (LJ_TCB *)0 )									/* 确保被删除的任务是存在的                             */
 	{
 		if ( (gLJRdyTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0x00 )	/* 如果被删除的任务处于就绪表中就从就绪表中移除 */
 		{
 			gLJRdyGrp &= ~ptcb->ljTcbBitY;
 		}
#if LJ_EVENT_EN > 0
		pevent = ptcb->ljTcbEventPtr;
		if ( pevent != (LJ_EVENT *)0 )							/* 如果任务处于信号量、互斥型信号量、邮箱、消息队列的等 */
		{														/* 待表中，则它从自己所处的表中被去除 */
			if ( (pevent->ljEventTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0 )
			{
				pevent->ljEventGrp &= ~ptcb->ljTcbBitY;
			}
		}
#endif
		ptcb->ljTcbDly	= 0;
		ptcb->ljTcbStat = LJ_STAT_RDY;
		if ( gLJLockNesting < 255 )
		{
			gLJLockNesting++;
		}
		LJ_EXIT_CRITICAL( );
		LJ_Dummy( );
		LJ_ENTER_CRITICAL( );
		if ( gLJLockNesting > 0 )			/* 在LJTaskDel()重新关中断后，它通过把锁定嵌套计数器减1，重新允许任务调度   */
		{
			gLJLockNesting--;
		}
		gLJTaskCtr--;											/* 给任务计数器减1，说明LJ_RTOS管理的任务少了一个       */
		gLJTcbPrioTbl[prio] = (LJ_TCB *)0;						/* 将指向被删除任务的任务控制块LJ_TCB的指针置为NULL     */
		if ( ptcb->ljTcbPrev == (LJ_TCB *)0 )
		{
			ptcb->ljTcbNext->ljTcbPrev	= (LJ_TCB *)0;
			gLJTcbList					= ptcb->ljTcbNext;
		}
		else
		{
			ptcb->ljTcbPrev->ljTcbNext	= ptcb->ljTcbNext;
			ptcb->ljTcbNext->ljTcbPrev	= ptcb->ljTcbPrev;
		}
		ptcb->ljTcbNext	= gLJTcbFreeList;						/* 将被删除任务的任务控制块LJ_TCB退回到空闲LJ_TCB表中  */
		gLJTcbFreeList	= ptcb;
		LJ_EXIT_CRITICAL( );
		LJ_Sched( );
		
		return (LJ_NO_ERR);
 	}
 	
 	LJ_EXIT_CRITICAL( );
 	return (LJ_TASK_DEL_ERR);
 }
 
 #endif
 
/*
 *======================================================================================================================
 * 函数名称：LJTaskDelReq
 * 功能描述：请求删除任务, 防止任务删除那些占用资源的任务，如果不这样删除，那些被占用的资源会因为没有被释放而丢失
 *
 * 传    参：prio (INT8U)     ：请求被删除任务的优先级
 *
 * 返 回 值：LJ_NO_ERR        ：如果函数调用成功
 *           LJ_TASK_NOT_EXIST：如果被删除的任务已经被删除，这允许调用者直到是否这个请求已被执行
 *           LJ_TASK_DEL_IDLE ：如果请求删除的任务是LJ_RTOS中的空闲任务
 *           LJ_PRIO_INVALID  ：如果请求删除任务的优先级无效(prio >= LJ_LOWEST_PRIO)
 *           LJ_TASK_DEL_REQ  ：一个任务是否被删除的内部标志
 * 使    用：
 *         请求删除其他任务的任务代码：
 *         void RequestorTask( void *pdata )
 *         {
 *         		pdata = pdata;
 *              while ( 1 )
 *              {
 *              	应用程序代码
 *                  if ( 删除任务的条件 )
 *                  {
 *                  	while ( LJTaskDelReq( TASK_TO_DEL_PRIO ) != LJ_TASK_NOT_EXIST )
 *                      {
 *							LJTimeDly( 1 );
 *                      }
 *                  }
 *                  应用程序代码
 *              }
 *         }
 *         需要删除自己的任务代码：
 *         void TaskToBeDeleted( void *pdata )
 *         {
 *				pdata = pdata;
 *              while ( 1 )
 *              {
 *					应用程序代码
 *                  if ( LJTaskDelReq( LJ_PRIO_SELF ) == LJ_TASK_DEL_REQ )
 *                  {
 *						释放所占用的资源
 *                      释放所有动态内存
 *                      LJTaskDel( LJ_PRIO_SELF );
 *                  }
 *                  else
 *                  {
 *						应用程序代码
 *                  }
 *              }
 *         }
 *======================================================================================================================
 */
 #if LJ_TASK_DEL_EN > 0
 
 INT8U
 LJTaskDelReq( INT8U prio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	BOOLEAN		 stat;
 	INT8U		 err;
 	LJ_TCB		*ptcb;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio == LJ_IDLE_PRIO )
 	{
 		return (LJ_TASK_DEL_IDLE);								/* 如果需要被删除的任务是空闲任务，则报错返回           */
 	}
 	if ( prio >= LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* 确保调用者请求删除的任务的优先级是有效的             */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 	if ( prio == LJ_PRIO_SELF )
 	{
 		LJ_ENTER_CRITICAL( );
 		stat = gLJTcbCur->ljTcbDelReq;
 		LJ_EXIT_CRITICAL( );
 		
 		return (stat);
 	}
 	LJ_ENTER_CRITICAL( );
 	ptcb = gLJTcbPrioTbl[prio];
 	if ( ptcb != (LJ_TCB *)0 )
 	{
 		ptcb->ljTcbDelReq	= LJ_TASK_DEL_REQ;
 		err					= LJ_NO_ERR;
 	}
 	else
 	{
 		err					= LJ_TASK_NOT_EXIST;
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (err);
 }
 
 #endif
 
/*
 *=====================================================================================================================
 * 函数名称：LJTaskSuspend
 * 功能描述：挂起任务
 * 
 * 传    参：prio (INT8U)        ：将要挂起任务的优先级
 *
 * 返 回 值：LJ_NO_ERR           ：如果挂起任务函数调用成功
 *           LJ_TASK_SUSPEND_IDLE：如果要挂起的任务是空闲任务则不被允许
 *           LJ_PRIO_INVALID     ：如果要挂起的任务的优先级无效(prio >= LJ_LOWESR_PRIO)
 *           LJ_TASK_SUSPEND_PRIO：如果要挂起的任务不存在
 *=====================================================================================================================
 */
 #if LJ_TASK_SUSPEND_EN > 0
 
 INT8U
 LJTaskSuspend( INT8U prio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	BOOLEAN		 self;
 	LJ_TCB		*ptcb;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio == LJ_IDLE_PRIO )									/* 确保要挂起的任务不是空闲任务                       */
 	{
 		return (LJ_TASK_SUSPEND_IDLE);
 	}
 	if ( prio >= LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* 确保要挂起的任务的优先级有效                       */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( prio == LJ_PRIO_SELF )
 	{
 		prio = gLJTcbCur->ljTcbPrio;
 		self = LJ_TRUE;
 	}
 	else if ( prio == gLJTcbCur->ljTcbPrio )
 	{
 		self = LJ_TRUE;
 	}
 	else
 	{
 		self = LJ_FALSE;
 	}
 	ptcb = gLJTcbPrioTbl[prio];									/* 检验要挂起的任务是否存在                          */
 	if ( ptcb == (LJ_TCB *)0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_TASK_SUSPEND_PRIO);
 	}
 	if ( (gLJRdyTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0x00 )	/* 将要挂起的任务移除就绪表                      */
 	{
 		gLJRdyGrp	&= ~ptcb->ljTcbBitY;
 	}
 	ptcb->ljTcbStat |= LJ_STAT_SUSPEND;
 	LJ_EXIT_CRITICAL( );
 	if ( self == LJ_TRUE )										/* 在要挂起的任务是调用本函数的任务时，进行任务调度  */
 	{
 		LJ_Sched( );
 	}
 	
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *===================================================================================================================
 * 函数名称：LJTaskResume
 * 功能描述：恢复任务
 *
 * 传    参：prio (INT8U)         ：所要恢复的任务的优先级
 *
 * 返 回 值：LJ_NO_ERR            ：如果恢复任务函数调用成功
 *           LJ_PRIO_INVALID      ：如果要恢复的任务的优先级无效(prio >= LJ_LOWEST_PRIO)
 *           LJ_TASK_RESUME_PRIO  ：如果要恢复的任务不存在
 *           LJ_TASK_NOT_SUSPENDED：如果要恢复的任务没有被挂起
 *===================================================================================================================
 */
 #if LJ_TASK_SUSPEND_EN > 0
 
 INT8U
 LJTaskResume( INT8U prio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_TCB		*ptcb;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio >= LJ_LOWEST_PRIO )								/* 确保要恢复任务的优先级有效                       */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	ptcb = gLJTcbPrioTbl[prio];
 	if ( ptcb == (LJ_TCB *)0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_TASK_RESUME_PRIO);
 	}
 	if ( (ptcb->ljTcbStat & LJ_STAT_SUSPEND) != 0x00 )
 	{
 		if ( ((ptcb->ljTcbStat &= ~LJ_STAT_SUSPEND) == LJ_STAT_RDY) &&
 		      (ptcb->ljTcbDly == 0) )
 		{
 			gLJRdyGrp				|= ptcb->ljTcbBitY;
 			gLJRdyTbl[ptcb->ljTcbY] |= ptcb->ljTcbBitX;
 			LJ_EXIT_CRITICAL( );
 			LJ_Sched( );
 		}
 		else
 		{
 			LJ_EXIT_CRITICAL( );
 		}
 		
 		return (LJ_NO_ERR);
 	}
 	
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_TASK_NOT_SUSPENDED);
 }
 
 #endif

/*
 *================================================================================================================
 * 函数名称：LJTaskQuery
 * 功能描述：获得任务的信息
 *
 * 传    参：prio     (INT8U)：想要获得的任务信息的任务优先级
 *           pdata (LJ_TCB *)：想要获得的任务的任务控制块指针
 * 
 * 返 回 值：LJ_NO_ERR       ：如果函数调用成功
 *           LJ_PRIO_INVALID ：如果任务优先级无效(prio >= LJ_LOWEST_PRIO)
 *           LJ_PRIO_ERR     ：如果想要获得的任务信息的任务没有被创建
 *================================================================================================================
 */
 #if LJ_TASK_QUERY_EN > 0
 
 INT8U
 LJTaskQuery( INT8U prio, LJ_TCB *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_TCB		*ptcb;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( prio > LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* 确保任务优先级有效                             */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( prio == LJ_PRIO_SELF )
 	{
 		prio = gLJTcbCur->ljTcbPrio;
 	}
 	ptcb = gLJTcbPrioTbl[prio];
 	if ( ptcb == (LJ_TCB *)0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_PRIO_ERR);
 	}
 	memcpy( pdata, ptcb, sizeof (LJ_TCB) );
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_ERR);
 }
 
 #endif