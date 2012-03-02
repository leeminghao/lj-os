/*
 *=======================================================================================================
 *                                                 LJ_RTOS
 *                                           The Real-Time Kernel
 * 
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_mutex.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��LJ_RTOS�������ź�������
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/22
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=======================================================================================================
 */
 
 #include <lj_rtos.h>
 
 #define LJ_MUTEX_KEEP_LOWER_8	0x00ff
 #define LJ_MUTEX_KEEP_UPPER_8	0xff00
 
 #define LJ_MUTEX_AVAILABLE		0x00ff
 
/*
 *=======================================================================================================
 * �������ƣ�LJMutexCreate
 * ��������������һ���������ź���
 * 
 * ��    �Σ�prio  (INT8U)�����ȼ��̳����ȼ�����һ�������ȼ�������ռ�����������ʱ����һ���������ȼ�������
 *                        �����Ի������ź�������ʱ�Ͱѵ����ȼ���������ȼ����������ֵ
 *           err (INT8U *)��ָ���������ָ��
 *                          LJ_NO_ERR         ���������������óɹ�
 *                          LJ_ERR_CREATE_ISR ������������жϷ����ӳ����д���һ��������
 *                          LJ_PRIO_EXIST     �����ָ����PIP�Ѿ���һ��������ռ����
 *                          LJ_ERR_PEVENT_NULL�����û�ж�����¼����ƿ������
 *                          LJ_PRIO_INVALID   �����ָ�������ȼ��̳����ȼ���Ч(prio >= LJ_LOWEST_PRIO)
 *
 * �� �� ֵ��!= (void *)0 ��һ��ָ���¼����ƿ��ָ��
 *           == (void *)0 ������д���
 *
 * ע    �⣺.ljEventCnt�ĸ�8λ���ڱ���PIP��ֵ������8λ����Դ������ռ��ʱ��ֵΪ0xff,��������ռ��ʱΪ
 *           ռ��mutex���������ȼ�
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ��������ISR�д����������ź���               */
 	{
 		*err = LJ_ERR_CREATE_ISR;
 		return ((LJ_EVENT *)0);
 	}
 #if LJ_ARG_CHK_EN
 	if ( prio >= LJ_LOWEST_PRIO )						/* ȷ�������PIP��Ч                             */
 	{
 		*err = LJ_PRIO_INVALID;
 		return ((LJ_EVENT *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbPrioTbl[prio] != (LJ_TCB *)0 )			/* ȷ��PIPû�б�������ռ��                       */
 	{
 		*err = LJ_PRIO_EXIST;
 		LJ_EXIT_CRITICAL( );
 		return ((LJ_EVENT *)0);
 	}
 	gLJTcbPrioTbl[prio] = (LJ_TCB *)1;					/* ����������ȼ�����ֹʹ��������ȼ������������*/
 	pevent				= gLJEventFreeList;				/* ��ͼ�ӿ����¼����ƿ�õ�һ���¼����ƿ�ECB     */
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
 	pevent->ljEventCnt	= (prio << 8) | LJ_MUTEX_AVAILABLE;	/* ��MUTEX��Ч��ͬʱ��PIP��������            */
 	pevent->ljEventPtr	= (void *)0;
 	LJ_EventWaitListInit( pevent );						/* ��ʼ���ȴ��¼��������б�                      */
 	*err = LJ_NO_ERR;
 	
 	return (pevent);
 }
 
 #endif
 
/*
 *========================================================================================================
 * �������ƣ�LJMutexPend
 * �����������ȴ�һ���������ź���
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ��һ���������ź���ECB��ָ��
 *           timeout    (INT16U)���ȴ���ʱ��ʱ��(ʱ�ӽ�����)
 *           err       (INT8U *)��ָ��������ָ�룺
 *                                LJ_NO_ERR         ������������óɹ�
 *                                LJ_TIMEOUT        �����������ָ����ʱ���ڻ�û�еȵ��������ź���
 *                                LJ_ERR_EVENT_TYPE ��������ݵĲ���һ���������ź������͵�ָ��
 *                                LJ_ERR_PEVENT_NULL��'pevent'��һ����ָ��
 *                                LJ_ERR_PEND_ISR   ����������жϷ����ӳ����е��ô˺���
 *
 * �� �� ֵ��void
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ�������жϷ����ӳ�����øú���               */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return;
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��'pevent'ָ�벻�ǿ�ָ��'NULL'                */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return;
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )	/* ȷ��'pevent'ָ����ǻ�����ECBָ��               */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return;
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	/* 
 	 * ���.ljEventCnt�ĵ�8λ����0xff(����LJ_MUTEX_AVAILABLE), ���mutex��Ч��LJMutexPend()��mutex�������
 	 * �ú�������������.ljEventCnt�ĵ�8λ���ڸ���������ȼ�������.ljEventPtrָ����øú���������������
 	 * �ƿ�TCB, Ȼ�󷵻�.
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
 	 * ���mutex�����������ռ�ã���ô��������Ӧ��������̬��ֱ��ռ��mutex�������ͷ���mutex, ���ߵȴ���ʱ
 	 * ���timeout != 0, ��LJMutexPend()���������ֱ��timeout�ĳ�ʱʱ�䵽
 	 * ���timeout == 0, �����������ڵصȴ�mutex
 	 */
 	pip		= (INT8U)(pevent->ljEventCnt >> 8);			/* ��ȡmutex�е�PIP                               */
 	mprio	= (INT8U)(pevent->ljEventCnt & LJ_MUTEX_KEEP_LOWER_8);	/* ��ȡռ��mutex����������ȼ�        */
 	ptcb	= (LJ_TCB *)(pevent->ljEventPtr);			/* ��ȡָ��ռ��mutex�����������ƿ�ָ��          */
 	
 	/*
 	 * ���ռ��mutex��������ȼ��ȵ���LJMutexPend()��������������ȼ��ͣ�ռ��mutex����������ȼ��ͱ�������
 	 * mutex�����ȼ��̳����ȼ�pip
 	 */
 	if ( ptcb->ljTcbPrio != pip && mprio > gLJTcbCur->ljTcbPrio )
 	{
 		if ( (gLJRdyTbl[ptcb->ljTcbY] & ptcb->ljTcbBitX) != 0x00 )	/* ȷ��ռ��mutex�������Ƿ�������̬  */
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
 	LJ_EventTaskWait( pevent );								/* ʹ��ǰ����LJMutexPend()�������������̬    */
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbCur->ljTcbStat & LJ_STAT_MUTEX )
 	{
 		LJ_EventTo( pevent );								/* ���ڵȴ���ʱ����������                     */
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
 * �������ƣ�LJMutexPost
 * �����������ͷ�һ���������ź���
 *
 * ��    �Σ�pevent (LJ_EVENT *)    ��ָ�򻥳����ź���ECB��ָ��
 *
 * �� �� ֵ��LJ_NO_ERR              ������������óɹ�
 *           LJ_ERR_EVENT_TYPE      ��������ݵĲ���һ��ָ�򻥳����ź���ECB��ָ��
 *           LJ_ERR_PEVENT_NULL     ��������ݵ�'pevent'��NULLָ��
 *           LJ_ERR_POST_ISR        ������ú��������жϷ����ӳ����е��õ�
 *           LJ_ERR_NOT_MUTEX_OWNER ��������øú�����������ռ��ָ���������ź���������
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ�������жϷ����ӳ����е��øú���             */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��'pevent'����NULLָ��                       */
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
 	if ( gLJTcbCur->ljTcbPrio != pip && 				/* ȷ���ͷ�mutex������ȷʵ��ռ��mutex������       */
 	     gLJTcbCur->ljTcbPrio != prio )	
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_ERR_NOT_MUTEX_OWNER);
 	}
 #endif
 	if ( gLJTcbCur->ljTcbPrio == pip )					/* �鿴ռ��mutex����������ȼ��Ƿ��Ѿ�������PIP   */
 	{
 		/* 
 		 * �����ñ�����������������������PIPλ����ɾȥ���Żص����������ԭ�������ȼ�λ����
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
 	if ( pevent->ljEventGrp != 0x00 )					/* �鿴�Ƿ������ڵȴ�mutex������                  */
 	{
 		prio				= LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_MUTEX );
 		pevent->ljEventCnt &= LJ_MUTEX_KEEP_UPPER_8;
 		pevent->ljEventCnt |= prio;						/* �µ�ռ��mutex��������ȼ���������mutex��ECB��  */
 		pevent->ljEventPtr  = gLJTcbPrioTbl[prio];
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );									/* ����LJ_Sched()�鿴����������̬�������Ƿ��ǵ� */
 		return (LJ_NO_ERR);								/* ǰ����̬���������ȼ���ߵ�                     */
 	}
 	pevent->ljEventCnt	|= LJ_MUTEX_KEEP_LOWER_8;		/* ���û�еȴ�mutex��������.ljEventCnt�ĵ�8λ  */
 														/* ��Ϊ0xff, ����mutex��Ч����������              */
 	pevent->ljEventPtr	 = (void *)0;
 	LJ_EXIT_CRITICAL( );
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *=========================================================================================================
 * �������ƣ�LJMutexDel
 * ����������ɾ��һ���������ź���
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ��һ���������ź���ECB��ָ��
 *           opt         (INT8U)��ɾ��ѡ��
 *           err       (INT8U *)��ָ���������ָ��
 *                                LJ_NO_ERR          ������ú������óɹ�
 *                                LJ_ERR_DEL_ISR     ����������жϷ����ӳ����е��øú���
 *                                LJ_ERR_INVALID_OPT �����ָ����ɾ��ѡ��Ϸ�
 *                                LJ_ERR_TASK_WAITING�����������ȴ��������ź���
 *                                LJ_ERR_EVENT_TYPE  ����������ָ�벻��ָ�򻥳����ź���ECB��ָ��
 *                                LJ_ERR_PEVENT_NULL ����������ָ����NULLָ��
 *
 * �� �� ֵ��pevent            ��������������Ĵ���
 *           (LJ_EVENT *)0     ���ɹ�ɾ���������ź���
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������ն˷����ӳ����е��øú���           */
 	{
 		*err = LJ_ERR_DEL_ISR;
 		return (pevent);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��'pevent'����NULLָ��                       */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return (pevent);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )	/* ȷ��'pevent'��ָ�򻥳����ź���ECB��ָ��        */
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
 		case LJ_DEL_NO_PEND:							/* û������ȴ����mutexʱ����ECB����Ϊδʹ�ã��� */
 			if ( tasksWaiting == LJ_FALSE )				/* �ͻص������¼����ƿ��б���                     */
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
 			if ( tasksWaiting == LJ_TRUE )				/* ֻ�е��������ڵȴ����mutexʱ���ŵ��õ��Ⱥ��� */
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
 * �������ƣ�LJMutexAccept
 * �����������޵ȴ��Ļ�ȡ�������ź���(���񲻹���)
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ�򻥳����ź���ECB��ָ��
 *           err       (INT8U *)��ָ���������ָ��
 *                                LJ_NO_ERR         ������ú������óɹ�
 *                                LJ_ERR_EVENT_TYPE �����'pevent'����ָ��һ���������ź���ECB��ָ��
 *                                LJ_ERR_PEVENT_NULL�����'pevent'ΪNULLָ��
 *                                LJ_ERR_PEND_ISR   ������ú��������жϷ����ӳ����е���
 *
 * �� �� ֵ��== 1               ������л������ź���������
 *           == 0               ��a) ���û�л������ź���������
 *                                b) ���'pevent'����ָ�򻥳����ź���ECB��ָ��
 *                                c) ����ú��������жϷ����ӳ����б����õ�
 *========================================================================================================
 */
 #if LJ_MUTEX_EN > 0 && LJ_MUTEX_ACCEPT_EN > 0
 
 INT8U
 LJMutexAccept( LJ_EVENT *pevent, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 	if ( gLJIntNesting > 0 )							/* ȷ���������жϷ����ӳ����е��øú���           */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return (0);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��'pevent'����NULLָ��                       */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return (0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MUTEX )	/* ȷ��'pevent'��ָ�򻥳����ź���ECB��ָ��        */
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
 * �������ƣ�LJMutexQuery
 * ������������ȡ�������ź����ĵ�ǰ״̬
 *
 * ��    �Σ�pevent     (LJ_EVENT *)��ָ�򻥳����ź���ECB��ָ��
 *           pdata (LJ_MUTEX_DATA *)���������ź�������LJ_MUTEX_DATA��ָ��
 *
 * �� �� ֵ��LJ_NO_ERR              ��������øú����ɹ�
 *           LJ_ERR_QUERY_ISR       ����������жϷ����ӳ����е��øú�����
 *           LJ_ERR_PEVENT_NULL     �����'pevent'��NULLָ��
 *           LJ_ERR_EVENT_TYPE      �����'pevent'����ָ�򻥳����ź���ECB��ָ��
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������жϷ����ӳ����е��øú���           */
 	{
 		return (LJ_ERR_QUERY_ISR);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��'pevent'����NULLָ��                       */
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