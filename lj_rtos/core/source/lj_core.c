/*
 *==================================================================================================
 *                                          LJ_RTOS
 *                                    The Real-Time Kernel
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_core.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ����ڶ���LJ_RTOS�ĺ��Ĵ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *==================================================================================================
 */
 
 #define LJ_GLOBAL									/* �ڱ��ļ��ж�ȫ�ֱ������ж���                 */
 #include <lj_rtos.h>
 
/*
 *==================================================================================================
 * ʹ����������̬�������
 *==================================================================================================
 */
 INT8U  const  gLJMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
 
/*
 *==================================================================================================
 * �ҳ��������̬��������ȼ�����������
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
 
/*========================================LJ_RTOS����ԭ��===========================================*/

 static void LJ_InitMisc( void );
 static void LJ_InitRdyList( void );
 static void LJ_InitTcbList( void );
 static void LJ_InitTaskIdle( void );
 static void LJ_InitEventList( void );

/*
 *==================================================================================================
 * �������ƣ�LJInit
 * ������������ʼ��LJ_RTOS
 * ��    �Σ�void
 * �� �� ֵ��void
 *==================================================================================================
 */
 void
 LJInit( void )
 {
 	LJ_InitMisc( );										/* ��ʼ��ȫ�ֱ���                           */
 	
 	LJ_InitRdyList( );									/* ��ʼ��������                             */
 	LJ_InitTcbList( );									/* ��ʼ������������ƿ�����                 */
 	
 	LJ_InitTaskIdle( );									/* ��ʼ����������                           */
 
 	LJ_InitEventList( );								/* ��ʼ�������¼����ƿ�����                 */
 	
 #if (LJ_Q_EN > 0) && (LJ_MAX_QS > 0)
 	LJ_QInit( );										/* ��ʼ�����ж��п��ƿ�����                 */
 #endif
 
 #if (LJ_MEM_EN > 0) && (LJ_MAX_MEM_PART > 0)
 	LJ_MemInit( );										/* ��ʼ�������ڴ���ƿ�����                 */
 #endif
 }
 
/*
 *==================================================================================================
 * �������ƣ�LJStart
 * ������������ʼ������
 * ��    �Σ�void
 * �� �� ֵ��void
 *==================================================================================================
 */
 void
 LJStart( void )
 {
 	INT8U	y;
 	INT8U	x;
 	
 	if ( gLJRunning == LJ_FALSE )
 	{
 														/* ���Ҿ����������ȼ���ߵ�����            	*/
 		y				= gLJUnMapTbl[gLJRdyGrp];		/* ��gLJRdyGrp������ڼ�λ����Ϊ1           */
 		x				= gLJUnMapTbl[gLJRdyTbl[y]];	/* ��ȥgLJRdyTbl[y]�в�������ڼ�λ����Ϊ1  */
 		gLJPrioHighRdy	= (INT8U)((y << 3) + x);		/* ��������ȼ������������ȼ�             */
 		gLJPrioCur		= gLJPrioHighRdy;
 		gLJTcbHighRdy	= gLJTcbPrioTbl[gLJPrioHighRdy];
 		gLJTcbCur		= gLJTcbHighRdy;
 		
 		LJStartHighRdy( );
 	}
 }
 
/*
 *==================================================================================================
 * �������ƣ�LJIntEnter
 * ����������֪ͨLJ_RTOS�жϷ����ӳ���ISR��ʼ
 * ��    �Σ�void
 * �� �� ֵ��void
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
 * �������ƣ�LJIntExit
 * ����������֪ͨLJ_RTOS�뿪�жϷ����ӳ���ISR
 * ��    �Σ�void
 * �� �� ֵ��void
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
 			gLJIntExitY				= gLJUnMapTbl[gLJRdyGrp];	/* ���Ҿ����������ȼ���ߵ�����      */
 			gLJPrioHighRdy			= (INT8U)((gLJIntExitY << 3) + gLJUnMapTbl[gLJRdyTbl[gLJIntExitY]]);
 			
	 		if ( gLJPrioHighRdy != gLJPrioCur )
	 		{
	 			gLJTcbHighRdy = gLJTcbPrioTbl[gLJPrioHighRdy];
	 			gLJCtxSwCtr++;
	 			LJIntCtxSw( );						/* ͨ������LJIntCtxSw(), ��ISR��ִ�������л����� */
	 		}
 		}
 	}
 	LJ_EXIT_CRITICAL( );
 }
 
/*
 *==================================================================================================
 * �������ƣ�LJSchedLock
 * ����������������������
 * ��    �Σ�void
 * �� �� ֵ��void
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
 * �������ƣ�LJSchedUnlock
 * ����������������������
 * ��    �Σ�void
 * �� �� ֵ��void
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
 * �������ƣ�LJVersion
 * ������������ȡ��ǰLJ_RTOS�ں˵İ汾��
 * ��    �Σ�void
 * �� �� ֵ��LJ_VERSION (INT16U)��LJ_RTOS�汾�� * 100
 *==================================================================================================
 */
 INT16U
 LJVersion( void )
 {
 	return (LJ_VERSION);
 }
 
/*
 *==================================================================================================
 * �������ƣ�LJTimeTick
 * ����������ʱ�ӽ��ĺ����Ľ��ķ���
 * ��    �Σ�void
 * �� �� ֵ��void
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
 	gLJTime++;										/* �����ϵͳ�ϵ�������ϵͳʱ�ӽ�����           */
 	LJ_EXIT_CRITICAL( );
 #endif
 	
 	if ( gLJRunning == LJ_TRUE )
 	{
 		ptcb = gLJTcbList;
 													/* LJTimeTick()��gLJTcbList��ʼ������LJ_TCB���� */
 		while ( ptcb->ljTcbPrio != LJ_IDLE_PRIO )	/* ����һֱ������������                         */
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
 					else				/* ȷʵ��������LJTaskSuspend()����������򲻻�������̬  */
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
 * �������ƣ�LJ_Sched
 * �������������������
 * ��    �Σ�void
 * �� �� ֵ��void
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
 	 * ������������жϷ����ӳ�����ߵ�����һ�θ������������������
 	 * ������Ⱥ���LJ_Sched���˳������������
 	 */
 	if ( (gLJIntNesting == 0) && (gLJLockNesting == 0) )
 	{
 		y				= gLJUnMapTbl[gLJRdyGrp];			/* �ҳ�����̬�����ȼ���ߵ�����         */
 		gLJPrioHighRdy	= (INT8U)((y << 3) + gLJUnMapTbl[gLJRdyTbl[y]]);
 		if ( gLJPrioHighRdy != gLJPrioCur )					/* ���������ȼ������Ƿ��ǵ�ǰ����     */
 		{
 			gLJTcbHighRdy = gLJTcbPrioTbl[gLJPrioHighRdy];
 			gLJCtxSwCtr++;
 			LJ_TASK_SW( );									/* ����ã����ʵ���ϵ������л�         */
 		}
 	}
 	LJ_EXIT_CRITICAL( );	
 }
 
/*
 *===================================================================================================
 * �������ƣ�LJ_TaskIdle
 * ������������������ϵͳ��û����������������̬��Ͷ������
 * ��    �Σ�pdata	(void *)���������
 * �� �� ֵ��void
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
 * �������ƣ�LJ_InitMisc
 * ������������ʼ��LJ_RTOS�ж����ȫ�ֱ���
 * ��    �Σ�void
 * �� �� ֵ��static void
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
 * �������ƣ�LJ_InitRdyList
 * ������������ʼ��LJ_RTOS�еľ�����
 * ��    �Σ�void
 * �� �� ֵ��static void
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
 * �������ƣ�LJ_InitTcbList
 * ������������ʼ��LJ_RTOS�еĿ���������ƿ��б�
 * ��    �Σ�void
 * �� �� ֵ��static void
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
 * �������ƣ�LJ_InitTaskIdle
 * ������������ʼ����������
 * ��    �Σ�void
 * �� �� ֵ��static void
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
 * �������ƣ�LJ_InitEventList
 * ������������ʼ�������¼����ƿ�����
 * ��    �Σ�void
 * �� �� ֵ��static void
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
 * �������ƣ�LJ_TcbInit
 * ������������ʼ��������ƿ�
 *
 * ��    �Σ�prio		(INT8U)����������ȼ�
 *           ptos	 (LJ_STK *)��ָ������ջջ����ָ��
 *           pbos	 (LJ_STK *)��ָ������ջջ�׵�ָ��
 *           id	  	   (INT16U)�������־��
 *           stkSize   (INT32U)����ջ������
 *           pext	   (void *)��LJ_TCB�е���չָ��
 *           opt       (INT16U)��LJ_TCB��ѡ����
 *
 * �� �� ֵ��LJ_NO_ERR		   ������������óɹ�
 *           LJ_NO_MORE_TCB    �����û�и���Ŀ���������ƿ鱻���䣬����Ͳ��ܱ�����
 *
 * ע    �⣺�������LJ_RTOS���ڲ�������Ӧ�ó����ܵ��øú���
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
 	ptcb = gLJTcbFreeList;							/* ��ͼ��LJ_TCB������еõ�һ��������ƿ�            */
 	if ( ptcb != (LJ_TCB *)0 )						/* �����������п����LJ_TCB,���LJ_TCB�ͱ���ʼ���� */
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
 		ptcb->ljTcbNext		= gLJTcbList;			/* ��LJ_TCB���뵽�Ѿ����������˫��������            */
 		ptcb->ljTcbPrev		= (LJ_TCB *)0;
 		if ( gLJTcbList != (LJ_TCB *)0 )
 		{
 			gLJTcbList->ljTcbPrev = ptcb;
 		}
 		gLJTcbList			= ptcb;
 		
 		gLJRdyGrp		   		|= ptcb->ljTcbBitY;	/* ʹ����������̬                                 */
 		gLJRdyTbl[ptcb->ljTcbY] |= ptcb->ljTcbBitX;
 		
 		LJ_EXIT_CRITICAL( );
 		
 		return (LJ_NO_ERR);	
 	}
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_MORE_TCB);
 }
 
/*
 *=======================================================================================================
 * �������ƣ�LJ_Dummy
 * �����������������κε�ʵ���Բ���������LJTaskDel()�����ն˿��ŵ����������ִ��һ��ָ��
 * ��    �Σ�void
 * �� �� ֵ��void
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
 * �������ƣ�LJ_EventTaskRdy
 * ������������ĳ���¼�����ʱ�����ȴ����¼������б��е�������ȼ��������ھ���̬
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ���¼����ƿ��ָ��
 *           msg        (void *)��ָ����Ϣ��ָ��
 *           msk         (INT8U)�����������ƿ�״̬λ������
 *
 * �� �� ֵ��prio        (INT8U)�����ؾ�����������ȼ�
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
 	
 	y		= gLJUnMapTbl[pevent->ljEventGrp];			/* ȷ��HPT������gLJEventTbl[]�е��ֽ�����         */
 	bitY	= gLJMapTbl[y];								/* ���ø������õ�HPT������gLJEventGrp�е�λ������ */
 	x		= gLJUnMapTbl[pevent->ljEventTbl[y]];		/* �ж�HPT������gLJEventTbl[]�е���Ӧλ��λ��     */
 	bitX	= gLJMapTbl[x];								/* �õ�HPT������gLJEventTbl[]����Ӧ��λ������     */
 	prio	= (INT8U)((y << 3) + x);					/* ���Ҫ�������̬��������ȼ���������ȼ�       */
 	if ( (pevent->ljEventTbl[y] &= ~bitX) == 0x00 )		/* �ӵȴ������б���ɾ��HPT����                    */
 	{
 		pevent->ljEventGrp &= ~bitY;
 	}
 	ptcb				= gLJTcbPrioTbl[prio];			/* �ҵ�ָ��������������ƿ�TCB��ָ��            */
 	ptcb->ljTcbDly		= 0;
 	ptcb->ljTcbEventPtr = (LJ_EVENT *)0;
 #if ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)) || (LJ_MBOX_EN > 0)
 	ptcb->ljTcbMsg		= msg;							/* ����ǵȴ���Ϣ����Ӧ����Ϣ��Ϊ�������ݸ���   */
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
 * �������ƣ�LJ_EventWaitListInit
 * ������������ʼ��һ���¼����ƿ�ĵȴ������б�
 * ��    �Σ�pEvnet (LJ_EVENT *)��ָ���¼����ƿ��ָ��
 * �� �� ֵ��void
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
 * �������ƣ�LJ_EventTaskWait
 * ����������ʹһ���������ȴ�״̬,ʹ��ǰ����Ӿ����������룬���ŵ���Ӧ���¼����ƿ�ĵȴ������б���
 * ��    �Σ�pevent (LJ_EVENT *)��ָ���¼����ƿ��ָ��
 * �� �� ֵ��void
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
 * �������ƣ�LJ_EventTo
 * �������������ڵȴ���ʱ����������Ϊ����̬
 * ��    �Σ�pevent (LJ_EVENT *)��ָ���¼����ƿ��ָ��
 * �� �� ֵ��void
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