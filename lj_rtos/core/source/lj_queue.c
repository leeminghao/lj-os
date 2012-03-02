/*
 *=======================================================================================================
 *                                               LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_queue.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��LJ_RTOS��Ϣ���еĹ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/23
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=======================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *=======================================================================================================
 * �������ƣ�LJ_QInit
 * ������������ʼ�����ж��п��ƿ�����
 * ��    �Σ�void
 * �� �� ֵ��void
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
 * �������ƣ�LJQCreate
 * ��������������һ����Ϣ����
 *
 * ��    �Σ�start (void **) ��ָ����Ϣ�����ָ�� void *messageStorage[size]
 *           size   (INT16U) ��ָ����Ϣ����Ĵ�С
 *
 * �� �� ֵ��!= (LJ_EVENT *)0��ָ����Ϣ����ECB��ָ��
 *           == (LJ_EVENT *)0�����û�п�ʹ�õ��¼����ƿ�
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������жϷ����ӳ����е��ô˺���          */
 	{
 		return ((LJ_EVENT *)0);
 	}
 	LJ_ENTER_CRITICAL( );
 	pevent = gLJEventFreeList;							/* ��ͼ�ӿ����¼����ƿ�ECB������ȡ��һ��ECB      */
 	if ( gLJEventFreeList != (LJ_EVENT *)0 )
 	{
 		gLJEventFreeList = gLJEventFreeList->ljEventPtr;
 	}
 	LJ_EXIT_CRITICAL( );
 	if ( pevent != (LJ_EVENT *)0 )
 	{
 		LJ_ENTER_CRITICAL( );
 		pq = gLJQFreeList;								/* ��ͼ�ӿ��ж��п��ƿ������еõ�һ�����п��ƿ�  */
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
 * �������ƣ�LJQPend
 * �����������ȴ���Ϣ�����е���Ϣ
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *           timeout    (INT16U)���ȴ���ʱʱ��
 *           err       (INT8U *)��ָ���������ָ�룺
 *                                LJ_NO_ERR         ������������óɹ�
 *                                LJ_TIMEOUT        ������������ó�ʱ
 *                                LJ_ERR_EVENT_TYPE �����'pevent'����ָ��һ����Ϣ����ECB��ָ��
 *                                LJ_ERR_PEVENT_NULL�����'pevent'��NULLָ��
 *                                LJ_ERR_PEND_ISR   ����������жϷ����ӳ����е��øú���
 *
 * �� �� ֵ��!= (void *)0      ��ָ����յ�����Ϣ
 *           == (void *)0      ��a)���û�н��յ���Ϣ
 *                               b)���'pevent'��һ��NULLָ��
 *                               c)���'pevent'����ָ����Ϣ����ECB��ָ��
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
 	
 	if ( gLJIntNesting > 0 )								/* ȷ���������жϷ����ӳ����е��øú���          */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return ((void *)0);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )							/* ȷ��'pevent'���ǿ�ָ��                        */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return ((void *)0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )			/* ȷ��'pevent'ָ�������Ϣ����ECB��ָ��         */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return ((void *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pq = (LJ_Q *)pevent->ljEventPtr;
 	if ( pq->ljQEntries > 0 )								/* �ж���Ϣ�������Ƿ񻹴�����Ϣ                  */
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
 	LJ_EventTaskWait( pevent );								/* û����Ϣ��ȴ�                                */
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );
 	LJ_ENTER_CRITICAL( );
 	msg = gLJTcbCur->ljTcbMsg;
 	if ( msg != (void *)0 )									/* �ȵ�����Ϣ                                    */
 	{
 		gLJTcbCur->ljTcbMsg			= (void *)0;
 		gLJTcbCur->ljTcbStat		= LJ_STAT_RDY;
 		gLJTcbCur->ljTcbEventPtr	= (LJ_EVENT *)0;
 		LJ_EXIT_CRITICAL( );
 		*err						= LJ_NO_ERR;
 		return (msg);
 	}
 	LJ_EventTo( pevent );									/* ��Ϊ�ȴ���ʱ�����������ھ���̬                */
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_TIMEOUT;
 	
 	return ((void *)0);
 }
 
 #endif
 
/*
 *============================================================================================================
 * �������ƣ�LJQPost
 * ��������������Ϣ���з���һ����Ϣ
 *
 * ��    �Σ�pevent (LJ_EVENT *) ��ָ����Ϣ����ECB
 *           msg        (void *) ��ָ��Ҫ������Ϣ��ָ��
 *           opt         (INT8U) ��������Ϣѡ��
 *                                 LJ_POST_OPT_NONE     ��������Ϣ����������(FIFO)
 *                                 LJ_POST_OPT_BROADCAST��������Ϣ�����еȴ���Ϣ��������Ϣ������
 *                                 LJ_POST_OPT_FRONT    ��������Ϣ����������(LIFO)
 *
 * �� �� ֵ��LJ_NO_ERR           ������������óɹ�
 *           LJ_Q_FULL           �������Ϣ��������
 *           LJ_ERR_EVENT_TYPE   �����'pevent'����һ��ָ����Ϣ����ECB��ָ��
 *           LJ_ERR_PEVENT_NULL  �����'pevent'��һ��NULLָ��
 *           LJ_ERR_POST_NULL_PTR��������͵���Ϣ�ǿ���Ϣ
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
 	if ( pevent == (LJ_EVENT *)0 )							/* ȷ��'pevent'����NULLָ��                       */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( msg == (void *)0 )									/* ȷ�����͵Ĳ��ǿ���Ϣ                           */
 	{
 		return (LJ_ERR_POST_NULL_PTR);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_Q )			/* ȷ��'pevent'ָ�������Ϣ����ECB��ָ��          */
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventGrp != 0x00 )						/* ����Ƿ��������ڵȴ���Ϣ�����е���Ϣ           */
 	{
 		if ( (opt & LJ_POST_OPT_BROADCAST) != 0x00 )		/* ����ǲ��ǹ㲥��ʽ                             */
 		{
 			while ( pevent->ljEventGrp != 0x00 )
 			{
 				LJ_EventTaskRdy( pevent, msg, LJ_STAT_Q );
 			}
 		}
 		else
 		{
 			LJ_EventTaskRdy( pevent, msg, LJ_STAT_Q );		/* ������ǹ㲥�ķ�ʽ����ֻ�еȴ��б��е�HPT����������̬ */
 		}
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );										/* ���������ǲ���������ȼ�����                 */
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
 * �������ƣ�LJQDel
 * ����������ɾ��һ����Ϣ����
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *           opt         (INT8U)��ɾ��ѡ��
 *                                LJ_DEL_NO_PEND     ��ɾ����Ϣ�������û������ȴ�
 *                                LJ_DEL_ALWAYS      ��ɾ����Ϣ���м���������ȴ�
 *           err       (INT8U *)��ָ���������ָ��
 *                                LJ_NO_ERR          ��������ú����ɹ�
 *                                LJ_ERR_DEL_ISR     ��������жϷ����ӳ����е��øú���
 *                                LJ_ERR_INVALID_OPT �����ɾ��ѡ����Ч
 *                                LJ_ERR_TASK_WAITING������������ڵȴ���Ϣ����
 *                                LJ_ERR_EVENT_TYPE  �����'pevent'����ָ����Ϣ����ECB��ָ��
 *                                LJ_ERR_PEVENT_NULL �����'pevent'��NULLָ��
 *
 * �� �� ֵ��pevent (LJ_EVENT *)��������������
 *                 (LJ_EVENT *)0�����ɾ����Ϣ���гɹ�
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������жϷ����ӳ����е��ô˺���               */
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
 		case LJ_DEL_NO_PEND:							/* ���û������ȴ���Ϣ��������Ϣ��ֱ��ɾ����Ϣ����  */
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
 			
 		case LJ_DEL_ALWAYS:								/* ����ɾ����Ϣ���У������ȴ������б��е��������ھ���̬ */
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
 * �������ƣ�LJQAccept
 * �����������޵ȴ��Ĵ���Ϣ�����л����Ϣ
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *
 * �� �� ֵ��!= (void *)0       �������Ϣ����������Ϣ������
 *           == (void *)0       ��1)�����Ϣ����Ϊ��
 *                                2)���'pevent'ΪNULLָ��
 *                                3)���'pevent'����ָ����Ϣ����ECB��ָ��
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
 * �������ƣ�LJQFlush
 * ���������������Ϣ����
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB���ƿ�ָ��
 *
 * �� �� ֵ��LJ_NO_ERR          ��������øú����ɹ�
 *           LJ_ERR_EVENT_TYPE  �����'pevent'����ָ����Ϣ����ECB��ָ��
 *           LJ_ERR_PEVENT_NULL �����'pevent'ΪNULLָ��
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
 * �������ƣ�LJQQuery
 * ������������ȡ��Ϣ���е�״̬
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *           pdata (LJ_Q_DATA *)��ָ����Ϣ�������ݿ��ƿ��ָ��
 *
 * �� �� ֵ��LJ_NO_ERR          ������ú������óɹ�
 *           LJ_ERR_EVENT_TYPE  �����'pevent'����ָ����Ϣ����ECB��ָ��
 *           LJ_ERR_PEVENT_NULL �����'pevent'��NULLָ��
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