/*
 *==============================================================================================
 *                                              LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：mmu.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明与MMU有关的控制函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/14
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==============================================================================================
 */
 
 #ifndef __MMU_H
 #define __MMU_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 #include "option.h"
 
 #define DESC_SEC			(0x2 | 1 << 4)
 #define CACHE_BACK			(3 << 2)
 #define CACHE_NOT_BACK 	(2 << 2)
 #define NOT_CACHE_BACK		(1 << 2)
 #define NOT_CACHE_NOT_BACK	(0 << 2)
 #define AP_RW				(3 << 10)
 #define AP_RO				(2 << 10)
 
 #define DOMAIN_FAULT		(0x0)
 #define DOMAIN_CHK			(0x1)
 #define DOMAIN_NOTCHK		(0x3)
 #define DOMAIN0			(0x0 << 5)
 #define DOMAIN1			(0x1 << 5)
 
 #define DOMAIN0_ATTR		(DOMAIN_CHK << 0)
 #define DOMAIN1_ATTR		(DOMAIN_FAULT << 2)
 
 #define RW_CB				(AP_RW | DOMAIN0 | CACHE_BACK         | DESC_SEC)
 #define RW_CNB				(AP_RW | DOMAIN0 | CACHE_NOT_BACK     | DESC_SEC)
 #define RW_NCNB			(AP_RW | DOMAIN0 | NOT_CACHE_NOT_BACK | DESC_SEC)
 #define RW_FAULT			(AP_RW | DOMAIN1 | NOT_CACHE_NOT_BACK | DESC_SEC)
 
 void MMUInit( void );
 void MMUSetMTT( U32 vaddrStart, U32 vaddrEnd, U32 paddrStart, U32 attr );
 void ChangeRomCacheStatus( U32 attr );
 
/*
 *=================================================================================================
 * 以下声明函数完全由汇编语言实现
 *=================================================================================================
 */
 U32  SetIF( void );
 void WrIF( U32 cpsrValue );
 void ClrIF( void );
 
 void MMUEnableICache( void );
 void MMUDisableICache( void );
 void MMUEnableDCache( void );
 void MMUDisableDCache( void );
 void MMUEnableAlignFault( void );
 void MMUDisableAlignFault( void );
 void MMUEnableMMU( void );
 void MMUDisableMMU( void );
 void MMUSetTTBase( U32 base );
 void MMUSetDomain( U32 domain );
 
 void MMUSetFastBusMode( void );
 void MMUSetAsyncBusMode( void );
 
 void MMUInvalidateIDCache( void );
 void MMUInvalidateICache( void );
 void MMUInvalidateICacheMVA( U32 mva );
 void MMUPrefetchICacheMVA( U32 mva );
 void MMUInvalidateDCache( void );
 void MMUInvalidateDCacheMVA( U32 mva );
 void MMUCleanDCacheMVA( U32 mva );
 void MMUCleanInvalidateDCacheMVA( U32 mva );
 void MMUCleanDCacheIndex( U32 index );
 void MMUCleanInvalidateDCacheIndex( U32 index );
 void MMUWaitForInterrupt( void );
 
 void MMUInvalidateTLB( void );
 void MMUInvalidateITLB( void );
 void MMUInvalidateITLBMVA( U32 mva );
 void MMUInvalidateDTLB( void );
 void MMUInvalidateDTLBMVA( U32 mva );
 
 void MMUSetDCacheLockdownBase( U32 base );
 void MMUSetICacheLockdownBase( U32 base );
 
 void MMUSetDTLBLockdown( U32 baseVictim );
 void MMUSetITLBLockdown( U32 baseVictim );
 
 void MMUSetProcessId( U32 pid );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __MMU_H */