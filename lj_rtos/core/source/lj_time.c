/*
 *=================================================================================================
 *                                               LJ_RTOS
 *                                        The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_time.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��LJ_RTOSʱ�������
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/20
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *================================================================================================
 * �������ƣ�LJTimeDly
 * ������������������ʱ�ض���ʱ�ӽ���
 * ��    �Σ�ticks	(INT16U)��ʱ�ӽ�����
 * �� �� ֵ��void
 *================================================================================================
 */
 void
 LJTimeDly( INT16U ticks )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	cpuSR;
 #endif
 
 	if ( ticks > 0 )						/* ��ָ��0ֵ�����ʾ��������������ʱ��������������  */
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
 * �������ƣ�LJTimeDlyHMSM
 * ����������ʹ������ʱָ����ʱ���֡��롢����
 *
 * ��    �Σ�hours   (INT8U)��������ʱ��Сʱ��
 *           minutes (INT8U)��������ʱ�ķ�����
 *           seconds (INT8U)��������ʱ������
 *           milli  (INT16U)��������ʱ�ĺ�����
 *
 * �� �� ֵ��LJ_NO_ERR
 *           LJ_TIME_INVALID_MINUTES
 *           LJ_TIME_INVALID_SECONDS
 *           LJ_TIME_INVALID_MILLI
 *           LJ_TIME_ZERO_DLY
 *
 * ע    �⣺�������ָ������ʱʱ���������ʱ�ӽ��ĵ����ʣ��㲻����ʱ10ms������ʱ�ӽ����ж���
 *           ÿ100ms����һ��ʱ�ӽ����ж�
 *===============================================================================================
 */
 #if LJ_TIME_DLY_HMSM_EN > 0
 
 INT8U
 LJTimeDlyHMSM( INT8U hours, INT8U minutes, INT8U seconds, INT16U milli )
 {
 	INT32U		ticks;
 	INT16U		loops;
 	
 	if ( hours > 0 || minutes > 0 || seconds > 0 || milli > 0 )	/* ��������Ƿ�������Чֵ      */
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
 	
 	
	 	ticks = (INT32U)hours	 * 3600 * LJ_TICKS_PER_SEC	   /* ���㽫������ʱ��ʱ�ӽ�����     */
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
 * �������ƣ�LJTimeDlyResume
 * �����������ָ���ʱ������, ͨ����������ȡ����ʱ��ʹ�Լ����ھ���̬
 *
 * ��    �Σ�prio (INT8U)����Ҫ��ʱ��������ȼ�
 *
 * �� �� ֵ��LJ_NO_ERR        ����������ɹ�
 *           LJ_PRIO_INVALID  ����������ȼ���Ч(prio > LJ_LOWEST_PRIO)
 *           LJ_TIME_NOT_DLY  ������û�б���ʱ
 *           LJ_TASK_NOT_EXIST����Ҫ���ѵ����񲻴���
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
 * �������ƣ�LJTimeGet
 * ������������ȡϵͳʱ��
 * ��    �Σ�void
 * �� �� ֵ��ticks (INT32U)����ǰϵͳʱ�ӽ�����
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
 * �������ƣ�LJTimeSet
 * ��������������ϵͳʱ��
 * ��    �Σ�ticks (INT32U)��ϵͳʱ�ӽ�����
 * �� �� ֵ��void
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