#include "lcd.h"
#include "rtc.h"
#include "port.h"

S32 year, month, day, dayOfWeek, hour, minute, second;

int
Main( void )
{
	U32 i = 0;
	
	PortInit( );
	RtcInit( );
	LcdN35Init( );
	
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
		
		LcdPrintf( 40,  40, COLOR( 0x1F,0x3F,0x1F), COLOR( 0x00,0x00,0x00),0,"%02d:%02d:%02d\n", hour, minute, second );
     	LcdPrintf( 40,  15, COLOR( 0x1F,0x3F,0x1F), COLOR( 0x00,0x00,0x00),0,"%4d-%02d-%02d ÐÇÆÚ%d \n",
         	       year, month, day,dayOfWeek );
	}
	
	return 0;
}