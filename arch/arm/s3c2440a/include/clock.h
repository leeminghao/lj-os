/*
 *=============================================================================================
 *                                              LJ_TROS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�clock.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ���������s3c2440aʱ����Ƶ�ĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/14
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=============================================================================================
 */
 
 #ifndef __CLOCK_H
 #define __CLOCK_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "regaddr.h"
 #include "define.h"
 
 #define ENTER_PWDN( clkcon )		((void (*)(int))0x20)( clkcon )		/* ��Դ����            */
 
 void StartPointAfterSleepWakeUp( void );								/* �������ʵ��        */
 void ChangeClockDivider( U32 hdivnVal, U32 pdivnVal );
 void ChangeUPLLValue( U32 m, U32 p, U32 s );
 void ChangeMPLLValue( U32 m, U32 p, U32 s );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __CLOCK_H */