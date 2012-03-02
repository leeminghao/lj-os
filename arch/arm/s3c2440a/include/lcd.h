/*
 *==============================================================================================
 *                                               LJ_RTOS
 *                                         The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 * 
 * �ļ����ƣ�lcd.h
 * �ļ���ʶ��c����ͷ�ļ�
 * ժ    Ҫ�����ļ�����������ʼ���Ϳ���LCD���ĺ���
 *
 * ��ǰ�汾��0.0
 * ��    �ߣ�������
 * ������ڣ�2010/11/15
 *
 * ȡ���汾��
 * ��    �ߣ�
 * ������ڣ�
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