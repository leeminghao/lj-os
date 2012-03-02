#define GPBCON	(*(volatile unsigned *)0x56000010)
#define GPBDAT	(*(volatile unsigned *)0x56000014)
#define GPBUP	(*(volatile unsigned *)0x56000018)

void
Delay( int x )
{
	int k, j;
	
	while ( x )
	{
		for ( k = 0; k <= 0xff; k++ )
		{
			for ( j = 0; j <= 0xff; j++ )
				;
		}
		x--;
	}
}

int
Main( void )
{
	GPBCON  = 0x1dd7fc;
	GPBDAT  = ((1 << 5) | (1 << 6) | (1 << 8) | (1 << 10));
	GPBDAT &= 0xffe;
	GPBUP   = 0x00;
	
	while ( 1 )
	{
		GPBDAT = ~(1 << 5);
		Delay( 500 );
		GPBDAT = ~(1 << 6);
		Delay( 500 );
		GPBDAT = ~(1 << 8);
		Delay( 500 );
		GPBDAT = ~(1 << 10);
		Delay( 500 );
	}
	
	return 0;
}