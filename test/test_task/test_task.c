#include "target.h"
#include "lj_rtos.h"
#include "lj_cpu.h"

LJ_STK	gMainTaskStk[1024 * 2];

void MainTask( void *pdata );

int
Main( void )
{
	TargetInit( );
	
	LJInit( );
	
	LJTaskCreate( MainTask, (void *)0, &gMainTaskStk[1024 * 2 - 1], 1 );
	
	LJStart( );
	
	return 0;
}

void
MainTask( void *pdata )
{
	LcdN35Init( );
}
