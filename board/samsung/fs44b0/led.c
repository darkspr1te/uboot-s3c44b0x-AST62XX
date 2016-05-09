/*
 * (C) Copyright 2006
 * Atmel Nordic AB <www.atmel.com>
 * Ulf Samuelsson <ulf@atmel.com>
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
#include <asm/hardware.h>

void	green_LED_on(void)
{
		PDATC = PDATC | (1<<1) ;
}

void	 yellow_LED_on(void)
{
		PDATC = PDATC | (1<<2) ;
}

void	 red_LED_on(void)
{
		PDATC = PDATC | (1<<3) ;
}

void	 blue_LED_on(void)
{
		PDATE = PDATE | (1<<5) ;
}
void	green_LED_off(void)
{
		PDATC = PDATC & (~(1<<1)) ;
}

void	yellow_LED_off(void)
{
		PDATC = PDATC & (~(1<<2)) ;
}

void	red_LED_off(void)
{
		PDATC = PDATC & (~(1<<3)) ;
}

void	blue_LED_off(void)
{
		PDATE = PDATE & (~(1<<5)) ;
}

void coloured_LED_init (void)
{
	PDATC = 0x0100;	/* All IO is low */
	PCONC = 0xfff4ff54;
	PUPC  = 0x0000;	/* PULL UP RESISTOR should be enabled to I/O */
	PCONE	= 0x20428;	/*0->input, 1 2->TXD0 RXD0, 3 4->input, 5->led, 6->buzzer, 7->led, 8->CODECLK */
	PDATE	= 0x57;
	PUPE	= 0x000;	/* disable all pull-up */
}
