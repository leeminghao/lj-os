/*
 *===========================================================================================
 *                                            LJ_RTOS
 *                                       The Real-Time Kernel
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�colock.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�ʵ�������޸�s3c2440aʱ��Ƶ�ʵĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/14
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *==========================================================================================
 */
 
 #include "clock.h"
 
/*
 *==========================================================================================
 * �������ƣ�ChangeClockDivider
 * �����������ı�ʱ�ӷ�Ƶ��
 * ��    �Σ�hdivnVal (U32)��FCLK : HCLK
 *           pdivnVal (U32)��HCLK : PCLK
 * �� �� ֵ��void
 *==========================================================================================
 */
 void
 ChangeClockDivider( U32 hdivnVal, U32 pdivnVal )
 {
 	int hdivn = 2;
 	int pdivn = 0;
 	
 	switch ( hdivnVal )
 	{
 		case 11: hdivn = 0; break;
 		case 12: hdivn = 1; break;
 		case 13: hdivn = 3; break;
 		case 16: hdivn = 3; break;
 		case 14: hdivn = 2; break;
 		case 18: hdivn = 2; break;
 	}
 	
 	switch ( pdivnVal )
 	{
 		case 11: pdivn = 0; break;
 		case 12: pdivn = 1; break;
 	}
 	
 	R_CLKDIVN = (hdivn << 1) | pdivn;
 	
 	switch ( hdivnVal )
 	{
 		case 16:
 			R_CAMDIVN = (R_CAMDIVN & ~(3 << 8)) | (1 << 8);
 			break;
 			
 		case 18:
 			R_CAMDIVN = (R_CAMDIVN & ~(3 << 8)) | (1 << 9);
 			break;
 	}
 	
 	if ( hdivn != 0 )
 	{
 		//MMUSetAsyncBusMode( );
 	}
 	else
 	{
 		//MMUSetFastBusMode( );
 	}
 }
 
/*
 *=================================================================================================
 * �������ƣ�ChangeMPLLValue
 * �����������ı�MPLL��ֵ��MPLL = (2 * mdiv * Fin) / (pdiv * 2^sdiv)
 * ��    �Σ�mdiv (U32), pdiv (U32), sdiv (U32)
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 ChangeMPLLValue( U32 mdiv, U32 pdiv, U32 sdiv )
 {
 	R_MPLLCON = (mdiv << 12) | (pdiv << 4) | sdiv;
 }
 
/*
 *=================================================================================================
 * �������ƣ�ChangeUPLLValue
 * �����������ı�UPLL��ֵ��UPLL = (mdiv * Fin) / (pdiv * 2^sdiv)
 * ��    �Σ�mdiv (U32), pdiv (U32), sdiv (U32)
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 ChangeUPLLValue( U32 mdiv, U32 pdiv, U32 sdiv )
 {
 	R_UPLLCON = (mdiv << 12) | (pdiv << 4) | (sdiv);
 }