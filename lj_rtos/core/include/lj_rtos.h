/*
 *==============================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_rtos.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ���������LJ_RTOSʹ�õĳ����ͺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *==============================================================================================================
 */
 
 #ifndef __LJ_RTOS_H
 #define __LJ_RTOS_H
 
 #ifdef __cplusplus
 extern "C"
 #endif
 
 #include <lj_cfg.h>											/* ���û�Ӧ�ó��������йص�ͷ�ļ�               */
 #include <lj_cpu.h>											/* ��CPU�ͱ������йص�ͷ�ļ�                    */
 #include <lj_err.h>											/* LJ_RTOS��������������                        */
 
 #include <string.h>
 											
 
/*==================================================�汾��======================================================*/

 #define LJ_VERSION					0							/* �ں˰汾�� = 0.0 * 100                       */
 
/*=================================================��������=====================================================*/

 #ifndef LJ_FALSE
 #define LJ_FALSE					0
 #endif
 
 #ifndef LJ_TRUE
 #define LJ_TRUE					1
 #endif
 
 #define LJ_ASCII_NUL				0
 
 #ifndef LJ_GLOBAL
 #define LJ_EXT		extern
 #else
 #define LJ_EXT
 #endif
 
 #define LJ_PRIO_SELF				0xff						/* �����������ȼ�                               */
 
 #define LJ_RDY_TBL_SIZE	(LJ_LOWEST_PRIO / 8 + 1)			/* ��������������������û��������õ�           */
 																/* ��С���ȼ�����������ɽ�ʡ�ռ��ʹ��         */
 #define LJ_EVENT_TBL_SIZE	(LJ_LOWEST_PRIO / 8 + 1)			/* �ȴ��¼����������                           */
 															
 #define LJ_N_SYS_TASKS		1									/* LJ_RTOS�е�ϵͳ�������                      */
 
 #define LJ_IDLE_PRIO		(LJ_LOWEST_PRIO)					/* ������������ȼ�(ϵͳ����)                   */
 
 #define LJ_TASK_IDLE_ID	65535								/* ���������ID                                 */
 
 #define LJ_EVENT_EN ((LJ_SEM_EN > 0) || LJ_MUTEX_EN > 0 || (LJ_MBOX_EN > 0) || ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)))
 
/*=================================================����״̬=====================================================*/

 #define LJ_STAT_RDY			0x00							/* ����̬                                       */
 #define LJ_STAT_SEM			0x01							/* �ȴ�һ���ź���                               */
 #define LJ_STAT_MUTEX			0x02							/* �ȴ�һ�������ź���                           */
 #define LJ_STAT_MBOX			0x04							/* �ȴ�һ����Ϣ����                             */
 #define LJ_STAT_Q				0x08							/* �ȴ���Ϣ�����е�һ����Ϣ                     */
 #define LJ_STAT_SUSPEND		0x10							/* ���񱻹���                                   */
 
/*=================================================�¼�����=====================================================*/
 
 #define LJ_EVENT_TYPE_UNUSED	0								/* δʹ��                                       */
 #define LJ_EVENT_TYPE_SEM		1								/* �ź���                                       */
 #define LJ_EVENT_TYPE_MUTEX	2								/* �������ź���                                 */
 #define LJ_EVENT_TYPE_MBOX		3								/* ��Ϣ����                                     */
 #define LJ_EVENT_TYPE_Q		4								/* ��Ϣ����                                     */
 
/*===============================================ɾ���¼�ѡ��===================================================*/

 #define LJ_DEL_NO_PEND			0								/* û������ȴ��¼�                             */
 #define LJ_DEL_ALWAYS			1								/* ���еȴ����¼������񶼽������̬             */
 
/*
 *===============================================================================================================
 *                                              LJ???Post()��ѡ��
 *
 * ���ж����ѡ������������������LJMboxPost() �� LJQPost()
 *===============================================================================================================
 */
 #define LJ_POST_OPT_NONE		0x00							/* ��ȴ������б��е�HPT���񴫵���Ϣ(FIFO)      */
 #define LJ_POST_OPT_BROADCAST	0x01							/* ��ȴ������б��е���������㲥��Ϣ           */
 #define LJ_POST_OPT_FRONT		0x02							/* ��ȴ������б��е�HPT���񴫵���Ϣ(LIFO)      */
 
/*=================================================����ѡ��=====================================================*/

 #define LJ_TASK_OPT_STK_CHK	0x0001							/* ���������ջ���                             */
 #define LJ_TASK_OPT_STK_CLR	0x0002							/* ��������ջ                                 */
 
/*================================================�¼����ƿ�====================================================*/

 #if (LJ_EVENT_EN > 0) && (LJ_MAX_EVENTS > 0)
 
 typedef struct lj_event
 {
 	INT8U	 ljEventType;										/* �¼�����                                     */
 	INT8U	 ljEventGrp;										/* �ȴ��������ڵ���                             */
 	INT16U	 ljEventCnt;										/* �ź���������                                 */
 	void	*ljEventPtr;										/* ָ����Ϣ������Ϣ���е�ָ��                   */
 	INT8U	 ljEventTbl[LJ_EVENT_TBL_SIZE];						/* �ȴ������б�                                 */
 	
 } LJ_EVENT;
 
 #endif
 
/*================================================�ź�������====================================================*/

 #if LJ_SEM_EN > 0
 
 typedef struct lj_sem_data
 {
 	INT16U	ljCnt;												/* �ź���������                                 */
 	INT8U	ljEventGrp;											/* �ȴ��������ڵ���                             */
 	INT8U	ljEventTbl[LJ_EVENT_TBL_SIZE];						/* �ȴ������б�                                 */
 	
 } LJ_SEM_DATA;
 
 #endif
 
/*=============================================�������ź�������=================================================*/

 #if LJ_MUTEX_EN > 0
 
 typedef struct lj_mutex_data
 {
 	INT8U	ljEventTbl[LJ_EVENT_TBL_SIZE];						/* �ȴ������б�                                 */
 	INT8U	ljEventGrp;											/* �ȴ��������ڵ���                             */
 	INT8U	ljValue;											/* �������ź���ֵ(0 = �ѱ�ʹ��, 1 = ��ʹ�õ�)   */
 	INT8U	ljOwnerPrio;										/* ռ�û��������������ȼ���0xff��ʾû������ռ�� */
 	INT8U	ljMutexPIP;											/* ���ȼ��̳����ȼ����Ը������HPT��������ȼ�  */
 	
 } LJ_MUTEX_DATA;
 
 #endif
 
/*===============================================��Ϣ��������===================================================*/

 #if LJ_MBOX_EN > 0
 
 typedef struct lj_mbox_data
 {
 	void	*ljMsg;												/* ָ����Ϣ��������Ϣ��ָ��                     */
 	INT8U	 ljEventTbl[LJ_EVENT_TBL_SIZE];						/* �ȴ������б�                                 */
 	INT8U	 ljEventGrp;										/* �ȴ��������ڵ���                             */
 	
 } LJ_MBOX_DATA;
 
 #endif
 
/*===============================================��Ϣ��������===================================================*/

 #if LJ_Q_EN > 0
 
 typedef struct lj_q											/* ��Ϣ���п��ƿ�                               */
 {
 	struct	lj_q	 *ljQPtr;									/* �ڿ��ж��п��ƿ�������ָ����һ�����п��ƿ�   */
 	void			**ljQStart;									/* ָ����Ϣ���е�ָ���������ʼ��ַ��ָ��       */
 	void			**ljQEnd;									/* ָ����Ϣ���н�����Ԫ����һ����ַ��ָ��       */
 	void			**ljQIn;									/* ָ����Ϣ�����в�����һ����Ϣ��λ�õ�ָ��     */
 	void			**ljQOut;									/* ָ����Ϣ��������һ��ȡ����Ϣ��λ�õ�ָ��     */
 	INT16U			  ljQSize;									/* ��Ϣ�����п����ɵ��ܵ���Ϣ��                 */
 	INT16U			  ljQEntries;								/* ��Ϣ�����е�ǰ����Ϣ��                       */
 	
 } LJ_Q;
 
 typedef struct lj_q_data
 {
 	void			 *ljMsg;									/* ָ����Ϣ��������һ��ȡ����Ϣ��λ�õ�ָ��     */
 	INT16U			  ljNMsgs;									/* ��Ϣ�����е���Ϣ��Ŀ                         */
 	INT16U			  ljQSize;									/* ��Ϣ���е�������                             */
 	INT8U			  ljEventTbl[LJ_EVENT_TBL_SIZE];			/* �ȴ������б�                                 */
 	INT8U			  ljEventGrp;								/* �ȴ��������ڵ���                             */
 	
 } LJ_Q_DATA;
 
 #endif
 
/*===============================================�����ջ����===================================================*/

 #if LJ_TASK_CREATE_EXT_EN > 0
 
 typedef struct lj_stk_data
 {
 	INT32U		ljFree;											/* ��ջ�Ѿ���ռ�õ��ֽ���                       */
 	INT32U		ljUsed;											/* ��ջ�����ֽ���                               */
 	
 } LJ_STK_DATA;
 
 #endif 
 
 
/*================================================������ƿ�====================================================*/

 typedef struct lj_tcb											/* ������ƿ�                                   */
 {
 	LJ_STK			*ljTcbStkTop;								/* ָ��ǰ�����ջջ����ָ��                   */
 	
 #if LJ_TASK_CREATE_EXT_EN > 0
 	void			*ljTcbExtPtr;								/* ָ���û������������ƿ���չ                 */
 	LJ_STK			*ljTcbStkBottom;							/* ָ�������ջջ�׵�ָ��                       */
 	INT32U			 ljTcbStkSize;								/* ����ջ�п����ɵ�ָ��Ԫ��Ŀ                   */
 	INT16U			 ljTcbOpt;									/* ѡ���LJ_TASK_OPT_STK_CHK(�����ջ����)    */
 																/*         LJ_TASK_OPT_STK_CLR(��������ջ)    */
 	INT16U			 ljTcbId;									/* ���ڴ洢�����ʶ����                         */
 #endif
 	
 	struct lj_tcb	*ljTcbNext;									/* ljTcbNext��ljTcbPrev����������ƿ�           */
 	struct lj_tcb	*ljTcbPrev; 								/* LJ_TCB˫�������ǰ������                     */
 	
 #if LJ_EVENT_EN
 	LJ_EVENT		*ljTcbEventPtr;								/* ָ���¼����ƿ��ָ��                         */
 #endif
 
 #if ((LJ_Q_EN > 0) && (LJ_MAX_QS > 0)) || (LJ_MBOX_EN > 0)
 	void			*ljTcbMsg;									/* ָ�򴫵ݸ��������Ϣ��ָ��                   */
 #endif
 	
 	INT16U			 ljTcbDly;									/* ����������ʱ�ı���                           */
 	INT8U			 ljTcbStat;									/* ��ǰ�����״̬                               */
 	INT8U			 ljTcbPrio;									/* ��ǰ��������ȼ���min = 63, max = 0          */
 	
 	INT8U			 ljTcbX;									/* ����4��ֵ���ڼ�������������̬              */
 	INT8U			 ljTcbY;									/* �����ȴ��¼�����״̬�Ĺ��̵�               */
 	INT8U			 ljTcbBitX;									/* ��Щֵ��������ʱ��õģ�����������         */
 	INT8U			 ljTcbBitY;									/* �м�����Щֵ                                 */
 	
 #if LJ_TASK_DEL_EN > 0
 	BOOLEAN			 ljTcbDelReq;								/* ���ڱ�ʾ�������Ƿ���ɾ���Ĳ�������           */
 #endif
 	
 } LJ_TCB;

/*==============================================�ڴ��������====================================================*/

 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 typedef struct lj_mem											/* �ڴ���ƿ�                                   */
 {
 	void			*ljMemAddr;									/* ָ���ڴ������ʼ��ַ��ָ��                   */
 	void			*ljMemFreeList;								/* ָ����һ�������ڴ���ƿ�                     */
 	INT32U			 ljMemBlkSize;								/* �ڴ�������ڴ��Ĵ�С                       */
 	INT32U			 ljMemNBlks;								/* �ڴ�������ܵ��ڴ������                     */
 	INT32U			 ljMemNFree;								/* �ڴ�����е�ǰ���Ի�õĿ����ڴ�������     */
 	
 } LJ_MEM;
 
 typedef struct lj_mem_data
 {
 	void			*ljAddr;									/* ָ���ڴ�����׵�ַ��ָ��                     */
 	void			*ljFreeList;								/* ָ������ڴ�������׵�ַ��ָ��               */
 	INT32U			 ljBlkSize;									/* ÿ���ڴ���������ֽ���                       */
 	INT32U			 ljNBlks;									/* �ڴ�����ܵ��ڴ����                         */
 	INT32U			 ljNFree;									/* �����ڴ����                                 */
 	INT32U			 ljNUsed;									/* ����ʹ�õ��ڴ����                           */
 	
 } LJ_MEM_DATA;
 
 #endif
 
/*================================================ȫ�ֱ���======================================================*/
 
 LJ_EXT INT32U				 gLJCtxSwCtr;						/* �����л�����������                           */
 LJ_EXT INT8U				 gLJIntNesting;						/* �ж�Ƕ�ײ���                                 */
 LJ_EXT INT8U				 gLJIntExitY;
 LJ_EXT INT8U				 gLJLockNesting;					/* ������������                                 */
 
 LJ_EXT INT8U				 gLJPrioCur;						/* ��ǰ�������ȼ�                               */
 LJ_EXT INT8U				 gLJPrioHighRdy;					/* ������ȼ��������ȼ�                         */
 
 LJ_EXT INT8U				 gLJRdyGrp;							/* ������������Ϊ������                         */
 LJ_EXT INT8U				 gLJRdyTbl[LJ_RDY_TBL_SIZE];
 
 LJ_EXT BOOLEAN				 gLJRunning;						/* ���LJ_RTOS�Ƿ�������                        */
 
 LJ_EXT INT8U				 gLJTaskCtr;						/* �Ѿ�����������ĸ���                         */
 
 LJ_EXT LJ_TCB				*gLJTcbCur;							/* ָ��ǰ�����е������������ƿ�ָ��         */
 LJ_EXT LJ_TCB				*gLJTcbFreeList;					/* ָ�����������ƿ��б��ָ��                 */
 LJ_EXT LJ_TCB				*gLJTcbHighRdy;						/* ָ��������ȼ�������ƿ��ָ��               */
 LJ_EXT LJ_TCB				*gLJTcbList;						/* ָ��������ƿ��б�˫�����ָ��               */
 LJ_EXT LJ_TCB				*gLJTcbPrioTbl[LJ_LOWEST_PRIO + 1];	/* �Ѵ���������ƿ�ָ������                     */
 LJ_EXT LJ_TCB				 gLJTcbTbl[LJ_MAX_TASKS + LJ_N_SYS_TASKS];	/* ������ƿ�����                       */
 
 #if (LJ_EVENT_EN > 0) && (LJ_MAX_EVENTS > 0)
 LJ_EXT LJ_EVENT			*gLJEventFreeList;					/* ָ������¼����ƿ������ָ��                 */
 LJ_EXT LJ_EVENT			 gLJEventTbl[LJ_MAX_EVENTS];		/* �����¼����ƿ�����                           */
 #endif
 
 #if (LJ_Q_EN > 0) && (LJ_MAX_QS > 0)
 LJ_EXT	LJ_Q				*gLJQFreeList;						/* ָ����ж��п��ƿ�����                       */
 LJ_EXT	LJ_Q	 			 gLJQTbl[LJ_MAX_QS];				/* ��Ϣ���п��ƿ�����                           */
 #endif
 
 #if (LJ_MEM_EN > 0) && (LJ_MAX_MEM_PART > 0)
 LJ_EXT	LJ_MEM				*gLJMemFreeList;					/* ָ������ڴ���ƿ�����                       */
 LJ_EXT	LJ_MEM				 gLJMemTbl[LJ_MAX_MEM_PART];		/* �ڴ�������ƿ�����                           */
 #endif
 
 #if LJ_TIME_GET_SET_EN > 0
 LJ_EXT volatile INT32U		 gLJTime;							/* ��ǰϵͳʱ��(��ϵͳʱ�ӽ�������ʾ)           */
 #endif
 
 LJ_EXT LJ_STK				 gLJTaskIdleStk[LJ_TASK_IDLE_STK_SIZE];		/* ���������ջ                         */
 LJ_EXT volatile INT32U		 gLJIdleCtr;						/* �������������                               */
 
 extern INT8U const			 gLJMapTbl[];						/* ���ȼ�->λ����  ���                         */
 extern INT8U const			 gLJUnMapTbl[];						/* ���ȼ�->����    ���                         */
 
 
/*============================================���������ԭ��==================================================*/

 #if LJ_TASK_CREATE_EN > 0
 INT8U   LJTaskCreate( void (*task)( void *pd ), void *pdata, LJ_STK *ptos, INT8U prio );
 #endif
 
 #if LJ_TASK_CREATE_EXT_EN > 0
 INT8U	 LJTaskCreateExt( void		(*task)( void *pd ),
 						  void		 *pdata,
 						  LJ_STK	 *ptos,
 						  INT8U		  prio,
 						  INT16U	  id,
 						  LJ_STK	 *pbos,
 						  INT32U	  stkSize,
 						  void		 *pext,
 						  INT16U	  opt );
 #endif
 
 #if LJ_TASK_CHANGE_PRIO_EN > 0
 INT8U	 LJTaskChangePrio( INT8U oldPrio, INT8U newPrio );
 #endif

 #if LJ_TASK_CREATE_EXT_EN > 0
 INT8U	 LJTaskStkChk( INT8U prio, LJ_STK_DATA *pdata );
 #endif
 
 #if LJ_TASK_DEL_EN > 0
 INT8U	 LJTaskDel( INT8U prio );
 INT8U	 LJTaskDelReq( INT8U prio );
 #endif
 
 #if LJ_TASK_SUSPEND_EN > 0
 INT8U	 LJTaskSuspend( INT8U prio );
 INT8U	 LJTaskResume( INT8U prio );
 #endif
 
 #if LJ_TASK_QUERY_EN > 0
 INT8U	 LJTaskQuery( INT8U prio, LJ_TCB *pdata );
 #endif
 
/*================================================ʱ�����======================================================*/

 void    LJTimeTick( void );
 void	 LJTimeDly( INT16U ticks );
 
 #if LJ_TIME_DLY_HMSM_EN > 0
 INT8U	 LJTimeDlyHMSM( INT8U hours, INT8U minutes, INT8U seconds, INT16U milli );
 #endif
 
 #if LJ_TIME_DLY_RESUME_EN > 0
 INT8U	 LJTimeDlyResume( INT8U prio );
 #endif
 
 #if LJ_TIME_GET_SET_EN > 0
 INT32U	 LJTimeGet( void );
 void	 LJTimeSet( INT32U ticks );
 #endif

/*===============================================�ź�������=====================================================*/

 #if LJ_SEM_EN > 0
 
 LJ_EVENT	*LJSemCreate( INT16U cnt );
 
 void		 LJSemPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 INT8U		 LJSemPost( LJ_EVENT *pevent );
 
 #if LJ_SEM_DEL_EN > 0
 LJ_EVENT	*LJSemDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_SEM_ACCEPT_EN > 0
 INT16U		 LJSemAccept( LJ_EVENT *pevent );
 #endif
 
 #if LJ_SEM_QUERY_EN > 0
 INT8U		 LJSemQuery( LJ_EVENT *pevent, LJ_SEM_DATA *pdata );
 #endif
 
 #endif
 
/*============================================�������ź�������==================================================*/
 
 #if LJ_MUTEX_EN > 0
 
 LJ_EVENT	*LJMutexCreate( INT8U prio, INT8U *err );
 
 void		 LJMutexPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 INT8U		 LJMutexPost( LJ_EVENT *pevent );
 
 #if LJ_MUTEX_DEL_EN > 0
 LJ_EVENT	*LJMutexDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_MUTEX_ACCEPT_EN > 0
 INT8U		 LJMutexAccept( LJ_EVENT *pevent, INT8U *err );
 #endif
 
 #if LJ_MUTEX_QUERY_EN > 0
 INT8U		 LJMutexQuery( LJ_EVENT *pevent, LJ_MUTEX_DATA *pdata );
 #endif
 
 #endif
 
/*==============================================��Ϣ�������====================================================*/

 #if LJ_MBOX_EN > 0
 
 LJ_EVENT	*LJMboxCreate( void *msg );
 
 void		*LJMboxPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 #if LJ_MBOX_POST_EN > 0
 INT8U		 LJMboxPost( LJ_EVENT *pevent, void *msg, INT8U opt );
 #endif
 
 #if LJ_MBOX_DEL_EN > 0
 LJ_EVENT	*LJMboxDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_MBOX_ACCEPT_EN > 0
 void		*LJMboxAccept( LJ_EVENT *pevent );
 #endif
 
 #if LJ_MBOX_QUERY_EN > 0
 INT8U		 LJMboxQuery( LJ_EVENT *pevent, LJ_MBOX_DATA *pdata );
 #endif
 
 #endif
 
/*==============================================��Ϣ���й���====================================================*/

 #if LJ_Q_EN > 0
 
 LJ_EVENT	*LJQCreate( void **start, INT16U size );
 
 void		*LJQPend( LJ_EVENT *pevent, INT16U timeout, INT8U *err );
 
 #if LJ_Q_POST_EN > 0
 INT8U		 LJQPost( LJ_EVENT *pevent, void *msg, INT8U opt );
 #endif
 
 #if LJ_Q_DEL_EN > 0
 LJ_EVENT	*LJQDel( LJ_EVENT *pevent, INT8U opt, INT8U *err );
 #endif
 
 #if LJ_Q_ACCEPT_EN > 0
 void		*LJQAccept( LJ_EVENT *pevent );
 #endif
 
 #if LJ_Q_FLUSH_EN > 0
 INT8U		 LJQFlush( LJ_EVENT *pevent );
 #endif
 
 #if LJ_Q_QUERY_EN > 0
 INT8U		 LJQQuery( LJ_EVENT *pevent, LJ_Q_DATA *pdata );
 #endif
 
 #endif
 
/*================================================�ڴ����======================================================*/

 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 LJ_MEM		*LJMemCreate( void *addr, INT32U nblks, INT32U blkSize, INT8U *err );
 void		*LJMemGet( LJ_MEM *pmem, INT8U *err );
 INT8U		 LJMemPut( LJ_MEM *pmem, void *pblk );
 
 #if LJ_MEM_QUERY_EN > 0
 INT8U		 LJMemQuery( LJ_MEM *pmem, LJ_MEM_DATA *pdata );
 #endif
 
 #endif
 
/*==============================================�����ԭ��====================================================*/

 void    LJInit( void );
 void    LJStart( void );
 
 void	 LJIntEnter( void );
 void	 LJIntExit( void );
 
 #if LJ_SCHED_LOCK_EN > 0
 void	 LJSchedLock( void );
 void	 LJSchedUnlock( void );
 #endif
 
 INT16U	 LJVersion( void );
 
/*==============================================�ڲ�����ԭ��====================================================*/

 INT8U   LJ_TcbInit( INT8U prio, LJ_STK *ptos, LJ_STK *pbos, INT16U id, INT32U stkSize, void *pext, INT16U opt );
 void    LJ_Sched( void );
 void	 LJ_TaskIdle( void *pdata );
 
 #if LJ_TASK_DEL_EN > 0
 void	 LJ_Dummy( void );
 #endif
 
 #if LJ_EVENT_EN > 0
 void	 LJ_EventWaitListInit( LJ_EVENT *pevent );
 INT8U	 LJ_EventTaskRdy( LJ_EVENT *pevent, void *msg, INT8U msk );
 void	 LJ_EventTaskWait( LJ_EVENT *pevent );
 void	 LJ_EventTo( LJ_EVENT *pevent );
 #endif
 
 #if LJ_Q_EN > 0
 void	 LJ_QInit( void );
 #endif
 
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 void	 LJ_MemInit( void );
 #endif
 
/*============================================��CPU�йغ���ԭ��=================================================*/

 LJ_STK *LJTaskStkInit( void (*task)( void *pd ), void *pdata, LJ_STK *ptos, INT16U opt );	/* c����ʵ��        */
 
 void    LJStartHighRdy( void );												/* �������ʵ��(lj_cpu_asm.s)   */
 void	 LJCtxSw( void );														/* �������ʵ��(lj_cpu_asm.s)   */
 void	 LJTickIsr( void );														/* �������ʵ��(lj_cpu_asm.s)   */
 void	 LJIntCtxSw( void );													/* �������ʵ��(lj_cpu_asm.s)   */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_RTOS_H */