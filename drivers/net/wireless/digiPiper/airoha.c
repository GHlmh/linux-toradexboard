/*
 * Ubec AH7230 radio support.
 *
 * Copyright © 2008  Digi International, Inc
 *
 * Author: Contact support@digi.com for information about this software.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <net/mac80211.h>
#include <net/wireless.h>

#include "pipermain.h"
#include "mac.h"
#include "airoha.h"

#define RF_AIROHA_7230      (0)
#define RF_AIROHA_2236      (1)

#define TRANSCEIVER     RF_AIROHA_7230

/*
 * Number of us to change channels.  I counted the number of udelays once
 * and it was about 1030, plus the 1 us delays for each register write.
 * So probably about 1200 in reality, I'm saying 1500 to be safe.
 */
#define CHANNEL_CHANGE_TIME     (1500)

#define readReg(reg)                priv->read_reg(priv, reg)
#define writeReg(reg, value, op)    priv->write_reg(priv, reg, value, op)

static void InitializeRF(struct ieee80211_hw *hw, int band_selection);
static int al7230_set_txpwr(struct ieee80211_hw *hw, uint8_t val);

#define MacSetTxPower(x)        al7230_set_txpwr(hw, x)


static const struct
{
    unsigned int integer;
    unsigned int fraction;
    unsigned int address4;
    unsigned int tracking;
} freqTableAiroha_7230[] =
{
    { 0, 0, 0, 0 },             //                    0
    
    // 2.4 GHz band (802.11b/g)
    { 0x00379, 0x13333, 0x7FD78, TRACK_BG_BAND }, // B-1   (2412 MHz)   1
    { 0x00379, 0x1B333, 0x7FD78, TRACK_BG_BAND }, // B-2   (2417 MHz)   2
    { 0x00379, 0x03333, 0x7FD78, TRACK_BG_BAND }, // B-3   (2422 MHz)   3
    { 0x00379, 0x0B333, 0x7FD78, TRACK_BG_BAND }, // B-4   (2427 MHz)   4
    { 0x0037A, 0x13333, 0x7FD78, TRACK_BG_BAND }, // B-5   (2432 MHz)   5
    { 0x0037A, 0x1B333, 0x7FD78, TRACK_BG_BAND }, // B-6   (2437 MHz)   6
    { 0x0037A, 0x03333, 0x7FD78, TRACK_BG_BAND }, // B-7   (2442 MHz)   7
    { 0x0037A, 0x0B333, 0x7FD78, TRACK_BG_BAND }, // B-8   (2447 MHz)   8
    { 0x0037B, 0x13333, 0x7FD78, TRACK_BG_BAND }, // B-9   (2452 MHz)   9
    { 0x0037B, 0x1B333, 0x7FD78, TRACK_BG_BAND }, // B-10  (2457 MHz)  10
    { 0x0037B, 0x03333, 0x7FD78, TRACK_BG_BAND }, // B-11  (2462 MHz)  11
    { 0x0037B, 0x0B333, 0x7FD78, TRACK_BG_BAND }, // B-12  (2467 MHz)  12
    { 0x0037C, 0x13333, 0x7FD78, TRACK_BG_BAND }, // B-13  (2472 MHz)  13
    { 0x0037C, 0x06666, 0x7FD78, TRACK_BG_BAND }, // B-14  (2484 MHz)  14

    { 0, 0, 0, 0 },             // reserved for future b/g expansion   15
    { 0, 0, 0, 0 },             // reserved for future b/g expansion   16

    // Extended 4 GHz bands (802.11a) - Lower Band
    { 0x0FF52, 0x00000, 0x67F78, TRACK_4920_4980_A_BAND }, // L-184 (4920 MHz)  17
    { 0x0FF52, 0x0AAAA, 0x77F78, TRACK_4920_4980_A_BAND }, // L-188 (4940 MHz)  18
    { 0x0FF53, 0x15555, 0x77F78, TRACK_4920_4980_A_BAND }, // L-192 (4960 MHz)  19
    { 0x0FF53, 0x00000, 0x67F78, TRACK_4920_4980_A_BAND }, // L-196 (4980 MHz)  20

    // Extended 5 GHz bands (802.11a)
    { 0x0FF54, 0x00000, 0x67F78, TRACK_5150_5350_A_BAND }, // A-8   (5040 MHz)  21 tracking?
    { 0x0FF54, 0x0AAAA, 0x77F78, TRACK_5150_5350_A_BAND }, // A-12  (5060 MHz)  22 tracking?
    { 0x0FF55, 0x15555, 0x77F78, TRACK_5150_5350_A_BAND }, // A-16  (5080 MHz)  23 tracking?
    { 0x0FF56, 0x05555, 0x77F78, TRACK_5150_5350_A_BAND }, // A-34  (5170 MHz)  24
    { 0x0FF56, 0x0AAAA, 0x77F78, TRACK_5150_5350_A_BAND }, // A-36  (5180 MHz)  25
    { 0x0FF57, 0x10000, 0x77F78, TRACK_5150_5350_A_BAND }, // A-38  (5190 MHz)  26
    { 0x0FF57, 0x15555, 0x77F78, TRACK_5150_5350_A_BAND }, // A-40  (5200 MHz)  27
    { 0x0FF57, 0x1AAAA, 0x77F78, TRACK_5150_5350_A_BAND }, // A-42  (5210 MHz)  28
    { 0x0FF57, 0x00000, 0x67F78, TRACK_5150_5350_A_BAND }, // A-44  (5220 MHz)  29
    { 0x0FF57, 0x05555, 0x77F78, TRACK_5150_5350_A_BAND }, // A-46  (5230 MHz)  30
    { 0x0FF57, 0x0AAAA, 0x77F78, TRACK_5150_5350_A_BAND }, // A-48  (5240 MHz)  31
    
    { 0x0FF58, 0x15555, 0x77F78, TRACK_5150_5350_A_BAND }, // A-52  (5260 MHz)  32
    { 0x0FF58, 0x00000, 0x67F78, TRACK_5150_5350_A_BAND }, // A-56  (5280 MHz)  33
    { 0x0FF58, 0x0AAAA, 0x77F78, TRACK_5150_5350_A_BAND }, // A-60  (5300 MHz)  34
    { 0x0FF59, 0x15555, 0x77F78, TRACK_5150_5350_A_BAND }, // A-64  (5320 MHz)  35
    
    { 0x0FF5C, 0x15555, 0x77F78, TRACK_5470_5725_A_BAND }, // A-100 (5500 MHz)  36
    { 0x0FF5C, 0x00000, 0x67F78, TRACK_5470_5725_A_BAND }, // A-104 (5520 MHz)  37
    { 0x0FF5C, 0x0AAAA, 0x77F78, TRACK_5470_5725_A_BAND }, // A-108 (5540 MHz)  38
    { 0x0FF5D, 0x15555, 0x77F78, TRACK_5470_5725_A_BAND }, // A-112 (5560 MHz)  39
    { 0x0FF5D, 0x00000, 0x67F78, TRACK_5470_5725_A_BAND }, // A-116 (5580 MHz)  40
    { 0x0FF5D, 0x0AAAA, 0x77F78, TRACK_5470_5725_A_BAND }, // A-120 (5600 MHz)  41
    { 0x0FF5E, 0x15555, 0x77F78, TRACK_5470_5725_A_BAND }, // A-124 (5620 MHz)  42
    { 0x0FF5E, 0x00000, 0x67F78, TRACK_5470_5725_A_BAND }, // A-128 (5640 MHz)  43
    { 0x0FF5E, 0x0AAAA, 0x77F78, TRACK_5470_5725_A_BAND }, // A-132 (5660 MHz)  44
    { 0x0FF5F, 0x15555, 0x77F78, TRACK_5470_5725_A_BAND }, // A-136 (5680 MHz)  45
    { 0x0FF5F, 0x00000, 0x67F78, TRACK_5470_5725_A_BAND }, // A-140 (5700 MHz)  46
    
    { 0x0FF60, 0x18000, 0x77F78, TRACK_5725_5825_A_BAND }, // A-149 (5745 MHz)  47
    { 0x0FF60, 0x02AAA, 0x77F78, TRACK_5725_5825_A_BAND }, // A-153 (5765 MHz)  48
    { 0x0FF60, 0x0D555, 0x77F78, TRACK_5725_5825_A_BAND }, // A-157 (5785 MHz)  49
    { 0x0FF61, 0x18000, 0x77F78, TRACK_5725_5825_A_BAND }, // A-161 (5805 MHz)  50
    { 0x0FF61, 0x02AAA, 0x77F78, TRACK_5725_5825_A_BAND }, // A-165 (5825 MHz)  51
};

static struct ieee80211_channel al7230_bg_channels[] = {
	{ .center_freq = 2412 },
	{ .center_freq = 2417 },
	{ .center_freq = 2422 },
	{ .center_freq = 2427 },
	{ .center_freq = 2432 },
	{ .center_freq = 2437 },
	{ .center_freq = 2442 },
	{ .center_freq = 2447 },
	{ .center_freq = 2452 },
	{ .center_freq = 2457 },
	{ .center_freq = 2462 },
	{ .center_freq = 2467 },
	{ .center_freq = 2472 },
	{ .center_freq = 2484 },
};

static const struct ieee80211_rate al7230_bg_rates[] = {
	/* psk/cck rates */
	{
		.bitrate = 10,
		.flags = IEEE80211_RATE_SHORT_PREAMBLE,
	},
	{
		.bitrate = 20,
		.flags = IEEE80211_RATE_SHORT_PREAMBLE,
	},
	{
		.bitrate = 55,
		.flags = IEEE80211_RATE_SHORT_PREAMBLE,
	},
	{
		.bitrate = 110,
		.flags = IEEE80211_RATE_SHORT_PREAMBLE,
	},
	/* ofdm rates */
	{
		.bitrate = 60,
		.hw_value = 0xb,
	},
	{
		.bitrate = 90,
		.hw_value = 0xf,
	},
	{
		.bitrate = 120,
		.hw_value = 0xa,
	},
	{
		.bitrate = 180,
		.hw_value = 0xe,
	},
	{
		.bitrate = 240,
		.hw_value = 0x9,
	},
	{
		.bitrate = 360,
		.hw_value = 0xd,
	},
	{
		.bitrate = 480,
		.hw_value = 0x8,
	},
	{
		.bitrate = 540,
		.hw_value = 0xc,
	},
};


    

static int WriteRF(struct ieee80211_hw *hw, unsigned char reg, unsigned int val)
{
	struct piper_priv *priv = hw->priv;
	int err;

	err  = writeReg(BB_SPI_DATA, val << 4 | reg, op_write);
	udelay(3);      /* Mike Schaffner says to allow 2 us or more between all writes*/
	return err;
}



static int al7230_rf_set_chan(struct ieee80211_hw *hw, int channel)
{
	struct piper_priv *priv = hw->priv;
    /* current frequency band */
    //static int rf_band = IEEE80211_BAND_2GHZ; 

#if 0
    if (CHAN_5G (channel))
        rf_band = IEEE80211_BAND_5GHZ;
    else
        rf_band = IEEE80211_BAND_2GHZ;       	
#else
    enum ieee80211_band rf_band = IEEE80211_BAND_2GHZ;
#endif
    /* Disable the rx processing path */
	writeReg(BB_GENERAL_CTL, ~BB_GENERAL_CTL_RX_EN, op_and);
    	                

#if (TRANSCEIVER == RF_AIROHA_2236)
    	writeReg(BB_GENERAL_STAT, BB_GENERAL_STAT_B_EN, op_or);
    	                
#if 0
        if (macParams.band == WLN_BAND_B)
        {
            // turn off OFDM
        	writeReg(BB_GENERAL_STAT, ~BB_GENERAL_STAT_A_EN, op_and);
        }
        else
        {
            // turn on OFDM
        	writeReg(BB_GENERAL_STAT, BB_GENERAL_STAT_A_EN, op_or);
        }
#else
        writeReg(BB_GENERAL_STAT, ~BB_GENERAL_STAT_A_EN, op_and);
#endif            
        /* set the 802.11b/g frequency band specific tracking constant */
    	writeReg(BB_TRACK_CONTROL, 0xff00ffff, op_and);
    	                
    	writeReg(BB_TRACK_CONTROL, TRACK_BG_BAND, op_or);
    	                
                                
        /* perform chip and frequency-band specific RF initialization */
        InitializeRF(hw, rf_band);    
        
        MacSetTxPower (priv->tx_power);
    
        WriteRF(hw, 0, freqTableAiroha_2236[channel].integer); 
        WriteRF(hw, 1, freqTableAiroha_2236[channel].fraction); 
    
        /* critical delay for correct calibration */
        udelay (10);
    
        /* TXON, PAON and RXON should all be low before Calibration 
         * TXON and PAON will be low as long as no frames are written to the TX DATA fifo.  
         * RXON will be low as long as the receive path is not enabled (bit 0 of GEN CTL register is 0).  
         */   
    
            /* calibrate RF transceiver */
    
            /* TXDCOC->active; RCK->disable */
        WriteRF(hw, 15, 0x00D87 );
        udelay (50);	
        /* TXDCOC->disable; RCK->enable */		
        WriteRF(hw, 15, 0x00787 );		
        udelay (50);	
        /* TXDCOC->disable; RCK->disable */
        WriteRF(hw, 15, 0x00587 );
        udelay (50);
            
        /* configure the baseband processing engine */		    
    	writeReg(BB_GENERAL_CTL, ~BB_GENERAL_CTL_GEN_5GEN, op_and);
    
        /*Re-enable the rx processing path */
    	writeReg(BB_GENERAL_CTL, BB_GENERAL_CTL_RX_EN, op_or);
        
#elif (TRANSCEIVER == RF_AIROHA_7230)
        /* enable the frequency-band specific PA */
        if (rf_band == IEEE80211_BAND_2GHZ)
        {
            //HW_GEN_CONTROL &= ~GEN_PA_ON;
        	writeReg(BB_GENERAL_STAT, BB_GENERAL_STAT_A_EN | BB_GENERAL_STAT_B_EN, op_or);
        	                
            /* set the 802.11b/g frequency band specific tracking constant */
        	writeReg(BB_TRACK_CONTROL, 0xff00ffff, op_and);
        	                
        	writeReg(BB_TRACK_CONTROL, TRACK_BG_BAND, op_or);
        	                
        }
        else
        {
            //HW_GEN_CONTROL |= GEN_PA_ON; 
    
            // turn off PSK/CCK  
        	writeReg(BB_GENERAL_STAT, ~BB_GENERAL_STAT_B_EN, op_and);

            // turn on OFDM
        	writeReg(BB_GENERAL_STAT, BB_GENERAL_STAT_A_EN, op_or);
        	                            
            /* Set the 802.11a frequency sub-band specific tracking constant */	      
            /* All 8 supported 802.11a channels are in this 802.11a frequency sub-band */
        	writeReg(BB_TRACK_CONTROL, 0xff00ffff, op_and);
        	                
        	writeReg(BB_TRACK_CONTROL, freqTableAiroha_7230[channel].tracking, op_or);
        	                
        } 
    
        /* perform chip and frequency-band specific RF initialization */
        InitializeRF(hw, rf_band);
        
        MacSetTxPower (priv->tx_power);
    
        /* Set the channel frequency */
        WriteRF(hw, 0, freqTableAiroha_7230[channel].integer); 
        WriteRF(hw, 1, freqTableAiroha_7230[channel].fraction); 
        WriteRF(hw, 4, freqTableAiroha_7230[channel].address4); 
    
        /* critical delay for correct calibration */
        udelay (10);
    
        // Select the frequency band: 5Ghz or 2.4Ghz
        if (rf_band == IEEE80211_BAND_5GHZ)
        {
            /* calibrate RF transceiver */
    
            /* TXDCOC->active; RCK->disable */
            WriteRF(hw, 15, 0x9ABA8 );
            udelay (50);	
    
            /* TXDCOC->disable; RCK->enable */		
            WriteRF(hw, 15, 0x3ABA8 );	
            udelay (50);		
    
            /* TXDCOC->disable; RCK->disable */
            WriteRF(hw, 15, 0x12BAC );
            udelay (50);	
    
            /* configure the baseband processing engine */
            //HW_GEN_CONTROL |= GEN_5GEN;  
        	writeReg(BB_GENERAL_CTL, BB_GENERAL_CTL_GEN_5GEN, op_or);
        	                
        }
        else
        {
            /* calibrate RF transceiver */
    
            /* TXDCOC->active; RCK->disable */
            WriteRF(hw, 15, 0x9ABA8 );
            udelay (50);	
    
            /* TXDCOC->disable; RCK->enable */		
            WriteRF(hw, 15, 0x3ABA8 );	
            udelay (50);		
    
            /* TXDCOC->disable; RCK->disable */
            WriteRF(hw, 15, 0x1ABA8 );
            udelay (50);	
    
            /* configure the baseband processing engine */		    
        	writeReg(BB_GENERAL_CTL, ~BB_GENERAL_CTL_GEN_5GEN, op_and);
        	                
        }
    
        /*Re-enable the rx processing path */
	    udelay(150); /* Airoha says electronics will be ready in 150 us */
    	writeReg(BB_GENERAL_CTL, BB_GENERAL_CTL_RX_EN, op_or);

#endif


    return 0;
}




static int al7230_set_txpwr(struct ieee80211_hw *hw, uint8_t value)
{    
#if (TRANSCEIVER == RF_AIROHA_2236)
    const unsigned char powerTable_2236[] = 
    {
        4, 10, 10, 18, 22, 22, 28, 28,
        33, 33, 36, 38, 40, 43, 45, 47
    };  /* AL2236 */
            WriteRF(digi, 9, 0x05440 | powerTable_2236[value & 0xf]);  
#elif (TRANSCEIVER == RF_AIROHA_7230)
    const unsigned char powerTable_7230[] = 
    {
        0x14, 0x14, 0x14, 0x18, 0x18, 0x1c, 0x1c, 0x20,
        0x20, 0x24, 0x24, 0x29, 0x29, 0x2c, 0x2c, 0x30
    };
    WriteRF(hw, 11, 0x08040 | powerTable_7230[value & 0xf]);

#endif

    return 0;
}



static void InitializeRF(struct ieee80211_hw *hw, int band_selection)
{
	struct piper_priv *priv = hw->priv;
	
#if (TRANSCEIVER == RF_AIROHA_2236)

            digi_dbg("**** transceiver == RF_AIROHA_2236\n");
            /* Initial settings for 20 MHz reference frequency, 802.11b/g */	                   
            writeReg(BB_OUTPUT_CONTROL, 0xfffffcff, op_and);
            writeReg(BB_OUTPUT_CONTROL, 0x00000200, op_or);
            udelay(150);
               
            /* CH_integer: Frequency register 0 */ 
            WriteRF(hw, 0, 0x01f79 );

            /* CH_fraction: Frequency register 1 */		
            WriteRF(hw, 1, 0x03333 );

            /*Config 1 = default value */
            WriteRF(hw, 2, 0x00B80 );

            /*Config 2 = default value */		
            WriteRF(hw, 3, 0x00E7F );

            /*Config 3 = default value */		
            WriteRF(hw, 4, 0x0905A );

            /*Config 4 = default value */
            WriteRF(hw, 5, 0x0F4DC );

            /*Config 5 = default value */
            WriteRF(hw, 6, 0x0805B );

            /*Config 6 = Crystal frequency /2 to pll reference divider */
            WriteRF(hw, 7, 0x0116C );

            /*Config 7 = RSSI = default value */
            WriteRF(hw, 8, 0x05B68 );   

            /* TX gain control for LA2236 */
            WriteRF(hw, 9, 0x05460 );   // sit at the middle

            /* RX Gain = digi specific value: AGC adjustment is done over the GC1-GC7 
            IC pins interface. AGC MAX GAIN value is configured in the FPGA BB register 
            instead of the RF register here below */
            WriteRF(hw, 10, 0x001BB );

            /* TX Gain = digi specific vaue: TX GAIN set using the register */
            WriteRF(hw, 11, 0x000f9 );   
               
            /* PA current = default value */
            WriteRF(hw, 12, 0x039D8 );

            /* Config 8 = default value  */
            WriteRF(hw, 13, 0x08000 );

            /* Config 9 = default value */
            WriteRF(hw, 14, 0x00000 );

            /* Config 10 = default value  */
            WriteRF(hw, 15, 0x00587 );

            //MacSetTxPower (macParams.tx_power);

            /* Calibration procedure */
            writeReg(BB_OUTPUT_CONTROL, 0x00000300, op_or);
            udelay(150);
    
            /* TXDCOC->active; RCK->disable */
            WriteRF(hw, 15, 0x00D87 );
            udelay(50);

            /* TXDCOC->disable; RCK->enable */		
            WriteRF(hw, 15, 0x00787 );	
            udelay(50);	

            /* TXDCOC->disable; RCK->disable */
            WriteRF(hw, 15, 0x00587 );	
            udelay(50);	
        
#elif (TRANSCEIVER == RF_AIROHA_7230)      
            switch(band_selection)
            {	
                case IEEE80211_BAND_2GHZ:
                    
                    /* Initial settings for 20 MHz reference frequency, 802.11b/g */
                    writeReg(BB_OUTPUT_CONTROL, 0xfffffcff, op_and);
                    writeReg(BB_OUTPUT_CONTROL, 0x00000200, op_or);
                    udelay(150);

                    /* Frequency register 0 */ 
                    WriteRF(hw, 0, 0x00379 );

                    /* Frequency register 1 */		
                    WriteRF(hw, 1, 0x13333 );
                    udelay(10);

                    /*Config 1 = default value */
                    WriteRF(hw, 2, 0x841FF );

                    /*Config 2 = default value */		
                    WriteRF(hw, 3, 0x3FDFA );

                    /*Config 3 = default value */		
                    WriteRF(hw, 4, 0x7FD78 );

                    /*Config 4 = default value */
                    WriteRF(hw, 5, 0x802BF );

                    /*Config 5 = default value */
                    WriteRF(hw, 6, 0x56AF3 );

                    /*Config 6 = Crystal frequency /2 to pll reference divider */
                    WriteRF(hw, 7, 0xCE000 );

                    /*Config 7 = RSSI = default value */
                    WriteRF(hw, 8, 0x6EBC0 );

                    /* Filter BW  = default value */
                    WriteRF(hw, 9, 0x221BB );

                    /* RX Gain = digi specific value: AGC adjustment is done over the GC1-GC7 
                    IC pins interface. AGC MAX GAIN value is configured in the FPGA BB register 
                    instead of the RF register here below */
                    WriteRF(hw, 10, 0xE0040 );   

                    /* TX Gain = digi specific vaue: TX GAIN set using the register */
                    // WriteRF(hw, 11, 0x08070);
                    MacSetTxPower (priv->tx_power);  //Digi value                                        

                    /* PA current = default value */
                    WriteRF(hw, 12, 0x000A3 );

                    /* Config 8 = default value  */
                    WriteRF(hw, 13, 0xFFFFF );

                    /* Config 9 = default value */
                    WriteRF(hw, 14, 0x00000 );

                    /* Config 10 = default value  */
                    WriteRF(hw, 15, 0x1ABA8 );

                    /* Calibration procedure */
                    writeReg(BB_OUTPUT_CONTROL, 0x00000300, op_or);

                    udelay(150);

                    /* Calibration procedure */

                    /* TXDCOC->active; RCK->disable */
                    WriteRF(hw, 15, 0x9ABA8 );
                    udelay(50);

                    /* TXDCOC->disable; RCK->enable */		
                    WriteRF(hw, 15, 0x3ABA8 );	
                    udelay(50);	

                    /* TXDCOC->disable; RCK->disable */
                    WriteRF(hw, 15, 0x1ABA8 );
                    udelay(50);				
                   
                    break;
                
                case IEEE80211_BAND_5GHZ:
                
                    /* Initial settings for 20 MHz reference frequency, 802.11a */
                    writeReg(BB_OUTPUT_CONTROL, 0xfffffcff, op_and);
                    writeReg(BB_OUTPUT_CONTROL, 0x00000200, op_or);
                    udelay(150);


                    /* Frequency register 0 */ 
                    WriteRF(hw, 0, 0x0FF56 );

                    /* Frequency register 1 */		
                    WriteRF(hw, 1, 0x0AAAA );
                    
                    udelay(10); 

                    /*Config 1 = default value */
                    WriteRF(hw, 2, 0x451FE );

                    /*Config 2 = default value */		
                    WriteRF(hw, 3, 0x5FDFA );

                    /*Config 3 = default value */		
                    WriteRF(hw, 4, 0x67f78 );

                    /*Config 4 = default value */
                    WriteRF(hw, 5, 0x853FF );

                    /*Config 5 = default value */
                    WriteRF(hw, 6, 0x56AF3 );

                    /*Config 6 = Crystal frequency /2 to pll reference divider */
                    WriteRF(hw, 7, 0xCE000 );

                    /*Config 7 = RSSI = default value */
                    WriteRF(hw, 8, 0x6EBC0 );

                    /* Filter BW  = default value */
                    WriteRF(hw, 9, 0x221BB );

                    /* RX Gain = digi value */
                    WriteRF(hw, 10, 0xE0600 ); 

                    /* TX Gain = digi specific vaue: TX GAIN set using the register */
                    // WriteRF(hw, 11, 0x08070 );  
                    MacSetTxPower (priv->tx_power);  //Digi value                    

                    /* PA current = default value */
                    WriteRF(hw, 12, 0x00143 );

                    /* Config 8 = default value  */
                    WriteRF(hw, 13, 0xFFFFF );

                    /* Config 9 = default value */
                    WriteRF(hw, 14, 0x00000 );

                    /* Config 10 = default value  */
                    WriteRF(hw, 15, 0x12BAC );

                    /* Calibration procedure */
                    writeReg(BB_OUTPUT_CONTROL, 0x00000300, op_or);

                    udelay(150);

                    /* Calibration procedure */

                    /* TXDCOC->active; RCK->disable */
                    WriteRF(hw, 15, 0x9ABA8 );
                    udelay(50);

                    /* TXDCOC->disable; RCK->enable */		
                    WriteRF(hw, 15, 0x3ABA8 );
                    udelay(50);		

                    /* TXDCOC->disable; RCK->disable */
                    WriteRF(hw, 15, 0x12BAC );	
                    udelay(50);
                    break;
            }
#endif    
}





static int al7230_rf_stop(struct ieee80211_hw *hw)
{
	return 0;
}


static void getOfdmBrs(u64 brsBitMask, unsigned int *ofdm, unsigned int *psk)
{
    /*
     * brsBitMask is a bit mask into the al7230_bg_rates array.  Bit 0 refers
     * to the first entry in the array, bit 1 the second, and so on.  The first
     * 4 bits/array entries refer to the PSK bit rates we support, the next 8
     * bits/array entries refer to the OFDM rates we support.  So the PSK BRS
     * mask is bits 0-3, the OFDM bit mask is bits 4-11.
     */
     
     *psk = brsBitMask & 0xf;
     *ofdm = (brsBitMask & 0xff0) >> 4;
}


    

static struct ieee80211_supported_band al7230_bands[] = {
	{
        	.band = IEEE80211_BAND_2GHZ,
        	.n_channels = ARRAY_SIZE(al7230_bg_channels),
        	.n_bitrates = ARRAY_SIZE(al7230_bg_rates),
        	.channels = (struct ieee80211_channel *) al7230_bg_channels,
        	.bitrates = (struct ieee80211_rate *) al7230_bg_rates,
	},
};


struct digi_rf_ops al7230_rf_ops = {
	.name = "Airoha 7230",
	.init = InitializeRF,
	.stop = al7230_rf_stop,
	.set_chan = al7230_rf_set_chan,
	.set_pwr = al7230_set_txpwr,
	.channelChangeTime = CHANNEL_CHANGE_TIME,
	.getOfdmBrs = getOfdmBrs,

	.bands = al7230_bands,
	.n_bands = ARRAY_SIZE(al7230_bands),
};
EXPORT_SYMBOL_GPL(al7230_rf_ops);
