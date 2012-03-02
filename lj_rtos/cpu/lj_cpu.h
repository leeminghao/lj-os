/*
 *=============================================================================================
 *                                                LJ_RTOS
 *                                          The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_cpu.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ�����������CPU��������йص���������
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/16
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=============================================================================================
 */
 
 #ifndef __LJ_CPU_H
 #define __LJ_CPU_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 typedef unsigned char    BOOLEAN;					/* ��������                                */
 typedef unsigned char    INT8U;					/* �޷���8λ����                           */
 typedef signed   char    INT8S;					/* �з���8λ����                           */
 typedef unsigned short   INT16U;					/* �޷���16λ����                          */
 typedef signed   short   INT16S;					/* �з���16λ����                          */
 typedef unsigned int     INT32U;					/* �޷���32λ����                          */
 typedef signed   int     INT32S;					/* �з���32λ����                          */
 
 typedef INT32U			  LJ_STK;					/* ���ö�ջ���Ϊ32λ                      */
 
 typedef INT32U			  LJ_CPU_SR;				/* ����CPU״̬�Ĵ������Ϊ32λ             */
 
 
 #define LJ_CRITICAL_METHOD		1					/* ����ѡ�񿪹��жϵķ���                  */
 
 #if LJ_CRITICAL_METHOD == 1
 #define LJ_ENTER_CRITICAL( ) (cpuSR = LJCpuSaveSR( ))	  	  /* �����ٽ�����Ҳ���ǹ��ж�      */
 #define LJ_EXIT_CRITICAL( )   (LJCpuRestoreSR( cpuSR ))	  /* �˳��ٽ�����Ҳ���ǿ��ж�      */
 #endif
 
 #define LJ_STK_GROWTH		1	/* 1 = ��ջ�Ӹߵ�ַ��͵�ַ����, 0 = ��ջ�ӵ͵�ַ��ߵ�ַ����  */
 
 #define LJ_TASK_SW( )		LJCtxSw( )				/* �����л�                                */
 
 #if LJ_CRITICAL_METHOD == 1						/* ���������жϺ���                        */
 LJ_CPU_SR LJCpuSaveSR( void );
 void      LJCpuRestoreSR( LJ_CPU_SR cpuSr );
 #endif
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_CPU_H */