/*
 *================================================================================
 *										LJ_RTOS
 *								  The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rigths Reserved
 *
 * �ļ����ƣ�option.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ�����һЩ������ַ��������ʱ�ӵ�����
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/10
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *================================================================================
 */
 
 #ifndef __OPTION_H
 #define __OPTION_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #define USE_MAIN			FALSE
 
 #define FIN 	(12000000)				/* ����Ƶ��                                */
 //#define FIN		(16934400)

 #if FIN==12000000
 #define FCLK 304800000
 //#define FCLK 300000000
 //#define FCLK 400000000
 //#define FCLK 451200000
 //#define FCLK 532800000
	#if FCLK==271500000
	#define HCLK (FCLK/2)
	#define PCLK (HCLK/2)
	#elif FCLK==304800000
	#define HCLK (FCLK/3)
	#define PCLK (HCLK/2)
	#elif FCLK==300000000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==400000000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==451200000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==532800000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#endif
 #else	//FIN=16.9344MHz
 #define FCLK 296352000
	#if FCLK==266716800
	#define HCLK (FCLK/2)
	#define PCLK (HCLK/2)
	#elif FCLK==296352000
	#define HCLK (FCLK/3)
	#define PCLK (HCLK/2)
	#elif FCLK==399651840
	#define HCLK (FCLK/3)
	#define PCLK (HCLK/2)
	#elif FCLK==530611200
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==541900800
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#endif
 #endif

 #define UCLK 48000000					/* USBʱ��                                      */
 
 #define RAM_STARTADDRESS 	0x30000000
 #define MMUTT_STARTADDRESS 0x33ff8000
 #define STACK_BASEADDRESS	0x33ff8000
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __OPTION_H */