/*
 *=================================================================================================
 *                                             LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�main.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ����ڴ�������������LJ_RTOS
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/24
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *=================================================================================================
 */
 
 #include "app.h"
 #include "printf.h"
 
 LJ_STK		gMainTaskStk[MAIN_TASK_STK_LENGTH];
 LJ_STK		gTask0Stk[TASK0_STK_LENGTH];
 LJ_STK		gTask1Stk[TASK1_STK_LENGTH];
 LJ_STK		gTask2Stk[TASK2_STK_LENGTH];
 
 int gYear, gMonth, gDay, gDayOfWeek, gHour, gMinute, gSecond;
 
 int
 Main( int argc, char *argv[] )
 {
 	TargetInit( );											/* ��ʼ��Ŀ���(ʱ�ӡ�MMU���˿ڡ�����) */
 	LJInit( );												/* ��ʼ��LJ_RTOS                       */
 	LJTimeSet( 0 );											/* ��ʼ��ϵͳʱ��                      */
 	
 															/* ����ϵͳ��ʼ����                    */
 	LJTaskCreate( MainTask, (void *)0, &gMainTaskStk[MAIN_TASK_STK_LENGTH - 1], MAIN_TASK_PRIO );
 	
 	LJStart( );												/* ��ʼ�����񣬲�����                  */
 	
 	return 0;
 }
 
 void
 MainTask( void *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	cpuSR;
 #endif
 
 	LJ_ENTER_CRITICAL( );
 	Timer0Init( );											/* ��ʼ����ʱ��                       */
 	IsrInit( );												/* ������ʱ���ж�(ÿ�뷢��200��)      */
 	LJ_EXIT_CRITICAL( );
 	
 	LJPrintfInit( );
 	LJTaskCreate( Task0, (void *)0, &gTask0Stk[TASK0_STK_LENGTH - 1], TASK0_PRIO );
 	LJTaskCreate( Task1, (void *)0, &gTask1Stk[TASK1_STK_LENGTH - 1], TASK1_PRIO );
 	LJTaskCreate( Task2, (void *)0, &gTask2Stk[TASK2_STK_LENGTH - 1], TASK2_PRIO );
 	
 	while ( 1 )
 	{
 		LJPrintf( "\nEnter Main Task\n" );
 		LJTimeDly( LJ_TICKS_PER_SEC );
 	} 
 }
 
 void
 Task0( void *pdata )
 {
 	pdata = pdata;
 	
 	while ( 1 )
 	{
 		LcdN35Init( );
 		LJTimeDly( LJ_TICKS_PER_SEC );
 	}
 }
 
 void
 Task1( void *pdata )
 {
 	INT16U	testCnt = 0;
 	
 	pdata = pdata;
 	
 	while ( 1 )
 	{
 		testCnt++;
 		
 		if ( testCnt % 2 )
 		{
 			R_GPBDAT = 0x0000;
 		}
 		else
 		{
 			R_GPBDAT = 0x07fe;
 		}
 		
 		LJTimeDly( LJ_TICKS_PER_SEC );
 	}
 }
 
 void
 Task2( void *pdata )
 {
 	INT32S	i = 0;
	
	pdata = pdata;
	
	RtcInit( );

	while ( 1 )
	{
		i++;
		if ( i > 99 )
		{
			i = 0;
		}
		
		if ( R_BCDYEAR == 0x99 )
		{
			gYear = 1999;
		}
		else
		{
			gYear		= (2000 + FROM_BCD( R_BCDYEAR & 0xff) );
			gMonth		= FROM_BCD( R_BCDMON & 0x1f );
			gDay			= FROM_BCD( R_BCDDATE & 0x3f );
			gDayOfWeek	= FROM_BCD( R_BCDDAY & 0x7 );
			gHour		= FROM_BCD( R_BCDHOUR & 0x3f );
			gMinute		= FROM_BCD( R_BCDMIN & 0x7f );
			gSecond		= FROM_BCD( R_BCDSEC & 0x7f );
		}
		
		LcdPrintf( 40,  40, COLOR( 0x1F,0x3F,0x1F), COLOR( 0x00,0x00,0x00), 1, "%02d:%02d:%02d\n", gHour, gMinute, gSecond );
     	LcdPrintf( 40,  15, COLOR( 0x1F,0x3F,0x1F), COLOR( 0x00,0x00,0x00), 1, "%4d-%02d-%02d ����%d \n",
         	       gYear, gMonth, gDay, gDayOfWeek );
         	       
        LJTimeDly( LJ_TICKS_PER_SEC );
    }
 }