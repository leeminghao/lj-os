/*
 *============================================================================================
 *                                           LJ_RTOS
 *                                    The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�rtc.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ�����������RTC�йصĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/16
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *============================================================================================
 */
 
 #ifndef __RTC_H
 #define __RTC_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 #include "regaddr.h"
 
 #define TO_BCD( n )	((((n) / 10) << 4) | ((n) % 10))
 #define FROM_BCD( n )	((((n) >> 4) * 10) + ((n) & 0xf))
 
 void RtcInit( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __RTC_H */