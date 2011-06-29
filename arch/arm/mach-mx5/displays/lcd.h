/*
 * arch/arm/mach-mx5/displays/lcd.h
 *
 * Copyright (C) 2010 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#ifndef __ASM_ARCH_MXC_CCWMX51_DISPLAYS_LCD_H__
#define __ASM_ARCH_MXC_CCWMX51_DISPLAYS_LCD_H__

#if defined(CONFIG_MODULE_CCXMX51)
#include "../iomux.h"
#include "../mx51_pins.h"
#endif

static void lcd_bl_enable(int enable, int vif)
{
#if defined(CONFIG_MODULE_CCXMX51)
	gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_DI1_PIN11), !enable);
	gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_DI1_PIN12), !enable);
	if (vif == 0)
		gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_DI1_PIN11), !enable);
#if defined(CONFIG_JSCCWMX51_V2)
	else if (vif == 1)
		gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_DI1_PIN12), !enable);
#endif
#elif defined(CONFIG_MODULE_CCXMX53)
#define DISP1_ENABLE_PAD	MX53_PAD_DI0_PIN4__GPIO_4_20
#define DISP1_ENABLE_GPIO	(3*32 + 20)
	gpio_set_value(DISP1_ENABLE_GPIO, !enable);
#endif /* CONFIG_MODULE_CCXMX51 */
}


static void lcd_init(int vif)
{
	/* Initialize lcd enable gpio and video interface lines */
	gpio_video_active(vif,
#if defined(CONFIG_MODULE_CCXMX51)
			 PAD_CTL_DRV_HIGH | PAD_CTL_SRE_FAST);
#else
			0);
#endif
}

static struct fb_videomode lq70y3dg3b = {
	.name          = "LQ070Y3DG3B",
	.refresh       = 60,
	.xres          = 800,
	.yres          = 480,
	.pixclock	= 44000,
	.left_margin	= 0,
	.right_margin	= 50,
	.upper_margin  = 25,
	.lower_margin  = 10,
	.hsync_len     = 128,
	.vsync_len     = 10,
	.vmode         = FB_VMODE_NONINTERLACED,
	.sync          = FB_SYNC_EXT,
	.flag          = 0,
};

static struct fb_videomode lq121k1lg52 = {
	.name          = "LQ121K1LG52",
	.refresh       = 60,
	.xres          = 1280,
	.yres          = 800,
	.pixclock      = 30200 /* 44.3333Mhz in ps  */,
	.left_margin   = 10,
	.right_margin  = 370,
	.upper_margin  = 10,
	.lower_margin  = 10,
	.hsync_len     = 10,
	.vsync_len     = 10,
	.vmode         = FB_VMODE_NONINTERLACED,
	.sync          = FB_SYNC_CLK_LAT_FALL,
	.flag          = 0,
};

static struct fb_videomode lcd_custom_1 = {
	.name		= "custom1",
	.refresh	= 0,			/* Refresh rate in Hz */
	.xres		= 0,			/* Resolution in the x axis */
	.yres		= 0,			/* Reslution in the y axis */
	.pixclock	= 0,			/* Pixelclock/dotclock,picoseconds. */
	.left_margin	= 0,			/* Horizontal Back Porch (HBP) */
	.right_margin	= 0,			/* Horizontal Front Porch (HFP) */
	.upper_margin	= 0,			/* Vertical Back Porch (VBP) */
	.lower_margin	= 0,			/* Vetical Front Porch (VFP) */
	.hsync_len	= 0,			/* Horizontal sync pulse width */
	.vsync_len	= 0,			/* Vertical sync pulse width */
	.vmode		= FB_VMODE_NONINTERLACED,/* Video mode */
	.sync		= FB_SYNC_EXT,		/* Data clock polarity */
};

static struct fb_videomode lcd_custom_2 = {
	.name		= "custom2",
	.refresh	= 0,			/* Refresh rate in Hz */
	.xres		= 0,			/* Resolution in the x axis */
	.yres		= 0,			/* Reslution in the y axis */
	.pixclock	= 0,			/* Pixelclock or dotclock,picoseconds.*/
	.left_margin	= 0,			/* Horizontal Back Porch (HBP) */
	.right_margin	= 0,			/* Horizontal Front Porch (HFP) */
	.upper_margin	= 0,			/* Vertical Back Porch (VBP) */
	.lower_margin	= 0,			/* Vetical Front Porch (VFP) */
	.hsync_len	= 0,			/* Horizontal sync pulse width */
	.vsync_len	= 0,			/* Vertical sync pulse width */
	.vmode		= FB_VMODE_NONINTERLACED,/* Video mode */
	.sync		= FB_SYNC_EXT,		/* Data clock polarity */
};

struct ccwmx5x_lcd_pdata lcd_panel_list[] = {
	{
		.fb_pdata = {
			.interface_pix_fmt = VIDEO_PIX_FMT,
			.mode_str = "LQ070Y3DG3B",
			.mode = &lq70y3dg3b,
		},
		.bl_enable = lcd_bl_enable,
		.init = &lcd_init,
	}, {
		.fb_pdata = {
			.interface_pix_fmt = VIDEO_PIX_FMT,
			.mode_str = "LQ121K1LG52",
			.mode = &lq121k1lg52,
		},
		.bl_enable = lcd_bl_enable,
		.init = &lcd_init,
	}, {
		.fb_pdata = {
			.interface_pix_fmt = VIDEO_PIX_FMT,
			.mode_str = "custom1",
			.mode = &lcd_custom_1,
		},
		.bl_enable = NULL,
		.init = &lcd_init,
	}, {
		.fb_pdata = {
			.interface_pix_fmt = VIDEO_PIX_FMT,
			.mode_str = "custom2",
			.mode = &lcd_custom_2,
		},
		.bl_enable = NULL,
		.init = &lcd_init,
	},
};
#endif /* __ASM_ARCH_MXC_CCWMX51_DISPLAYS_LCD_H__ */

