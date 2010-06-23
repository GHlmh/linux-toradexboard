/*
 * Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2009-2010 Digi International, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <mach/hardware.h>
#include <mach/gpio.h>

#include "iomux.h"
#include "mx51_pins.h"

static void ccwmx51_mmc2_gpio_active(void);


/**
 * iomux settings for the external ethernet mac
 */
#if defined(CONFIG_SMSC911X) || defined(CONFIG_SMSC911X_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_ext_eth_pins[] = {
	{MX51_PIN_EIM_CS5, IOMUX_CONFIG_ALT0,
			(PAD_CTL_HYS_ENABLE | PAD_CTL_PUE_KEEPER | PAD_CTL_DRV_MEDIUM), },
	{MX51_PIN_EIM_OE, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA0, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA1, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA2, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA3, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA4, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA5, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA6, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_DA7, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D16, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D17, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D18, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D19, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D20, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D21, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D22, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D23, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D24, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D25, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D26, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D27, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D28, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D29, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D30, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_EIM_D31, IOMUX_CONFIG_ALT0,},
	{MX51_PIN_GPIO1_9, IOMUX_CONFIG_ALT0, (PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PU), },
};
#endif

#if defined(CONFIG_MMC_IMX_ESDHCI) || defined(CONFIG_MMC_IMX_ESDHCI_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_mmc_pins[] = {
	/* SDHC1*/
	{
		MX51_PIN_SD1_CMD, IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD1_CLK, IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD1_DATA0, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD1_DATA1, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD1_DATA2, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD1_DATA3, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_GPIO1_0, IOMUX_CONFIG_GPIO | IOMUX_CONFIG_SION,
		(PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PU),
	},

	/* SDHC3*/
	{
		MX51_PIN_NANDF_RDY_INT, IOMUX_CONFIG_ALT5 | IOMUX_CONFIG_SION,
		(PAD_CTL_DRV_MAX | PAD_CTL_22K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_NANDF_CS7, IOMUX_CONFIG_ALT5,
		(PAD_CTL_DRV_MAX | PAD_CTL_22K_PU | PAD_CTL_SRE_FAST),
	},
	{	/* SD3 DATA0 */
		MX51_PIN_NANDF_D8, IOMUX_CONFIG_ALT5,
		(PAD_CTL_DRV_MAX | PAD_CTL_22K_PU | PAD_CTL_SRE_FAST),
		MUX_IN_ESDHC3_IPP_DAT0_IN_SELECT_INPUT, INPUT_CTL_PATH1
	},
	{	/* SD3 DATA1 */
		MX51_PIN_NANDF_D9, IOMUX_CONFIG_ALT5,
		(PAD_CTL_DRV_MAX | PAD_CTL_22K_PU | PAD_CTL_SRE_FAST),
		MUX_IN_ESDHC3_IPP_DAT1_IN_SELECT_INPUT, INPUT_CTL_PATH1
	},
	{	/* SD3 DATA2 */
		MX51_PIN_NANDF_D10, IOMUX_CONFIG_ALT5,
		(PAD_CTL_DRV_MAX | PAD_CTL_22K_PU | PAD_CTL_SRE_FAST),
		MUX_IN_ESDHC3_IPP_DAT2_IN_SELECT_INPUT, INPUT_CTL_PATH1
	},
	{	/* SD3 DATA3 */
		MX51_PIN_NANDF_D11, IOMUX_CONFIG_ALT5,
		(PAD_CTL_DRV_MAX | PAD_CTL_22K_PU | PAD_CTL_SRE_FAST),
		MUX_IN_ESDHC3_IPP_DAT3_IN_SELECT_INPUT, INPUT_CTL_PATH1
	},
	{	/* SD3 Card detect */
		MX51_PIN_GPIO_NAND, IOMUX_CONFIG_GPIO | IOMUX_CONFIG_SION,
		(PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PU),
	},
	{	/* SD3 Write protect */
		MX51_PIN_NANDF_CS1, IOMUX_CONFIG_GPIO | IOMUX_CONFIG_SION,
		(PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PU),
	},
};
#endif

#if defined(CONFIG_USB_EHCI_ARC_H1) || defined(CONFIG_USB_EHCI_ARC_H1_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_usbh1_pins[] = {
	{	/* USBH1_STP */
		MX51_PIN_USBH1_STP, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_PUE_KEEPER |
		PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_CLK */
		MX51_PIN_USBH1_CLK, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_PUE_KEEPER |
		PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE | PAD_CTL_DDR_INPUT_CMOS),
	},
	{	/* USBH1_DIR */
		MX51_PIN_USBH1_DIR, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_PUE_KEEPER |
		PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE | PAD_CTL_DDR_INPUT_CMOS),
	},
	{	/* USBH1_NXT */
		MX51_PIN_USBH1_NXT, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_PUE_KEEPER |
		PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE | PAD_CTL_DDR_INPUT_CMOS),
	},
	{	/* USBH1_DATA0 */
		MX51_PIN_USBH1_DATA0, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA1 */
		MX51_PIN_USBH1_DATA1, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA2 */
		MX51_PIN_USBH1_DATA2, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA3 */
		MX51_PIN_USBH1_DATA3, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA4 */
		MX51_PIN_USBH1_DATA4, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA5 */
		MX51_PIN_USBH1_DATA5, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA6 */
		MX51_PIN_USBH1_DATA6, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH1_DATA7 */
		MX51_PIN_USBH1_DATA7, IOMUX_CONFIG_ALT0,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
	{	/* USBH PHY RESET */
		MX51_PIN_DISPB2_SER_RS, IOMUX_CONFIG_GPIO,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_47K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
};
#endif

#if defined(CONFIG_FB_MXC_SYNC_PANEL) || defined(CONFIG_FB_MXC_SYNC_PANEL_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_video1_pins[] = {
	{	/* DISP1 DAT0 */
		MX51_PIN_DISP1_DAT0, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT1 */
		MX51_PIN_DISP1_DAT1, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT2 */
		MX51_PIN_DISP1_DAT2, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT3 */
		MX51_PIN_DISP1_DAT3, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT4 */
		MX51_PIN_DISP1_DAT4, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT5 */
		MX51_PIN_DISP1_DAT5, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT6 */
		MX51_PIN_DISP1_DAT6, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT7 */
		MX51_PIN_DISP1_DAT7, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT8 */
		MX51_PIN_DISP1_DAT8, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT9 */
		MX51_PIN_DISP1_DAT9, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT10 */
		MX51_PIN_DISP1_DAT10, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT11 */
		MX51_PIN_DISP1_DAT11, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT12 */
		MX51_PIN_DISP1_DAT12, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT13 */
		MX51_PIN_DISP1_DAT13, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT14 */
		MX51_PIN_DISP1_DAT14, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT15 */
		MX51_PIN_DISP1_DAT15, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT16 */
		MX51_PIN_DISP1_DAT16, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT17 */
		MX51_PIN_DISP1_DAT17, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT18 */
		MX51_PIN_DISP1_DAT18, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT19 */
		MX51_PIN_DISP1_DAT19, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT20 */
		MX51_PIN_DISP1_DAT20, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT21 */
		MX51_PIN_DISP1_DAT21, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT22 */
		MX51_PIN_DISP1_DAT22, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/* DISP1 DAT23 */
		MX51_PIN_DISP1_DAT23, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_DRV_LOW | PAD_CTL_SRE_FAST),
	},
	{	/*  LCD1 Power Enable, as gpio */
		MX51_PIN_DI1_PIN11, IOMUX_CONFIG_GPIO,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_47K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_HYS_ENABLE),
	},
};
#endif

#if defined(CONFIG_I2C_MXC) || defined(CONFIG_I2C_MXC_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_i2c_pins[] = {
#ifdef CONFIG_I2C_MXC_SELECT1
	{
		MX51_PIN_SD2_CMD, IOMUX_CONFIG_ALT1 | IOMUX_CONFIG_SION,
		(PAD_CTL_SRE_FAST | PAD_CTL_ODE_OPENDRAIN_ENABLE | PAD_CTL_HYS_ENABLE |
		PAD_CTL_100K_PU | PAD_CTL_DRV_HIGH),
		MUX_IN_I2C1_IPP_SDA_IN_SELECT_INPUT, INPUT_CTL_PATH2,
	},
	{
		MX51_PIN_SD2_CLK, IOMUX_CONFIG_ALT1 | IOMUX_CONFIG_SION,
		(PAD_CTL_SRE_FAST | PAD_CTL_ODE_OPENDRAIN_ENABLE | PAD_CTL_HYS_ENABLE |
		PAD_CTL_100K_PU | PAD_CTL_DRV_HIGH),
		MUX_IN_I2C1_IPP_SCL_IN_SELECT_INPUT, INPUT_CTL_PATH2,
	},
#endif
#ifdef CONFIG_I2C_MXC_SELECT2
	{
		MX51_PIN_GPIO1_2, IOMUX_CONFIG_ALT2 | IOMUX_CONFIG_SION,
		(PAD_CTL_SRE_FAST | PAD_CTL_ODE_OPENDRAIN_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_100K_PU | PAD_CTL_HYS_ENABLE),
		MUX_IN_I2C2_IPP_SCL_IN_SELECT_INPUT, INPUT_CTL_PATH3,
	},
	{
		MX51_PIN_GPIO1_3, IOMUX_CONFIG_ALT2 | IOMUX_CONFIG_SION,
		(PAD_CTL_SRE_FAST | PAD_CTL_ODE_OPENDRAIN_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_100K_PU | PAD_CTL_HYS_ENABLE),
		MUX_IN_I2C2_IPP_SDA_IN_SELECT_INPUT, INPUT_CTL_PATH3,
	},
#endif
#ifdef CONFIG_I2C_MXC_SELECT3
	{
		MX51_PIN_I2C1_CLK, IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE |
		PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU | PAD_CTL_SRE_SLOW),
	},
	{
		MX51_PIN_I2C1_DAT, IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE |
		PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU | PAD_CTL_SRE_SLOW),
	}
#endif
};
#endif /* defined(CONFIG_I2C_MXC) || defined(CONFIG_I2C_MXC_MODULE) */

static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_devices_pins[] = {
	{	/* PMIC interrupt line */
		MX51_PIN_GPIO1_5, IOMUX_CONFIG_GPIO | IOMUX_CONFIG_SION,
		(PAD_CTL_SRE_SLOW | PAD_CTL_DRV_MEDIUM | PAD_CTL_100K_PU |
		PAD_CTL_HYS_ENABLE | PAD_CTL_DRV_VOT_HIGH),
	},
#if defined(CONFIG_INPUT_MMA7455L) || defined(CONFIG_INPUT_MMA7455L_MODULE)
	{	/* MMA7455L interrupt line */
		MX51_PIN_GPIO1_6, IOMUX_CONFIG_GPIO,
	},
	{
		MX51_PIN_GPIO1_7, IOMUX_CONFIG_ALT2,
		(PAD_CTL_DRV_HIGH | PAD_CTL_PUE_PULL |
		PAD_CTL_100K_PU | PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST),
	},
#endif
};

#if defined(CONFIG_SND_SOC_WM8753) || defined(CONFIG_SND_SOC_WM8753_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_audio_pins[] = {

	/* TODO: the SSI interface should be selectable through configuration */
	{	/* AUD3_BB_CK */
		MX51_PIN_AUD3_BB_CK, IOMUX_CONFIG_ALT0 ,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_HYS_NONE | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_PKE_ENABLE |
		PAD_CTL_PUE_KEEPER ),
	},
	{	/* AUD3_BB_FS */
		MX51_PIN_AUD3_BB_FS, IOMUX_CONFIG_ALT0 ,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_HYS_NONE |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE),
	},
	{	/* AUD3_BB_RXD */
		MX51_PIN_AUD3_BB_RXD, IOMUX_CONFIG_ALT0 ,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_HYS_NONE |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE),
	},
	{	/* AUD3_BB_TXD */
		MX51_PIN_AUD3_BB_TXD, IOMUX_CONFIG_ALT0 ,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_100K_PU |
		PAD_CTL_HYS_NONE | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_PKE_ENABLE |
		PAD_CTL_PUE_KEEPER ),
	},
};
#endif

#if defined(CONFIG_SPI_MXC) || defined(CONFIG_SPI_MXC_MODULE)
static struct mxc_iomux_pin_cfg __initdata ccwmx51_cspi_pins[] = {
#ifdef CONFIG_SPI_MXC_SELECT1
	/* ECSPI1 */
	{	/* MISO */
		MX51_PIN_CSPI1_MISO, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_PUE_KEEPER | PAD_CTL_DRV_HIGH |
		PAD_CTL_SRE_FAST),
	},
	{	/* MOSI */
		MX51_PIN_CSPI1_MOSI, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_PUE_KEEPER | PAD_CTL_DRV_HIGH |
		PAD_CTL_SRE_FAST),
	},
	{	/* SCLK */
		MX51_PIN_CSPI1_SCLK, IOMUX_CONFIG_ALT0,
		(PAD_CTL_HYS_NONE | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_PUE_KEEPER | PAD_CTL_DRV_HIGH |
		PAD_CTL_SRE_FAST),
	},
	{	/* SS0 */
		MX51_PIN_CSPI1_SS0, IOMUX_CONFIG_GPIO,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_47K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE),
	},
#ifdef CONFIG_SPI_MXC_SELECT1_SS1
	{	/* SS1 */
		MX51_PIN_CSPI1_SS1, IOMUX_CONFIG_GPIO,
		(PAD_CTL_SRE_FAST | PAD_CTL_DRV_HIGH | PAD_CTL_47K_PU |
		PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE),
	},
#endif
#endif
#ifdef CONFIG_SPI_MXC_SELECT2
	/* ECSPI2 */
	{	/* SCLK */
		MX51_PIN_NANDF_RB2, IOMUX_CONFIG_ALT2,
		(PAD_CTL_SRE_SLOW | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_DRV_LOW |
		PAD_CTL_PUE_KEEPER | PAD_CTL_HYS_ENABLE),
	},
	{	/* MISO */
		MX51_PIN_NANDF_RB3, IOMUX_CONFIG_ALT2,
		(PAD_CTL_SRE_SLOW | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_DRV_HIGH |
		PAD_CTL_PUE_KEEPER | PAD_CTL_HYS_ENABLE),
	},
	{	/* MOSI */
		MX51_PIN_NANDF_D15, IOMUX_CONFIG_ALT2,
		(PAD_CTL_SRE_SLOW | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_DRV_HIGH |
		PAD_CTL_PUE_KEEPER | PAD_CTL_HYS_ENABLE),
	},
	{	/* SS0 */
		MX51_PIN_NANDF_RDY_INT, IOMUX_CONFIG_ALT2,
		(PAD_CTL_SRE_SLOW | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_DRV_LOW |
		PAD_CTL_PUE_KEEPER | PAD_CTL_HYS_ENABLE),
	},
	{	/* SI_VER_TO2, SS1 */
		MX51_PIN_NANDF_D12, IOMUX_CONFIG_ALT2,
		(PAD_CTL_SRE_SLOW | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_DRV_HIGH |
		PAD_CTL_PUE_KEEPER | PAD_CTL_HYS_ENABLE),
	},
	{	/* SI_VER_TO2, RDY */
		MX51_PIN_NANDF_RB1, IOMUX_CONFIG_ALT2,
		(PAD_CTL_SRE_SLOW | PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_DRV_LOW |
		PAD_CTL_PUE_KEEPER | PAD_CTL_HYS_ENABLE),
	},
#endif
#ifdef CONFIG_SPI_MXC_SELECT3
	/* ECSPI3 */
	{
		MX51_PIN_USBH1_CLK, IOMUX_CONFIG_ALT2,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PU |
		 PAD_CTL_DRV_MEDIUM | PAD_CTL_HYS_ENABLE | PAD_CTL_SRE_SLOW),
	},
	{
		MX51_PIN_USBH1_DATA4, IOMUX_CONFIG_ALT2,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE | PAD_CTL_PUE_KEEPER |
		PAD_CTL_DRV_MEDIUM | PAD_CTL_HYS_ENABLE | PAD_CTL_SRE_SLOW),
	},
	{
		MX51_PIN_USBH1_DATA5, IOMUX_CONFIG_ALT2,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE | PAD_CTL_PUE_KEEPER |
		PAD_CTL_DRV_MEDIUM | PAD_CTL_HYS_ENABLE | PAD_CTL_SRE_SLOW),
	},
	{
		MX51_PIN_USBH1_NXT, IOMUX_CONFIG_ALT2,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE | PAD_CTL_PUE_KEEPER |
		PAD_CTL_DRV_MEDIUM | PAD_CTL_HYS_ENABLE | PAD_CTL_SRE_SLOW),
	},
	{
		MX51_PIN_USBH1_DIR, IOMUX_CONFIG_ALT2,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PU |
		PAD_CTL_DRV_MEDIUM | PAD_CTL_HYS_ENABLE | PAD_CTL_SRE_SLOW),
	},
	{
		MX51_PIN_USBH1_STP, IOMUX_CONFIG_ALT2,
		(PAD_CTL_ODE_OPENDRAIN_NONE | PAD_CTL_HYS_ENABLE | PAD_CTL_PUE_KEEPER |
		PAD_CTL_DRV_MEDIUM | PAD_CTL_HYS_ENABLE | PAD_CTL_SRE_SLOW),
	},
#endif
};

/* workaround for ecspi chipselect pin may not keep correct level when idle */
void ccwmx51_gpio_spi_chipselect_active(int busnum, int ssb_pol, int chipselect)
{
	u8 mask = 0x1 << (chipselect - 1);

	switch (busnum) {
	case 1:
		switch (chipselect) {
		case 0x1:
			gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS0),
				       (ssb_pol & mask) ?  1 : 0);
			break;
		case 0x2:
			gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS1),
				       (ssb_pol & mask) ?  1 : 0);
			break;
		default:
			break;
		}
		break;
	case 2:
	case 3:
	default:
		break;
	}
}
EXPORT_SYMBOL(ccwmx51_gpio_spi_chipselect_active);

void ccwmx51_gpio_spi_chipselect_inactive(int busnum, int ssb_pol,
					  int chipselect)
{
	u8 mask = 0x1 << (chipselect - 1);

	switch (busnum) {
	case 1:
		switch (chipselect) {
		case 0x1:
			gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS0),
				       (ssb_pol & mask) ?  0 : 1);
			break;
		case 0x2:
			gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS1),
				       (ssb_pol & mask) ?  0 : 1);
			break;
		default:
			break;
		}
		break;
	case 2:
	case 3:
	default:
		break;
	}
}
EXPORT_SYMBOL(ccwmx51_gpio_spi_chipselect_inactive);

#endif /* defined(CONFIG_SPI_MXC) || defined(CONFIG_SPI_MXC_MODULE) */

void __init ccwmx51_io_init(void)
{
	int i;

#if defined(CONFIG_SMSC911X) || defined(CONFIG_SMSC911X_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_ext_eth_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_ext_eth_pins[i].pin,
				ccwmx51_iomux_ext_eth_pins[i].mux_mode);
		if (ccwmx51_iomux_ext_eth_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_ext_eth_pins[i].pin,
					ccwmx51_iomux_ext_eth_pins[i].pad_cfg);
		if (ccwmx51_iomux_ext_eth_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_ext_eth_pins[i].in_select,
					ccwmx51_iomux_ext_eth_pins[i].in_mode);
	}
#endif

#if defined(CONFIG_MMC_IMX_ESDHCI) || defined(CONFIG_MMC_IMX_ESDHCI_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_mmc_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_mmc_pins[i].pin,
				  ccwmx51_iomux_mmc_pins[i].mux_mode);
		if (ccwmx51_iomux_mmc_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_mmc_pins[i].pin,
					  ccwmx51_iomux_mmc_pins[i].pad_cfg);
		if (ccwmx51_iomux_mmc_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_mmc_pins[i].in_select,
					    ccwmx51_iomux_mmc_pins[i].in_mode);
	}
#endif

#if defined(CONFIG_USB_EHCI_ARC_H1) || defined(CONFIG_USB_EHCI_ARC_H1_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_usbh1_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_usbh1_pins[i].pin,
				  ccwmx51_iomux_usbh1_pins[i].mux_mode);
		if (ccwmx51_iomux_usbh1_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_usbh1_pins[i].pin,
					  ccwmx51_iomux_usbh1_pins[i].pad_cfg);
		if (ccwmx51_iomux_usbh1_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_usbh1_pins[i].in_select,
					    ccwmx51_iomux_usbh1_pins[i].in_mode);
	}
#endif

#if defined(CONFIG_FB_MXC_SYNC_PANEL) || defined(CONFIG_FB_MXC_SYNC_PANEL_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_video1_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_video1_pins[i].pin,
				  ccwmx51_iomux_video1_pins[i].mux_mode);
		if (ccwmx51_iomux_video1_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_video1_pins[i].pin,
					  ccwmx51_iomux_video1_pins[i].pad_cfg);
		if (ccwmx51_iomux_video1_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_video1_pins[i].in_select,
					    ccwmx51_iomux_video1_pins[i].in_mode);
	}
	/*  LCD Power Enable */
	gpio_request(IOMUX_TO_GPIO(MX51_PIN_DI1_PIN11), "gpio3_0");
	gpio_direction_output(IOMUX_TO_GPIO(MX51_PIN_DI1_PIN11), 0);
#endif

#if defined(CONFIG_I2C_MXC) || defined(CONFIG_I2C_MXC_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_i2c_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_i2c_pins[i].pin,
				ccwmx51_iomux_i2c_pins[i].mux_mode);
		if (ccwmx51_iomux_i2c_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_i2c_pins[i].pin,
					ccwmx51_iomux_i2c_pins[i].pad_cfg);
		if (ccwmx51_iomux_i2c_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_i2c_pins[i].in_select,
					ccwmx51_iomux_i2c_pins[i].in_mode);
	}
#endif

#if defined(CONFIG_SND_SOC_WM8753) || defined(CONFIG_SND_SOC_WM8753_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_audio_pins); i++) {
		mxc_request_iomux(ccwmx51_audio_pins[i].pin,
				  ccwmx51_audio_pins[i].mux_mode);
		if (ccwmx51_audio_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_audio_pins[i].pin,
					  ccwmx51_audio_pins[i].pad_cfg);
		if (ccwmx51_audio_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_audio_pins[i].in_select,
					    ccwmx51_audio_pins[i].in_mode);
	}
#endif

#if defined(CONFIG_SPI_MXC) || defined(CONFIG_SPI_MXC_MODULE)
	for (i = 0; i < ARRAY_SIZE(ccwmx51_cspi_pins); i++) {
		mxc_request_iomux(ccwmx51_cspi_pins[i].pin,
				  ccwmx51_cspi_pins[i].mux_mode);
		if (ccwmx51_cspi_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_cspi_pins[i].pin,
					  ccwmx51_cspi_pins[i].pad_cfg);
		if (ccwmx51_cspi_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_cspi_pins[i].in_select,
					    ccwmx51_cspi_pins[i].in_mode);
	}
#ifdef CONFIG_SPI_MXC_SELECT1
	gpio_request(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS0), "cspi1_ss0");
	gpio_direction_output(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS0), 0);
	gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS0), 0);
#ifdef CONFIG_SPI_MXC_SELECT1_SS1
	gpio_request(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS1), "cspi1_ss1");
	gpio_direction_output(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS1), 0);
	gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSPI1_SS1), 0);
#endif
#endif

#endif

	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_devices_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_devices_pins[i].pin,
				  ccwmx51_iomux_devices_pins[i].mux_mode);
		if (ccwmx51_iomux_devices_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_devices_pins[i].pin,
					  ccwmx51_iomux_devices_pins[i].pad_cfg);
		if (ccwmx51_iomux_devices_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_devices_pins[i].in_select,
					    ccwmx51_iomux_devices_pins[i].in_mode);
	}

	/* PMIC interrupt line */
	gpio_request(IOMUX_TO_GPIO(MX51_PIN_GPIO1_5), "gpio1_5");
	gpio_direction_input(IOMUX_TO_GPIO(MX51_PIN_GPIO1_5));

#if defined(CONFIG_USB_EHCI_ARC_H1) || defined(CONFIG_USB_EHCI_ARC_H1_MODULE)
	/* USB PHY/HUB reset*/
	gpio_request(IOMUX_TO_GPIO(MX51_PIN_DISPB2_SER_RS), "gpio3_8");
	gpio_direction_output(IOMUX_TO_GPIO(MX51_PIN_DISPB2_SER_RS), 0);
	msleep(1);
	gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_DISPB2_SER_RS), 1);
#endif

#if defined(CONFIG_MMC_IMX_ESDHCI) || defined(CONFIG_MMC_IMX_ESDHCI_MODULE)
	/* For the wireless module */
	ccwmx51_mmc2_gpio_active();
#endif
}

#if defined(CONFIG_MMC_IMX_ESDHCI) || defined(CONFIG_MMC_IMX_ESDHCI_MODULE)
/* IOMUX settings, for the wireless interface */
static struct mxc_iomux_pin_cfg __initdata ccwmx51_iomux_mmc2_pins[] = {
	/* SDHC2*/
	{
		MX51_PIN_SD2_CMD, IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD2_CLK, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD2_DATA0, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD2_DATA1, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD2_DATA2, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
	{
		MX51_PIN_SD2_DATA3, IOMUX_CONFIG_ALT0,
		(PAD_CTL_PUE_KEEPER | PAD_CTL_PKE_ENABLE | PAD_CTL_DRV_HIGH |
		PAD_CTL_47K_PU | PAD_CTL_SRE_FAST),
	},
};

static void ccwmx51_mmc2_gpio_active(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(ccwmx51_iomux_mmc2_pins); i++) {
		mxc_request_iomux(ccwmx51_iomux_mmc2_pins[i].pin,
				  ccwmx51_iomux_mmc2_pins[i].mux_mode);
		if (ccwmx51_iomux_mmc2_pins[i].pad_cfg)
			mxc_iomux_set_pad(ccwmx51_iomux_mmc2_pins[i].pin,
					  ccwmx51_iomux_mmc2_pins[i].pad_cfg);
		if (ccwmx51_iomux_mmc2_pins[i].in_select)
			mxc_iomux_set_input(ccwmx51_iomux_mmc2_pins[i].in_select,
					    ccwmx51_iomux_mmc2_pins[i].in_mode);
	}
}

void ccwmx51_mmc2_gpio_inactive(void)
{
}
#endif

#if defined(CONFIG_SERIAL_MXC) || defined(CONFIG_SERIAL_MXC_MODULE)
#define SERIAL_PORT_PAD		(PAD_CTL_HYS_ENABLE | PAD_CTL_PKE_ENABLE | \
				 PAD_CTL_PUE_PULL | PAD_CTL_DRV_HIGH | \
				 PAD_CTL_SRE_FAST)

void gpio_uart_active(int port, int no_irda)
{
	/* Configure the IOMUX control registers for the UART signals */
	switch (port) {

	case 0:		/* UART 1 IOMUX Configs */
#ifdef CONFIG_UART1_ENABLED
		mxc_request_iomux(MX51_PIN_UART1_RXD, IOMUX_CONFIG_ALT0);
		mxc_request_iomux(MX51_PIN_UART1_TXD, IOMUX_CONFIG_ALT0);
		mxc_iomux_set_pad(MX51_PIN_UART1_RXD, SERIAL_PORT_PAD);
		mxc_iomux_set_pad(MX51_PIN_UART1_TXD, SERIAL_PORT_PAD);
		mxc_iomux_set_input(MUX_IN_UART1_IPP_UART_RXD_MUX_SELECT_INPUT, INPUT_CTL_PATH0);

		/* TODO enable CTS/RTS if selected */
#endif
		break;

	case 1:		/* UART 2 IOMUX Configs */
#ifdef CONFIG_UART2_ENABLED
		mxc_request_iomux(MX51_PIN_UART2_RXD, IOMUX_CONFIG_ALT0);
		mxc_request_iomux(MX51_PIN_UART2_TXD, IOMUX_CONFIG_ALT0);
		mxc_iomux_set_pad(MX51_PIN_UART2_RXD, SERIAL_PORT_PAD);
		mxc_iomux_set_pad(MX51_PIN_UART2_TXD, SERIAL_PORT_PAD);
		mxc_iomux_set_input(MUX_IN_UART2_IPP_UART_RXD_MUX_SELECT_INPUT, INPUT_CTL_PATH2);

		/*  TODO enable CTS/RTS if selected */
#endif
		break;
	case 2:		/* UART 3 IOMUX Configs */
#ifdef CONFIG_UART3_ENABLED
		mxc_request_iomux(MX51_PIN_UART3_RXD, IOMUX_CONFIG_ALT1);
		mxc_request_iomux(MX51_PIN_UART3_TXD, IOMUX_CONFIG_ALT1);
		mxc_iomux_set_pad(MX51_PIN_UART3_RXD, SERIAL_PORT_PAD);
		mxc_iomux_set_pad(MX51_PIN_UART3_TXD, SERIAL_PORT_PAD);
		mxc_iomux_set_input(MUX_IN_UART3_IPP_UART_RXD_MUX_SELECT_INPUT, INPUT_CTL_PATH4);

		/* TODO enable CTS/RTS if selected */
#endif
		break;
	default:
		break;
	}

}

#else
void gpio_uart_active(int port, int no_irda) {}
#endif
void gpio_uart_inactive(int port, int no_irda) {}
EXPORT_SYMBOL(gpio_uart_active);
EXPORT_SYMBOL(gpio_uart_inactive);



