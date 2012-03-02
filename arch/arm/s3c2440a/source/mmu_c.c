/*
 *=============================================================================================
 *                                             LJ_RTOS
 *                                        The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：mmu_c.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现MMU有关的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/15
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *============================================================================================
 */
 
 #include "mmu.h"
 
 extern char __ENTRY[];
 
/*
 *============================================================================================
 * 函数名称：MMUInit
 * 功能描述：MMU初始化函数
 * 传    参：void
 * 返 回 值：void
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
 * 函数名称：ChangeRomCacheStatus
 * 功能描述：改变Rom Cache的属性
 * 传    参：attr (U32)：属性( RW_CB, RW_CNB, RW_NCNB, RW_FAULT )
 * 返 回 值：void
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
 * 函数名称：MMUSetMTT
 * 功能描述：完成虚拟存储空间到屋里存储空间的映射
 * 传    参：vaddrStart (U32)：虚拟地址起始地址
 *           vaddrEnd   (U32)：虚拟地址结束地址
 *           paddrStart (U32)：物理地址起始地址
 *           attr       (U32)：属性
 * 返 回 值：void
 *==============================================================================================
 */
 void
 MMUSetMTT( U32 vaddrStart, U32 vaddrEnd, U32 paddrStart, U32 attr )
 {
 	volatile U32 *pTT;
 	volatile int  i, nSec;
 	
 	pTT  = (U32 *)MMUTT_STARTADDRESS + (vaddrStart >> 20);		/* 获取一级描述符地址             */
 	nSec = (vaddrEnd >> 20) - (vaddrStart >> 20);               /* 一共要进行几次基于段的地址变换 */
 	
 	for ( i = 0; i <= nSec; i++ )
 	{
 		*pTT++ = attr | (((paddrStart >> 20) + i) << 20);		/* 将物理地址存入一级描述符地址中 */
 	}
 }