/*
 * Realtek 8019AS Ethernet
 * (C) Copyright 2002-2003
 * Xue Ligong(lgxue@hotmail.com),Wang Kehao, ESLAB, whut.edu.cn
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

/*
 * This code works in 8bit mode.
 * If you need to work in 16bit mode, PLS change it!
 */

#include <common.h>
#include <command.h>
#include "rtl8019.h"
#include <net.h>
#ifdef CONFIG_DRIVER_RTL8019




/* packet page register access functions */

static unsigned char get_reg (unsigned int regno)
{
	return (*(volatile unsigned char *) regno);
}
static unsigned short get_reg_short (unsigned int regno)
{
	return (*(volatile unsigned short *) regno);
}
static void put_reg (unsigned int regno, unsigned char val)
{
	*(volatile unsigned char *) regno = val;
}
static void put_reg_short (unsigned int regno, unsigned short val)
{
	*(volatile unsigned short *) regno = val;
}
static void eth_reset (void)
{
	unsigned char ucTemp;

	/* reset NIC */
	ucTemp = get_reg (RTL8019_RESET);
	put_reg (RTL8019_RESET, ucTemp);
	put_reg (RTL8019_INTERRUPTSTATUS, 0xff);
	udelay (2000);		/* wait for 2ms */
}


void rtl8019_halt (struct eth_device *dev)
{
	put_reg (RTL8019_COMMAND, 0x01);
}

int rtl8019_init (struct eth_device *dev,bd_t * bd)
{
	uchar enetaddr[6];
	eth_reset ();
	put_reg (RTL8019_COMMAND, RTL8019_PAGE0STOP);//0x21
	put_reg (RTL8019_DATACONFIGURATION, 0x48);
	put_reg (RTL8019_REMOTEBYTECOUNT0, 0x00);
	put_reg (RTL8019_REMOTEBYTECOUNT1, 0x00);
	put_reg (RTL8019_RECEIVECONFIGURATION, 0x00);	/*00; */
	put_reg (RTL8019_TRANSMITPAGE, RTL8019_TPSTART);
	put_reg (RTL8019_TRANSMITCONFIGURATION, 0x00);//maybe 0x?
	put_reg (RTL8019_PAGESTART, RTL8019_PSTART);
	put_reg (RTL8019_BOUNDARY, RTL8019_PSTART);
	put_reg (RTL8019_PAGESTOP, RTL8019_PSTOP);
	put_reg (RTL8019_INTERRUPTSTATUS, 0xff);
	put_reg (RTL8019_INTERRUPTMASK, 0x11);	/*b; */
	put_reg (RTL8019_COMMAND, RTL8019_PAGE1STOP);
  	eth_getenv_enetaddr("ethaddr", enetaddr);
	printf("MAC:%x-%x-%x-%x-%x-%x\n",enetaddr[0],enetaddr[1],enetaddr[2],enetaddr[3],enetaddr[4],enetaddr[5]);    
   	put_reg (RTL8019_PHYSICALADDRESS0, enetaddr[0]);
       	put_reg (RTL8019_PHYSICALADDRESS1, enetaddr[1]);
       	put_reg (RTL8019_PHYSICALADDRESS2, enetaddr[2]);
       	put_reg (RTL8019_PHYSICALADDRESS3, enetaddr[3]);
       	put_reg (RTL8019_PHYSICALADDRESS4, enetaddr[4]);
       	put_reg (RTL8019_PHYSICALADDRESS5, enetaddr[5]);	
	put_reg (RTL8019_MULTIADDRESS0, 0x00);
	put_reg (RTL8019_MULTIADDRESS1, 0x00);
	put_reg (RTL8019_MULTIADDRESS2, 0x00);
	put_reg (RTL8019_MULTIADDRESS3, 0x00);
	put_reg (RTL8019_MULTIADDRESS4, 0x00);
	put_reg (RTL8019_MULTIADDRESS5, 0x00);
	put_reg (RTL8019_MULTIADDRESS6, 0x00);
	put_reg (RTL8019_MULTIADDRESS7, 0x00);
	put_reg (RTL8019_CURRENT, RTL8019_PSTART);
	put_reg (RTL8019_COMMAND, RTL8019_PAGE0);
	put_reg (RTL8019_TRANSMITCONFIGURATION, 0xe0);	/*58; */
	//put_reg (RTL8019_TRANSMITCONFIGURATION, 0xe6);	/*58; */

	return 0;
}

static unsigned char nic_to_pc (void)
{
	unsigned char rec_head_status;
	unsigned char next_packet_pointer;
//	unsigned char packet_length0;
//	unsigned char packet_length1;
	unsigned short rxlen = 0;
	unsigned int i = 4;
	unsigned char current_point;
	unsigned short *saddr, temp;

	/*
	 * The RTL8019's first 4B is packet status,page of next packet
	 * and packet length(2B).So we receive the fist 4B.
	 */
	put_reg (RTL8019_REMOTESTARTADDRESS1, get_reg (RTL8019_BOUNDARY));
	put_reg (RTL8019_REMOTESTARTADDRESS0, 0x00);
	put_reg (RTL8019_REMOTEBYTECOUNT1, 0x00);
	put_reg (RTL8019_REMOTEBYTECOUNT0, 0x04);

	put_reg (RTL8019_COMMAND, RTL8019_REMOTEDMARD);

	temp = get_reg_short (RTL8019_DMA_DATA);
	rec_head_status = (temp & 0xff);
	next_packet_pointer = (temp >> 8) & 0xff;
	rxlen = get_reg_short (RTL8019_DMA_DATA);

	put_reg (RTL8019_COMMAND, RTL8019_PAGE0);
	rxlen -= 4;

	if (rxlen > PKTSIZE_ALIGN + PKTALIGN)
		printf ("packet too big! %d\n",rxlen);

	/*Receive the packet */
	put_reg (RTL8019_REMOTESTARTADDRESS0, 0x04);
	put_reg (RTL8019_REMOTESTARTADDRESS1, get_reg (RTL8019_BOUNDARY));

	put_reg (RTL8019_REMOTEBYTECOUNT0, (rxlen & 0xff));
	put_reg (RTL8019_REMOTEBYTECOUNT1, ((rxlen >> 8) & 0xff));


	put_reg (RTL8019_COMMAND, RTL8019_REMOTEDMARD);

	for (saddr = (unsigned short *) NetRxPackets[0], i = rxlen>>1; i > 0; i--)
		*saddr++ = get_reg_short (RTL8019_DMA_DATA);
	/* Pass the packet up to the protocol layers. */
	NetReceive (NetRxPackets[0], rxlen);

	while (!(get_reg (RTL8019_INTERRUPTSTATUS)) & 0x40);	/* wait for the op. */

	/*
	 * To test whether the packets are all received,get the
	 * location of current point
	 */
	put_reg (RTL8019_COMMAND, RTL8019_PAGE1);
	current_point = get_reg (RTL8019_CURRENT);
	put_reg (RTL8019_COMMAND, RTL8019_PAGE0);
	put_reg (RTL8019_BOUNDARY, next_packet_pointer);
	return current_point;
}

/* Get a data block via Ethernet */
int rtl8019_recv (struct eth_device *dev)
{
	unsigned char temp, current_point,othertemp,secondtemp;

	put_reg (RTL8019_COMMAND, RTL8019_PAGE0);

	while (1) {
		
		temp = get_reg (RTL8019_INTERRUPTSTATUS);
		othertemp= get_reg(RTL8019_REG_0a);
		secondtemp =get_reg(RTL8019_REG_0b);
	//	printf("ISR %02x",temp);
		//printf("rega %02x",othertemp);
	//	printf("regb %02x",secondtemp);
		
		if (temp & 0x90) {
			/*overflow */
		//	printf("over\n");
			put_reg (RTL8019_COMMAND, RTL8019_PAGE0STOP);
			udelay (2000);
			put_reg (RTL8019_REMOTEBYTECOUNT0, 0);
			put_reg (RTL8019_REMOTEBYTECOUNT1, 0);
			put_reg (RTL8019_TRANSMITCONFIGURATION, 2);
			do {
				current_point = nic_to_pc ();
				//printf("nic to pc\n");
			} while (get_reg (RTL8019_BOUNDARY) != current_point);

			put_reg (RTL8019_TRANSMITCONFIGURATION, 0xe0);
		}

		if (temp & 0x1) {
			/*packet received */
			do {
				put_reg (RTL8019_INTERRUPTSTATUS, 0x01);
				current_point = nic_to_pc ();
				//printf("packet rec\n");
			} while (get_reg (RTL8019_BOUNDARY) != current_point);
		}
		//printf("while 1 exit\n");
		if (!(temp & 0x1))
			return 0;
		/* done and exit. */
	}
}

/* Send a data block via Ethernet. */
int rtl8019_send (struct eth_device *dev,volatile void *packet, int length)
{
	volatile unsigned char *p;
	volatile unsigned short *shortp;
	unsigned int pn, i, count;

	//printf("eth_send....length=%d\n", length);
	count = length; if(count < 60) count = 60;
	pn = length >> 1;
	p = (volatile unsigned char *) packet;
#if 0
	for(i = 0; i < length; i++)
		printf("%02x ", *p++);
	printf("\n");
#endif
	shortp = (volatile unsigned short *) packet;
	//print_8019_reg();

	while (get_reg (RTL8019_COMMAND) == RTL8019_TRANSMIT);

	put_reg (RTL8019_REMOTESTARTADDRESS0, 0);
	put_reg (RTL8019_REMOTESTARTADDRESS1, RTL8019_TPSTART);
	put_reg (RTL8019_REMOTEBYTECOUNT0, (count & 0xff));
	put_reg (RTL8019_REMOTEBYTECOUNT1, ((count >> 8) & 0xff));

	put_reg (RTL8019_COMMAND, RTL8019_REMOTEDMAWR);
	while (pn > 0) {
		put_reg_short (RTL8019_DMA_DATA, *shortp++);
		pn--;
	}

	pn = length >> 1;

	while (pn < 60) {	/*Padding */
		put_reg_short (RTL8019_DMA_DATA, 0);
		pn++;
	}

	while (!(get_reg (RTL8019_INTERRUPTSTATUS)) & 0x40);

	put_reg (RTL8019_INTERRUPTSTATUS, 0x40);
	put_reg (RTL8019_TRANSMITPAGE, RTL8019_TPSTART);
	put_reg (RTL8019_TRANSMITBYTECOUNT0, (count & 0xff));
	put_reg (RTL8019_TRANSMITBYTECOUNT1, ((count >> 8 & 0xff)));
	put_reg (RTL8019_COMMAND, RTL8019_TRANSMIT);
	return 0;
}

int rtl8019_initialize(u8 dev_num, int base_addr)
{
	struct eth_device *dev;
	dev = malloc(sizeof(*dev));
	if (!dev) {
		return 0;
	}
	memset(dev, 0, sizeof(*dev));

	dev->init = rtl8019_init;
	dev->halt = rtl8019_halt;
	dev->send = rtl8019_send;
	dev->recv = rtl8019_recv;

	sprintf(dev->name, "%s-%hu", "RTL8019", dev_num);
	//printf(dev->name, "%s-%hu", "RTL8019", dev_num);
	eth_register(dev);
	return 0;
}

#endif /* CONFIG_DRIVER_RTL8019 */
