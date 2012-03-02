/*
 *=============================================================================================
 *                                             LJ_RTOS
 *                                        The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�mmu_c.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��MMU�йصĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/15
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *============================================================================================
 */
 
 #include "mmu.h"
 
 extern char __ENTRY[];
 
/*
 *============================================================================================
 * �������ƣ�MMUInit
 * ����������MMU��ʼ������
 * ��    �Σ�void
 * �� �� ֵ��void
 *============================================================================================
 */
 void
 MMUInit( void )
 {
 	int i;
 	int j;
 	
 	MMUDisableDCache( );
 	MMUDisableICache( );
 	
 	for ( i = 0; i < 64; i++ )
 	{
 		for ( j = 0; j < 8; j++ )
 		{
 			MMUCleanInvalidateDCacheIndex( (i << 26) | (j << 5) );
 		}
 	}
 	MMUInvalidateICache( );
 	
 	MMUDisableMMU( );
 	MMUInvalidateTLB( );
 	
 	MMUSetMTT( 0x00000000, 0x03f00000, (U32)__ENTRY, RW_CB );
 	MMUSetMTT( 0x04000000, 0x07f00000, 0, RW_NCNB );
 	MMUSetMTT( 0x08000000, 0x0ff00000, 0x08000000, RW_CNB );
    MMUSetMTT( 0x10000000, 0x17f00000, 0x10000000, RW_NCNB );
    MMUSetMTT( 0x18000000, 0x1ff00000, 0x18000000, RW_NCNB );
    MMUSetMTT( 0x20000000, 0x27f00000, 0x20000000, RW_CNB );
    MMUSetMTT( 0x28000000, 0x2ff00000, 0x28000000, RW_NCNB );
    MMUSetMTT( 0x30000000, 0x30100000, 0x30000000, RW_NCNB );
    MMUSetMTT( 0x30200000, 0x33e00000, 0x30200000, RW_NCNB );
    MMUSetMTT( 0x33f00000, 0x33f00000, 0x33f00000, RW_NCNB );
    MMUSetMTT( 0x38000000, 0x3ff00000, 0x38000000, RW_NCNB ); 
    MMUSetMTT( 0x40000000, 0x47f00000, 0x40000000, RW_NCNB );
    MMUSetMTT( 0x48000000, 0x5af00000, 0x48000000, RW_NCNB ); 
    MMUSetMTT( 0x5b000000, 0x5b000000, 0x5b000000, RW_NCNB );
    MMUSetMTT( 0x5b100000, 0xfff00000, 0x5b100000, RW_FAULT );

    MMUSetTTBase( MMUTT_STARTADDRESS );
    MMUSetDomain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR); 
   
    MMUSetProcessId(0x0);
    MMUEnableAlignFault();
    	
    MMUEnableMMU();
    MMUEnableICache();
    MMUEnableDCache();
 }
 
/*
 *==============================================================================================
 * �������ƣ�ChangeRomCacheStatus
 * �����������ı�Rom Cache������
 * ��    �Σ�attr (U32)������( RW_CB, RW_CNB, RW_NCNB, RW_FAULT )
 * �� �� ֵ��void
 *==============================================================================================
 */
 void 
 ChangeRomCacheStatus( U32 attr )
 {
    int i,j;
    
    MMUDisableDCache();
    MMUDisableICache();
    
    for(i=0;i<64;i++)
    {
    	for(j=0;j<8;j++)
    	{
    	    MMUCleanInvalidateDCacheIndex((i<<26)|(j<<5));
    	}
    }
    	    
    MMUInvalidateICache( );
    MMUDisableMMU( );
    MMUInvalidateTLB( );
    
    MMUSetMTT( 0x00000000, 0x07f00000, 0x00000000, attr );	//bank0
    MMUSetMTT( 0x08000000, 0x0ff00000, 0x08000000, attr );	//bank1
    
    MMUEnableMMU( );
    MMUEnableICache( );
    MMUEnableDCache( );
 }
 
/*
 *==============================================================================================
 * �������ƣ�MMUSetMTT
 * �����������������洢�ռ䵽����洢�ռ��ӳ��
 * ��    �Σ�vaddrStart (U32)�������ַ��ʼ��ַ
 *           vaddrEnd   (U32)�������ַ������ַ
 *           paddrStart (U32)�������ַ��ʼ��ַ
 *           attr       (U32)������
 * �� �� ֵ��void
 *==============================================================================================
 */
 void
 MMUSetMTT( U32 vaddrStart, U32 vaddrEnd, U32 paddrStart, U32 attr )
 {
 	volatile U32 *pTT;
 	volatile int  i, nSec;
 	
 	pTT  = (U32 *)MMUTT_STARTADDRESS + (vaddrStart >> 20);		/* ��ȡһ����������ַ             */
 	nSec = (vaddrEnd >> 20) - (vaddrStart >> 20);               /* һ��Ҫ���м��λ��ڶεĵ�ַ�任 */
 	
 	for ( i = 0; i <= nSec; i++ )
 	{
 		*pTT++ = attr | (((paddrStart >> 20) + i) << 20);		/* �������ַ����һ����������ַ�� */
 	}
 }