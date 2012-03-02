/*
 *=====================================================================================================
 *                                              LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：printf.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现输出的函数(测试LJ_RTOS)
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/24
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=====================================================================================================
 */
 
 #include "printf.h"
 
 #define UART_MSG_NUM			25
 #define UART_MSG_BUF_LENGTH	100
 
 LJ_STK	gTaskUartStk[TASK_UART_STK_LENGTH];			/* 串口任务堆栈大小                                */
 
 static LJ_MEM   *pUartMem;							/* 指向串口内存分区的指针                          */
 static INT8U     uartBuf[UART_MSG_NUM][UART_MSG_BUF_LENGTH];	/* 串口内存分区大小                    */
 static LJ_EVENT *pUartQ;							/* 指向串口发送接受数据的消息队列                  */
 static void     *msgUart[UART_MSG_NUM];			/* 消息队列中存放消息的指针数组                    */
 
 void
 LJPrintfInit( void )
 {
 	INT8U	 err;
 	
 	pUartMem = LJMemCreate( uartBuf, UART_MSG_NUM, UART_MSG_BUF_LENGTH, &err );/* 为串口创建一个内存分区 */
 	pUartQ   = LJQCreate( &msgUart[0], UART_MSG_NUM );						   /* 为串口创建一个消息队列 */
 	LJTaskCreate( TaskUart, (void *)0, &gTaskUartStk[TASK_UART_STK_LENGTH - 1], TASK_UART_PRIO );
 }
 
 void
 LJPrintf( const char *fmt, ... )
 {
 	char	*pUartBuf;
 	INT8U	 err;
 	va_list	 ap;
 	
 	pUartBuf = LJMemGet( pUartMem, &err );
 	va_start( ap, fmt );
 	vsprintf( pUartBuf, fmt, ap );
 	va_end( ap );
 	LJQPost( pUartQ, pUartBuf, LJ_POST_OPT_NONE );
 }
 
 void
 TaskUart( void *pdata )
 {
 	char	 *pUartMsg;
 	
 	pdata = pdata;
 	
 	while ( 1 )
 	{
 		pUartMsg = LJQAccept( pUartQ );
 		while ( pUartMsg )
 		{
 			UartSendString( pUartMsg );
 			LJMemPut( pUartMem, pUartMsg );
 			pUartMsg = LJQAccept( pUartQ );
 		}
 		
 		LJTimeDly( LJ_TICKS_PER_SEC / 5 );
 	}
 }