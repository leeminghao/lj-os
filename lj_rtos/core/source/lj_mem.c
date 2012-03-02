/*
 *======================================================================================================
 *                                             LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lj_mem.c
 * 文件标识：c语言源文件
 * 摘    要：本文件用于实现LJ_RTOS内存管理
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/24
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *======================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *======================================================================================================
 * 函数名称：LJ_MemInit
 * 功能描述：初始化空余内存控制块链表
 * 传    参：void
 * 返 回 值：void
 *======================================================================================================
 */
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 void
 LJ_MemInit( void )
 {
 #if LJ_MAX_MEM_PART == 1
 	gLJMemFreeList					= (LJ_MEM *)&gLJMemTbl[0];
 	gLJMemFreeList->ljMemFreeList	= (void *)0;
 	gLJMemFreeList->ljMemAddr		= (void *)0;
 	gLJMemFreeList->ljMemNFree		= 0;
 	gLJMemFreeList->ljMemNBlks		= 0;
 	gLJMemFreeList->ljMemBlkSize	= 0;
 #endif
 
 #if LJ_MAX_MEM_PART >= 2
 	LJ_MEM	*pmem;
 	INT16U	 i;
 	
 	pmem = (LJ_MEM *)&gLJMemTbl[0];
 	for ( i = 0; i < (LJ_MAX_MEM_PART - 1); i++ )
 	{
 		pmem->ljMemFreeList = (void *)&gLJMemTbl[i + 1];
 		pmem->ljMemAddr		= (void *)0;
 		pmem->ljMemNFree    = 0;
 		pmem->ljMemNBlks    = 0;
 		pmem->ljMemBlkSize  = 0;
 		pmem++;
 	}
 	pmem->ljMemFreeList		= (void *)0;
 	pmem->ljMemAddr         = (void *)0;
 	pmem->ljMemNFree        = 0;
 	pmem->ljMemNBlks        = 0;
 	pmem->ljMemBlkSize      = 0;
 	
 	gLJMemFreeList			= (LJ_MEM *)&gLJMemTbl[0];
 #endif
 }
 
 #endif
 
/*
 *======================================================================================================
 * 函数名称：LJMemCreate
 * 功能描述：建立一个内存分区
 * 
 * 传    参：addr    (void *)：内存分区的起始地址
 *           nblks   (INT32U)：内存分区中的内存块数
 *           blkSize (INT32U)：每个内存块的大小
 *           err    (INT8U *)：指向错误代码的指针
 *                             LJ_NO_ERR          ：如果调用函数成功
 *                             LJ_MEM_INVALID_ADDR：如果指定了一个非法内存分区地址
 *                             LJ_MEM_INVALID_PART：如果没有空闲分区可利用
 *                             LJ_MEM_INVALID_BLKS：如果指定了非法内存块数( >= 2 )
 *                             LJ_MEM_INVALID_SIZE：如果指定了非法的内存块大小(必须大于指针的大小)
 *
 * 返 回 值：!= (LJ_MEM *)0  ：内存分区被创建
 *           == (LJ_MEM *)0  ：如果内存分区没有创建成功
 *======================================================================================================
 */
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 LJ_MEM *
 LJMemCreate( void *addr, INT32U nblks, INT32U blkSize, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	LJ_MEM		*pmem;
 	INT8U		*pblk;
 	void	   **plink;
 	INT32U		 i;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( addr == (void *)0 )					/* 定义的内存分区地址必须是有效的                      */
 	{
 		*err = LJ_MEM_INVALID_ADDR;
 		return ((LJ_MEM *)0);
 	}
 	if ( nblks < 2 )							/* 每个内存分区必须含有2个内存块                       */
 	{
 		*err = LJ_MEM_INVALID_BLKS;
 		return ((LJ_MEM *)0);
 	}
 	if ( blkSize < sizeof (void *) )			/* 每个内存块至少需容得下一个指针，因为同一分区中的所有*/
 	{											/* 空余内存块是由指针链串联起来的                      */
 		*err = LJ_MEM_INVALID_SIZE;
 		return ((LJ_MEM *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pmem = gLJMemFreeList;
 	if ( gLJMemFreeList != (LJ_MEM *)0 )
 	{
 		gLJMemFreeList = (LJ_MEM *)gLJMemFreeList->ljMemFreeList;
 	}
 	LJ_EXIT_CRITICAL( );
 	if ( pmem == (LJ_MEM *)0 )
 	{
 		*err = LJ_MEM_INVALID_PART;
 		return ((LJ_MEM *)0);
 	}
 	plink = (void **)addr;						/* 将所有的内存块链接成一个单向链表                    */
 	pblk  = (INT8U *)addr + blkSize;
 	for ( i = 0; i < (nblks - 1); i++ )
 	{
 		*plink = (void *)pblk;
 		 plink = (void **)pblk;
 		 pblk  = pblk + blkSize;
 	}
 	*plink				 = (void *)0;
 	 pmem->ljMemAddr	 = addr;
 	 pmem->ljMemFreeList = addr;
 	 pmem->ljMemNFree    = nblks;
 	 pmem->ljMemNBlks    = blkSize;
 	 *err                = LJ_NO_ERR;
 	 return (pmem);
 }
 
 #endif
 
/*
 *======================================================================================================
 * 函数名称：LJMemGet
 * 功能描述：从内存分区中分配一个内存块
 * 
 * 传    参：pmem (LJ_MEM *)：指向内存分区的指针
 *           err   (INT8U *)：指向错误代码的指针
 *                            LJ_NO_ERR          ：如果该函数调用成功
 *                            LJ_MEM_NO_FREE_BLKS：如果没有多余的空闲内存块被分配
 *                            LJ_MEM_INVALID_PMEM：如果'pmem'是一个NULL指针
 *
 * 返 回 值：!= (LJ_MEM *)0 ：如果没有错误产生
 *           == (LJ_MEM *)0 ：如果有错误产生
 *=====================================================================================================
 */
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 void *
 LJMemGet( LJ_MEM *pmem, INT8U *err )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 	void		*pblk;
 	
 #if LJ_ARG_CHK_EN > 0
 	if ( pmem == (LJ_MEM *)0 )							/* 确保'pmem'不是NULL指针                       */
 	{
 		*err = LJ_MEM_INVALID_PMEM;
 		return ((LJ_MEM *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pmem->ljMemNFree > 0 )							/* 查看是否有空余的内存块                       */
 	{
 		pblk				= pmem->ljMemFreeList;		/* 如果有空余的内存块，就从空余内存块链表中删除 */
 		pmem->ljMemFreeList = *(void **)pblk;			/* 第一个内存块                                 */
 		pmem->ljMemNFree--;
 		LJ_EXIT_CRITICAL( );
 		*err = LJ_NO_ERR;
 		return (pblk);
 	}
 	LJ_EXIT_CRITICAL( );
 	*err = LJ_MEM_NO_FREE_BLKS;
 	return ((void *)0);
 }
 
 #endif
 
/*
 *======================================================================================================
 * 函数名称：LJMemPut
 * 功能描述：释放一个内存块
 *
 * 传    参：pmem (LJ_MEM *)    ：指向内存分区的指针
 *           pblk   (void *)    ：指向内存块的指针
 *
 * 返 回 值：LJ_NO_ERR          ：如果函数调用成功
 *           LJ_MEM_FULL        ：如果释放的内存块多于分配的内存块数
 *           LJ_MEM_INVALID_PMEM：如果'pmem'是NULL指针
 *           LJ_MEM_INVALID_PBLK：如果'pblk'是NULL指针
 *======================================================================================================
 */
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0
 
 INT8U
 LJMemPut( LJ_MEM *pmem, void *pblk )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 #if LJ_ARG_CHK_EN > 0
 	if ( pmem == (LJ_MEM *)0 )							/* 确保'pmem'不是NULL指针                      */
 	{
 		return (LJ_MEM_INVALID_PMEM);
 	}
 	if ( pblk == (void *)0 )							/* 确保'pblk'不是NULL指针                      */
 	{
 		return (LJ_MEM_INVALID_PBLK);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pmem->ljMemNFree >= pmem->ljMemNBlks )			/* 检查内存分区是否已满                        */
 	{
 		LJ_EXIT_CRITICAL( );
 		return (LJ_MEM_FULL);
 	}
 	*(void **)pblk		= pmem->ljMemFreeList;
 	pmem->ljMemFreeList	= pblk;
 	pmem->ljMemNFree++;
 	LJ_EXIT_CRITICAL( );
 	
 	return (LJ_NO_ERR);
 }
 
 #endif
 
/*
 *======================================================================================================
 * 函数名称：LJMemQuery
 * 功能描述：查询一个内存分区的状态
 * 
 * 传    参：pmem       (LJ_MEM *)：一个指向内存分区的指针
 *           pdata (LJ_MEM_DATA *)：一个指向内存分区数据结构的数据指针
 *
 * 返 回 值：LJ_NO_ERR            ：如果函数调用成功
 *           LJ_MEM_INVALID_PMEM  ：如果'pmem'是NULL指针
 *           LJ_MEM_INVALID_PDATA ：如果'pdata'是NULL指针
 *======================================================================================================
 */
 #if LJ_MEM_EN > 0 && LJ_MAX_MEM_PART > 0 && LJ_MEM_QUERY_EN > 0
 
 INT8U
 LJMemQuery( LJ_MEM *pmem, LJ_MEM_DATA *pdata )
 {
 #if LJ_CRITICAL_METHOD == 1
 	LJ_CPU_SR	 cpuSR;
 #endif
 
 #if LJ_ARG_CHK_EN > 0
 	if ( pmem == (LJ_MEM *)0 )
 	{
 		return (LJ_MEM_INVALID_PMEM);
 	}
 	if ( pdata == (LJ_MEM_DATA *)0 )
 	{
 		return (LJ_MEM_INVALID_PDATA);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	pdata->ljAddr		= pmem->ljMemAddr;
 	pdata->ljFreeList	= pmem->ljMemFreeList;
 	pdata->ljBlkSize	= pmem->ljMemBlkSize;
 	pdata->ljNBlks		= pmem->ljMemNBlks;
 	pdata->ljNFree		= pmem->ljMemNFree;
 	LJ_EXIT_CRITICAL( );
 	pdata->ljNUsed		= pmem->ljMemNBlks - pmem->ljMemNFree;
 	return (LJ_NO_ERR);
 }
 
 #endif