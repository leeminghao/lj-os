/*
 *==========================================================================================
 * 											LJ_RTOS
 *									  The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：uart.c
 * 文件标识：c语言源文件
 * 摘    要：本文件实现目标板通过串口与宿主机通信的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/11
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==========================================================================================
 */
 
 #include "uart.h"
 
 static U32 s_whichUart = 0;	// 定于静态变量用于实现串口的选择
 
/*
 *===============================================================================================
 * 函数名称：UartInit
 * 功能描述：初始化所有串口，主要为设置波特率和寄存器配置
 * 传    参：pclk (U32)：为APB总线上外设提供的时钟大小默认设置为50MHz，其为0时选择默认设置
 *           baud (U32)：波特率大小，即串口的默认传输速率
 * 返 回 值：void
 *===============================================================================================
 */
 void
 UartInit( U32 pclk, U32 baud )
 {
 	if ( pclk == 0 )
 	{
 		pclk = PCLK;
 	}
 	
 	R_UFCON0 = 0x0;		// 禁用Uart0 FIFO
 	R_UFCON1 = 0x0;		// 禁用Uart1 FIFO
 	R_UFCON2 = 0x0;		// 禁用Uart2 FIFO
 	R_UMCON0 = 0x0;		// 在禁用AFC模式下，不激活nRST, Uart0
 	R_UMCON1 = 0x0;		// 在禁用AFC模式下，不激活nRST, Uart1
 	
 	R_ULCON0  = 0x3;	// 1位停止位，8位数据位数
 	R_UCON0   = 0x245;
 	R_UBRDIV0 = ((int)(pclk / 16 / baud + 0.5) - 1);
 	
 	R_ULCON1  = 0x3;
 	R_UCON1   = 0x245;
 	R_UBRDIV1 = ((int)(pclk / 16 / baud + 0.5) - 1);
 	
 	R_ULCON2  = 0x3;
 	R_UCON2   = 0x245;
 	R_UBRDIV2 = ((int)(pclk / 16 / baud + 0.5) - 1);
 	
 	UartTxEmpty( s_whichUart );	// 等待所选串口的传送缓冲区为空，传送移位寄存器为空
 }
 
 void
 UartSelect( U32 port )			
 {
 	s_whichUart = port;			// 选择串口
 }
 
/*
 *=================================================================================================
 * 函数名称：UartTxEmpty
 * 功能描述：等待串口缓冲区为空，并且传送移位寄存器为空
 * 传    参：port (U32)：串口号
 * 返 回 值：void
 *=================================================================================================
 */
 void
 UartTxEmpty( U32 port )
 {
 	if ( port == 0 )
 	{
 		while ( !(R_UTRSTAT0 & 0x4) )
 			;
 	}
 	else if ( port == 1 )
 	{
 		while ( !(R_UTRSTAT1 & 0x4) )
 			;
 	}
 	else if ( port == 2 )
 	{
 		while ( !(R_UTRSTAT2 & 0x4) )
 			;
 	}
 }
 
/*
 *=================================================================================================
 * 函数名称：UartGetCh
 * 功能描述：返回接受缓冲区中接受到的一个数据，如果接受缓冲区中没有数据，则一直等待
 * 传    参：void
 * 返 回 值：U8：接受缓冲区中接受到的一个数据
 *=================================================================================================
 */
 char
 UartGetCh( void )
 {
 	if ( s_whichUart == 0 )
 	{
 		while ( !(R_UTRSTAT0 & 0x1) )
 			;
 		return READ_URXH0;
 	}
 	else if ( s_whichUart == 1 )
 	{
 		while ( !(R_UTRSTAT1 & 0x1) )
 			;
 		return READ_URXH1;
 	}
 	else if ( s_whichUart == 2 )
 	{
 		while ( !(R_UTRSTAT2 & 0x1) )
 			;
 		return READ_URXH2;
 	}
 }
 
/*
 *==================================================================================================
 * 函数名称：UartGetChAccept
 * 功能描述：返回接受缓冲区中接受到的一个数据，如果接受缓冲区中没有数据，则返回0
 * 传    参：void
 * 返 回 值：0：缓冲区中没有数据	U8：接受缓冲区中的一个数据
 *==================================================================================================
 */
 char
 UartGetChAccept( void )
 {
 	if ( s_whichUart == 0 )
 	{
 		if ( R_UTRSTAT0 & 0x1 )
 		{
 			return READ_URXH0;
 		}
 		else
 		{
 			return 0;
 		}
 	}
 	else if ( s_whichUart == 1 )
 	{
 		if ( R_UTRSTAT1 & 0x1 )
 		{
 			return READ_URXH1;
 		}
 		else
 		{
 			return 0;
 		}
 	}
 	else if ( s_whichUart == 2 )
 	{
 		if ( R_UTRSTAT2 & 0x1 )
 		{
 			return READ_URXH2;
 		}
 		else
 		{
 			return 0;
 		}
 	}
 }
 
/*
 *=================================================================================================
 * 函数名称：UartGetString
 * 功能描述：从宿主机获取一个字符串
 * 传    参：pstr (U8 *)：字符串缓冲区
 * 返 回 值：void
 *=================================================================================================
 */
 void
 UartGetString( char *pstr )
 {
 	char *pstr2 = pstr;
 	char  ch;
 	
 	while ( (ch = UartGetCh( )) != '\r' )
 	{
 		if ( ch == '\b' )
 		{
 			if ( (int)pstr2 < (int)pstr )
 			{
 				UartPrintf( "\b \b" );
 				pstr--;
 			}
 		}
 		else
 		{
 			*pstr++ = ch;
 			UartSendByte( ch );
 		}
 	}
 	*pstr = '\0';
 	UartSendByte( '\n' );
 }
 
/*=================================================================================================
 * 函数名称：UartSendByte
 * 功能描述：向宿主机发送一个字节数据
 * 传    参：data (U8)：想要发送的数据
 * 返 回 值：void
 *=================================================================================================
 */
 void
 UartSendByte( char data )
 {
 	if ( s_whichUart == 0 )
 	{
 		if ( data == '\n' )
 		{
 			while ( !(R_UTRSTAT0 & 0x2) )			// 等待发送缓冲区为空
 				;
 			Delay( 10 );
 			WRITE_UTXH0( '\r' );
 		}
 		
 		while ( !(R_UTRSTAT0 & 0x2) )
 			;
 		Delay( 10 );
 		WRITE_UTXH0( data );
 	}
 	else if ( s_whichUart == 1 )
 	{
 		if ( data == '\n' )
 		{
 			while ( !(R_UTRSTAT0 & 0x2) )
 				;
 			Delay( 10 );
 			WRITE_UTXH0( '\r' );
 		}
 		
 		while ( !(R_UTRSTAT0 & 0x2) )
 			;
 		Delay( 10 );
 		WRITE_UTXH0( data );
 	}
 	else if ( s_whichUart == 2 )
 	{
 		if ( data == '\n' )
 		{
 			while ( !(R_UTRSTAT0 & 0x2) )
 				;
 			Delay( 10 );
 			WRITE_UTXH0( '\r' );
 		}
 		
 		while ( !(R_UTRSTAT0 & 0x2) )
 			;
 		Delay( 10 );
 		WRITE_UTXH0( data );
 	} 
 }
 
/*
 *=================================================================================================
 * 函数名称：UartSendString
 * 功能描述：通过串口向宿主机发送一个字符串
 * 传    参：pstr (U8 *)：将要发送的字符串
 * 返 回 值：void
 *=================================================================================================
 */
 void
 UartSendString( char *pstr )
 {
 	while ( *pstr != '\0' )
 	{
 		UartSendByte( *pstr++ );
 	}
 }
 
 #if !USE_MAIN
/*
 *=================================================================================================
 * 函数名称：UartPrintf
 * 功能描述：通过串口向宿主机输出字符串
 * 传    参：fmt (const U8 *)：将要输出的字符串
 * 返 回 值：void
 *=================================================================================================
 */
 void
 UartPrintf( const char *fmt, ...)
 {
 	va_list ap;
 	char    buf[STR_LEN];
 	
 	va_start( ap, fmt );
 	vsprintf( buf, fmt, ap );
 	va_end( ap );
 	
 	UartSendString( buf );
 }
 #endif