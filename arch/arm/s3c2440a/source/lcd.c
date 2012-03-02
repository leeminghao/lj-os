/*
 *==============================================================================================
 *                                             LJ_RTOS
 *                                       The Real-Time Kernel
 *
 * (c) Copyright 2010, 李明浩
 * All Rights Reserved
 *
 * 文件名称：lcd.c
 * 文件标识：c语言源文件
 * 摘    要：本文件实现lcd屏初始化和控制函数
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
 
 #define VBPD_240320			(3)							/* 垂直同步信号的后肩1              */
 #define VFPD_240320			(5)							/* 垂直同步信号的前肩5              */
 #define VSPW_240320			(15)						/* 垂直同步信号的脉宽1              */
 
 #define HBPD_240320			(58)						/* 水平同步信号的后肩35             */
 #define HFPD_240320			(15)						/* 水平同步信号的前肩19             */
 #define HSPW_240320			(8)							/* 水平同步信号的脉宽5              */
 
 #define CLKVAL_TFT_240320		(4)							/* 确定VCLK的速率                   */
 
 extern U8 gImage_image[];
 extern U8 gImage_logo[];
 extern U8 __CHS[];
 extern U8 __VGA[];
 
 U16 gLcdBuffer[SCR_YSIZE_TFT_240320][SCR_XSIZE_TFT_240320];
 
/*
 *==============================================================================================
 * 函数名称：LcdInit
 * 功能描述：初始化LCD屏
 * 传    数：void
 * 返 回 值：static void
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
 	 * VCLK = 10MHz, VM速率 = 每帧一次, TFT, 16bpp, 禁止LCD video和LCD控制信号
 	 */
 	R_LCDCON1 = (CLKVAL_TFT_240320 << 8) | (MVAL_USED << 7) | (3 << 5) | (12 << 1) | 0;
 	/*
 	 * 每帧开始无效行数为3, LCD垂直尺寸为239, 每帧结束无效行数为5, 垂直同步脉宽为15
 	 */
 	R_LCDCON2 = (VBPD_240320 << 24) | (LINEVAL_TFT_240320 << 14) | (VFPD_240320 << 6) | (VSPW_240320);
 	/*
 	 * 在HSYNC信号下降沿和有效数据开始之间的VCLK脉冲数 = 58
 	 * LCD显示器水平尺寸为319
 	 * 在HSYNC信号上升沿和有效数据结束之间的VCLK脉冲数 = 15
 	 */
 	R_LCDCON3 = (HBPD_240320 << 19) | (HOZVAL_TFT_240320 << 8) | (HFPD_240320);
 	/*
 	 * MVAL = 13, 其在MMODE位被设置成1时，确定VM信号的速率
 	 * HSYNC脉冲宽度 = 8
 	 */
 	R_LCDCON4 = (MVAL << 8) | (HSPW_240320);
 	/*
 	 * 16bpp: 5 : 6 : 5视频数据格式, 视频数据在VCLK信号上升沿读取, HSYNC脉冲极性反向
 	 * VSYNC脉冲信号极性反向, VD视频数据脉冲极性正常, VDEN脉冲极性正常, LCD_PWREN输出信号使能
 	 * 字节交换不使能, 半字交换使能
 	 */
 	R_LCDCON5 = (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8) | (0 << 7) | (0 << 6) |
 	            (1 <<  3) | (BSWP << 1) | (HWSWP);
 	R_LCDSADDR1  = (((U32)gLcdBuffer >> 22) << 21) | M5D( (U32)gLcdBuffer >> 1);
 	R_LCDSADDR2  = M5D( ((U32)gLcdBuffer + (SCR_XSIZE_TFT_240320 * LCD_YSIZE_TFT_240320 * 2)) >> 1 );
 	R_LCDSADDR3  = (((SCR_XSIZE_TFT_240320 - LCD_XSIZE_TFT_240320) / 1) << 11) | (LCD_XSIZE_TFT_240320 / 1);
 	R_LCDINTMSK |= (3);										/* LCD帧同步中断屏蔽, LCD的FIFO中断请求被屏蔽 */
 	R_TCONSEL   &= ~7;
 	R_TPAL       =  0;
 }

/*
 *=================================================================================================
 * 函数名称：LcdEnvidOnOff
 * 功能描述：LCD视屏输出使能, 1开启视频输出
 * 传    参：onoff (U32)
 * 返 回 值：staitc void
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
 * 函数名称：LcdPowerEnable
 * 功能描述：LCD电源控制控制引脚使能
 * 传    参：invpwren (U32)：确定PWREN信号极性
 *           pwren    (U32)：LCD_PWREN输出信号使能
 * 返 回 值：static void
 *================================================================================================
 */
 static void
 LcdPowerEnable( U32 invpwren, U32 pwren )
 {
 	R_GPGUP  = R_GPGUP & (~(1 << 4)) | (1 << 4);			/* GPG4引脚上拉不使能                 */
 	R_GPGCON = R_GPGCON & (~(3 << 8)) | (3 << 8);			/* GPG4 = LCD_PWREN                   */
 	R_GPGDAT = R_GPGDAT | (1 << 4);
 	
 	R_LCDCON5 = R_LCDCON5 & (~(1 << 3)) | (pwren << 3);		/* PWREN                              */
 	R_LCDCON5 = R_LCDCON5 & (~(1 << 5)) | (invpwren << 5);	/* INVPWREN                           */
 }
 
/*
 *================================================================================================
 * 函数名称：PutPixel
 * 功能描述：单个像素的显示数据输出
 * 传    参：x (U32), y (U32), col (U16)
 * 返 回 值：void
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
 * 函数名称：GUIPaint
 * 功能描述：单个像素的显示数据输出
 * 传    参：x (U32), y (U32), col (U16)
 * 返 回 值：void
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
 * 函数名称：LcdClearScr
 * 功能描述：LCD屏全屏填充颜色和清屏
 * 传    参：col (U16)
 * 返 回 值：void
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
 * 函数名称：GlibLine
 * 功能描述：在TFT LCD屏上画一条直线
 * 传    参：(x1 (S32), y1 (S32))：起点, (x2 (S32), y2 (S32))：终点, col (U16)：颜色
 * 返 回 值：void
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
 * 函数名称：GlibRectangle
 * 功能描述：在LCD屏上画一个矩形
 * 传    参：(x1 (S32), y1 (S32))：起点, (x2 (S32), y2 (S32))：对称终点, col (U16)：颜色
 * 返 回 值：void
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
 * 函数名称：GlibFilledRectangle
 * 功能描述：使用特定的颜色在LCD屏上填充矩形
 * 传    参：(x1 (S32), y1 (S32))：起点, (x2 (S32), y2 (S32))：对称终点, col (U16)：颜色
 * 返 回 值：void
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
 * 函数名称：PaintBmp
 * 功能描述：在LCD屏上显示特定大小的图片
 * 传    参：(x0 (S32), y0 (S32))：显示图片的位置
 *           height (U32)：图片高度
 *           width  (U32)：图片宽度
 *           bmp   (U8 *)：图像数据指针
 * 返 回 值：void
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
 * 函数名称：LcdPutASCII
 * 功能描述：在LCD屏幕上制定坐标点写ASCII字符
 * 传    参：(x (U32), y (U32))：坐标位置
 *            ch           (U8)：所要显示的字符
 *            col         (U16)：字符颜色
 *            bkCol       (U16)：背景颜色
 *            st          (U32)：确认是否显示背景颜色, 1--默认
 * 返 回 值：void
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
 * 函数名称：LcdPutHZ
 * 功能描述：在LCD屏幕上制定坐标点写汉字
 * 传    参：(x (U32), y (U32))：坐标位置
 *            QW          (U16)：所要显示的汉字区位码
 *            col         (U16)：字符颜色
 *            bkCol       (U16)：背景颜色
 *            st          (U32)：确认是否显示背景颜色, 1--默认
 * 返 回 值：void
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
 		mask = 0x80;									/* 首先显示汉字的左半部分                 */
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
 		
 		mask = 0x80;									/* 再次显示汉字的右半部分                */
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
 * 函数名称：LcdPrintf
 * 功能描述：在LCD屏上显示输出的字符或者汉字
 * 传    参：(x (U32), y (U32))：坐标位置
 *            col         (U16)：字符颜色
 *            bkCol       (U16)：背景颜色
 *            st          (U32)：确认是否显示背景颜色, 1--默认
 *            fmt      (char *)：显示格式
 * 返 回 值：void
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
 				if ( *pStr > 0xA0 & *(pStr + 1) > 0xA0 )		/* 中文输出                       */
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
 * 函数名称：LcdN35Init
 * 功能描述：针对当前LCD屏进行初始化操作
 * 传    参：void
 * 返 回 值：void
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
	//LcdPrintf( 40, 100, COLOR(0xff, 0xff, 0xff), COLOR(0x00, 0x00, 0x00), 1, "HELLO, 李明浩\n" );
	//GlibRectangle( 20, 20, 200, 200, COLOR( 0xff, 0xff, 0xff ) );
	//GlibFilledRectangle( 20, 20, 200, 200, COLOR( 0xff, 0xff, 0xff ) );
 }