/*
 *===========================================================================================
 *                                            LJ_RTOS
 *                                       The Real-Time Kernel
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：colock.c
 * 文件标识：c语言源文件
 * 摘    要：本文件实现用于修改s3c2440a时钟频率的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/14
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==========================================================================================
 */
 
 #include "clock.h"
 
/*
 *==========================================================================================
 * 函数名称：ChangeClockDivider
 * 功能描述：改变时钟分频比
 * 传    参：hdivnVal (U32)：FCLK : HCLK
 *           pdivnVal (U32)：HCLK : PCLK
 * 返 回 值：void
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
 * 函数名称：ChangeMPLLValue
 * 功能描述：改变MPLL的值：MPLL = (2 * mdiv * Fin) / (pdiv * 2^sdiv)
 * 传    参：mdiv (U32), pdiv (U32), sdiv (U32)
 * 返 回 值：void
 *=================================================================================================
 */
 void
 ChangeMPLLValue( U32 mdiv, U32 pdiv, U32 sdiv )
 {
 	R_MPLLCON = (mdiv << 12) | (pdiv << 4) | sdiv;
 }
 
/*
 *=================================================================================================
 * 函数名称：ChangeUPLLValue
 * 功能描述：改变UPLL的值，UPLL = (mdiv * Fin) / (pdiv * 2^sdiv)
 * 传    参：mdiv (U32), pdiv (U32), sdiv (U32)
 * 返 回 值：void
 *=================================================================================================
 */
 void
 ChangeUPLLValue( U32 mdiv, U32 pdiv, U32 sdiv )
 {
 	R_UPLLCON = (mdiv << 12) | (pdiv << 4) | (sdiv);
 }