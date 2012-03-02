#include <lj_rtos.h>
#include <target.h>
#include <app_cfg.h>

LJ_STK	MainTaskStk[MainTaskStkLength];
LJ_STK	Task0Stk[Task0StkLength];
LJ_STK	Task1Stk[Task1StkLength];
LJ_STK	Task2Stk[Task2StkLength];

S32 year, month, day, dayOfWeek, hour, minute, second;

S32 testCnt = 0;

LJ_EVENT *pevent;
void     *msgTbl[10];

int
Main( void )
{
	TargetInit( );
	
	LJInit( );
	
	LJTimeSet( 0 );
	
	pevent = LJQCreate( msgTbl, 10 );
	
	LJTaskCreate( MainTask, (void *)0, &MainTaskStk[MainTaskStkLength - 1], MainTaskPrio );
	
	LJStart( );
	
	return 0;
}

void
MainTask( void *pdata )
{
#if LJ_CRITICAL_METHOD == 1
	LJ_CPU_SR	cpuSR;
#endif
	INT8U		*err;
	INT8U		*msg = NULL;
	pdata = pdata;
	
	LJ_ENTER_CRITICAL( );
	
	Timer0Init( );
	IsrInit( );
	
	LJ_EXIT_CRITICAL( );
	
	//pevent = LJMutexCreate( 4, err );
	//pevent = LJMboxCreate( (void *)0 );
	LJTaskCreate( Task0, (void *)0, &Task0Stk[Task0StkLength - 1], Task0Prio );
	//LJTaskCreate( Task1, (void *)0, &Task1Stk[Task1StkLength - 1], Task1Prio );
	LJTaskCreate( Task2, (void *)0, &Task2Stk[Task2StkLength - 1], Task2Prio );
	
	while ( 1 )
	{
		//LJSchedLock( );	
		//UartPrintf( "*****************\n" );
		UartPrintf( "\nEnter Main Task\n" );
		//msg = (INT8U *)LJMboxPend( pevent, 0, err );
		
		//msg = (INT8U *)LJMboxAccept( pevent );
		//msg = (INT8U *)LJQPend( pevent, 0, err );
		msg = (INT8U *)LJQAccept( pevent );
		UartPrintf( "main: %s\n", msg );
		LJTimeDlyHMSM( 0, 0, 1, 0 );
		//UartPrintf( "*****************\n" );
		
		//LJMutexPend( pevent, 0, err );
		//LJMutexAccept( pevent, err );
		//UartPrintf( "aaaaaa\n" );
		//LJMutexPost( pevent );
		
		
		/*if ( LJTaskDelReq( LJ_PRIO_SELF ) == LJ_TASK_DEL_REQ )
		{
			LJTaskDel( LJ_PRIO_SELF );
		}*/
	}
	//LJSchedUnlock( );
}

void
Task0( void *pdata )
{
	LJ_TCB	tcb;
	INT8U	*err;
	INT8U   *msg = 0;
	pdata = pdata;
	
	while ( 1 )
	{
		LcdN35Init( );
		//UartPrintf( "-------------\n" );
		UartPrintf( "ticks = %d\n", LJTimeGet( ) ); 
		//LJMboxDel( pevent, LJ_DEL_ALWAYS, err );
		//LJTaskChangePrio( 8, 4 );
		//msg = (INT8U *)LJMboxPend( pevent, 0, err );
		//UartPrintf( "%s\n", msg );
		//UartPrintf( "--------------\n" );
		
		//LJMutexPend( pevent, 0, err );
		//LJMutexAccept( pevent, err );
		//UartPrintf( "bbbbb\n" );
		LJQPost( pevent, "hello\n", 2 );
		LJQFlush( pevent );
		//LJQDel( pevent, LJ_DEL_ALWAYS, err );
		LJTimeDly( 200 );
		//LJMutexPost( pevent );
		//LJMutexDel( pevent, LJ_DEL_NO_PEND, err );
		//LJTimeDly( 200 );
		
		
		//LJTaskSuspend( 5 );
		//pevent = LJSemCreate( 0 );
		//LJSemPend( pevent, 0, err );
		//LJSemAccept( pevent );
		/*if ( LJTaskDelReq( LJ_PRIO_SELF ) == LJ_TASK_DEL_REQ )
		{
			LJTaskDel( LJ_PRIO_SELF );
		}
		
		while ( LJTaskDelReq( LJ_PRIO_SELF ) != LJ_TASK_NOT_EXIST )
		{
			LJTimeDly( 1 );
		}*/
		//LJTaskChangePrio( LJ_PRIO_SELF, 4 );
		//LJTimeDlyResume( 5 );
		/*LJTaskQuery( LJ_PRIO_SELF, &tcb );
		UartPrintf( "%x\n", tcb.ljTcbStkTop );
		UartPrintf( "%d\n", tcb.ljTcbDly );
		UartPrintf( "%d\n", tcb.ljTcbStat );
		UartPrintf( "%d\n", tcb.ljTcbPrio );
		UartPrintf( "%d\n", tcb.ljTcbX );
		UartPrintf( "%d\n", tcb.ljTcbY );
		UartPrintf( "%d\n", tcb.ljTcbBitX );
		UartPrintf( "%d\n", tcb.ljTcbBitY );*/
	}
}

void
Task1( void *pdata )
{
	pdata = pdata;
	
	R_GPBCON  = 0x1dd7fc;
	R_GPBDAT  = ((1 << 5) | (1 << 6) | (1 << 8) | (1 << 10));
	R_GPBDAT &= 0xffe;
	R_GPBUP   = 0x00;
	
	while ( 1 )
	{
		testCnt++;
		
		if ( testCnt % 2 )
		{
			R_GPBDAT = 0x0000;
		}
		else
		{
			//R_GPBDAT = 0x07efe;
		}
		
		LJTimeDly( 200 );
	}
}

void
Task2( void *pdata )
{
	INT32S	i = 0;
	INT8U   *err;
	INT8U   *msg = 0;
	
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
			year = 1999;
		}
		else
		{
			year		= (2000 + FROM_BCD( R_BCDYEAR & 0xff) );
			month		= FROM_BCD( R_BCDMON & 0x1f );
			day			= FROM_BCD( R_BCDDATE & 0x3f );
			dayOfWeek	= FROM_BCD( R_BCDDAY & 0x7 );
			hour		= FROM_BCD( R_BCDHOUR & 0x3f );
			minute		= FROM_BCD( R_BCDMIN & 0x7f );
			second		= FROM_BCD( R_BCDSEC & 0x7f );
		}
		
		LcdPrintf( 40,  40, COLOR( 0x1F,0x3F,0x1F), COLOR( 0x00,0x00,0x00), 1, "%02d:%02d:%02d\n", hour, minute, second );
     	LcdPrintf( 40,  15, COLOR( 0x1F,0x3F,0x1F), COLOR( 0x00,0x00,0x00), 1, "%4d-%02d-%02d ÐÇÆÚ%d \n",
         	       year, month, day, dayOfWeek );
        
        //msg = (INT8U *)LJQPend( pevent, 0, err );
		//UartPrintf( "task2: %s\n", msg );
        LJTimeDly( 200 );
        //LJMboxPost( pevent, "lj, I love you", 0 );
        //LJSemDel( pevent, LJ_DEL_ALWAYS, err );
        //LJSemPost( pevent );
       // LJTaskResume( 5 );
	}
	
}