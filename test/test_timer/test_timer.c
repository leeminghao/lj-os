#include "time.h"
#include "irq.h"
#include "uart.h"

int
Main( void )
{
	UartInit( 0, 115200 );
	Timer0Init( );
	IsrInit( );
	
	return 0;
}