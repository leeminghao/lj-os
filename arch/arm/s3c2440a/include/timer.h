/*
 *=============================================================================================
 * 												LJ_RTOS
 * 										  The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�time.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ������붨ʱ�����йصĺ���
 * 
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/11
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=============================================================================================
 */
 
 #ifndef __TIME_H
 #define __TIME_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 #include "option.h"
 #include "regaddr.h"
 #include <lj_cfg.h>
 
 void Delay( U32 time );
 void Timer0Init( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif		/* __TIME_H */