#include "uart.h"

int
Main( void )
{
	U8 ch;
	char buf[STR_LEN];
	
	while ( (ch = UartGetChAccept( )) != 0 )
	{
		UartSendByte( ch );
	}
	
	return 0;
}