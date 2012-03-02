/*
 *==============================================================================================
 *                                               LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 * 
 * 文件名称：lcd.h
 * 文件标识：c语言头文件
 * 摘    要：本文件用于声明初始化和控制LCD屏的函数
 *
 * 当前版本：0.0
 * 作    者：李明浩
 * 完成日期：2010/11/15
 *
 * 取代版本：
 * 作    者：
 * 完成日期：
 *==============================================================================================
 */
 
 #ifndef __LJ_LCD_H
 #define __LJ_LCD_H
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
 #include "define.h"
 #include "regaddr.h"
 #include "option.h"
 #include <stdio.h>
 #include <stdarg.h>
 
 #define BUF_SIZE					256
 #define COLOR( red, green, blue )	((red << 11) | (green << 5) | (blue))
 
 void LcdN35Init( void );
 void LcdClearScr( U16 c );
 void PutPixel( U32 x, U32 y, U16 col );
 void GUIPoint( U32 x, U32 y, U16 col );
 void GlibRectangle( S32 x1, S32 y1, S32 x2, S32 y2, U16 col );
 void GlibFilledRectangle( S32 x1, S32 y1, S32 x2, S32 y2, U16 col );
 void GlibLine( S32 x1, S32 y1, S32 x2, S32 y2, U16 col );
 void PaintBmp( S32 x0, S32 y0, U32 length, U32 width, U8 *bmp );
 
 void LcdPutASCII( U32 x, U32 y, U8 ch, U16 col, U16 bkCol, U32 st );
 void LcdPutHZ( U32 x, U32 y, U16 QW, U16 col, U16 bkCol, U32 st );
 void LcdPrintf( U32 x, U32 y, U16 col, U16 bkCol, U32 st, char *fmt, ... );
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* __LJ_LCD_H */