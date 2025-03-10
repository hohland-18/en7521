/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2011, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rt5592_ate.c

	Abstract:
	Specific ATE funcitons and variables for RT5572/RT5592

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifdef RT5592

#include "rt_config.h"

#ifndef RTMP_RF_RW_SUPPORT
#error "You Should Enable compile flag RTMP_RF_RW_SUPPORT for this chip"
#endif /* RTMP_RF_RW_SUPPORT */

extern RT5592_FREQUENCY_PLAN RT5592_Frequency_Plan[];
extern REG_PAIR_BW RF5592Reg_BW_2G_5G[];
extern REG_PAIR RF5592Reg_CCK[];
extern REG_PAIR RF5592Reg_OFDM_2G[];
extern REG_PAIR RF5592Reg_OFDM_2G_5G[];
extern REG_PAIR BBP5592Reg_2G[];
extern REG_PAIR BBP5592Reg_GLRT_2G[];
extern REG_PAIR BBP5592Reg_5G[];
extern REG_PAIR BBP5592Reg_GLRT_5G[];
extern UCHAR NUM_RF5592REG_2G;				
extern UCHAR NUM_RF5592REG_5G;				
extern UCHAR NUM_RF5592REG_CHANNEL_2G;		
extern UCHAR NUM_RF5592REG_CHANNEL_5G;		
extern UCHAR NUM_RF5592REG_BW_2G_5G;		
extern UCHAR NUM_RF5592REG_CCK;			
extern UCHAR NUM_RF5592REG_OFDM_2G_5G;	
extern UCHAR NUM_RF5592REG_OFDM_2G;	
extern UCHAR NUM_BBP5592REG_2G;		
extern UCHAR NUM_BBP5592REG_5G;				
extern UCHAR NUM_BBP5592REG_GLRT_2G_5G;				
extern UCHAR NUM_BBP5592REG_GLRT_2G;		
extern UCHAR NUM_BBP5592REG_GLRT_5G;
#ifdef RTMP_MAC_PCI
extern REG_PAIR_CHANNEL RF5592Reg_Channel_2G[];
extern REG_PAIR_CHANNEL RF5592Reg_Channel_5G[];
extern REG_PAIR RF5592Reg_2G[];
extern REG_PAIR RF5592Reg_5G[];
#endif /* RTMP_MAC_PCI */

#define MDSM_NORMAL_TX_POWER							0x00
#define MDSM_DROP_TX_POWER_BY_6dBm						0x01
#define MDSM_DROP_TX_POWER_BY_12dBm						0x02
#define MDSM_ADD_TX_POWER_BY_6dBm						0x03
#define MDSM_BBP_R1_STATIC_TX_POWER_CONTROL_MASK		0x03

/* both for RT5572 and RT5592 */
VOID RT55x2ATEAsicSwitchChannel(
    IN PRTMP_ADAPTER pAd)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UINT32 Value = 0;
	CHAR TxPwer = 0, TxPwer2 = 0;
	UCHAR index = 0, Channel = 0, reg_index = 0;
	/* added to prevent RF register reading error */
	UCHAR RFValue = 0, PreRFValue = 0;
	enum XTAL Xtal;
	const struct _RT5592_FREQUENCY_ITEM *pFrequencyItem;
	INTERNAL_1_STRUCT Internal_1 = { { 0 } };

	SYNC_CHANNEL_WITH_QA(pATEInfo, &Channel);

	/* fill Tx power value */
	TxPwer = pATEInfo->TxPower0;
	TxPwer2 = pATEInfo->TxPower1;

	DBGPRINT(RT_DEBUG_TRACE,
		(" [%s] Channel = %d, pATEInfo->TxWI.BW = %d , pATEInfo->RFFreqOffset = %d, "
		"pATEInfo->TxPower0 = %d, pATEInfo->TxPower1 = %d\n", 
		__FUNCTION__, Channel, pATEInfo->TxWI.BW, pATEInfo->RFFreqOffset,
		pATEInfo->TxPower0, pATEInfo->TxPower1));

	if (IS_RT5592(pAd))
	{
		if (Channel > 14)
		{
			/* A band */
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R11, &RFValue);
			RFValue &= 0xF3;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R11, RFValue);

			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R09, &RFValue);
			RFValue &= 0x7F;
			RFValue |= 0x80;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R09, RFValue);

			/* In A band, the RF R36 bit[7] should be 0x0. */
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R36, &RFValue);
			RFValue &= 0x7F;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R36, RFValue);
		}
		else
		{
			/* G band */
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R11, &RFValue);
			RFValue &= 0xF3;
			RFValue |= 0x08;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R11, RFValue);

			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R09, &RFValue);
			RFValue &= 0x7F;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R09, RFValue);

			/* In G band, the RF R36 bit[7] should be 0x1. */
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R36, &RFValue);
			RFValue |= 0x80;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R36, RFValue);
		}		
	}
	RTMP_IO_READ32(pAd, LDO_CFG0, &Value);

	if (Channel <= 14)
	{
		if (pATEInfo->TxWI.BW == BW_40)
		{
			Value = ((Value & ~0x1C000000) | 0x14000000);
		}
		else
		{
			Value = ((Value & ~0x1C000000) | 0x00000000);
		}
	}
	else
	{
		Value = ((Value & ~0x1C000000) | 0x14000000);
	}

	RTMP_IO_WRITE32(pAd, LDO_CFG0, Value);

	RTMP_IO_READ32(pAd, DEBUG_INDEX, &Value);
	Xtal = (Value & 0x80000000) ? XTAL40M : XTAL20M;

	pFrequencyItem = RT5592_Frequency_Plan[Xtal].pFrequencyPlan;

	for (index = 0; index < RT5592_Frequency_Plan[Xtal].totalFreqItem; index++, pFrequencyItem++)
	{
		if (Channel == pFrequencyItem->Channel)
		{
			/* Frequeny plan setting */

			/*  
				N setting
				R9[4], R8[7:0] (RF PLL freq selection) 
			*/
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R08, &RFValue);
			RFValue = (pFrequencyItem->N & 0x00ff);
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R08, RFValue);

			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R09, &RFValue);
			RFValue = RFValue & ~0x10;
			RFValue |= ((pFrequencyItem->N & 0x0100) >> 8) << 4;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R09, RFValue);
			
			/* 
				K setting 
				R9[3:0] (RF PLL freq selection)
			*/
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R09, &RFValue);
			RFValue = RFValue & ~0x0f;
			RFValue |= (pFrequencyItem->K & 0x0f);
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R09, RFValue);

			/* 
				mode setting 
				R9[7] (RF PLL freq selection)
				R11[3:2] (RF PLL)
				mod=8 => 0x0
				mod=10 => 0x2
			*/
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R11, &RFValue);
			RFValue = RFValue & ~0x0c;
			RFValue |= ((pFrequencyItem->mod - 0x8) & 0x3) << 2;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R11, RFValue);

			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R09, &RFValue);
			RFValue = RFValue & ~0x80;
			RFValue |= (((pFrequencyItem->mod - 0x8) & 0x4) >> 2) << 7;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R09, RFValue);

			/* 
				R setting 
				R11[1:0]
				R=1 => 0x0
				R=3 => 0X2
			*/
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R11, &RFValue);
			RFValue = RFValue & ~0x03;
			RFValue |= (pFrequencyItem->R - 0x1);
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R11, RFValue);
	
			/* RF setting */
			if (Channel <= 14)
			{
				/* RF for G band */
				for (reg_index = 0; reg_index < NUM_RF5592REG_2G; reg_index++)
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_2G[reg_index].Register, 
							                   RF5592Reg_2G[reg_index].Value);
				/* RF for G band per channel */
				for (reg_index = 0; reg_index < NUM_RF5592REG_CHANNEL_2G; reg_index++)
				{
					if ((Channel >= RF5592Reg_Channel_2G[reg_index].FirstChannel) && 
						(Channel <= RF5592Reg_Channel_2G[reg_index].LastChannel))
						ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_Channel_2G[reg_index].Register, 
					                     	       RF5592Reg_Channel_2G[reg_index].Value);
				}

				if (pATEInfo->TxWI.PHYMODE == MODE_CCK)
				{
					/* RF for CCK */
					for (reg_index = 0; reg_index < NUM_RF5592REG_CCK; reg_index++)
						ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_CCK[reg_index].Register,
							                       RF5592Reg_CCK[reg_index].Value);
				} 
				else
				{
					/* RF for G band OFDM */
					for (reg_index = 0; reg_index < NUM_RF5592REG_OFDM_2G; reg_index++)
						ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_OFDM_2G[reg_index].Register,
									               RF5592Reg_OFDM_2G[reg_index].Value);
				}

				/* 
					R49 CH0 TX power ALC code(RF DAC value) 
					G-band bit<7:6>=1:0, bit<5:0> range from 0x0~0x27
				*/
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R49, &RFValue);
				RFValue = RFValue & ~0xC0;
				RFValue |= (0x2 << 6);

				RFValue = ((RFValue & ~0x3F) | (TxPwer & 0x3F));
				
				if ((RFValue & 0x3F) > 0x27)
					RFValue = ((RFValue & ~0x3F) | 0x27);
				
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R49, RFValue);

				/* 
					R50 CH0 TX power ALC code(RF DAC value) 
					G-band bit<7:6>=1:0, bit<5:0> range from 0x0~0x27
				*/
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R50, &RFValue);
				RFValue = RFValue & ~0xC0;
				RFValue |= (0x2 << 6);

				RFValue = ((RFValue & ~0x3F) | (TxPwer2 & 0x3F));
				
				if ((RFValue & 0x3F) > 0x27)
					RFValue = ((RFValue & ~0x3F) | 0x27);
				
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R50, RFValue);
			}
			else
			{
				/* RF for A band */
				for (reg_index = 0; reg_index < NUM_RF5592REG_5G; reg_index++)
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_5G[reg_index].Register, 
							                   RF5592Reg_5G[reg_index].Value);
				/* RF for A band per channel */
				for (reg_index = 0; reg_index < NUM_RF5592REG_CHANNEL_5G; reg_index++)
				{
					if ((Channel >= RF5592Reg_Channel_5G[reg_index].FirstChannel) && 
						(Channel <= RF5592Reg_Channel_5G[reg_index].LastChannel))
						ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_Channel_5G[reg_index].Register, 
					                     	       RF5592Reg_Channel_5G[reg_index].Value);
				}
					
				/* 
					R49 CH0 TX power ALC code(RF DAC value) 
					A-band bit<7:6>=1:1, bit<5:0> range from 0x0~0x2B
				*/
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R49, &RFValue);
				RFValue = RFValue & ~0xC0;

#ifdef RT5592EP_SUPPORT
				if (pAd->chipCap.Priv == RT5592_TYPE_EP)
					RFValue |= (0x0 << 6);
				else
#endif /* RT5592EP_SUPPORT */
				RFValue |= (0x3 << 6); 

				RFValue = ((RFValue & ~0x3F) | (TxPwer & 0x3F));
				
				if ((RFValue & 0x3F) > 0x2B)
					RFValue = ((RFValue & ~0x3F) | 0x2B);
				
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R49, RFValue);

				/* 
					R50 CH1 TX power ALC code(RF DAC value) 
					A-band bit<7:6>=1:1, bit<5:0> range from 0x0~0x2B
				*/
				ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R50, &RFValue);
				RFValue = RFValue & ~0xC0;

#ifdef RT5592EP_SUPPORT
				if (pAd->chipCap.Priv == RT5592_TYPE_EP)
					RFValue |= (0x0 << 6);
				else
#endif /* RT5592EP_SUPPORT */
				RFValue |= (0x3 << 6);

				RFValue = ((RFValue & ~0x3F) | (TxPwer2 & 0x3F));
				
				if ((RFValue & 0x3F) > 0x2B)
					RFValue = ((RFValue & ~0x3F) | 0x2B);
				
				ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R50, RFValue);
			}

			/* Enable RF block */
			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R01, &RFValue);

			/* Enable rf_block_en, pll_en */
			RFValue = ((RFValue & ~0x3) | 0x3);

			if (pAd->Antenna.field.TxPath == 2)
			{
				/* Enable tx0_en, tx1_en */
				RFValue = ((RFValue & ~0x28) | 0x28);
			}
			else if (pAd->Antenna.field.TxPath == 1)
			{
				/* Enable tx0_en */
				RFValue = ((RFValue & ~0x28) | 0x08);
			}

			if (pAd->Antenna.field.RxPath == 2)
			{
				/* Enable rx0_en, rx1_en */
				RFValue = ((RFValue & ~0x14) | 0x14);
			}
			else if (pAd->Antenna.field.RxPath == 1)
			{
				/* Enable rx0_en */
				RFValue = ((RFValue & ~0x14) | 0x04);
			}
			
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, RFValue);
		
			RFValue = 0xE4;
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R06, RFValue);

			/* RF for A/G band BW */
			for (reg_index = 0; reg_index < NUM_RF5592REG_BW_2G_5G; reg_index++)
			{
				if (pATEInfo->TxWI.BW == RF5592Reg_BW_2G_5G[reg_index].BW)
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_BW_2G_5G[reg_index].Register,
				                               RF5592Reg_BW_2G_5G[reg_index].Value);
			}

			/* RF for A/G band OFDM */
			if (pATEInfo->TxWI.PHYMODE != MODE_CCK)
			{
				for (reg_index = 0; reg_index < NUM_RF5592REG_OFDM_2G_5G; reg_index++)
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF5592Reg_OFDM_2G_5G[reg_index].Register,
							                   RF5592Reg_OFDM_2G_5G[reg_index].Value);
			}


			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R17, &RFValue);
			PreRFValue = RFValue;
		
			RFValue = ((RFValue & ~0x7F) | (pATEInfo->RFFreqOffset & 0x7F)); 

			RFValue = min((INT)RFValue, 0x5F);
			if (PreRFValue != RFValue)
			{
#ifdef RTMP_MAC_PCI
				if (IS_PCI_INF(pAd)) /* RT5592 */
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, RFValue);
#endif /* RTMP_MAC_PCI */

			}


			ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R03, (PUCHAR)&RFValue);
			RFValue = ((RFValue & ~0x80) | 0x80); /* vcocal_en (initiate VCO calibration (reset after completion)) - It should be at the end of RF configuration. */
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R03, (UCHAR)RFValue);

			/* must not remove it */
			pAd->LatchRfRegs.Channel = Channel; /* Channel latch */

			DBGPRINT(RT_DEBUG_TRACE,
				("%s : SwitchChannel#%d(RF=%d, Pwr0=%d, Pwr1=%d, %dT), "
				"N=0x%02X, K=0x%02X, R=0x%02X, Xtal=%d\n",
				__FUNCTION__,
				Channel,
				pAd->RfIcType,
				TxPwer,
				TxPwer2,
				pAd->Antenna.field.TxPath,
				pFrequencyItem->N,
				pFrequencyItem->K,
				pFrequencyItem->R,
				Xtal));

			break;
		}
	}

	ATEAsicSetTxRxPath(pAd);

	/* BBP setting */
	if (pATEInfo->TxWI.BW == BW_20)
	{
		/* set BBP R4 = 0x40 for BW = 20 MHz */
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, 0x40);
	}
	else
	{
		/* set BBP R4 = 0x50 for BW = 40 MHz */
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, 0x50);
	}

	if (Channel <= 14)
	{
		ULONG	TxPinCfg = 0x00050F0A;/* Gary 2007/08/09 0x050A0A */

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - GET_LNA_GAIN(pAd)));

		/* BBP for G band */
		for (reg_index = 0; reg_index < NUM_BBP5592REG_2G; reg_index++)
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP5592Reg_2G[reg_index].Register, BBP5592Reg_2G[reg_index].Value);

		/* BBP for G band GLRT */
		for (reg_index = 0; reg_index < NUM_BBP5592REG_GLRT_2G; reg_index++)
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, BBP5592Reg_GLRT_2G[reg_index].Register);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, BBP5592Reg_GLRT_2G[reg_index].Value);
		}

		/* 2.4G band selection PIN, bit1 and bit2 are complement.(bit2=1) */
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		Value &= (~0x6);
		Value |= (0x04);
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);


		/* Turn off unused PA or LNA when only 1T or 1R */
		if (pAd->Antenna.field.TxPath == 1)
			TxPinCfg &= 0xFFFFFFF3;
		if (pAd->Antenna.field.RxPath == 1)
			TxPinCfg &= 0xFFFFF3FF;

		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);

		if (IS_PCIE_INF(pAd))
		{
			/* CH#14 channel interference */
			RTMP_IO_READ32(pAd, INTERNAL_1, &Internal_1.word);

			if (Channel == 14) /* Channel #14 */
			{
				Internal_1.field.PCIE_PHY_TX_ATTEN_EN = 1; /* Enable PCIe PHY Tx attenuation */
				Internal_1.field.PCIE_PHY_TX_ATTEN_VALUE = 0; 
			}
			else /* Channel #1~#13 */
			{
				Internal_1.field.PCIE_PHY_TX_ATTEN_EN = 0; /* Disable PCIe PHY Tx attenuation */
				Internal_1.field.PCIE_PHY_TX_ATTEN_VALUE = 0; /* n/a */
			}

			RTMP_IO_WRITE32(pAd, INTERNAL_1, Internal_1.word);
		}
	}
	else
	{
		ULONG	TxPinCfg = 0x00050F05;/* Gary 2007/8/9 0x050505 */

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R62, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R63, (0x37 - GET_LNA_GAIN(pAd)));
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R64, (0x37 - GET_LNA_GAIN(pAd)));

		/* BBP for A band */
		for (reg_index = 0; reg_index < NUM_BBP5592REG_5G; reg_index++)
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP5592Reg_5G[reg_index].Register, BBP5592Reg_5G[reg_index].Value);

		/* BBP for A band GLRT */
		for (reg_index = 0; reg_index < NUM_BBP5592REG_GLRT_5G; reg_index++)
		{
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R195, BBP5592Reg_GLRT_5G[reg_index].Register);
			ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R196, BBP5592Reg_GLRT_5G[reg_index].Value);
		}

		/* 5G band selection PIN, bit1 and bit2 are complement.(bit1=1) */
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		Value &= (~0x6);
		Value |= (0x02);
		RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);

		/* Turn off unused PA or LNA when only 1T or 1R */
		if (pAd->Antenna.field.TxPath == 1)
			TxPinCfg &= 0xFFFFFFF3;

		if (pAd->Antenna.field.RxPath == 1)
			TxPinCfg &= 0xFFFFF3FF;

		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);
	}

	/* AGC VGA init value */
	ATE_CHIP_RX_VGA_GAIN_INIT(pAd);
	
#ifdef IQ_CAL_SUPPORT
	RT55x2ATEIQCompensation(pAd, pATEInfo->Channel);	
#endif /* IQ_CAL_SUPPORT */
	
	RtmpOsMsDelay(1);  
}


/* both for RT5572 and RT5592 */
INT RT55x2ATETxPwrHandler(
	IN PRTMP_ADAPTER pAd,
	IN char index)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	CHAR TxPower = 0;
	UCHAR RFValue = 0;
	UCHAR Channel = pATEInfo->Channel;

#ifdef RALINK_QA
	if ((pATEInfo->bQATxStart == TRUE) || (pATEInfo->bQARxStart == TRUE))
	{
		return 0;
	}
	else
#endif /* RALINK_QA */
	if (index == 0)
	{
		TxPower = pATEInfo->TxPower0;
	}
	else if (index == 1)
	{
		TxPower = pATEInfo->TxPower1;
	}
	else
	{
		DBGPRINT_ERR(("%s : Only TxPower0 and TxPower1 are adjustable !\n", __FUNCTION__));
		DBGPRINT_ERR(("%s : TxPower%d is out of range !\n", __FUNCTION__, index));
		return -1;
	}

	if (Channel <= 14) /* G band */
	{
		/* 
			R49 CH0 TX power ALC code(RF DAC value) 
			G-band bit<7:6>=1:0, bit<5:0> range from 0x0~0x27

			R50 CH0 TX power ALC code(RF DAC value) 
			G-band bit<7:6>=1:0, bit<5:0> range from 0x0~0x27
		*/
		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R49 + index, &RFValue);
		RFValue = RFValue & ~0xC0;
		RFValue |= (0x2 << 6);

		RFValue = ((RFValue & ~0x3F) | (TxPower & 0x3F));

		if ((RFValue & 0x3F) > 0x27)
			RFValue = ((RFValue & ~0x3F) | 0x27);

		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R49 + index, RFValue);
	}
	else /* A band */
	{
		/* 
			R49 CH0 TX power ALC code(RF DAC value) 
			A-band bit<7:6>=1:1, bit<5:0> range from 0x0~0x2B

			R50 CH1 TX power ALC code(RF DAC value) 
			A-band bit<7:6>=1:1, bit<5:0> range from 0x0~0x2B
		*/

		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R49 + index, &RFValue);
		RFValue = RFValue & ~0xC0;

#ifdef RT5592EP_SUPPORT
		if (pAd->chipCap.Priv == RT5592_TYPE_EP) /* MTK */
			RFValue |= (0x0 << 6);
		else
#endif /* RT5592EP_SUPPORT */
		RFValue |= (0x3 << 6);

		RFValue = ((RFValue & ~0x3F) | (TxPower & 0x3F));

		if ((RFValue & 0x3F) > 0x2B)
		RFValue = ((RFValue & ~0x3F) | 0x2B);

		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R49 + index, RFValue);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s : (TxPower%d=%d)\n", __FUNCTION__, index, TxPower));
	
	return 0;
}	


/* both for RT5572 and RT5592 */
VOID RT55x2ATERxVGAInit(
	IN PRTMP_ADAPTER		pAd)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UCHAR R66 = 0x30;
	CHAR LNAGain = GET_LNA_GAIN(pAd);
	
	if (pATEInfo->Channel <= 14)
		R66 = 0x1C + (2 * LNAGain);
	else
		R66 = 0x24 + (2 * LNAGain);

	ATEBBPWriteWithRxChain(pAd, BBP_R66, R66, RX_CHAIN_ALL);

	/* RX AGC LNA MM Select threshold */
	ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R83, 0x7A);


	return;
}


VOID RT55x2ATEAsicExtraPowerOverMAC(
	IN	PRTMP_ADAPTER 		pAd)
{
	ULONG	ExtraPwrOverMAC = 0;
	ULONG	ExtraPwrOverTxPwrCfg7 = 0, ExtraPwrOverTxPwrCfg8 = 0, ExtraPwrOverTxPwrCfg9 = 0;

	/* For OFDM_54 and HT_MCS_7, extra fill the corresponding register value into MAC 0x13D4 */
	RTMP_IO_READ32(pAd, 0x1318, &ExtraPwrOverMAC);  
	ExtraPwrOverTxPwrCfg7 |= (ExtraPwrOverMAC & 0x0000FF00) >> 8; /* Get Tx power for OFDM 54 */
	RTMP_IO_READ32(pAd, 0x131C, &ExtraPwrOverMAC);  
	ExtraPwrOverTxPwrCfg7 |= (ExtraPwrOverMAC & 0x0000FF00) << 8; /* Get Tx power for HT MCS 7 */			
	RTMP_IO_WRITE32(pAd, TX_PWR_CFG_7, ExtraPwrOverTxPwrCfg7);

	/* For STBC_MCS_7, extra fill the corresponding register value into MAC 0x13DC */
	RTMP_IO_READ32(pAd, 0x1324, &ExtraPwrOverMAC);  
	ExtraPwrOverTxPwrCfg9 |= (ExtraPwrOverMAC & 0x0000FF00) >> 8; /* Get Tx power for STBC MCS 7 */
	RTMP_IO_WRITE32(pAd, TX_PWR_CFG_9, ExtraPwrOverTxPwrCfg9);

	/*  For HT_MCS_15, extra fill the corresponding register value into MAC 0x13DC */
	RTMP_IO_READ32(pAd, 0x1320, &ExtraPwrOverMAC);  
	ExtraPwrOverTxPwrCfg8 |= (ExtraPwrOverMAC & 0x0000FF00) >> 8; /* Get Tx power for HT MCS 15 */
	RTMP_IO_WRITE32(pAd, TX_PWR_CFG_8, ExtraPwrOverTxPwrCfg8);
		
	DBGPRINT(RT_DEBUG_INFO, ("Offset =0x13D8, TxPwr = 0x%08X, ", (UINT)ExtraPwrOverTxPwrCfg8));
	
	DBGPRINT(RT_DEBUG_INFO, ("Offset = 0x13D4, TxPwr = 0x%08X, Offset = 0x13DC, TxPwr = 0x%08X\n", 
		(UINT)ExtraPwrOverTxPwrCfg7, 
		(UINT)ExtraPwrOverTxPwrCfg9));
}


VOID RT55x2ATEAsicGetTxPowerOffset(
	IN PRTMP_ADAPTER 			pAd,
	IN PULONG 					TxPwr)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	CONFIGURATION_OF_TX_POWER_CONTROL_OVER_MAC CfgOfTxPwrCtrlOverMAC;

	DBGPRINT(RT_DEBUG_INFO, ("-->%s\n", __FUNCTION__));

	NdisZeroMemory(&CfgOfTxPwrCtrlOverMAC, sizeof(CfgOfTxPwrCtrlOverMAC));

	CfgOfTxPwrCtrlOverMAC.NumOfEntries = 5; /* MAC 0x1314, 0x1318, 0x131C, 0x1320 and 1324 */

	if (pATEInfo->TxWI.BW == BW_40)
	{
		if (pATEInfo->Channel > 14)
		{
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].MACRegisterOffset = TX_PWR_CFG_0;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].RegisterValue = pAd->Tx40MPwrCfgABand[0];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].MACRegisterOffset = TX_PWR_CFG_1;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].RegisterValue = pAd->Tx40MPwrCfgABand[1];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].MACRegisterOffset = TX_PWR_CFG_2;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].RegisterValue = pAd->Tx40MPwrCfgABand[2];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].MACRegisterOffset = TX_PWR_CFG_3;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].RegisterValue = pAd->Tx40MPwrCfgABand[3];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].MACRegisterOffset = TX_PWR_CFG_4;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].RegisterValue = pAd->Tx40MPwrCfgABand[4];
		}
		else
		{
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].MACRegisterOffset = TX_PWR_CFG_0;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].RegisterValue = pAd->Tx40MPwrCfgGBand[0];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].MACRegisterOffset = TX_PWR_CFG_1;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].RegisterValue = pAd->Tx40MPwrCfgGBand[1];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].MACRegisterOffset = TX_PWR_CFG_2;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].RegisterValue = pAd->Tx40MPwrCfgGBand[2];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].MACRegisterOffset = TX_PWR_CFG_3;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].RegisterValue = pAd->Tx40MPwrCfgGBand[3];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].MACRegisterOffset = TX_PWR_CFG_4;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].RegisterValue = pAd->Tx40MPwrCfgGBand[4];
		}
	}
	else
	{
		if (pATEInfo->Channel > 14)
		{
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].MACRegisterOffset = TX_PWR_CFG_0;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].RegisterValue = pAd->Tx20MPwrCfgABand[0];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].MACRegisterOffset = TX_PWR_CFG_1;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].RegisterValue = pAd->Tx20MPwrCfgABand[1];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].MACRegisterOffset = TX_PWR_CFG_2;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].RegisterValue = pAd->Tx20MPwrCfgABand[2];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].MACRegisterOffset = TX_PWR_CFG_3;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].RegisterValue = pAd->Tx20MPwrCfgABand[3];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].MACRegisterOffset = TX_PWR_CFG_4;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].RegisterValue = pAd->Tx20MPwrCfgABand[4];
		}
		else
		{
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].MACRegisterOffset = TX_PWR_CFG_0;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[0].RegisterValue = pAd->Tx20MPwrCfgGBand[0];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].MACRegisterOffset = TX_PWR_CFG_1;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[1].RegisterValue = pAd->Tx20MPwrCfgGBand[1];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].MACRegisterOffset = TX_PWR_CFG_2;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[2].RegisterValue = pAd->Tx20MPwrCfgGBand[2];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].MACRegisterOffset = TX_PWR_CFG_3;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[3].RegisterValue = pAd->Tx20MPwrCfgGBand[3];
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].MACRegisterOffset = TX_PWR_CFG_4;
			CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[4].RegisterValue = pAd->Tx20MPwrCfgGBand[4];
		}
	}

	NdisCopyMemory(TxPwr, (UCHAR *)&CfgOfTxPwrCtrlOverMAC, sizeof(CfgOfTxPwrCtrlOverMAC));

	DBGPRINT(RT_DEBUG_OFF, ("<--%s\n", __FUNCTION__));
}


#ifdef RTMP_TEMPERATURE_COMPENSATION
VOID RT55x2ATEAsicGetAutoAgcOffsetForTemperatureSensor(
	IN PRTMP_ADAPTER 		pAd,
	IN PCHAR				pDeltaPwr,
	IN PCHAR				pTotalDeltaPwr,
	IN PCHAR				pAgcCompensate,
	IN PCHAR 				pDeltaPowerByBbpR1)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	const TX_POWER_TUNING_ENTRY_STRUCT *TxPowerTuningTable;
	TX_POWER_TUNING_ENTRY_STRUCT *TxPowerTuningTableEntry0 = NULL; /* Ant0 */
	TX_POWER_TUNING_ENTRY_STRUCT *TxPowerTuningTableEntry1 = NULL; /* Ant1 */
	BBP_R49_STRUC	BbpR49;
	BOOLEAN			bAutoTxAgc = FALSE;
	PCHAR			pTxAgcCompensate = NULL;
	UCHAR 			RFValue = 0;
	CHAR			TuningTableUpperBound = 0, TuningTableIndex0 = 0, TuningTableIndex1 = 0;
	INT				CurrentTemp = 0;
	INT RefTemp;
	INT *LookupTable;
	INT	LookupTableIndex = pAd->TxPowerCtrl.LookupTableIndex + TEMPERATURE_COMPENSATION_LOOKUP_TABLE_OFFSET;
	INT channel_index = 0;

	DBGPRINT(RT_DEBUG_INFO, ("-->%s\n", __FUNCTION__));
	
	BbpR49.byte = 0;
	*pTotalDeltaPwr = 0;

	/* Get calibrated per channel DAC. */
	for (channel_index=0; channel_index<MAX_NUM_OF_CHANNELS; channel_index++)
	{
		if (pATEInfo->Channel == pAd->TxPower[channel_index].Channel)
		{
			pATEInfo->TxPower0 = pAd->TxPower[channel_index].Power;
			pATEInfo->TxPower1 = pAd->TxPower[channel_index].Power2;
			break;
		}
	}

	if (channel_index == MAX_NUM_OF_CHANNELS)
	{
		DBGPRINT_ERR(("Channel DAC not found\n"));
		return;
	}

#ifdef A_BAND_SUPPORT
	if (pATEInfo->Channel > 14)
	{
		/* a band channel */
		bAutoTxAgc = pAd->bAutoTxAgcA;
		pTxAgcCompensate = &pAd->TxAgcCompensateA;
		TxPowerTuningTable = pChipCap->TxPowerTuningTable_5G;
		RefTemp = pAd->TxPowerCtrl.RefTemp[IEEE80211_BAND_5G];
		LookupTable = &pAd->TxPowerCtrl.LookupTable[IEEE80211_BAND_5G][0];
		TuningTableUpperBound = pChipCap->TxAlcTxPowerUpperBound_5G;
	}
	else
#endif /* A_BAND_SUPPORT */
	{
		/* bg band channel */
		bAutoTxAgc = pAd->bAutoTxAgcG;
		pTxAgcCompensate = &pAd->TxAgcCompensateG;
		TxPowerTuningTable = pChipCap->TxPowerTuningTable_2G;
		RefTemp = pAd->TxPowerCtrl.RefTemp[IEEE80211_BAND_2G];
		LookupTable = &pAd->TxPowerCtrl.LookupTable[IEEE80211_BAND_2G][0];
		TuningTableUpperBound = pChipCap->TxAlcTxPowerUpperBound_2G;
	}

	/* AutoTxAgc in EEPROM means temperature compensation enabled/diablded. */
	if (bAutoTxAgc)
	{ 
		/* Current temperature */
		ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R49, &BbpR49.byte);
		CurrentTemp = (CHAR)BbpR49.byte;

		DBGPRINT(RT_DEBUG_TRACE, ("\n\n\n"));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] BBP_R49 = %02x, current temp = %d\n", BbpR49.byte, CurrentTemp));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] RefTemp = %d\n", RefTemp));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] index = %d\n", pAd->TxPowerCtrl.LookupTableIndex));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] f(%d)= %d\n", pAd->TxPowerCtrl.LookupTableIndex - 1, LookupTable[LookupTableIndex - 1]));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] f(%d)= %d\n", pAd->TxPowerCtrl.LookupTableIndex, LookupTable[LookupTableIndex]));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] f(%d)= %d\n", pAd->TxPowerCtrl.LookupTableIndex + 1, LookupTable[LookupTableIndex + 1]));

		if (CurrentTemp > RefTemp + LookupTable[LookupTableIndex + 1] + ((LookupTable[LookupTableIndex + 1] - LookupTable[LookupTableIndex]) >> 2) &&
			LookupTableIndex < 32)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] ++\n"));
			LookupTableIndex++;
			pAd->TxPowerCtrl.LookupTableIndex++;
		}
		else if (CurrentTemp < RefTemp + LookupTable[LookupTableIndex] - ((LookupTable[LookupTableIndex] - LookupTable[LookupTableIndex - 1]) >> 2) &&
			LookupTableIndex > 0)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] --\n"));
			LookupTableIndex--;
			pAd->TxPowerCtrl.LookupTableIndex--;
		}
		else
		{
			DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] ==\n"));
		}

		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] idxTxPowerTable=%d, idxTxPowerTable2=%d, TuningTableUpperBound=%d\n",
			pAd->TxPowerCtrl.idxTxPowerTable + pAd->TxPowerCtrl.LookupTableIndex,
			pAd->TxPowerCtrl.idxTxPowerTable2 + pAd->TxPowerCtrl.LookupTableIndex,
			TuningTableUpperBound));

		TuningTableIndex0 = pAd->TxPowerCtrl.idxTxPowerTable 
									+ pAd->TxPowerCtrl.LookupTableIndex 
									+ pATEInfo->TxPower0;

		/* The boundary verification */ 
		TuningTableIndex0 = (TuningTableIndex0 > TuningTableUpperBound) ? TuningTableUpperBound : TuningTableIndex0;
		TuningTableIndex0 = (TuningTableIndex0 < LOWERBOUND_TX_POWER_TUNING_ENTRY) ? 
							LOWERBOUND_TX_POWER_TUNING_ENTRY : TuningTableIndex0;
		TxPowerTuningTableEntry0 = &TxPowerTuningTable[TuningTableIndex0 + TX_POWER_TUNING_ENTRY_OFFSET];
		
		TuningTableIndex1 = pAd->TxPowerCtrl.idxTxPowerTable2 
									+ pAd->TxPowerCtrl.LookupTableIndex 
									+ pATEInfo->TxPower1;

		/* The boundary verification */
		TuningTableIndex1 = (TuningTableIndex1 > TuningTableUpperBound) ? TuningTableUpperBound : TuningTableIndex1;
		TuningTableIndex1 = (TuningTableIndex1 < LOWERBOUND_TX_POWER_TUNING_ENTRY) ? 
							LOWERBOUND_TX_POWER_TUNING_ENTRY : TuningTableIndex1;
		TxPowerTuningTableEntry1 = &(TxPowerTuningTable[TuningTableIndex1 + TX_POWER_TUNING_ENTRY_OFFSET]);
			
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] (tx0)RF_TX_ALC = %x, MAC_PowerDelta = %d, TuningTableIndex = %d\n",
			TxPowerTuningTableEntry0->RF_TX_ALC, TxPowerTuningTableEntry0->MAC_PowerDelta, TuningTableIndex0));
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] (tx1)RF_TX_ALC = %x, MAC_PowerDelta = %d, TuningTableIndex = %d\n",
			TxPowerTuningTableEntry1->RF_TX_ALC, TxPowerTuningTableEntry1->MAC_PowerDelta, TuningTableIndex1));

		/* Update RF_R49 [0:5] */
		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R49, &RFValue);
		RFValue = ((RFValue & ~0x3F) | TxPowerTuningTableEntry0->RF_TX_ALC);
		if ((RFValue & 0x3F) > 0x27) /* The valid range of the RF R49 (<5:0>tx0_alc<5:0>) is 0x00~0x27 */
		{
			RFValue = ((RFValue & ~0x3F) | 0x27);
		}
		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] Update RF_R49[0:5] to 0x%x\n", TxPowerTuningTableEntry0->RF_TX_ALC));
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R49, RFValue);

		/* Update RF_R50 [0:5] */
		ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R50, &RFValue);
		RFValue = ((RFValue & ~0x3F) | TxPowerTuningTableEntry1->RF_TX_ALC);
		if ((RFValue & 0x3F) > 0x27) /* The valid range of the RF R49 (<5:0>tx0_alc<5:0>) is 0x00~0x27 */
		{
			RFValue = ((RFValue & ~0x3F) | 0x27);
		}

		DBGPRINT(RT_DEBUG_TRACE, ("[ATE temp. compensation] Update RF_R50[0:5] to 0x%x\n", TxPowerTuningTableEntry1->RF_TX_ALC));
		ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R50, RFValue);
		
		*pTotalDeltaPwr = TxPowerTuningTableEntry0->MAC_PowerDelta;
		
	}

	*pAgcCompensate = *pTxAgcCompensate;
	DBGPRINT(RT_DEBUG_INFO, ("<--%s\n", __FUNCTION__));
}
#endif /* RTMP_TEMPERATURE_COMPENSATION */


VOID ATEAsicCompensatePowerViaBBP(
	IN 		PRTMP_ADAPTER 		pAd,
	INOUT	PCHAR				pTotalDeltaPower) 
{
	UCHAR		BbpR1 = 0;
	
	DBGPRINT(RT_DEBUG_TRACE, ("%s: <Before BBP R1> TotalDeltaPower = %d dBm\n", __FUNCTION__, *pTotalDeltaPower));

	/* The BBP R1 controls the transmit power for all rates */
	ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpR1);
	BbpR1 &= ~MDSM_BBP_R1_STATIC_TX_POWER_CONTROL_MASK;

	if (*pTotalDeltaPower <= -12)
	{
		*pTotalDeltaPower += 12;
		BbpR1 |= MDSM_DROP_TX_POWER_BY_12dBm;

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpR1);

		DBGPRINT(RT_DEBUG_TRACE, ("%s: Drop the transmit power by 12 dBm (BBP R1)\n", __FUNCTION__));
	}
	else if ((*pTotalDeltaPower <= -6) && (*pTotalDeltaPower > -12))
	{
		*pTotalDeltaPower += 6;
		BbpR1 |= MDSM_DROP_TX_POWER_BY_6dBm;		

		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpR1);

		DBGPRINT(RT_DEBUG_TRACE, ("%s: Drop the transmit power by 6 dBm (BBP R1)\n", __FUNCTION__));
	}
	else
	{
		/* Control the the transmit power by using the MAC only */
		ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpR1);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s: <After BBP R1> TotalDeltaPower = %d dBm, BbpR1 = 0x%02X \n", __FUNCTION__, *pTotalDeltaPower, BbpR1));
}


/*
==========================================================================
	Description:
		Gives CCK TX rate 2 more dB TX power.
		This routine works only in ATE mode.

		calculate desired Tx power in RF R3.Tx0~5,	should consider -
		0. if current radio is a noisy environment (pAd->DrsCounters.fNoisyEnvironment)
		1. TxPowerPercentage
		2. auto calibration based on TSSI feedback
		3. extra 2 db for CCK
		4. -10 db upon very-short distance (AvgRSSI >= -40db) to AP

	NOTE: Since this routine requires the value of (pAd->DrsCounters.fNoisyEnvironment),
		it should be called AFTER MlmeDynamicTxRatSwitching()
==========================================================================
*/
VOID RT55x2ATEAsicAdjustTxPower(
	IN PRTMP_ADAPTER pAd) 
{
	INT			i, j;
	CHAR 		Value;
/*	CHAR		Rssi = -127; */
	CHAR		DeltaPwr = 0;
	CHAR		TxAgcCompensate = 0;
	CHAR		DeltaPowerByBbpR1 = 0; 
	CHAR		TotalDeltaPower = 0; /* (non-positive number) including the transmit power controlled by the MAC and the BBP R1 */
	CONFIGURATION_OF_TX_POWER_CONTROL_OVER_MAC CfgOfTxPwrCtrlOverMAC = {0};	

	/* Get Tx rate offset table which from EEPROM 0xDEh ~ 0xEFh */
	RT55x2ATEAsicGetTxPowerOffset(pAd, (PULONG)&CfgOfTxPwrCtrlOverMAC);
#ifdef RTMP_TEMPERATURE_COMPENSATION
	/* Get temperature compensation delta power value */
	RT55x2ATEAsicGetAutoAgcOffsetForTemperatureSensor(
		pAd, &DeltaPwr, &TotalDeltaPower, &TxAgcCompensate, &DeltaPowerByBbpR1);
#endif /* RTMP_TEMPERATURE_COMPENSATION */

	DBGPRINT(RT_DEBUG_INFO, ("%s: DeltaPwr=%d, TotalDeltaPower=%d, TxAgcCompensate=%d, DeltaPowerByBbpR1=%d\n",
			__FUNCTION__,
			DeltaPwr,
			TotalDeltaPower,
			TxAgcCompensate,
			DeltaPowerByBbpR1));


	/* The transmit power controlled by the BBP */
	TotalDeltaPower += DeltaPowerByBbpR1; 
	/* The transmit power controlled by the MAC */
	TotalDeltaPower += DeltaPwr; 	

	ATEAsicCompensatePowerViaBBP(pAd, &TotalDeltaPower); /* MTK */

	/* In ATE mode, power will be adjusted per sec */
	if (1 /* pATEInfo->OneSecPeriodicRound % 4 == 0 */)
	{
		/* Set new Tx power for different Tx rates */
		for (i=0; i < CfgOfTxPwrCtrlOverMAC.NumOfEntries; i++)
		{
			if (CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[i].RegisterValue != 0xffffffff)
			{	
				for (j=0; j<8; j++)
				{
					Value = (CHAR)((CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[i].RegisterValue >> j*4) & 0x0F);

#ifdef RTMP_TEMPERATURE_COMPENSATION
					/* The upper bounds of MAC 0x1314 ~ 0x1324 are variable */
					if ((pAd->TxPowerCtrl.bInternalTxALC == FALSE) && (pAd->chipCap.bTempCompTxALC == TRUE))
					{
						switch (0x1314 + (i * 4))
						{
							case 0x1314: 
							{
								if ((Value + TotalDeltaPower) < 0)
								{
									Value = 0;
								}
								else if ((Value + TotalDeltaPower) > 0xE)
								{
									Value = 0xE;
								}
								else
								{
									Value += TotalDeltaPower;
								}
							}
							break;

							case 0x1318: 
							{
								if ((j >= 0) && (j <= 3))
								{
									if ((Value + TotalDeltaPower) < 0)
									{
										Value = 0;
									}
									else if ((Value + TotalDeltaPower) > 0xC)
									{
										Value = 0xC;
									}
									else
									{
										Value += TotalDeltaPower;
									}
								}
								else
								{
									if ((Value + TotalDeltaPower) < 0)
									{
										Value = 0;
									}
									else if ((Value + TotalDeltaPower) > 0xE)
									{
										Value = 0xE;
									}
									else
									{
										Value += TotalDeltaPower;
									}
								}
							}
							break;

							case 0x131C: 
							{
								if ((j == 0) || (j == 2) || (j == 3))
								{
									if ((Value + TotalDeltaPower) < 0)
									{
										Value = 0;
									}
									else if ((Value + TotalDeltaPower) > 0xC)
									{
										Value = 0xC;
									}
									else
									{
										Value += TotalDeltaPower;
									}
								}
								else
								{
									if ((Value + TotalDeltaPower) < 0)
									{
										Value = 0;
									}
									else if ((Value + TotalDeltaPower) > 0xE)
									{
										Value = 0xE;
									}
									else
									{
										Value += TotalDeltaPower;
									}
								}
							}
							break;

							case 0x1320: 
							{
								if ((j == 0) || (j == 2) || (j == 3) || ((j >= 4) && (j <= 7)))
								{
									if ((Value + TotalDeltaPower) < 0)
									{
										Value = 0;
									}
									else if ((Value + TotalDeltaPower) > 0xC)
									{
										Value = 0xC;
									}
									else
									{
										Value += TotalDeltaPower;
									}
								}
								else
								{
									if ((Value + TotalDeltaPower) < 0)
									{
										Value = 0;
									}
									else if ((Value + TotalDeltaPower) > 0xE)
									{
										Value = 0xE;
									}
									else
									{
										Value += TotalDeltaPower;
									}
								}
							}
							break;

							case 0x1324: 
							{
								if ((Value + TotalDeltaPower) < 0)
								{
									Value = 0;
								}
								else if ((Value + TotalDeltaPower) > 0xC)
								{
									Value = 0xC;
								}
								else
								{
									Value += TotalDeltaPower;
								}
							}
							break;

							default: 
							{							
								/* do nothing */
								DBGPRINT_ERR(("%s: Unknown register = 0x%x\n", __FUNCTION__, (0x1314 + (i * 4))));
							}
							break;
						}
					}
					else
#endif /* RTMP_TEMPERATURE_COMPENSATION */
					{
						if ((Value + TotalDeltaPower) < 0)
						{
							Value = 0;
						}
						else if ((Value + TotalDeltaPower) > 0xC)
						{
							Value = 0xC;
						}
						else
						{
							Value += TotalDeltaPower; 
						}
					}

					/* Fill new value into the corresponding MAC offset */
					CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[i].RegisterValue = (CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[i].RegisterValue & ~(0x0000000F << j*4)) | (Value << j*4);
				}

				RTMP_IO_WRITE32(pAd, CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[i].MACRegisterOffset, CfgOfTxPwrCtrlOverMAC.TxPwrCtrlOverMAC[i].RegisterValue);

			}
		}

		/* It has been moved to ATEPeriodicExec */
		/* Extra set MAC registers to compensate Tx power if any */
/*		ATEAsicExtraPowerOverMAC(pAd); */
	}
}



/* 
==========================================================================
    Description:
        Set RT5572/RT5592 ATE RF BW
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	RT55x2_Set_ATE_TX_BW_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UCHAR value = 0;
	UCHAR BBPCurrentBW;
	
	BBPCurrentBW = simple_strtol(arg, 0, 10);

	if (BBPCurrentBW == 0)
	{
		pATEInfo->TxWI.BW = BW_20;
	}
	else
	{
		pATEInfo->TxWI.BW = BW_40;
 	}

	if ((pATEInfo->TxWI.PHYMODE == MODE_CCK) && (pATEInfo->TxWI.BW == BW_40))
	{
		DBGPRINT_ERR(("Set_ATE_TX_BW_Proc!! Warning!! CCK only supports 20MHZ!!\n"));
		DBGPRINT_ERR(("Bandwidth switch to 20!!\n"));
		pATEInfo->TxWI.BW = BW_20;
	}

	/* Turn on BBP 20MHz mode by request here. */
	if (pATEInfo->TxWI.BW == BW_20)
	{
		if (pATEInfo->Channel == 14)
		{
			INT TxMode = pATEInfo->TxWI.PHYMODE;

			if (TxMode == MODE_CCK)
			{
				/* when Channel==14 && Mode==CCK && BandWidth==20M, BBP R4 bit5=1 */
 				ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &value);
				value |= 0x20; /* set bit5=1 */
 				ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, value);				
			}
		}
	}

	return TRUE;
}	


VOID RT55x2ATEAsicSetTxRxPath(
    IN PRTMP_ADAPTER pAd)
{
	PATE_INFO   pATEInfo = &(pAd->ate);
	UCHAR	RFValue = 0, BbpValue = 0;

	RFValue = 0x03;

	/* Set TX path, pAd->TxAntennaSel : 0 -> All, 1 -> TX0, 2 -> TX1 */
	switch(pAd->Antenna.field.TxPath)
	{
		case 2:
			switch (pATEInfo->TxAntennaSel)
			{
				case 1:
					/* set BBP R1, bit 4:3 = 00 */
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
					BbpValue &= 0xE7;		/* 11100111B */
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);

					/* set RF R1, bit 7:5:3 = 001  */					
					RFValue &=  ~TXPowerEnMask;
					RFValue = RFValue | 0x08;
					break;
				case 2:	
					/* set BBP R1, bit 4:3 = 01 */
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
					BbpValue &= 0xE7;	
					BbpValue |= 0x08;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);

					/* set RF R1, bit 7:5:3 = 010 */					
					RFValue &=  ~TXPowerEnMask;
					RFValue = RFValue | 0x20;
					break;
				default:		
					/* set BBP R1, bit 4:3 = 10 */
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BbpValue);
					BbpValue &= 0xE7;
					BbpValue |= 0x10;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BbpValue);

					/* set RF R1, bit 7:5:3 = 011 */
					RFValue &=  ~TXPowerEnMask;
					RFValue = RFValue | 0x28;
				break;						
		}
		break;

	default:
			/* set RF R1, bit 7:5:3 = 011 */
			RFValue &=  ~TXPowerEnMask;
			RFValue = RFValue | 0x28;
			break;
	}

	/* Set RX path, pAd->RxAntennaSel : 0 -> All, 1 -> RX0, 2 -> RX1, 3 -> RX2 */
	switch (pAd->Antenna.field.RxPath)
	{
		case 3:
			switch (pATEInfo->RxAntennaSel)
			{
				case 1:
					/* set BBP R3, bit 4:3:1:0 = 0000 */							
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x00;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);

					/* set RF R1, bit 6:4:2 = 110 */
					ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R01, &RFValue);
					RFValue = RFValue & 0xAB;
					RFValue = RFValue | 0x50;
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, RFValue);
					break;
				case 2:
					/* set BBP R3, bit 4:3:1:0 = 0001	 */							
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x01;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);									

					/* set RF R1, bit 6:4:2 = 101 */
					ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R01, &RFValue);
					RFValue = RFValue & 0xAB;
					RFValue = RFValue | 0x44;
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, RFValue);
					break;
				case 3:	
					/* set BBP R3, bit 4:3:1:0 = 0002 */								
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x02;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);

					/* set RF R1, bit 6:4:2 = 011 */
					ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R01, &RFValue);
					RFValue = RFValue & 0xAB;
					RFValue = RFValue | 0x14;
					ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, RFValue);
					break;								
				default:	
					/* set BBP R3, bit 4:3:1:0 = 1000 */
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x10;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								

					/* set RF R1, bit 6:4:2 = 000 */
					/* RT30xxReadRFRegister(pAd, RF_R01, (PUCHAR)&Value); */
					/* Value = Value & 0xAB; */
					/* RT30xxWriteRFRegister(pAd, RF_R01, (UCHAR)Value); */
					break;
			}
			break;

		case 2:						
			switch (pATEInfo->RxAntennaSel)
		{
				case 1:	
					/* set BBP R3, bit 4:3:1:0 = 0000 */
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x00;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								

					/* set RF R1, bit 6:4:2 = 001 */				
					RFValue &=  ~RXPowerEnMask;
					RFValue |=  0x04;
					break;
				case 2:								
					/* set BBP R3, bit 4:3:1:0 = 0001 */
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x01;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);

					/* set RF R1, bit 6:4:2 = 010 */
					RFValue &=  ~RXPowerEnMask;
					RFValue |= 0x10;
					break;							
			default:
					/* set BBP R3, bit 4:3:1:0 = 0100 */		
					ATE_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BbpValue);
					BbpValue &= 0xE4;
					BbpValue |= 0x08;
					ATE_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BbpValue);								

					/* set RF R1, bit 6:4:2 = 011 */
					RFValue &=  ~RXPowerEnMask;
					RFValue |= 0x14;
				break;
		}
		break;		

		default:
			/* set RF R1, bit 6:4:2 = 011 */
			RFValue &=  ~RXPowerEnMask;
			RFValue |= 0x14;			
			break;		
	}

	ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R01, RFValue);

}


/* 
==========================================================================
    Description:
        Set RT5572/RT5592 ATE RF central frequency offset
        
    Return:
        TRUE if all parameters are OK, FALSE otherwise
==========================================================================
*/
INT	RT55x2_Set_ATE_TX_FREQ_OFFSET_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	PATE_INFO pATEInfo = &(pAd->ate);
	UCHAR RFFreqOffset = 0;
	UCHAR RFValue = 0;
	UCHAR PreRFValue = 0;
	
	RFFreqOffset = simple_strtol(arg, 0, 10);

	if (RFFreqOffset >= 96)
	{
		DBGPRINT_ERR(("Set_ATE_TX_FREQ_OFFSET_Proc::Out of range(0 ~ 95).\n"));
		return FALSE;
	}

	pATEInfo->RFFreqOffset = RFFreqOffset;

	ATE_RF_IO_READ8_BY_REG_ID(pAd, RF_R17, &RFValue);
	PreRFValue = RFValue;

	/* xo_code (C1 value control) - Crystal calibration */
	RFValue = ((RFValue & ~0x7F) | (pATEInfo->RFFreqOffset & 0x7F)); 
	RFValue = min((INT)RFValue, 0x5F);

	if (PreRFValue != RFValue)
	{
#ifdef RTMP_MAC_PCI
		if (IS_PCI_INF(pAd)) /* RT5592 */
			ATE_RF_IO_WRITE8_BY_REG_ID(pAd, RF_R17, RFValue);
#endif /* RTMP_MAC_PCI */

	}

	return TRUE;
}

#ifdef RTMP_MAC_PCI
struct _ATE_CHIP_STRUCT RALINK5592 =
{
	/* functions */
	.ChannelSwitch = RT55x2ATEAsicSwitchChannel,
	.TxPwrHandler = RT55x2ATETxPwrHandler,
	.TssiCalibration = NULL,
	.ExtendedTssiCalibration = NULL /* RT5592_ATETssiCalibrationExtend */,
	.RxVGAInit = RT55x2ATERxVGAInit,
	.AsicSetTxRxPath = RT55x2ATEAsicSetTxRxPath,
	.AdjustTxPower = RT55x2ATEAsicAdjustTxPower,
	.AsicExtraPowerOverMAC = RT55x2ATEAsicExtraPowerOverMAC,
	
	/* command handlers */
	.Set_BW_Proc = RT55x2_Set_ATE_TX_BW_Proc,
	.Set_FREQ_OFFSET_Proc = RT55x2_Set_ATE_TX_FREQ_OFFSET_Proc,

	/* variables */
	.maxTxPwrCnt = 5,
	.bBBPStoreTXCARR = FALSE,
	.bBBPStoreTXCARRSUPP = FALSE,	
	.bBBPStoreTXCONT = FALSE,
	.bBBPLoadATESTOP = FALSE,
};
#endif /* RTMP_MAC_PCI */


#endif /* RT5592 */

/* End of rt5592_ate.c */
