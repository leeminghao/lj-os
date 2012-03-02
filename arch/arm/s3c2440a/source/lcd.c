/*
 *==============================================================================================
 *                                             LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, ������
 * All Rights Reserved
 *
 * �ļ����ƣ�lcd.c
 * �ļ���ʶ��c����Դ�ļ�
 * ժ    Ҫ�����ļ�ʵ��lcd����ʼ���Ϳ��ƺ���
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
 
 #include "lcd.h"
 #include <lj_rtos.h>
 
 #define MVAL		(13)
 #define MVAL_USED	(0)
 #define INVVDEN	(1)
 #define BSWP		(0)
 #define HWSWP		(1)
 
 #define M5D( n )	((n) & 0x1fffff)
 
 #define LCD_XSIZE_TFT_240320	(320)
 #define LCD_YSIZE_TFT_240320	(240)
 
 #define SCR_XSIZE_TFT_240320	(320)
 #define SCR_YSIZE_TFT_240320	(240)
 
 #define HOZVAL_TFT_240320		(LCD_XSIZE_TFT_240320 - 1)
 #define LINEVAL_TFT_240320		(LCD_YSIZE_TFT_240320 - 1)
 
 #define VBPD_240320			(3)							/* ��ֱͬ���źŵĺ��1              */
 #define VFPD_240320			(5)							/* ��ֱͬ���źŵ�ǰ��5              */
 #define VSPW_240320			(15)						/* ��ֱͬ���źŵ�����1              */
 
 #define HBPD_240320			(58)						/* ˮƽͬ���źŵĺ��35             */
 #define HFPD_240320			(15)						/* ˮƽͬ���źŵ�ǰ��19             */
 #define HSPW_240320			(8)							/* ˮƽͬ���źŵ�����5              */
 
 #define CLKVAL_TFT_240320		(4)							/* ȷ��VCLK������                   */
 
 extern U8 gImage_image[];
 extern U8 gImage_logo[];
 extern U8 __CHS[];
 extern U8 __VGA[];
 
 U16 gLcdBuffer[SCR_YSIZE_TFT_240320][SCR_XSIZE_TFT_240320];
 
/*
 *==============================================================================================
 * �������ƣ�LcdInit
 * ������������ʼ��LCD��
 * ��    ����void
 * �� �� ֵ��static void
 *==============================================================================================
 */
 static void
 LcdInit( void )
 {
 	R_GPCUP  = 0x00000000;
 	R_GPCCON = 0xaaaa02a9;
 	
 	R_GPDUP  = 0x00000000;
 	R_GPDCON = 0xaaaaaaaa;
 	
 	/* 
 	 * VCLK = 10MHz, VM���� = ÿ֡һ��, TFT, 16bpp, ��ֹLCD video��LCD�����ź�
 	 */
 	R_LCDCON1 = (CLKVAL_TFT_240320 << 8) | (MVAL_USED << 7) | (3 << 5) | (12 << 1) | 0;
 	/*
 	 * ÿ֡��ʼ��Ч����Ϊ3, LCD��ֱ�ߴ�Ϊ239, ÿ֡������Ч����Ϊ5, ��ֱͬ������Ϊ15
 	 */
 	R_LCDCON2 = (VBPD_240320 << 24) | (LINEVAL_TFT_240320 << 14) | (VFPD_240320 << 6) | (VSPW_240320);
 	/*
 	 * ��HSYNC�ź��½��غ���Ч���ݿ�ʼ֮���VCLK������ = 58
 	 * LCD��ʾ��ˮƽ�ߴ�Ϊ319
 	 * ��HSYNC�ź������غ���Ч���ݽ���֮���VCLK������ = 15
 	 */
 	R_LCDCON3 = (HBPD_240320 << 19) | (HOZVAL_TFT_240320 << 8) | (HFPD_240320);
 	/*
 	 * MVAL = 13, ����MMODEλ�����ó�1ʱ��ȷ��VM�źŵ�����
 	 * HSYNC������ = 8
 	 */
 	R_LCDCON4 = (MVAL << 8) | (HSPW_240320);
 	/*
 	 * 16bpp: 5 : 6 : 5��Ƶ���ݸ�ʽ, ��Ƶ������VCLK�ź������ض�ȡ, HSYNC���弫�Է���
 	 * VSYNC�����źż��Է���, VD��Ƶ�������弫������, VDEN���弫������, LCD_PWREN����ź�ʹ��
 	 * �ֽڽ�����ʹ��, ���ֽ���ʹ��
 	 */
 	R_LCDCON5 = (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8) | (0 << 7) | (0 << 6) |
 	            (1 <<  3) | (BSWP << 1) | (HWSWP);
 	R_LCDSADDR1  = (((U32)gLcdBuffer >> 22) << 21) | M5D( (U32)gLcdBuffer >> 1);
 	R_LCDSADDR2  = M5D( ((U32)gLcdBuffer + (SCR_XSIZE_TFT_240320 * LCD_YSIZE_TFT_240320 * 2)) >> 1 );
 	R_LCDSADDR3  = (((SCR_XSIZE_TFT_240320 - LCD_XSIZE_TFT_240320) / 1) << 11) | (LCD_XSIZE_TFT_240320 / 1);
 	R_LCDINTMSK |= (3);										/* LCD֡ͬ���ж�����, LCD��FIFO�ж��������� */
 	R_TCONSEL   &= ~7;
 	R_TPAL       =  0;
 }

/*
 *=================================================================================================
 * �������ƣ�LcdEnvidOnOff
 * ����������LCD�������ʹ��, 1������Ƶ���
 * ��    �Σ�onoff (U32)
 * �� �� ֵ��staitc void
 *=================================================================================================
 */
 static void
 LcdEnvidOnOff( U32 onoff )
 {
	if ( onoff == 1 )
	{
		R_LCDCON1 |= 1;
	}
	else
	{
		R_LCDCON1 = R_LCDCON1 & 0x3fffe;
	}
 }
 
/*
 *================================================================================================
 * �������ƣ�LcdPowerEnable
 * ����������LCD��Դ���ƿ�������ʹ��
 * ��    �Σ�invpwren (U32)��ȷ��PWREN�źż���
 *           pwren    (U32)��LCD_PWREN����ź�ʹ��
 * �� �� ֵ��static void
 *================================================================================================
 */
 static void
 LcdPowerEnable( U32 invpwren, U32 pwren )
 {
 	R_GPGUP  = R_GPGUP & (~(1 << 4)) | (1 << 4);			/* GPG4����������ʹ��                 */
 	R_GPGCON = R_GPGCON & (~(3 << 8)) | (3 << 8);			/* GPG4 = LCD_PWREN                   */
 	R_GPGDAT = R_GPGDAT | (1 << 4);
 	
 	R_LCDCON5 = R_LCDCON5 & (~(1 << 3)) | (pwren << 3);		/* PWREN                              */
 	R_LCDCON5 = R_LCDCON5 & (~(1 << 5)) | (invpwren << 5);	/* INVPWREN                           */
 }
 
/*
 *================================================================================================
 * �������ƣ�PutPixel
 * �����������������ص���ʾ�������
 * ��    �Σ�x (U32), y (U32), col (U16)
 * �� �� ֵ��void
 *================================================================================================
 */
 void
 PutPixel( U32 x, U32 y, U16 col )
 {
 	if ( (x < SCR_XSIZE_TFT_240320) && (y < SCR_YSIZE_TFT_240320) )
 	{
 		gLcdBuffer[y][x] = col;
 	}
 }
 
/*
 *================================================================================================
 * �������ƣ�GUIPaint
 * �����������������ص���ʾ�������
 * ��    �Σ�x (U32), y (U32), col (U16)
 * �� �� ֵ��void
 *================================================================================================
 */
 void
 GUIPoint( U32 x, U32 y, U16 col )
 {
 	if ( (x < SCR_XSIZE_TFT_240320) && (y < SCR_YSIZE_TFT_240320) )
 	{
 		gLcdBuffer[y][x] = col;
 	}
 }
 
/*
 *===============================================================================================
 * �������ƣ�LcdClearScr
 * ����������LCD��ȫ�������ɫ������
 * ��    �Σ�col (U16)
 * �� �� ֵ��void
 *===============================================================================================
 */
 void
 LcdClearScr( U16 col )
 {
 	U32 x, y;
 	
 	for ( y = 0; y < SCR_YSIZE_TFT_240320; y++ )
 	{
 		for ( x = 0; x < SCR_XSIZE_TFT_240320; x++ )
 		{
 			gLcdBuffer[y][x] = col;
 		}
 	}
 }
 
/*
 *===============================================================================================
 * �������ƣ�GlibLine
 * ������������TFT LCD���ϻ�һ��ֱ��
 * ��    �Σ�(x1 (S32), y1 (S32))�����, (x2 (S32), y2 (S32))���յ�, col (U16)����ɫ
 * �� �� ֵ��void
 *===============================================================================================
 */
 void
 GlibLine( S32 x1, S32 y1, S32 x2, S32 y2, U16 col )
 {
 	S32 dx, dy, e;
	
	dx = x2 - x1;
	dy = y2 - y1;
	
	if ( dx >= 0 )
	{
		if ( dy >= 0 )
		{
			if ( dx >= dy )			// 1/8 ocatant
			{
				e = dy - dx / 2;
				while ( x1 <= x2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						y1 += 1;
						e  -= dx;
					}
					x1 += 1;
					e  += dy;
				}
			}
			else	// 2/8 octant
			{
				e = dx - dy / 2;
				while ( y1 <= y2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						x1 += 1;
						e  -= dy;
					}
					y1 += 1;
					e  += dx;
				}
			}
		}
		else	// dy < 0
		{
			dy = -dy;
			
			if ( dx >= dy )		// 8/8 octant
			{
				e = dy - dx / 2;
				while ( x1 <= x2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						y1 -= 1;
						e  -= dx;
					}
					x1 += 1;
					e  += dy;
				}
			}
			else	// 7/8 octant
			{
				e = dx - dy / 2;
				while ( y1 >= y2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						x1 += 1;
						e  -= dy;
					}
					y1 -= 1;
					e  += dx;
				}
			}
		}
	}
	else	// dx < 0
	{
		dx = -dx;
		if ( dy >= 0 )
		{
			if ( dx >= dy )	// 4/8octant
			{
				e = dy - dx / 2;
				while ( x1 >= x2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						y1 += 1;
						e  -= dx;
					}
					x1 -= 1;
					e  += dy;
				}
			}
			else		// 3/8 octant
			{
				e = dx - dy / 2;
				while ( y1 <= y2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						x1 -= 1;
						e  -= dy;
					}
					y1 += 1;
					e  += dx;
				}
			}
		}
		else	// dy < 0
		{
			dy = -dy;
			
			if ( dx >= dy )
			{
				e = dy - dx / 2;
				while ( x1 >= x2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						y1 -= 1;
						e  -= dx;
					}
					x1 -= 1;
					e  += dy;
				}
			}
			else		// 6/8 octant
			{
				e = dx - dy / 2;
				while ( y1 >= y2 )
				{
					PutPixel( x1, y1, col );
					if ( e > 0 )
					{
						x1 -=  1;
						e  -= dy;
					}
					y1 -= 1;
					e  += dx;
				}
			}
		}
	}
 }
 
/*
 *=================================================================================================
 * �������ƣ�GlibRectangle
 * ������������LCD���ϻ�һ������
 * ��    �Σ�(x1 (S32), y1 (S32))�����, (x2 (S32), y2 (S32))���Գ��յ�, col (U16)����ɫ
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 GlibRectangle( S32 x1, S32 y1, S32 x2, S32 y2, U16 col )
 {
 	GlibLine( x1, y1, x1, y2, col );
 	GlibLine( x1, y2, x2, y2, col );
 	GlibLine( x2, y2, x2, y1, col );
 	GlibLine( x2, y1, x1, y1, col );
 }
 
/*
 *=================================================================================================
 * �������ƣ�GlibFilledRectangle
 * ����������ʹ���ض�����ɫ��LCD����������
 * ��    �Σ�(x1 (S32), y1 (S32))�����, (x2 (S32), y2 (S32))���Գ��յ�, col (U16)����ɫ
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 GlibFilledRectangle( S32 x1, S32 y1, S32 x2, S32 y2, U16 col )
 {
 	S32 i;
 	
 	for ( i = y1; i < y2; i++ )
 	{
 		GlibLine( x1, i, x2, i, col );
 	}
 }
 
/*
 *=================================================================================================
 * �������ƣ�PaintBmp
 * ������������LCD������ʾ�ض���С��ͼƬ
 * ��    �Σ�(x0 (S32), y0 (S32))����ʾͼƬ��λ��
 *           height (U32)��ͼƬ�߶�
 *           width  (U32)��ͼƬ���
 *           bmp   (U8 *)��ͼ������ָ��
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 PaintBmp( S32 x0, S32 y0, U32 length, U32 width, U8 *bmp )
 {
 	S32 x, y;
 	S32 p = 0;
 	U16 col;
 	
 	for ( y = y0; y < width; y++ )
 	{
 		for ( x = x0; x < length; x++ )
 		{
 			col = bmp[p + 1] | (bmp[p] << 8);
 			
 			if ( (x0 + x) < SCR_XSIZE_TFT_240320 && (y0 + y) < SCR_YSIZE_TFT_240320 )
 			{
 				gLcdBuffer[y0 + y][x0 + x] = col;
 			}
 			
 			p += 2;
 		}
 	}
 }
 
/*
 *=================================================================================================
 * �������ƣ�LcdPutASCII
 * ������������LCD��Ļ���ƶ������дASCII�ַ�
 * ��    �Σ�(x (U32), y (U32))������λ��
 *            ch           (U8)����Ҫ��ʾ���ַ�
 *            col         (U16)���ַ���ɫ
 *            bkCol       (U16)��������ɫ
 *            st          (U32)��ȷ���Ƿ���ʾ������ɫ, 1--Ĭ��
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 LcdPutASCII( U32 x, U32 y, U8 ch, U16 col, U16 bkCol, U32 st )
 {
 	U16  i, j;
 	U8  *pZK;
 	U8   mask, buf;
 	
 	pZK = &__VGA[ch * 16];
 	
 	for ( i = 0; i < 16; i++ )
 	{
 		mask = 0x80;
 		buf  = pZK[i];
 		
 		for ( j = 0; j < 8; j++ )
 		{
 			if ( buf & mask )
 			{
 				PutPixel( x + j, y + i, col );
 			}
 			else
 			{
 				if ( !st )
 				{
 					PutPixel( x + j, y + i, bkCol );
 				}
 			}
 			
 			mask = mask >> 1;
 		}
 	}
 }
 
/*
 *=================================================================================================
 * �������ƣ�LcdPutHZ
 * ������������LCD��Ļ���ƶ������д����
 * ��    �Σ�(x (U32), y (U32))������λ��
 *            QW          (U16)����Ҫ��ʾ�ĺ�����λ��
 *            col         (U16)���ַ���ɫ
 *            bkCol       (U16)��������ɫ
 *            st          (U32)��ȷ���Ƿ���ʾ������ɫ, 1--Ĭ��
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 LcdPutHZ( U32 x, U32 y, U16 QW, U16 col, U16 bkCol, U32 st )
 {
 	U16  i, j;
 	U8   mask, buf;
 	U8  *pZK;
 	
 	pZK = &__CHS[(((QW >> 8) - 1) * 94 + (QW & 0x00ff) - 1) * 32];
 	for ( i = 0; i < 16; i++ )
 	{
 		mask = 0x80;									/* ������ʾ���ֵ���벿��                 */
 		buf  = pZK[i * 2];
 		for ( j = 0; j < 8; j++ )
 		{
 			if ( buf &  mask )
 			{
 				PutPixel( x + j, y + i, col );
 			}
 			else
 			{
 				if ( !st )
 				{
 					PutPixel( x + j, y + i, bkCol );
 				}
 			}
 			
 			mask = mask >> 1;
 		}
 		
 		mask = 0x80;									/* �ٴ���ʾ���ֵ��Ұ벿��                */
 		buf = pZK[i * 2 + 1];
 		for ( j = 0; j < 8; j++ )
 		{
 			if ( buf & mask )
 			{
 				PutPixel( x + j + 8, y + i, col );
 			}
 			else
 			{
 				if ( !st )
 				{
 					PutPixel( x + j + 8, y + i, bkCol );
 				}
 			}
 			
 			mask = mask >> 1;
 		}
 	}
 }
 
/*
 *=================================================================================================
 * �������ƣ�LcdPrintf
 * ������������LCD������ʾ������ַ����ߺ���
 * ��    �Σ�(x (U32), y (U32))������λ��
 *            col         (U16)���ַ���ɫ
 *            bkCol       (U16)��������ɫ
 *            st          (U32)��ȷ���Ƿ���ʾ������ɫ, 1--Ĭ��
 *            fmt      (char *)����ʾ��ʽ
 * �� �� ֵ��void
 *=================================================================================================
 */
 void
 LcdPrintf( U32 x, U32 y, U16 col, U16 bkCol, U32 st, char *fmt, ... )
 {
 	char     buf[BUF_SIZE];
 	va_list  ap;
 	U8      *pStr = (U8 *)buf;
 	U32      i = 0;
 	
 	va_start( ap, fmt );
 	vsprintf( buf, fmt, ap );
 	va_end( ap );
 	
 	while ( *pStr != 0 )
 	{
 		switch ( *pStr )
 		{
 			case '\n': break;
 			default  : 
 				if ( *pStr > 0xA0 & *(pStr + 1) > 0xA0 )		/* �������                       */
 				{
 					LcdPutHZ( x, y, (*pStr - 0xA0) * 0x0100 + *(pStr + 1) - 0xA0, col, bkCol, st );
 					pStr++;
 					i++;
 					
 					x += 16;
 				}
 				else
 				{
 					LcdPutASCII( x, y, *pStr, col, bkCol, st );
 					
 					x += 8;
 				}
 				break;
 		}
 		
 		pStr++;
 		i++;
 		
 		if ( i > BUF_SIZE )
 		{
 			break;
 		}
 	}
 }
 
/*
 *=============================================================================================
 * �������ƣ�LcdN35Init
 * ������������Ե�ǰLCD�����г�ʼ������
 * ��    �Σ�void
 * �� �� ֵ��void
 *=============================================================================================
 */
 void
 LcdN35Init( void )
 {
 	LcdInit( );
 	
 	LcdPowerEnable( 0, 1 );
 	LcdEnvidOnOff( 1 );
 	
 	
	//LcdClearScr( COLOR( 0x00, 0x00, 0x1f ) );  
	PaintBmp(0, 0, 320, 240, gImage_image );
	LJTimeDly( LJ_TICKS_PER_SEC );
	PaintBmp(0, 0, 320, 240, gImage_logo );
	//LcdPrintf( 40, 100, COLOR(0xff, 0xff, 0xff), COLOR(0x00, 0x00, 0x00), 1, "HELLO, ������\n" );
	//GlibRectangle( 20, 20, 200, 200, COLOR( 0xff, 0xff, 0xff ) );
	//GlibFilledRectangle( 20, 20, 200, 200, COLOR( 0xff, 0xff, 0xff ) );
 }