#include "regaddr.h"
#include "time.h"
#include "clock.h"

int
Main( void )
{
	R_CLKCON = 0x0;
	
	ENTER_PWDN( R_CLKCON );
	
	Delay( 50000 );
	StartPointAfterSleepWakeUp( );
	
	return 0;
}