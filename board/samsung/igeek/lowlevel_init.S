/* Bank 0 parameter lower memory */ 
.equ    B0_Tacs,      0x3 /* 0clk */
.equ    B0_Tcos,      0x3 /* 0clk */
.equ    B0_Tacc,      0x7 /* 14clk */
.equ    B0_Tcoh,      0x3 /* 0clk */
.equ    B0_Tah,       0x3 /* 0clk */
.equ    B0_Tacp,      0x1 /* 0clk */
.equ    B0_PMC,       0x0 /* normal(1data) */
/* Bank 1 parameter */
.equ    B1_Tacs,      0x0 /* 4clk */
.equ    B1_Tcos,      0x1 /* 4clk */
.equ    B1_Tacc,      0x2 /* 14clkv */
.equ    B1_Tcoh,      0x1 /* 4clk */
.equ    B1_Tah,       0x0 /* 4clk */
.equ    B1_Tacp,      0x0 /* 6clk */
.equ    B1_PMC,       0x0 /* normal(1data) */

/* Bank 2 parameter -  */
.equ    B2_Tacs,      0x0 /* 4clk */
.equ    B2_Tcos,      0x2 /* 4clk */
.equ    B2_Tacc,      0x4 /* 14clk */
.equ    B2_Tcoh,      0x2 /* 4clk */
.equ    B2_Tah,       0x3 /* 4clk */
.equ    B2_Tacp,      0x3 /* 6clk */
.equ    B2_PMC,       0x0 /* normal(1data) */

/* Bank 3 parameter */
.equ    B3_Tacs,      0x3 /* 4clk */
.equ    B3_Tcos,      0x3 /* 4clk */
.equ    B3_Tacc,      0x7 /* 14clk */
.equ    B3_Tcoh,      0x3 /* 4clk */
.equ    B3_Tah,       0x3 /* 4clk */
.equ    B3_Tacp,      0x3 /* 6clk */
.equ    B3_PMC,       0x0 /* normal(1data) */

/* Bank 4 parameter */
.equ    B4_Tacs,      0x3 /* 4clk */
.equ    B4_Tcos,      0x3 /* 4clk */
.equ    B4_Tacc,      0x7 /* 14clk */
.equ    B4_Tcoh,      0x3 /* 4clk */
.equ    B4_Tah,       0x3 /* 4clk */
.equ    B4_Tacp,      0x3 /* 6clk */
.equ    B4_PMC,       0x0 /* normal(1data) */

/* Bank 5 parameter */
.equ    B5_Tacs,      0x0 /* 4clk */
.equ    B5_Tcos,      0x1 /* 4clk */
.equ    B5_Tacc,      0x4 /* 14clk */
.equ    B5_Tcoh,      0x1 /* 4clk */
.equ    B5_Tah,       0x0 /* 4clk */
.equ    B5_Tacp,      0x0 /* 6clk */
.equ    B5_PMC,       0x0 /* normal(1data) */

/* Bank 6(if SROM) parameter */
.equ    B6_Tacs,      0x3 /* 4clk */
.equ    B6_Tcos,      0x3 /* 4clk */
.equ    B6_Tacc,      0x7 /* 14clk */
.equ    B6_Tcoh,      0x3 /* 4clk */
.equ    B6_Tah,       0x3 /* 4clk */
.equ    B6_Tacp,      0x3 /* 6clk */
.equ    B6_PMC,       0x0 /* normal(1data) */

/* Bank 7(if SROM) parameter */
.equ    B7_Tacs,      0x3 /* 4clk */
.equ    B7_Tcos,      0x3 /* 4clk */
.equ    B7_Tacc,      0x7 /* 14clk */
.equ    B7_Tcoh,      0x3 /* 4clk */
.equ    B7_Tah,       0x3 /* 4clk */
.equ    B7_Tacp,      0x3 /* 6clk */
.equ    B7_PMC,       0x0 /* normal(1data) */

/* Bank 6 parameter */
.equ    B6_MT,        0x3 /* SDRAM */
.equ    B6_Trcd,      0x1 /* 2clk */
.equ    B6_SCAN,      0x0 /* 8bit */

.equ    B7_MT,        0x3 /* SDRAM */
.equ    B7_Trcd,      0x1 /* 2clk */ 
.equ    B7_SCAN,      0x0 /* 8bit */


/* REFRESH parameter */
.equ    REFEN,        0x1 /* Refresh enable */
.equ    TREFMD,       0x0 /* CBR(CAS before RAS)/Auto refresh */
.equ    Trp,        0x0 /* 2clk */
.equ    Trc,        0x3 /* 0x1=5clk 0x3=11clk*/
.equ    Tchr,       0x0 /* 0x2=3clk 0x0=0clks  */
.equ    REFCNT,       1550

/* MEMORY_CONFIG第一个值是BWSCON寄存器，各位表示的情况如下:
  [0    ]   : 0 表示 LITTLE ENDIAN, 1 表示BIG ENDIAN 
  [1 - 2]   : 表示blank 0 数据总线宽度 00:8-bit 01:16-bit 10:32-bit
  [4 - 5]   : 表示blank 1 数据总线宽度
  [6    ]   : 表示blank 1 wait开关, 0:WAIT disable  1:WAIT enable
  [7    ]   : 表示blank 1 是否使用 UB/LB, 0:not using UB/LB, 1:using UB/LB
  [8 - 9]   : 表示blank 2 数据总线宽度
  [10   ]   : 表示blank 2 wait开关
  [11   ]   : 表示blank 1 是否使用 UB/LB
  ........
 */
MEMORY_CONFIG:
  .long   0x01001102 /* Bank0 = OM[1:0] , Bank1-7 16bit, Bank2=Nowait,UB/LB*/
  .word ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC)) /*GCS0*/
  .word ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC)) /*GCS1*/
  .word ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC)) /*GCS2*/
  .word ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC)) /*GCS3*/
  .word ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC)) /*GCS4*/
  .word ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC)) /*GCS5*/
  .word ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))  /*GCS6*/
  .word ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))  /*GCS7*/
  .word ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)  /*REFRESH RFEN=1, TREFMD=0, trp=3clk, trc=5clk, tchr=3clk,count=1019*/
  .word 0x10      /*SCLK power down mode, BANKSIZE 16M/16M*/
  .word 0x20      /*MRSR6 CL=2clk*/
  .word 0x20      /*MRSR7*/

.globl lowlevel_init
lowlevel_init:

	/*
		the next instruction fail due memory relocation...
		we'll find the right MEMORY_CONFIG address with the next 3 lines...
	*/
	/*ldr r0, =MEMORY_CONFIG*/
	mov r0, pc
	ldr r1, =(0x38+4)
	sub r0, r0, r1

	ldmia r0, {r1-r13}
	ldr   r0, =0x01c80000
	stmia r0, {r1-r13}
	mov pc, lr
