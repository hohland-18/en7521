/*
 ***************************************************************************
 * MediaTek Inc. 
 *
 * All rights reserved. source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek, Inc. is obtained.
 ***************************************************************************

	Module Name:
	qa_agent.h
*/

#ifndef __QA_AGENT_H__
#define __QA_AGENT_H__

#define HQA_CMD_MAGIC_NO 0x18142880

/* RX STAT */
#define HQA_RX_STAT_MACFCSERRCNT                    0x1
#define HQA_RX_STAT_MAC_MDRDYCNT                    0x2
#define HQA_RX_STAT_PHY_MDRDYCNT                    0x3
#define HQA_RX_STAT_PHY_FCSERRCNT                   0x4
#define HQA_RX_STAT_PD					            0x5
#define HQA_RX_STAT_CCK_SIG_SFD                     0x6
#define HQA_RX_STAT_OFDM_SIG_TAG					0x7
#define HQA_RX_STAT_RSSI                            0x8
#define HQA_RX_RESET_PHY_COUNT                      0x9

/* FW Event Callback */
VOID HQA_GetThermalValue_CB(struct cmd_msg *msg, char *Data, UINT16 Len);


typedef struct _HQA_CMD_FRAME {
 	UINT32		MagicNo;
	UINT16		Type;
	UINT16		Id;
	UINT16		Length;
	UINT16		Sequence;
	UCHAR		Data[2048];
}  __attribute__((packed))HQA_CMD_FRAME;

typedef INT32 (*HQA_CMD_HANDLER)(
	struct _RTMP_ADAPTER *pAd,
	RTMP_IOCTL_INPUT_STRUCT	*wrq,
	HQA_CMD_FRAME *HqaCmdFrame);

typedef struct _HQA_CMD_TABLE{
	HQA_CMD_HANDLER *CmdSet;
	UINT32	CmdSetSize;
	UINT32	CmdOffset;
} HQA_CMD_TABLE;

typedef struct _HQA_RX_STAT{
    UINT32 MacFCSErr;
    UINT32 MacMdrdy;
	UINT32 FCSErr_CCK;
	UINT32 FCSErr_OFDM;
    UINT32 CCK_PD;
	UINT32 OFDM_PD;
	UINT32 CCK_SIG_Err;
	UINT32 CCK_SFD_Err;
	UINT32 OFDM_SIG_Err;
	UINT32 OFDM_TAG_Err;
	UINT32 WB_RSSSI0;
	UINT32 IB_RSSSI0;
	UINT32 WB_RSSSI1;
	UINT32 IB_RSSSI1;
	UINT32 PhyMdrdyCCK;
	UINT32 PhyMdrdyOFDM;
	UINT32 DriverRxCount;
	UINT32 RCPI0;
	UINT32 RCPI1;
	UINT32 FreqOffsetFromRX;
	UINT32 RSSI0;
	UINT32 RSSI1;
} __attribute__((packed))HQA_RX_STAT;

#endif /*  __QA_AGENT_H__ */
