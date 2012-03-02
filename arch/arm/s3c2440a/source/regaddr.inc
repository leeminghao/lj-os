;====================================================================================
;										LJ_RTOS
;								The Real-Time Kernel
; 
; (c) Copyright 2010, 李明浩
; All Rights Reserved
;
; 文件名称：regaddr.inc
; 文件标识：配置文件(相当于c语言头文件)
; 摘    要：本文件主要用于声明s3c2440a的寄存器地址
;
; 当前版本：0.0
; 作    者：李明浩
; 完成日期：2010/11/8
;
; 取代版本：
; 作    者：
; 完成日期：
;====================================================================================

; 内存控制寄存器
BWSCON		EQU		0x48000000		; 总线宽度&等待控制寄存器
BANKCON0	EQU		0x48000004		; ROM, 完成装入工作
BANKCON1	EQU		0x48000008		; Bank1 控制寄存器
BANKCON2	EQU		0x4800000c		; Bank2 控制寄存器
BANKCON3	EQU		0x48000010		; Bank3 控制寄存器
BANKCON4	EQU		0x48000014		; Bank4 控制寄存器
BANKCON5	EQU		0x48000018		; Bank5 控制寄存器
BANKCON6	EQU		0x4800001c		; Bank6 控制寄存器
BANKCON7	EQU		0x48000020		; Bank7 控制寄存器
REFRESH		EQU		0x48000024		; SDRAM 刷新控制寄存器
BANKSIZE	EQU		0x48000028		; 存储块大小控制寄存器
MRSRB6		EQU		0x4800002c		; 模式寄存器集寄存器BANK6
MRSRB7		EQU		0x48000030		; 模式寄存器集寄存器BANK7

; 时钟及电源管理
LOCKTIME	EQU		0x4c000000		; 锁定时间计数寄存器
MPLLCON		EQU		0x4c000004		; MPLL配置寄存器
UPLLCON		EQU		0x4c000008		; UPLL配置寄存器
CLKCON		EQU		0x4c00000c		; 时钟控制寄存器
CLKSLOW		EQU		0x4c000010		; 时钟减慢控制寄存器
CLKDIVN		EQU		0x4c000014		; 时钟分频器控制寄存器

; 中断控制器
SRCPND		EQU		0x4a000000		; 源未决寄存器
INTMOD		EQU		0x4a000004		; 中断模式寄存器
INTMSK		EQU		0x4a000008		; 中断屏蔽寄存器
PRIORITY	EQU		0x4a00000c		; 优先级寄存器
INTPND		EQU		0x4a000010		; 中断未决寄存器
INTOFFSET	EQU		0x4a000014		; 中断偏移寄存器
SUBSRCPND	EQU		0x4a000018		; 子中断源未决寄存器
SUBINTMSK	EQU		0x4a00001c		; 子中断屏蔽寄存器

; 看门狗定时器
WTCON		EQU		0x53000000		; 看门狗控制寄存器
WTDAT		EQU		0x53000004		; 看门狗计数常数寄存器
WTCNT		EQU		0x53000008		; 看门狗计数器寄存器

; Nand Flash
NFCONF		EQU		0x4e000000		; Nand Flash 配置寄存器
NFCONT		EQU		0x4e000004		; Nand Flash 控制寄存器

; I/O Port
GPBCON		EQU		0x56000010
GPBDAT		EQU		0x56000014
GPBUP		EQU		0x56000018

; 端口其他寄存器
MISCCR		EQU		0x56000080		; 多控制寄存器
DCLKCON		EQU		0x56000084		; DCLK外部源时钟寄存器
EXTINT0		EQU		0x56000088		; 外部中断控制寄存器0
EXTINT1		EQU		0x5600008c		; 外部中断控制寄存器1
EXTINT2		EQU		0x56000090		; 外部中断控制寄存器2
EINTFLT0	EQU		0x56000094		; 外部中断过滤寄存器0(保留)
EINTFLT1	EQU		0x56000098		; 外部中断过滤寄存器1(保留)
EINTFLT2	EQU		0x5600009c		; 外部中断过滤寄存器2
EINTFLT3	EQU		0x560000a0		; 外部中断过滤寄存器3
EINTMASK	EQU		0x560000a4		; 外部中断屏蔽寄存器
EINTPEND	EQU		0x560000a8		; 外部中断未决寄存器
GSTATUS0	EQU		0x560000ac		; 通用状态寄存器0
GSTATUS1	EQU		0x560000b0		; 通用状态寄存器1
GSTATUS2	EQU		0x560000b4		; 通用状态寄存器2
GSTATUS3	EQU		0x560000b8		; 通用状态寄存器3
GSTATUS4	EQU		0x560000bc		; 通用状态寄存器4
MSLCON		EQU		0x560000cc		; 内存睡眠控制寄存器

	END
