/*
 *=====================================================================================================
 *                                            LJ_RTOS
 *                                      The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_cfg.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ������û���LJ_RTOS��������ʹ��
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/17
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=====================================================================================================
 */
 
 #ifndef __LJ_CFG_H
 #define __LJ_CFG_H
 
 #ifdef __cpluspluls
 extern "C"
 {
 #endif
 
 											/*========================�����ջ��С====================*/
 #define LJ_TASK_IDLE_STK_SIZE		128		/* ���������ջ��С(ָ��Ԫ��Ŀ)                           */
 
 											/*============================����========================*/
 #define LJ_LOWEST_PRIO				63		/* ����LJ_RTOS������ȼ�����ע�⣺���� < 64             */
 #define LJ_ARG_CHK_EN				1		/* �����趨�Ƿ���������LJ_RTOS�еĺ���ִ�в������      */
 											/* ���ܣ�1 = ����0 = ������                             */
 #define LJ_MAX_TASKS				30		/* ���ڶ����û������п���ʹ�õ����������                 */
 #define LJ_MAX_EVENTS				20		/* ���ڶ���ϵͳ�пɷ�����¼����ƿ�������Ŀ             */
 #define LJ_MAX_QS					25		/* ���ڶ���ϵͳ�п��Դ�������Ϣ���е������Ŀ             */
 #define LJ_MAX_MEM_PART			25		/* ���ڶ���ϵͳ���ڴ����������                         */
 #define LJ_SCHED_LOCK_EN			1		/* �����Ƿ�ʹ��LJSchedLock()��LJSchedUnlock()             */
 
 											/*==========================�������======================*/
 #define LJ_TASK_CHANGE_PRIO_EN		1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJTaskChangePrio()����          */
 #define LJ_TASK_CREATE_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJTaskCreate()����              */
 #define LJ_TASK_CREATE_EXT_EN		1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJTaskCreateExt()����           */
 #define LJ_TASK_DEL_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJTaskDel()��LJTaskDelReq()���� */
 #define LJ_TASK_SUSPEND_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJTaskSuspend()��LJTaskResume() */
 #define LJ_TASK_QUERY_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJTaskQuery()����               */
 #define LJ_TICKS_PER_SEC			200		/* �趨ϵͳÿ�������ϵͳʱ�ӽ�����                       */
 
 											/*==========================ʱ�����======================*/
 #define LJ_TIME_DLY_HMSM_EN		1		/* �����Ƿ�ʹ��LJTimeDlyHMSM()����                        */
 #define LJ_TIME_DLY_RESUME_EN		1		/* �����Ƿ�ʹ��LJTimeDlyResume()����                      */
 #define LJ_TIME_GET_SET_EN			1		/* �����Ƿ�ʹ��LJTimeGet()��LJTimeSet()����               */
 
 											/*===========================�ź���=======================*/
 #define LJ_SEM_EN					1		/* �����Ƿ�ʹ��LJ_RTOS�е��ź���������                  */
 #define LJ_SEM_ACCEPT_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJSemAccept()����               */
 #define LJ_SEM_DEL_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJSemDel()����                  */
 #define LJ_SEM_QUERY_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJSemQuery()����                */
 
 											/*========================�������ź���====================*/
 #define LJ_MUTEX_EN				1		/* �����Ƿ�ʹ�û������ź�����������������ݽṹ           */
 #define LJ_MUTEX_ACCEPT_EN 		1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMutexAccept()����             */
 #define LJ_MUTEX_DEL_EN 			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMutexDel()����                */
 #define LJ_MUTEX_QUERY_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMutexQuery()����              */
 
 											/*==========================��Ϣ����======================*/
 #define LJ_MBOX_EN					1		/* �����Ƿ�ʹ��LJ_RTOS�е���Ϣ���亯������������ݽṹ    */
 #define LJ_MBOX_POST_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMboxPost()����                */
 #define LJ_MBOX_DEL_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMboxDel()����                 */
 #define LJ_MBOX_ACCEPT_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMboxAccept()����              */
 #define LJ_MBOX_QUERY_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMboxQuery()����               */
 
 											/*==========================��Ϣ����======================*/
 #define LJ_Q_EN					1		/* �����Ƿ�ʹ��LJ_RTOS�е���Ϣ���к���������ص����ݽṹ  */
 #define LJ_Q_POST_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJQPost()����                   */
 #define LJ_Q_DEL_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJQDel()����                    */
 #define LJ_Q_ACCEPT_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJQAccept()����                 */
 #define LJ_Q_FLUSH_EN				1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJQFlush()����                  */
 #define LJ_Q_QUERY_EN              1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJQQuery()����                  */
 
 											/*==========================�ڴ����======================*/
 #define LJ_MEM_EN					1		/* �����Ƿ�ʹ��LJ_RTOS�е��ڴ�����������������ݽṹ  */
 #define LJ_MEM_QUERY_EN			1		/* �����Ƿ�ʹ��LJ_RTOS�е�LJMemQuery()����                */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_CFG_H */