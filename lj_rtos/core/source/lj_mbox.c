/*
 *=========================================================================================================
 *                                                LJ_RTOS
 *                                          The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_mbox.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��LJ_RTOS�е���Ϣ�������
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/23
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *========================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *========================================================================================================
 * �������ƣ�LJMboxCreate
 * ��������������һ����Ϣ����
 *
 * ��    �Σ�msg (void *)    ��ָ��һ��Ͷ�ݵ���Ϣ�����е���Ϣ��ָ�룬���ΪNULL, �򴴽�һ���յ���Ϣ����
 *
 * �� �� ֵ��!= (LJ_EVENT *)0������һ��ָ����Ϣ����ECB��ָ��
 *           == (LJ_EVENT *)0�����û�п��е��¼����ƿ������
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������жϷ����ӳ����е��ô˺���           */
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
 	if ( pevent != (LJ_EVENT *)0 )						/* ȷ���¼����ƿ����                             */
 	{
 		pevent->ljEventType = LJ_EVENT_TYPE_MBOX;
 		pevent->ljEventCnt  = 0;
 		pevent->ljEventPtr  = msg;
 		LJ_EventWaitListInit( pevent );					/* ��ʼ���ȴ������б�                             */
 	}
 	
 	return (pevent);
 }
 
 #endif
 
/*
 *=========================================================================================================
 * �������ƣ�LJMboxPend
 * �����������ȴ���Ϣ�����е���Ϣ
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *           timeout    (INT16U)���ȴ���ʱ��ʱ��(��ʱ�ӽ��ı�ʾ)
 *           err       (INT8U *)��ָ���������ָ�룺
 *                                LJ_NO_ERR         ������������óɹ�
 *                                LJ_TIMEOUT        ������ȴ���ʱ
 *                                LJ_ERR_EVENT_TYPE �����'pevent'����ָ����Ϣ����ECB��ָ��
 *                                LJ_ERR_PEND_ISR   ������ú��������жϷ����ӳ����е��õ�
 *                                LJ_ERR_PEVENT_NULL�����'pevent'��һ��NULLָ��
 *
 * �� �� ֵ��!= (void *)0       �����յ�����Ϣָ��
 *           == (void *)0       ��a)���û�н��յ���Ϣ
 *                                b)���'pevent'��һ��NULLָ��
 *                                c)���'pevent'����ָ����Ϣ����ECB��ָ��
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
 	
 	if ( gLJIntNesting > 0 )						/* ȷ���������жϷ����ӳ����е��øú���                */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return ((void *)0);
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )					/* ȷ��'pevent'����һ��NULLָ��                        */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return ((void *)0);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )/* ȷ��'pevent'��һ��ָ����Ϣ����ECB��ָ��             */
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
 	msg = gLJTcbCur->ljTcbMsg;						/* �ȵ���ָ������Ϣ                                   */
 	if ( msg != (void *)0 )
 	{
 		gLJTcbCur->ljTcbMsg  	 = (void *)0;
 		gLJTcbCur->ljTcbStat	 = LJ_STAT_RDY;
 		gLJTcbCur->ljTcbEventPtr = (LJ_EVENT *)0;
 		LJ_EXIT_CRITICAL( );
 		*err                     = LJ_NO_ERR;
 		return (msg);
 	}
 	LJ_EventTo( pevent );							/* �ȴ���ʱ                                           */
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_TIMEOUT;
 	
 	return ((void *)0);
 }
 
 #endif
 
/*
 *=========================================================================================================
 * �������ƣ�LJMboxPost
 * ��������������Ϣ�����з���һ����Ϣ
 *
 * ��    �Σ�pevent (LJ_EVENT *) ��ָ����Ϣ����ECB��ָ��
 *           msg        (void *) ��ָ����Ϣ��ָ��
 *           opt         (INT8U) ����Ϣѡ��
 *                                 LJ_POST_OPT_NONE     ��������Ϣ����������(������Ϊ�ȴ��б��е�HPT����)
 *                                 LJ_POST_OPT_BROADCAST��������Ϣ���ȴ��б�����������
 *
 * �� �� ֵ��LJ_NO_ERR           ������������óɹ�
 *           LJ_MBOX_FULL        �������Ϣ�����Ѿ�����Ϣ��
 *           LJ_ERR_EVENT_TYPE   �����'pevent'����ָ����Ϣ����ECB��ָ��
 *           LJ_ERR_PEVENT_NULL  �����'pevent'��һ��NULLָ��
 *           LJ_ERR_POST_NULL_PTR������㷢�͵���һ������Ϣ
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
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��'pevent'����NULLָ��                        */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( msg == (void *)0 )								/* ȷ�����Ƿ��Ϳ���Ϣ                              */
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
 		LJ_Sched( );									/* ���������Ƿ���ϵͳ�����ȼ���ߵľ���̬����   */
 		return (LJ_NO_ERR);
 	}
 	if ( pevent->ljEventPtr != (void *)0 )				/* ���û������ȴ�����ô���͵���ϢӦ����������   */
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
 * �������ƣ�LJMboxDel
 * ����������ɾ��һ������
 * 
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *           opt         (INT8U)��ɾ��ѡ��
 *           err       (INT8U *)��ָ���������ָ�룺
 *                                LJ_NO_ERR          ������������óɹ�
 *                                LJ_ERR_DEL_ISR     ����������жϷ����ӳ����е��øú���
 *                                LJ_ERR_INVALID_OPT �����ɾ��ѡ��Ƿ�
 *                                LJ_ERR_TASK_WAITING������������ڵȴ���Ϣ�����е���Ϣ
 *                                LJ_ERR_EVENT_TYPE  �����'pevent'����ָ����Ϣ����ECB��ָ��
 *                                LJ_ERR_PEVENT_NULL �����'pevent'��NULLָ��
 *
 * �� �� ֵ��pevent (LJ_EVENT *)�����������������
 *                 (LJ_EVENT *)0������ɹ�ɾ����Ϣ����
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������жϷ����ӳ����е��øú���           */
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
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_MBOX )	/* ȷ��'pevent'��ָ����Ϣ����ECB��ָ��            */
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
 		case LJ_DEL_NO_PEND:							/* û�еȴ������������ʱ��ɾ������Ϣ����         */
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
 			
 		case LJ_DEL_ALWAYS:								/* �����еȴ�����Ϣ��������񶼽������̬         */
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
 * �������ƣ�LJMboxAccept
 * �����������޵ȴ��ش������еõ�һ����Ϣ
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *
 * �� �� ֵ��!= (void *)0       �������Ϣ����������Ϣ���ã������������������Ϣ
 *           == (void *)0       ��a)�����Ϣ������û����Ϣ����
 *                                b)���'pevent'��NULLָ��
 *                                c)���'pevent'����ָ����Ϣ����ECB��ָ��
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
 * �������ƣ�LJMboxQuery
 * ������������ѯһ�������״̬
 * 
 * ��    �Σ�pevent     (LJ_EVENT *)��ָ����Ϣ����ECB��ָ��
 *           pdata  (LJ_MBOX_DATA *)��ָ����Ϣ�������ݵ�ָ��
 *
 * �� �� ֵ��LJ_NO_ERR              ���ú������óɹ�
 *           LJ_ERR_EVENT_TYPE      �����'pevent'����ָ����Ϣ����ECB��ָ��
 *           LJ_ERR_PEVENT_NULL     �����'pevent'��һ��NULLָ��
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