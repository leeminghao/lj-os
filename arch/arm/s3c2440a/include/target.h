/*
 *===============================================================================================
 *                                         LJ_RTOS
 *                                   The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�target.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ�����������ʼ��Ŀ���ĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/18
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *===============================================================================================
 */
 
 #ifndef __TARGET_H
 #define __TARGET_H
 
 #ifdef __cplusplus
 extern "C"
 { 
 #endif
 
 #include "define.h"
 #include "irq.h"
 #include "option.h"
 #include "regaddr.h"
 #include "timer.h"
 #include "uart.h"
 #include "clock.h"
 #include "mmu.h"
 #include "lcd.h"
 #include "rtc.h"
 #include "port.h"
 
 void TargetInit( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __TARGET_H */