/*
   cx231xx-core.c - driver for Conexant Cx23100/101/102
				USB video capture devices

   Copyright (C) 2008 <srinivasa.deevi at conexant dot com>
				Based on em28xx driver

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/vmalloc.h>
#include <media/v4l2-common.h>

#include "cx231xx.h"
#include "cx231xx-reg.h"

/* #define ENABLE_DEBUG_ISOC_FRAMES */

static unsigned int core_debug;
module_param(core_debug, int, 0644);
MODULE_PARM_DESC(core_debug, "enable debug messages [core]");

#define cx231xx_coredbg(fmt, arg...) do {\
	if (core_debug) \
		printk(KERN_INFO "%s %s :"fmt, \
			 dev->name, __func__ , ##arg); } while (0)

static unsigned int reg_debug;
module_param(reg_debug, int, 0644);
MODULE_PARM_DESC(reg_debug, "enable debug messages [URB reg]");

#define cx231xx_regdbg(fmt, arg...) do {\
	if (reg_debug) \
		printk(KERN_INFO "%s %s :"fmt, \
			 dev->name, __func__ , ##arg); } while (0)

static int alt = CX231XX_PINOUT;
module_param(alt, int, 0644);
MODULE_PARM_DESC(alt, "alternate setting to use for video endpoint");

#define cx231xx_isocdbg(fmt, arg...) do {\
	if (core_debug) \
		printk(KERN_INFO "%s %s :"fmt, \
			 dev->name, __func__ , ##arg); } while (0)

/*****************************************************************
*             Device control list functions     				 *
******************************************************************/

static LIST_HEAD(cx231xx_devlist);
static DEFINE_MUTEX(cx231xx_devlist_mutex);

/*
 * cx231xx_realease_resources()
 * unregisters the v4l2,i2c and usb devices
 * called when the device gets disconected or at module unload
*/
void cx231xx_remove_from_devlist(struct cx231xx *dev)
{
	mutex_lock(&cx231xx_devlist_mutex);
	list_del(&dev->devlist);
	mutex_unlock(&cx231xx_devlist_mutex);
};

void cx231xx_add_into_devlist(struct cx231xx *dev)
{
	mutex_lock(&cx231xx_devlist_mutex);
	list_add_tail(&dev->devlist, &cx231xx_devlist);
	mutex_unlock(&cx231xx_devlist_mutex);
};

static LIST_HEAD(cx231xx_extension_devlist);
static DEFINE_MUTEX(cx231xx_extension_devlist_lock);

int cx231xx_register_extension(struct cx231xx_ops *ops)
{
	struct cx231xx *dev = NULL;

	mutex_lock(&cx231xx_devlist_mutex);
	mutex_lock(&cx231xx_extension_devlist_lock);
	list_add_tail(&ops->next, &cx231xx_extension_devlist);
	list_for_each_entry(dev, &cx231xx_devlist, devlist)
		ops->init(dev);

	printk(KERN_INFO DRIVER_NAME ": %s initialized\n", ops->name);
	mutex_unlock(&cx231xx_extension_devlist_lock);
	mutex_unlock(&cx231xx_devlist_mutex);
	return 0;
}
EXPORT_SYMBOL(cx231xx_register_extension);

void cx231xx_unregister_extension(struct cx231xx_ops *ops)
{
	struct cx231xx *dev = NULL;

	mutex_lock(&cx231xx_devlist_mutex);
	list_for_each_entry(dev, &cx231xx_devlist, devlist)
		ops->fini(dev);

	mutex_lock(&cx231xx_extension_devlist_lock);
	printk(KERN_INFO DRIVER_NAME ": %s removed\n", ops->name);
	list_del(&ops->next);
	mutex_unlock(&cx231xx_extension_devlist_lock);
	mutex_unlock(&cx231xx_devlist_mutex);
}
EXPORT_SYMBOL(cx231xx_unregister_extension);

void cx231xx_init_extension(struct cx231xx *dev)
{
	struct cx231xx_ops *ops = NULL;

	mutex_lock(&cx231xx_extension_devlist_lock);
	if (!list_empty(&cx231xx_extension_devlist)) {
		list_for_each_entry(ops, &cx231xx_extension_devlist, next) {
			if (ops->init)
				ops->init(dev);
		}
	}
	mutex_unlock(&cx231xx_extension_devlist_lock);
}

void cx231xx_close_extension(struct cx231xx *dev)
{
	struct cx231xx_ops *ops = NULL;

	mutex_lock(&cx231xx_extension_devlist_lock);
	if (!list_empty(&cx231xx_extension_devlist)) {
		list_for_each_entry(ops, &cx231xx_extension_devlist, next) {
			if (ops->fini)
				ops->fini(dev);
		}
	}
	mutex_unlock(&cx231xx_extension_devlist_lock);
}

/****************************************************************
*               U S B related functions                         *
*****************************************************************/
int cx231xx_send_usb_command(struct cx231xx_i2c *i2c_bus,
			     struct cx231xx_i2c_xfer_data *req_data)
{
	int status = 0;
	struct cx231xx *dev = i2c_bus->dev;
	struct VENDOR_REQUEST_IN ven_req;

	u8 saddr_len = 0;
	u8 _i2c_period = 0;
	u8 _i2c_nostop = 0;
	u8 _i2c_reserve = 0;

	/* Get the I2C period, nostop and reserve parameters */
	_i2c_period = i2c_bus->i2c_period;
	_i2c_nostop = i2c_bus->i2c_nostop;
	_i2c_reserve = i2c_bus->i2c_reserve;

	saddr_len = req_data->saddr_len;

	/* Set wValue */
	if (saddr_len == 1)	/* need check saddr_len == 0  */
		ven_req.wValue =
		    req_data->
		    dev_addr << 9 | _i2c_period << 4 | saddr_len << 2 |
		    _i2c_nostop << 1 | I2C_SYNC | _i2c_reserve << 6;
	else
		ven_req.wValue =
		    req_data->
		    dev_addr << 9 | _i2c_period << 4 | saddr_len << 2 |
		    _i2c_nostop << 1 | I2C_SYNC | _i2c_reserve << 6;

	/* set channel number */
	if (req_data->direction & I2C_M_RD) {
		/* channel number, for read,spec required channel_num +4 */
		ven_req.bRequest = i2c_bus->nr + 4;
	} else
		ven_req.bRequest = i2c_bus->nr;	/* channel number,  */

	/* set index value */
	switch (saddr_len) {
	case 0:
		ven_req.wIndex = 0;	/* need check */
		break;
	case 1:
		ven_req.wIndex = (req_data->saddr_dat & 0xff);
		break;
	case 2:
		ven_req.wIndex = req_data->saddr_dat;
		break;
	}

	/* set wLength value */
	ven_req.wLength = req_data->buf_size;

	/* set bData value */
	ven_req.bData = 0;

	/* set the direction */
	if (req_data->direction) {
		ven_req.direction = USB_DIR_IN;
		memset(req_data->p_buffer, 0x00, ven_req.wLength);
	} else
		ven_req.direction = USB_DIR_OUT;

	/* set the buffer for read / write */
	ven_req.pBuff = req_data->p_buffer;

#if 0
	{
		int i = 0;

		cx231xx_isocdbg(": USB Control Pipe Request: I2C Bus#%d\n",
				i2c_bus->nr);
		cx231xx_isocdbg("bRequest = %x  : ", ven_req.bRequest);
		cx231xx_isocdbg("wValue = %x  : ", ven_req.wValue);
		cx231xx_isocdbg("wIndex = %x  : ", ven_req.wIndex);
		cx231xx_isocdbg("wLength = %x\n", ven_req.wLength);
		cx231xx_isocdbg("pBuff : ");
		for (i = 0; i < ven_req.wLength; i++) {
			cx231xx_isocdbg(" %2x", ven_req.pBuff[i]);
			if ((i > 0) && (i % 10 == 0))
				cx231xx_isocdbg("\n        ");
		}
		cx231xx_isocdbg("\n\n");
	}
#endif

	/* call common vendor command request */
	status = cx231xx_send_vendor_cmd(dev, &ven_req);
	if (status < 0) {
		cx231xx_info
		    ("UsbInterface::sendCommand, failed with status -%d\n",
		     status);
	}

	return status;
}
EXPORT_SYMBOL_GPL(cx231xx_send_usb_command);

/*
 * cx231xx_read_ctrl_reg()
 * reads data from the usb device specifying bRequest and wValue
 */
int cx231xx_read_ctrl_reg(struct cx231xx *dev, u8 req, u16 reg,
			  char *buf, int len)
{
	u8 val = 0;
	int ret;
	int pipe = usb_rcvctrlpipe(dev->udev, 0);

	if (dev->state & DEV_DISCONNECTED)
		return -ENODEV;

	if (len > URB_MAX_CTRL_SIZE)
		return -EINVAL;

	switch (len) {
	case 1:
		val = ENABLE_ONE_BYTE;
		break;
	case 2:
		val = ENABLE_TWE_BYTE;
		break;
	case 3:
		val = ENABLE_THREE_BYTE;
		break;
	case 4:
		val = ENABLE_FOUR_BYTE;
		break;
	default:
		val = 0xFF;	/* invalid option */
	}

	if (val == 0xFF)
		return -EINVAL;

	if (reg_debug) {
		cx231xx_isocdbg("(pipe 0x%08x): "
				"IN:  %02x %02x %02x %02x %02x %02x %02x %02x ",
				pipe,
				USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
				req, 0, val,
				reg & 0xff, reg >> 8, len & 0xff, len >> 8);
	}

	mutex_lock(&dev->ctrl_urb_lock);
	ret = usb_control_msg(dev->udev, pipe, req,
			      USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			      val, reg, dev->urb_buf, len, HZ);
	if (ret < 0) {
		cx231xx_isocdbg(" failed!\n");
		/* mutex_unlock(&dev->ctrl_urb_lock); */
		return ret;
	}

	if (len)
		memcpy(buf, dev->urb_buf, len);

	mutex_unlock(&dev->ctrl_urb_lock);

	if (reg_debug) {
		int byte;

		cx231xx_isocdbg("<<<");
		for (byte = 0; byte < len; byte++)
			cx231xx_isocdbg(" %02x", (unsigned char)buf[byte]);
		cx231xx_isocdbg("\n");
	}

	return ret;
}

int cx231xx_send_vendor_cmd(struct cx231xx *dev,
				struct VENDOR_REQUEST_IN *ven_req)
{
	int ret;
	int pipe = 0;

	if (dev->state & DEV_DISCONNECTED)
		return -ENODEV;

	if ((ven_req->wLength > URB_MAX_CTRL_SIZE))
		return -EINVAL;

	if (ven_req->direction)
		pipe = usb_rcvctrlpipe(dev->udev, 0);
	else
		pipe = usb_sndctrlpipe(dev->udev, 0);

	if (reg_debug) {
		int byte;

		cx231xx_isocdbg("(pipe 0x%08x): "
				"OUT: %02x %02x %02x %04x %04x %04x >>>",
				pipe,
				ven_req->
				direction | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
				ven_req->bRequest, 0, ven_req->wValue,
				ven_req->wIndex, ven_req->wLength);

		for (byte = 0; byte < ven_req->wLength; byte++)
			cx231xx_isocdbg(" %02x",
					(unsigned char)ven_req->pBuff[byte]);
		cx231xx_isocdbg("\n");
	}

	mutex_lock(&dev->ctrl_urb_lock);
	ret = usb_control_msg(dev->udev, pipe, ven_req->bRequest,
			      ven_req->
			      direction | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			      ven_req->wValue, ven_req->wIndex, ven_req->pBuff,
			      ven_req->wLength, HZ);
	mutex_unlock(&dev->ctrl_urb_lock);

	return ret;
}

/*
 * cx231xx_write_ctrl_reg()
 * sends data to the usb device, specifying bRequest
 */
int cx231xx_write_ctrl_reg(struct cx231xx *dev, u8 req, u16 reg, char *buf,
			   int len)
{
	u8 val = 0;
	int ret;
	int pipe = usb_sndctrlpipe(dev->udev, 0);

	if (dev->state & DEV_DISCONNECTED)
		return -ENODEV;

	if ((len < 1) || (len > URB_MAX_CTRL_SIZE))
		return -EINVAL;

	switch (len) {
	case 1:
		val = ENABLE_ONE_BYTE;
		break;
	case 2:
		val = ENABLE_TWE_BYTE;
		break;
	case 3:
		val = ENABLE_THREE_BYTE;
		break;
	case 4:
		val = ENABLE_FOUR_BYTE;
		break;
	default:
		val = 0xFF;	/* invalid option */
	}

	if (val == 0xFF)
		return -EINVAL;

	if (reg_debug) {
		int byte;

		cx231xx_isocdbg("(pipe 0x%08x): "
			"OUT: %02x %02x %02x %02x %02x %02x %02x %02x >>>",
			pipe,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			req, 0, val, reg & 0xff,
			reg >> 8, len & 0xff, len >> 8);

		for (byte = 0; byte < len; byte++)
			cx231xx_isocdbg(" %02x", (unsigned char)buf[byte]);
		cx231xx_isocdbg("\n");
	}

	mutex_lock(&dev->ctrl_urb_lock);
	memcpy(dev->urb_buf, buf, len);
	ret = usb_control_msg(dev->udev, pipe, req,
			      USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			      val, reg, dev->urb_buf, len, HZ);
	mutex_unlock(&dev->ctrl_urb_lock);

	return ret;
}

/****************************************************************
*           USB Alternate Setting functions                     *
*****************************************************************/

int cx231xx_set_video_alternate(struct cx231xx *dev)
{
	int errCode, prev_alt = dev->video_mode.alt;
	unsigned int min_pkt_size = dev->width * 2 + 4;
	u32 usb_interface_index = 0;

	/* When image size is bigger than a certain value,
	   the frame size should be increased, otherwise, only
	   green screen will be received.
	 */
	if (dev->width * 2 * dev->height > 720 * 240 * 2)
		min_pkt_size *= 2;

	if (dev->width > 360) {
		/* resolutions: 720,704,640 */
		dev->video_mode.alt = 3;
	} else if (dev->width > 180) {
		/* resolutions: 360,352,320,240 */
		dev->video_mode.alt = 2;
	} else if (dev->width > 0) {
		/* resolutions: 180,176,160,128,88 */
		dev->video_mode.alt = 1;
	} else {
		/* Change to alt0 BULK to release USB bandwidth */
		dev->video_mode.alt = 0;
	}

	/* Get the correct video interface Index */
	usb_interface_index =
	    dev->current_pcb_config.hs_config_info[0].interface_info.
	    video_index + 1;

	if (dev->video_mode.alt != prev_alt) {
		cx231xx_coredbg("minimum isoc packet size: %u (alt=%d)\n",
				min_pkt_size, dev->video_mode.alt);
		dev->video_mode.max_pkt_size =
		    dev->video_mode.alt_max_pkt_size[dev->video_mode.alt];
		cx231xx_coredbg("setting alternate %d with wMaxPacketSize=%u\n",
				dev->video_mode.alt,
				dev->video_mode.max_pkt_size);
		cx231xx_info
		    (" setting alt %d with wMaxPktSize=%u , Interface = %d\n",
		     dev->video_mode.alt, dev->video_mode.max_pkt_size,
		     usb_interface_index);
		errCode =
		    usb_set_interface(dev->udev, usb_interface_index,
				      dev->video_mode.alt);
		if (errCode < 0) {
			cx231xx_errdev
			    ("cannot change alt number to %d (error=%i)\n",
			     dev->video_mode.alt, errCode);
			return errCode;
		}
	}
	return 0;
}

int cx231xx_set_alt_setting(struct cx231xx *dev, u8 index, u8 alt)
{
	int status = 0;
	u32 usb_interface_index = 0;
	u32 max_pkt_size = 0;

	switch (index) {
	case INDEX_TS1:
		usb_interface_index =
		    dev->current_pcb_config.hs_config_info[0].interface_info.
		    ts1_index + 1;
		dev->video_mode.alt = alt;
		if (dev->ts1_mode.alt_max_pkt_size != NULL)
			max_pkt_size = dev->ts1_mode.max_pkt_size =
			    dev->ts1_mode.alt_max_pkt_size[dev->ts1_mode.alt];
		break;
	case INDEX_TS2:
		usb_interface_index =
		    dev->current_pcb_config.hs_config_info[0].interface_info.
		    ts2_index + 1;
		break;
	case INDEX_AUDIO:
		usb_interface_index =
		    dev->current_pcb_config.hs_config_info[0].interface_info.
		    audio_index + 1;
		dev->adev.alt = alt;
		if (dev->adev.alt_max_pkt_size != NULL)
			max_pkt_size = dev->adev.max_pkt_size =
			    dev->adev.alt_max_pkt_size[dev->adev.alt];
		break;
	case INDEX_VIDEO:
		usb_interface_index =
		    dev->current_pcb_config.hs_config_info[0].interface_info.
		    video_index + 1;
		dev->video_mode.alt = alt;
		if (dev->video_mode.alt_max_pkt_size != NULL)
			max_pkt_size = dev->video_mode.max_pkt_size =
			    dev->video_mode.alt_max_pkt_size[dev->video_mode.
							     alt];
		break;
	case INDEX_VANC:
		usb_interface_index =
		    dev->current_pcb_config.hs_config_info[0].interface_info.
		    vanc_index + 1;
		dev->vbi_mode.alt = alt;
		if (dev->vbi_mode.alt_max_pkt_size != NULL)
			max_pkt_size = dev->vbi_mode.max_pkt_size =
			    dev->vbi_mode.alt_max_pkt_size[dev->vbi_mode.alt];
		break;
	case INDEX_HANC:
		usb_interface_index =
		    dev->current_pcb_config.hs_config_info[0].interface_info.
		    hanc_index + 1;
		dev->sliced_cc_mode.alt = alt;
		if (dev->sliced_cc_mode.alt_max_pkt_size != NULL)
			max_pkt_size = dev->sliced_cc_mode.max_pkt_size =
			    dev->sliced_cc_mode.alt_max_pkt_size[dev->
								 sliced_cc_mode.
								 alt];
		break;
	default:
		break;
	}

	if (alt > 0 && max_pkt_size == 0) {
		cx231xx_errdev
		("can't change interface %d alt no. to %d: Max. Pkt size = 0\n",
		usb_interface_index, alt);
		return -1;
	}

	cx231xx_info
	    (" setting alternate %d with wMaxPacketSize=%u , Interface = %d\n",
	     alt, max_pkt_size, usb_interface_index);

	if (usb_interface_index > 0) {
		status = usb_set_interface(dev->udev, usb_interface_index, alt);
		if (status < 0) {
			cx231xx_errdev
			("can't change interface %d alt no. to %d (err=%i)\n",
			usb_interface_index, alt, status);
			return status;
		}
	}

	return status;
}
EXPORT_SYMBOL_GPL(cx231xx_set_alt_setting);

int cx231xx_gpio_set(struct cx231xx *dev, struct cx231xx_reg_seq *gpio)
{
	int rc = 0;

	if (!gpio)
		return rc;

	/* Send GPIO reset sequences specified at board entry */
	while (gpio->sleep >= 0) {
		rc = cx231xx_set_gpio_value(dev, gpio->bit, gpio->val);
		if (rc < 0)
			return rc;

		if (gpio->sleep > 0)
			msleep(gpio->sleep);

		gpio++;
	}
	return rc;
}

int cx231xx_set_mode(struct cx231xx *dev, enum cx231xx_mode set_mode)
{
	if (dev->mode == set_mode)
		return 0;

	if (set_mode == CX231XX_SUSPEND) {
		/* Set the chip in power saving mode */
		dev->mode = set_mode;
	}

	/* Resource is locked */
	if (dev->mode != CX231XX_SUSPEND)
		return -EINVAL;

	dev->mode = set_mode;

	if (dev->mode == CX231XX_DIGITAL_MODE)
		;/* Set Digital power mode */
	else
		;/* Set Analog Power mode */

	return 0;
}
EXPORT_SYMBOL_GPL(cx231xx_set_mode);

/*****************************************************************
*                URB Streaming functions                         *
******************************************************************/

/*
 * IRQ callback, called by URB callback
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)
static void cx231xx_irq_callback(struct urb *urb, struct pt_regs *regs)
#else
static void cx231xx_irq_callback(struct urb *urb)
#endif
{
	struct cx231xx_dmaqueue *dma_q = urb->context;
	struct cx231xx_video_mode *vmode =
	    container_of(dma_q, struct cx231xx_video_mode, vidq);
	struct cx231xx *dev = container_of(vmode, struct cx231xx, video_mode);
	int rc, i;

	switch (urb->status) {
	case 0:		/* success */
	case -ETIMEDOUT:	/* NAK */
		break;
	case -ECONNRESET:	/* kill */
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	default:		/* error */
		cx231xx_isocdbg("urb completition error %d.\n", urb->status);
		break;
	}

	/* Copy data from URB */
	spin_lock(&dev->video_mode.slock);
	rc = dev->video_mode.isoc_ctl.isoc_copy(dev, urb);
	spin_unlock(&dev->video_mode.slock);

	/* Reset urb buffers */
	for (i = 0; i < urb->number_of_packets; i++) {
		urb->iso_frame_desc[i].status = 0;
		urb->iso_frame_desc[i].actual_length = 0;
	}
	urb->status = 0;

	urb->status = usb_submit_urb(urb, GFP_ATOMIC);
	if (urb->status) {
		cx231xx_isocdbg("urb resubmit failed (error=%i)\n",
				urb->status);
	}
}

/*
 * Stop and Deallocate URBs
 */
void cx231xx_uninit_isoc(struct cx231xx *dev)
{
	struct urb *urb;
	int i;

	cx231xx_isocdbg("cx231xx: called cx231xx_uninit_isoc\n");

	dev->video_mode.isoc_ctl.nfields = -1;
	for (i = 0; i < dev->video_mode.isoc_ctl.num_bufs; i++) {
		urb = dev->video_mode.isoc_ctl.urb[i];
		if (urb) {
			if (!irqs_disabled())
				usb_kill_urb(urb);
			else
				usb_unlink_urb(urb);

			if (dev->video_mode.isoc_ctl.transfer_buffer[i]) {
				usb_buffer_free(dev->udev,
						urb->transfer_buffer_length,
						dev->video_mode.isoc_ctl.
						transfer_buffer[i],
						urb->transfer_dma);
			}
			usb_free_urb(urb);
			dev->video_mode.isoc_ctl.urb[i] = NULL;
		}
		dev->video_mode.isoc_ctl.transfer_buffer[i] = NULL;
	}

	kfree(dev->video_mode.isoc_ctl.urb);
	kfree(dev->video_mode.isoc_ctl.transfer_buffer);

	dev->video_mode.isoc_ctl.urb = NULL;
	dev->video_mode.isoc_ctl.transfer_buffer = NULL;
	dev->video_mode.isoc_ctl.num_bufs = 0;

	cx231xx_capture_start(dev, 0, Raw_Video);
}
EXPORT_SYMBOL_GPL(cx231xx_uninit_isoc);

/*
 * Allocate URBs and start IRQ
 */
int cx231xx_init_isoc(struct cx231xx *dev, int max_packets,
		      int num_bufs, int max_pkt_size,
		      int (*isoc_copy) (struct cx231xx *dev, struct urb *urb))
{
	struct cx231xx_dmaqueue *dma_q = &dev->video_mode.vidq;
	int i;
	int sb_size, pipe;
	struct urb *urb;
	int j, k;
	int rc;

	cx231xx_isocdbg("cx231xx: called cx231xx_prepare_isoc\n");

	dev->video_input = dev->video_input > 2 ? 2 : dev->video_input;

	cx231xx_info("Setting Video mux to %d\n", dev->video_input);
	video_mux(dev, dev->video_input);

	/* De-allocates all pending stuff */
	cx231xx_uninit_isoc(dev);

	dev->video_mode.isoc_ctl.isoc_copy = isoc_copy;
	dev->video_mode.isoc_ctl.num_bufs = num_bufs;
	dma_q->pos = 0;
	dma_q->is_partial_line = 0;
	dma_q->last_sav = 0;
	dma_q->current_field = -1;
	dma_q->field1_done = 0;
	dma_q->lines_per_field = dev->height / 2;
	dma_q->bytes_left_in_line = dev->width << 1;
	dma_q->lines_completed = 0;
	for (i = 0; i < 8; i++)
		dma_q->partial_buf[i] = 0;

	dev->video_mode.isoc_ctl.urb =
	    kzalloc(sizeof(void *) * num_bufs, GFP_KERNEL);
	if (!dev->video_mode.isoc_ctl.urb) {
		cx231xx_errdev("cannot alloc memory for usb buffers\n");
		return -ENOMEM;
	}

	dev->video_mode.isoc_ctl.transfer_buffer =
	    kzalloc(sizeof(void *) * num_bufs, GFP_KERNEL);
	if (!dev->video_mode.isoc_ctl.transfer_buffer) {
		cx231xx_errdev("cannot allocate memory for usbtransfer\n");
		kfree(dev->video_mode.isoc_ctl.urb);
		return -ENOMEM;
	}

	dev->video_mode.isoc_ctl.max_pkt_size = max_pkt_size;
	dev->video_mode.isoc_ctl.buf = NULL;

	sb_size = max_packets * dev->video_mode.isoc_ctl.max_pkt_size;

	/* allocate urbs and transfer buffers */
	for (i = 0; i < dev->video_mode.isoc_ctl.num_bufs; i++) {
		urb = usb_alloc_urb(max_packets, GFP_KERNEL);
		if (!urb) {
			cx231xx_err("cannot alloc isoc_ctl.urb %i\n", i);
			cx231xx_uninit_isoc(dev);
			return -ENOMEM;
		}
		dev->video_mode.isoc_ctl.urb[i] = urb;

		dev->video_mode.isoc_ctl.transfer_buffer[i] =
		    usb_buffer_alloc(dev->udev, sb_size, GFP_KERNEL,
				     &urb->transfer_dma);
		if (!dev->video_mode.isoc_ctl.transfer_buffer[i]) {
			cx231xx_err("unable to allocate %i bytes for transfer"
				    " buffer %i%s\n",
				    sb_size, i,
				    in_interrupt() ? " while in int" : "");
			cx231xx_uninit_isoc(dev);
			return -ENOMEM;
		}
		memset(dev->video_mode.isoc_ctl.transfer_buffer[i], 0, sb_size);

		pipe =
		    usb_rcvisocpipe(dev->udev, dev->video_mode.end_point_addr);

		usb_fill_int_urb(urb, dev->udev, pipe,
				 dev->video_mode.isoc_ctl.transfer_buffer[i],
				 sb_size, cx231xx_irq_callback, dma_q, 1);

		urb->number_of_packets = max_packets;
		urb->transfer_flags = URB_ISO_ASAP;

		k = 0;
		for (j = 0; j < max_packets; j++) {
			urb->iso_frame_desc[j].offset = k;
			urb->iso_frame_desc[j].length =
			    dev->video_mode.isoc_ctl.max_pkt_size;
			k += dev->video_mode.isoc_ctl.max_pkt_size;
		}
	}

	init_waitqueue_head(&dma_q->wq);

	/* submit urbs and enables IRQ */
	for (i = 0; i < dev->video_mode.isoc_ctl.num_bufs; i++) {
		rc = usb_submit_urb(dev->video_mode.isoc_ctl.urb[i],
				    GFP_ATOMIC);
		if (rc) {
			cx231xx_err("submit of urb %i failed (error=%i)\n", i,
				    rc);
			cx231xx_uninit_isoc(dev);
			return rc;
		}
	}

	cx231xx_capture_start(dev, 1, Raw_Video);

	return 0;
}
EXPORT_SYMBOL_GPL(cx231xx_init_isoc);

/*****************************************************************
*             Device Init/UnInit functions                       *
******************************************************************/
int cx231xx_dev_init(struct cx231xx *dev)
{
	int errCode = 0;

	/* Initialize I2C bus */

	/* External Master 1 Bus */
	dev->i2c_bus[0].nr = 0;
	dev->i2c_bus[0].dev = dev;
	dev->i2c_bus[0].i2c_period = I2C_SPEED_1M;	/* 1MHz */
	dev->i2c_bus[0].i2c_nostop = 0;
	dev->i2c_bus[0].i2c_reserve = 0;

	/* External Master 2 Bus */
	dev->i2c_bus[1].nr = 1;
	dev->i2c_bus[1].dev = dev;
	dev->i2c_bus[1].i2c_period = I2C_SPEED_1M;	/* 1MHz */
	dev->i2c_bus[1].i2c_nostop = 0;
	dev->i2c_bus[1].i2c_reserve = 0;

	/* Internal Master 3 Bus */
	dev->i2c_bus[2].nr = 2;
	dev->i2c_bus[2].dev = dev;
	dev->i2c_bus[2].i2c_period = I2C_SPEED_400K;	/* 400kHz */
	dev->i2c_bus[2].i2c_nostop = 0;
	dev->i2c_bus[2].i2c_reserve = 0;

	/* register I2C buses */
	cx231xx_i2c_register(&dev->i2c_bus[0]);
	cx231xx_i2c_register(&dev->i2c_bus[1]);
	cx231xx_i2c_register(&dev->i2c_bus[2]);

	/* init hardware */
	/* Note : with out calling set power mode function,
	afe can not be set up correctly */
	errCode = cx231xx_set_power_mode(dev, POLARIS_AVMODE_ANALOGT_TV);
	if (errCode < 0) {
		cx231xx_errdev
		    ("%s: Failed to set Power - errCode [%d]!\n",
		     __func__, errCode);
		return errCode;
	}

	/* initialize Colibri block */
	errCode = cx231xx_afe_init_super_block(dev, 0x23c);
	if (errCode < 0) {
		cx231xx_errdev
		    ("%s: cx231xx_afe init super block - errCode [%d]!\n",
		     __func__, errCode);
		return errCode;
	}
	errCode = cx231xx_afe_init_channels(dev);
	if (errCode < 0) {
		cx231xx_errdev
		    ("%s: cx231xx_afe init channels - errCode [%d]!\n",
		     __func__, errCode);
		return errCode;
	}

	/* Set DIF in By pass mode */
	errCode = cx231xx_dif_set_standard(dev, DIF_USE_BASEBAND);
	if (errCode < 0) {
		cx231xx_errdev
		    ("%s: cx231xx_dif set to By pass mode - errCode [%d]!\n",
		     __func__, errCode);
		return errCode;
	}

	/* I2S block related functions */
	errCode = cx231xx_i2s_blk_initialize(dev);
	if (errCode < 0) {
		cx231xx_errdev
		    ("%s: cx231xx_i2s block initialize - errCode [%d]!\n",
		     __func__, errCode);
		return errCode;
	}

	/* init control pins */
	errCode = cx231xx_init_ctrl_pin_status(dev);
	if (errCode < 0) {
		cx231xx_errdev("%s: cx231xx_init ctrl pins - errCode [%d]!\n",
			       __func__, errCode);
		return errCode;
	}

	/* set AGC mode to Analog */
	errCode = cx231xx_set_agc_analog_digital_mux_select(dev, 1);
	if (errCode < 0) {
		cx231xx_errdev
		    ("%s: cx231xx_AGC mode to Analog - errCode [%d]!\n",
		     __func__, errCode);
		return errCode;
	}

	/* set all alternate settings to zero initially */
	cx231xx_set_alt_setting(dev, INDEX_VIDEO, 0);
	cx231xx_set_alt_setting(dev, INDEX_VANC, 0);
	cx231xx_set_alt_setting(dev, INDEX_HANC, 0);
	if (dev->board.has_dvb)
		cx231xx_set_alt_setting(dev, INDEX_TS1, 0);

	/* set the I2C master port to 3 on channel 1 */
	errCode = cx231xx_enable_i2c_for_tuner(dev, I2C_3);

	return errCode;
}
EXPORT_SYMBOL_GPL(cx231xx_dev_init);

void cx231xx_dev_uninit(struct cx231xx *dev)
{
	/* Un Initialize I2C bus */
	cx231xx_i2c_unregister(&dev->i2c_bus[2]);
	cx231xx_i2c_unregister(&dev->i2c_bus[1]);
	cx231xx_i2c_unregister(&dev->i2c_bus[0]);
}
EXPORT_SYMBOL_GPL(cx231xx_dev_uninit);

/*****************************************************************
*              G P I O related functions                         *
******************************************************************/
int cx231xx_send_gpio_cmd(struct cx231xx *dev, u32 gpio_bit, u8 * gpio_val,
			  u8 len, u8 request, u8 direction)
{
	int status = 0;
	struct VENDOR_REQUEST_IN ven_req;

	/* Set wValue */
	ven_req.wValue = (u16) (gpio_bit >> 16 & 0xffff);

	/* set request */
	if (!request) {
		if (direction)
			ven_req.bRequest = VRT_GET_GPIO;	/* 0x8 gpio */
		else
			ven_req.bRequest = VRT_SET_GPIO;	/* 0x9 gpio */
	} else {
		if (direction)
			ven_req.bRequest = VRT_GET_GPIE;	/* 0xa gpie */
		else
			ven_req.bRequest = VRT_SET_GPIE;	/* 0xb gpie */
	}

	/* set index value */
	ven_req.wIndex = (u16) (gpio_bit & 0xffff);

	/* set wLength value */
	ven_req.wLength = len;

	/* set bData value */
	ven_req.bData = 0;

	/* set the buffer for read / write */
	ven_req.pBuff = gpio_val;

	/* set the direction */
	if (direction) {
		ven_req.direction = USB_DIR_IN;
		memset(ven_req.pBuff, 0x00, ven_req.wLength);
	} else
		ven_req.direction = USB_DIR_OUT;

#if 0
	cx231xx_isocdbg("bRequest = %x\n", ven_req.bRequest);
	cx231xx_isocdbg("wValue = %x\n", ven_req.wValue);
	cx231xx_isocdbg("wIndex = %x\n", ven_req.wIndex);
	cx231xx_isocdbg("wLength = %x\n", ven_req.wLength);
	for (int i = 0; i < ven_req.wLength; i++)
		cx231xx_isocdbg("pBuff[i] = %x\n", ven_req.pBuff[i]);
#endif

	/* call common vendor command request */
	status = cx231xx_send_vendor_cmd(dev, &ven_req);
	if (status < 0) {
		cx231xx_info
		    ("UsbInterface::sendCommand, failed with status -%d\n",
		     status);
	}

	return status;
}
EXPORT_SYMBOL_GPL(cx231xx_send_gpio_cmd);

/*****************************************************************
 *    C O N T R O L - Register R E A D / W R I T E functions     *
 *****************************************************************/
int cx231xx_mode_register(struct cx231xx *dev, u16 address, u32 mode)
{
	u8 value[4] = { 0x0, 0x0, 0x0, 0x0 };
	u32 tmp = 0;
	int status = 0;

	status =
	    cx231xx_read_ctrl_reg(dev, VRT_GET_REGISTER, address, value, 4);
	if (status < 0)
		return status;

	tmp = *((u32 *) value);
	tmp |= mode;

	value[0] = (u8) tmp;
	value[1] = (u8) (tmp >> 8);
	value[2] = (u8) (tmp >> 16);
	value[3] = (u8) (tmp >> 24);

	status =
	    cx231xx_write_ctrl_reg(dev, VRT_SET_REGISTER, address, value, 4);

	return status;
}

/*****************************************************************
 *            I 2 C Internal C O N T R O L   functions           *
 *****************************************************************/
int cx231xx_read_i2c_data(struct cx231xx *dev, u8 dev_addr, u16 saddr,
			  u8 saddr_len, u32 *data, u8 data_len)
{
	int status = 0;
	struct cx231xx_i2c_xfer_data req_data;
	u8 value[4] = { 0, 0, 0, 0 };

	if (saddr_len == 0)
		saddr = 0;
	else if (saddr_len == 0)
		saddr &= 0xff;

	/* prepare xfer_data struct */
	req_data.dev_addr = dev_addr >> 1;
	req_data.direction = I2C_M_RD;
	req_data.saddr_len = saddr_len;
	req_data.saddr_dat = saddr;
	req_data.buf_size = data_len;
	req_data.p_buffer = (u8 *) value;

	/* usb send command */
	status = dev->cx231xx_send_usb_command(&dev->i2c_bus[0], &req_data);

	if (status >= 0) {
		/* Copy the data read back to main buffer */
		if (data_len == 1)
			*data = value[0];
		else
			*data =
			    value[0] | value[1] << 8 | value[2] << 16 | value[3]
			    << 24;
	}

	return status;
}

int cx231xx_write_i2c_data(struct cx231xx *dev, u8 dev_addr, u16 saddr,
			   u8 saddr_len, u32 data, u8 data_len)
{
	int status = 0;
	u8 value[4] = { 0, 0, 0, 0 };
	struct cx231xx_i2c_xfer_data req_data;

	value[0] = (u8) data;
	value[1] = (u8) (data >> 8);
	value[2] = (u8) (data >> 16);
	value[3] = (u8) (data >> 24);

	if (saddr_len == 0)
		saddr = 0;
	else if (saddr_len == 0)
		saddr &= 0xff;

	/* prepare xfer_data struct */
	req_data.dev_addr = dev_addr >> 1;
	req_data.direction = 0;
	req_data.saddr_len = saddr_len;
	req_data.saddr_dat = saddr;
	req_data.buf_size = data_len;
	req_data.p_buffer = value;

	/* usb send command */
	status = dev->cx231xx_send_usb_command(&dev->i2c_bus[0], &req_data);

	return status;
}

int cx231xx_reg_mask_write(struct cx231xx *dev, u8 dev_addr, u8 size,
			   u16 register_address, u8 bit_start, u8 bit_end,
			   u32 value)
{
	int status = 0;
	u32 tmp;
	u32 mask = 0;
	int i;

	if (bit_start > (size - 1) || bit_end > (size - 1))
		return -1;

	if (size == 8) {
		status =
		    cx231xx_read_i2c_data(dev, dev_addr, register_address, 2,
					  &tmp, 1);
	} else {
		status =
		    cx231xx_read_i2c_data(dev, dev_addr, register_address, 2,
					  &tmp, 4);
	}

	if (status < 0)
		return status;

	mask = 1 << bit_end;
	for (i = bit_end; i > bit_start && i > 0; i--)
		mask = mask + (1 << (i - 1));

	value <<= bit_start;

	if (size == 8) {
		tmp &= ~mask;
		tmp |= value;
		tmp &= 0xff;
		status =
		    cx231xx_write_i2c_data(dev, dev_addr, register_address, 2,
					   tmp, 1);
	} else {
		tmp &= ~mask;
		tmp |= value;
		status =
		    cx231xx_write_i2c_data(dev, dev_addr, register_address, 2,
					   tmp, 4);
	}

	return status;
}

int cx231xx_read_modify_write_i2c_dword(struct cx231xx *dev, u8 dev_addr,
					u16 saddr, u32 mask, u32 value)
{
	u32 temp;
	int status = 0;

	status = cx231xx_read_i2c_data(dev, dev_addr, saddr, 2, &temp, 4);

	if (status < 0)
		return status;

	temp &= ~mask;
	temp |= value;

	status = cx231xx_write_i2c_data(dev, dev_addr, saddr, 2, temp, 4);

	return status;
}

u32 cx231xx_set_field(u32 field_mask, u32 data)
{
	u32 temp;

	for (temp = field_mask; (temp & 1) == 0; temp >>= 1)
		data <<= 1;

	return data;
}
