/*
 *=====================================================================================================
 *                                              LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�printf.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ������ĺ���(����LJ_RTOS)
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/24
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=====================================================================================================
 */
 
 #include "printf.h"
 
 #define UART_MSG_NUM			25
 #define UART_MSG_BUF_LENGTH	100
 
 LJ_STK	gTaskUartStk[TASK_UART_STK_LENGTH];			/* ���������ջ��С                                */
 
 static LJ_MEM   *pUartMem;							/* ָ�򴮿��ڴ������ָ��                          */
 static INT8U     uartBuf[UART_MSG_NUM][UART_MSG_BUF_LENGTH];	/* �����ڴ������С                    */
 static LJ_EVENT *pUartQ;							/* ָ�򴮿ڷ��ͽ������ݵ���Ϣ����                  */
 static void     *msgUart[UART_MSG_NUM];			/* ��Ϣ�����д����Ϣ��ָ������                    */
 
 void
 LJPrintfInit( void )
 {
 	INT8U	 err;
 	
 	pUartMem = LJMemCreate( uartBuf, UART_MSG_NUM, UART_MSG_BUF_LENGTH, &err );/* Ϊ���ڴ���һ���ڴ���� */
 	pUartQ   = LJQCreate( &msgUart[0], UART_MSG_NUM );						   /* Ϊ���ڴ���һ����Ϣ���� */
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