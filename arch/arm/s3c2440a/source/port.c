/*
 *============================================================================================
 *                                              LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：port.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现I/O端口的初始化
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/16
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *============================================================================================
 */
 #include "port.h"
 
/*
 *============================================================================================
 * 函数名称：PortInit
 * 功能描述：初始化I/O端口
 * 传    参：void
 * 返 回 值：void
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