/*
 * QF9700 one chip USB 1.1 ethernet devices
 *
 * Author : jokeliujl <jokeliu@163.com>
 * Date : 2010-10-01
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

//#define DEBUG

//#include <linux/module.h>
//#include <linux/sched.h>
//#include <linux/stddef.h>
//#include <linux/init.h>
//#include <linux/netdevice.h>
//#include <linux/etherdevice.h>
//#include <linux/ethtool.h>
//#include <linux/mii.h>
//#include <linux/usb.h>
//#include <linux/crc32.h>
//#include <linux/usb/usbnet.h>

#include <common.h>
#include <usb.h>
#include <linux/mii.h>
#include "usb_ether.h"

#include "qf9700.h"
/* local vars */
#define QF_BASE_NAME "qf9700"
static int curr_eth_dev; /* index for name of next device detected */

struct qf_dongle {
	unsigned short vendor;
	unsigned short product;
};
/*
 *  qf_dongle is structure and dongles is the var on the struct
 * */
static struct qf_dongle qf_dongles[] = {
	{ 0x0fe6, 0x9700 },	/* Apple USB Ethernet Adapter */
	{ 0x0000, 0x0000 }	/* END - Do not remove */
};

/* ------------------------------------------------------------------------------------------ */
/* qf9700 mac and phy operations */
/* qf9700 read some registers from MAC */

int qf_eth_probe(struct usb_device *dev, unsigned int ifnum,
		      struct ueth_data *ss)
{
	struct usb_interface *iface;
	struct usb_interface_descriptor *iface_desc;
	int i;

	/* let's examine the device now */
	iface = &dev->config.if_desc[ifnum];
	iface_desc = &dev->config.if_desc[ifnum].desc;

	for (i = 0; qf_dongles[i].vendor != 0; i++) {
		if (dev->descriptor.idVendor == qf_dongles[i].vendor &&
		    dev->descriptor.idProduct == qf_dongles[i].product)
			/* Found a supported dongle */
			break;
	}

	if (qf_dongles[i].vendor == 0)
		return 0;

	memset(ss, 0, sizeof(struct ueth_data));

	/* At this point, we know we've got a live one */
	debug("\n\nUSB Ethernet device detected: %#04x:%#04x\n",
	      dev->descriptor.idVendor, dev->descriptor.idProduct);

	/* Initialize the ueth_data structure with some useful info */
	ss->ifnum = ifnum;
	ss->pusb_dev = dev;
	ss->subclass = iface_desc->bInterfaceSubClass;
	ss->protocol = iface_desc->bInterfaceProtocol;

	/*
	 * We are expecting a minimum of 3 endpoints - in, out (bulk), and
	 * int. We will ignore any others.
	 */
	for (i = 0; i < iface_desc->bNumEndpoints; i++) {
		/* is it an BULK endpoint? */
		if ((iface->ep_desc[i].bmAttributes &
		     USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK) {
			if (iface->ep_desc[i].bEndpointAddress & USB_DIR_IN)
				ss->ep_in = iface->ep_desc[i].bEndpointAddress &
					USB_ENDPOINT_NUMBER_MASK;
			else
				ss->ep_out =
					iface->ep_desc[i].bEndpointAddress &
					USB_ENDPOINT_NUMBER_MASK;
		}

		/* is it an interrupt endpoint? */
		if ((iface->ep_desc[i].bmAttributes &
		    USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT) {
			ss->ep_int = iface->ep_desc[i].bEndpointAddress &
				USB_ENDPOINT_NUMBER_MASK;
			ss->irqinterval = iface->ep_desc[i].bInterval;
		}
	}
	debug("Endpoints In %d Out %d Int %d\n",
		  ss->ep_in, ss->ep_out, ss->ep_int);

	/* Do some basic sanity checks, and bail if we find a problem */
	if (usb_set_interface(dev, iface_desc->bInterfaceNumber, 0) ||
	    !ss->ep_in || !ss->ep_out || !ss->ep_int) {
		debug("Problems with device\n");
		return 0;
	}
	dev->privptr = (void *)ss;
	return 1;
}

static int qf_init(struct eth_device *eth, bd_t *bd)
{
	int embd_phy;
	unsigned char buf[ETH_ALEN];
	u16 rx_ctl;
	struct ueth_data	*dev = (struct ueth_data *)eth->priv;
	int timeout = 0;
#define TIMEOUT_RESOLUTION 50	/* ms */
	int link_detected;

	debug("** %s()\n", __func__);
printf("darkspr1te usb eth debug found");
/*	if (asix_write_gpio(dev,
			AX_GPIO_RSE | AX_GPIO_GPO_2 | AX_GPIO_GPO2EN, 5) < 0)
		goto out_err;
*/
	/* 0x10 is the phy id of the embedded 10/100 ethernet phy */
/*	embd_phy = ((asix_get_phy_addr(dev) & 0x1f) == 0x10 ? 1 : 0);
	if (asix_write_cmd(dev, AX_CMD_SW_PHY_SELECT,
				embd_phy, 0, 0, NULL) < 0) {
		debug("Select PHY #1 failed\n");
		goto out_err;
	}

	if (asix_sw_reset(dev, AX_SWRESET_IPPD | AX_SWRESET_PRL) < 0)
		goto out_err;

	if (asix_sw_reset(dev, AX_SWRESET_CLEAR) < 0)
		goto out_err;

	if (embd_phy) {
		if (asix_sw_reset(dev, AX_SWRESET_IPRL) < 0)
			goto out_err;
	} else {
		if (asix_sw_reset(dev, AX_SWRESET_PRTE) < 0)
			goto out_err;
	}

	rx_ctl = asix_read_rx_ctl(dev);
	debug("RX_CTL is 0x%04x after software reset\n", rx_ctl);
	if (asix_write_rx_ctl(dev, 0x0000) < 0)
		goto out_err;

	rx_ctl = asix_read_rx_ctl(dev);*/
	debug("RX_CTL is 0x%04x setting to 0x0000\n", rx_ctl);

	/* Get the MAC address */
/*	if (asix_read_cmd(dev, AX_CMD_READ_NODE_ID,
				0, 0, ETH_ALEN, buf) < 0) {
		debug("Failed to read MAC address.\n");
		goto out_err;
	}
	memcpy(eth->enetaddr, buf, ETH_ALEN);
	debug("MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
		eth->enetaddr[0], eth->enetaddr[1],
		eth->enetaddr[2], eth->enetaddr[3],
		eth->enetaddr[4], eth->enetaddr[5]);

	dev->phy_id = asix_get_phy_addr(dev);
	if (dev->phy_id < 0)
		debug("Failed to read phy id\n");

	if (asix_sw_reset(dev, AX_SWRESET_PRL) < 0)
		goto out_err;

	if (asix_sw_reset(dev, AX_SWRESET_IPRL | AX_SWRESET_PRL) < 0)
		goto out_err;

	asix_mdio_write(dev, dev->phy_id, MII_BMCR, BMCR_RESET);
	asix_mdio_write(dev, dev->phy_id, MII_ADVERTISE,
			ADVERTISE_ALL | ADVERTISE_CSMA);
	mii_nway_restart(dev);

	if (asix_write_medium_mode(dev, AX88772_MEDIUM_DEFAULT) < 0)
		goto out_err;

	if (asix_write_cmd(dev, AX_CMD_WRITE_IPG0,
				AX88772_IPG0_DEFAULT | AX88772_IPG1_DEFAULT,
				AX88772_IPG2_DEFAULT, 0, NULL) < 0) {
		debug("Write IPG,IPG1,IPG2 failed\n");
		goto out_err;
	}

	if (asix_write_rx_ctl(dev, AX_DEFAULT_RX_CTL) < 0)
		goto out_err;

	do {
		link_detected = asix_mdio_read(dev, dev->phy_id, MII_BMSR) &
			BMSR_LSTATUS;
		if (!link_detected) {
			if (timeout == 0)
				printf("Waiting for Ethernet connection... ");
			udelay(TIMEOUT_RESOLUTION * 1000);
			timeout += TIMEOUT_RESOLUTION;
		}
	} while (!link_detected && timeout < PHY_CONNECT_TIMEOUT);
	if (link_detected) {
		if (timeout != 0)
			printf("done.\n");
	} else {
		printf("unable to connect.\n");
		goto out_err;
	}
*/
	return 0;
out_err:
	return -1;
}


void qf_eth_before_probe(void)
{
	curr_eth_dev = 0;
}

int qf_eth_get_info(struct usb_device *dev, struct ueth_data *ss,
				struct eth_device *eth)
{
	if (!eth) {
		sprintf("%s: missing parameter.\n", __func__);
		return 0;
	}
	sprintf(eth->name, "%s%d", QF_BASE_NAME, curr_eth_dev++);
	eth->init = qf_init;
/*	eth->send = qf_send;
	eth->recv = qf_recv;
	eth->halt = qf_halt;*/
	eth->priv = ss;

	
	return 1;
}
