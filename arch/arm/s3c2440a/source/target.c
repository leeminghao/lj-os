/*
 *==============================================================================================
 *                                           LJ_RTOS
 *                                     The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：target.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现初始化目标板的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/18
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==============================================================================================
 */
 
 #include "target.h"
 
/*
 *==============================================================================================
 * 函数名称：TargetInit
 * 功能描述：初始化目标板，完成以下功能：
 *           1. 时钟初始化
 *           2. mmu初始化
 *           3. 串口初始化
 *           4. 串口初始化
 * 传    参：void
 * 返 回 值：void
 *==============================================================================================
 */
 void
 TargetInit( void )
 {
 	U32		i;
 	U8		key;
 	U32		mpllVal = 0;
 	
 	i		= 2;						/* 使用400MHz主频                                      */
 	switch ( i )
 	{
 		case 0:							/* 200MHz */
 			key = 12;
 			mpllVal = (92 << 12) | (4 << 4) | (1);
 			break;
 			
 		case 1:							/* 300MHz */
 			key = 14;
 			mpllVal = (67 << 12) | (1 << 4) | (1);
 			break;
 			
 		case 2:							/* 400MHz */
 			key = 14;
 			mpllVal = (92 << 12) | (1 << 4) | (1);
 			break;
 			
 		case 3:							/* 440MHz */
 			key = 14;
 			mpllVal = (102 << 12) | (1 << 4) | (1);
 			break;
 			
 		default:						/* 默认使用400MHz */
 			key = 14;
 			mpllVal = (92 << 12) | (1 << 4) | (1);
 			break;
 	}
 	
 	ChangeMPLLValue( (mpllVal >> 12) & 0xff, (mpllVal >> 4) & 0x3f, mpllVal & 3 );	/* 时钟初始化 */
 	ChangeClockDivider( key, 12 );
 	
 	MMUDisableICache( );				
 	MMUDisableDCache( );
 	MMUInit( );							/* MMU初始化                                           */
 	
 	PortInit( );						/* 端口初始化                                          */
 	
 	UartSelect( 0 );					/* 串口选择                                            */
 	UartInit( 0, 115200 );				/* 串口初始化                                          */
 }