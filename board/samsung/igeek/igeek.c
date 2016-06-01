/*
 * (C) Copyright 2004
 * DAVE Srl
 * http://www.dave-tech.it
 * http://www.wawnet.biz
 * mailto:info@wawnet.biz
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <netdev.h>
#include <asm/hardware.h>
#include "lcd_s3c44b0.h"
#include "logo.h"

DECLARE_GLOBAL_DATA_PTR;
#define DISPLAY_FLIP 
/*
 * Miscelaneous platform dependent initialization
 */
/*
 * see drivers/usb/host/sl811-hdc.c for bank/memory address for USB chip 800_0000 is bank4 on s3c44b0x
#define	 SL811_ADR (0x8000000)
#define	 SL811_DAT (0x800FFFF)
* */
int buffer[128*64/8] = {
	};
int buffer_big[132*64/8] = {
	};
int board_early_init_f (void)
{
	PCONE = 0x0A4569;
	PDATE = 0x0;
	PUPE = 0x000;
/*
	init_port();
	reset_lcd();//put LCD into a know state
	lcd_init();
	
	 // send_cmd(CMD_SET_COM_NORMAL);
	send_cmd(CMD_SET_ADC_NORMAL);
    send_cmd(CMD_SET_COM_REVERSE);
 #ifdef DISPLAY_FLIP 
 	send_cmd(CMD_SET_COM_NORMAL);
	send_cmd(CMD_SET_ADC_REVERSE);
#endif

	//write_buffer(logo);
//	write_buffer(buffer);
	//udelay(1000);
	display_picture(bat_two);*/
	return 0;
}

int board_init (void)
{
	u32 temp;

	/* Configuration Port Control Register*/
	/* Port A */
	PCONA = 0x3ff;

	/* Port B */
	PCONB = 0x7ff;/*bank4 must 1-nGSC4 for usb
	PDATB = 0x7cf;

	/* Port C */
	
	/*PCONC = 0xfff4ff54;
	PDATC = 0x0000;
	PUPC = 0x0000;
	*/
	PCONC = 0x0f05ff55;
    PUPC=0x30f0;
    PDATC = 0x0000;

	/* Port D */
	
	PCOND = 0xaaaa;
	PUPD = 0xff;
	

	/* Port E */
/*	PCONE = 0x26568;
	PDATE = 0xc0;
	PUPE = 0x000;*/
/* darksprite
 * */
	PCONE = 0x0A4569;
	PDATE = 0x0;
	PUPE = 0x000;
	
	/* Port F */
	//PCONF = 0x24900a;
	PCONF = 0xA456A;
	PDATF  = 0x63; /* fs44b0-eth_reset tied high level */
	
	PUPF = 0x000;
	

	/* Port G */
	//PCONG = 0xff3c;
	PUPG = 0x00;
	PCONG = 0x3; /*PG0= EINT0= ETH_INT prepared for linux kernel*/

	INTMSK = 0x07ffffff;
	INTCON = 0x05;

	SPUCR = 0x6;
    /*
    	Configure chip ethernet interrupt as High level
    	Port G EINT 0-7 EINT0 -> CHIP ETHERNET
    */
	temp = EXTINT;
  	temp &= ~0x7;
    temp |= 0x1; /*LEVEL_HIGH*/
    EXTINT = temp;
	//EXTINT = 0x40440;

    /*
    	Reset SMSC LAN91C96 chip
    */
    /*
    temp= PCONF;
    temp |= 0x00000040;
    PCONF = temp;

	/* Reset high 
    temp = PDATF;
    temp |= (1 << 3);
    PDATF = temp;

    /* Short delay 
    for (temp=0;temp<10;temp++)
    {
		/* NOP 
    }

    /* Reset low 
    temp = PDATF;
    temp &= ~(1 << 3);
    PDATF = temp;
*/

	/* arch number MACH_TYPE_MBA44B0 */
	gd->bd->bi_arch_number = MACH_TYPE_S3C44B0;

	/* location of boot parameters */
	//gd->bd->bi_boot_params = 0x0c000100;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;
	gd->bd->bi_baudrate = 115200;
	serial_init();
	serial_setbrg();
	init_port();
	reset_lcd();//put LCD into a know state
	lcd_init();
	
	 //  send_cmd(CMD_SET_COM_NORMAL);
	send_cmd(CMD_SET_ADC_NORMAL);
    send_cmd(CMD_SET_COM_REVERSE);
 #ifdef DISPLAY_FLIP 
 	send_cmd(CMD_SET_COM_NORMAL);
	send_cmd(CMD_SET_ADC_REVERSE);
#endif

	display_picture(blank);
	empty_buffer(buffer);//set LC screen buffer to empty
	write_buffer(logo);
	//write_buffer(buffer);
	udelay(1000);
	display_picture(igeek_logo_small);

#ifdef CONFIG_BOARD_SPECIFIC_LED
#ifdef STATUS_LED_BIT
status_led_set(STATUS_LED_BOOT, STATUS_LED_BLINKING);
#endif
#endif

	return 0;
}

int dram_init (void)
{
	//gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	//gd->ram_size = PHYS_SDRAM_1_SIZE;
gd->ram_size = get_ram_size((long *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE);
	return (0);
}

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
#include <linux/mtd/nand.h>
extern struct nand_chip nand_dev_desc[CFG_MAX_NAND_DEVICE];

void nand_init(void)
{
	nand_probe(CFG_NAND_BASE);
	if (nand_dev_desc[0].ChipID != NAND_ChipID_UNKNOWN) {
		print_size(nand_dev_desc[0].totlen, "\n");
	}
}
#endif
#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:\tS3c44B0X aka ac-100 fingerprint\n");
	printf("CPU Speed is %d MHZ\n",CONFIG_S3C44B0_CLOCK_SPEED);
	return 0;
}
#endif

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
        int rc = 0;
#ifdef CONFIG_DRIVER_RTL8019
        rc = rtl8019_initialize(0, 0);
#endif
        return rc;
}
#endif
