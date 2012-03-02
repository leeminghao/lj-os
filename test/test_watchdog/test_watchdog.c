#include "irq.h"
#include "uart.h"
#include "regaddr.h"

static int g_count;

void		WatchDogTest( void );
__irq void	WatchDogIsr( void );

int
Main( void )
{
	UartInit( 0, 115200 );
	UartPrintf( "watchdog test is beginning\n" );
	WatchDogTest( );
	
	return 0;
}

void
WatchDogTest( void )
{
	ISR_WDT_AC97 = (U32)WatchDogIsr;
	
	ClearPending( BIT_WDT_AC97 );
	
	R_WTCON  = ((99 << 8) | (3 << 3));
	R_WTDAT  = 1500;
	R_WTCNT  = 1500;
	R_WTCON |= (1 << 5) | (1 << 2);
	
	ENABLE_IRQ( BIT_WDT_AC97 );
	ENABLE_SUB_IRQ( BIT_SUB_WDT );
	
	while ( g_count < 11 )
	{
		//UartPrintf( "R_WTCNT = %d\n", R_WTCNT );
	}
}

__irq void 
WatchDogIsr( void )
{
	g_count++;
	
	if ( g_count < 11 )
	{
		UartPrintf( "%ds\n", g_count );
		//UartPrintf( "R_WTCNT = %d\n", R_WTCNT );
	}
	else
	{
		DISABLE_IRQ( BIT_WDT_AC97 );
		DISABLE_SUB_IRQ( BIT_SUB_WDT );
		UartPrintf( "watch dog interrupt is running\n" );
	}
	
	ClearPending( BIT_WDT_AC97 );
	R_SUBSRCPND = BIT_SUB_WDT;
	//ClearSubPending( BIT_SUB_WDT );
}