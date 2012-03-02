/*
 *==========================================================================================
 * 										LJ_RTOS
 *								  The Real-Time Kernel
 *
 * (c) Copyrights 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：define.h
 * 文件标识：c语言头文件
 * 摘    要：本文件进行一些常量的声明和类型的定义
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/11
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *=========================================================================================
 */
 
 #ifndef __DEFINE_H
 #define __DEFINE_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 typedef unsigned int	U32;
 typedef signed   int	S32;
 typedef unsigned short	U16;
 typedef signed   short	S16;
 typedef unsigned char	U8;
 typedef signed   char	S8;
 
 #define TRUE			1
 #define FALSE			0
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __DEFINE_H */