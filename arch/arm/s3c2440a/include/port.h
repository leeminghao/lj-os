/*
 *============================================================================================
 *                                           LJ_RTOS
 *                                     The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�port.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ���������I/O�˿ڵĳ�ʼ������
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
 
 #ifndef __PORT_H
 #define __PORT_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "regaddr.h"
 
 void PortInit( void );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __PORT_H */