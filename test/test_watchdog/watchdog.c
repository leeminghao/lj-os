#include "time.h"
#include "irq.h"
#include "regaddr.h"
#include "uart.h"

static void __irq
WatchDogIsr( void )
{
	UartPrintf( "Watchdog isr\n" );
	R_GPBDAT |= 0x1;
	
	R_SRCPND    = 0x1 << 9;
	R_SUBSRCPND = 0x1 << 13;
	R_INTPND    = 0x1 << 9;
}

int
Main( void )
{
	int light;
	int temp;
	int i;
	
	R_GPBCON = 0x111401;
	R_GPBUP  = 0x7ff;
	
	R_WTCON  = 0xf9 << 8;
	R_WTDAT  = 50000;		// ³¬Ê±Îª4Ãë
	R_WTCNT  = 50000;
	
	R_WTCON     = (1 << 5) | (1 << 2);
	R_SRCPND    = 0x1 << 9;
	R_SUBSRCPND = 0x1 << 13;
	R_INTPND    = 0x1 << 9;
	R_SUBINTMSK = ~(0x1 << 13);
	R_INTMSK    = ~(0x1 << 9);
	
	ISR_WDT_AC97 = (U32)WatchDogIsr;
	
	while ( 1 )
	{
		light = 0x10;
		
		for ( i = 0; i < 4; i++ )
		{
			if ( i < 2 )
			{
				light    = light << 1;
				temp     = light | 1;
				R_GPBDAT = ~temp;
			}
			else
			{
				light    = light << 2;
				temp     = light | 1;
				R_GPBDAT = ~temp; 
			}
			Delay( 50000000 );
		}
	}
	
	return 0;
}