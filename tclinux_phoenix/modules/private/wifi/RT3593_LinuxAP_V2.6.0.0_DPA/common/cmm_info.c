/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Module Name:
	cmm_info.c
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */
 
#include	"rt_config.h"


INT	Show_SSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_TxBurst_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_TxPreamble_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_TxPower_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_Channel_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_BGProtection_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_RTSThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_FragThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

#ifdef DOT11_N_SUPPORT
INT	Show_HtBw_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtMcs_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtGi_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtOpMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtExtcha_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtMpduDensity_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtBaWinSize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtRdg_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtAmsdu_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_HtAutoBa_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);
#endif /* DOT11_N_SUPPORT */

INT	Show_CountryRegion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_CountryRegionABand_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_CountryCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

#ifdef AGGREGATION_SUPPORT
INT	Show_PktAggregate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);
#endif /* AGGREGATION_SUPPORT */

#ifdef WMM_SUPPORT
INT	Show_WmmCapable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);
#endif /* WMM_SUPPORT */

INT	Show_IEEE80211H_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);


INT	Show_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_EncrypType_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_DefaultKeyID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_Key1_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_Key2_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_Key3_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_Key4_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

INT	Show_PMK_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);

#ifdef WSC_STA_SUPPORT
INT	Show_WpsManufacturer_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf);

INT	Show_WpsModelName_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf);

INT	Show_WpsDeviceName_Proc(	
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf);

INT	Show_WpsModelNumber_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf);

INT	Show_WpsSerialNumber_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf);
#endif // WSC_STA_SUPPORT //

#ifdef SINGLE_SKU
INT	Show_ModuleTxpower_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen);
#endif /* SINGLE_SKU */

extern INT	Set_AP_WscConfStatus_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

extern INT	Set_AP_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

extern INT	Set_AP_EncrypType_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

extern INT	Set_AP_SSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

extern INT	Set_AP_WPAPSK_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

static struct {
	PSTRING name;
	INT (*show_proc)(PRTMP_ADAPTER pAdapter, PSTRING arg, ULONG BufLen);
} *PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC, RTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC[] = {
#ifdef DBG
	{"SSID",					Show_SSID_Proc}, 
	{"WirelessMode",			Show_WirelessMode_Proc},       
	{"TxBurst",					Show_TxBurst_Proc},
	{"TxPreamble",				Show_TxPreamble_Proc},
	{"TxPower",					Show_TxPower_Proc},
	{"Channel",					Show_Channel_Proc},            
	{"BGProtection",			Show_BGProtection_Proc},
	{"RTSThreshold",			Show_RTSThreshold_Proc},       
	{"FragThreshold",			Show_FragThreshold_Proc},      
#ifdef DOT11_N_SUPPORT
	{"HtBw",					Show_HtBw_Proc},
	{"HtMcs",					Show_HtMcs_Proc},
	{"HtGi",					Show_HtGi_Proc},
	{"HtOpMode",				Show_HtOpMode_Proc},
	{"HtExtcha",				Show_HtExtcha_Proc},
	{"HtMpduDensity",			Show_HtMpduDensity_Proc},
	{"HtBaWinSize",		        Show_HtBaWinSize_Proc},
	{"HtRdg",		        	Show_HtRdg_Proc},
	{"HtAmsdu",		        	Show_HtAmsdu_Proc},
	{"HtAutoBa",		        Show_HtAutoBa_Proc},
#endif /* DOT11_N_SUPPORT */
	{"CountryRegion",			Show_CountryRegion_Proc},
	{"CountryRegionABand",		Show_CountryRegionABand_Proc},
	{"CountryCode",				Show_CountryCode_Proc},
#ifdef AGGREGATION_SUPPORT
	{"PktAggregate",			Show_PktAggregate_Proc},       
#endif

#ifdef WMM_SUPPORT
	{"WmmCapable",				Show_WmmCapable_Proc},         
#endif         
	{"IEEE80211H",				Show_IEEE80211H_Proc},
	{"AuthMode",				Show_AuthMode_Proc},           
	{"EncrypType",				Show_EncrypType_Proc},         
	{"DefaultKeyID",			Show_DefaultKeyID_Proc},       
	{"Key1",					Show_Key1_Proc},               
	{"Key2",					Show_Key2_Proc},               
	{"Key3",					Show_Key3_Proc},               
	{"Key4",					Show_Key4_Proc},               
	{"PMK",						Show_PMK_Proc},
#ifdef SINGLE_SKU
	{"ModuleTxpower",			Show_ModuleTxpower_Proc},
#endif /* SINGLE_SKU */
#endif /* DBG */	
	{NULL, NULL}
};

/*
    ==========================================================================
    Description:
        Get Driver version.

    Return:
    ==========================================================================
*/
INT Set_DriverVersion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		DBGPRINT(RT_DEBUG_TRACE, ("Driver version-%s\n", AP_DRIVER_VERSION));
#endif /* CONFIG_AP_SUPPORT */


    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Country Region.
        This command will not work, if the field of CountryRegion in eeprom is programmed.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryRegion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int retval;
	
#ifdef EXT_BUILD_CHANNEL_LIST
	return -EOPNOTSUPP;
#endif /* EXT_BUILD_CHANNEL_LIST */

	retval = RT_CfgSetCountryRegion(pAd, arg, BAND_24G);
	if (retval == FALSE)
		return FALSE;
	
	/* if set country region, driver needs to be reset*/
	BuildChannelList(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_CountryRegion_Proc::(CountryRegion=%d)\n", pAd->CommonCfg.CountryRegion));
	
	return TRUE;
}


/* 
    ==========================================================================
    Description:
        Set Country Region for A band.
        This command will not work, if the field of CountryRegion in eeprom is programmed.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_CountryRegionABand_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int retval;

#ifdef EXT_BUILD_CHANNEL_LIST
	return -EOPNOTSUPP;
#endif /* EXT_BUILD_CHANNEL_LIST */

	retval = RT_CfgSetCountryRegion(pAd, arg, BAND_5G);
	if (retval == FALSE)
		return FALSE;

	/* if set country region, driver needs to be reset*/
	BuildChannelList(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_CountryRegionABand_Proc::(CountryRegion=%d)\n", pAd->CommonCfg.CountryRegionForABand));
	
	return TRUE;
}


INT	Set_Cmm_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg,
	IN	BOOLEAN			FlgIsDiffMbssModeUsed)
{
	INT	success = TRUE;
	UINT32 i = 0;

#ifdef CONFIG_AP_SUPPORT
#ifdef MBSS_SUPPORT
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	if (FlgIsDiffMbssModeUsed == 0)
		success = RT_CfgSetWirelessMode(pAd, arg);
	else
		success = RT_CfgSetMbssWirelessMode(pAd, arg);
#else
	success = RT_CfgSetWirelessMode(pAd, arg);
#endif /* MBSS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

	i = i; /* aviod compile warning */


	if (success)
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			/* recover Wmm Capable for "each" BSS */
			/* all phy mode of MBSS are the same */
			for(i=0; i<pAd->ApCfg.BssidNum; i++)
			{
				pAd->ApCfg.MBSSID[i].bWmmCapable = \
										pAd->ApCfg.MBSSID[i].bWmmCapableOrg;

#ifdef MBSS_SUPPORT
				/* In Same-MBSS Mode, all phy modes are the same */
				if (FlgIsDiffMbssModeUsed == 0)
					pAd->ApCfg.MBSSID[i].PhyMode = pAd->CommonCfg.PhyMode;
#endif /* MBSS_SUPPORT */
			}

			/* TODO: Is the function BuildChannelList() still necessary here, due to it also been called in RTMPSetPhyMode()!*/
			BuildChannelList(pAd);
			RTMPSetPhyMode(pAd, pAd->CommonCfg.PhyMode);
		}
#endif /* CONFIG_AP_SUPPORT */


		/* it is needed to set SSID to take effect*/
#ifdef DOT11_N_SUPPORT
		SetCommonHT(pAd);
#endif /* DOT11_N_SUPPORT */


#ifdef CONFIG_AP_SUPPORT
#ifdef MBSS_SUPPORT
		DBGPRINT(RT_DEBUG_TRACE, ("Set_Cmm_WirelessMode_Proc::(=%d)\n", pAd->CommonCfg.PhyMode));
		DBGPRINT(RT_DEBUG_TRACE, ("Set_Cmm_WirelessMode_Proc::(BSS%d=%d)\n",
				pObj->ioctl_if, pAd->ApCfg.MBSSID[pObj->ioctl_if].PhyMode));

		for(i=0; i<pAd->ApCfg.BssidNum; i++)
		{
			/*
				When last mode is not 11B-only, new mode is 11B, we need to re-make
				beacon frame content.

				Because we put support rate/extend support rate element in
				APMakeBssBeacon(), not APUpdateBeaconFrame().
			*/
			APMakeBssBeacon(pAd, i);
		}
#endif /* MBSS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */
	}
	else
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Set_WirelessMode_Proc::parameters out of range\n"));
	}
	
	return success;
}


#ifdef CONFIG_AP_SUPPORT
#ifdef MBSS_SUPPORT
/* 
    ==========================================================================
    Description:
        Set Wireless Mode for MBSS
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_MBSS_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	INT MaxPhyMode = PHY_11G;
	LONG WirelessMode;


	/* sanity check */
#ifdef DOT11_N_SUPPORT
	if (!RTMP_TEST_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N))
		MaxPhyMode = PHY_11N_5G;
#endif /* DOT11_N_SUPPORT */

	WirelessMode = simple_strtol(arg, 0, 10);
	if (WirelessMode > MaxPhyMode)
		return FALSE; /* out of range */

	if ((WirelessMode == PHY_11ABG_MIXED)
#ifdef DOT11_N_SUPPORT
		|| (WirelessMode == PHY_11ABGN_MIXED)
		|| (WirelessMode == PHY_11AGN_MIXED)
#endif /* DOT11_N_SUPPORT */
		)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("mbss> Wrong phy mode for AP!\n"));
		return FALSE;
	}

	/* assign wireless mode for the BSS */
	pAd->ApCfg.MBSSID[pObj->ioctl_if].PhyMode = WirelessMode;

	/*
		If the band is different with other BSS, we will correct it in
		RT_CfgSetMbssWirelessMode()
	*/
	return Set_Cmm_WirelessMode_Proc(pAd, arg, 1);
}
#endif /* MBSS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

/* 
    ==========================================================================
    Description:
        Set Wireless Mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	return Set_Cmm_WirelessMode_Proc(pAd, arg, 0);
}

/* 
    ==========================================================================
    Description:
        Set Channel
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Channel_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef CONFIG_AP_SUPPORT
	INT		i;
#endif /* CONFIG_AP_SUPPORT */
 	INT		success = TRUE;
	UCHAR	Channel;	

	Channel = (UCHAR) simple_strtol(arg, 0, 10);

	/* check if this channel is valid*/
	if (ChannelSanity(pAd, Channel) == TRUE)
	{
		success = TRUE;
	}
	else
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			Channel = FirstChannel(pAd);
			DBGPRINT(RT_DEBUG_WARN,("This channel is out of channel list, set as the first channel(%d) \n ", Channel));
		}
#endif /* CONFIG_AP_SUPPORT */

	}

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		if (((pAd->CommonCfg.PhyMode == PHY_11A)
#ifdef DOT11_N_SUPPORT
			|| (pAd->CommonCfg.PhyMode == PHY_11AN_MIXED)
#endif /* DOT11_N_SUPPORT */
			)
			&& (pAd->CommonCfg.bIEEE80211H == TRUE))
		{
			for (i = 0; i < pAd->ChannelListNum; i++)
			{
				if (pAd->ChannelList[i].Channel == Channel)
				{
					if (pAd->ChannelList[i].RemainingTimeForUse > 0)
					{
						DBGPRINT(RT_DEBUG_ERROR, ("ERROR: previous detection of a radar on this channel(Channel=%d)\n", Channel));
						success = FALSE;
						break;
					}
					else
					{
						DBGPRINT(RT_DEBUG_TRACE, ("RemainingTimeForUse %d ,Channel %d\n", pAd->ChannelList[i].RemainingTimeForUse, Channel));
					}
				}
			}
		}

		if (success == TRUE)
		{
			pAd->CommonCfg.Channel = Channel;
#ifdef DOT11_N_SUPPORT
			N_ChannelCheck(pAd);
#endif /* DOT11_N_SUPPORT */
			if ((pAd->CommonCfg.Channel > 14 )
				&& (pAd->CommonCfg.bIEEE80211H == TRUE))
			{
				if (pAd->CommonCfg.RadarDetect.RDMode == RD_SILENCE_MODE)
				{
					APStop(pAd);
					APStartUp(pAd);
				}
				else
				{
					NotifyChSwAnnToPeerAPs(pAd, ZERO_MAC_ADDR, pAd->CurrentAddress, 1, pAd->CommonCfg.Channel);
					pAd->CommonCfg.RadarDetect.RDMode = RD_SWITCHING_MODE;
					pAd->CommonCfg.RadarDetect.CSCount = 0;
				}
			}
			else
			{
				APStop(pAd);
				APStartUp(pAd);
			}
		}
	}
#endif /* CONFIG_AP_SUPPORT */

	if (success == TRUE)
		DBGPRINT(RT_DEBUG_TRACE, ("Set_Channel_Proc::(Channel=%d)\n", pAd->CommonCfg.Channel));

	return success;
}


/* 
    ==========================================================================
    Description:
        Set Short Slot Time Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ShortSlot_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int retval;
	
	retval = RT_CfgSetShortSlot(pAd, arg);
	if (retval == TRUE)
		DBGPRINT(RT_DEBUG_TRACE, ("Set_ShortSlot_Proc::(ShortSlot=%d)\n", pAd->CommonCfg.bUseShortSlotTime));

	return retval;
}


/* 
    ==========================================================================
    Description:
        Set Tx power
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxPower_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	LONG TxPower;
	INT   success = FALSE;

	TxPower = simple_strtol(arg, 0, 10);
	if (TxPower <= 100)
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			pAd->CommonCfg.TxPowerPercentage = TxPower;
#endif /* CONFIG_AP_SUPPORT */

		success = TRUE;
	}
	else
		success = FALSE;

	DBGPRINT(RT_DEBUG_TRACE, ("Set_TxPower_Proc::(TxPowerPercentage=%ld)\n", pAd->CommonCfg.TxPowerPercentage));

	return success;
}

/* 
    ==========================================================================
    Description:
        Set 11B/11G Protection
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_BGProtection_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	switch (simple_strtol(arg, 0, 10))
	{
		case 0: /*AUTO*/
			pAd->CommonCfg.UseBGProtection = 0;
			break;
		case 1: /*Always On*/
			pAd->CommonCfg.UseBGProtection = 1;
			break;
		case 2: /*Always OFF*/
			pAd->CommonCfg.UseBGProtection = 2;
			break;		
		default:  /*Invalid argument */
			return FALSE;
	}

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		APUpdateCapabilityAndErpIe(pAd);
#endif /* CONFIG_AP_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("Set_BGProtection_Proc::(BGProtection=%ld)\n", pAd->CommonCfg.UseBGProtection));	

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set TxPreamble
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxPreamble_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	RT_802_11_PREAMBLE	Preamble;

	Preamble = (RT_802_11_PREAMBLE)simple_strtol(arg, 0, 10);

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	if (Preamble == Rt802_11PreambleAuto)
		return FALSE;
#endif /* CONFIG_AP_SUPPORT */

	switch (Preamble)
	{
		case Rt802_11PreambleShort:
			pAd->CommonCfg.TxPreamble = Preamble;
			break;
		case Rt802_11PreambleLong:
			pAd->CommonCfg.TxPreamble = Preamble;
			break;
		default: /*Invalid argument */
			return FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Set_TxPreamble_Proc::(TxPreamble=%ld)\n", pAd->CommonCfg.TxPreamble));

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set RTS Threshold
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_RTSThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	 NDIS_802_11_RTS_THRESHOLD           RtsThresh;

	RtsThresh = simple_strtol(arg, 0, 10);

	if((RtsThresh > 0) && (RtsThresh <= MAX_RTS_THRESHOLD))
		pAd->CommonCfg.RtsThreshold  = (USHORT)RtsThresh;
	else
		return FALSE; /*Invalid argument */

	DBGPRINT(RT_DEBUG_TRACE, ("Set_RTSThreshold_Proc::(RTSThreshold=%d)\n", pAd->CommonCfg.RtsThreshold));

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Fragment Threshold
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_FragThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	 NDIS_802_11_FRAGMENTATION_THRESHOLD     FragThresh;

	FragThresh = simple_strtol(arg, 0, 10);

	if (FragThresh > MAX_FRAG_THRESHOLD || FragThresh < MIN_FRAG_THRESHOLD)
	{ 
		/*Illegal FragThresh so we set it to default*/
		pAd->CommonCfg.FragmentThreshold = MAX_FRAG_THRESHOLD;
	}
	else if (FragThresh % 2 == 1)
	{
		/*
			The length of each fragment shall always be an even number of octets, 
			except for the last fragment of an MSDU or MMPDU, which may be either 
			an even or an odd number of octets.
		*/
		pAd->CommonCfg.FragmentThreshold = (USHORT)(FragThresh - 1);
	}
	else
	{
		pAd->CommonCfg.FragmentThreshold = (USHORT)FragThresh;
	}


	DBGPRINT(RT_DEBUG_TRACE, ("Set_FragThreshold_Proc::(FragThreshold=%d)\n", pAd->CommonCfg.FragmentThreshold));

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set TxBurst
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_TxBurst_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	LONG TxBurst;

	TxBurst = simple_strtol(arg, 0, 10);
	if (TxBurst == 1)
		pAd->CommonCfg.bEnableTxBurst = TRUE;
	else if (TxBurst == 0)
		pAd->CommonCfg.bEnableTxBurst = FALSE;
	else
		return FALSE;  /*Invalid argument */
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_TxBurst_Proc::(TxBurst=%d)\n", pAd->CommonCfg.bEnableTxBurst));

	return TRUE;
}


#ifdef RTMP_MAC_PCI
INT Set_ShowRF_Proc(
	IN  PRTMP_ADAPTER		pAd,
	IN  PSTRING			arg)
{
	int ShowRF = simple_strtol(arg, 0, 10);
	
	if (ShowRF == 1)
		pAd->ShowRf = TRUE;
	else
		pAd->ShowRf = FALSE;
	
	return TRUE;
}
#endif /* RTMP_MAC_PCI */


#ifdef AGGREGATION_SUPPORT
/* 
    ==========================================================================
    Description:
        Set TxBurst
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_PktAggregate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	LONG aggre;

	aggre = simple_strtol(arg, 0, 10);

	if (aggre == 1)
		pAd->CommonCfg.bAggregationCapable = TRUE;
	else if (aggre == 0)
		pAd->CommonCfg.bAggregationCapable = FALSE;
	else
		return FALSE;  /*Invalid argument */

#ifdef CONFIG_AP_SUPPORT
#ifdef PIGGYBACK_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		pAd->CommonCfg.bPiggyBackCapable = pAd->CommonCfg.bAggregationCapable;
		RTMPSetPiggyBack(pAd, pAd->CommonCfg.bPiggyBackCapable);
	}
#endif /* PIGGYBACK_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("Set_PktAggregate_Proc::(AGGRE=%d)\n", pAd->CommonCfg.bAggregationCapable));

	return TRUE;
}
#endif


#ifdef INF_PPA_SUPPORT
INT	Set_INF_AMAZON_SE_PPA_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PUCHAR			arg)
{
	ULONG aggre;
	UINT status;
	
	aggre = simple_strtol(arg, 0, 10);

	if (aggre == 1)
	{
		if(pAd->PPAEnable==TRUE)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("INF_AMAZON_SE_PPA already enabled \n"));
		}
		else
		{
			if (ppa_hook_directpath_register_dev_fn) 
			{
				UINT32 g_if_id;
				
				if (pAd->pDirectpathCb == NULL) 
				{
/*					pAd->pDirectpathCb = (PPA_DIRECTPATH_CB *) kmalloc (sizeof(PPA_DIRECTPATH_CB), GFP_ATOMIC);*/
					os_alloc_mem(NULL, (UCHAR **)&(pAd->pDirectpathCb), sizeof(PPA_DIRECTPATH_CB));
					DBGPRINT(RT_DEBUG_TRACE, ("Realloc memory for  pDirectpathCb ??\n"));
				}

				/* register callback */
				pAd->pDirectpathCb->rx_fn = ifx_ra_start_xmit;
				pAd->pDirectpathCb->stop_tx_fn = NULL;
				pAd->pDirectpathCb->start_tx_fn = NULL;

				status = ppa_hook_directpath_register_dev_fn(&g_if_id, pAd->net_dev, pAd->pDirectpathCb, PPA_F_DIRECTPATH_REGISTER|PPA_F_DIRECTPATH_ETH_IF);

				if(status==IFX_SUCCESS)
				{
					pAd->g_if_id=g_if_id;
					DBGPRINT(RT_DEBUG_TRACE, ("register INF_AMAZON_SE_PPA success :ret:%d id:%d:%d\n",status,pAd->g_if_id,g_if_id));
					pAd->PPAEnable=TRUE;
				}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE, ("register INF_AMAZON_SE_PPA fail :ret:%d\n",status));
				}

			}
			else
			{
				DBGPRINT(RT_DEBUG_TRACE, ("INF_AMAZON_SE_PPA enable fail : there is no INF_AMAZON_SE_PPA module . \n"));
			}
		}

		
	}
	else if (aggre == 0)
	{
		if(pAd->PPAEnable==FALSE)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("INF_AMAZON_SE_PPA already disable \n"));
		}
		else
		{
			if (ppa_hook_directpath_register_dev_fn) 
			{
				UINT32 g_if_id;
				g_if_id=pAd->g_if_id;

				DBGPRINT(RT_DEBUG_TRACE, ("g_if_id=%d \n",pAd->g_if_id));
				status=ppa_hook_directpath_register_dev_fn(&g_if_id, pAd->net_dev, NULL, 0/*PPA_F_DIRECTPATH_REGISTER*/);

				if(status==1)
				{
					pAd->g_if_id=0;
					DBGPRINT(RT_DEBUG_TRACE, ("unregister INF_AMAZON_SE_PPA success :ret:%d\n",status));
					pAd->PPAEnable=FALSE;
				}
				else
				{
					DBGPRINT(RT_DEBUG_TRACE, ("unregister INF_AMAZON_SE_PPA fail :ret:%d\n",status));
				}

			}
			else
			{
				DBGPRINT(RT_DEBUG_TRACE, ("INF_AMAZON_SE_PPA enable fail : there is no INF_AMAZON_SE_PPA module . \n"));
			}
		}

	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Invalid argument %d \n",aggre));
		return FALSE;  /*Invalid argument */
	}	

	return TRUE;

}
#endif /* INF_PPA_SUPPORT */


/* 
    ==========================================================================
    Description:
        Set IEEE80211H.
        This parameter is 1 when needs radar detection, otherwise 0
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_IEEE80211H_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    LONG ieee80211h;

	ieee80211h = simple_strtol(arg, 0, 10);

	if (ieee80211h == 1)
		pAd->CommonCfg.bIEEE80211H = TRUE;
	else if (ieee80211h == 0)
		pAd->CommonCfg.bIEEE80211H = FALSE;
	else
		return FALSE;  /*Invalid argument */
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_IEEE80211H_Proc::(IEEE80211H=%d)\n", pAd->CommonCfg.bIEEE80211H));

	return TRUE;
}

#ifdef WSC_INCLUDED
INT	Set_WscGenPinCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    PWSC_CTRL   pWscControl = NULL;
    POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
    UCHAR	    apidx = pObj->ioctl_if;
    
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef APCLI_SUPPORT
	    if (pObj->ioctl_if_type == INT_APCLI)
	    {
	        pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
	        DBGPRINT(RT_DEBUG_TRACE, ("IF(apcli%d) Set_WscGenPinCode_Proc:: This command is from apcli interface now.\n", apidx));
	    }
	    else
#endif /* APCLI_SUPPORT */
	    {
			pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
	        DBGPRINT(RT_DEBUG_TRACE, ("IF(ra%d) Set_WscGenPinCode_Proc:: This command is from ra interface now.\n", apidx));
	    }
	}
#endif /* CONFIG_AP_SUPPORT */



	if (pWscControl == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: pWscControl == NULL!\n", __FUNCTION__));
		return TRUE;
	}

	if (pWscControl->WscEnrollee4digitPinCode)	
	{
		pWscControl->WscEnrolleePinCodeLen = 4;
		pWscControl->WscEnrolleePinCode = WscRandomGen4digitPinCode(pAd);
	}
	else
	{
		pWscControl->WscEnrolleePinCodeLen = 8;
		pWscControl->WscEnrolleePinCode = WscRandomGeneratePinCode(pAd, apidx);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Set_WscGenPinCode_Proc:: Enrollee PinCode\t\t%08u\n", pWscControl->WscEnrolleePinCode));

	return TRUE;
}

INT Set_WscVendorPinCode_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
	PWSC_CTRL   pWscControl;
	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;
	UCHAR       apidx = pObj->ioctl_if;

#ifdef CONFIG_AP_SUPPORT
/*	POS_COOKIE  pObj = (POS_COOKIE) pAd->OS_Cookie;*/
/*	UCHAR       apidx = pObj->ioctl_if;*/

	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef APCLI_SUPPORT
		if (pObj->ioctl_if_type == INT_APCLI)
		{
			pWscControl = &pAd->ApCfg.ApCliTab[apidx].WscControl;
			DBGPRINT(RT_DEBUG_TRACE, ("Set_WscVendorPinCode_Proc() for apcli(%d)\n", apidx));
		}
		else
#endif /* APCLI_SUPPORT */
		{
			pWscControl = &pAd->ApCfg.MBSSID[apidx].WscControl;
			DBGPRINT(RT_DEBUG_TRACE, ("Set_WscVendorPinCode_Proc() for ra%d!\n", apidx));
		}
	}
#endif /* CONFIG_AP_SUPPORT */



	return RT_CfgSetWscPinCode(pAd, arg, pWscControl);
}
#endif /* WSC_INCLUDED */

#ifdef DBG
/* 
    ==========================================================================
    Description:
        For Debug information
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_Debug_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	DBGPRINT(RT_DEBUG_TRACE, ("==> Set_Debug_Proc *******************\n"));

    if(simple_strtol(arg, 0, 10) <= RT_DEBUG_LOUD)
        RTDebugLevel = simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, ("<== Set_Debug_Proc(RTDebugLevel = %ld)\n", RTDebugLevel));

	return TRUE;
}
#endif

INT	Show_DescInfo_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef RTMP_MAC_PCI
	INT i, QueIdx=0;
/*  ULONG	RegValue;*/
        PRT28XX_RXD_STRUC pRxD;
        PTXD_STRUC pTxD;
#ifdef RT_BIG_ENDIAN
	PRT28XX_RXD_STRUC		pDestRxD;
	RXD_STRUC				RxD;
	PTXD_STRUC      pDestTxD;
	TXD_STRUC       TxD;	
#endif /* RT_BIG_ENDIAN */
	PRTMP_TX_RING	pTxRing = &pAd->TxRing[QueIdx];	
	PRTMP_MGMT_RING	pMgmtRing = &pAd->MgmtRing;	
	PRTMP_RX_RING	pRxRing = &pAd->RxRing;	
	
	for(i=0;i<TX_RING_SIZE;i++)
	{	
#ifdef RT_BIG_ENDIAN
		pDestTxD = (PTXD_STRUC) pTxRing->Cell[i].AllocVa;
		TxD = *pDestTxD;
		pTxD = &TxD;
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	    pTxD = (PTXD_STRUC) pTxRing->Cell[i].AllocVa;
#endif /* RT_BIG_ENDIAN */
	    DBGPRINT(RT_DEBUG_OFF, ("Desc #%d\n",i));
	    hex_dump("Tx Descriptor", (PUCHAR)pTxD, 16);
	    DBGPRINT(RT_DEBUG_OFF, ("pTxD->DMADONE = %x\n", pTxD->DMADONE));
	}    
	DBGPRINT(RT_DEBUG_OFF, ("---------------------------------------------------\n"));
	for(i=0;i<MGMT_RING_SIZE;i++)
	{	
#ifdef RT_BIG_ENDIAN
		pDestTxD = (PTXD_STRUC) pMgmtRing->Cell[i].AllocVa;
		TxD = *pDestTxD;
		pTxD = &TxD;
		RTMPDescriptorEndianChange((PUCHAR)pTxD, TYPE_TXD);
#else
	    pTxD = (PTXD_STRUC) pMgmtRing->Cell[i].AllocVa;
#endif /* RT_BIG_ENDIAN */
	    DBGPRINT(RT_DEBUG_OFF, ("Desc #%d\n",i));
	    hex_dump("Mgmt Descriptor", (PUCHAR)pTxD, 16);
	    DBGPRINT(RT_DEBUG_OFF, ("pMgmt->DMADONE = %x\n", pTxD->DMADONE));
	}    
	DBGPRINT(RT_DEBUG_OFF, ("---------------------------------------------------\n"));
	for(i=0;i<RX_RING_SIZE;i++)
	{	
#ifdef RT_BIG_ENDIAN
		pDestRxD = (PRT28XX_RXD_STRUC) pRxRing->Cell[i].AllocVa;
		RxD = *pDestRxD;
		pRxD = &RxD;
		RTMPDescriptorEndianChange((PUCHAR)pRxD, TYPE_RXD);
#else
	    pRxD = (PRT28XX_RXD_STRUC) pRxRing->Cell[i].AllocVa;
#endif /* RT_BIG_ENDIAN */
	    DBGPRINT(RT_DEBUG_OFF, ("Desc #%d\n",i));
	    hex_dump("Rx Descriptor", (PUCHAR)pRxD, 16);
	    DBGPRINT(RT_DEBUG_OFF, ("pRxD->DDONE = %x\n", pRxD->DDONE));
	}
#endif /* RTMP_MAC_PCI */

	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Reset statistics counter

    Arguments:
        pAd            Pointer to our adapter
        arg                 

    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_ResetStatCounter_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	/*UCHAR           i;*/
	/*MAC_TABLE_ENTRY *pEntry;*/
    
	DBGPRINT(RT_DEBUG_TRACE, ("==>Set_ResetStatCounter_Proc\n"));

	/* add the most up-to-date h/w raw counters into software counters*/
	NICUpdateRawCounters(pAd);
    
	NdisZeroMemory(&pAd->WlanCounters, sizeof(COUNTER_802_11));
	NdisZeroMemory(&pAd->Counters8023, sizeof(COUNTER_802_3));
	NdisZeroMemory(&pAd->RalinkCounters, sizeof(COUNTER_RALINK));

	/* Reset HotSpot counter*/

#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */

#ifdef TXBF_SUPPORT
	if (pAd->chipCap.FlgHwTxBfCap)
	{
		int i;
		for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
			NdisZeroMemory(&pAd->MacTab.Content[i].TxBFCounters, sizeof(pAd->MacTab.Content[i].TxBFCounters));
	}
#endif /* TXBF_SUPPORT */

	return TRUE;
}

/*
	========================================================================
	
	Routine Description:
		Add WPA key process.
		In Adhoc WPANONE, bPairwise = 0;  KeyIdx = 0;

	Arguments:
		pAd 					Pointer to our adapter
		pBuf							Pointer to the where the key stored

	Return Value:
		NDIS_SUCCESS					Add key successfully

	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/

BOOLEAN RTMPCheckStrPrintAble(
    IN  CHAR *pInPutStr, 
    IN  UCHAR strLen)
{
    UCHAR i=0;
    
    for (i=0; i<strLen; i++)
    {
        if ((pInPutStr[i] < 0x20) ||
            (pInPutStr[i] > 0x7E))
            return FALSE;
    }
    
    return TRUE;
}

/*
	========================================================================
	
	Routine Description:
		Remove WPA Key process

	Arguments:
		pAd 					Pointer to our adapter
		pBuf							Pointer to the where the key stored

	Return Value:
		NDIS_SUCCESS					Add key successfully

	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/

#if defined(CONFIG_STA_SUPPORT) || defined(APCLI_WPA_SUPPLICANT_SUPPORT)
NDIS_STATUS RTMPWPARemoveKeyProc(
	IN	PRTMP_ADAPTER	pAd,
	IN	PVOID			pBuf)
{
	PNDIS_802_11_REMOVE_KEY pKey;
	ULONG					KeyIdx;
	NDIS_STATUS 			Status = NDIS_STATUS_FAILURE;
	BOOLEAN 	bTxKey; 		/* Set the key as transmit key*/
	BOOLEAN 	bPairwise;		/* Indicate the key is pairwise key*/
	BOOLEAN 	bKeyRSC;		/* indicate the receive  SC set by KeyRSC value.*/
								/* Otherwise, it will set by the NIC.*/
	BOOLEAN 	bAuthenticator; /* indicate key is set by authenticator.*/
	INT 		i;
#ifdef APCLI_SUPPORT
#ifdef APCLI_WPA_SUPPLICANT_SUPPORT
	UCHAR ifIndex;
	BOOLEAN apcliEn=FALSE;
	INT 		idx, BssIdx;
	POS_COOKIE	pObj = (POS_COOKIE) pAd->OS_Cookie;
#endif/*APCLI_WPA_SUPPLICANT_SUPPORT*/
#endif/*APCLI_SUPPORT*/
	DBGPRINT(RT_DEBUG_TRACE,("---> RTMPWPARemoveKeyProc\n"));
	
	pKey = (PNDIS_802_11_REMOVE_KEY) pBuf;
	KeyIdx = pKey->KeyIndex & 0xff;
	/* Bit 31 of Add-key, Tx Key*/
	bTxKey		   = (pKey->KeyIndex & 0x80000000) ? TRUE : FALSE;
	/* Bit 30 of Add-key PairwiseKey*/
	bPairwise	   = (pKey->KeyIndex & 0x40000000) ? TRUE : FALSE;
	/* Bit 29 of Add-key KeyRSC*/
	bKeyRSC 	   = (pKey->KeyIndex & 0x20000000) ? TRUE : FALSE;
	/* Bit 28 of Add-key Authenticator*/
	bAuthenticator = (pKey->KeyIndex & 0x10000000) ? TRUE : FALSE;

	/* 1. If bTx is TRUE, return failure information*/
	if (bTxKey == TRUE)
		return(NDIS_STATUS_INVALID_DATA);

	/* 2. Check Pairwise Key*/
	if (bPairwise)
	{
		/* a. If BSSID is broadcast, remove all pairwise keys.*/
		/* b. If not broadcast, remove the pairwise specified by BSSID*/
		for (i = 0; i < SHARE_KEY_NUM; i++)
		{
#ifdef APCLI_SUPPORT
#ifdef APCLI_WPA_SUPPLICANT_SUPPORT
			if (pObj->ioctl_if_type == INT_APCLI)
			{
				/*if (MAC_ADDR_EQUAL(pAd->ApCfg.ApCliTab[ifIndex].SharedKey[i].BssId, pKey->BSSID)) */
				{
					ifIndex = pObj->ioctl_if;
					BssIdx = pAd->ApCfg.BssidNum + MAX_MESH_NUM + ifIndex;
					DBGPRINT(RT_DEBUG_TRACE,("APCLI RTMPWPARemoveKeyProc(KeyIdx=%d)\n", i));
					pAd->ApCfg.ApCliTab[ifIndex].SharedKey[i].KeyLen = 0;
					pAd->ApCfg.ApCliTab[ifIndex].SharedKey[i].CipherAlg = CIPHER_NONE;
					AsicRemoveSharedKeyEntry(pAd, BssIdx, (UCHAR)i);
					Status = NDIS_STATUS_SUCCESS;
					break;
				}
			}
			else
#endif/*APCLI_WPA_SUPPLICANT_SUPPORT*/
#endif/*APCLI_SUPPORT*/
			{
			}
		}
	}
	/* 3. Group Key*/
	else
	{
		/* a. If BSSID is broadcast, remove all group keys indexed*/
		/* b. If BSSID matched, delete the group key indexed.*/
		DBGPRINT(RT_DEBUG_TRACE,("RTMPWPARemoveKeyProc(KeyIdx=%ld)\n", KeyIdx));
		pAd->SharedKey[BSS0][KeyIdx].KeyLen = 0;
		pAd->SharedKey[BSS0][KeyIdx].CipherAlg = CIPHER_NONE;
		AsicRemoveSharedKeyEntry(pAd, BSS0, (UCHAR)KeyIdx);
		Status = NDIS_STATUS_SUCCESS;
	}

	return (Status);
}
#endif /* defined(CONFIG_STA_SUPPORT) || defined(APCLI_WPA_SUPPLICANT_SUPPORT) */




/*
	========================================================================
	
	Routine Description:
	 	As STA's BSSID is a WC too, it uses shared key table.
	 	This function write correct unicast TX key to ASIC WCID.
 	 	And we still make a copy in our MacTab.Content[BSSID_WCID].PairwiseKey.
		Caller guarantee TKIP/AES always has keyidx = 0. (pairwise key)
		Caller guarantee WEP calls this function when set Txkey,  default key index=0~3.
		
	Arguments:
		pAd 					Pointer to our adapter
		pKey							Pointer to the where the key stored

	Return Value:
		NDIS_SUCCESS					Add key successfully

	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
/*
	========================================================================
	Routine Description:
		Change NIC PHY mode. Re-association may be necessary. possible settings
		include - PHY_11B, PHY_11BG_MIXED, PHY_11A, and PHY_11ABG_MIXED 

	Arguments:
		pAd - Pointer to our adapter
		phymode  - 

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	========================================================================
*/
VOID	RTMPSetPhyMode(
	IN	PRTMP_ADAPTER	pAd,
	IN	ULONG phymode)
{
	INT i;
	/* the selected phymode must be supported by the RF IC encoded in E2PROM*/

	pAd->CommonCfg.PhyMode = (UCHAR)phymode;

	DBGPRINT(RT_DEBUG_TRACE,("RTMPSetPhyMode : PhyMode=%d, channel=%d \n", pAd->CommonCfg.PhyMode, pAd->CommonCfg.Channel));
#ifdef EXT_BUILD_CHANNEL_LIST
	BuildChannelListEx(pAd);
#else
	BuildChannelList(pAd);
#endif /* EXT_BUILD_CHANNEL_LIST */

	/* sanity check user setting*/
	for (i = 0; i < pAd->ChannelListNum; i++)
	{
		if (pAd->CommonCfg.Channel == pAd->ChannelList[i].Channel)
			break;
	}

	if (i == pAd->ChannelListNum)
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		if (pAd->CommonCfg.Channel != 0)
				pAd->CommonCfg.Channel = FirstChannel(pAd);
#endif /* CONFIG_AP_SUPPORT */
		DBGPRINT(RT_DEBUG_ERROR, ("RTMPSetPhyMode: channel is out of range, use first channel=%d \n", pAd->CommonCfg.Channel));
	}
	
	NdisZeroMemory(pAd->CommonCfg.SupRate, MAX_LEN_OF_SUPPORTED_RATES);
	NdisZeroMemory(pAd->CommonCfg.ExtRate, MAX_LEN_OF_SUPPORTED_RATES);
	NdisZeroMemory(pAd->CommonCfg.DesireRate, MAX_LEN_OF_SUPPORTED_RATES);
	switch (phymode) {
		case PHY_11B:
			pAd->CommonCfg.SupRate[0]  = 0x82;	  /* 1 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[1]  = 0x84;	  /* 2 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[2]  = 0x8B;	  /* 5.5 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[3]  = 0x96;	  /* 11 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRateLen  = 4;
			pAd->CommonCfg.ExtRateLen  = 0;
			pAd->CommonCfg.DesireRate[0]  = 2;	   /* 1 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[1]  = 4;	   /* 2 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[2]  = 11;    /* 5.5 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[3]  = 22;    /* 11 mbps, in units of 0.5 Mbps*/
			/*pAd->CommonCfg.HTPhyMode.field.MODE = MODE_CCK;  This MODE is only FYI. not use*/
			break;

		case PHY_11G:
		case PHY_11BG_MIXED:
		case PHY_11ABG_MIXED:
#ifdef DOT11_N_SUPPORT
		case PHY_11N_2_4G:
		case PHY_11ABGN_MIXED:
		case PHY_11BGN_MIXED:
		case PHY_11GN_MIXED:
#endif /* DOT11_N_SUPPORT */
			pAd->CommonCfg.SupRate[0]  = 0x82;	  /* 1 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[1]  = 0x84;	  /* 2 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[2]  = 0x8B;	  /* 5.5 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[3]  = 0x96;	  /* 11 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[4]  = 0x12;	  /* 9 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[5]  = 0x24;	  /* 18 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[6]  = 0x48;	  /* 36 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[7]  = 0x6c;	  /* 54 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRateLen  = 8;
			pAd->CommonCfg.ExtRate[0]  = 0x0C;	  /* 6 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.ExtRate[1]  = 0x18;	  /* 12 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.ExtRate[2]  = 0x30;	  /* 24 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.ExtRate[3]  = 0x60;	  /* 48 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.ExtRateLen  = 4;
			pAd->CommonCfg.DesireRate[0]  = 2;	   /* 1 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[1]  = 4;	   /* 2 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[2]  = 11;    /* 5.5 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[3]  = 22;    /* 11 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[4]  = 12;    /* 6 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[5]  = 18;    /* 9 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[6]  = 24;    /* 12 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[7]  = 36;    /* 18 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[8]  = 48;    /* 24 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[9]  = 72;    /* 36 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[10] = 96;    /* 48 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[11] = 108;   /* 54 mbps, in units of 0.5 Mbps*/
			break;

		case PHY_11A:
#ifdef DOT11_N_SUPPORT
		case PHY_11AN_MIXED:
		case PHY_11AGN_MIXED:
		case PHY_11N_5G:
#endif /* DOT11_N_SUPPORT */
			pAd->CommonCfg.SupRate[0]  = 0x8C;	  /* 6 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[1]  = 0x12;	  /* 9 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[2]  = 0x98;	  /* 12 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[3]  = 0x24;	  /* 18 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[4]  = 0xb0;	  /* 24 mbps, in units of 0.5 Mbps, basic rate*/
			pAd->CommonCfg.SupRate[5]  = 0x48;	  /* 36 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[6]  = 0x60;	  /* 48 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRate[7]  = 0x6c;	  /* 54 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.SupRateLen  = 8;
			pAd->CommonCfg.ExtRateLen  = 0;
			pAd->CommonCfg.DesireRate[0]  = 12;    /* 6 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[1]  = 18;    /* 9 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[2]  = 24;    /* 12 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[3]  = 36;    /* 18 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[4]  = 48;    /* 24 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[5]  = 72;    /* 36 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[6]  = 96;    /* 48 mbps, in units of 0.5 Mbps*/
			pAd->CommonCfg.DesireRate[7]  = 108;   /* 54 mbps, in units of 0.5 Mbps*/
			/*pAd->CommonCfg.HTPhyMode.field.MODE = MODE_OFDM;  This MODE is only FYI. not use*/
			break;

		default:
			break;
	}

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		UINT	apidx;
		
		for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
		{
			MlmeUpdateTxRates(pAd, FALSE, apidx);
		}	
#ifdef WDS_SUPPORT
		for (apidx = 0; apidx < MAX_WDS_ENTRY; apidx++)
		{				
			MlmeUpdateTxRates(pAd, FALSE, apidx + MIN_NET_DEVICE_FOR_WDS);			
		}
#endif /* WDS_SUPPORT */
#ifdef APCLI_SUPPORT
		for (apidx = 0; apidx < MAX_APCLI_NUM; apidx++)
		{				
			MlmeUpdateTxRates(pAd, FALSE, apidx + MIN_NET_DEVICE_FOR_APCLI);			
		}
#endif /* APCLI_SUPPORT */		
	}
#endif /* CONFIG_AP_SUPPORT */


	pAd->CommonCfg.BandState = UNKNOWN_BAND;	
}


#ifdef DOT11_N_SUPPORT
/*
	========================================================================
	Routine Description:
		Caller ensures we has 802.11n support.
		Calls at setting HT from AP/STASetinformation

	Arguments:
		pAd - Pointer to our adapter
		phymode  - 

	========================================================================
*/
VOID	RTMPSetHT(
	IN	PRTMP_ADAPTER	pAd,
	IN	OID_SET_HT_PHYMODE *pHTPhyMode)
{
	/*ULONG	*pmcs;*/
	UINT32	Value = 0;
	UCHAR	BBPValue = 0;
	UCHAR	BBP3Value = 0;
	UCHAR	RxStream = pAd->CommonCfg.RxStream;
#ifdef CONFIG_AP_SUPPORT
	INT		apidx;
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
	/* sanity check for extention channel */
	if (CHAN_PropertyCheck(pAd, pAd->CommonCfg.Channel,
						CHANNEL_NO_FAT_BELOW | CHANNEL_NO_FAT_ABOVE) == TRUE)
	{
		/* only 20MHz is allowed */
		pHTPhyMode->BW = 0;
	}
	else if (pHTPhyMode->ExtOffset == EXTCHA_BELOW)
	{
		/* extension channel below this channel is not allowed */
		if (CHAN_PropertyCheck(pAd, pAd->CommonCfg.Channel,
						CHANNEL_NO_FAT_BELOW) == TRUE)
		{
			pHTPhyMode->ExtOffset = EXTCHA_ABOVE;
		}
	}
	else if (pHTPhyMode->ExtOffset == EXTCHA_ABOVE)
	{
		/* extension channel above this channel is not allowed */
		if (CHAN_PropertyCheck(pAd, pAd->CommonCfg.Channel,
						CHANNEL_NO_FAT_ABOVE) == TRUE)
		{
			pHTPhyMode->ExtOffset = EXTCHA_BELOW;
		}
	}
#endif /* CONFIG_AP_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("RTMPSetHT : HT_mode(%d), ExtOffset(%d), MCS(%d), BW(%d), STBC(%d), SHORTGI(%d)\n",
										pHTPhyMode->HtMode, pHTPhyMode->ExtOffset, 
										pHTPhyMode->MCS, pHTPhyMode->BW,
										pHTPhyMode->STBC, pHTPhyMode->SHORTGI));
			
	/* Don't zero supportedHyPhy structure.*/
	RTMPZeroMemory(&pAd->CommonCfg.HtCapability, sizeof(pAd->CommonCfg.HtCapability));
	RTMPZeroMemory(&pAd->CommonCfg.AddHTInfo, sizeof(pAd->CommonCfg.AddHTInfo));
	RTMPZeroMemory(&pAd->CommonCfg.NewExtChanOffset, sizeof(pAd->CommonCfg.NewExtChanOffset));
	RTMPZeroMemory(&pAd->CommonCfg.DesiredHtPhy, sizeof(pAd->CommonCfg.DesiredHtPhy));

   	if (pAd->CommonCfg.bRdg)
	{
		pAd->CommonCfg.HtCapability.ExtHtCapInfo.PlusHTC = 1;
		pAd->CommonCfg.HtCapability.ExtHtCapInfo.RDGSupport = 1;
	}
	else
	{
		pAd->CommonCfg.HtCapability.ExtHtCapInfo.PlusHTC = 0;
		pAd->CommonCfg.HtCapability.ExtHtCapInfo.RDGSupport = 0;
	}


	if (RxStream == 1)
	{
		pAd->CommonCfg.HtCapability.HtCapParm.MaxRAmpduFactor = 2;
		pAd->CommonCfg.DesiredHtPhy.MaxRAmpduFactor = 2;
	}
	else
	{
		pAd->CommonCfg.HtCapability.HtCapParm.MaxRAmpduFactor = 3;
		pAd->CommonCfg.DesiredHtPhy.MaxRAmpduFactor = 3;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("RTMPSetHT : RxBAWinLimit = %d\n", pAd->CommonCfg.BACapability.field.RxBAWinLimit));

	/* Mimo power save, A-MSDU size, */
	pAd->CommonCfg.DesiredHtPhy.AmsduEnable = (USHORT)pAd->CommonCfg.BACapability.field.AmsduEnable;
	pAd->CommonCfg.DesiredHtPhy.AmsduSize = (UCHAR)pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.DesiredHtPhy.MimoPs = (UCHAR)pAd->CommonCfg.BACapability.field.MMPSmode;
	pAd->CommonCfg.DesiredHtPhy.MpduDensity = (UCHAR)pAd->CommonCfg.BACapability.field.MpduDensity;

	pAd->CommonCfg.HtCapability.HtCapInfo.AMsduSize = (USHORT)pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.HtCapability.HtCapInfo.MimoPs = (USHORT)pAd->CommonCfg.BACapability.field.MMPSmode;
	pAd->CommonCfg.HtCapability.HtCapParm.MpduDensity = (UCHAR)pAd->CommonCfg.BACapability.field.MpduDensity;
	
	DBGPRINT(RT_DEBUG_TRACE, ("RTMPSetHT : AMsduSize = %d, MimoPs = %d, MpduDensity = %d, MaxRAmpduFactor = %d\n", 
													pAd->CommonCfg.DesiredHtPhy.AmsduSize, 
													pAd->CommonCfg.DesiredHtPhy.MimoPs,
													pAd->CommonCfg.DesiredHtPhy.MpduDensity,
													pAd->CommonCfg.DesiredHtPhy.MaxRAmpduFactor));
	
	if(pHTPhyMode->HtMode == HTMODE_GF)
	{
		pAd->CommonCfg.HtCapability.HtCapInfo.GF = 1;
		pAd->CommonCfg.DesiredHtPhy.GF = 1;
	}
	else
		pAd->CommonCfg.DesiredHtPhy.GF = 0;
	
	/* Decide Rx MCSSet*/
	switch (RxStream)
	{
		case 1:			
			pAd->CommonCfg.HtCapability.MCSSet[0] =  0xff;
			pAd->CommonCfg.HtCapability.MCSSet[1] =  0x00;
			break;

		case 2:
			pAd->CommonCfg.HtCapability.MCSSet[0] =  0xff;
			pAd->CommonCfg.HtCapability.MCSSet[1] =  0xff;
			break;

		case 3: /* 3*3*/
			pAd->CommonCfg.HtCapability.MCSSet[0] =  0xff;
			pAd->CommonCfg.HtCapability.MCSSet[1] =  0xff;
			pAd->CommonCfg.HtCapability.MCSSet[2] =  0xff;
			break;
	}

	if (pAd->CommonCfg.bForty_Mhz_Intolerant && (pHTPhyMode->BW == BW_40) /* && (pAd->CommonCfg.Channel <= 14)*/)
	{
		pHTPhyMode->BW = BW_20;
		pAd->CommonCfg.HtCapability.HtCapInfo.Forty_Mhz_Intolerant = 1;
	}

	if(pHTPhyMode->BW == BW_40)
	{
		pAd->CommonCfg.HtCapability.MCSSet[4] = 0x1; /* MCS 32*/
		pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth = 1;
		if (pAd->CommonCfg.Channel <= 14) 		
			pAd->CommonCfg.HtCapability.HtCapInfo.CCKmodein40 = 1;

		pAd->CommonCfg.DesiredHtPhy.ChannelWidth = 1;
		pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = 1;
		pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = (pHTPhyMode->ExtOffset == EXTCHA_BELOW)? (EXTCHA_BELOW): EXTCHA_ABOVE;
		/* Set Regsiter for extension channel position.*/
		RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBP3Value);
		if ((pHTPhyMode->ExtOffset == EXTCHA_BELOW))
		{
			Value |= 0x1;
			BBP3Value |= (0x20);
			RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);
		}
		else if ((pHTPhyMode->ExtOffset == EXTCHA_ABOVE))
		{
			Value &= 0xfe;
			BBP3Value &= (~0x20);
			RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);
		}

		/* Turn on BBP 40MHz mode now only as AP . */
		/* Sta can turn on BBP 40MHz after connection with 40MHz AP. Sta only broadcast 40MHz capability before connection.*/
		if ((pAd->OpMode == OPMODE_AP) || INFRA_ON(pAd) || ADHOC_ON(pAd)
			)
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPValue);
			BBPValue &= (~0x18);
#ifdef GREENAP_SUPPORT
			if (pAd->ApCfg.bGreenAPActive == 0)
#endif /* GREENAP_SUPPORT */
			BBPValue |= 0x10;
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, BBPValue);

			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBP3Value);
#ifdef GREENAP_SUPPORT
			if (pAd->ApCfg.bGreenAPActive == 1)
				pAd->CommonCfg.BBPCurrentBW = BW_20;
			else
#endif /* GREENAP_SUPPORT */
			pAd->CommonCfg.BBPCurrentBW = BW_40;

		}
	}
	else
	{
		pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth = 0;
		pAd->CommonCfg.DesiredHtPhy.ChannelWidth = 0;
		pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = 0;
		pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = EXTCHA_NONE;
		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;
		/* Turn on BBP 20MHz mode by request here.*/
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &BBPValue);
			BBPValue &= (~0x18);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, BBPValue);
			pAd->CommonCfg.BBPCurrentBW = BW_20;

		}
	}
		
	if(pHTPhyMode->STBC == STBC_USE)
	{
		/*
			TxSTBC
				0:not supported, 
				1:if supported
		*/
		if (pAd->Antenna.field.TxPath >= 2)
		{
			pAd->CommonCfg.HtCapability.HtCapInfo.TxSTBC = 1;
			pAd->CommonCfg.DesiredHtPhy.TxSTBC = 1;
		}
		else
		{
			pAd->CommonCfg.HtCapability.HtCapInfo.TxSTBC = 0;
			pAd->CommonCfg.DesiredHtPhy.TxSTBC = 0; 	
		}
		
		/*
			RxSTBC
				0: not support,							
				1: support for 1SS
				2: support for 1SS, 2SS
				3: support for 1SS, 2SS, 3SS
		*/
		if (pAd->Antenna.field.RxPath >= 1)
		{
			pAd->CommonCfg.HtCapability.HtCapInfo.RxSTBC = 1;
			pAd->CommonCfg.DesiredHtPhy.RxSTBC = 1;
		}
		else
		{
			pAd->CommonCfg.HtCapability.HtCapInfo.RxSTBC = 0; 
			pAd->CommonCfg.DesiredHtPhy.RxSTBC = 0; 	
		}
	}
	else
	{
		pAd->CommonCfg.DesiredHtPhy.TxSTBC = 0;
		pAd->CommonCfg.DesiredHtPhy.RxSTBC = 0;
	}

	if(pHTPhyMode->SHORTGI == GI_400)
	{
		pAd->CommonCfg.HtCapability.HtCapInfo.ShortGIfor20 = 1;
		pAd->CommonCfg.HtCapability.HtCapInfo.ShortGIfor40 = 1;
		pAd->CommonCfg.DesiredHtPhy.ShortGIfor20 = 1;
		pAd->CommonCfg.DesiredHtPhy.ShortGIfor40 = 1;
	}
	else
	{
		pAd->CommonCfg.HtCapability.HtCapInfo.ShortGIfor20 = 0;
		pAd->CommonCfg.HtCapability.HtCapInfo.ShortGIfor40 = 0;
		pAd->CommonCfg.DesiredHtPhy.ShortGIfor20 = 0;
		pAd->CommonCfg.DesiredHtPhy.ShortGIfor40 = 0;
	}
	
	/* We support link adaptation for unsolicit MCS feedback, set to 2.*/
	pAd->CommonCfg.AddHTInfo.ControlChan = pAd->CommonCfg.Channel;
	/* 1, the extension channel above the control channel. */
	
	/* EDCA parameters used for AP's own transmission*/
	if (pAd->CommonCfg.APEdcaParm.bValid == FALSE)
	{
		pAd->CommonCfg.APEdcaParm.bValid = TRUE;
		pAd->CommonCfg.APEdcaParm.Aifsn[0] = 3;
		pAd->CommonCfg.APEdcaParm.Aifsn[1] = 7;
		pAd->CommonCfg.APEdcaParm.Aifsn[2] = 1;
		pAd->CommonCfg.APEdcaParm.Aifsn[3] = 1;
	
		pAd->CommonCfg.APEdcaParm.Cwmin[0] = 4;
		pAd->CommonCfg.APEdcaParm.Cwmin[1] = 4;
		pAd->CommonCfg.APEdcaParm.Cwmin[2] = 3;
		pAd->CommonCfg.APEdcaParm.Cwmin[3] = 2;

		pAd->CommonCfg.APEdcaParm.Cwmax[0] = 6;
		pAd->CommonCfg.APEdcaParm.Cwmax[1] = 10;
		pAd->CommonCfg.APEdcaParm.Cwmax[2] = 4;
		pAd->CommonCfg.APEdcaParm.Cwmax[3] = 3;

		pAd->CommonCfg.APEdcaParm.Txop[0]  = 0;
		pAd->CommonCfg.APEdcaParm.Txop[1]  = 0;
		pAd->CommonCfg.APEdcaParm.Txop[2]  = 94;	
		pAd->CommonCfg.APEdcaParm.Txop[3]  = 47;	
	}
	AsicSetEdcaParm(pAd, &pAd->CommonCfg.APEdcaParm);
	
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
		{				
			RTMPSetIndividualHT(pAd, apidx);			
		}
#ifdef WDS_SUPPORT
		for (apidx = 0; apidx < MAX_WDS_ENTRY; apidx++)
		{				
			RTMPSetIndividualHT(pAd, apidx + MIN_NET_DEVICE_FOR_WDS);			
		}
#endif /* WDS_SUPPORT */
#ifdef APCLI_SUPPORT
		for (apidx = 0; apidx < MAX_APCLI_NUM; apidx++)
		{				
			RTMPSetIndividualHT(pAd, apidx + MIN_NET_DEVICE_FOR_APCLI);			
		}
#endif /* APCLI_SUPPORT */
	}
#endif /* CONFIG_AP_SUPPORT */

#ifdef TXBF_SUPPORT
	if (pAd->chipCap.FlgHwTxBfCap)
	{
		/* Set ETxBF */
		setETxBFCap(pAd, &pAd->CommonCfg.HtCapability.TxBFCap);

		/* Check ITxBF */
		pAd->CommonCfg.RegTransmitSetting.field.ITxBfEn &= rtmp_chk_itxbf_calibration(pAd);

		/* Apply to ASIC */
		rtmp_asic_set_bf(pAd);
	}
#endif /* TXBF_SUPPORT */


}

/*
	========================================================================
	Routine Description:
		Caller ensures we has 802.11n support.
		Calls at setting HT from AP/STASetinformation

	Arguments:
		pAd - Pointer to our adapter
		phymode  - 

	========================================================================
*/
VOID	RTMPSetIndividualHT(
	IN	PRTMP_ADAPTER		pAd,
	IN	UCHAR				apidx)
{	
	PRT_HT_PHY_INFO		pDesired_ht_phy = NULL;
	UCHAR	TxStream = pAd->CommonCfg.TxStream;		
	UCHAR	DesiredMcs	= MCS_AUTO;
	UCHAR	encrypt_mode = Ndis802_11EncryptionDisabled;
						
	do
	{


#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT	
			if (apidx >= MIN_NET_DEVICE_FOR_APCLI)
			{				
				UCHAR	idx = apidx - MIN_NET_DEVICE_FOR_APCLI;
						
				if (idx < MAX_APCLI_NUM)
				{
					pDesired_ht_phy = &pAd->ApCfg.ApCliTab[idx].DesiredHtPhyInfo;									
					DesiredMcs = pAd->ApCfg.ApCliTab[idx].DesiredTransmitSetting.field.MCS;							
					encrypt_mode = pAd->ApCfg.ApCliTab[idx].WepStatus;
					pAd->ApCfg.ApCliTab[idx].bAutoTxRateSwitch = (DesiredMcs == MCS_AUTO) ? TRUE : FALSE;
					break;
				}
				else
				{
					DBGPRINT(RT_DEBUG_ERROR, ("RTMPSetIndividualHT: invalid idx(%d)\n", idx));
					return;
				}
			}
#endif /* APCLI_SUPPORT */

		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{

#ifdef WDS_SUPPORT
			if (apidx >= MIN_NET_DEVICE_FOR_WDS)
			{				
				UCHAR	idx = apidx - MIN_NET_DEVICE_FOR_WDS;
						
				if (idx < MAX_WDS_ENTRY)
				{
					pDesired_ht_phy = &pAd->WdsTab.WdsEntry[idx].DesiredHtPhyInfo;									
					DesiredMcs = pAd->WdsTab.WdsEntry[idx].DesiredTransmitSetting.field.MCS;							
					/*encrypt_mode = pAd->WdsTab.WdsEntry[idx].WepStatus;*/
					pAd->WdsTab.WdsEntry[idx].bAutoTxRateSwitch = (DesiredMcs == MCS_AUTO) ? TRUE : FALSE;
					break;
				}
				else
				{
					DBGPRINT(RT_DEBUG_ERROR, ("RTMPSetIndividualHT: invalid apidx(%d)\n", apidx));
					return;
				}
			}
#endif /* WDS_SUPPORT */
			if ((apidx < pAd->ApCfg.BssidNum) && (apidx < MAX_MBSSID_NUM(pAd)) && (apidx < HW_BEACON_MAX_NUM))
			{								
				pDesired_ht_phy = &pAd->ApCfg.MBSSID[apidx].DesiredHtPhyInfo;									
				DesiredMcs = pAd->ApCfg.MBSSID[apidx].DesiredTransmitSetting.field.MCS;			
				encrypt_mode = pAd->ApCfg.MBSSID[apidx].WepStatus;
				pAd->ApCfg.MBSSID[apidx].bWmmCapable = TRUE;	
				pAd->ApCfg.MBSSID[apidx].bAutoTxRateSwitch = (DesiredMcs == MCS_AUTO) ? TRUE : FALSE;
				break;
			}

			DBGPRINT(RT_DEBUG_ERROR, ("RTMPSetIndividualHT: invalid apidx(%d)\n", apidx));
			return;
		}			
#endif /* CONFIG_AP_SUPPORT */
	} while (FALSE);

	if (pDesired_ht_phy == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("RTMPSetIndividualHT: invalid apidx(%d)\n", apidx));
		return;
	}
	RTMPZeroMemory(pDesired_ht_phy, sizeof(RT_HT_PHY_INFO));

	DBGPRINT(RT_DEBUG_TRACE, ("RTMPSetIndividualHT : Desired MCS = %d\n", DesiredMcs));
	/* Check the validity of MCS */
	if ((TxStream == 1) && ((DesiredMcs >= MCS_8) && (DesiredMcs <= MCS_15)))
	{
		DBGPRINT(RT_DEBUG_WARN, ("RTMPSetIndividualHT: MCS(%d) is invalid in 1S, reset it as MCS_7\n", DesiredMcs));
		DesiredMcs = MCS_7;		
	}

	if ((pAd->CommonCfg.DesiredHtPhy.ChannelWidth == BW_20) && (DesiredMcs == MCS_32))
	{
		DBGPRINT(RT_DEBUG_WARN, ("RTMPSetIndividualHT: MCS_32 is only supported in 40-MHz, reset it as MCS_0\n"));
		DesiredMcs = MCS_0;		
	}
	   		
	/* 	
		WFA recommend to restrict the encryption type in 11n-HT mode.
	 	So, the WEP and TKIP are not allowed in HT rate. 
	*/
	if (pAd->CommonCfg.HT_DisallowTKIP && IS_INVALID_HT_SECURITY(encrypt_mode))
	{
		DBGPRINT(RT_DEBUG_WARN, ("%s : Use legacy rate in WEP/TKIP encryption mode (apidx=%d)\n", 
									__FUNCTION__, apidx));
		return;
	}

	if (pAd->CommonCfg.HT_Disable)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("%s : HT is disabled\n", __FUNCTION__));
		return;
	}
			
	pDesired_ht_phy->bHtEnable = TRUE;
					 
	/* Decide desired Tx MCS*/
	switch (TxStream)
	{
		case 1:
			if (DesiredMcs == MCS_AUTO)
			{
				pDesired_ht_phy->MCSSet[0]= 0xff;
				pDesired_ht_phy->MCSSet[1]= 0x00;
			}
			else if (DesiredMcs <= MCS_7)
			{
				pDesired_ht_phy->MCSSet[0]= 1<<DesiredMcs;
				pDesired_ht_phy->MCSSet[1]= 0x00;
			}			
			break;

		case 2:
			if (DesiredMcs == MCS_AUTO)
			{
				pDesired_ht_phy->MCSSet[0]= 0xff;
				pDesired_ht_phy->MCSSet[1]= 0xff;
			}
			else if (DesiredMcs <= MCS_15)
			{
				ULONG mode;
				
				mode = DesiredMcs / 8;
				if (mode < 2)
					pDesired_ht_phy->MCSSet[mode] = (1 << (DesiredMcs - mode * 8));
			}			
			break;

		case 3: /* 3*3*/
			if (DesiredMcs == MCS_AUTO)
			{
				/* MCS0 ~ MCS23, 3 bytes */
				pDesired_ht_phy->MCSSet[0]= 0xff;
				pDesired_ht_phy->MCSSet[1]= 0xff;
				pDesired_ht_phy->MCSSet[2]= 0xff;
			}
			else if (DesiredMcs <= MCS_23)
			{
				ULONG mode;

				mode = DesiredMcs / 8;
				if (mode < 3)
					pDesired_ht_phy->MCSSet[mode] = (1 << (DesiredMcs - mode * 8));
			}
			break;
	}							

	if(pAd->CommonCfg.DesiredHtPhy.ChannelWidth == BW_40)
	{
		if (DesiredMcs == MCS_AUTO || DesiredMcs == MCS_32)
			pDesired_ht_phy->MCSSet[4] = 0x1;
	}

	/* update HT Rate setting				*/
    if (pAd->OpMode == OPMODE_STA)
	{
        MlmeUpdateHtTxRates(pAd, BSS0);
	}
    else
	    MlmeUpdateHtTxRates(pAd, apidx);
}

/*
	========================================================================
	Routine Description:
		Clear the desire HT info per interface
		
	Arguments:
	
	========================================================================
*/
VOID RTMPDisableDesiredHtInfo(
	IN	PRTMP_ADAPTER		pAd)
{
#ifdef CONFIG_AP_SUPPORT
	UINT8				apidx = 0;
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
		{				
			RTMPZeroMemory(&pAd->ApCfg.MBSSID[apidx].DesiredHtPhyInfo, sizeof(RT_HT_PHY_INFO));		
		}
#ifdef WDS_SUPPORT
		for (apidx = 0; apidx < MAX_WDS_ENTRY; apidx++)
		{				
			RTMPZeroMemory(&pAd->WdsTab.WdsEntry[apidx].DesiredHtPhyInfo, sizeof(RT_HT_PHY_INFO));		
		}
#endif /* WDS_SUPPORT */
#ifdef APCLI_SUPPORT
		for (apidx = 0; apidx < MAX_APCLI_NUM; apidx++)
		{				
			RTMPZeroMemory(&pAd->ApCfg.ApCliTab[apidx].DesiredHtPhyInfo, sizeof(RT_HT_PHY_INFO));
		}
#endif /* APCLI_SUPPORT */
	}
#endif /* CONFIG_AP_SUPPORT */


}


INT	SetCommonHT(
	IN	PRTMP_ADAPTER	pAd)
{
	OID_SET_HT_PHYMODE		SetHT;
	
	if (pAd->CommonCfg.PhyMode < PHY_11ABGN_MIXED)
	{
		/* Clear previous HT information */
		RTMPDisableDesiredHtInfo(pAd);
		return FALSE;
	}
	
	SetHT.PhyMode = (RT_802_11_PHY_MODE)pAd->CommonCfg.PhyMode;
	SetHT.TransmitNo = ((UCHAR)pAd->Antenna.field.TxPath);
	SetHT.HtMode = (UCHAR)pAd->CommonCfg.RegTransmitSetting.field.HTMODE;
	SetHT.ExtOffset = (UCHAR)pAd->CommonCfg.RegTransmitSetting.field.EXTCHA;
	SetHT.MCS = MCS_AUTO;
	SetHT.BW = (UCHAR)pAd->CommonCfg.RegTransmitSetting.field.BW;
	SetHT.STBC = (UCHAR)pAd->CommonCfg.RegTransmitSetting.field.STBC;
	SetHT.SHORTGI = (UCHAR)pAd->CommonCfg.RegTransmitSetting.field.ShortGI;		

	RTMPSetHT(pAd, &SetHT);

#ifdef DOT11N_DRAFT3
	if(pAd->CommonCfg.bBssCoexEnable && pAd->CommonCfg.Bss2040NeedFallBack)
	{
		pAd->CommonCfg.AddHTInfo.AddHtInfo.RecomWidth = 0;
		pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset = 0;
		pAd->CommonCfg.LastBSSCoexist2040.field.BSS20WidthReq = 1;
		pAd->CommonCfg.Bss2040CoexistFlag |= BSS_2040_COEXIST_INFO_SYNC;
		pAd->CommonCfg.Bss2040NeedFallBack = 1;
	}
#endif // DOT11N_DRAFT3 //

	return TRUE;
}

/*
	========================================================================
	Routine Description:
		Update HT IE from our capability.
		
	Arguments:
		Send all HT IE in beacon/probe rsp/assoc rsp/action frame.
		
	
	========================================================================
*/
VOID	RTMPUpdateHTIE(
	IN	RT_HT_CAPABILITY	*pRtHt,
	IN		UCHAR				*pMcsSet,
	OUT		HT_CAPABILITY_IE *pHtCapability,
	OUT		ADD_HT_INFO_IE		*pAddHtInfo)
{
	RTMPZeroMemory(pHtCapability, sizeof(HT_CAPABILITY_IE));
	RTMPZeroMemory(pAddHtInfo, sizeof(ADD_HT_INFO_IE));
	
		pHtCapability->HtCapInfo.ChannelWidth = pRtHt->ChannelWidth;
		pHtCapability->HtCapInfo.MimoPs = pRtHt->MimoPs;
		pHtCapability->HtCapInfo.GF = pRtHt->GF;
		pHtCapability->HtCapInfo.ShortGIfor20 = pRtHt->ShortGIfor20;
		pHtCapability->HtCapInfo.ShortGIfor40 = pRtHt->ShortGIfor40;
		pHtCapability->HtCapInfo.TxSTBC = pRtHt->TxSTBC;
		pHtCapability->HtCapInfo.RxSTBC = pRtHt->RxSTBC;
		pHtCapability->HtCapInfo.AMsduSize = pRtHt->AmsduSize;
		pHtCapability->HtCapParm.MaxRAmpduFactor = pRtHt->MaxRAmpduFactor;
		pHtCapability->HtCapParm.MpduDensity = pRtHt->MpduDensity;

		pAddHtInfo->AddHtInfo.ExtChanOffset = pRtHt->ExtChanOffset ;
		pAddHtInfo->AddHtInfo.RecomWidth = pRtHt->RecomWidth;
		pAddHtInfo->AddHtInfo2.OperaionMode = pRtHt->OperaionMode;
		pAddHtInfo->AddHtInfo2.NonGfPresent = pRtHt->NonGfPresent;
		RTMPMoveMemory(pAddHtInfo->MCSSet, /*pRtHt->MCSSet*/pMcsSet, 4); /* rt2860 only support MCS max=32, no need to copy all 16 uchar.*/
	
        DBGPRINT(RT_DEBUG_TRACE,("RTMPUpdateHTIE <== \n"));
}
#endif /* DOT11_N_SUPPORT */

/*
	========================================================================
	Description:
		Add Client security information into ASIC WCID table and IVEIV table.
    Return:
	========================================================================
*/
VOID	RTMPAddWcidAttributeEntry(
	IN	PRTMP_ADAPTER	pAd,
	IN	UCHAR			BssIdx,
	IN 	UCHAR		 	KeyIdx,
	IN 	UCHAR		 	CipherAlg,
	IN 	MAC_TABLE_ENTRY *pEntry)
{
	UINT32		WCIDAttri = 0;
	USHORT		offset;
	UCHAR		IVEIV = 0;
	USHORT		Wcid = 0;
#ifdef CONFIG_AP_SUPPORT
	BOOLEAN		IEEE8021X = FALSE;
#endif /* CONFIG_AP_SUPPORT */

	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
#ifdef APCLI_SUPPORT
			if (BssIdx >= MIN_NET_DEVICE_FOR_APCLI)
			{
				if (pEntry)		
					BssIdx -= MIN_NET_DEVICE_FOR_APCLI;		
				else
				{
					DBGPRINT(RT_DEBUG_WARN, ("RTMPAddWcidAttributeEntry: AP-Client link doesn't need to set Group WCID Attribute. \n"));	
					return;
				}
			}	
			else 
#endif /* APCLI_SUPPORT */
#ifdef WDS_SUPPORT
			if (BssIdx >= MIN_NET_DEVICE_FOR_WDS)
			{
				if (pEntry)		
					BssIdx = BSS0;		
				else
				{
					DBGPRINT(RT_DEBUG_WARN, ("RTMPAddWcidAttributeEntry: WDS link doesn't need to set Group WCID Attribute. \n"));	
					return;
				}
			}	
			else
#endif /* WDS_SUPPORT */
			{
				if (BssIdx >= pAd->ApCfg.BssidNum)
				{
					DBGPRINT(RT_DEBUG_ERROR, ("RTMPAddWcidAttributeEntry: The BSS-index(%d) is out of range for MBSSID link. \n", BssIdx));	
					return;
				}
			}

			/* choose wcid number*/
			if (pEntry)
				Wcid = pEntry->Aid;
			else
				GET_GroupKey_WCID(pAd, Wcid, BssIdx);		

#ifdef DOT1X_SUPPORT
			if ((BssIdx < pAd->ApCfg.BssidNum) && (BssIdx < MAX_MBSSID_NUM(pAd)) && (BssIdx < HW_BEACON_MAX_NUM))
				IEEE8021X = pAd->ApCfg.MBSSID[BssIdx].IEEE8021X;
#endif /* DOT1X_SUPPORT */			
		}
#endif /* CONFIG_AP_SUPPORT */
	}

	/* Update WCID attribute table*/
	offset = MAC_WCID_ATTRIBUTE_BASE + (Wcid * HW_WCID_ATTRI_SIZE);
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		/*
			1.	Wds-links and Mesh-links always use Pair-wise key table.
			2. 	When the CipherAlg is TKIP, AES or the dynamic WEP is enabled,
				it needs to set key into Pair-wise Key Table.
			3.	The pair-wise key security mode is set NONE, it means as no security.
		*/
		if (pEntry && (IS_ENTRY_WDS(pEntry) || IS_ENTRY_MESH(pEntry)))
			WCIDAttri = (BssIdx<<4) | (CipherAlg<<1) | PAIRWISEKEYTABLE;
		else if ((pEntry) && 
				((CipherAlg == CIPHER_TKIP) || 
				 (CipherAlg == CIPHER_AES) || 
				 (CipherAlg == CIPHER_NONE) || 
				 (IEEE8021X == TRUE)))
			WCIDAttri = (BssIdx<<4) | (CipherAlg<<1) | PAIRWISEKEYTABLE;
		else
			WCIDAttri = (BssIdx<<4) | (CipherAlg<<1) | SHAREDKEYTABLE;
	}
#endif /* CONFIG_AP_SUPPORT */

		
	RTMP_IO_WRITE32(pAd, offset, WCIDAttri);

		
	/* Update IV/EIV table*/
	offset = MAC_IVEIV_TABLE_BASE + (Wcid * HW_IVEIV_ENTRY_SIZE);

	/* WPA mode*/
	if ((CipherAlg == CIPHER_TKIP) || (CipherAlg == CIPHER_AES))
	{	
		/* Eiv bit on. keyid always is 0 for pairwise key 			*/
		IVEIV = (KeyIdx <<6) | 0x20;	
	}	 
	else
	{
		/* WEP KeyIdx is default tx key. */
		IVEIV = (KeyIdx << 6);	
	}

	/* For key index and ext IV bit, so only need to update the position(offset+3).*/
#ifdef RTMP_MAC_PCI	
	RTMP_IO_WRITE8(pAd, offset+3, IVEIV);
#endif /* RTMP_MAC_PCI */
	
	DBGPRINT(RT_DEBUG_TRACE,("RTMPAddWcidAttributeEntry: WCID #%d, KeyIndex #%d, Alg=%s\n",Wcid, KeyIdx, CipherName[CipherAlg]));
	DBGPRINT(RT_DEBUG_TRACE,("	WCIDAttri = 0x%x \n",  WCIDAttri));	

}

/* 
    ==========================================================================
    Description:
        Parse encryption type
Arguments:
    pAdapter                    Pointer to our adapter
    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
    ==========================================================================
*/
PSTRING GetEncryptType(CHAR enc)
{
    if(enc == Ndis802_11WEPDisabled)
        return "NONE";
    if(enc == Ndis802_11WEPEnabled)
    	return "WEP";
    if(enc == Ndis802_11Encryption2Enabled)
    	return "TKIP";
    if(enc == Ndis802_11Encryption3Enabled)
    	return "AES";
	if(enc == Ndis802_11Encryption4Enabled)
    	return "TKIPAES";
#ifdef WAPI_SUPPORT
	if(enc == Ndis802_11EncryptionSMS4Enabled)
    	return "SMS4";
#endif /* WAPI_SUPPORT */
    else
    	return "UNKNOW";
}

PSTRING GetAuthMode(CHAR auth)
{
    if(auth == Ndis802_11AuthModeOpen)
    	return "OPEN";
    if(auth == Ndis802_11AuthModeShared)
    	return "SHARED";
	if(auth == Ndis802_11AuthModeAutoSwitch)
    	return "AUTOWEP";
    if(auth == Ndis802_11AuthModeWPA)
    	return "WPA";
    if(auth == Ndis802_11AuthModeWPAPSK)
    	return "WPAPSK";
    if(auth == Ndis802_11AuthModeWPANone)
    	return "WPANONE";
    if(auth == Ndis802_11AuthModeWPA2)
    	return "WPA2";
    if(auth == Ndis802_11AuthModeWPA2PSK)
    	return "WPA2PSK";
	if(auth == Ndis802_11AuthModeWPA1WPA2)
    	return "WPA1WPA2";
	if(auth == Ndis802_11AuthModeWPA1PSKWPA2PSK)
    	return "WPA1PSKWPA2PSK";
#ifdef WAPI_SUPPORT
	if(auth == Ndis802_11AuthModeWAICERT)
    	return "WAI-CERT";
	if(auth == Ndis802_11AuthModeWAIPSK)
    	return "WAI-PSK";
#endif /* WAPI_SUPPORT */
	
    	return "UNKNOW";
}		


/* 
    ==========================================================================
    Description:
        Get site survey results
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage:
        		1.) UI needs to wait 4 seconds after issue a site survey command
        		2.) iwpriv ra0 get_site_survey
        		3.) UI needs to prepare at least 4096bytes to get the results
    ==========================================================================
*/
#define	LINE_LEN	(4+33+20+23+9+7+7+3)	/* Channel+SSID+Bssid+Security+Signal+WiressMode+ExtCh+NetworkType*/

VOID	RTMPCommSiteSurveyData(
	IN  PSTRING		msg,
	IN  PBSS_ENTRY	pBss,
	IN  UINT32		MsgLen)
{
	INT         Rssi = 0;
	UINT        Rssi_Quality = 0;
	NDIS_802_11_NETWORK_TYPE    wireless_mode;
	CHAR		Ssid[MAX_LEN_OF_SSID +1];
	STRING		SecurityStr[32] = {0};
	NDIS_802_11_ENCRYPTION_STATUS	ap_cipher = Ndis802_11EncryptionDisabled;
	NDIS_802_11_AUTHENTICATION_MODE	ap_auth_mode = Ndis802_11AuthModeOpen;

		/*Channel*/
		sprintf(msg+strlen(msg),"%-4d", pBss->Channel);


		/*SSID*/
	NdisZeroMemory(Ssid, (MAX_LEN_OF_SSID +1));
	if (RTMPCheckStrPrintAble((PCHAR)pBss->Ssid, pBss->SsidLen))
		NdisMoveMemory(Ssid, pBss->Ssid, pBss->SsidLen);
	else
	{
		INT idx = 0;
		sprintf(Ssid, "0x");
		for (idx = 0; (idx < 14) && (idx < pBss->SsidLen); idx++)
			sprintf(Ssid + 2 + (idx*2), "%02X", (UCHAR)pBss->Ssid[idx]);
	}
		sprintf(msg+strlen(msg),"%-33s", Ssid);      
		
		/*BSSID*/
		sprintf(msg+strlen(msg),"%02x:%02x:%02x:%02x:%02x:%02x   ", 
			pBss->Bssid[0], 
			pBss->Bssid[1],
			pBss->Bssid[2], 
			pBss->Bssid[3], 
			pBss->Bssid[4], 
			pBss->Bssid[5]);
	
	/*Security*/
	RTMPZeroMemory(SecurityStr, 32);
	if ((Ndis802_11AuthModeWPA <= pBss->AuthMode) &&
		(pBss->AuthMode <= Ndis802_11AuthModeWPA1PSKWPA2PSK))
	{
		if (pBss->AuthMode == Ndis802_11AuthModeWPANone)
		{
			ap_auth_mode = pBss->AuthMode;
				ap_cipher = pBss->WPA.PairCipher;
		}
		else if (pBss->AuthModeAux == Ndis802_11AuthModeOpen)
		{
			ap_auth_mode = pBss->AuthMode;
			if ((ap_auth_mode == Ndis802_11AuthModeWPA) || 
				(ap_auth_mode == Ndis802_11AuthModeWPAPSK))
			{
				if (pBss->WPA.PairCipherAux == Ndis802_11WEPDisabled)
					ap_cipher = pBss->WPA.PairCipher;
				else 
					ap_cipher = Ndis802_11Encryption4Enabled;
			}
			else if ((ap_auth_mode == Ndis802_11AuthModeWPA2) || 
					 (ap_auth_mode == Ndis802_11AuthModeWPA2PSK))
			{
				if (pBss->WPA2.PairCipherAux == Ndis802_11WEPDisabled)
					ap_cipher = pBss->WPA2.PairCipher;
				else 
					ap_cipher = Ndis802_11Encryption4Enabled;
			}
		}
		else if ((pBss->AuthMode == Ndis802_11AuthModeWPAPSK) || 
				 (pBss->AuthMode == Ndis802_11AuthModeWPA2PSK))
		{
			if ((pBss->AuthModeAux == Ndis802_11AuthModeWPAPSK) ||
				(pBss->AuthModeAux == Ndis802_11AuthModeWPA2PSK))
				ap_auth_mode = Ndis802_11AuthModeWPA1PSKWPA2PSK;
			else
				ap_auth_mode = pBss->AuthMode;
			
			if (pBss->WPA.PairCipher != pBss->WPA2.PairCipher)
				ap_cipher = Ndis802_11Encryption4Enabled;
			else if ((pBss->WPA.PairCipher == pBss->WPA2.PairCipher) &&
					 (pBss->WPA.PairCipherAux != pBss->WPA2.PairCipherAux))
				ap_cipher = Ndis802_11Encryption4Enabled;
			else if ((pBss->WPA.PairCipher == pBss->WPA2.PairCipher) &&
					 (pBss->WPA.PairCipherAux == pBss->WPA2.PairCipherAux) &&
					 (pBss->WPA.PairCipherAux != Ndis802_11WEPDisabled))
				ap_cipher = Ndis802_11Encryption4Enabled;
			else if ((pBss->WPA.PairCipher == pBss->WPA2.PairCipher) &&
					 (pBss->WPA.PairCipherAux == pBss->WPA2.PairCipherAux) &&
					 (pBss->WPA.PairCipherAux == Ndis802_11WEPDisabled))
				ap_cipher = pBss->WPA.PairCipher;
		}
		else if ((pBss->AuthMode == Ndis802_11AuthModeWPA) || 
				 (pBss->AuthMode == Ndis802_11AuthModeWPA2))
		{
			if ((pBss->AuthModeAux == Ndis802_11AuthModeWPA) ||
				(pBss->AuthModeAux == Ndis802_11AuthModeWPA2))
				ap_auth_mode = Ndis802_11AuthModeWPA1WPA2;
			else
				ap_auth_mode = pBss->AuthMode;
			
			if (pBss->WPA.PairCipher != pBss->WPA2.PairCipher)
				ap_cipher = Ndis802_11Encryption4Enabled;
			else if ((pBss->WPA.PairCipher == pBss->WPA2.PairCipher) &&
					 (pBss->WPA.PairCipherAux != pBss->WPA2.PairCipherAux))
				ap_cipher = Ndis802_11Encryption4Enabled;
			else if ((pBss->WPA.PairCipher == pBss->WPA2.PairCipher) &&
					 (pBss->WPA.PairCipherAux == pBss->WPA2.PairCipherAux) &&
					 (pBss->WPA.PairCipherAux != Ndis802_11WEPDisabled))
				ap_cipher = Ndis802_11Encryption4Enabled;
			else if ((pBss->WPA.PairCipher == pBss->WPA2.PairCipher) &&
					 (pBss->WPA.PairCipherAux == pBss->WPA2.PairCipherAux) &&
					 (pBss->WPA.PairCipherAux == Ndis802_11WEPDisabled))
				ap_cipher = pBss->WPA.PairCipher;
		}

		sprintf(SecurityStr, "%s/%s", GetAuthMode((CHAR)ap_auth_mode), GetEncryptType((CHAR)ap_cipher));		
	}			
	else
	{
		ap_auth_mode = pBss->AuthMode;
		ap_cipher = pBss->WepStatus;		
		if (ap_cipher == Ndis802_11WEPDisabled)
			sprintf(SecurityStr, "NONE");
		else if (ap_cipher == Ndis802_11WEPEnabled)
			sprintf(SecurityStr, "WEP");
		else
			sprintf(SecurityStr, "%s/%s", GetAuthMode((CHAR)ap_auth_mode), GetEncryptType((CHAR)ap_cipher));		
	}
	
	sprintf(msg+strlen(msg), "%-23s", SecurityStr);

		/* Rssi*/
		Rssi = (INT)pBss->Rssi;
		if (Rssi >= -50)
			Rssi_Quality = 100;
		else if (Rssi >= -80)    /* between -50 ~ -80dbm*/
			Rssi_Quality = (UINT)(24 + ((Rssi + 80) * 26)/10);
		else if (Rssi >= -90)   /* between -80 ~ -90dbm*/
			Rssi_Quality = (UINT)(((Rssi + 90) * 26)/10);
		else    /* < -84 dbm*/
			Rssi_Quality = 0;
		sprintf(msg+strlen(msg),"%-9d", Rssi_Quality);
		
		/* Wireless Mode*/
		wireless_mode = NetworkTypeInUseSanity(pBss);
		if (wireless_mode == Ndis802_11FH ||
			wireless_mode == Ndis802_11DS)
			sprintf(msg+strlen(msg),"%-7s", "11b");
		else if (wireless_mode == Ndis802_11OFDM5)
			sprintf(msg+strlen(msg),"%-7s", "11a");
		else if (wireless_mode == Ndis802_11OFDM5_N)
			sprintf(msg+strlen(msg),"%-7s", "11a/n");
		else if (wireless_mode == Ndis802_11OFDM24)
			sprintf(msg+strlen(msg),"%-7s", "11b/g");
		else if (wireless_mode == Ndis802_11OFDM24_N)
			sprintf(msg+strlen(msg),"%-7s", "11b/g/n");
		else
			sprintf(msg+strlen(msg),"%-7s", "unknow");

		/* Ext Channel*/
		if (pBss->AddHtInfoLen > 0)
		{
			if (pBss->AddHtInfo.AddHtInfo.ExtChanOffset == EXTCHA_ABOVE)
				sprintf(msg+strlen(msg),"%-7s", " ABOVE");
			else if (pBss->AddHtInfo.AddHtInfo.ExtChanOffset == EXTCHA_BELOW)
				sprintf(msg+strlen(msg),"%-7s", " BELOW");
			else
				sprintf(msg+strlen(msg),"%-7s", " NONE");
		}
		else
		{
			sprintf(msg+strlen(msg),"%-7s", " NONE");
		}
		
		/*Network Type		*/
		if (pBss->BssType == BSS_ADHOC)
			sprintf(msg+strlen(msg),"%-3s", " Ad");
		else
			sprintf(msg+strlen(msg),"%-3s", " In");

        sprintf(msg+strlen(msg),"\n");
	
	return;
}

#if defined (AP_SCAN_SUPPORT) || defined (CONFIG_STA_SUPPORT)
VOID RTMPIoctlGetSiteSurvey(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	RTMP_IOCTL_INPUT_STRUCT	*wrq)
{
	PSTRING		msg;
	INT 		i=0;	 
	INT			WaitCnt;
	INT 		Status=0;	
    INT         max_len = LINE_LEN;		
	PBSS_ENTRY	pBss;
	UINT32 TotalLen, BufLen = IW_SCAN_MAX_DATA;


	TotalLen = sizeof(CHAR)*((MAX_LEN_OF_BSS_TABLE)*max_len) + 100;


	os_alloc_mem(NULL, (PUCHAR *)&msg, TotalLen);

	if (msg == NULL)
	{   
		DBGPRINT(RT_DEBUG_TRACE, ("RTMPIoctlGetSiteSurvey - msg memory alloc fail.\n"));
		return;
	}

	memset(msg, 0 , TotalLen);
	sprintf(msg,"%s","\n");

	sprintf(msg+strlen(msg),"%-4s%-33s%-20s%-23s%-9s%-7s%-7s%-3s\n",
	    "Ch", "SSID", "BSSID", "Security", "Siganl(%)", "W-Mode", " ExtCH"," NT");	

#ifdef WSC_INCLUDED
	sprintf(msg+strlen(msg)-1,"%-4s%-5s\n", " WPS", " DPID");
#endif /* WSC_INCLUDED */


	WaitCnt = 0;
	while ((ScanRunning(pAdapter) == TRUE) && (WaitCnt++ < 200))
		OS_WAIT(500);	

	for(i=0; i<pAdapter->ScanTab.BssNr ;i++)
	{
		pBss = &pAdapter->ScanTab.BssEntry[i];
		
		if( pBss->Channel==0)
			break;

		if((strlen(msg)+100 ) >= BufLen)
			break;


		RTMPCommSiteSurveyData(msg, pBss, TotalLen);
		
#ifdef WSC_INCLUDED
        /*WPS*/
        if (pBss->WpsAP & 0x01)
			sprintf(msg+strlen(msg)-1,"%-4s", " YES");
		else
			sprintf(msg+strlen(msg)-1,"%-4s", "  NO");

		if (pBss->WscDPIDFromWpsAP == DEV_PASS_ID_PIN)
			sprintf(msg+strlen(msg),"%-5s\n", " PIN");
		else if (pBss->WscDPIDFromWpsAP == DEV_PASS_ID_PBC)
			sprintf(msg+strlen(msg),"%-5s\n", " PBC");
		else
			sprintf(msg+strlen(msg),"%-5s\n", " ");
#endif /* WSC_INCLUDED */

	}

	wrq->u.data.length = strlen(msg);
	Status = copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length);

	DBGPRINT(RT_DEBUG_TRACE, ("RTMPIoctlGetSiteSurvey - wrq->u.data.length = %d\n", wrq->u.data.length));
	os_free_mem(NULL, (PUCHAR)msg);	
}
#endif




#define	MAC_LINE_LEN	(1+14+4+4+4+4+10+10+10+6+6)	/* "\n"+Addr+aid+psm+datatime+rxbyte+txbyte+current tx rate+last tx rate+"\n" */
VOID RTMPIoctlGetMacTable(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	INT i;
/*	RT_802_11_MAC_TABLE MacTab;*/
	RT_802_11_MAC_TABLE *pMacTab = NULL;
	RT_802_11_MAC_ENTRY *pDst;
	MAC_TABLE_ENTRY *pEntry;
	char *msg;

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **)&pMacTab, sizeof(RT_802_11_MAC_TABLE));
	if (pMacTab == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __FUNCTION__));
		return;
	}

	NdisZeroMemory(pMacTab, sizeof(RT_802_11_MAC_TABLE));
	pMacTab->Num = 0;
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		pEntry = &(pAd->MacTab.Content[i]);
		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC))
		{
			pDst = &pMacTab->Entry[pMacTab->Num];

			pDst->ApIdx = (UCHAR)pEntry->apidx;
			COPY_MAC_ADDR(pDst->Addr, &pEntry->Addr);
			pDst->Aid = (UCHAR)pEntry->Aid;
			pDst->Psm = pEntry->PsMode;
#ifdef DOT11_N_SUPPORT
			pDst->MimoPs = pEntry->MmpsMode;
#endif /* DOT11_N_SUPPORT */

			/* Fill in RSSI per entry*/
			pDst->AvgRssi0 = pEntry->RssiSample.AvgRssi0;
			pDst->AvgRssi1 = pEntry->RssiSample.AvgRssi1;
			pDst->AvgRssi2 = pEntry->RssiSample.AvgRssi2;

			/* the connected time per entry*/
			pDst->ConnectedTime = pEntry->StaConnectTime;
			pDst->TxRate.word = pEntry->HTPhyMode.word;
									
									
			pMacTab->Num += 1;
		}
	}

	wrq->u.data.length = sizeof(RT_802_11_MAC_TABLE);
	if (copy_to_user(wrq->u.data.pointer, pMacTab, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_TRACE, ("%s: copy_to_user() fail\n", __FUNCTION__));
	}


/*	msg = kmalloc(sizeof(CHAR)*(MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN), MEM_ALLOC_FLAG);*/
	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN));
	if (msg == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s():Alloc memory failed\n", __FUNCTION__));
		goto LabelOK;
	}
	memset(msg, 0 ,MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN );
	sprintf(msg,"%s","\n");
	sprintf(msg+strlen(msg),"%-14s%-4s%-4s%-4s%-4s%-6s%-6s%-10s%-10s%-10s\n",
		"MAC", "AP",  "AID", "PSM", "AUTH", "CTxR", "LTxR","LDT", "RxB", "TxB");
	
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC))
		{
			if((strlen(msg)+MAC_LINE_LEN ) >= (MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN) )
				break;	
			sprintf(msg+strlen(msg),"%02x%02x%02x%02x%02x%02x  ",
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->apidx);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->Aid);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->PsMode);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->AuthState);
			sprintf(msg+strlen(msg),"%-6d",RateIdToMbps[pAd->MacTab.Content[i].CurrTxRate]);
			sprintf(msg+strlen(msg),"%-6d",0/*RateIdToMbps[pAd->MacTab.Content[i].HTPhyMode.word]*/); /* ToDo*/
			sprintf(msg+strlen(msg),"%-10d",0/*pAd->MacTab.Content[i].HSCounter.LastDataPacketTime*/); /* ToDo*/
			sprintf(msg+strlen(msg),"%-10d",0/*pAd->MacTab.Content[i].HSCounter.TotalRxByteCount*/); /* ToDo*/
			sprintf(msg+strlen(msg),"%-10d\n",0/*pAd->MacTab.Content[i].HSCounter.TotalTxByteCount*/); /* ToDo*/

		}
	} 
	/* for compatible with old API just do the printk to console*/

	DBGPRINT(RT_DEBUG_TRACE, ("%s", msg));
/*	kfree(msg);*/
	os_free_mem(NULL, msg);

LabelOK:
	if (pMacTab != NULL)
		os_free_mem(NULL, pMacTab);
}

#ifdef INF_AR9
#ifdef AR9_MAPI_SUPPORT
#ifdef CONFIG_AP_SUPPORT
VOID RTMPAR9IoctlGetMacTable(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	INT i;
	char *msg;

/*	msg = kmalloc(sizeof(CHAR)*(MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN), MEM_ALLOC_FLAG);*/
	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN));
	if (msg == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s():Alloc memory failed\n", __FUNCTION__));
		return;
	}
	memset(msg, 0 ,MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN );
	sprintf(msg,"%s","\n");
	sprintf(msg+strlen(msg),"%-14s%-4s%-4s%-4s%-4s%-6s%-6s%-10s%-10s%-10s\n",
		"MAC", "AP",  "AID", "PSM", "AUTH", "CTxR", "LTxR","LDT", "RxB", "TxB");
	
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		if (IS_ENTRY_CLIENT(pEntry) && (pEntry->Sst == SST_ASSOC))
		{
			if((strlen(msg)+MAC_LINE_LEN ) >= (MAX_LEN_OF_MAC_TABLE*MAC_LINE_LEN) )
				break;	
			sprintf(msg+strlen(msg),"%02x%02x%02x%02x%02x%02x  ",
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->apidx);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->Aid);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->PsMode);
			sprintf(msg+strlen(msg),"%-4d", (int)pEntry->AuthState);
			sprintf(msg+strlen(msg),"%-6d",RateIdToMbps[pAd->MacTab.Content[i].CurrTxRate]);
			sprintf(msg+strlen(msg),"%-6d",0/*RateIdToMbps[pAd->MacTab.Content[i].HTPhyMode.word]*/); /* ToDo*/
			sprintf(msg+strlen(msg),"%-10d",0/*pAd->MacTab.Content[i].HSCounter.LastDataPacketTime*/); /* ToDo*/
			sprintf(msg+strlen(msg),"%-10d",0/*pAd->MacTab.Content[i].HSCounter.TotalRxByteCount*/); /* ToDo*/
			sprintf(msg+strlen(msg),"%-10d\n",0/*pAd->MacTab.Content[i].HSCounter.TotalTxByteCount*/); /* ToDo*/

		}
	} 
	/* for compatible with old API just do the printk to console*/
	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_OFF, ("%s", msg));
	}

/*	kfree(msg);*/
	os_free_mem(NULL, msg);
}

VOID RTMPIoctlGetSTAT2(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	char *msg;
	PMULTISSID_STRUCT	pMbss;
	INT apidx;


/*	msg = kmalloc(sizeof(CHAR)*(pAd->ApCfg.BssidNum*(14*128)), MEM_ALLOC_FLAG);*/
	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(pAd->ApCfg.BssidNum*(14*128)));
	if (msg == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s():Alloc memory failed\n", __FUNCTION__));
		return;
	}
	memset(msg, 0 ,pAd->ApCfg.BssidNum*(14*128));
	sprintf(msg,"%s","\n");
	
	for (apidx=0; apidx<pAd->ApCfg.BssidNum; apidx++)
	{
		pMbss=&pAd->ApCfg.MBSSID[apidx];
		
		sprintf(msg+strlen(msg),"ra%d\n",apidx);
		sprintf(msg+strlen(msg),"bytesTx = %ld\n",(pMbss->TransmittedByteCount));
		sprintf(msg+strlen(msg),"bytesRx = %ld\n",(pMbss->ReceivedByteCount));
		sprintf(msg+strlen(msg),"pktsTx = %ld\n",pMbss->TxCount);
		sprintf(msg+strlen(msg),"pktsRx = %ld\n",pMbss->RxCount);
		sprintf(msg+strlen(msg),"errorsTx = %ld\n",pMbss->TxErrorCount);
		sprintf(msg+strlen(msg),"errorsRx = %ld\n",pMbss->RxErrorCount);
		sprintf(msg+strlen(msg),"discardPktsTx = %ld\n",pMbss->TxDropCount);
		sprintf(msg+strlen(msg),"discardPktsRx = %ld\n",pMbss->RxDropCount);
		sprintf(msg+strlen(msg),"ucPktsTx = %ld\n",pMbss->ucPktsTx);
		sprintf(msg+strlen(msg),"ucPktsRx = %ld\n",pMbss->ucPktsRx);
		sprintf(msg+strlen(msg),"mcPktsTx = %ld\n",pMbss->mcPktsTx);
		sprintf(msg+strlen(msg),"mcPktsRx = %ld\n",pMbss->mcPktsRx);
		sprintf(msg+strlen(msg),"bcPktsTx = %ld\n",pMbss->bcPktsTx);
		sprintf(msg+strlen(msg),"bcPktsRx = %ld\n",pMbss->bcPktsRx);
		
	}

	wrq->u.data.length = strlen(msg);
	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_OFF, ("%s", msg));
	}

/*	kfree(msg);*/
	os_free_mem(NULL, msg);
}


VOID RTMPIoctlGetRadioDynInfo(
	IN PRTMP_ADAPTER pAd, 
	IN RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	char *msg;
	PMULTISSID_STRUCT	pMbss;
	INT status,bandwidth,ShortGI;
	

/*	msg = kmalloc(sizeof(CHAR)*(4096), MEM_ALLOC_FLAG);*/
	os_alloc_mem(NULL, (UCHAR **)&msg, sizeof(CHAR)*(4096));
	if (msg == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s():Alloc memory failed\n", __FUNCTION__));
		return;
	}
	memset(msg, 0 ,4096);
	sprintf(msg,"%s","\n");
	

		pMbss=&pAd->ApCfg.MBSSID[0];
		if(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF))
			status = 0;
		else
			status = 1;

		if(pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth  == BW_40)
			bandwidth = 1;
		else
			bandwidth = 0;

		if(pAd->CommonCfg.RegTransmitSetting.field.ShortGI == GI_800)
			ShortGI = 1;
		else
			ShortGI = 0;

		
		sprintf(msg+strlen(msg),"status = %d\n",status);
		sprintf(msg+strlen(msg),"channelsInUse = %d\n",pAd->ChannelListNum);
		sprintf(msg+strlen(msg),"channel = %d\n",pAd->CommonCfg.Channel);
		sprintf(msg+strlen(msg),"chanWidth = %d\n",bandwidth);
		sprintf(msg+strlen(msg),"guardIntvl = %d\n",ShortGI);
		sprintf(msg+strlen(msg),"MCS = %d\n",pMbss->DesiredTransmitSetting.field.MCS);
		
	wrq->u.data.length = strlen(msg);

	if (copy_to_user(wrq->u.data.pointer, msg, wrq->u.data.length))
	{
		DBGPRINT(RT_DEBUG_OFF, ("%s", msg));
	}

/*	kfree(msg);*/
	os_free_mem(NULL, msg);
}
#endif/*CONFIG_AP_SUPPORT*/
#endif/*AR9_MAPI_SUPPORT*/
#endif/* INF_AR9 */

#ifdef DOT11_N_SUPPORT
INT	Set_BASetup_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    UCHAR mac[6], tid;
	PSTRING token;
	STRING sepValue[] = ":", DASH = '-';
	INT i;
    MAC_TABLE_ENTRY *pEntry;

/*
	The BASetup inupt string format should be xx:xx:xx:xx:xx:xx-d, 
		=>The six 2 digit hex-decimal number previous are the Mac address, 
		=>The seventh decimal number is the tid value.
*/
	/*DBGPRINT(RT_DEBUG_TRACE,("\n%s\n", arg));*/
	
	if(strlen(arg) < 19)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 plus the "-" and tid value in decimal format.*/
		return FALSE;

	token = strchr(arg, DASH);
	if ((token != NULL) && (strlen(token)>1))
	{
		tid = (UCHAR) simple_strtol((token+1), 0, 10);
		/* tid is 0 ~ 7; Or kernel will crash in BAOriSessionSetUp() */
		if (tid > (NUM_OF_TID-1))
			return FALSE;
		
		*token = '\0';
		for (i = 0, token = rstrtok(arg, &sepValue[0]); token; token = rstrtok(NULL, &sepValue[0]), i++)
		{
			if((strlen(token) != 2) || (!isxdigit(*token)) || (!isxdigit(*(token+1))))
				return FALSE;
			AtoH(token, (&mac[i]), 1);
		}
		if(i != 6)
			return FALSE;

		DBGPRINT(RT_DEBUG_OFF, ("\n%02x:%02x:%02x:%02x:%02x:%02x-%02x\n", 
								mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], tid));

	    pEntry = MacTableLookup(pAd, (PUCHAR) mac);

    	if (pEntry) {
        	DBGPRINT(RT_DEBUG_OFF, ("\nSetup BA Session: Tid = %d\n", tid));
	        BAOriSessionSetUp(pAd, pEntry, tid, 0, 100, TRUE);
    	}

		return TRUE;
	}

	return FALSE;

}

INT	Set_BADecline_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG bBADecline;

	bBADecline = simple_strtol(arg, 0, 10);

	if (bBADecline == 0)
	{
		pAd->CommonCfg.bBADecline = FALSE;
	}
	else if (bBADecline == 1)
	{
		pAd->CommonCfg.bBADecline = TRUE;
	}
	else 
	{
		return FALSE; /*Invalid argument*/
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Set_BADecline_Proc::(BADecline=%d)\n", pAd->CommonCfg.bBADecline));

	return TRUE;
}

INT	Set_BAOriTearDown_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    UCHAR mac[6], tid;
	PSTRING token;
	STRING sepValue[] = ":", DASH = '-';
	INT i;
    MAC_TABLE_ENTRY *pEntry;

    /*DBGPRINT(RT_DEBUG_TRACE,("\n%s\n", arg));*/
/*
	The BAOriTearDown inupt string format should be xx:xx:xx:xx:xx:xx-d, 
		=>The six 2 digit hex-decimal number previous are the Mac address, 
		=>The seventh decimal number is the tid value.
*/
    if(strlen(arg) < 19)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 plus the "-" and tid value in decimal format.*/
		return FALSE;

	token = strchr(arg, DASH);
	if ((token != NULL) && (strlen(token)>1))
	{
		tid = simple_strtol((token+1), 0, 10);
		/* tid will be 0 ~ 7; Or kernel will crash in BAOriSessionTearDown() */
		if (tid > (NUM_OF_TID-1))
			return FALSE;
		
		*token = '\0';
		for (i = 0, token = rstrtok(arg, &sepValue[0]); token; token = rstrtok(NULL, &sepValue[0]), i++)
		{
			if((strlen(token) != 2) || (!isxdigit(*token)) || (!isxdigit(*(token+1))))
				return FALSE;
			AtoH(token, (&mac[i]), 1);
		}
		if(i != 6)
			return FALSE;

	    DBGPRINT(RT_DEBUG_OFF, ("\n%02x:%02x:%02x:%02x:%02x:%02x-%02x", 
								mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], tid));

	    pEntry = MacTableLookup(pAd, (PUCHAR) mac);

	    if (pEntry) {
	        DBGPRINT(RT_DEBUG_OFF, ("\nTear down Ori BA Session: Tid = %d\n", tid));
	        BAOriSessionTearDown(pAd, pEntry->Aid, tid, FALSE, TRUE);
	    }

		return TRUE;
	}

	return FALSE;

}

INT	Set_BARecTearDown_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    UCHAR mac[6], tid;
	PSTRING token;
	STRING sepValue[] = ":", DASH = '-';
	INT i;
    MAC_TABLE_ENTRY *pEntry;

    /*DBGPRINT(RT_DEBUG_TRACE,("\n%s\n", arg));*/
/*
	The BARecTearDown inupt string format should be xx:xx:xx:xx:xx:xx-d, 
		=>The six 2 digit hex-decimal number previous are the Mac address, 
		=>The seventh decimal number is the tid value.
*/
    if(strlen(arg) < 19)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 plus the "-" and tid value in decimal format.*/
		return FALSE;

	token = strchr(arg, DASH);
	if ((token != NULL) && (strlen(token)>1))
	{
		tid = simple_strtol((token+1), 0, 10);
		/* tid will be 0 ~ 7; Or kernel will crash in BARecSessionTearDown() */
		if (tid > (NUM_OF_TID-1))
			return FALSE;
		
		*token = '\0';
		for (i = 0, token = rstrtok(arg, &sepValue[0]); token; token = rstrtok(NULL, &sepValue[0]), i++)
		{
			if((strlen(token) != 2) || (!isxdigit(*token)) || (!isxdigit(*(token+1))))
				return FALSE;
			AtoH(token, (&mac[i]), 1);
		}
		if(i != 6)
			return FALSE;

		DBGPRINT(RT_DEBUG_OFF, ("\n%02x:%02x:%02x:%02x:%02x:%02x-%02x", 
								mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], tid));

		pEntry = MacTableLookup(pAd, (PUCHAR) mac);

		if (pEntry) {
		    DBGPRINT(RT_DEBUG_OFF, ("\nTear down Rec BA Session: Tid = %d\n", tid));
		    BARecSessionTearDown(pAd, pEntry->Aid, tid, FALSE);
		}

		return TRUE;
	}

	return FALSE;

}

INT	Set_HtBw_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG HtBw;

	HtBw = simple_strtol(arg, 0, 10);
	if (HtBw == BW_40)
		pAd->CommonCfg.RegTransmitSetting.field.BW  = BW_40;
	else if (HtBw == BW_20)
		pAd->CommonCfg.RegTransmitSetting.field.BW  = BW_20;
	else
		return FALSE;  /*Invalid argument */

	SetCommonHT(pAd);
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtBw_Proc::(HtBw=%d)\n", pAd->CommonCfg.RegTransmitSetting.field.BW));

	return TRUE;
}

INT	Set_HtMcs_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG HtMcs, Mcs_tmp, ValidMcs = 15;
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
#endif /* CONFIG_AP_SUPPORT */	

#ifdef DOT11N_SS3_SUPPORT
	if (pAd->CommonCfg.TxStream >= 3)
		ValidMcs = 23;
#endif /* DOT11N_SS3_SUPPORT */

	Mcs_tmp = simple_strtol(arg, 0, 10);
		
	if (Mcs_tmp <= ValidMcs || Mcs_tmp == 32)			
		HtMcs = Mcs_tmp;	
	else
		HtMcs = MCS_AUTO;	

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredTransmitSetting.field.MCS = HtMcs;
		DBGPRINT(RT_DEBUG_TRACE, ("Set_HtMcs_Proc::(HtMcs=%d) for ra%d\n", 
				pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredTransmitSetting.field.MCS, pObj->ioctl_if));
	}
#endif /* CONFIG_AP_SUPPORT */

	SetCommonHT(pAd);
	
	return TRUE;
}

INT	Set_HtGi_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG HtGi;

	HtGi = simple_strtol(arg, 0, 10);
		
	if ( HtGi == GI_400)			
		pAd->CommonCfg.RegTransmitSetting.field.ShortGI = GI_400;
	else if ( HtGi == GI_800 )
		pAd->CommonCfg.RegTransmitSetting.field.ShortGI = GI_800;
	else 
		return FALSE; /* Invalid argument */

	SetCommonHT(pAd);
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtGi_Proc::(ShortGI=%d)\n",pAd->CommonCfg.RegTransmitSetting.field.ShortGI));

	return TRUE;
}


INT	Set_HtTxBASize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR Size;

	Size = simple_strtol(arg, 0, 10);
		
	if (Size <=0 || Size >=64)
	{
		Size = 8;
	}
	pAd->CommonCfg.TxBASize = Size-1;
	DBGPRINT(RT_DEBUG_ERROR, ("Set_HtTxBASize ::(TxBASize= %d)\n", Size));

	return TRUE;
}

INT	Set_HtDisallowTKIP_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);
		
	if (Value == 1)
	{
		pAd->CommonCfg.HT_DisallowTKIP = TRUE;
	}
	else
	{
		pAd->CommonCfg.HT_DisallowTKIP = FALSE;
	}
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtDisallowTKIP_Proc ::%s\n", 
				(pAd->CommonCfg.HT_DisallowTKIP == TRUE) ? "enabled" : "disabled"));

	return TRUE;
}

INT	Set_HtOpMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{

	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);

	if (Value == HTMODE_GF)
		pAd->CommonCfg.RegTransmitSetting.field.HTMODE  = HTMODE_GF;
	else if ( Value == HTMODE_MM )
		pAd->CommonCfg.RegTransmitSetting.field.HTMODE  = HTMODE_MM;
	else 
		return FALSE; /*Invalid argument */

	SetCommonHT(pAd);
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtOpMode_Proc::(HtOpMode=%d)\n",pAd->CommonCfg.RegTransmitSetting.field.HTMODE));

	return TRUE;

}	

INT	Set_HtStbc_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{

	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	
	if (Value == STBC_USE)
		pAd->CommonCfg.RegTransmitSetting.field.STBC = STBC_USE;
	else if ( Value == STBC_NONE )
		pAd->CommonCfg.RegTransmitSetting.field.STBC = STBC_NONE;
	else 
		return FALSE; /*Invalid argument */

	SetCommonHT(pAd);
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_Stbc_Proc::(HtStbc=%d)\n",pAd->CommonCfg.RegTransmitSetting.field.STBC));

	return TRUE;											
}

INT	Set_HtHtc_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{

	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->HTCEnable = FALSE;
	else if ( Value ==1 )
        pAd->HTCEnable = TRUE;
	else 
		return FALSE; /*Invalid argument 	*/
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtHtc_Proc::(HtHtc=%d)\n",pAd->HTCEnable));

	return TRUE;		
}
			
INT	Set_HtExtcha_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{

	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	
	if (Value == 0)
		pAd->CommonCfg.RegTransmitSetting.field.EXTCHA  = EXTCHA_BELOW;
	else if ( Value ==1 )
        pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_ABOVE;
	else 
		return FALSE; /*Invalid argument 	*/
	
	SetCommonHT(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtExtcha_Proc::(HtExtcha=%d)\n",pAd->CommonCfg.RegTransmitSetting.field.EXTCHA));

	return TRUE;			
}

INT	Set_HtMpduDensity_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	
	if (Value <=7)
		pAd->CommonCfg.BACapability.field.MpduDensity = Value;
	else
		pAd->CommonCfg.BACapability.field.MpduDensity = 4;

	SetCommonHT(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtMpduDensity_Proc::(HtMpduDensity=%d)\n",pAd->CommonCfg.BACapability.field.MpduDensity));

	return TRUE;																																	
}

INT	Set_HtBaWinSize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);

#ifdef CONFIG_AP_SUPPORT
	/* Intel IOT*/
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		Value = 64;
#endif /* CONFIG_AP_SUPPORT */

	if (Value >=1 && Value <= 64)
	{
		pAd->CommonCfg.REGBACapability.field.RxBAWinLimit = Value;
		pAd->CommonCfg.BACapability.field.RxBAWinLimit = Value;
	}
	else
	{
        pAd->CommonCfg.REGBACapability.field.RxBAWinLimit = 64;
		pAd->CommonCfg.BACapability.field.RxBAWinLimit = 64;
	}
	
	SetCommonHT(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtBaWinSize_Proc::(HtBaWinSize=%d)\n",pAd->CommonCfg.BACapability.field.RxBAWinLimit));

	return TRUE;																																	
}		

INT	Set_HtRdg_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	
	if (Value == 0)			
		pAd->CommonCfg.bRdg = FALSE;
	else if ( Value ==1 )
	{
		pAd->HTCEnable = TRUE;
        pAd->CommonCfg.bRdg = TRUE;
	}
	else 
		return FALSE; /*Invalid argument*/
	
	SetCommonHT(pAd);	
		
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtRdg_Proc::(HtRdg=%d)\n",pAd->CommonCfg.bRdg));

	return TRUE;																																	
}		

INT	Set_HtLinkAdapt_Proc(																																																																																																																																																																																																																																																																																																																			
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->bLinkAdapt = FALSE;
	else if ( Value ==1 )
	{
			pAd->HTCEnable = TRUE;
			pAd->bLinkAdapt = TRUE;
	}
	else
		return FALSE; /*Invalid argument*/
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtLinkAdapt_Proc::(HtLinkAdapt=%d)\n",pAd->bLinkAdapt));

	return TRUE;																																	
}		

INT	Set_HtAmsdu_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->CommonCfg.BACapability.field.AmsduEnable = FALSE;
	else if ( Value == 1 )
        pAd->CommonCfg.BACapability.field.AmsduEnable = TRUE;
	else
		return FALSE; /*Invalid argument*/
	
	SetCommonHT(pAd);	
		
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtAmsdu_Proc::(HtAmsdu=%d)\n",pAd->CommonCfg.BACapability.field.AmsduEnable));

	return TRUE;																																	
}			

INT	Set_HtAutoBa_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
	{
		pAd->CommonCfg.BACapability.field.AutoBA = FALSE;
		pAd->CommonCfg.BACapability.field.Policy = BA_NOTUSE;
	}
    else if (Value == 1)	
    {
		pAd->CommonCfg.BACapability.field.AutoBA = TRUE;
		pAd->CommonCfg.BACapability.field.Policy = IMMED_BA;
    }
	else
		return FALSE; /*Invalid argument*/
	
    pAd->CommonCfg.REGBACapability.field.AutoBA = pAd->CommonCfg.BACapability.field.AutoBA;
	pAd->CommonCfg.REGBACapability.field.Policy = pAd->CommonCfg.BACapability.field.Policy;
	SetCommonHT(pAd);	
		
	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtAutoBa_Proc::(HtAutoBa=%d)\n",pAd->CommonCfg.BACapability.field.AutoBA));

	return TRUE;				
		
}		
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						
INT	Set_HtProtect_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->CommonCfg.bHTProtect = FALSE;
    else if (Value == 1)	
		pAd->CommonCfg.bHTProtect = TRUE;
	else
		return FALSE; /*Invalid argument*/

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtProtect_Proc::(HtProtect=%d)\n",pAd->CommonCfg.bHTProtect));

	return TRUE;
}

INT	Set_SendPSMPAction_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
    UCHAR mac[6], mode;
	PSTRING token;
	STRING sepValue[] = ":", DASH = '-';
	INT i;
    MAC_TABLE_ENTRY *pEntry;

    /*DBGPRINT(RT_DEBUG_TRACE,("\n%s\n", arg));*/
/*
	The BARecTearDown inupt string format should be xx:xx:xx:xx:xx:xx-d, 
		=>The six 2 digit hex-decimal number previous are the Mac address, 
		=>The seventh decimal number is the mode value.
*/
    if(strlen(arg) < 19)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 plus the "-" and mode value in decimal format.*/
		return FALSE;

   	token = strchr(arg, DASH);
	if ((token != NULL) && (strlen(token)>1))
	{
		mode = simple_strtol((token+1), 0, 10);
		if (mode > MMPS_ENABLE)
			return FALSE;
		
		*token = '\0';
		for (i = 0, token = rstrtok(arg, &sepValue[0]); token; token = rstrtok(NULL, &sepValue[0]), i++)
		{
			if((strlen(token) != 2) || (!isxdigit(*token)) || (!isxdigit(*(token+1))))
				return FALSE;
			AtoH(token, (&mac[i]), 1);
		}
		if(i != 6)
			return FALSE;

		DBGPRINT(RT_DEBUG_OFF, ("\n%02x:%02x:%02x:%02x:%02x:%02x-%02x", 
								mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mode));

		pEntry = MacTableLookup(pAd, mac);

		if (pEntry) {
		    DBGPRINT(RT_DEBUG_OFF, ("\nSendPSMPAction MIPS mode = %d\n", mode));
		    SendPSMPAction(pAd, pEntry->Aid, mode);
		}

		return TRUE;
	}

	return FALSE;


}

INT	Set_HtMIMOPSmode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;
	
	Value = simple_strtol(arg, 0, 10);
	
	if (Value <=3)
		pAd->CommonCfg.BACapability.field.MMPSmode = Value;
	else
		pAd->CommonCfg.BACapability.field.MMPSmode = 3;

	SetCommonHT(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtMIMOPSmode_Proc::(MIMOPS mode=%d)\n",pAd->CommonCfg.BACapability.field.MMPSmode));

	return TRUE;																																	
}

#ifdef CONFIG_AP_SUPPORT
/* 
    ==========================================================================
    Description:
        Set Tx Stream number
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_HtTxStream_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG 	Value;	
		
	Value = simple_strtol(arg, 0, 10);

	if ((Value <= 3) && (Value >= 1) && (Value <= pAd->Antenna.field.TxPath)) /* 3*3*/
		pAd->CommonCfg.TxStream = Value;
	else
		pAd->CommonCfg.TxStream = pAd->Antenna.field.TxPath;

	if ((pAd->MACVersion < RALINK_2883_VERSION) &&
		(pAd->CommonCfg.TxStream > 2))
	{
		pAd->CommonCfg.TxStream = 2; /* only 2 TX streams for RT2860 series*/
	}

	SetCommonHT(pAd);

	APStop(pAd);
	APStartUp(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtTxStream_Proc::(Tx Stream=%d)\n",pAd->CommonCfg.TxStream));
		
	return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Rx Stream number
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_HtRxStream_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG 	Value;	
		
	Value = simple_strtol(arg, 0, 10);

	if ((Value <= 3) && (Value >= 1) && (Value <= pAd->Antenna.field.RxPath))
		pAd->CommonCfg.RxStream = Value;
	else
		pAd->CommonCfg.RxStream = pAd->Antenna.field.RxPath;

	if ((pAd->MACVersion < RALINK_2883_VERSION) &&
		(pAd->CommonCfg.RxStream > 2)) /* 3*3*/
	{
		pAd->CommonCfg.RxStream = 2; /* only 2 RX streams for RT2860 series*/
	}

	SetCommonHT(pAd);

	APStop(pAd);
	APStartUp(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtRxStream_Proc::(Rx Stream=%d)\n",pAd->CommonCfg.RxStream));
		
	return TRUE;
}

#ifdef DOT11_N_SUPPORT
#ifdef GREENAP_SUPPORT
INT	Set_GreenAP_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
	{
		pAd->ApCfg.bGreenAPActive=FALSE;
		pAd->ApCfg.bGreenAPEnable = FALSE;
	}
	else if (Value == 1)	
		pAd->ApCfg.bGreenAPEnable = TRUE;
	else
		return FALSE; /*Invalid argument*/

	DBGPRINT(RT_DEBUG_TRACE, ("Set_GreenAP_Proc::(bGreenAPEnable=%d)\n",pAd->ApCfg.bGreenAPEnable));

	return TRUE;
}
#endif /* GREENAP_SUPPORT */
#endif /* DOT11_N_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

INT	Set_ForceShortGI_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->WIFItestbed.bShortGI = FALSE;
	else if (Value == 1)	
		pAd->WIFItestbed.bShortGI = TRUE;
	else
		return FALSE; /*Invalid argument*/

	SetCommonHT(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_ForceShortGI_Proc::(ForceShortGI=%d)\n", pAd->WIFItestbed.bShortGI));

	return TRUE;
}



INT	Set_ForceGF_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->WIFItestbed.bGreenField = FALSE;
	else if (Value == 1)	
		pAd->WIFItestbed.bGreenField = TRUE;
	else
		return FALSE; /*Invalid argument*/

	SetCommonHT(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("Set_ForceGF_Proc::(ForceGF=%d)\n", pAd->WIFItestbed.bGreenField));

	return TRUE;
}

INT	Set_HtMimoPs_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);
	if (Value == 0)
		pAd->CommonCfg.bMIMOPSEnable = FALSE;
	else if (Value == 1)	
		pAd->CommonCfg.bMIMOPSEnable = TRUE;
	else
		return FALSE; /*Invalid argument*/

	DBGPRINT(RT_DEBUG_TRACE, ("Set_HtMimoPs_Proc::(HtMimoPs=%d)\n",pAd->CommonCfg.bMIMOPSEnable));

	return TRUE;
}


#ifdef DOT11N_DRAFT3
INT Set_HT_BssCoex_Proc(
	IN	PRTMP_ADAPTER		pAd,
	IN	PSTRING				pParam)
{
	UCHAR bBssCoexEnable = simple_strtol(pParam, 0, 10);

	pAd->CommonCfg.bBssCoexEnable = ((bBssCoexEnable == 1) ? TRUE: FALSE);

	DBGPRINT(RT_DEBUG_TRACE, ("Set bBssCoexEnable=%d!\n", pAd->CommonCfg.bBssCoexEnable));
	
	return TRUE;
}


INT Set_HT_BssCoexApCntThr_Proc(
	IN	PRTMP_ADAPTER		pAd,
	IN	PSTRING				pParam)
{
	pAd->CommonCfg.BssCoexApCntThr = simple_strtol(pParam, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE, ("Set BssCoexApCntThr=%d!\n", pAd->CommonCfg.BssCoexApCntThr));
	
	return TRUE;
}
#endif /* DOT11N_DRAFT3 */

#endif /* DOT11_N_SUPPORT */


INT	Set_FixedTxMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
#endif /* CONFIG_AP_SUPPORT */		
	INT	fix_tx_mode = FIXED_TXMODE_HT;

	fix_tx_mode = RT_CfgSetFixedTxPhyMode(arg);
	
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredTransmitSetting.field.FixedTxMode = fix_tx_mode;
#endif /* CONFIG_AP_SUPPORT */
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_FixedTxMode_Proc::(FixedTxMode=%d)\n", fix_tx_mode));

	return TRUE;
}

#ifdef CONFIG_APSTA_MIXED_SUPPORT
INT	Set_OpMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);

#ifdef RTMP_MAC_PCI
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
#endif /* RTMP_MAC_PCI */
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Can not switch operate mode on interface up !! \n"));
		return FALSE;
	}

	if (Value == 0)
		pAd->OpMode = OPMODE_STA;
	else if (Value == 1)	
		pAd->OpMode = OPMODE_AP;
	else
		return FALSE; /*Invalid argument*/

	DBGPRINT(RT_DEBUG_TRACE, ("Set_OpMode_Proc::(OpMode=%s)\n", pAd->OpMode == 1 ? "AP Mode" : "STA Mode"));

	return TRUE;
}
#endif /* CONFIG_APSTA_MIXED_SUPPORT */



#ifdef STREAM_MODE_SUPPORT
/*
	========================================================================
	Routine Description:
		Set the enable/disable the stream mode
		
	Arguments:
		1:	enable for 1SS 
		2:	enable for 2SS
		3:	enable for 1SS and 2SS
		0:	disable

	Notes:
		Currently only support 1SS
	========================================================================
*/
INT Set_StreamMode_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	UINT32 streamWord, reg, regAddr;
	
	if (pAd->chipCap.FlgHwStreamMode == FALSE)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("chip not supported feature\n"));
		return FALSE;
	}
	
	pAd->CommonCfg.StreamMode = (simple_strtol(arg, 0, 10) & 0x3);
	DBGPRINT(RT_DEBUG_TRACE, ("%s():StreamMode=%d\n", __FUNCTION__, pAd->CommonCfg.StreamMode));

	streamWord = StreamModeRegVal(pAd);
	for (regAddr = TX_CHAIN_ADDR0_H; regAddr <= TX_CHAIN_ADDR3_H; regAddr += 8)
	{
		RTMP_IO_READ32(pAd, regAddr, &reg);
		reg &= (~0x000F0000);
		RTMP_IO_WRITE32(pAd, regAddr, streamWord | reg);
	}

	return TRUE;
}


/*
	
*/
INT Set_StreamModeMac_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	return FALSE;
}

INT Set_StreamModeMCS_Proc(
	IN  PRTMP_ADAPTER   pAd,
	IN  PSTRING         arg)
{
	pAd->CommonCfg.StreamModeMCS = simple_strtol(arg, 0, 16);
	DBGPRINT(RT_DEBUG_TRACE, ("%s():StreamModeMCS=%02X\n", 
				__FUNCTION__, pAd->CommonCfg.StreamModeMCS));
	
	return TRUE;
}
#endif /* STREAM_MODE_SUPPORT */







INT Set_LongRetryLimit_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PSTRING			arg)
{
	TX_RTY_CFG_STRUC	tx_rty_cfg;
	UCHAR				LongRetryLimit = (UCHAR)simple_strtol(arg, 0, 10);

	RTMP_IO_READ32(pAdapter, TX_RTY_CFG, &tx_rty_cfg.word);
	tx_rty_cfg.field.LongRtyLimit = LongRetryLimit;
	RTMP_IO_WRITE32(pAdapter, TX_RTY_CFG, tx_rty_cfg.word);
	DBGPRINT(RT_DEBUG_TRACE, ("IF Set_LongRetryLimit_Proc::(tx_rty_cfg=0x%x)\n", tx_rty_cfg.word));
	return TRUE;
}

INT Set_ShortRetryLimit_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PSTRING			arg)
{
	TX_RTY_CFG_STRUC	tx_rty_cfg;
	UCHAR				ShortRetryLimit = (UCHAR)simple_strtol(arg, 0, 10);

	RTMP_IO_READ32(pAdapter, TX_RTY_CFG, &tx_rty_cfg.word);
	tx_rty_cfg.field.ShortRtyLimit = ShortRetryLimit;
	RTMP_IO_WRITE32(pAdapter, TX_RTY_CFG, tx_rty_cfg.word);	
	DBGPRINT(RT_DEBUG_TRACE, ("IF Set_ShortRetryLimit_Proc::(tx_rty_cfg=0x%x)\n", tx_rty_cfg.word));
	return TRUE;
}

INT Set_AutoFallBack_Proc(
	IN	PRTMP_ADAPTER	pAdapter, 
	IN	PSTRING			arg)
{
	return RT_CfgSetAutoFallBack(pAdapter, arg);
}



PSTRING RTMPGetRalinkAuthModeStr(
    IN  NDIS_802_11_AUTHENTICATION_MODE authMode)
{
	switch(authMode)
	{
		case Ndis802_11AuthModeOpen:
			return "OPEN";
		case Ndis802_11AuthModeWPAPSK:
			return "WPAPSK";
		case Ndis802_11AuthModeShared:
			return "SHARED";
		case Ndis802_11AuthModeAutoSwitch:
			return "WEPAUTO";
		case Ndis802_11AuthModeWPA:
			return "WPA";
		case Ndis802_11AuthModeWPA2:
			return "WPA2";
		case Ndis802_11AuthModeWPA2PSK:
			return "WPA2PSK";
        case Ndis802_11AuthModeWPA1PSKWPA2PSK:
			return "WPAPSKWPA2PSK";
        case Ndis802_11AuthModeWPA1WPA2:
			return "WPA1WPA2";
		case Ndis802_11AuthModeWPANone:
			return "WPANONE";
		default:
			return "UNKNOW";
	}
}

PSTRING RTMPGetRalinkEncryModeStr(
    IN  USHORT encryMode)
{
	switch(encryMode)
	{
		case Ndis802_11WEPDisabled:
			return "NONE";
		case Ndis802_11WEPEnabled:
			return "WEP";        
		case Ndis802_11Encryption2Enabled:
			return "TKIP";
		case Ndis802_11Encryption3Enabled:
			return "AES";
        case Ndis802_11Encryption4Enabled:
			return "TKIPAES";
		default:
			return "UNKNOW";
	}
}

INT RTMPShowCfgValue(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			pName,
	IN	PSTRING			pBuf,
	IN	UINT32			MaxLen)
{
	INT	Status = 0;	
	
	for (PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC = RTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC; PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->name; PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC++)
	{
		if (!strcmp(pName, PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->name)) 
		{						
			if(PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->show_proc(pAd, pBuf, MaxLen))
				Status = -EINVAL;
			break;  /*Exit for loop.*/
		}
	}

	if(PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->name == NULL)
	{
		snprintf(pBuf, MaxLen, "\n");
		for (PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC = RTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC; PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->name; PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC++)
		{
			if ((strlen(pBuf) + strlen(PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->name)) >= MaxLen)
				break;
			sprintf(pBuf, "%s%s\n", pBuf, PRTMP_PRIVATE_STA_SHOW_CFG_VALUE_PROC->name);
		}
	}
	
	return Status;
}

INT	Show_SSID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	UCHAR	ssid_str[33];


	NdisZeroMemory(&ssid_str[0], 33);
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
		NdisMoveMemory(&ssid_str[0], 
						pAd->ApCfg.MBSSID[pObj->ioctl_if].Ssid, 
						pAd->ApCfg.MBSSID[pObj->ioctl_if].SsidLen);
	}
#endif /* CONFIG_AP_SUPPORT */


	snprintf(pBuf, BufLen, "\t%s", ssid_str);
	return 0;
}

INT	Show_WirelessMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	switch(pAd->CommonCfg.PhyMode)
	{
		case PHY_11BG_MIXED:
			snprintf(pBuf, BufLen, "\t11B/G");
			break;
		case PHY_11B:
			snprintf(pBuf, BufLen, "\t11B");
			break;
		case PHY_11A:
			snprintf(pBuf, BufLen, "\t11A");
			break;
		case PHY_11ABG_MIXED:
			snprintf(pBuf, BufLen, "\t11A/B/G");
			break;
		case PHY_11G:
			snprintf(pBuf, BufLen, "\t11G");
			break;
#ifdef DOT11_N_SUPPORT
		case PHY_11ABGN_MIXED:
			snprintf(pBuf, BufLen, "\t11A/B/G/N");
			break;
		case PHY_11N_2_4G:
			snprintf(pBuf, BufLen, "\t11N only with 2.4G");
			break;
		case PHY_11GN_MIXED:
			snprintf(pBuf, BufLen, "\t11G/N");
			break;
		case PHY_11AN_MIXED:
			snprintf(pBuf, BufLen, "\t11A/N");
			break;
		case PHY_11BGN_MIXED:
			snprintf(pBuf, BufLen, "\t11B/G/N");
			break;
		case PHY_11AGN_MIXED:
			snprintf(pBuf, BufLen, "\t11A/G/N");
			break;
		case PHY_11N_5G:
			snprintf(pBuf, BufLen, "\t11N only with 5G");
			break;
#endif /* DOT11_N_SUPPORT */
		default:
			snprintf(pBuf, BufLen, "\tUnknow Value(%d)", pAd->CommonCfg.PhyMode);
			break;
	}
	return 0;
}


INT	Show_TxBurst_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.bEnableTxBurst ? "TRUE":"FALSE");
	return 0;
}

INT	Show_TxPreamble_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	switch(pAd->CommonCfg.TxPreamble)
	{
		case Rt802_11PreambleShort:
			snprintf(pBuf, BufLen, "\tShort");
			break;
		case Rt802_11PreambleLong:
			snprintf(pBuf, BufLen, "\tLong");
			break;
		case Rt802_11PreambleAuto:
			snprintf(pBuf, BufLen, "\tAuto");
			break;
		default:
			snprintf(pBuf, BufLen, "\tUnknown Value(%lu)", pAd->CommonCfg.TxPreamble);
			break;
	}
	
	return 0;
}

INT	Show_TxPower_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%lu", pAd->CommonCfg.TxPowerPercentage);
	return 0;
}

INT	Show_Channel_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%d", pAd->CommonCfg.Channel);
	return 0;
}

INT	Show_BGProtection_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	switch(pAd->CommonCfg.UseBGProtection)
	{
		case 1: /*Always On*/
			snprintf(pBuf, BufLen, "\tON");
			break;
		case 2: /*Always OFF*/
			snprintf(pBuf, BufLen, "\tOFF");
			break;
		case 0: /*AUTO*/
			snprintf(pBuf, BufLen, "\tAuto");
			break;
		default:
			snprintf(pBuf, BufLen, "\tUnknow Value(%lu)", pAd->CommonCfg.UseBGProtection);
			break;
	}
	return 0;
}

INT	Show_RTSThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%u", pAd->CommonCfg.RtsThreshold);
	return 0;
}

INT	Show_FragThreshold_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%u", pAd->CommonCfg.FragmentThreshold);
	return 0;
}

#ifdef DOT11_N_SUPPORT
INT	Show_HtBw_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	if (pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40)
	{
		snprintf(pBuf, BufLen, "\t40 MHz");
	}
	else
	{
        snprintf(pBuf, BufLen, "\t20 MHz");
	}
	return 0;
}

INT	Show_HtMcs_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		snprintf(pBuf, BufLen, "\t%u", pAd->ApCfg.MBSSID[pObj->ioctl_if].DesiredTransmitSetting.field.MCS);
#endif /* CONFIG_AP_SUPPORT */

	return 0;
}

INT	Show_HtGi_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	switch(pAd->CommonCfg.RegTransmitSetting.field.ShortGI)
	{
		case GI_400:
			snprintf(pBuf, BufLen, "\tGI_400");
			break;
		case GI_800:
			snprintf(pBuf, BufLen, "\tGI_800");
			break;
		default:
			snprintf(pBuf, BufLen, "\tUnknow Value(%u)", pAd->CommonCfg.RegTransmitSetting.field.ShortGI);
			break;
	}
	return 0;
}

INT	Show_HtOpMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	switch(pAd->CommonCfg.RegTransmitSetting.field.HTMODE)
	{
		case HTMODE_GF:
			snprintf(pBuf, BufLen, "\tGF");
			break;
		case HTMODE_MM:
			snprintf(pBuf, BufLen, "\tMM");
			break;
		default:
			snprintf(pBuf, BufLen, "\tUnknow Value(%u)", pAd->CommonCfg.RegTransmitSetting.field.HTMODE);
			break;
	}
	return 0;
}

INT	Show_HtExtcha_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	switch(pAd->CommonCfg.RegTransmitSetting.field.EXTCHA)
	{
		case EXTCHA_BELOW:
			snprintf(pBuf, BufLen, "\tBelow");
			break;
		case EXTCHA_ABOVE:
			snprintf(pBuf, BufLen, "\tAbove");
			break;
		default:
			snprintf(pBuf, BufLen, "\tUnknow Value(%u)", pAd->CommonCfg.RegTransmitSetting.field.EXTCHA);
			break;
	}
	return 0;
}


INT	Show_HtMpduDensity_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%u", pAd->CommonCfg.BACapability.field.MpduDensity);
	return 0;
}

INT	Show_HtBaWinSize_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%u", pAd->CommonCfg.BACapability.field.RxBAWinLimit);
	return 0;
}

INT	Show_HtRdg_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.bRdg ? "TRUE":"FALSE");
	return 0;
}

INT	Show_HtAmsdu_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.BACapability.field.AmsduEnable ? "TRUE":"FALSE");
	return 0;
}

INT	Show_HtAutoBa_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.BACapability.field.AutoBA ? "TRUE":"FALSE");
	return 0;
}
#endif /* DOT11_N_SUPPORT */

INT	Show_CountryRegion_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%d", pAd->CommonCfg.CountryRegion);
	return 0;
}

INT	Show_CountryRegionABand_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%d", pAd->CommonCfg.CountryRegionForABand);
	return 0;
}

INT	Show_CountryCode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.CountryCode);
	return 0;
}

#ifdef AGGREGATION_SUPPORT
INT	Show_PktAggregate_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.bAggregationCapable ? "TRUE":"FALSE");
	return 0;
}
#endif /* AGGREGATION_SUPPORT */

#ifdef WMM_SUPPORT
INT	Show_WmmCapable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		snprintf(pBuf, BufLen, "\t%s", pAd->ApCfg.MBSSID[pObj->ioctl_if].bWmmCapable ? "TRUE":"FALSE");
#endif /* CONFIG_AP_SUPPORT */

	
	return 0;
}
#endif /* WMM_SUPPORT */

INT	Show_IEEE80211H_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\t%s", pAd->CommonCfg.bIEEE80211H ? "TRUE":"FALSE");
	return 0;
}


INT	Show_AuthMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	NDIS_802_11_AUTHENTICATION_MODE	AuthMode = Ndis802_11AuthModeOpen; 
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		AuthMode = pAd->ApCfg.MBSSID[pObj->ioctl_if].AuthMode;
#endif /* CONFIG_AP_SUPPORT */


	if ((AuthMode >= Ndis802_11AuthModeOpen) && 
		(AuthMode <= Ndis802_11AuthModeWPA1PSKWPA2PSK))
		snprintf(pBuf, BufLen, "\t%s", RTMPGetRalinkAuthModeStr(AuthMode));
#ifdef WAPI_SUPPORT
	else if (AuthMode == Ndis802_11AuthModeWAICERT)
		snprintf(pBuf, BufLen, "\t%s", "WAI_CERT");
	else if (AuthMode == Ndis802_11AuthModeWAIPSK)
		snprintf(pBuf, BufLen, "\t%s", "WAI_PSK");				 
#endif /* WAPI_SUPPORT */
	else
		snprintf(pBuf, BufLen, "\tUnknow Value(%d)", AuthMode);
	
	return 0;
}

INT	Show_EncrypType_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	NDIS_802_11_WEP_STATUS	WepStatus = Ndis802_11WEPDisabled;
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		WepStatus = pAd->ApCfg.MBSSID[pObj->ioctl_if].WepStatus;
#endif /* CONFIG_AP_SUPPORT */


	if ((WepStatus >= Ndis802_11WEPEnabled) && 
		(WepStatus <= Ndis802_11Encryption4KeyAbsent))
		snprintf(pBuf, BufLen, "\t%s", RTMPGetRalinkEncryModeStr(WepStatus));
#ifdef WAPI_SUPPORT
	else if (WepStatus == Ndis802_11EncryptionSMS4Enabled)
		snprintf(pBuf, BufLen, "\t%s", "WPI_SMS4");
#endif /* WAPI_SUPPORT */
	else
		snprintf(pBuf, BufLen, "\tUnknow Value(%d)", WepStatus);
	
	return 0;
}

INT	Show_DefaultKeyID_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	UCHAR DefaultKeyId = 0;
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		DefaultKeyId = pAd->ApCfg.MBSSID[pObj->ioctl_if].DefaultKeyId;
#endif /* CONFIG_AP_SUPPORT */


	snprintf(pBuf, BufLen, "\t%d", DefaultKeyId);

	return 0;
}

INT	Show_WepKey_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN  INT				KeyIdx,
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	UCHAR   Key[16] = {0}, KeyLength = 0;
	INT		index = BSS0;
#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		index = pObj->ioctl_if;
#endif /* CONFIG_AP_SUPPORT */

	KeyLength = pAd->SharedKey[index][KeyIdx].KeyLen;
	NdisMoveMemory(Key, pAd->SharedKey[index][KeyIdx].Key, KeyLength);		
		
	/*check key string is ASCII or not*/
    if (RTMPCheckStrPrintAble((PCHAR)Key, KeyLength))
        sprintf(pBuf, "\t%s", Key);
    else
    {
        int idx;
        sprintf(pBuf, "\t");
        for (idx = 0; idx < KeyLength; idx++)
            sprintf(pBuf+strlen(pBuf), "%02X", Key[idx]);
    }
	return 0;
}

INT	Show_Key1_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	Show_WepKey_Proc(pAd, 0, pBuf, BufLen);
	return 0;
}

INT	Show_Key2_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	Show_WepKey_Proc(pAd, 1, pBuf, BufLen);
	return 0;
}

INT	Show_Key3_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	Show_WepKey_Proc(pAd, 2, pBuf, BufLen);
	return 0;
}

INT	Show_Key4_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	Show_WepKey_Proc(pAd, 3, pBuf, BufLen);
	return 0;
}

INT	Show_PMK_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	INT 	idx;
	UCHAR	PMK[32] = {0};

#ifdef CONFIG_AP_SUPPORT    
    POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		NdisMoveMemory(PMK, pAd->ApCfg.MBSSID[pObj->ioctl_if].PMK, 32);
#endif /* CONFIG_AP_SUPPORT */

	
    sprintf(pBuf, "\tPMK = ");
    for (idx = 0; idx < 32; idx++)
        sprintf(pBuf+strlen(pBuf), "%02X", PMK[idx]);

	return 0;
}

#ifdef WSC_STA_SUPPORT
INT	Show_WpsManufacturer_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf)
{
	sprintf(pBuf, "\tManufacturer = %s", pAd->StaCfg.WscControl.RegData.SelfInfo.Manufacturer);	
	return 0;
}

INT	Show_WpsModelName_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf)
{
	sprintf(pBuf, "\tModelName = %s", pAd->StaCfg.WscControl.RegData.SelfInfo.ModelName);
	return 0;
}

INT	Show_WpsDeviceName_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf)
{
	sprintf(pBuf, "\tDeviceName = %s", pAd->StaCfg.WscControl.RegData.SelfInfo.DeviceName);
	return 0;
}

INT	Show_WpsModelNumber_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf)
{
	sprintf(pBuf, "\tModelNumber = %s", pAd->StaCfg.WscControl.RegData.SelfInfo.ModelNumber);
	return 0;
}

INT	Show_WpsSerialNumber_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf)
{
	sprintf(pBuf, "\tSerialNumber = %s", pAd->StaCfg.WscControl.RegData.SelfInfo.SerialNumber);
	return 0;
}
#endif /* WSC_STA_SUPPORT */

#ifdef SINGLE_SKU
INT	Show_ModuleTxpower_Proc(
	IN	PRTMP_ADAPTER	pAd,
	OUT	PSTRING			pBuf,
	IN	ULONG			BufLen)
{
	snprintf(pBuf, BufLen, "\tModuleTxpower = %d", pAd->CommonCfg.ModuleTxpower);
	return 0;
}
#endif /* SINGLE_SKU */

#ifdef APCLI_SUPPORT
 INT RTMPIoctlConnStatus(
	IN	PRTMP_ADAPTER	pAd, 
 	IN	PSTRING			arg)
{
 
 	INT i=0;
 	POS_COOKIE pObj;
 	UCHAR ifIndex;
	BOOLEAN bConnect=FALSE;
 	
 	pObj = (POS_COOKIE) pAd->OS_Cookie;
 
 
 
 	DBGPRINT(RT_DEBUG_TRACE, ("==>RTMPIoctlConnStatus\n"));
 
 	if (pObj->ioctl_if_type != INT_APCLI)
 		return FALSE;
 
 	ifIndex = pObj->ioctl_if;
 	
 
 	DBGPRINT(RT_DEBUG_OFF, ("=============================================================\n"));
 	if((pAd->ApCfg.ApCliTab[ifIndex].CtrlCurrState == APCLI_CTRL_CONNECTED)
 		&& (pAd->ApCfg.ApCliTab[ifIndex].SsidLen != 0))
 	{
 		for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
 		{
 			PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
 
 			if ( IS_ENTRY_APCLI(pEntry)
				&& (pEntry->Sst == SST_ASSOC)
				&& (pEntry->PortSecured == WPA_802_1X_PORT_SECURED))
 				{
 					DBGPRINT(RT_DEBUG_OFF, ("ApCli%d Connected AP : %02X:%02X:%02X:%02X:%02X:%02X   SSID:%s\n",ifIndex,
 						pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
 						pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5],
 						pAd->ApCfg.ApCliTab[ifIndex].Ssid));
					bConnect=TRUE;
 				}
 		}

		if (!bConnect)
			DBGPRINT(RT_DEBUG_OFF, ("ApCli%d Connected AP : Disconnect\n",ifIndex));
 	}
 	else
 	{
 		DBGPRINT(RT_DEBUG_OFF, ("ApCli%d Connected AP : Disconnect\n",ifIndex));
 	}
 	DBGPRINT(RT_DEBUG_OFF, ("=============================================================\n"));
     	DBGPRINT(RT_DEBUG_TRACE, ("<==RTMPIoctlConnStatus\n"));
 	return TRUE;
}
#endif/*APCLI_SUPPORT*/

void  getRate(HTTRANSMIT_SETTING HTSetting, ULONG* fLastTxRxRate)

{
	 INT MCSMappingRateTable[] =
	{2,  4,   11,  22, /* CCK*/
	12, 18,   24,  36, 48, 72, 96, 108, /* OFDM*/
	13, 26,   39,  52,  78, 104, 117, 130, 26,  52,  78, 104, 156, 208, 234, 260, /* 20MHz, 800ns GI, MCS: 0 ~ 15*/
	39, 78,  117, 156, 234, 312, 351, 390,										  /* 20MHz, 800ns GI, MCS: 16 ~ 23*/
	27, 54,   81, 108, 162, 216, 243, 270, 54, 108, 162, 216, 324, 432, 486, 540, /* 40MHz, 800ns GI, MCS: 0 ~ 15*/
	81, 162, 243, 324, 486, 648, 729, 810,										  /* 40MHz, 800ns GI, MCS: 16 ~ 23*/
	14, 29,   43,  57,  87, 115, 130, 144, 29, 59,   87, 115, 173, 230, 260, 288, /* 20MHz, 400ns GI, MCS: 0 ~ 15*/
	43, 87,  130, 173, 260, 317, 390, 433,										  /* 20MHz, 400ns GI, MCS: 16 ~ 23*/
	30, 60,   90, 120, 180, 240, 270, 300, 60, 120, 180, 240, 360, 480, 540, 600, /* 40MHz, 400ns GI, MCS: 0 ~ 15*/
	90, 180, 270, 360, 540, 720, 810, 900};

	int rate_count = sizeof(MCSMappingRateTable)/sizeof(int);
	int rate_index = 0;  
	int value = 0;

#ifdef DOT11_N_SUPPORT
    if (HTSetting.field.MODE >= MODE_HTMIX)
    {
/*    	rate_index = 12 + ((UCHAR)ht_setting.field.BW *16) + ((UCHAR)ht_setting.field.ShortGI *32) + ((UCHAR)ht_setting.field.MCS);*/
    	rate_index = 12 + ((UCHAR)HTSetting.field.BW *24) + ((UCHAR)HTSetting.field.ShortGI *48) + ((UCHAR)HTSetting.field.MCS);
    }
    else 
#endif /* DOT11_N_SUPPORT */
    if (HTSetting.field.MODE == MODE_OFDM)                
    	rate_index = (UCHAR)(HTSetting.field.MCS) + 4;
    else if (HTSetting.field.MODE == MODE_CCK)   
    	rate_index = (UCHAR)(HTSetting.field.MCS);

    if (rate_index < 0)
        rate_index = 0;
    
    if (rate_index >= rate_count)
        rate_index = rate_count-1;

    value = (MCSMappingRateTable[rate_index] * 5)/10;
	*fLastTxRxRate=(ULONG)value;
	return;
}


#ifdef TXBF_SUPPORT

/*
	Set_ReadITxBf_Proc - Read Implicit BF profile and display it
		iwpriv ra0 set ReadITxBf=<profile number>
*/
INT	Set_ReadITxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int profileNum = simple_strtol(arg, 0, 10);
	int scIndex, i, maxCarriers;

	Read_TxBfProfile(pAd, &profData, profileNum, TRUE);

	/* Display profile. Note: each column is displayed as a row. This shortens the display */
	DBGPRINT(RT_DEBUG_OFF, ("---ITxBF Profile: %d - %dx%d, %dMHz\n",
		profileNum, profData.rows, profData.columns, profData.fortyMHz? 40: 20));

	maxCarriers = profData.fortyMHz? PROFILE_MAX_CARRIERS_40: PROFILE_MAX_CARRIERS_20;

	for (scIndex=0; scIndex<maxCarriers; scIndex++) {
		for (i=0; i<profData.rows; i++) {
			DBGPRINT(RT_DEBUG_OFF, ("%d %d    ", Unpack_IBFValue(profData.data[scIndex], 2*i+1), Unpack_IBFValue(profData.data[scIndex], 2*i)));
		}
		DBGPRINT(RT_DEBUG_OFF, ("\n"));

		if (profData.columns>1) {
			for (i=0; i<profData.rows; i++) {
				DBGPRINT(RT_DEBUG_OFF, ("%d %d    ", Unpack_IBFValue(profData.data[scIndex], 2*i+7), Unpack_IBFValue(profData.data[scIndex], 2*i+6)));
			}
			DBGPRINT(RT_DEBUG_OFF, ("\n"));
		}
	}

	return TRUE;
}


/*
	Set_ReadETxBf_Proc - Read Explicit BF profile and display it
		usage: iwpriv ra0 set ReadETxBf=<profile number>
*/
INT	Set_ReadETxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int profileNum = simple_strtol(arg, 0, 10);
	int scIndex, i, maxCarriers;

	Read_TxBfProfile(pAd, &profData, profileNum, FALSE);

	/* Dump ETxBF profile values. Note: each column is displayed as a row. This shortens the display */
	DBGPRINT(RT_DEBUG_OFF, ("---ETxBF Profile: %d - %dx%d, %dMHz, grp=%d\n",
		profileNum, profData.rows, profData.columns, profData.fortyMHz? 40: 20, profData.grouping));

	maxCarriers = profData.fortyMHz? PROFILE_MAX_CARRIERS_40: PROFILE_MAX_CARRIERS_20;

	for (scIndex=0; scIndex<maxCarriers; scIndex++) {
		for (i=0; i<profData.rows; i++) {
			DBGPRINT(RT_DEBUG_OFF, ("%d %d\t", (CHAR)(profData.data[scIndex][6*i]), (CHAR)(profData.data[scIndex][6*i+1]) ));
		}
		DBGPRINT(RT_DEBUG_OFF, ("\n"));

		if (profData.columns>1) {
			for (i=0; i<profData.rows; i++) {
				DBGPRINT(RT_DEBUG_OFF, ("%d %d    ", (CHAR)(profData.data[scIndex][6*i+2]), (CHAR)(profData.data[scIndex][6*i+3]) ));
			}
			DBGPRINT(RT_DEBUG_OFF, ("\n"));
		}

		if (profData.columns>2) {
			for (i=0; i<profData.rows; i++) {
				DBGPRINT(RT_DEBUG_OFF, ("%d %d    ", (CHAR)(profData.data[scIndex][6*i+4]), (CHAR)(profData.data[scIndex][6*i+5]) ));
			}
			DBGPRINT(RT_DEBUG_OFF, ("\n"));
		}
	}

	return TRUE;
}


/*
	Set_WriteITxBf_Proc - Write Implicit BF matrix
		usage: iwpriv ra0 set WriteITxBf=<profile number>
		Assumes profData contains a valid Implicit Profile
*/
INT	Set_WriteITxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int profileNum = simple_strtol(arg, 0, 10);

	if (!profData.impProfile)
		return FALSE;

	Write_TxBfProfile(pAd, &profData, profileNum);

	return TRUE;
}


/*
	Set_WriteETxBf_Proc - Write Explicit BF matrix
		usage: iwpriv ra0 set WriteETxBf=<profile number>
		Assumes profData contains a valid Explicit Profile
*/
INT	Set_WriteETxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int profileNum = simple_strtol(arg, 0, 10);

	if (profData.impProfile)
		return FALSE;

	Write_TxBfProfile(pAd, &profData, profileNum);

	return TRUE;
}


/*
	Set_StatITxBf_Proc - Compute power of each chain in Implicit BF matrix
		usage: iwpriv ra0 set StatITxBf=<profile number>
*/
INT	Set_StatITxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int scIndex, maxCarriers, i;
	unsigned long col1Power[3] = {0,0,0}, col2Power[3] = {0,0,0};
	int profileNum = simple_strtol(arg, 0, 10);
	PROFILE_DATA *pProfData;

	pProfData = (PROFILE_DATA *)kmalloc(sizeof(PROFILE_DATA), MEM_ALLOC_FLAG);
	if (pProfData == NULL)
	{
		DBGPRINT(RT_DEBUG_OFF, ("Set_StatITxBf_Proc: kmalloc failed\n"));
		return FALSE;
	}

	Read_TxBfProfile(pAd, pProfData, profileNum, TRUE);

	maxCarriers = pProfData->fortyMHz? PROFILE_MAX_CARRIERS_40: PROFILE_MAX_CARRIERS_20;

	for (scIndex=0; scIndex<maxCarriers; scIndex++) {
		for (i=0; i<pProfData->rows; i++) {
			int ival = Unpack_IBFValue(pProfData->data[scIndex], 2*i+1);
			int qval = Unpack_IBFValue(pProfData->data[scIndex], 2*i);
			col1Power[i] += ival*ival+qval*qval;

			if (pProfData->columns==2) {
				ival = Unpack_IBFValue(pProfData->data[scIndex], 2*i+7);
				qval = Unpack_IBFValue(pProfData->data[scIndex], 2*i+6);
				col2Power[i] += ival*ival+qval*qval;
			}
		}
	}

	/* Remove implied scale factor of 2^-16. Convert to thousandths */
	for (i=0; i<pProfData->rows; i++) {
		col1Power[i] >>= 12;
		col1Power[i] = ((col1Power[i]*1000)/maxCarriers)>>4;
		col2Power[i] >>= 12;
		col2Power[i] = ((col2Power[i]*1000)/maxCarriers)>>4;
	}

	/* Display stats */
	DBGPRINT(RT_DEBUG_OFF, ("ITxBF Stats:\n  %dx1=[0.%03lu 0.%03lu, 0.%03lu]\n",
				pProfData->rows, col1Power[0], col1Power[1], col1Power[2]));
	if (pProfData->columns==2) {
		DBGPRINT(RT_DEBUG_OFF, ("  %dx2=[0.%03lu 0.%03lu, 0.%03lu]\n",
					pProfData->rows, (col1Power[0]+col2Power[0])/2, (col1Power[1]+col2Power[1])/2,
					(col1Power[2]+col2Power[2])/2) );
	}

	kfree(pProfData);

	return TRUE;
}

/*
	Set_StatETxBf_Proc - Compute power of each chain in Explicit BF matrix
		usage: iwpriv ra0 set StatETxBf=<profile number>
*/
INT	Set_StatETxBf_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	int scIndex, maxCarriers, i;
	unsigned long col1Power[3] = {0,0,0}, col2Power[3] = {0,0,0}, col3Power[3] = {0,0,0};
	int profileNum = simple_strtol(arg, 0, 10);
	PROFILE_DATA *pProfData;

	pProfData = (PROFILE_DATA *)kmalloc(sizeof(PROFILE_DATA), MEM_ALLOC_FLAG);
	if (pProfData == NULL)
	{
		DBGPRINT(RT_DEBUG_OFF, ("Set_StatETxBf_Proc: kmalloc failed\n"));
		return FALSE;
	}

	Read_TxBfProfile(pAd, pProfData, profileNum, FALSE);

	maxCarriers = pProfData->fortyMHz? PROFILE_MAX_CARRIERS_40: PROFILE_MAX_CARRIERS_20;

	for (scIndex=0; scIndex<maxCarriers; scIndex++) {
		for (i=0; i<pProfData->rows; i++) {
			int ival = (CHAR)(pProfData->data[scIndex][6*i]);
			int qval = (CHAR)(pProfData->data[scIndex][6*i+1]);
			col1Power[i] += ival*ival+qval*qval;

			if (pProfData->columns>1) {
				ival = (CHAR)(pProfData->data[scIndex][6*i+2]);
				qval = (CHAR)(pProfData->data[scIndex][6*i+3]);
				col2Power[i] += ival*ival+qval*qval;
			}

			if (pProfData->columns>2) {
				ival = (CHAR)(pProfData->data[scIndex][6*i+4]);
				qval = (CHAR)(pProfData->data[scIndex][6*i+5]);
				col3Power[i] += ival*ival+qval*qval;
			}
		}
	}

	/* Remove implied scale factor of 2^-14. Convert to thousandths */
	for (i=0; i<pProfData->rows; i++) {
		col1Power[i] >>= 10;
		col1Power[i] = ((col1Power[i]*1000)/maxCarriers)>>4;
		col2Power[i] >>= 10;
		col2Power[i] = ((col2Power[i]*1000)/maxCarriers)>>4;
		col3Power[i] >>= 10;
		col3Power[i] = ((col3Power[i]*1000)/maxCarriers)>>4;
	}

	/* Display stats */
	DBGPRINT(RT_DEBUG_OFF, ("ETxBF Stats:\n  %dx1=[0.%03lu 0.%03lu, 0.%03lu]\n",
				pProfData->rows, col1Power[0], col1Power[1], col1Power[2]));
	if (pProfData->columns==2) {
		DBGPRINT(RT_DEBUG_OFF, ("  %dx2=[0.%03lu 0.%03lu, 0.%03lu]\n",
					pProfData->rows, (col1Power[0]+col2Power[0])/2,
					(col1Power[1]+col2Power[1])/2, (col1Power[2]+col2Power[2])/2) );
		}
	if (pProfData->columns==3) {
		DBGPRINT(RT_DEBUG_OFF, ("  %dx3=[0.%03lu 0.%03lu, 0.%03lu]\n",
					pProfData->rows, (col1Power[0]+col2Power[0]+col3Power[0])/3,
					(col1Power[1]+col2Power[1]+col3Power[1])/3,
					(col1Power[2]+col2Power[2]+col3Power[2])/3) );
	}

	kfree(pProfData);

	return TRUE;
}


/*
	Set_TxBfTag_Proc - Display BF Profile Tags
		usage: "iwpriv ra0 set TxBfTag=n"
					n: 0=>all,
					   1=>Explicit,
					   2=>Implicit,
					   3=>dump Power table
*/
INT	Set_TxBfTag_Proc(
	IN	PRTMP_ADAPTER	pAd, 
    IN  PSTRING         arg)
{
	int argVal = simple_strtol(arg, 0, 10);
	int profileNum;

	if (argVal==0 || argVal==1) {
		/* Display Explicit tagfield */
		DBGPRINT(RT_DEBUG_OFF, ("---Explicit TxBfTag:\n"));
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R179, 4);
		for (profileNum=0; profileNum<4; profileNum++)
			displayTagfield(pAd, profileNum, FALSE);
	}

	if (argVal==0 || argVal==2) {
		/* Display Implicit tagfield */
		DBGPRINT(RT_DEBUG_OFF, ("---Implicit TxBfTag:\n"));
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R179, 0);
		for (profileNum=0; profileNum<4; profileNum++)
			displayTagfield(pAd, profileNum, TRUE);
	}
	
	if (argVal==3) {
		int i;
		/* 4. Dump power table */
		for (i = 0; i < (14 + 12 + 16 + 7); i++)
			DBGPRINT(RT_DEBUG_OFF, ("%d: Ch%2d=[%d, %d %d]\n", i,
						pAd->TxPower[i].Channel, pAd->TxPower[i].Power,
						pAd->TxPower[i].Power2, pAd->TxPower[i].Power3));
	}

	return TRUE;
}


/*
	Set_InvTxBfTag_Proc - Invalidate BF Profile Tags
		usage: "iwpriv ra0 set InvTxBfTag=n"
		Reset Valid bit and zero out MAC address of each profile. The next profile will be stored in profile 0
*/
INT	Set_InvTxBfTag_Proc(
	IN	PRTMP_ADAPTER	pAd, 
    IN  PSTRING         arg)
{
	int profileNum;
	UCHAR row[EXP_MAX_BYTES];
	UCHAR r163Value = 0;

	/* Disable Profile Updates during access */
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R163, &r163Value);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R163, r163Value & ~0x88);

	/* Invalidate Implicit tags */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R179, 0);
	for (profileNum=0; profileNum<4; profileNum++) {
		Read_TagField(pAd, row, profileNum);
		row[0] &= 0x7F;
		row[1] = row[2] = row[3] = row[4] = row[5] = row[6] = 0xAA;
		Write_TagField(pAd, row, profileNum);
	}

	/* Invalidate Explicit tags */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R179, 4);
	for (profileNum=0; profileNum<4; profileNum++) {
		Read_TagField(pAd, row, profileNum);
		row[0] &= 0x7F;
		row[1] = row[2] = row[3] = row[4] = row[5] = row[6] = 0x55;
		Write_TagField(pAd, row, profileNum);
	}

	/* Restore Profile Updates */
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R163, r163Value);
	
	return TRUE;
}


/*
	Set_ITxBfTimeout_Proc - Set ITxBF timeout value
	usage: iwpriv ra0 set ITxBfTimeout=<decimal timeout in units of 25 microsecs>
*/
INT Set_ITxBfTimeout_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 65535) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ITxBfTimeout_Proc: value > 65535!\n"));
		return FALSE;
	}

    pAd->CommonCfg.ITxBfTimeout = t;
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R179, 0x02);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R180, 0);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R182, pAd->CommonCfg.ITxBfTimeout & 0xFF);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R180, 1);
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R182, (pAd->CommonCfg.ITxBfTimeout>>8) & 0xFF);
	
    DBGPRINT(RT_DEBUG_TRACE, ("Set_ITxBfTimeout_Proc::(ITxBfTimeout=%d)\n", (int)pAd->CommonCfg.ITxBfTimeout));
	return TRUE;
}


/*
	Set_ETxBfTimeout_Proc - Set ITxBF timeout value
		usage: iwpriv ra0 set ETxBfTimeout=<decimal timeout in units of 25 microsecs>
*/
INT Set_ETxBfTimeout_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 65535) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ETxBfTimeout_Proc: value > 65535!\n"));
		return FALSE;
	}

    pAd->CommonCfg.ETxBfTimeout = t;
	RTMP_IO_WRITE32(pAd, TX_TXBF_CFG_3, pAd->CommonCfg.ETxBfTimeout);
    DBGPRINT(RT_DEBUG_TRACE, ("Set_ETxBfTimeout_Proc::(ETxBfTimeout=%d)\n", (int)pAd->CommonCfg.ETxBfTimeout));
	return TRUE;
}


/*
	Set_ETxBfCodebook_Proc - Set ETxBf Codebook 
	usage: iwpriv ra0 set ETxBfCodebook=0 to 3
*/
INT Set_ETxBfCodebook_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	TX_TXBF_CFG_0_STRUC regValue;
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 3) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ETxBfCodebook_Proc: value > 3!\n"));
		return FALSE;
	}

	RTMP_IO_READ32(pAd, TX_TXBF_CFG_0, &regValue.word);
	regValue.field.EtxbfFbkCode = t;
	RTMP_IO_WRITE32(pAd, TX_TXBF_CFG_0, regValue.word);
	return TRUE;
}


/*
	Set_ETxBfCoefficient_Proc - Set ETxBf Coefficient 
		usage: iwpriv ra0 set ETxBfCoefficient=0 to 3
*/
INT Set_ETxBfCoefficient_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	TX_TXBF_CFG_0_STRUC regValue;
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 3) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ETxBfCoefficient_Proc: value > 3!\n"));
		return FALSE;
	}

	RTMP_IO_READ32(pAd, TX_TXBF_CFG_0, &regValue.word);
	regValue.field.EtxbfFbkCoef = t;
	RTMP_IO_WRITE32(pAd, TX_TXBF_CFG_0, regValue.word);
	return TRUE;
}


/*
	Set_ETxBfGrouping_Proc - Set ETxBf Grouping 
		usage: iwpriv ra0 set ETxBfGrouping=0 to 2
*/
INT Set_ETxBfGrouping_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	TX_TXBF_CFG_0_STRUC regValue;
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 2) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ETxBfGrouping_Proc: value > 2!\n"));
		return FALSE;
	}

	RTMP_IO_READ32(pAd, TX_TXBF_CFG_0, &regValue.word);
	regValue.field.EtxbfFbkNg = t;
	RTMP_IO_WRITE32(pAd, TX_TXBF_CFG_0, regValue.word);
	return TRUE;
}


/*
	Set_ETxBfNoncompress_Proc - Set ETxBf Noncompress option 
		usage: iwpriv ra0 set ETxBfNoncompress=0 or 1
*/
INT Set_ETxBfNoncompress_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 1) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set_ETxBfNoncompress_Proc: value > 1!\n"));
		return FALSE;
	}

	pAd->CommonCfg.ETxBfNoncompress = t;
	return TRUE;
}


/*
	Set_ETxBfIncapable_Proc - Set ETxBf Incapable option
		usage: iwpriv ra0 set ETxBfIncapable=0 or 1
*/
INT Set_ETxBfIncapable_Proc(
    IN  PRTMP_ADAPTER   pAd,
    IN  PSTRING         arg)
{
	ULONG t = simple_strtol(arg, 0, 10);

	if (t > 1)
		return FALSE;

	pAd->CommonCfg.ETxBfIncapable = t;
	setETxBFCap(pAd, &pAd->CommonCfg.HtCapability.TxBFCap);

	return TRUE;
}


/*
	Set_ITxBfDivCal_Proc - Calculate ITxBf Divider Calibration parameters
	usage: iwpriv ra0 set ITxBfDivCal=dd
			0=>display calibration parameters
			1=>update EEPROM values
			2=>update BBP R176
			10=>display calibration parameters and dump capture data
			11=>Skip divider calibration, just capture and dump capture data
*/
INT	Set_ITxBfDivCal_Proc(
	IN	PRTMP_ADAPTER	pAd, 
    IN  PSTRING         arg)
{
	int calFunction;

	calFunction = simple_strtol(arg, 0, 10);

	return ITxBFDividerCalibration(pAd, calFunction, 0, NULL);
}


/*
	Set_ITxBfLNACal_Proc - Calculate ITxBf LNA Calibration parameters
	usage: iwpriv ra0 set ITxBfLnaCal=dd
			0=>display calibration parameters
			1=>update EEPROM values
			2=>update BBP R174
			10=>display calibration parameters and dump capture data
*/
INT	Set_ITxBfLnaCal_Proc(
	IN	PRTMP_ADAPTER	pAd,
    IN  PSTRING         arg)
{
	UCHAR channel = pAd->CommonCfg.Channel;
	int calFunction;

#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		channel = pAd->ate.Channel;
#endif /* RALINK_ATE */

	calFunction = simple_strtol(arg, 0, 10);

	return ITxBFLNACalibration(pAd, calFunction, 0, channel<=14);
}


/*
	Set_ITxBfCal_Proc - Calculate ITxBf Calibration parameters
	usage: "iwpriv ra0 set ITxBfCal=[0 | 1]"
			0=>calculate values, 1=>update BBP and EEPROM
*/
INT	Set_ITxBfCal_Proc(
	IN	PRTMP_ADAPTER	pAd, 
    IN  PSTRING         arg)
{
	int calFunction = simple_strtol(arg, 0, 10);
	int calParams[2];
	int ret;
	UCHAR channel = pAd->CommonCfg.Channel;

#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		channel = pAd->ate.Channel;
#endif /* RALINK_ATE */

	ret = iCalcCalibration(pAd, calParams, 0);
	if (ret < 0) {
		if (ret == -3)
			DBGPRINT(RT_DEBUG_OFF, ("Set_ITxBfCal_Proc: kmalloc failed\n"));
		else if (ret == -2)
		DBGPRINT(RT_DEBUG_OFF, ("Set_ITxBfCal_Proc: MAC Address mismatch\n"));
		else
		DBGPRINT(RT_DEBUG_OFF, ("Set_ITxBfCal_Proc: Invalid profiles\n"));
		return FALSE;
	}

	/* Display result */
	DBGPRINT((calFunction==0? RT_DEBUG_OFF: RT_DEBUG_WARN),
				("ITxBfCal Result = [0x%02x 0x%02x]\n", calParams[0], calParams[1]));

#ifdef RALINK_ATE
	pAd->ate.calParams[0] = (UCHAR)calParams[0];
	pAd->ate.calParams[1] = (UCHAR)calParams[1];

	/* Double check */
	DBGPRINT((calFunction==0? RT_DEBUG_OFF: RT_DEBUG_WARN),
				("ITxBfCal Result in ATE = [0x%02x 0x%02x]\n", pAd->ate.calParams[0], pAd->ate.calParams[1]));
#endif /* RALINK_ATE */

	/* Update BBP R176 and EEPROM for Ant 0 and 2 */
	if (calFunction == 1) {
		UCHAR r27Value = 0, r173Value = 0;
		ITXBF_PHASE_PARAMS phaseParams;
		UCHAR divPhase[2] = {0}, phaseValues[2] = {0};

		/* Read R173 to see if Phase compensation is already enabled */
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R173, &r173Value);

		/* Select Ant 0 */
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &r27Value);
		r27Value &= ~0x60;
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, r27Value);

		/* Update R176 */
		if (r173Value & 0x08) {
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R176, &phaseValues[0]);
			phaseValues[0] += calParams[0];
		}
		else
			phaseValues[0] = calParams[0];
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R176, phaseValues[0]);

		/* Select Ant 2 */
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, r27Value | 0x40);

		/* Update R176 */
		if (r173Value & 0x08) {
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R176, &phaseValues[1]);
			phaseValues[1] += calParams[1];
		}
		else
			phaseValues[1] = calParams[1];
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R176, phaseValues[1]);

		/* Enable TX Phase Compensation */
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R173, r173Value | 0x08);

		/* Remove Divider phase */
		ITxBFDividerCalibration(pAd, 3, 0, divPhase);
		phaseValues[0] -= divPhase[0];
		phaseValues[1] -= divPhase[1];

		/* Update EEPROM */

		ITxBFGetEEPROM(pAd, &phaseParams, 0, 0);

		/* Only allow calibration on specific channels */
		if (channel == 1) {
			phaseParams.gBeg[0] = phaseValues[0];
			phaseParams.gBeg[1] = phaseValues[1];
		}
		else if (channel == 14) {
			phaseParams.gEnd[0] = phaseValues[0];
			phaseParams.gEnd[1] = phaseValues[1];
		}
		else if (channel == 36) {
			phaseParams.aLowBeg[0] = phaseValues[0];
			phaseParams.aLowBeg[1] = phaseValues[1];
		}
		else if (channel == 64) {
			phaseParams.aLowEnd[0] = phaseValues[0];
			phaseParams.aLowEnd[1] = phaseValues[1];
		}
		else if (channel == 100) {
			phaseParams.aMidBeg[0] = phaseValues[0];
			phaseParams.aMidBeg[1] = phaseValues[1];
		}
		else if (channel == 128) {
			phaseParams.aMidEnd[0] = phaseValues[0];
			phaseParams.aMidEnd[1] = phaseValues[1];
		}
		else if (channel == 132) {
			phaseParams.aHighBeg[0] = phaseValues[0];
			phaseParams.aHighBeg[1] = phaseValues[1];
		}
		else if (channel == 165) {
			phaseParams.aHighEnd[0] = phaseValues[0];
			phaseParams.aHighEnd[1] = phaseValues[1];
		}
		else {
			DBGPRINT(RT_DEBUG_OFF,
					("Invalid channel: %d\nMust calibrate channel 1, 14, 36, 64, 100, 128, 132 or 165", channel) );
			return FALSE;
		}
		ITxBFSetEEPROM(pAd, &phaseParams, 0, 0);

		DBGPRINT(RT_DEBUG_WARN, ("Set_ITxBfCal_Proc: Calibration Parameters updated\n"));
	}

	return TRUE;
}


/* 
	Set_ETxBfEnCond_Proc - enable/disable ETxBF
	usage: iwpriv ra0 set ETxBfEnCond=dd
		0=>disable, 1=>enable
	Note: After use this command, need to re-run apStartup()/LinkUp() operations to sync all status.
		  If ETxBfIncapable!=0 then we don't need to reassociate.
*/
INT	Set_ETxBfEnCond_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR i, enableETxBf;
	MAC_TABLE_ENTRY		*pEntry;
	UINT8	byteValue;

	enableETxBf = simple_strtol(arg, 0, 10);

	if (enableETxBf > 1)
		return FALSE;

	pAd->CommonCfg.ETxBfEnCond = enableETxBf && (pAd->Antenna.field.TxPath > 1);
	pAd->CommonCfg.RegTransmitSetting.field.TxBF = enableETxBf==0? 0: 1;

	setETxBFCap(pAd, &pAd->CommonCfg.HtCapability.TxBFCap);
	rtmp_asic_set_bf(pAd);

	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		pEntry = &pAd->MacTab.Content[i];
		if (!IS_ENTRY_NONE(pEntry))
		{
			pEntry->eTxBfEnCond = clientSupportsETxBF(pAd, &pEntry->HTCapability.TxBFCap)? enableETxBf: 0;
			pEntry->bfState = READY_FOR_SNDG0;
		}
	}


	if (pAd->CommonCfg.RegTransmitSetting.field.ITxBfEn || enableETxBf)
	{
		RT30xxReadRFRegister(pAd, RF_R39, (PUCHAR)&byteValue);
		byteValue |= 0x40;
		RT30xxWriteRFRegister(pAd, RF_R39, (UCHAR)byteValue);

		RT30xxReadRFRegister(pAd, RF_R49, (PUCHAR)&byteValue);
		byteValue |= 0x20;
		RT30xxWriteRFRegister(pAd, RF_R49, (UCHAR)byteValue);
	}
	else
	{
		/* depends on Gary Tsao's comments. we shall disable it */
		if (pAd->CommonCfg.RegTransmitSetting.field.ITxBfEn == 0)
		{
			RT30xxReadRFRegister(pAd, RF_R39, (PUCHAR)&byteValue);
			byteValue &= (~0x40);
			RT30xxWriteRFRegister(pAd, RF_R39, (UCHAR)byteValue);

			RT30xxReadRFRegister(pAd, RF_R49, (PUCHAR)&byteValue);
			byteValue &= (~0x20);
			RT30xxWriteRFRegister(pAd, RF_R49, (UCHAR)byteValue);
		}
	}


	return TRUE;	
}

INT	Set_NoSndgCntThrd_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR i;
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++){
		pAd->MacTab.Content[i].noSndgCntThrd = simple_strtol(arg, 0, 10);
	}
	return TRUE;	
}

INT	Set_NdpSndgStreams_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR i;
	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++){
		pAd->MacTab.Content[i].ndpSndgStreams = simple_strtol(arg, 0, 10);
	}
	return TRUE;	
}


INT	Set_Trigger_Sounding_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR			macAddr[MAC_ADDR_LEN];
	CHAR			*value;
	INT				i;
	MAC_TABLE_ENTRY *pEntry = NULL;

	/* Mac address acceptable format 01:02:03:04:05:06 length 17 */
	if(strlen(arg) != 17)
		return FALSE;

	for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":")) 
	{
		if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
			return FALSE;  /*Invalid*/

		AtoH(value, &macAddr[i++], 1);
	}

	/*DBGPRINT(RT_DEBUG_TRACE, ("TriggerSounding=%02x:%02x:%02x:%02x:%02x:%02x\n",*/
	/*		macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5], macAddr[6]) );*/
	pEntry = MacTableLookup(pAd, macAddr);
	if (pEntry==NULL)
		return FALSE;

	Trigger_Sounding_Packet(pAd, SNDG_TYPE_SOUNDING, 0, pEntry->sndgMcs, pEntry);

	return TRUE;
}

/*
	Set_ITxBfEn_Proc - enable/disable ITxBF
	usage: iwpriv ra0 set ITxBfEn=dd
	0=>disable, 1=>enable
*/
INT	Set_ITxBfEn_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UCHAR i;
	UCHAR enableITxBF;
	BOOLEAN bCalibrated;
	UINT8 byteValue;

	enableITxBF = simple_strtol(arg, 0, 10);

	if (enableITxBF > 1)
		return FALSE;

	bCalibrated = rtmp_chk_itxbf_calibration(pAd);
	DBGPRINT(RT_DEBUG_TRACE, ("Set ITxBfEn=%d, calibration of ITxBF=%d, so enableITxBF=%d!\n", 
					enableITxBF , bCalibrated, (enableITxBF & bCalibrated)));
	
	enableITxBF &= bCalibrated;
	
	pAd->CommonCfg.RegTransmitSetting.field.ITxBfEn = enableITxBF && (pAd->Antenna.field.TxPath > 1);

	rtmp_asic_set_bf(pAd);

	for (i=0; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		MAC_TABLE_ENTRY *pMacEntry = &pAd->MacTab.Content[i];
		if ((!IS_ENTRY_NONE(pMacEntry)) && (pAd->Antenna.field.TxPath> 1))
			pMacEntry->iTxBfEn = enableITxBF;
	}

	if (enableITxBF || pAd->CommonCfg.ETxBfEnCond)
	{
		RT30xxReadRFRegister(pAd, RF_R39, (PUCHAR)&byteValue);
		byteValue |= 0x40;
		RT30xxWriteRFRegister(pAd, RF_R39, (UCHAR)byteValue);

		RT30xxReadRFRegister(pAd, RF_R49, (PUCHAR)&byteValue);
		byteValue |= 0x20;
		RT30xxWriteRFRegister(pAd, RF_R49, (UCHAR)byteValue);
	}
	
	/* If enabling ITxBF then set LNA compensation, do a Divider Calibration and update BBP registers */
	if (enableITxBF) {
		ITxBFLoadLNAComp(pAd);
		ITxBFDividerCalibration(pAd, 2, 0, NULL);
	}
	else
	{
		/* depends on Gary Tsao's comments. */
		if (pAd->CommonCfg.ETxBfEnCond == 0)
		{
		RT30xxReadRFRegister(pAd, RF_R39, (PUCHAR)&byteValue);
			byteValue &= (~0x40);
		RT30xxWriteRFRegister(pAd, RF_R39, (UCHAR)byteValue);

		RT30xxReadRFRegister(pAd, RF_R49, (PUCHAR)&byteValue);
			byteValue &= (~0x20);
		RT30xxWriteRFRegister(pAd, RF_R49, (UCHAR)byteValue);
	}
	
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R173, 0);

		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R27, &byteValue);
		byteValue &= ~0x60;
		for ( i = 0; i < 3; i++)
		{	
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R27, (byteValue & (i << 5)));
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R174, 0);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R176, 0);
		}
	}
	return TRUE;	
}
#endif /* TXBF_SUPPORT */


#ifdef DOT11_N_SUPPORT
void assoc_ht_info_debugshow(
	IN PRTMP_ADAPTER pAd,
	IN MAC_TABLE_ENTRY *pEntry,
	IN UCHAR HTCapability_Len,
	IN HT_CAPABILITY_IE *pHTCapability)
{
	HT_CAP_INFO			*pHTCap;
	HT_CAP_PARM		*pHTCapParm;
	EXT_HT_CAP_INFO		*pExtHT;
#ifdef TXBF_SUPPORT
	HT_BF_CAP			*pBFCap;
#endif /* TXBF_SUPPORT */


	if (pHTCapability && (HTCapability_Len > 0))
	{
		pHTCap = &pHTCapability->HtCapInfo;
		pHTCapParm = &pHTCapability->HtCapParm;
		pExtHT = &pHTCapability->ExtHtCapInfo;
#ifdef TXBF_SUPPORT
		pBFCap = &pHTCapability->TxBFCap;
#endif /* TXBF_SUPPORT */

		DBGPRINT(RT_DEBUG_TRACE, ("Peer - 11n HT Info\n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\tHT Cap Info: \n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t AdvCode(%d), BW(%d), MIMOPS(%d), GF(%d), ShortGI_20(%d), ShortGI_40(%d)\n",
			pHTCap->AdvCoding, pHTCap->ChannelWidth, pHTCap->MimoPs, pHTCap->GF,
			pHTCap->ShortGIfor20, pHTCap->ShortGIfor40));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t TxSTBC(%d), RxSTBC(%d), DelayedBA(%d), A-MSDU(%d), CCK_40(%d)\n",
			pHTCap->TxSTBC, pHTCap->RxSTBC, pHTCap->DelayedBA, pHTCap->AMsduSize, pHTCap->CCKmodein40));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t PSMP(%d), Forty_Mhz_Intolerant(%d), L-SIG(%d)\n",
			pHTCap->PSMP, pHTCap->Forty_Mhz_Intolerant, pHTCap->LSIGTxopProSup));

		DBGPRINT(RT_DEBUG_TRACE, ("\tHT Parm Info: \n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t MaxRx A-MPDU Factor(%d), MPDU Density(%d)\n",
			pHTCapParm->MaxRAmpduFactor, pHTCapParm->MpduDensity));

		DBGPRINT(RT_DEBUG_TRACE, ("\tHT MCS set: \n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t RxMCS(%02x %02x %02x %02x %02x) MaxRxMbps(%d) TxMCSSetDef(%02x)\n",
			pHTCapability->MCSSet[0], pHTCapability->MCSSet[1], pHTCapability->MCSSet[2],
			pHTCapability->MCSSet[3], pHTCapability->MCSSet[4],
			(pHTCapability->MCSSet[11]<<8) + pHTCapability->MCSSet[10],
			pHTCapability->MCSSet[12]));

		DBGPRINT(RT_DEBUG_TRACE, ("\tExt HT Cap Info: \n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t PCO(%d), TransTime(%d), MCSFeedback(%d), +HTC(%d), RDG(%d)\n",
			pExtHT->Pco, pExtHT->TranTime, pExtHT->MCSFeedback, pExtHT->PlusHTC, pExtHT->RDGSupport));

#ifdef TXBF_SUPPORT
        	DBGPRINT(RT_DEBUG_TRACE, ("\tTX BF Cap: \n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t ImpRxCap(%d), RXStagSnd(%d), TXStagSnd(%d), RxNDP(%d), TxNDP(%d) ImpTxCap(%d)\n",
			pBFCap->TxBFRecCapable, pBFCap->RxSoundCapable, pBFCap->TxSoundCapable,
			pBFCap->RxNDPCapable, pBFCap->TxNDPCapable, pBFCap->ImpTxBFCapable));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t Calibration(%d), ExpCSICapable(%d), ExpComSteerCapable(%d), ExpCSIFbk(%d), ExpNoComBF(%d) ExpComBF(%d)\n",
			pBFCap->Calibration, pBFCap->ExpCSICapable, pBFCap->ExpComSteerCapable,
			pBFCap->ExpCSIFbk, pBFCap->ExpNoComBF, pBFCap->ExpComBF));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\t MinGrouping(%d), CSIBFAntSup(%d), NoComSteerBFAntSup(%d), ComSteerBFAntSup(%d), CSIRowBFSup(%d) ChanEstimation(%d)\n",
			pBFCap->MinGrouping, pBFCap->CSIBFAntSup, pBFCap->NoComSteerBFAntSup,
			pBFCap->ComSteerBFAntSup, pBFCap->CSIRowBFSup, pBFCap->ChanEstimation));
#endif /* TXBF_SUPPORT */

		DBGPRINT(RT_DEBUG_TRACE, ("\nPeer - MODE=%d, BW=%d, MCS=%d, ShortGI=%d, MaxRxFactor=%d, MpduDensity=%d, MIMOPS=%d, AMSDU=%d\n",
			pEntry->HTPhyMode.field.MODE, pEntry->HTPhyMode.field.BW,
			pEntry->HTPhyMode.field.MCS, pEntry->HTPhyMode.field.ShortGI,
			pEntry->MaxRAmpduFactor, pEntry->MpduDensity,
			pEntry->MmpsMode, pEntry->AMsduSize));

#ifdef DOT11N_DRAFT3
		DBGPRINT(RT_DEBUG_TRACE, ("\tExt Cap Info: \n"));
		DBGPRINT(RT_DEBUG_TRACE, ("\t\tBss2040CoexistMgmt=%d\n", pEntry->BSS2040CoexistenceMgmtSupport));
#endif /* DOT11N_DRAFT3 */
	}
}


INT	Set_BurstMode_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	ULONG Value;

	Value = simple_strtol(arg, 0, 10);

	if (Value == 1)
	{
		pAd->CommonCfg.bRalinkBurstMode= TRUE;
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RALINK_BURST_MODE);
		AsicEnableRalinkBurstMode(pAd);
	}
	else
	{
		pAd->CommonCfg.bRalinkBurstMode = FALSE;
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RALINK_BURST_MODE);
		AsicDisableRalinkBurstMode(pAd);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Set_BurstMode_Proc ::%s\n", 
				(pAd->CommonCfg.bRalinkBurstMode == TRUE) ? "enabled" : "disabled"));

	return TRUE;
}
#endif /* DOT11_N_SUPPORT */


INT Set_RateAdaptInterval(
	IN RTMP_ADAPTER *pAd,
	IN PSTRING arg)
{
	UINT32 ra_time,  ra_qtime;
	PSTRING token;
	char sep = ':';
	ULONG irqFlags;
	
/*
	The ra_interval inupt string format should be d:d, in units of ms. 
		=>The first decimal number indicates the rate adaptation checking period, 
		=>The second decimal number indicates the rate adaptation quick response checking period.
*/
	DBGPRINT(RT_DEBUG_TRACE,("%s():%s\n", __FUNCTION__, arg));
	
	token = strchr(arg, sep);
	if (token != NULL)
	{
		*token = '\0';

		if (strlen(arg) && strlen(token+1))
		{
			ra_time = simple_strtol(arg, 0, 10);
			ra_qtime = simple_strtol(token+1, 0, 10);
			DBGPRINT(RT_DEBUG_OFF, ("%s():Set RateAdaptation TimeInterval as(%d:%d) ms\n", 
						__FUNCTION__, ra_time, ra_qtime));

			RTMP_IRQ_LOCK(&pAd->irq_lock, irqFlags);
			pAd->ra_interval = ra_time;
			pAd->ra_fast_interval = ra_qtime;
#ifdef CONFIG_AP_SUPPORT
			if (pAd->ApCfg.ApQuickResponeForRateUpTimerRunning == TRUE)
			{
				BOOLEAN Cancelled;
				
				RTMPCancelTimer(&pAd->ApCfg.ApQuickResponeForRateUpTimer, &Cancelled);	
				pAd->ApCfg.ApQuickResponeForRateUpTimerRunning = FALSE;
			}
#endif /* CONFIG_AP_SUPPORT  */
			RTMP_IRQ_UNLOCK(&pAd->irq_lock, irqFlags);
			return TRUE;
		}
	}
	
	return FALSE;

}


INT Set_VcoPeriod_Proc(
	IN RTMP_ADAPTER		*pAd,
	IN PSTRING			arg)
{
	pAd->chipCap.VcoPeriod = simple_strtol(arg, 0, 10);

	DBGPRINT(RT_DEBUG_TRACE,
			("VCO Period = %d seconds\n", pAd->chipCap.VcoPeriod));
	return TRUE;
}

#ifdef SINGLE_SKU
INT Set_ModuleTxpower_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	UINT16 Value;

	Value = (UINT16)simple_strtol(arg, 0, 10);
	pAd->CommonCfg.ModuleTxpower = Value;
	DBGPRINT(RT_DEBUG_TRACE, ("IF Set_ModuleTxpower_Proc::(ModuleTxpower=%d)\n", pAd->CommonCfg.ModuleTxpower));
	return TRUE;
}

#endif /* SINGLE_SKU */

