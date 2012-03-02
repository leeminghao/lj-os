/*
 *============================================================================================
 *                                              LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�port.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��I/O�˿ڵĳ�ʼ��
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
 #include "port.h"
 
/*
 *============================================================================================
 * �������ƣ�PortInit
 * ������������ʼ��I/O�˿�
 * ��    �Σ�void
 * �� �� ֵ��void
 *============================================================================================
 */
 void
 PortInit( void )
 {
 	R_GPACON = 0x7fffff;
 	
 	R_GPBCON = 0x155555;
 	R_GPBUP  = 0x7ff;
 	
 	R_GPCCON = 0xaaaaaaaa;
 	R_GPCUP  = 0xffff;
 	
 	R_GPDCON = 0xaaaaaaaa;
 	R_GPDUP  = 0xffff;
 	
 	R_GPECON = 0xaaaaaaaa;
 	R_GPEUP  = 0xffff;
 	
 	R_GPFCON = 0x55aa;
 	R_GPFUP  = 0xff;
 	
 	R_GPGCON = 0xff95ffba;
 	R_GPGUP  = 0xffff;
 	
 	R_GPHCON = 0x2afaaa;
 	R_GPHUP  = 0x7ff;
 	
 	R_GPJDAT = (1 << 12) | (0 << 11);
 	R_GPJCON = 0x016aaaa;
 	R_GPJUP  = ~(0 << 12) | (1 << 11);
 	
 	R_GPJDAT = (0 << 12) | (0 << 11);
 	R_GPJCON = 0x016aaaa;
 	R_GPJUP  = 0x1fff;
 	
 	R_EXTINT0 = 0x22222222;
 	R_EXTINT1 = 0x22222222;
 	R_EXTINT2 = 0x22222222;
 }