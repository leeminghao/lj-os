/*
 *================================================================================================================
 *                                           LJ_RTOS
 *                                    The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_task.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ������������йصĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/21
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *================================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *================================================================================================================
 * �������ƣ�LJTaskCreate
 * ��������������һ������
 * ��    �Σ�task  (void (*)(void *pd))�� ָ����������ָ��
 *           pdata             (void *)�� ����ʼִ��ʱ�����ݸ�����Ĳ���ָ��
 *           ptos            (LJ_STK *)�� �Ƿ��������Ķ�ջ��ջ��ָ��
 *           prio               (INT8U)�� �������������ȼ� 
 *
 * �� �� ֵ��LJ_NO_ERR                 �� ����������óɹ�
 *           LJ_PRIO_EXIT              �� ����������ȼ��Ѿ�����
 *           LJ_PRIO_INVALID           �� �����������ȼ������������ȼ�ֵ
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
 		psp = (void *)LJTaskStkInit( task, pdata, ptos, 0 );				/* ��������Ķ�ջ                            */
 		err = LJ_TcbInit( prio, psp, (void *)0, 0, 0, (void *)0, 0 );
 		if ( err == LJ_NO_ERR )
 		{
 			LJ_ENTER_CRITICAL( );
 			gLJTaskCtr++;
 			LJ_EXIT_CRITICAL( );
 			if ( gLJRunning == LJ_TRUE )			/* ��LJTaskCreate����ĳ������ִ�й����б����ã����������ᱻ���� */
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
 * �������ƣ�LJTaskCreateExt
 * �������������ڴ���һ�����񣬸������û����Զ����û����ƿ������չ
 *
 * ��    �Σ�task  (void (*)(void *pd))��ָ����������ָ��
 *           pdata             (void *)������ʼִ��ʱ�����ݸ�����Ĳ���ָ��
 *           ptos            (LJ_STK *)���Ƿ��������Ķ�ջ��ջ��ָ��
 *           prio               (INT8U)���������������ȼ�
 *           id				   (INT16U)��ΪҪ���������񴴽�һ������ı�־��
 *           pbos            (LJ_STK *)��ָ�������ջջ�׵�ָ��
 *           stkSize           (INT32U)��ָ����ջ������
 *           pext              (void *)��ָ���û����ӵ��������ָ�룬������չ�����������ƿ�
 *           opt               (INT16U)�������趨ѡ�ָ���Ƿ������ջ����
 *
 * �� �� ֵ��LJ_NO_ERR                 ������������óɹ�
 *           LJ_PRIO_EXIT              ������������ȼ��Ѿ�����
 *           LJ_PRIO_INVALID           �������������ȼ������������ȼ�ֵ
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
 * �������ƣ�LJTaskChangePrio
 * �������������ڸı���������ȼ�
 *
 * ��    �Σ�oldPrio (INT8U)������ԭ�����ȼ�
 *           newPrio (INT8U)���µ��������ȼ�
 *
 * �� �� ֵ��LJ_NO_ERR      ���������óɹ�
 *           LJ_PRIO_INVALID���������ȼ���Ч(prio > LJ_LOWEST_PRIO)
 *           LJ_PRIO_EXIST  ���µ��������ȼ��Ѿ�����
 *           LJ_PRIO_ERR    ��ָ��Ҫ�޸���������ȼ�������
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
 		if ( oldPrio == LJ_PRIO_SELF )									/* �鿴�Ƿ��޸�����������ȼ�                    */
 		{
 			oldPrio = gLJTcbCur->ljTcbPrio;								/* ��ȡ���ȼ�                                    */
 		}
 		ptcb = gLJTcbPrioTbl[oldPrio];
 		if ( ptcb != (LJ_TCB *)0 )										/* Ҫ�ı����ȼ�������������                    */
 		{
 			gLJTcbPrioTbl[oldPrio] = (LJ_TCB *)0;						/* �Ƴ�������ƿ�                                */
 			if ( (gLJRdyTbl[ptcb->ljTcbY] & ptcb->ljTcbBitX) != 0x00 )	/* ������Ӿ��������Ƴ���������ھ������еĻ�    */
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
 				pevent = ptcb->ljTcbEventPtr;							/* �ӵȴ��¼������б����Ƴ���Ҫ�޸����ȼ�������  */
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
 			LJ_Sched( );												/* ����������ȼ�����                             */
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
 * �������ƣ�LJTaskStkChk
 * ������������ջ���飬����ȷ������ʵ����Ҫ�Ķ�ջ�ռ�Ĵ�С
 *
 * ��    �Σ�prio          (INT8U)����������ȼ�
 *           pdata (LJ_STK_DATA *)�������ջ���ݴ�С
 *
 * �� �� ֵ��LJ_NO_ERR        ������ִ�гɹ�
 *           LJ_PRIO_INVALID  ���������ȼ���������������ȼ�(prio > LJ_LOWEST_PRIO)
 *           LJ_TASK_NOT_EXIST�����񲻴���
 *           LJ_TASK_OPT_ERR  ��ѡ����󣬼���û������LJ_TASK_OPT_STK_CHK�����񱻴���ʱ
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
 	if ( prio > LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )					/* ���鶨������ȼ��Ƿ�����Ч��Χ��           */
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
 * �������ƣ�LJTaskDel
 * ����������ɾ������(�ǽ����񷵻ز���������״̬��������˵������뱻ɾ����)
 *
 * ��    �Σ�prio     (INT8U)����Ҫɾ����������ȼ�
 *
 * �� �� ֵ��LJ_NO_ERR       ���������óɹ�
 *           LJ_TASK_DEL_IDLE�����Ҫɾ���������ǿ�������
 *           LJ_PRIO_INVALID ����������ȼ���Ч
 *           LJ_TASK_DEL_ERR ����Ҫɾ�������񲻴���
 *           LJ_TASK_DEL_ISR �����������ISR��ɾ������
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
 	
 	if ( gLJIntNesting > 0 )									/* ȷ���������ն˷����ӳ���ISR����ͼɾ��һ������        */
 	{
 		return (LJ_TASK_DEL_ISR);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( prio == LJ_IDLE_PRIO )									/* ȷ����ɾ���������ǿ�������                         */
 	{
 		return (LJ_TASK_DEL_IDLE);
 	}
 	if ( prio >= LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* ȷ���������ȼ�����Ч��                               */
 	{
 		return (LJ_PRIO_INVALID);
 	}
 #endif
 
 	LJ_ENTER_CRITICAL( );
 	if ( prio == LJ_PRIO_SELF )									/* ���øú������������ͨ���������LJ_PRIO_SELFɾ���Լ� */
 	{
 		prio = gLJTcbCur->ljTcbPrio;
 	}
 	ptcb = gLJTcbPrioTbl[prio];
 	if ( ptcb != (LJ_TCB *)0 )									/* ȷ����ɾ���������Ǵ��ڵ�                             */
 	{
 		if ( (gLJRdyTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0x00 )	/* �����ɾ���������ھ������оʹӾ��������Ƴ� */
 		{
 			gLJRdyGrp &= ~ptcb->ljTcbBitY;
 		}
#if LJ_EVENT_EN > 0
		pevent = ptcb->ljTcbEventPtr;
		if ( pevent != (LJ_EVENT *)0 )							/* ����������ź������������ź��������䡢��Ϣ���еĵ� */
		{														/* �����У��������Լ������ı��б�ȥ�� */
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
		if ( gLJLockNesting > 0 )			/* ��LJTaskDel()���¹��жϺ���ͨ��������Ƕ�׼�������1�����������������   */
		{
			gLJLockNesting--;
		}
		gLJTaskCtr--;											/* �������������1��˵��LJ_RTOS�������������һ��       */
		gLJTcbPrioTbl[prio] = (LJ_TCB *)0;						/* ��ָ��ɾ�������������ƿ�LJ_TCB��ָ����ΪNULL     */
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
		ptcb->ljTcbNext	= gLJTcbFreeList;						/* ����ɾ�������������ƿ�LJ_TCB�˻ص�����LJ_TCB����  */
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
 * �������ƣ�LJTaskDelReq
 * ��������������ɾ������, ��ֹ����ɾ����Щռ����Դ���������������ɾ������Щ��ռ�õ���Դ����Ϊû�б��ͷŶ���ʧ
 *
 * ��    �Σ�prio (INT8U)     ������ɾ����������ȼ�
 *
 * �� �� ֵ��LJ_NO_ERR        ������������óɹ�
 *           LJ_TASK_NOT_EXIST�������ɾ���������Ѿ���ɾ���������������ֱ���Ƿ���������ѱ�ִ��
 *           LJ_TASK_DEL_IDLE ���������ɾ����������LJ_RTOS�еĿ�������
 *           LJ_PRIO_INVALID  ���������ɾ����������ȼ���Ч(prio >= LJ_LOWEST_PRIO)
 *           LJ_TASK_DEL_REQ  ��һ�������Ƿ�ɾ�����ڲ���־
 * ʹ    �ã�
 *         ����ɾ�����������������룺
 *         void RequestorTask( void *pdata )
 *         {
 *         		pdata = pdata;
 *              while ( 1 )
 *              {
 *              	Ӧ�ó������
 *                  if ( ɾ����������� )
 *                  {
 *                  	while ( LJTaskDelReq( TASK_TO_DEL_PRIO ) != LJ_TASK_NOT_EXIST )
 *                      {
 *							LJTimeDly( 1 );
 *                      }
 *                  }
 *                  Ӧ�ó������
 *              }
 *         }
 *         ��Ҫɾ���Լ���������룺
 *         void TaskToBeDeleted( void *pdata )
 *         {
 *				pdata = pdata;
 *              while ( 1 )
 *              {
 *					Ӧ�ó������
 *                  if ( LJTaskDelReq( LJ_PRIO_SELF ) == LJ_TASK_DEL_REQ )
 *                  {
 *						�ͷ���ռ�õ���Դ
 *                      �ͷ����ж�̬�ڴ�
 *                      LJTaskDel( LJ_PRIO_SELF );
 *                  }
 *                  else
 *                  {
 *						Ӧ�ó������
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
 		return (LJ_TASK_DEL_IDLE);								/* �����Ҫ��ɾ���������ǿ��������򱨴���           */
 	}
 	if ( prio >= LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* ȷ������������ɾ������������ȼ�����Ч��             */
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
 * �������ƣ�LJTaskSuspend
 * ������������������
 * 
 * ��    �Σ�prio (INT8U)        ����Ҫ������������ȼ�
 *
 * �� �� ֵ��LJ_NO_ERR           ������������������óɹ�
 *           LJ_TASK_SUSPEND_IDLE�����Ҫ����������ǿ��������򲻱�����
 *           LJ_PRIO_INVALID     �����Ҫ�������������ȼ���Ч(prio >= LJ_LOWESR_PRIO)
 *           LJ_TASK_SUSPEND_PRIO�����Ҫ��������񲻴���
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
 	if ( prio == LJ_IDLE_PRIO )									/* ȷ��Ҫ����������ǿ�������                       */
 	{
 		return (LJ_TASK_SUSPEND_IDLE);
 	}
 	if ( prio >= LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* ȷ��Ҫ�������������ȼ���Ч                       */
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
 	ptcb = gLJTcbPrioTbl[prio];									/* ����Ҫ����������Ƿ����                          */
 	if ( ptcb == (LJ_TCB *)0 )
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_TASK_SUSPEND_PRIO);
 	}
 	if ( (gLJRdyTbl[ptcb->ljTcbY] &= ~ptcb->ljTcbBitX) == 0x00 )	/* ��Ҫ����������Ƴ�������                      */
 	{
 		gLJRdyGrp	&= ~ptcb->ljTcbBitY;
 	}
 	ptcb->ljTcbStat |= LJ_STAT_SUSPEND;
 	LJ_EXIT_CRITICAL( );
 	if ( self == LJ_TRUE )										/* ��Ҫ����������ǵ��ñ�����������ʱ�������������  */
 	{
 		LJ_Sched( );
 	}
 	
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *===================================================================================================================
 * �������ƣ�LJTaskResume
 * �����������ָ�����
 *
 * ��    �Σ�prio (INT8U)         ����Ҫ�ָ�����������ȼ�
 *
 * �� �� ֵ��LJ_NO_ERR            ������ָ����������óɹ�
 *           LJ_PRIO_INVALID      �����Ҫ�ָ�����������ȼ���Ч(prio >= LJ_LOWEST_PRIO)
 *           LJ_TASK_RESUME_PRIO  �����Ҫ�ָ������񲻴���
 *           LJ_TASK_NOT_SUSPENDED�����Ҫ�ָ�������û�б�����
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
 	if ( prio >= LJ_LOWEST_PRIO )								/* ȷ��Ҫ�ָ���������ȼ���Ч                       */
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
 * �������ƣ�LJTaskQuery
 * ��������������������Ϣ
 *
 * ��    �Σ�prio     (INT8U)����Ҫ��õ�������Ϣ���������ȼ�
 *           pdata (LJ_TCB *)����Ҫ��õ������������ƿ�ָ��
 * 
 * �� �� ֵ��LJ_NO_ERR       ������������óɹ�
 *           LJ_PRIO_INVALID ������������ȼ���Ч(prio >= LJ_LOWEST_PRIO)
 *           LJ_PRIO_ERR     �������Ҫ��õ�������Ϣ������û�б�����
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
 	if ( prio > LJ_LOWEST_PRIO && prio != LJ_PRIO_SELF )		/* ȷ���������ȼ���Ч                             */
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