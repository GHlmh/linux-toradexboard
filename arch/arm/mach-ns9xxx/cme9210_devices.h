/*
 * arch/arm/mach-ns9xxx/cme9210_devices.h
 *
 * Copyright (C) 2008 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

/* Digi Connect ME 9210 variants */
enum cme9210_variant {
	CME9210_BASIC = 0x37f,
	CME9210_CAN = 0x37d ,
};

enum cme9210_variant get_cme9210_variant(void);
void __init ns9xxx_add_device_cme9210_eth(void);
void __init ns9xxx_add_device_cme9210_uarta(int gpio_nr);
void __init ns9xxx_add_device_cme9210_uartb(int gpio_nr);
void __init ns9xxx_add_device_cme9210_uartc(int gpio_nr);
void __init ns9xxx_add_device_cme9210_flash(void);
void __init ns9xxx_add_device_cme9210_spi(void);
void __init ns9xxx_add_device_cme9210_i2c(void);

#define ns9xxx_add_device_cme9210_uarta_rxtx() \
	ns9xxx_add_device_cme9210_uarta(2)
#define ns9xxx_add_device_cme9210_uarta_ctsrtsrxtx() \
	ns9xxx_add_device_cme9210_uarta(4)
#define ns9xxx_add_device_cme9210_uarta_full() \
	ns9xxx_add_device_cme9210_uarta(8)
#define ns9xxx_add_device_cme9210_uartb_rxtx() \
	ns9xxx_add_device_cme9210_uartb(2)
#define ns9xxx_add_device_cme9210_uartc_rxtx() \
	ns9xxx_add_device_cme9210_uartc(2)
