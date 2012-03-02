;=============================================================================================
;										LJ_RTOS
;								The Real-Time Kernel
;
; (c) Copyright 2010, 李明浩
; All Rights Reserved
;
; 文件名称：memcfg.inc
; 文件标识：配置文件(同c语言中的头文件)
; 摘    要：本文件用于s3c2440a的内存块配置
;
; 当前版本：0.0
; 作    者：李明浩
; 完成日期：2010/11/8
;
; 取代版本：
; 作    者：
; 完成日期：
;=============================================================================================

; BWSCON--总线宽度和WAIT控制寄存器
DW8		EQU		(0x0)
DW16	EQU		(0x1)
DW32	EQU		(0x2)
WAIT	EQU		(0x1 << 2)
UBLB	EQU		(0x1 << 3)

; 根据不同的总线宽度进行不同配置, 未定义B0_BWSCON, 是因为BANK0作为启动ROM, 在访问
; 前必须确定总线宽度，由OM[0:1]决定
	ASSERT	:DEF:BUSWIDTH
	[ BUSWIDTH = 16
	
B1_BWSCON	EQU	(DW16)
B2_BWSCON	EQU	(DW16)
B3_BWSCON	EQU	(DW16)
B4_BWSCON	EQU	(DW16)
B5_BWSCON	EQU	(DW16)
B6_BWSCON	EQU	(DW16)
B7_BWSCON	EQU	(DW16)

	|

B1_BWSCON	EQU	(DW32)	
B2_BWSCON	EQU	(DW16)
B3_BWSCON	EQU	(0xd)
B4_BWSCON	EQU	(DW16)
B5_BWSCON	EQU	(DW16)
B6_BWSCON	EQU	(DW32)
B7_BWSCON	EQU	(DW32)
	]

; TACS 地址有效后，等待CS
; TCOS CS有效后，等待OE
; TACC 访问时间
; TCOH OE无效后，保持CS
; TAH CS无效后，保持地址
; TACP Page模式下的访问时间
; TPMC Page模式访问
; MT 内存类型
; TRCD RAS到CAS的延时(地址建立时间)
; SCAN 列地址位数
; TRP RAS预充电时间
; TSRC 半行周期时间
; TRC = TSRC + TRP 行周期时间

; BANKCON0
B0_TACS		EQU		0x0		;0clk
B0_TCOS		EQU		0x0		;0clk
B0_TACC		EQU		0x7		;14clk
B0_TCOH		EQU		0x0		;0clk
B0_TAH		EQU		0x0		;0clk
B0_TACP		EQU		0x0		;0clk
B0_PMC		EQU		0x0		;常规

; BANKCON1
B1_TACS		EQU		0x0		;0clk
B1_TCOS		EQU		0x0		;0clk
B1_TACC		EQU		0x7		;14clk
B1_TCOH		EQU		0x0		;0clk
B1_TAH		EQU		0x0		;0clk
B1_TACP		EQU		0x0		;0clk
B1_PMC		EQU		0x0		;常规

; BANKCON2
B2_TACS		EQU		0x0		;0clk
B2_TCOS		EQU		0x0		;0clk
B2_TACC		EQU		0x7		;14clk
B2_TCOH		EQU		0x0		;0clk
B2_TAH		EQU		0x0		;0clk
B2_TACP		EQU		0x0		;0clk
B2_PMC		EQU		0x0		;常规

; BANKCON3
B3_TACS		EQU		0x1		;1clk
B3_TCOS		EQU		0x3		;4clk
B3_TACC		EQU		0x7		;14clk
B3_TCOH		EQU		0x2		;2clk
B3_TAH		EQU		0x1		;1clk
B3_TACP		EQU		0x3		;6clk
B3_PMC		EQU		0x0		;常规

; BANKCON4
B4_TACS		EQU		0x0		;0clk
B4_TCOS		EQU		0x0		;0clk
B4_TACC		EQU		0x7		;14clk
B4_TCOH		EQU		0x0		;0clk
B4_TAH		EQU		0x0		;0clk
B4_TACP		EQU		0x0		;0clk
B4_PMC		EQU		0x0		;常规

; BANKCON5
B5_TACS		EQU		0x0		;0clk
B5_TCOS		EQU		0x0		;0clk
B5_TACC		EQU		0x7		;14clk
B5_TCOH		EQU		0x0		;0clk
B5_TAH		EQU		0x0		;0clk
B5_TACP		EQU		0x0		;0clk
B5_PMC		EQU		0x0		;常规

; BANKCON6
B6_MT		EQU		0x3		;SDRAM
B6_TRCD		EQU		0x2		;4clk
B6_SCAN		EQU		0x1		;9bit

; BANKCON7
B7_MT		EQU		0x3		;SDRAM
B7_TRCD		EQU		0x2		;4clk
B7_SCAN		EQU		0x1		;9bit

; REFRESH
REFRESH_EN		EQU		0x0		;刷新使能
REFRESH_MODE	EQU		0x0		;确定刷新模式/自动刷新
TRP				EQU		0x2		;4clk
TRC				EQU		0x2		;6clk

TCHR			EQU		0x2		;3clk
REFRESH_CNT		EQU		1296	;刷新计数值 = 2048 + 1 - HCLK * 刷新周期

	END



