#include "irq.h"
#include "uart.h"
#include "time.h"
#include "regaddr.h"
#include "lj_cpu.h"

static void __irq 
Eint0Isr( void )
{
	Delay( 1000 );
	UartPrintf( "EINT0 is occurred.\n" );
	ClearPending( BIT_EINT0 );
}

static void __irq
Eint2Isr( void )
{
	Delay( 1000 );
	UartPrintf( "EINT2 is occrred.\n" );
	ClearPending( BIT_EINT2 );
}

static void __irq
Eint3Isr( void )
{
	Delay( 1000 );
	UartPrintf( "EINT3 is occrred.\n" );
	ClearPending( BIT_EINT3 );
}

static void __irq
Eint4Isr( void )
{
	Delay( 1000 );
	UartPrintf( "EINT4 is occurred.\n" );
	ClearPending( BIT_EINT4_7 );
	ClearExtPending( BIT_EXT_EINT4 );
}

void
EintInit( void )
{
	R_GPFCON = R_GPFCON & ~(3) | (1 << 1);
	R_GPFCON = R_GPFCON & ~(3 << 4) | (1 << 5);
	R_GPFCON = R_GPFCON & ~(3 << 6) | (1 << 7);
	R_GPFCON = R_GPFCON & ~(3 << 8) | (1 << 9);
	
	R_GPFUP |= (1 << 0);
	R_GPFUP |= (1 << 2);
	R_GPFUP |= (1 << 3);
	R_GPFUP |= (1 << 4);
	
	R_EXTINT0 = (R_EXTINT0 & ~(7 << 0)) | (2 << 0);
	R_EXTINT0 = (R_EXTINT0 & ~(7 << 8)) | (2 << 8);
	R_EXTINT0 = (R_EXTINT0 & ~(7 << 12)) | (2 << 12);
	R_EXTINT0 = (R_EXTINT0 & ~(7 << 16)) | (2 << 16);
	
	ISR_EINT0 = (U32)Eint0Isr;
	ISR_EINT2 = (U32)Eint2Isr;
	ISR_EINT3 = (U32)Eint3Isr;
	ISR_EINT4_7 = (U32)Eint4Isr;
}

void
EnableEint( void )
{
	R_EINTPEND = 0xffffff;
	R_SRCPND |= BIT_EINT0 | BIT_EINT2 | BIT_EINT3 | BIT_EINT4_7;
	R_INTPND |= BIT_EINT0 | BIT_EINT2 | BIT_EINT3 | BIT_EINT4_7;
	R_EINTPEND |= (0x1 << 4);
	
	R_EINTMASK = ~(0x1 << 4);
	R_INTMSK   = ~(BIT_EINT0 | BIT_EINT2 | BIT_EINT3 | BIT_EINT4_7);
}

int
Main( void )
{
#if LJ_CRITICAL_METHOD == 1
LJ_CPU_SR	cpuSr;
#endif

	UartInit( 0, 115200 );
	EintInit( );
	EnableEint( );
	LJ_ENTER_CRITICAL( );
	while ( 1 )
	{
		Delay( 500000000 );
		LJ_EXIT_CRITICAL( );
	}
	
	return 0;
}