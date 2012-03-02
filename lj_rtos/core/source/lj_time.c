/*
 *=================================================================================================
 *                                               LJ_RTOS
 *                                        The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_time.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现LJ_RTOS时间管理函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/20
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *================================================================================================
 * 函数名称：LJTimeDly
 * 功能描述：将任务延时特定的时钟节拍
 * 传    参：ticks	(INT16U)：时钟节拍数
 * 返 回 值：void
 *================================================================================================
 */
 void
 LJTimeDly( INT16U ticks )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	cpuSR;
 #endif
 
 	if ( ticks > 0 )						/* 若指定0值，则表示不想对任务进行延时，函数立即返回  */
 	{
 		LJ_ENTER_CRITICAL( );
 		if ( (gLJRdyTbl[gLJTcbCur->ljTcbY] &= ~gLJTcbCur->ljTcbBitX) == 0 )
 		{
 			gLJRdyGrp &= ~gLJTcbCur->ljTcbBitY;
 		}
 		gLJTcbCur->ljTcbDly		= ticks;
 		LJ_EXIT_CRITICAL( );
 		
 		LJ_Sched( );
 	}
 }
 
/*
 *===============================================================================================
 * 函数名称：LJTimeDlyHMSM
 * 功能描述：使任务延时指定的时、分、秒、毫秒
 *
 * 传    参：hours   (INT8U)：任务延时的小时数
 *           minutes (INT8U)：任务延时的分钟数
 *           seconds (INT8U)：任务延时的秒数
 *           milli  (INT16U)：任务延时的毫秒数
 *
 * 返 回 值：LJ_NO_ERR
 *           LJ_TIME_INVALID_MINUTES
 *           LJ_TIME_INVALID_SECONDS
 *           LJ_TIME_INVALID_MILLI
 *           LJ_TIME_ZERO_DLY
 *
 * 注    意：这个函数指定的延时时间必须依靠时钟节拍的速率，你不能延时10ms如果你的时钟节拍中断是
 *           每100ms产生一个时钟节拍中断
 *===============================================================================================
 */
 #if LJ_TIME_DLY_HMSM_EN > 0
 
 INT8U
 LJTimeDlyHMSM( INT8U hours, INT8U minutes, INT8U seconds, INT16U milli )
 {
 	INT32U		ticks;
 	INT16U		loops;
 	
 	if ( hours > 0 || minutes > 0 || seconds > 0 || milli > 0 )	/* 检验参数是否定义了有效值      */
 	{
 		if ( minutes > 59 )
 		{
 			return (LJ_TIME_INVALID_MINUTES);
 		}
 		
 		if ( seconds > 59 )
 		{
 			return (LJ_TIME_INVALID_SECONDS);
 		}
 		
 		if ( milli > 999 )
 		{
 			return (LJ_TIME_INVALID_MILLI);	
 		}
 	
 	
	 	ticks = (INT32U)hours	 * 3600 * LJ_TICKS_PER_SEC	   /* 计算将任务延时的时钟节拍数     */
	 	      + (INT32U)minutes  *   60 * LJ_TICKS_PER_SEC
	 	      + (INT32U)seconds  *        LJ_TICKS_PER_SEC
	 	      + LJ_TICKS_PER_SEC * ((INT32U)milli
	 	      + 500 / LJ_TICKS_PER_SEC) / 1000;      
	 	loops = ticks / 65536;
	 	ticks = ticks % 65536;
	 	LJTimeDly( ticks );
	 	while ( loops > 0 )
	 	{
	 		LJTimeDly( 32768 );
	 		LJTimeDly( 32768 );
	 		loops--;
	 	}
	 	
	 	return (LJ_NO_ERR);
 	}
 	else
 	{
 		return (LJ_TIME_ZERO_DLY);
 	}
 }
 
 #endif
 
/*
 *================================================================================================
 * 函数名称：LJTimeDlyResume
 * 功能描述：恢复延时的任务, 通过其他任务取消延时而使自己处于就绪态
 *
 * 传    参：prio (INT8U)：所要延时任务的优先级
 *
 * 返 回 值：LJ_NO_ERR        ：唤醒任务成功
 *           LJ_PRIO_INVALID  ：任务的优先级无效(prio > LJ_LOWEST_PRIO)
 *           LJ_TIME_NOT_DLY  ：任务没有被延时
 *           LJ_TASK_NOT_EXIST：所要唤醒的任务不存在
 *================================================================================================
 */
 #if LJ_TIME_DLY_RESUME_EN > 0
 
 INT8U
 LJTimeDlyResume( INT8U prio )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_TCB		*ptcb;
 	
 	if ( prio >= LJ_LOWEST_PRIO )
 	{
 		return (LJ_PRIO_INVALID);
 	}
 	LJ_ENTER_CRITICAL( );
 	ptcb = (LJ_TCB *)gLJTcbPrioTbl[prio];
 	if ( ptcb != (LJ_TCB *)0 )
 	{
 		if ( ptcb->ljTcbDly != 0 )
 		{
 			ptcb->ljTcbDly = 0;
 			if ( (ptcb->ljTcbStat & LJ_STAT_SUSPEND) == LJ_STAT_RDY )
 			{
 				gLJRdyGrp				|= ptcb->ljTcbBitY;
 				gLJRdyTbl[ptcb->ljTcbY]	|= ptcb->ljTcbBitX;
 				LJ_EXIT_CRITICAL( );
 				LJ_Sched( );
 			}
 			else
 			{
 				LJ_EXIT_CRITICAL( );
 			}
 			
 			return (LJ_NO_ERR);
 		}
 		else
 		{
 			LJ_EXIT_CRITICAL( );
 			return (LJ_TIME_NOT_DLY);
 		}
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_TASK_NOT_EXIST);
 } 
 
 #endif
 
/*
 *======================================================================================================
 * 函数名称：LJTimeGet
 * 功能描述：获取系统时间
 * 传    参：void
 * 返 回 值：ticks (INT32U)：当前系统时钟节拍数
 *======================================================================================================
 */
 #if LJ_TIME_GET_SET_EN > 0
 
 INT32U
 LJTimeGet( void )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	INT32U		 ticks;
 	
 	LJ_ENTER_CRITICAL( );
 	ticks = gLJTime;
 	LJ_EXIT_CRITICAL( );
 	
 	return (ticks);
 }
 
 #endif
 
/*
 *=====================================================================================================
 * 函数名称：LJTimeSet
 * 功能描述：设置系统时间
 * 传    参：ticks (INT32U)：系统时钟节拍数
 * 返 回 值：void
 *=====================================================================================================
 */
 #if LJ_TIME_GET_SET_EN > 0
 
 void
 LJTimeSet( INT32U ticks )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	cpuSR;
 #endif
 
 	LJ_ENTER_CRITICAL( );
 	gLJTime = ticks;
 	LJ_EXIT_CRITICAL( );
 }
 
 #endif