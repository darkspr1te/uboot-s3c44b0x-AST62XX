/*
 * (C) Copyright 2006 OpenMoko, Inc.
 * Author: Harald Welte <laforge@openmoko.org>
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

#include <nand.h>
#include <asm/hardware.h>
#include <asm/io.h>

#define NF_CE_L()           (PDATC = PDATC & ~(1<<9))
#define NF_CE_H()           (PDATC = PDATC | (1<<9))
#define S3C44B0_ADDR_NALE 4
#define S3C44B0_ADDR_NCLE 2
#define NAND_BASE 0x02000000
#ifdef CONFIG_NAND_SPL

/* in the early stage of NAND flash booting, printf() is not available */
#define printf(fmt, args...)

static void nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;

	for (i = 0; i < len; i++)
		buf[i] = readb(this->IO_ADDR_R);
}
#endif

static void s3c44b0_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;
	debugX(1, "hwcontrol(): 0x%02x 0x%02x\n", cmd, ctrl);

	if (ctrl & NAND_CTRL_CHANGE) {
		
		ulong IO_ADDR_W = NAND_BASE;

		if (ctrl & NAND_CLE)
			IO_ADDR_W |= S3C44B0_ADDR_NCLE;
		if (ctrl & NAND_ALE)
			IO_ADDR_W |= S3C44B0_ADDR_NALE;

		chip->IO_ADDR_W = (void *)IO_ADDR_W;

		if (ctrl & NAND_NCE)
			NF_CE_L();
		else
			NF_CE_H();
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, chip->IO_ADDR_W);
}

static int s3c44b0_dev_ready(struct mtd_info *mtd)
{
	debugX(1, "dev_ready\n");
	return (PDATC&(1<<8));
}


int board_nand_init(struct nand_chip *nand)
{

	debugX(1, "board_nand_init()\n");

	/* initialize nand_chip data structure */
	nand->IO_ADDR_R = (void *)(0x02000000);
	nand->IO_ADDR_W = (void *)(0x02000000);

	nand->select_chip = NULL;

	/* read_buf and write_buf are default */
	/* read_byte and write_byte are default */
#ifdef CONFIG_NAND_SPL
	nand->read_buf = nand_read_buf;
#endif

	/* hwcontrol always must be implemented */
	nand->cmd_ctrl = s3c44b0_hwcontrol;

	nand->dev_ready = s3c44b0_dev_ready;

	nand->ecc.mode = NAND_ECC_SOFT;

#ifdef CONFIG_S3C44B0_NAND_BBT
	nand->options = NAND_USE_FLASH_BBT;
#else
	nand->options = 0;
#endif

	debugX(1, "end of nand_init\n");

	return 0;
}
