/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	chlist.c

	Abstract:

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Fonchi Wu   2007-12-19    created
*/

#ifndef __CHLIST_H__
#define __CHLIST_H__

#include "rtmp_type.h"
#include "rtmp_def.h"

#define BAND_5G		1
#define BAND_24G	2
#define BAND_BOTH	(BAND_5G | BAND_24G)

typedef struct _CH_DESC {
	UCHAR FirstChannel;
	UCHAR NumOfCh;
	UCHAR ChannelProp;
}CH_DESC, *PCH_DESC;

typedef struct _COUNTRY_REGION_CH_DESC {
	UCHAR RegionIndex;
	PCH_DESC pChDesc;
}COUNTRY_REGION_CH_DESC, *PCOUNTRY_REGION_CH_DESC;

#ifdef EXT_BUILD_CHANNEL_LIST
#define ODOR			0
#define IDOR			1
#define BOTH			2

typedef struct _CH_DESP {
	UCHAR FirstChannel;
	UCHAR NumOfCh;
	CHAR MaxTxPwr;			/* dBm */
	UCHAR Geography;			/* 0:out door, 1:in door, 2:both */
	BOOLEAN DfsReq;			/* Dfs require, 0: No, 1: yes. */
} CH_DESP, *PCH_DESP;

typedef struct _CH_REGION {
	UCHAR CountReg[3];
	UCHAR DfsType;			/* 0: CE, 1: FCC, 2: JAP, 3:JAP_W53, JAP_W56 */
	CH_DESP *pChDesp;
	BOOLEAN edcca_on ;
} CH_REGION, *PCH_REGION;

extern CH_REGION ChRegion[];
#endif /* EXT_BUILD_CHANNEL_LIST */


#ifdef ED_MONITOR
typedef struct _COUNTRY_PROP {
	UCHAR CountReg[3];
	UCHAR DfsType;			/* 0: CE, 1: FCC, 2: JAP, 3:JAP_W53, JAP_W56 */
	BOOLEAN edcca_on;
} COUNTRY_PROP, *PCOUNTRY_PROP;
#endif /* ED_MONITOR */

typedef struct _CH_FREQ_MAP_{
	UINT16		channel;
	UINT16		freqKHz;
}CH_FREQ_MAP;

extern CH_FREQ_MAP CH_HZ_ID_MAP[];
extern int CH_HZ_ID_MAP_NUM;




#define     MAP_CHANNEL_ID_TO_KHZ(_ch, _khz)                 \
			RTMP_MapChannelID2KHZ(_ch, (UINT32 *)&(_khz))
#define     MAP_KHZ_TO_CHANNEL_ID(_khz, _ch)                 \
			RTMP_MapKHZ2ChannelID(_khz, (INT *)&(_ch))

#ifdef ED_MONITOR
BOOLEAN GetEDCCASupport(
	IN PRTMP_ADAPTER pAd);
#endif /* ED_MONITOR */

#ifdef EXT_BUILD_CHANNEL_LIST
VOID BuildChannelListEx(
	IN PRTMP_ADAPTER pAd);

VOID BuildBeaconChList(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pBuf,
	OUT	PULONG pBufLen);
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef DOT11_N_SUPPORT
VOID N_ChannelCheck(
	IN PRTMP_ADAPTER pAd);

VOID N_SetCenCh(
	IN PRTMP_ADAPTER pAd);

BOOLEAN N_ChannelGroupCheck(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR Channel);

#endif /* DOT11_N_SUPPORT */

UINT8 GetCuntryMaxTxPwr(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 channel);

VOID RTMP_MapChannelID2KHZ(
	IN UCHAR Ch,
	OUT UINT32 *pFreq);

VOID RTMP_MapKHZ2ChannelID(
	IN ULONG Freq,
	OUT INT *pCh);

UCHAR GetChannel_5GHZ(
	IN PCH_DESC pChDesc, 
	IN UCHAR index);

UCHAR GetChannel_2GHZ(
	IN PCH_DESC pChDesc, 
	IN UCHAR index);

UCHAR GetChannelFlag(
	IN PCH_DESC pChDesc, 
	IN UCHAR index);

UINT16 TotalChNum(
	IN PCH_DESC pChDesc);
	
#endif /* __CHLIST_H__ */

