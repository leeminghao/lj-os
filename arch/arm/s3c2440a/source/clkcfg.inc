;===================================================================================
; 											  LJ_RTOS
;										The Real-Time Kernel
;
; (c) Copyright 2010, 李明浩
; All Rights Reserved
;
; 文件名称：clkcfg.inc
; 文件标识：配置文件(同c语言中的头文件)
; 摘    要：本文件用于s3c2440a时钟配置
;
; 当前版本：0.0
; 作    者：李明浩
; 完成日期：2010/11/8
;
; 取代版本：
; 作    者：
; 完成日期：
;====================================================================================

; 各种起始地址声明
STACK_BASEADDRESS		EQU		0x33ff8000			; 堆栈起始地址声明
MMUTT_STARTADDRESS		EQU		0x33ff8000			; 基地址-基址寄存器的初始值
ISR_STARTADDRESS		EQU		0x33ffff00			; 中断起始地址

	GBLL	USE_MAIN			; 用于判断是否使用main函数
USE_MAIN		SETL	{FALSE}

	GBLL	PLL_ON_START		; 用于锁相环使能判断
PLL_ON_START	SETL	{TRUE}

	GBLL	ENDIAN_CHANGE
ENDIAN_CHANGE	SETL	{FALSE}	; 用于判断是否为大端模式

	GBLA	ENTRY_BUS_WIDTH		; 用于判断入口总线宽度
ENTRY_BUS_WIDTH SETA	16

	GBLA	BUSWIDTH			; 用于判断总线宽度
BUSWIDTH		SETA	32

	GBLA	UCLK
UCLK		SETA		48000000	; 设置USB口时钟频率

	GBLA	XTAL_SEL				; 用于选择晶振频率
	GBLA	FCLK					; 内核时钟频率

; 选择晶振频率
XTAL_SEL	SETA		12000000
;XTAL_SEL	SETA		16934400

; 选择内核时钟频率
FCLK		SETA		304000000

; 选择时钟分频比(FCLK : HCLK : PCLK)
CLKDIV_VAL	EQU			7			; 1 : 3 : 6

; 具体时钟配置，按照芯片手册进行配置
 [ XTAL_SEL = 12000000
	
	[ FCLK = 271500000
M_MDIV	EQU	173
M_PDIV	EQU	2
M_SDIV	EQU	2
	]
	
	[ FCLK = 304000000
M_MDIV	EQU	68
M_PDIV	EQU	1
M_SDIV	EQU	1
	]
	
	[ UCLK = 48000000
U_MDIV	EQU	56
U_PDIV	EQU	2
U_SDIV	EQU	2
	]
	
	[ UCLK = 96000000
U_MDIV	EQU	56
U_PDIV	EQU	2
U_SDIV	EQU	1
	]
	
 |	; XTAL_SEL = 16.9344HZ
	
	[ FCLK = 296352000
M_MDIV	EQU	97
M_PDIV	EQU	1
M_SDIV	EQU	2
	]
	
	[ FCLK = 541900800
M_MDIV	EQU	120
M_PDIV	EQU	2
M_SDIV	EQU	1
	]
	
	[ UCLK = 48000000
M_MDIV	EQU	60
M_PDIV	EQU	4
M_SDIV	EQU	2
	]
	
	[ UCLK = 96000000
M_MDIV	EQU	60
M_PDIV	EQU	4
M_SDIV	EQU	1
	]
 ]

	END