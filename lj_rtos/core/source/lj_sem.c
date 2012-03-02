/*
 *===========================================================================================================
 *                                                  LJ_RTOS
 *                                            The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_sem.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ���ź���������
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/22
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *==========================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *==========================================================================================================
 * �������ƣ�LJSemCreate
 * ��������������һ���ź���
 *
 * ��    �Σ�cnt (INT16U)���ź�����ʼֵ
 *           1.����ź�����ʾһ�����߶���¼������Ļ�����ô���ź����ĳ�ʼֵͨ����Ϊ0
 *           2.����ź������ڶԹ�����Դ�ķ��ʣ���ô���ź����ĳ�ʼֵӦ��Ϊ1(��ֵ�ź���)
 *           3.����ź���������ʾ�����������n����ͬ����Դ����ô���ź����ĳ�ʼֵӦ��Ϊn,���ź�����Ϊһ��
 *             �ɼ������ź���ʹ��
 *
 * �� �� ֵ��!= (void *)0��һ��ָ���¼����ƿ��ָ��
 *           == (void *)0�����û���¼����ƿ����
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
 	
 	if ( gLJIntNesting > 0 )						/* ȷ��LJSemCreate()�������Ǳ��жϷ����ӳ���������      */
 	{
 		return ((LJ_EVENT *)0);
 	}
 	LJ_ENTER_CRITICAL( );
 	pevent = gLJEventFreeList;						/* ��ͼ�ӿ����¼����ƿ������л��һ���¼����ƿ�         */
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
 * �������ƣ�LJSemPend
 * �����������ȴ�һ���ź���
 *
 * ��    �Σ�pevent (LJ_EVENT *)��һ��ָ���¼����ƿ��ָ�룬�����ʾ�ź���
 *           timeout    (INT16U)���ȴ���ʱʱ��(ʱ�ӽ���)����� != 0, ������ȴ�ָ�����¼������ == 0, ��
 *                                ����һֱ�ȴ���ȥ
 *           err       (INT8U *)��ָ�������Ϣ��ָ�룬���ܵĴ�����ϢΪ��
 *                                LJ_NO_ERR         ������������óɹ�
 *                                LJ_TIMEOUT        ���������ȴ���ʱ
 *                                LJ_ERR_EVENT_TYPE �����û�ô���һ��ָ���ź�����ָ��
 *                                LJ_ERR_PEND_ISR   �������ISR�е��ô˺���
 *                                LJ_ERR_PEVENT_NULL�����'pevent'��һ��NULLָ��
 *
 * �� �� ֵ��void
 *========================================================================================================
 */
 #if LJ_SEM_EN > 0
 
 void
 LJSemPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 	if ( gLJIntNesting > 0 )							/* ȷ�������жϷ����ӳ�����øú���               */
 	{
 		*err = LJ_ERR_PEND_ISR;
 		return;
 	}
 #if LJ_ARG_CHK_EN > 0
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ��pevent��Ϊ��                               */
 	{
 		*err = LJ_ERR_PEVENT_NULL;
 		return;
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )		/* ȷ��peventָ����LJSemCreate()������ECB��       */
 	{
 		*err = LJ_ERR_EVENT_TYPE;
 		return;
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventCnt > 0 )						/* ����ź�������������Ч��                        */
 	{
 		pevent->ljEventCnt--;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		return;
 	}
 	gLJTcbCur->ljTcbStat |= LJ_STAT_SEM;				/* ���ź�����Ч�������������ȴ��ź���״̬        */
 	gLJTcbCur->ljTcbDly   = timeout;
 	LJ_EventTaskWait( pevent );
 	LJ_EXIT_CRITICAL( );
 	LJ_Sched( );						/* ���ڵò����ź���������������Ⱥ�������һ�����ȼ����ߵ���������  */
 	LJ_ENTER_CRITICAL( );
 	if ( gLJTcbCur->ljTcbStat & LJ_STAT_SEM )			/* �ڵ���LJ_Sched()����֮��������״̬            */
 	{
 		LJ_EventTo( pevent );			/* �ȴ���ʱ������LJTimeTick()������Ϊ����̬�Ĳ�������ΪLJSemPost */
 		LJ_EXIT_CRITICAL( );			/* �������ź������ѣ��ʵ���LJ_EventTo()����ȴ���ʱ������ӵȴ�����*/
 		*err = LJ_TIMEOUT;				/* �б���ɾ����������һ��"��ʱ����"����                            */
 	}
 	gLJTcbCur->ljTcbEventPtr	= (LJ_EVENT *)0;		/* LJSemPost()�Ѿ������ź�������ָ���ź���ECB��ָ��*/
 	LJ_EXIT_CRITICAL( );								/* �Ӹ������������ƿ���ɾ��                      */
 	*err = LJ_NO_ERR;
 }
 
 #endif
 
/*
 *==========================================================================================================
 * �������ƣ�LJSemPost
 * ��������������һ���ź���
 *
 * ��    �Σ�pevent (LJ_EVENT *)��ָ����LJSemCreate()������ECB���ƿ�ָ��
 *
 * �� �� ֵ��LJ_NO_ERR          ������������óɹ�
 *           LJ_SEM_OVF         ������ź���ֵ������16λ�޷��������ķ�Χ
 *           LJ_ERR_EVENT_TYPE  ��������ݵĲ���һ��ָ���ź���ECB��ָ��
 *           LJ_ERR_PEVENT_NULL ��������ݵ�'pevent'��һ��NULLָ��
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
 	if ( pevent == (LJ_EVENT *)0 )						/* ȷ���¼����ƿ�ָ�벻Ϊ��                         */
 	{
 		return (LJ_ERR_PEVENT_NULL);
 	}
 	if ( pevent->ljEventType != LJ_EVENT_TYPE_SEM )
 	{
 		return (LJ_ERR_EVENT_TYPE);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pevent->ljEventGrp != 0x00 )					/* ����Ƿ��������ڵȴ����ź���                     */
 	{
 		LJ_EventTaskRdy( pevent, (void *)0, LJ_STAT_SEM );	/* �����ȼ���ߵ�����ӵȴ��б���ȥ��           */
 		LJ_EXIT_CRITICAL( );
 		LJ_Sched( );									/* ���ӵȴ��б���ɾ���������Ƿ���ϵͳ�е�HPT����	*/ 
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
 * �������ƣ�LJSemDel
 * ����������ɾ��һ���ź���
 * 
 * ��    �Σ�pevent (LJ_EVENT *)��ָ���¼����ƿ��ָ��
 *           opt         (INT8U)��ɾ���ź���ѡ��
 *                                LJ_DEL_NO_PEND     ��û������ȴ��ź���
 *                                LJ_DEL_ALWAYS      �����еȴ����ź��������񶼽������̬
 *           err       (INT8U *)��ָ��������ָ��
 *                                LJ_NO_ERR          ������ú������óɹ�
 *                                LJ_ERR_DEL_ISR     �����������ISR��ɾ���ź���
 *                                LJ_ERR_INVALID_OPT ���������ָ����ɾ��ѡ��
 *                                LJ_ERR_TASK_WAITING�����������ȴ��ź���,��ָ����LJ_DEL_NO_PENDѡ��
 *                                LJ_ERR_EVENT_TYPE  ��������ݵ��¼�ָ�벻��ָ���ź���ECB
 *                                LJ_ERR_PEVENT_NULL �����'pevent'��NULLָ��
 *
 * �� �� ֵ��pevent (LJ_EVENT *)�����������������
 *           (LJ_EVENT *)0      ������ź������ɹ�ɾ��
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
 	
 	if ( gLJIntNesting > 0 )							/* ȷ���������ն˷����ӳ�����ɾ���ź���               */
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
 		case LJ_DEL_NO_PEND:							/* ��ʾ��û�������ڵȴ����ź���                      */
 			if ( tasksWaiting == LJ_FALSE )
 			{
 				pevent->ljEventType = LJ_EVENT_TYPE_UNUSED;
 				pevent->ljEventPtr  = gLJEventFreeList; /* ���ź���ECB�Żص������¼�������                   */
 				gLJEventFreeList    = pevent;
 				LJ_EXIT_CRITICAL( );
 				
 				return ((LJ_EVENT *)0);
 			}
 			else										/* ���ָ����LJ_DEL_NO_PENDѡ����������ڵȴ��򷵻� */
 			{
 				LJ_EXIT_CRITICAL( );
 				*err = LJ_ERR_TASK_WAITING;
 				
 				return (pevent);
 			}
 			
 		case LJ_DEL_ALWAYS:
 			while ( pevent->ljEventGrp != 0x00 )		/* �����еȴ��ź��������񶼴��ھ���̬                */
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
 * �������ƣ�LJSemAccept
 * �����������޵ȴ�������һ���ź�
 *
 * ��    �Σ�pevent	(LJ_EVENT *)��ָ���¼����ƿ��ָ��
 *
 * �� �� ֵ��>  0               ��������ź�����Դ�����ã����ź���ֵ�򵥵ļ�1�󷵻�
 *           == 0               �����û���ź��������û��߲�������򵥵ķ��أ������ȴ�
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
 * �������ƣ�LJSemQuery
 * ������������ѯһ���ź����ĵ�ǰ״̬
 *
 * ��    �Σ�pevent    (LJ_EVENT *)��ָ���¼����ƿ��ָ��
 *           pdata	(LJ_SEM_DATA *)��ָ�����ڼ�¼�ź�����Ϣ��ָ��
 *
 * �� �� ֵ��LJ_NO_ERR			   ������ú������óɹ�
 *           LJ_ERR_EVENT_TYPE     �����'pevent'δָ��һ���ź������͵��¼�ָ��
 *           LJ_ERR_PEVENT_NULL    �����'pevent'��һ��NULLָ��
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