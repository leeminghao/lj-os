/*
 *======================================================================================================
 *                                             LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lj_mem.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�����ʵ��LJ_RTOS�ڴ����
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/24
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
 *======================================================================================================
 */
 
 #include <lj_rtos.h>
 
/*
 *======================================================================================================
 * �������ƣ�LJ_MemInit
 * ������������ʼ�������ڴ���ƿ�����
 * ��    �Σ�void
 * �� �� ֵ��void
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
 * �������ƣ�LJMemCreate
 * ��������������һ���ڴ����
 * 
 * ��    �Σ�addr    (void *)���ڴ��������ʼ��ַ
 *           nblks   (INT32U)���ڴ�����е��ڴ����
 *           blkSize (INT32U)��ÿ���ڴ��Ĵ�С
 *           err    (INT8U *)��ָ���������ָ��
 *                             LJ_NO_ERR          ��������ú����ɹ�
 *                             LJ_MEM_INVALID_ADDR�����ָ����һ���Ƿ��ڴ������ַ
 *                             LJ_MEM_INVALID_PART�����û�п��з���������
 *                             LJ_MEM_INVALID_BLKS�����ָ���˷Ƿ��ڴ����( >= 2 )
 *                             LJ_MEM_INVALID_SIZE�����ָ���˷Ƿ����ڴ���С(�������ָ��Ĵ�С)
 *
 * �� �� ֵ��!= (LJ_MEM *)0  ���ڴ����������
 *           == (LJ_MEM *)0  ������ڴ����û�д����ɹ�
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
 	if ( addr == (void *)0 )					/* ������ڴ������ַ��������Ч��                      */
 	{
 		*err = LJ_MEM_INVALID_ADDR;
 		return ((LJ_MEM *)0);
 	}
 	if ( nblks < 2 )							/* ÿ���ڴ�������뺬��2���ڴ��                       */
 	{
 		*err = LJ_MEM_INVALID_BLKS;
 		return ((LJ_MEM *)0);
 	}
 	if ( blkSize < sizeof (void *) )			/* ÿ���ڴ���������ݵ���һ��ָ�룬��Ϊͬһ�����е�����*/
 	{											/* �����ڴ������ָ��������������                      */
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
 	plink = (void **)addr;						/* �����е��ڴ�����ӳ�һ����������                    */
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
 * �������ƣ�LJMemGet
 * �������������ڴ�����з���һ���ڴ��
 * 
 * ��    �Σ�pmem (LJ_MEM *)��ָ���ڴ������ָ��
 *           err   (INT8U *)��ָ���������ָ��
 *                            LJ_NO_ERR          ������ú������óɹ�
 *                            LJ_MEM_NO_FREE_BLKS�����û�ж���Ŀ����ڴ�鱻����
 *                            LJ_MEM_INVALID_PMEM�����'pmem'��һ��NULLָ��
 *
 * �� �� ֵ��!= (LJ_MEM *)0 �����û�д������
 *           == (LJ_MEM *)0 ������д������
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
 	if ( pmem == (LJ_MEM *)0 )							/* ȷ��'pmem'����NULLָ��                       */
 	{
 		*err = LJ_MEM_INVALID_PMEM;
 		return ((LJ_MEM *)0);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pmem->ljMemNFree > 0 )							/* �鿴�Ƿ��п�����ڴ��                       */
 	{
 		pblk				= pmem->ljMemFreeList;		/* ����п�����ڴ�飬�ʹӿ����ڴ��������ɾ�� */
 		pmem->ljMemFreeList = *(void **)pblk;			/* ��һ���ڴ��                                 */
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
 * �������ƣ�LJMemPut
 * �����������ͷ�һ���ڴ��
 *
 * ��    �Σ�pmem (LJ_MEM *)    ��ָ���ڴ������ָ��
 *           pblk   (void *)    ��ָ���ڴ���ָ��
 *
 * �� �� ֵ��LJ_NO_ERR          ������������óɹ�
 *           LJ_MEM_FULL        ������ͷŵ��ڴ����ڷ�����ڴ����
 *           LJ_MEM_INVALID_PMEM�����'pmem'��NULLָ��
 *           LJ_MEM_INVALID_PBLK�����'pblk'��NULLָ��
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
 	if ( pmem == (LJ_MEM *)0 )							/* ȷ��'pmem'����NULLָ��                      */
 	{
 		return (LJ_MEM_INVALID_PMEM);
 	}
 	if ( pblk == (void *)0 )							/* ȷ��'pblk'����NULLָ��                      */
 	{
 		return (LJ_MEM_INVALID_PBLK);
 	}
 #endif
 	LJ_ENTER_CRITICAL( );
 	if ( pmem->ljMemNFree >= pmem->ljMemNBlks )			/* ����ڴ�����Ƿ�����                        */
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
 * �������ƣ�LJMemQuery
 * ������������ѯһ���ڴ������״̬
 * 
 * ��    �Σ�pmem       (LJ_MEM *)��һ��ָ���ڴ������ָ��
 *           pdata (LJ_MEM_DATA *)��һ��ָ���ڴ�������ݽṹ������ָ��
 *
 * �� �� ֵ��LJ_NO_ERR            ������������óɹ�
 *           LJ_MEM_INVALID_PMEM  �����'pmem'��NULLָ��
 *           LJ_MEM_INVALID_PDATA �����'pdata'��NULLָ��
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