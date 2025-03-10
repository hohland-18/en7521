/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

/****************************************************************************

	Abstract:

	All related WMM ACM IOCTL function body.

***************************************************************************/


/* ----- Compile Option ------ */
#define IEEE80211E_SIMULATION

#include "rt_config.h"

#ifdef WMM_ACM_SUPPORT

/* IEEE802.11E related include files */
#include "acm_extr.h" /* used for other modules */
#include "acm_comm.h" /* used for edca/wmm */
#include "acm_edca.h" /* used for edca/wmm */



/* ----- Local Definition ------ */
#define ACM_QOS_SANITY_CHECK(__pAd)						\
	if (__pAd == NULL)									\
	{													\
		printk("err> __pAd == NULL!\n");				\
		return;											\
	}

#define ACM_ARGC_SANITY_CHECK(__Min, __Max) 			\
	if ((Argc != __Min) && (Argc != __Max)) 			\
	{													\
		printk("11e_err> parameters number error!\n");	\
		return;											\
	}

#define ACM_IN_SANITY_CHECK(__Condition, __Msg) 		\
	if (__Condition)									\
	{													\
		printk __Msg;									\
		goto LabelErr;									\
	}

#define ACM_NIN_DEC_GET(__Src, __Max, __MsgErr)			\
	{													\
		__Src = AcmCmdUtilNumGet(&pArgv);				\
		ACM_IN_SANITY_CHECK((__Src > __Max), __MsgErr);	\
	}

#define ACM_NIN_DEC_MGET(__Src, __Min, __Max, __MsgErr)	\
	{													\
		__Src = AcmCmdUtilNumGet(&pArgv);				\
		ACM_IN_SANITY_CHECK((__Src < __Min) || (__Src > __Max), __MsgErr);	\
	}

#define ACM_RANGE_SANITY_CHECK(__Range, __Min, __Max, __MsgErr) \
	{															\
		ACM_IN_SANITY_CHECK((__Range < __Min) || (__Range > __Max), __MsgErr);\
	}

#define ACM_RATE_MAX	((UINT32)300000000)		/* 300Mbps */

#ifdef IEEE80211E_SIMULATION
typedef struct _ACM_DATA_SIM {

	UCHAR MacSrc[6];
	UCHAR MacDst[6];

	UCHAR Direction;		/* 0: receive; 1:transmit */
	UCHAR Type;				/* 0: 11e; 1: WME */
	UCHAR TID;				/* 0 ~ 7 */
	UCHAR AckPolicy;		/* 0: normal ACK; 1: no ACK */

	UINT32 FrameSize;		/* data size */
	UINT32 FlgIsValidEntry;	/* 1: valid; 0: invalid */

	UINT16 NumSeq:12;
	UINT16 NumFrag:4;
} ACM_DATA_SIM;

static UINT32 gSimDelay = 0;
static UINT32 gSimDelayCount;
#endif // IEEE80211E_SIMULATION //


/* ----- Extern Variable ----- */
#ifdef ACM_MEMORY_TEST
extern UINT32 gAcmMemAllocNum;
extern UINT32 gAcmMemFreeNum;
#endif // ACM_MEMORY_TEST //

extern UCHAR gAcmTestFlag;

extern VOID ap_cmm_peer_assoc_req_action(
										IN PRTMP_ADAPTER pAd,
										IN MLME_QUEUE_ELEM *Elem,
										IN BOOLEAN isReassoc);


/* ----- Private Variable ----- */
static ACM_TCLAS gCMD_TCLAS_Group[ACM_TSPEC_TCLAS_MAX_NUM];
static UINT32 gTLS_Grp_ID;

#define SIM_AP_NAME "SampleACMAP"

#ifdef IEEE80211E_SIMULATION
static UCHAR gMAC_STA[6] = { 0x00, 0x0e, 0x2e, 0x82, 0xe7, 0x6d };
static UCHAR gMAC_AP[6] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x01 };

static UCHAR gSimTCPFlag;
static UCHAR gSimTCPDSCP;

#ifdef CONFIG_AP_SUPPORT
static UCHAR gDialogToken;
#endif // CONFIG_AP_SUPPORT //

ACMR_OS_TASK_STRUCT gTaskletSim;
ACMR_OS_TIMER_STRUCT gTimerSim;

static UINT32 gTaskDataSleep;
static ACMR_OS_SPIN_LOCK gSpinLockSim;

static BOOLEAN gCmdFlgIsInit;

#define ACM_DATA_SEM_LOCK(LabelSemErr) \
	do { ACMR_OS_SPIN_LOCK_BH(&gSpinLockSim); \
		if (0) goto LabelSemErr; } while(0);

#define ACM_DATA_SEM_UNLOCK() \
	do { ACMR_OS_SPIN_UNLOCK_BH(&gSpinLockSim); } while(0);

#define ACM_DATA_SEM_LOCK_(LabelSemErr) \
	do { ACMR_OS_SPIN_LOCK_BH(&gSpinLockSim); \
		if (0) goto LabelSemErr; } while(0);

#define ACM_DATA_SEM_UNLOCK_() \
	do { ACMR_OS_SPIN_UNLOCK_BH(&gSpinLockSim); } while(0);

#define ACM_MAX_NUM_OF_SIM_DATA_FLOW     5
static ACM_DATA_SIM gDATA_Sim[ACM_MAX_NUM_OF_SIM_DATA_FLOW];
#endif /* IEEE80211E_SIMULATION */

/* ----- Private Function ----- */
#define ACM_CMD_INPUT_PARAM_DECLARATION	\
	ACMR_PWLAN_STRUC pAd, INT32 Argc, CHAR *pArgv

VOID AcmCmdTclasReset(ACM_CMD_INPUT_PARAM_DECLARATION); //snowpin
VOID AcmCmdTclasCreate(ACM_CMD_INPUT_PARAM_DECLARATION); //snowpin


static VOID AcmCmdBandwidthDisplay(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdStreamDisplay(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdStreamFailDisplay(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdEDCAParamDisplay(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdDATLEnable(ACM_CMD_INPUT_PARAM_DECLARATION);

#ifdef CONFIG_AP_SUPPORT
static VOID AcmCmdAcmFlagCtrl(ACM_CMD_INPUT_PARAM_DECLARATION);
#endif // CONFIG_AP_SUPPORT //

VOID AcmCmdDeltsSend(ACM_CMD_INPUT_PARAM_DECLARATION); //snowpin
static VOID AcmCmdStreamFailClear(ACM_CMD_INPUT_PARAM_DECLARATION);


static VOID AcmCmdUapsdDisplay(ACM_CMD_INPUT_PARAM_DECLARATION);

#ifdef CONFIG_AP_SUPPORT
static VOID AcmCmdTspecReject(ACM_CMD_INPUT_PARAM_DECLARATION);
#endif // CONFIG_AP_SUPPORT //

static VOID AcmCmdStatistics(ACM_CMD_INPUT_PARAM_DECLARATION);



/* ----- Simulation Function ----- */
#ifdef IEEE80211E_SIMULATION
static VOID AcmCmdSimAssocBuild(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimReqRcv(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimDel(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimDataRv(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimDataTx(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimDataStop(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimDataSuspend(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimDataResume(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimReAssocBuild(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimNonQoSAssocBuild(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimNonQoSDataRv(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimRateSet(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimTcpTxEnable(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimStaMacSet(ACM_CMD_INPUT_PARAM_DECLARATION);
#ifdef CONFIG_AP_SUPPORT
static VOID AcmCmdSimRcvTriFrame(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimUapsdQueCtrl(ACM_CMD_INPUT_PARAM_DECLARATION);
#endif // CONFIG_AP_SUPPORT //
static VOID AcmCmdSimWmePSEnter(ACM_CMD_INPUT_PARAM_DECLARATION);
static VOID AcmCmdSimReqPsPollRcv(ACM_CMD_INPUT_PARAM_DECLARATION);
#endif // IEEE80211E_SIMULATION //

static VOID AcmCmdTestFlagCtrl(ACM_CMD_INPUT_PARAM_DECLARATION);


/* ----- Utility Function ----- */
static UINT32 AcmCmdUtilHexGet(CHAR **ppArgv);
static UINT32 AcmCmdUtilNumGet(CHAR **ppArgv);
static VOID AcmCmdUtilMacGet(CHAR **ppArgv, UCHAR *pDevMac);

static VOID AcmCmdStreamDisplayOne(ACMR_PWLAN_STRUC pAd,
									ACM_STREAM_INFO *pStream);

UCHAR AcmCmdInfoParse(ACMR_PWLAN_STRUC pAd,
					CHAR **ppArgv,
					ACM_TSPEC *pTspec,
					ACM_TS_INFO *pInfo,
					UCHAR *pStreamType);

UCHAR AcmCmdInfoParseAdvance(ACMR_PWLAN_STRUC pAd,
					CHAR **ppArgv,
					ACM_TSPEC *pTspec,
					ACM_TS_INFO *pInfo,
					UCHAR *pStreamType);

#ifdef IEEE80211E_SIMULATION

#ifdef CONFIG_AP_SUPPORT
static UINT32 ACM_CMD_WME_Action_Make(ACMR_PWLAN_STRUC pAd,
										ACM_STREAM_INFO *pStream,
										UCHAR *pPkt,
										UCHAR Action,
										UCHAR StatusCode,
										UCHAR TclasProcessing);
#endif // CONFIG_AP_SUPPORT //

static VOID ACM_CMD_Task_Data_Simulation(ULONG Data);
static VOID ACM_CMD_Sim_Data_Rv(ACMR_PWLAN_STRUC pAd, ACM_DATA_SIM *pInfo);
static VOID ACM_CMD_Sim_nonQoS_Data_Rv(ACMR_PWLAN_STRUC pAd, ACM_DATA_SIM *pInfo);
static VOID ACM_CMD_Sim_Data_Tx(ACMR_PWLAN_STRUC pAd, ACM_DATA_SIM *pInfo);
#endif // IEEE80211E_SIMULATION //




/* =========================== Public Function =========================== */

/*
========================================================================
Routine Description:
	Init command related global parameters.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID ACM_CMD_Init(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd)
{
#ifdef IEEE80211E_SIMULATION
	ACMR_LOCK_INIT(&gSpinLockSim);
	ACMR_TIMER_INIT(pAd, gTimerSim, ACMP_CMD_Timer_Data_Simulation, pAd);
#endif // IEEE80211E_SIMULATION //
} /* End of ACM_CMD_Init */


/*
========================================================================
Routine Description:
	Release command related global parameters.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID ACM_CMD_Release(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd)
{
#ifdef IEEE80211E_SIMULATION
	AcmCmdSimDataStop(pAd, 0, NULL);
#endif // IEEE80211E_SIMULATION //

	ACMR_LOCK_FREE(&gSpinLockSim);
} /* End of ACM_CMD_Release */




/* =========================== Private Function =========================== */

/*
========================================================================
Routine Description:
	Reset all TCLAS settings.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QSTA.
========================================================================
*/
VOID AcmCmdTclasReset( //snowpin
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	gTLS_Grp_ID = 0;
} /* End of AcmCmdTclasReset */


/*
========================================================================
Routine Description:
	Create a TCLAS for the future stream.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QSTA.
	2. Max 5 TCLAS for a stream.
	3. Users need to create TCLAS first.  Then create a stream later.
		If users want to create another stream, users
		shall reset all TCLAS and re-create TCLAS for another stream use.
	4. Command Format:
		wstclasadd [up:0~7] [type:0~2] [mask:hex]

========================================================================
*/
VOID AcmCmdTclasCreate( //snowpin
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACM_TCLAS *pTclas;
	UINT32 TclasLen[ACM_TSPEC_TCLAS_MAX_NUM+1] = { ACM_TCLAS_TYPE_ETHERNET_LEN,
												ACM_TCLAS_TYPE_IP_V4_LEN,
												ACM_TCLAS_TYPE_8021DQ_LEN };
	UCHAR *pClassifier;
	UINT32 IdByteNum;


	if (gTLS_Grp_ID >= ACM_TSPEC_TCLAS_MAX_NUM)
	{
		printk("\nErr> max TCLAS number is reached! "
				"Pls. reset the number first!\n");
		return;
	} /* End of if */

	pTclas = &gCMD_TCLAS_Group[gTLS_Grp_ID++];
	pTclas->UserPriority = AcmCmdUtilNumGet(&pArgv);
	pTclas->ClassifierType = AcmCmdUtilNumGet(&pArgv);
	pTclas->ClassifierMask = AcmCmdUtilNumGet(&pArgv);

	pClassifier = (UCHAR *)&pTclas->Clasifier;
	for(IdByteNum=0; IdByteNum<(TclasLen[pTclas->ClassifierType]-3); IdByteNum++)
		*pClassifier ++ = AcmCmdUtilHexGet(&pArgv);
	/* End of for */
} /* End of AcmCmdTclasCreate */




/*
========================================================================
Routine Description:
	Display current bandwidth status.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
static VOID AcmCmdBandwidthDisplay(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACM_BANDWIDTH_INFO BwInfo, *pInfo;
	UINT32 TimePerc;


	/* init */
	pInfo = &BwInfo;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* display */
	if (ACMP_BandwidthInfoGet(pAd, pInfo) != ACM_RTN_OK)
		return;
	/* End of if */

#ifdef CONFIG_AP_SUPPORT
	TimePerc = pInfo->AcmUsedTime * 100;
#ifdef ACM_CC_FUNC_MBSS
	TimePerc += pInfo->MbssTotalUsedTime * 100;
#endif // ACM_CC_FUNC_MBSS //
	TimePerc /= ACM_TIME_BASE;
	printk("\n\t(BSS) Current available bandwidth = %d %%\n", (100-TimePerc));
#endif // CONFIG_AP_SUPPORT //


#ifdef ACM_CC_FUNC_MBSS
	printk("\tCurrent ACM time for other BSS = %d us\n", pInfo->MbssTotalUsedTime);
#endif // ACM_CC_FUNC_MBSS //

	printk("\t(BSS) Station Count of the BSS: %d\n", pInfo->StationCount);
	printk("\t(BSS) Channel Utilization of the BSS: %d %%\n", (pInfo->ChanUtil*100/255));
	printk("\t(BSS) Available Adimission Capability of the BSS: %d us\n", (pInfo->AvalAdmCap<<5));
	printk("\t(BSS) Channel busy time: %dus\n\n", pInfo->ChanBusyTime);

	TimePerc = pInfo->AcUsedTime * 100;
	TimePerc /= ACM_TIME_BASE;

#ifdef CONFIG_AP_SUPPORT
	printk("\t(BSS) Current ACM time for EDCA = %d us\n", pInfo->AcUsedTime);
	printk("\t(BSS) Current ACM bandwidth for EDCA = %d %%\n", TimePerc);

	printk("\t(BSS) Current number of requested TSPECs (not yet response) = %d\n",
			pInfo->NumReqLink);

	if ((pInfo->NumAcLinkUp != 0) || (pInfo->NumAcLinkDn != 0) ||
		(pInfo->NumAcLinkDi != 0) || (pInfo->NumAcLinkBi != 0))
	{
		printk("\n\t(BSS) EDCA uplinks = %02d", pInfo->NumAcLinkUp);
		printk("\t(BSS)EDCA dnlinks = %02d\n", pInfo->NumAcLinkDn);
		printk("\t(BSS)EDCA bilinks = %02d\n", pInfo->NumAcLinkBi);
	} /* End of if */
#endif // CONFIG_AP_SUPPORT //

} /* End of AcmCmdBandwidthDisplay */


/*
========================================================================
Routine Description:
	Display current stream status.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP & QSTA.
	2. Command Format:
		wsshow [1:EDCA, 2:HCCA, 3:ALL] (Client MAC)
	3. If Client MAC doesnt exist, only requested TSPEC & dnlink
		(bidirectional link) are displayed for QAP; only requested
		TSPEC & uplink (bidirectional link) are displayed for QSTA.
		If you want to display uplinks, you should assign client MAC address.
========================================================================
*/
static VOID AcmCmdStreamDisplay(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACM_STREAM_INFO *pStream, *pStreamNext;
	UINT32 Type;
	UCHAR MacPeer[6];
	UINT32 NumStream, SizeBuf;
	UINT32 Category[2] = { ACM_SM_CATEGORY_REQ, ACM_SM_CATEGORY_ACT };
	UINT32 NumCategory;
	UINT32 IdCateNum, IdStmNum;


	/* init */
	NumCategory = 2;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* init */
	ACMR_MEM_ZERO(MacPeer, sizeof(MacPeer));

	Type = AcmCmdUtilNumGet(&pArgv);
	if (Type == 0)
		Type = 1; /* default: EDCA streams */
	/* End of if */

	if (Argc >= 2)
	{
		/* get Client MAC */
		AcmCmdUtilMacGet(&pArgv, MacPeer);

#ifdef IEEE80211E_SIMULATION
		if (*(UINT32 *)MacPeer == 0x00)
		{
			if (MacPeer[5] == 0x00)
				ACMR_MEM_COPY(MacPeer, gMAC_STA, 6);
			else
				ACMR_MEM_COPY(MacPeer, gMAC_STA, 5);
			/* End of if */
		} /* End of if */
#endif // IEEE80211E_SIMULATION //

		NumCategory = 1; /* input & output TS streams */
		Category[0] = ACM_SM_CATEGORY_PEER;
	} /* End of if */

	for(IdCateNum=0; IdCateNum<NumCategory; IdCateNum++)
	{
		NumStream = ACMP_StreamNumGet(pAd, Category[IdCateNum], Type, MacPeer);

		if (NumStream == 0)
		{
			if (Category[IdCateNum] == ACM_SM_CATEGORY_REQ)
				printk("\n    No any requested TSPEC exists!\n");
			else if (Category[IdCateNum] == ACM_SM_CATEGORY_PEER)
				printk("\n    No any TSPEC exists!\n");
			else
				printk("\n    No any output TSPEC exists!\n");
			/* End of if */
			continue;
		} /* End of if */

		SizeBuf = sizeof(ACM_STREAM_INFO) * NumStream;
		pStream = (ACM_STREAM_INFO *)ACMR_MEM_ALLOC(SizeBuf);

		if (pStream == NULL)
		{
			printk("11e_err> Allocate stream memory fail! "
					"AcmCmdStreamDisplay()\n");
			return;
		} /* End of if */

		if (ACMP_StreamsGet(pAd, Category[IdCateNum], Type,
							&NumStream, MacPeer, pStream) != ACM_RTN_OK)
		{
			printk("11e_err> Get stream information fail! "
					"AcmCmdStreamDisplay()\n");
			ACMR_MEM_FREE(pStream);
			return;
		} /* End of if */

		if (Category[IdCateNum] == ACM_SM_CATEGORY_REQ)
		{
			printk("\n\n    ------------------- All Requested List "
					"-------------------");
		}
		else
		{
			if (Category[IdCateNum] == ACM_SM_CATEGORY_ACT)
			{
				printk("\n\n    ------------------- All OUT stream List "
						"-------------------");
			}
			else
			{
				printk("\n\n    ------------------- The Device stream List "
						"-------------------");
			} /* End of if */
		} /* End of if */

		for(IdStmNum=0, pStreamNext=pStream; IdStmNum<NumStream; IdStmNum++)
		{
			/* display the stream information */
			AcmCmdStreamDisplayOne(pAd, pStreamNext);
			pStreamNext ++;
		} /* End of for */

		ACMR_MEM_FREE(pStream);
	} /* End of while */
} /* End of AcmCmdStreamDisplay */


/*
========================================================================
Routine Description:
	Display fail stream status.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
static VOID AcmCmdStreamFailDisplay(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACM_STREAM_INFO *pStream, *pStreamNext;
	UINT32 Category, Type;
	UINT32 NumStream, SizeBuf;
	UINT32 IdStmNum;


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* init */
	Category = ACM_SM_CATEGORY_ERR;
	Type = ACM_ACCESS_POLICY_MIX;

	/* get fail streams */
	NumStream = ACMP_StreamNumGet(pAd, Category, Type, NULL);

	if (NumStream == 0)
	{
		printk("    No any fail TSPEC exists!\n");
		return;
	} /* End of if */

	SizeBuf = sizeof(ACM_STREAM_INFO) * NumStream;
	pStream = (ACM_STREAM_INFO *)ACMR_MEM_ALLOC(SizeBuf);

	if (pStream == NULL)
	{
		printk("11e_err> Allocate stream memory fail! "
				"AcmCmdStreamFailDisplay()\n");
		return;
	} /* End of if */

	if (ACMP_StreamsGet(pAd, Category, Type, &NumStream, NULL, pStream) != \
																ACM_RTN_OK)
	{
		printk("11e_err> Get stream information fail! "
				"AcmCmdStreamFailDisplay()\n");
		ACMR_MEM_FREE(pStream);
		return;
	} /* End of if */

	for(IdStmNum=0, pStreamNext=pStream; IdStmNum<NumStream; IdStmNum++)
	{
		/* display the stream information */
		AcmCmdStreamDisplayOne(pAd, pStreamNext);
		pStreamNext ++;
	} /* End of for */

	ACMR_MEM_FREE(pStream);
} /* End of AcmCmdStreamFailDisplay */


/*
========================================================================
Routine Description:
	Display current EDCA parameters.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
static VOID AcmCmdEDCAParamDisplay(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACM_CTRL_INFO CtrlInfo, *pInfo = &CtrlInfo;
	UINT32 TimePerc, TimeAcm, TimeAcmMax;
	UINT32 IdAcNum, IdAcOther;


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get EDCA information */
	ACMP_ControlInfomationGet(pAd, pInfo);

	/* display information */
	printk("\n    Downgrade information:\n");

	for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
	{
		if (pInfo->FlgIsAcmEnable[IdAcNum])
		{
			if (pInfo->DowngradeAcNum[IdAcNum] < ACM_DEV_NUM_OF_AC)
			{
				printk("    AC%d ACM is enabled and Downgrade AC = %d\n",
						IdAcNum, pInfo->DowngradeAcNum[IdAcNum]);
			}
			else
				printk("    AC%d ACM is enabled and Downgrade AC = NONE\n", IdAcNum);
			/* End of if */
		}
		else
			printk("    AC%d ACM is disabled!\n", IdAcNum);
		/* End of if */
	} /* End of for */

	printk("\n    Channel Utilization Quota information:\n");
	printk("    Minimum Contention Period  = %d/%d service interval\n",
			pInfo->CP_MinNu, pInfo->CP_MinDe);
	printk("    Minimum Best Effort Period = %d/%d service interval\n",
			pInfo->BEK_MinNu, pInfo->BEK_MinDe);

	printk("\n    EDCA AC ACM information:\n");
	printk("    BW/AC\tAC0\t\tAC1\t\tAC2\t\tAC3");

#ifdef CONFIG_AP_SUPPORT
	if (pInfo->FlgDatl)
	{
		printk("\n    MIN BW\t");
		for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
			printk("%02d%%\t\t", pInfo->DatlBwMin[IdAcNum]);
		/* End of for */

		printk("\n    MAX BW\t");
		for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
			printk("%02d%%\t\t", pInfo->DatlBwMax[IdAcNum]);
		/* End of for */
	} /* End of if */
#endif // CONFIG_AP_SUPPORT //

	TimeAcmMax = ACM_TIME_BASE;

	printk("\n    USE BW\t");
	for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
	{
		TimeAcm = pInfo->AcmAcTime[IdAcNum];
		TimePerc = 0;

		for(IdAcOther=0; IdAcOther<ACM_DEV_NUM_OF_AC; IdAcOther++)
		{
			if (IdAcOther == IdAcNum)
				continue;
			/* End of if */

			TimePerc += pInfo->DatlBorAcBw[IdAcNum][IdAcOther];
		} /* End of for */

		TimePerc += TimeAcm;
		TimePerc *= 100;
		TimePerc /= TimeAcmMax;

#ifdef CONFIG_AP_SUPPORT
		if ((pInfo->FlgDatl) && (TimePerc > pInfo->DatlBwMax[IdAcNum]))
			printk("%02d%%\t\t", pInfo->DatlBwMax[IdAcNum]);
		else
#endif // CONFIG_AP_SUPPORT //
			printk("%02d%%\t\t", TimePerc);
		/* End of if */
	} /* End of for */

#ifdef CONFIG_AP_SUPPORT
	if (pInfo->FlgDatl)
	{
		printk("\n    AVL BW\t");
		for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
			printk("%02dus\t", (pInfo->AvalAdmCapAc[IdAcNum]<<5));
		/* End of for */
	} /* End of if */
#endif // CONFIG_AP_SUPPORT //

	printk("\n\n");
} /* End of AcmCmdEDCAParamDisplay */


/*
========================================================================
Routine Description:
	Enable or disable Dynamic ATL.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format: 25
		[enable/disable 1/0] (minimum bw threshold for AC0~AC3)
		(maximum bw threshold for AC0~AC3)
========================================================================
*/
static VOID AcmCmdDATLEnable(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR FlgIsEnable;
	UCHAR DatlBwMin[ACM_DEV_NUM_OF_AC];
	UCHAR DatlBwMax[ACM_DEV_NUM_OF_AC];
	UINT32 IdAcNum, SumBw;


	FlgIsEnable = AcmCmdUtilNumGet(&pArgv);

	if (Argc >= 2)
	{
		/* input parameters include minimum & maximum bandwidth threshold */
		for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
			DatlBwMin[IdAcNum] = AcmCmdUtilNumGet(&pArgv);
		/* End of for */

		for(IdAcNum=0, SumBw=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
		{
			DatlBwMax[IdAcNum] = AcmCmdUtilNumGet(&pArgv);
			SumBw += DatlBwMax[IdAcNum];
		} /* End of for */

		if (SumBw != ACM_DATL_BW_MAX_SUM)
			return;
		/* End of if */

		for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
		{
			if (DatlBwMin[IdAcNum] > DatlBwMax[IdAcNum])
				return; /* min should be <= max */
			/* End of if */
		} /* End of for */

		ACMP_DatlCtrl(pAd, FlgIsEnable, DatlBwMin, DatlBwMax);
	}
	else
		ACMP_DatlCtrl(pAd, FlgIsEnable, NULL, NULL);
	/* End of if */
} /* End of AcmCmdDATLEnable */


#ifdef CONFIG_AP_SUPPORT
/*
========================================================================
Routine Description:
	Enable or disable ACM Flag for each AC.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format: 10
		[AC0] [AC1] [AC2] [AC3]
========================================================================
*/
static VOID AcmCmdAcmFlagCtrl(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR FlgIsAcmEnabled[ACM_DEV_NUM_OF_AC];


	FlgIsAcmEnabled[0] = AcmCmdUtilNumGet(&pArgv);
	FlgIsAcmEnabled[1] = AcmCmdUtilNumGet(&pArgv);
	FlgIsAcmEnabled[2] = AcmCmdUtilNumGet(&pArgv);
	FlgIsAcmEnabled[3] = AcmCmdUtilNumGet(&pArgv);

	ACMP_EnableFlagReset(pAd, FlgIsAcmEnabled[0], FlgIsAcmEnabled[1],
						FlgIsAcmEnabled[2], FlgIsAcmEnabled[3]);
} /* End of AcmCmdAcmFlagCtrl */
#endif // CONFIG_AP_SUPPORT //


/*
========================================================================
Routine Description:
	Send a delts to a peer.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
	[Peer MAC] [TID:0~7]

	We deleted a TS based on the TSID, not Direction or AC ID.
========================================================================
*/
VOID AcmCmdDeltsSend(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACMR_STA_DB	*pCdb;
	ACM_STREAM *pStream;
	ACM_TS_INFO TsInfo;
	UINT32 TID;
	UCHAR MacPeer[6];
	ULONG SplFlags;


	/* init */
	pCdb = NULL;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get peer mac address */
	AcmCmdUtilMacGet(&pArgv, MacPeer);


#ifdef IEEE80211E_SIMULATION
#ifdef CONFIG_AP_SUPPORT
	if (*(UINT32 *)MacPeer == 0)
		ACMR_MEM_COPY(MacPeer, gMAC_STA, 6);
	/* End of if */
#endif // CONFIG_AP_SUPPORT //
#endif // IEEE80211E_SIMULATION //

	/* get input arguments */
	ACM_NIN_DEC_MGET(TID,  0, 7, ("err> TID fail!\n"));

	/* get sta entry */
	pCdb = ACMR_STA_ENTRY_GET(pAd, MacPeer);
	if (pCdb == NULL)
	{
		printk("11e_err> the peer does NOT exist "
				"0x%02x:%02x:%02x:%02x:%02x:%02x:!\n",
				MacPeer[0], MacPeer[1], MacPeer[2],
				MacPeer[3], MacPeer[4], MacPeer[5]);
		return;
	} /* End of if */

#ifdef CONFIG_AP_SUPPORT
	if ((pCdb->PsMode == PWR_SAVE) && (pCdb->bAPSDAllAC == 1))
	{
		printk("11e_msg> All AC are UAPSD, we can not send DELTS frame!\n");
		return;
	} /* End of if */
#endif // CONFIG_AP_SUPPORT //

	/* get management semaphore */
	ACM_TSPEC_IRQ_LOCK_CHK(pAd, SplFlags, LabelSemErr);

	/* find the request */
	TsInfo.TSID = TID;

	pStream = ACM_TC_Find(pAd, MacPeer, &TsInfo);
	if (pStream == NULL)
	{
		ACM_TSPEC_IRQ_UNLOCK(pAd, SplFlags, LabelSemErr);
		printk("acm_msg> can not find the stream (TID=%d)!\n", TID);
		return;
	} /* End of if */

	/* delete the stream */
#ifdef CONFIG_AP_SUPPORT
	pStream->Cause = TSPEC_CAUSE_DELETED_BY_QAP;
#endif // CONFIG_AP_SUPPORT //


	if (ACM_TC_Delete(pAd, pStream) == TRUE)
	{
		ACM_DELTS_SEND(pAd, pStream->pCdb, pStream, LabelSemErr);
	} /* End of if */

	/* release semaphore */
	ACM_TSPEC_IRQ_UNLOCK(pAd, SplFlags, LabelSemErr);

LabelErr:
	return;

LabelSemErr:
	/* management semaphore get fail */
	ACMR_DEBUG(ACMR_DEBUG_ERR,
				("11e_err> Semaphore Lock! AcmCmdDeltsSend()\n"));
	return;
} /* End of AcmCmdDeltsSend */


/*
========================================================================
Routine Description:
	Clear fail stream status.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
static VOID AcmCmdStreamFailClear(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* clear */
	ACMP_StreamFailClear(pAd);
} /* End of AcmCmdStreamFailClear */




/*
========================================================================
Routine Description:
	Display UAPSD information for a device.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
	[Peer MAC]
========================================================================
*/
static VOID AcmCmdUapsdDisplay(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR MacPeer[6];
#ifdef CONFIG_AP_SUPPORT
	ACMR_STA_DB	*pCdb;
	UINT32 IdAcNum;
#endif // CONFIG_AP_SUPPORT //


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* init */
	ACMR_MEM_ZERO(MacPeer, sizeof(MacPeer));

	if (Argc >= 1)
	{
		/* get Client MAC */
		AcmCmdUtilMacGet(&pArgv, MacPeer);

#ifdef IEEE80211E_SIMULATION
		if (*(UINT32 *)MacPeer == 0x00)
		{
			if (MacPeer[5] == 0x00)
				ACMR_MEM_COPY(MacPeer, gMAC_STA, 6);
			else
				ACMR_MEM_COPY(MacPeer, gMAC_STA, 5);
			/* End of if */
		} /* End of if */
#endif // IEEE80211E_SIMULATION //
	} /* End of if */

#ifdef CONFIG_AP_SUPPORT
	pCdb = ACMR_STA_ENTRY_GET(pAd, MacPeer);
	if (pCdb != NULL)
	{
		if (ACMR_STA_IS_IN_ACTIVE_MODE(pCdb))
			printk("\n    EDCA AC UAPSD information: (ACTIVE)\n");
		else
			printk("\n    EDCA AC UAPSD information: (POWER SAVE)\n");
		/* End of if */

		if (pCdb->MaxSPLength != 0)
		{
			printk("    Max SP Length: %d (%d frames)\n",
					pCdb->MaxSPLength, pCdb->MaxSPLength<<1);
		}
		else
			printk("    Max SP Length: 0 (all frames)\n");
		/* End of if */

		printk("    UAPSD/AC   AC0    AC1    AC2    AC3");

		printk("\n    Tr/De      ");

		for(IdAcNum=0; IdAcNum<ACM_DEV_NUM_OF_AC; IdAcNum++)
		{
			printk("%d/%d    ",
					pCdb->bAPSDCapablePerAC[IdAcNum],
					pCdb->bAPSDDeliverEnabledPerAC[IdAcNum]);
		} /* End of for */
	} /* End of if */
#endif // CONFIG_AP_SUPPORT //

	printk("\n");
} /* End of AcmCmdUapsdDisplay */


#ifdef CONFIG_AP_SUPPORT
/*
========================================================================
Routine Description:
	Enable or disable all TSPEC rejection function.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	Only for QAP.
========================================================================
*/
static VOID AcmCmdTspecReject(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR FlgIsEnable;


	FlgIsEnable = AcmCmdUtilNumGet(&pArgv);
	ACMP_TC_RejectCtrl(pAd, FlgIsEnable);
} /* End of AcmCmdTspecReject */
#endif // CONFIG_AP_SUPPORT //




/*
========================================================================
Routine Description:
	Display ACM related statistics count.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
========================================================================
*/
VOID AcmCmdStatistics(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACM_STATISTICS Stats, *pStats;


	pStats = &Stats;
	ACMP_StatisticsGet(pAd, pStats);

	printk("ACM Related Statistics Count:\n\n");
	printk("*Drop by ACM:\t\t%d\n", pStats->DropByACM);
	printk("*Drop by Time:\t\t%d\n", pStats->DropByAdmittedTime);
	printk("*Priority Change VO:\t%d\n", pStats->PriorityChange[ACM_VO_ID]);
	printk("*Priority Change VI:\t%d\n", pStats->PriorityChange[ACM_VI_ID]);
	printk("*Priority Change BK:\t%d\n", pStats->PriorityChange[ACM_BK_ID]);
	printk("*Priority Change BE:\t%d\n", pStats->PriorityChange[ACM_BE_ID]);
	printk("*Downgrade VO:\t\t%d\n", pStats->Downgrade[ACM_VO_ID]);
	printk("*Downgrade VI:\t\t%d\n", pStats->Downgrade[ACM_VI_ID]);
	printk("*Downgrade BK:\t\t%d\n", pStats->Downgrade[ACM_BK_ID]);
	printk("*Downgrade BE:\t\t%d\n", pStats->Downgrade[ACM_BE_ID]);

#ifdef ACM_CC_FUNC_11N
{
	UINT32 IdBa;

	printk("*Predict AMPDU:\t\t");
	for(IdBa=0; IdBa<sizeof(pStats->AMPDU)/sizeof(pStats->AMPDU[0]); IdBa++)
	{
		if ((IdBa != 0) && ((IdBa & 0x07) == 0))
			printk("\n\t\t\t");
		/* End of if */

		printk("%d\t", pStats->AMPDU[IdBa]);
	} /* End of for */
}
#endif // ACM_CC_FUNC_11N //

	printk("\n");
} /* End of AcmCmdStatistics */






#ifdef IEEE80211E_SIMULATION
/* =========================== Simulation Function ========================== */

/*
========================================================================
Routine Description:
	Simulate a QoS authentication req & association req event.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_assoc (Client MAC) (UAPSD AC0) (UAPSD AC1) (UAPSD AC2) (UAPSD AC3)
		(Is 11n) (HT Capability Info byte1) (HT Capability Info byte2)
		(MCS Set 0~7) (MCS Set 8~15) (MCS Set 16~23)
	3. For HT Capability Info
		bit1: Supported channel width set
		bit5: Short GI for 20 MHz
		bit6: Short GI for 40 MHz
	4. 11n association command example:
		iwpriv ra0 set acm=50_00_1_1_1_1_1_6e_00_ff_ff_00
========================================================================
*/
VOID AcmCmdSimAssocBuild(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef CONFIG_AP_SUPPORT
	NDIS_STATUS		Status;
	HEADER_802_11	HdrAuthAssoc;

	PUCHAR	pBufOut;
	ULONG	LenFrame;
	USHORT	AuthAlg, AuthSeq, AuthStatus;
	USHORT	AssocCap, AssocLsn;
	UCHAR	SSID_EID, SSID_Len, SSID[MAX_LEN_OF_SSID];
	UCHAR	SUPR_EID, SUPR_Len, SupRate[4] = { 0x82, 0x84, 0x8B, 0x96 };
	UCHAR	EXTR_EID, EXTR_Len, ExtRate[4] = { 0xB0, 0xC8, 0xE0, 0xEC };
	UCHAR	IE_WME[9] = {IE_VENDOR_SPECIFIC, 0x07, 0x00, 0x50, 0xf2,
						0x02, 0x00, 0x01, 0x0F};
	UCHAR	DevMac[6], *pStaMac;
	UCHAR	UapsdAc0, UapsdAc1, UapsdAc2, UapsdAc3;

	/* for 11n */
	UCHAR	IdMcsByte;
	UCHAR	Flg11N; /* is this a 11n station */
	UCHAR	HTCapInfoEID, HTCapInfoLen, HTCapInfo[26] = {
			0x6E, 0x00, 0x13, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


	/* init */
	pBufOut = NULL;
	SSID_Len = pAd->ApCfg.MBSSID[0].SsidLen;
	ACMR_MEM_COPY(SSID, pAd->ApCfg.MBSSID[0].Ssid, SSID_Len);

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get client MAC */
	if (Argc >= 1)
	{
		DevMac[0] = gMAC_STA[0];
		DevMac[1] = gMAC_STA[1];
		DevMac[2] = gMAC_STA[2];
		DevMac[3] = gMAC_STA[3];
		DevMac[4] = gMAC_STA[4];
		DevMac[5] = AcmCmdUtilHexGet(&pArgv);
		pStaMac = DevMac;

		if (DevMac[5] == 0x00)
			DevMac[5] = gMAC_STA[5];
		/* End of if */

		UapsdAc0 = AcmCmdUtilHexGet(&pArgv) & 0x01;
		UapsdAc1 = AcmCmdUtilHexGet(&pArgv) & 0x01;
		UapsdAc2 = AcmCmdUtilHexGet(&pArgv) & 0x01;
		UapsdAc3 = AcmCmdUtilHexGet(&pArgv) & 0x01;

		IE_WME[8] = (UapsdAc0 << 3) |
					(UapsdAc1 << 2) |
					(UapsdAc2 << 1) |
					(UapsdAc3);

		/* for 11n */
		Flg11N = AcmCmdUtilNumGet(&pArgv);

		if (Flg11N != 0)
		{
			HTCapInfoEID = 0x2D;
			HTCapInfoLen = 26;

			HTCapInfo[0] = AcmCmdUtilHexGet(&pArgv);
			HTCapInfo[1] = AcmCmdUtilHexGet(&pArgv);

			/* only support MCS0 ~ MCS23 */
			for(IdMcsByte=0; IdMcsByte<3; IdMcsByte++)
				HTCapInfo[3+IdMcsByte] = AcmCmdUtilHexGet(&pArgv);
			/* End of for */
		} /* End of if */
	}
	else
	{
		pStaMac = gMAC_STA;

		/* for 11n */
		Flg11N = 0;
	} /* End of if */

	/* allocate authentication request frame buffer */
	Status = MlmeAllocateMemory(pAd, &pBufOut);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		printk("11e_err> allocate auth buffer fail!\n");
		return;
	} /* End of if */

	/* init authentication frame body */
	AuthAlg = Ndis802_11AuthModeOpen;
	AuthSeq = 1;
	AuthStatus = MLME_SUCCESS;

	MgtMacHeaderInit(pAd, &HdrAuthAssoc, SUBTYPE_AUTH, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);
	MakeOutgoingFrame(pBufOut,			&LenFrame,
					sizeof(HEADER_802_11),	&HdrAuthAssoc,
					2,						&AuthAlg,
					2,						&AuthSeq,
					2,						&AuthStatus,
					END_OF_ARGS);

	/* send the virtual auth frame to MLME handler */
	printk("11e_msg> simulate a authen request...\n");

	REPORT_MGMT_FRAME_TO_MLME(pAd, 0, pBufOut, LenFrame, 0, 0, 0, 0);

	/* init association frame body */
	MgtMacHeaderInit(pAd, &HdrAuthAssoc, SUBTYPE_ASSOC_REQ, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);

	AssocCap = 0x0001;
	AssocLsn = 1;
	SSID_EID = IE_SSID;
	SUPR_EID = IE_SUPP_RATES;
	SUPR_Len = 4;
	EXTR_EID = IE_EXT_SUPP_RATES;
	EXTR_Len = 4;

	if (Flg11N != 0)
	{
		/* for 11n */
		MakeOutgoingFrame(pBufOut,				&LenFrame,
							sizeof(HEADER_802_11),	&HdrAuthAssoc,
							2,						&AssocCap,
							2,						&AssocLsn,
							1,						&SSID_EID,
							1,						&SSID_Len,
							SSID_Len,				SSID,
							1,						&SUPR_EID,
							1,						&SUPR_Len,
							SUPR_Len,				SupRate,
							1,						&EXTR_EID,
							1,						&EXTR_Len,
							EXTR_Len,				ExtRate,
							9,						&IE_WME[0],
							1,						&HTCapInfoEID,
							1,						&HTCapInfoLen,
							HTCapInfoLen,			HTCapInfo,
							END_OF_ARGS);
	}
	else
	{
		MakeOutgoingFrame(pBufOut,				&LenFrame,
							sizeof(HEADER_802_11),	&HdrAuthAssoc,
							2,						&AssocCap,
							2,						&AssocLsn,
							1,						&SSID_EID,
							1,						&SSID_Len,
							SSID_Len,				SSID,
							1,						&SUPR_EID,
							1,						&SUPR_Len,
							SUPR_Len,				SupRate,
							1,						&EXTR_EID,
							1,						&EXTR_Len,
							EXTR_Len,				ExtRate,
							9,						&IE_WME[0],
							END_OF_ARGS);
	} /* End of if */

	/* send the virtual assoc frame to MLME handler */
	printk("11e_msg> simulate a assoc request...\n");

	REPORT_MGMT_FRAME_TO_MLME(pAd, 0, pBufOut, LenFrame, 0, 0, 0, 0);

	/* free the frame buffer */
	MlmeFreeMemory(pAd, pBufOut);
#endif // CONFIG_AP_SUPPORT //
} /* End of AcmCmdSimAssocBuild */


/*
========================================================================
Routine Description:
	Simulate a ADDTS Request frame receive event.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP.

	[sta mac:xx:xx:xx:xx:xx:xx]
	[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
	[APSD:0~1] [nom size:byte] [inact:sec] [mean data rate:bps]
	[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
	[ack policy:0~3]
========================================================================
*/
VOID AcmCmdSimReqRcv(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef CONFIG_AP_SUPPORT
	ACM_STREAM_INFO	StreamInfo;
	ACMR_STA_DB	*pCdb;
	ACM_TSPEC *pTspec;
	ACM_TS_INFO	*pInfo;
	UCHAR StreamType;
	UCHAR MAC[6], *pStaMac;
	UCHAR TclasProcessing;

	NDIS_STATUS Status;
	HEADER_802_11 HdrAction;
	PUCHAR pBufOut;
	ULONG LenFrame;


	/* init */
	pCdb = NULL;
	pTspec = &StreamInfo.Tspec;
	pInfo = &pTspec->TsInfo;
	pStaMac = MAC;
	pBufOut = NULL;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get sta mac address */
	ACMR_MEM_COPY(MAC, gMAC_STA, 6);

	MAC[5] = AcmCmdUtilHexGet(&pArgv);
	if (MAC[5] == 0x00)
		ACMR_MEM_COPY(MAC, gMAC_STA, 6);
	/* End of if */

	/* get sta entry */
	pCdb = ACMR_STA_ENTRY_GET(pAd, MAC);
	if (pCdb == NULL)
	{
		printk("11e_err> the station %02x:%02x:%02x:%02x:%02x:%02x "
				"does NOT exist!\n",
				MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
		return;
	} /* End of if */

	/* parse input command */
	if (AcmCmdInfoParse(
							pAd,
							&pArgv,
							pTspec,
							pInfo,
							&StreamType) != 0)
	{
		return;
	} /* End of if */

	TclasProcessing = AcmCmdUtilNumGet(&pArgv);
	pInfo->AckPolicy = AcmCmdUtilNumGet(&pArgv); /* default 0 */

	/* vitual send a ADDTS request frame to our AP */
	Status = MlmeAllocateMemory(pAd, &pBufOut);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		printk("11e_err> allocate action buffer fail!\n");
		return;
	} /* End of if */

	/* init ADDTS request frame body */
	MgtMacHeaderInit(pAd, &HdrAction, SUBTYPE_ACTION, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);
	MakeOutgoingFrame(pBufOut,			&LenFrame,
					sizeof(HEADER_802_11),	&HdrAction,
					END_OF_ARGS);

	LenFrame += ACM_CMD_WME_Action_Make(pAd, &StreamInfo,
										&pBufOut[LenFrame],
										ACM_ACTION_WME_SETUP_REQ,
										0,
										TclasProcessing);

	/* send the ADDTS request frame to ACM module */
	ACMP_ManagementHandle(pAd, pCdb, ACMR_SUBTYPE_ACTION,
						pBufOut, LenFrame, 11000000);

	/* free the frame buffer */
	MlmeFreeMemory(pAd, pBufOut);
#endif // CONFIG_AP_SUPPORT //
} /* End of AcmCmdSimReqRcv */


/*
========================================================================
Routine Description:
	Delete a actived stream.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP & QSTA.
	2. Command Format: 52
		wssm_del [Client MAC] [type:1-WME] [TID:0~7] [dir:0~3]
========================================================================
*/
VOID AcmCmdSimDel(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACMR_WLAN_HEADER HdrAction;
	ACMR_STA_DB *pCdb;
	ULONG LenFrame;
	UCHAR *pBufFrame;
	NDIS_STATUS Status;

	/* WME tear down packet */
	ACM_WME_NOT_FRAME *pNotFrame;
	ACM_ELM_WME_TSPEC *pElmTspec;
	ACM_WME_TSPEC *pTspec;
	ACM_WME_TS_INFO *pInfo;
	UINT32 Type, TID, Dir;
	UCHAR DevMac[6];


	/* init */
	pCdb = NULL;
	LenFrame = 0;
	pBufFrame = NULL;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get input arguments */
#ifdef CONFIG_AP_SUPPORT
	memcpy(DevMac, gMAC_STA, 6);
	DevMac[5] = AcmCmdUtilHexGet(&pArgv);
	if (DevMac[5] == 0x00)
		DevMac[5] = gMAC_STA[5];
	/* End of if */
#endif // CONFIG_AP_SUPPORT //

	ACM_NIN_DEC_MGET(Type, 0, 1, ("err> type fail!\n"));
	ACM_NIN_DEC_MGET(TID,  0, 7, ("err> TID fail!\n"));
	ACM_NIN_DEC_MGET(Dir,  0, 3, ("err> direction fail!\n"));

	/* get sta entry */
	pCdb = ACMR_STA_ENTRY_GET(pAd, DevMac);
	if (pCdb == NULL)
	{
		printk("11e_err> the station does NOT exist!\n");
		return;
	} /* End of if */

	/* get an unused nonpaged memory */
	Status = MlmeAllocateMemory(pAd, &pBufFrame);
	if (Status != NDIS_STATUS_SUCCESS)
		return;
	/* End of if */

	/* make the frame header */
#ifdef CONFIG_AP_SUPPORT
	MgtMacHeaderInit(pAd, &HdrAction, SUBTYPE_ACTION, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid,
					DevMac);
#endif // CONFIG_AP_SUPPORT //

	MakeOutgoingFrame(pBufFrame,					&LenFrame,
					sizeof(ACMR_WLAN_HEADER),	&HdrAction,
					END_OF_ARGS);

	if (Type == 1)
	{
		/* init WME Tear down frame body */
		pNotFrame = (ACM_WME_NOT_FRAME *)&pBufFrame[LenFrame];
		pNotFrame->Category = ACM_CATEGORY_WME;
		pNotFrame->Action = ACM_ACTION_WME_TEAR_DOWN;
		pNotFrame->DialogToken = 0;
		pNotFrame->StatusCode = 0;

		pElmTspec = &pNotFrame->ElmTspec;
		pElmTspec->ElementId = ACM_ELM_WME_ID;
		pElmTspec->Length = ACM_ELM_WME_TSPEC_LEN;
		pElmTspec->OUI[0] = ACM_WME_OUI_0;
		pElmTspec->OUI[1] = ACM_WME_OUI_1;
		pElmTspec->OUI[2] = ACM_WME_OUI_2;
		pElmTspec->OUI_Type = ACM_WME_OUI_TYPE;
		pElmTspec->OUI_SubType = ACM_WME_OUI_SUBTYPE_TSPEC;
		pElmTspec->Version = ACM_WME_OUI_VERSION;

		pTspec = &pElmTspec->Tspec;
		memset((UCHAR *)pTspec, 0, sizeof(ACM_WME_TSPEC));

		pInfo = &pTspec->TsInfo;
		pInfo->Reserved1 = 0;
		pInfo->Reserved2 = 0;
		pInfo->UP = 0;
		pInfo->PSB = 0;
		pInfo->Reserved3 = 0;
		pInfo->Zero1 = 0;
		pInfo->One = 0;
		pInfo->Direction = Dir;
		pInfo->TID = TID;
		pInfo->Reserved4 = 0;

		LenFrame += ACM_NOT_FRAME_BODY_LEN;
	} /* End of if */

	/* dont care other parameters in TSPEC, such as NominalMsduSize, etc. */

	/* send the DELTS frame to ACM module */
#ifdef CONFIG_AP_SUPPORT
	ACM_ActionHandleByQAP(pAd, pCdb, pBufFrame, LenFrame, 11000000);
#endif // CONFIG_AP_SUPPORT //

LabelErr:
	/* free the frame buffer */
	if (pBufFrame != NULL)
		MlmeFreeMemory(pAd, pBufFrame);
	/* End of if */
} /* End of AcmCmdSimDel */


/*
========================================================================
Routine Description:
	Continue to receive QoS packets from a QSTA.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_datrv [Src Client MAC] [Dst Client MAC]
					[type:0-11e, 1-WME] [TID:0~7] [size] (ack: 0~1)
========================================================================
*/
VOID AcmCmdSimDataRv(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR MacSrc[6], MacDst[6];
	UINT32 Type, SizeData;
	UINT32 AckPolicy;
	UINT32 SizeFrag;
	UCHAR FlgIsRtsEnabled, FlgIsTimerEnabled;
	UCHAR TSID;
	UINT32 IdFlowNum;
/*	ULONG SplFlags; */


	/* init */
	AckPolicy = 0;
	SizeFrag = 0;
	FlgIsRtsEnabled = 0;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* sanity check for parameter number */
	ACM_ARGC_SANITY_CHECK(6, 9);

	/* active data task */
	if (gCmdFlgIsInit == 0)
	{
		/* invalid all flows */
		for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
			gDATA_Sim[IdFlowNum].FlgIsValidEntry = 0;
		/* End of for */

		/* init */
		gTaskDataSleep = 1;

		ACMR_TASK_INIT(pAd, gTaskletSim, ACM_CMD_Task_Data_Simulation, pAd, "ACM_SIM");

		gCmdFlgIsInit = 1;
	} /* End of if */

	/* get source Client MAC */
	MacSrc[0] = gMAC_STA[0];
	MacSrc[1] = gMAC_STA[1];
	MacSrc[2] = gMAC_STA[2];
	MacSrc[3] = gMAC_STA[3];
	MacSrc[4] = gMAC_STA[4];
	MacSrc[5] = AcmCmdUtilHexGet(&pArgv);

	/* get destination Client MAC */
	MacDst[0] = gMAC_STA[0];
	MacDst[1] = gMAC_STA[1];
	MacDst[2] = gMAC_STA[2];
	MacDst[3] = gMAC_STA[3];
	MacDst[4] = gMAC_STA[4];
	MacDst[5] = AcmCmdUtilHexGet(&pArgv);

	/* get type, TID, & SizeData */
	Type = AcmCmdUtilNumGet(&pArgv);
	TSID = AcmCmdUtilNumGet(&pArgv);
	SizeData = AcmCmdUtilNumGet(&pArgv);

	if (SizeData < 5)
		SizeData = 5;
	/* End of if */

	/* get ACK policy */
	if (Argc != 6)
	{
		AckPolicy = AcmCmdUtilNumGet(&pArgv);
		SizeFrag = AcmCmdUtilNumGet(&pArgv);
		FlgIsRtsEnabled = AcmCmdUtilNumGet(&pArgv);
	} /* End of if */

	/* semaphore lock */
	ACM_DATA_SEM_LOCK(LabelSemErr);

	for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
	{
		if (gDATA_Sim[IdFlowNum].FlgIsValidEntry == 0)
		{
			memcpy(gDATA_Sim[IdFlowNum].MacSrc, MacSrc, 6);
			memcpy(gDATA_Sim[IdFlowNum].MacDst, MacDst, 6);

			gDATA_Sim[IdFlowNum].Direction = 0; /* receive */
			gDATA_Sim[IdFlowNum].Type = Type;
			gDATA_Sim[IdFlowNum].TID = TSID;
			gDATA_Sim[IdFlowNum].AckPolicy = AckPolicy;

			gDATA_Sim[IdFlowNum].FrameSize = SizeData;
			gDATA_Sim[IdFlowNum].NumSeq = 0;
			gDATA_Sim[IdFlowNum].NumFrag = 0;

			gDATA_Sim[IdFlowNum].FlgIsValidEntry = 1;
			break;
		} /* End of if */
	} /* End of for */

	if (IdFlowNum == ACM_MAX_NUM_OF_SIM_DATA_FLOW)
		printk("err> No any free entry can be added!\n");
	else
		gTaskDataSleep = 0;
	/* End of if */

	ACM_DATA_SEM_UNLOCK();

	/* start simulation timer */
	FlgIsTimerEnabled = 0;
	ACMR_TIMER_ENABLE(FlgIsTimerEnabled, gTimerSim, ACM_STREAM_CHECK_OFFSET);


LabelSemErr:
	return;
} /* End of AcmCmdSimDataRv */


/*
========================================================================
Routine Description:
	Continue to transmit packets from upper layer.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format: 54
		wssm_dattx [Dst Client MAC]
					[type:0-11e, 1-WME] [UP:0~7] [size] [ack: 0~1]
========================================================================
*/
VOID AcmCmdSimDataTx(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR MacDst[6];
	UINT32 Type, TID, SizeData;
	UINT32 AckPolicy;
	UINT32 IdFlowNum;
	UCHAR FlgIsTimerEnabled;
/*	ULONG SplFlags; */


	/* init */
	AckPolicy = 0;

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* active data task */
	if (gCmdFlgIsInit == 0)
	{
		/* invalid all flows */
		for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
			gDATA_Sim[IdFlowNum].FlgIsValidEntry = 0;
		/* End of for */

		/* init */
		gTaskDataSleep = 1;

		ACMR_TASK_INIT(pAd, gTaskletSim, ACM_CMD_Task_Data_Simulation, pAd, "ACM_SIM");

		gCmdFlgIsInit = 1;
	} /* End of if */

	/* get destination Client MAC */
	ACMR_MEM_ZERO(MacDst, sizeof(MacDst));
	AcmCmdUtilMacGet(&pArgv, MacDst);

	if (*(UINT32 *)MacDst == 0x00)
	{
		pArgv -= 3;
		MacDst[0] = gMAC_STA[0];
		MacDst[1] = gMAC_STA[1];
		MacDst[2] = gMAC_STA[2];
		MacDst[3] = gMAC_STA[3];
		MacDst[4] = gMAC_STA[4];
		MacDst[5] = AcmCmdUtilHexGet(&pArgv);

		if (MacDst[5] == 0x00)
			MacDst[5] = gMAC_STA[5];
		/* End of if */
	} /* End of if */

	/* get type, TID, & SizeData */
	ACM_NIN_DEC_MGET(Type, 0, 1, ("err> type fail!\n"));
	ACM_NIN_DEC_MGET(TID,  0, 7, ("err> TID fail!\n"));
	ACM_NIN_DEC_MGET(SizeData, 5, 1500, ("err> size fail!\n"));
	ACM_NIN_DEC_MGET(AckPolicy, 0, 1, ("err> ack policy fail!\n"));

	/* semaphore lock */
	ACM_DATA_SEM_LOCK(LabelErr);

	for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
	{
		if (gDATA_Sim[IdFlowNum].FlgIsValidEntry == 0)
		{
			memcpy(gDATA_Sim[IdFlowNum].MacSrc, gMAC_AP, 6);
			memcpy(gDATA_Sim[IdFlowNum].MacDst, MacDst, 6);

			gDATA_Sim[IdFlowNum].Direction = 1; /* transmission */
			gDATA_Sim[IdFlowNum].Type = Type;   /* 11e or WME */
			gDATA_Sim[IdFlowNum].TID = TID;     /* 0 ~ 7 */
			gDATA_Sim[IdFlowNum].AckPolicy = AckPolicy; /* Normal or No */

			gDATA_Sim[IdFlowNum].FrameSize = SizeData;
			gDATA_Sim[IdFlowNum].NumSeq = 0; /* no use */
			gDATA_Sim[IdFlowNum].NumFrag = 0; /* no use */

			/* when receive & fragment, size = fragement size */
//			if ((gDATA_Sim[i].Direction == 0) && (SizeFrag > 0))
//				gDATA_Sim[i].size = SizeFrag;
			/* End of if */

			gDATA_Sim[IdFlowNum].FlgIsValidEntry = 1;
			break;
		} /* End of if */
	} /* End of for */

	if (IdFlowNum == ACM_MAX_NUM_OF_SIM_DATA_FLOW)
		printk("err> No any free entry can be added!\n");
	else
		gTaskDataSleep = 0; /* wake up traffic simulation task */
	/* End of if */

	ACM_DATA_SEM_UNLOCK();

	/* start simulation timer */
	FlgIsTimerEnabled = 0;
	ACMR_TIMER_ENABLE(FlgIsTimerEnabled, gTimerSim, ACM_STREAM_CHECK_OFFSET);


LabelErr:
	return;
} /* End of AcmCmdSimDataTx */


/*
========================================================================
Routine Description:
	Stop to continue to send packets to a QSTA.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_stp (Client MAC)
========================================================================
*/
VOID AcmCmdSimDataStop(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UINT32 IdFlowNum;
	UCHAR FlgIsEnable;
/*	ULONG SplFlags; */


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* semaphore lock */
	ACM_DATA_SEM_LOCK(LabelSemErr);

	/* stop simulation timer */
	ACMR_TIMER_DISABLE(FlgIsEnable, gTimerSim);

	/* invalid all flows */
	for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
	    gDATA_Sim[IdFlowNum].FlgIsValidEntry = 0;
	/* End of for */

	ACM_DATA_SEM_UNLOCK();

LabelSemErr:
	gTaskDataSleep = 1;
} /* End of AcmCmdSimDataStop */


/*
========================================================================
Routine Description:
	Suspend to send packets.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
VOID AcmCmdSimDataSuspend(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	gTaskDataSleep = 1;
} /* End of AcmCmdSimDataSuspend */


/*
========================================================================
Routine Description:
	Resume to send packets.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
VOID AcmCmdSimDataResume(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	if (Argc == 2)
	{
		/* get delay from the extra field */
		gSimDelay = AcmCmdUtilNumGet(&pArgv);
	} /* End of if */

	gTaskDataSleep = 0;
} /* End of AcmCmdSimDataResume */


/*
========================================================================
Routine Description:
	Simulate a QoS re-association req event.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_reassoc (Client MAC) (UAPSD AC0) (UAPSD AC1) (UAPSD AC2)
		(UAPSD AC3)
========================================================================
*/
VOID AcmCmdSimReAssocBuild(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef CONFIG_AP_SUPPORT
	NDIS_STATUS		Status;
	HEADER_802_11	HdrAuthAssoc;

	PUCHAR	pBufOut;
	ULONG	LenFrame;
	USHORT	AssocCap, AssocLsn;
	UCHAR	SSID_EID, SSID_Len, SSID[MAX_LEN_OF_SSID];
	UCHAR	SUPR_EID, SUPR_Len, SupRate[4] = { 0x82, 0x84, 0x8B, 0x96 };
	UCHAR	EXTR_EID, EXTR_Len, ExtRate[4] = { 0xB0, 0xC8, 0xE0, 0xEC };
	UCHAR	IE_WME[9] = {IE_VENDOR_SPECIFIC, 0x07, 0x00, 0x50, 0xf2,
						0x02, 0x00, 0x01, 0x0F};
	UCHAR	DevMac[6], *pStaMac;
	UCHAR	UapsdAc[4];


	/* init */
	pBufOut = NULL;
	SSID_Len = pAd->ApCfg.MBSSID[0].SsidLen;
	ACMR_MEM_COPY(SSID, pAd->ApCfg.MBSSID[0].Ssid, SSID_Len+1);

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get client MAC */
	if (Argc >= 1)
	{
		DevMac[0] = gMAC_STA[0];
		DevMac[1] = gMAC_STA[1];
		DevMac[2] = gMAC_STA[2];
		DevMac[3] = gMAC_STA[3];
		DevMac[4] = gMAC_STA[4];
		DevMac[5] = AcmCmdUtilHexGet(&pArgv);
		pStaMac = DevMac;

		UapsdAc[0] = AcmCmdUtilNumGet(&pArgv);
		UapsdAc[1] = AcmCmdUtilNumGet(&pArgv);
		UapsdAc[2] = AcmCmdUtilNumGet(&pArgv);
		UapsdAc[3] = AcmCmdUtilNumGet(&pArgv);

		IE_WME[8] = (UapsdAc[0] << 3) |
					(UapsdAc[1] << 2) |
					(UapsdAc[2] << 1) |
					(UapsdAc[3]);
	}
	else
		pStaMac = gMAC_STA;
	/* End of if */

	/* allocate authentication request frame buffer */
	Status = MlmeAllocateMemory(pAd, &pBufOut);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		printk("11e_err> allocate auth buffer fail!\n");
		return;
	} /* End of if */

	/* init association frame body */
	MgtMacHeaderInit(pAd, &HdrAuthAssoc, SUBTYPE_REASSOC_REQ, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);

	AssocCap = 0x0001;
	AssocLsn = 1;
	SSID_EID = IE_SSID;
	SUPR_EID = IE_SUPP_RATES;
	SUPR_Len = 4;
	EXTR_EID = IE_EXT_SUPP_RATES;
	EXTR_Len = 4;

	MakeOutgoingFrame(pBufOut,				&LenFrame,
						sizeof(HEADER_802_11),	&HdrAuthAssoc,
						2,						&AssocCap,
						2,						&AssocLsn,
						6,						pAd->ApCfg.MBSSID[BSS0].Bssid,
						1,						&SSID_EID,
						1,						&SSID_Len,
						SSID_Len,				SSID,
						1,						&SUPR_EID,
						1,						&SUPR_Len,
						SUPR_Len,				SupRate,
						1,						&EXTR_EID,
						1,						&EXTR_Len,
						EXTR_Len,				ExtRate,
						9,						&IE_WME[0],
						END_OF_ARGS);

	/* send the virtual assoc frame to MLME handler */
	printk("11e_msg> simulate a re-assoc request...\n");

	REPORT_MGMT_FRAME_TO_MLME(pAd, 0, pBufOut, LenFrame, 0, 0, 0, 0);

	/* free the frame buffer */
	MlmeFreeMemory(pAd, pBufOut);
#endif // CONFIG_AP_SUPPORT //
} /* End of AcmCmdSimReAssocBuild */


/*
========================================================================
Routine Description:
	Simulate a non-QoS authentication req & association req event.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_assoc (Client MAC)
========================================================================
*/
VOID AcmCmdSimNonQoSAssocBuild(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef CONFIG_AP_SUPPORT
	NDIS_STATUS		Status;
	HEADER_802_11	HdrAuthAssoc;

	PUCHAR	pBufOut;
	ULONG	LenFrame;
	USHORT	AuthAlg, AuthSeq, AuthStatus;
	USHORT	AssocCap, AssocLsn;
	UCHAR	SSID_EID, SSID_Len, SSID[MAX_LEN_OF_SSID];
	UCHAR	SUPR_EID, SUPR_Len, SupRate[4] = { 0x82, 0x84, 0x8B, 0x96 };
	UCHAR	EXTR_EID, EXTR_Len, ExtRate[4] = { 0xB0, 0xC8, 0xE0, 0xEC };
	UCHAR	DevMac[6], *pStaMac;


	/* init */
	pBufOut = NULL;
	SSID_Len = pAd->ApCfg.MBSSID[0].SsidLen;
	ACMR_MEM_COPY(SSID, pAd->ApCfg.MBSSID[0].Ssid, SSID_Len+1);

	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get client MAC */
	if (Argc > 1)
	{
		DevMac[0] = gMAC_STA[0];
		DevMac[1] = gMAC_STA[1];
		DevMac[2] = gMAC_STA[2];
		DevMac[3] = gMAC_STA[3];
		DevMac[4] = gMAC_STA[4];
		DevMac[5] = AcmCmdUtilNumGet(&pArgv);
		pStaMac = DevMac;
	}
	else
		pStaMac = gMAC_STA;
    /* End of if */

	/* allocate authentication request frame buffer */
	Status = MlmeAllocateMemory(pAd, &pBufOut);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		printk("11e_err> allocate auth buffer fail!\n");
		return;
	} /* End of if */

	/* init authentication frame body */
	AuthAlg = Ndis802_11AuthModeOpen;
	AuthSeq = 1;
	AuthStatus = MLME_SUCCESS;

	MgtMacHeaderInit(pAd, &HdrAuthAssoc, SUBTYPE_AUTH, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);
	MakeOutgoingFrame(pBufOut,			&LenFrame,
					sizeof(HEADER_802_11),	&HdrAuthAssoc,
					2,						&AuthAlg,
					2,						&AuthSeq,
					2,						&AuthStatus,
					END_OF_ARGS);

	/* send the virtual auth frame to MLME handler */
	REPORT_MGMT_FRAME_TO_MLME(pAd, 0, pBufOut, LenFrame, 0, 0, 0, 0);

	/* init association frame body */
	MgtMacHeaderInit(pAd, &HdrAuthAssoc, SUBTYPE_ASSOC_REQ, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);

	AssocCap = 0x0001;
	AssocLsn = 1;
	SSID_EID = IE_SSID;
	SUPR_EID = IE_SUPP_RATES;
	SUPR_Len = 4;
	EXTR_EID = IE_EXT_SUPP_RATES;
	EXTR_Len = 4;

	MakeOutgoingFrame(pBufOut,				&LenFrame,
						sizeof(HEADER_802_11),	&HdrAuthAssoc,
						2,						&AssocCap,
						2,						&AssocLsn,
						1,						&SSID_EID,
						1,						&SSID_Len,
						SSID_Len,				SSID,
						1,						&SUPR_EID,
						1,						&SUPR_Len,
						SUPR_Len,				SupRate,
						1,						&EXTR_EID,
						1,						&EXTR_Len,
						EXTR_Len,				ExtRate,
						END_OF_ARGS);

	/* send the virtual assoc frame to MLME handler */
	REPORT_MGMT_FRAME_TO_MLME(pAd, 0, pBufOut, LenFrame, 0, 0, 0, 0);

	/* free the frame buffer */
	MlmeFreeMemory(pAd, pBufOut);
#endif // CONFIG_AP_SUPPORT //
} /* End of AcmCmdSimNonQoSAssocBuild */


/*
========================================================================
Routine Description:
	Continue to receive non-QoS packets from a QSTA.

Arguments:
	pAd			- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_ndatrv [Src Client MAC] [Dst Client MAC] [size]
========================================================================
*/
VOID AcmCmdSimNonQoSDataRv(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	UCHAR MacSrc[6], MacDst[6];
	UINT32 SizeData;
	UINT32 IdFlowNum;
	UCHAR FlgIsTimerEnabled;
/*	ULONG SplFlags; */


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* active data task */
	if (gCmdFlgIsInit == 0)
	{
		/* invalid all flows */
		for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
			gDATA_Sim[IdFlowNum].FlgIsValidEntry = 0;
		/* End of for */

		/* init */
		gTaskDataSleep = 1;

		ACMR_TASK_INIT(pAd, gTaskletSim, ACM_CMD_Task_Data_Simulation, pAd, "ACM_SIM");

		gCmdFlgIsInit = 1;
	} /* End of if */

	/* get source Client MAC */
	MacSrc[0] = gMAC_STA[0];
	MacSrc[1] = gMAC_STA[1];
	MacSrc[2] = gMAC_STA[2];
	MacSrc[3] = gMAC_STA[3];
	MacSrc[4] = gMAC_STA[4];
	MacSrc[5] = AcmCmdUtilHexGet(&pArgv);

	/* get destination Client MAC */
	MacDst[0] = gMAC_STA[0];
	MacDst[1] = gMAC_STA[1];
	MacDst[2] = gMAC_STA[2];
	MacDst[3] = gMAC_STA[3];
	MacDst[4] = gMAC_STA[4];
	MacDst[5] = AcmCmdUtilHexGet(&pArgv);

	/* get size */
	SizeData = AcmCmdUtilNumGet(&pArgv);

	if (SizeData < 5)
		SizeData = 5;
	/* End of if */

	/* semaphore lock */
	ACM_DATA_SEM_LOCK(LabelSemErr);

	for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
	{
		if (gDATA_Sim[IdFlowNum].FlgIsValidEntry == 0)
		{
			memcpy(gDATA_Sim[IdFlowNum].MacSrc, MacSrc, 6);
			memcpy(gDATA_Sim[IdFlowNum].MacDst, MacDst, 6);

			gDATA_Sim[IdFlowNum].Direction = 2; /* non-QoS receive */
			gDATA_Sim[IdFlowNum].Type = 0;
			gDATA_Sim[IdFlowNum].TID = 0;
			gDATA_Sim[IdFlowNum].AckPolicy = 0;

			gDATA_Sim[IdFlowNum].FrameSize = SizeData;
			gDATA_Sim[IdFlowNum].NumSeq = 0;
			gDATA_Sim[IdFlowNum].NumFrag = 0;

			gDATA_Sim[IdFlowNum].FlgIsValidEntry = 1;
			break;
		} /* End of if */
	} /* End of for */

	if (IdFlowNum == ACM_MAX_NUM_OF_SIM_DATA_FLOW)
		printk("err> No any free entry can be added!\n");
	else
		gTaskDataSleep = 0;
	/* End of if */

	ACM_DATA_SEM_UNLOCK();

	/* start simulation timer */
	FlgIsTimerEnabled = 0;
	ACMR_TIMER_ENABLE(FlgIsTimerEnabled, gTimerSim, ACM_STREAM_CHECK_OFFSET);


LabelSemErr:
	return;
} /* End of AcmCmdSimNonQoSDataRv */


/*
========================================================================
Routine Description:
	Fix the transmission rate for a client.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		wssm_rate [Dst Client MAC] [rate]
========================================================================
*/
static VOID AcmCmdSimRateSet(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACMR_STA_DB *pCdb;
	UCHAR DevMac[6], *pStaMac;
	UINT32 Rate;


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* set */
	DevMac[0] = AcmCmdUtilHexGet(&pArgv);
	DevMac[1] = AcmCmdUtilHexGet(&pArgv);
	DevMac[2] = AcmCmdUtilHexGet(&pArgv);
	DevMac[3] = AcmCmdUtilHexGet(&pArgv);
	DevMac[4] = AcmCmdUtilHexGet(&pArgv);
	DevMac[5] = AcmCmdUtilHexGet(&pArgv);
	pStaMac = DevMac;

	Rate = AcmCmdUtilNumGet(&pArgv);

	pCdb = ACMR_STA_ENTRY_GET(pAd, pStaMac);

	/* not support */
} /* End of AcmCmdSimRateSet */


/*
========================================================================
Routine Description:
	Enable or disable TCP packet transmitting.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
VOID AcmCmdSimTcpTxEnable(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	gSimTCPFlag = AcmCmdUtilNumGet(&pArgv);
	gSimTCPDSCP = AcmCmdUtilNumGet(&pArgv);
} /* End of AcmCmdSimTcpTxEnable */


/*
========================================================================
Routine Description:
	Reset the STATION MAC address.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Command Format:
		[Dst Client MAC]
========================================================================
*/
static VOID AcmCmdSimStaMacSet(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	gMAC_STA[0] = AcmCmdUtilHexGet(&pArgv);
	gMAC_STA[1] = AcmCmdUtilHexGet(&pArgv);
	gMAC_STA[2] = AcmCmdUtilHexGet(&pArgv);
	gMAC_STA[3] = AcmCmdUtilHexGet(&pArgv);
	gMAC_STA[4] = AcmCmdUtilHexGet(&pArgv);
	gMAC_STA[5] = AcmCmdUtilHexGet(&pArgv);

	printk("11e_msg> station mac = 0x%02x%02x%02x%02x%02x%02x\n",
			gMAC_STA[0], gMAC_STA[1], gMAC_STA[2],
			gMAC_STA[3], gMAC_STA[4], gMAC_STA[5]);
} /* End of AcmCmdSimStaMacSet */


#ifdef CONFIG_AP_SUPPORT
/*
========================================================================
Routine Description:
	Receive a trigger frame from a QSTA.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Used to test Power Save management frame handle.
	3. Command Format:
		[Client MAC]
========================================================================
*/
static VOID AcmCmdSimRcvTriFrame(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef UAPSD_AP_SUPPORT
	ACMR_STA_DB *pCdb;
	UCHAR MacPeer[6];


	AcmCmdUtilMacGet(&pArgv, MacPeer);

	if (*(UINT32 *)MacPeer == 0x00)
	{
		if (MacPeer[5] == 0x00)
			ACMR_MEM_COPY(MacPeer, gMAC_STA, 6);
		else
			ACMR_MEM_COPY(MacPeer, gMAC_STA, 5);
		/* End of if */
	} /* End of if */

	pCdb = ACMR_STA_ENTRY_GET(pAd, MacPeer);
	if (pCdb == NULL)
	{
		printk("11e_err> the station does NOT exist!\n");
		return;
	} /* End of if */

	UAPSD_TriggerFrameHandle(pAd, pCdb, 7);
#endif // UAPSD_AP_SUPPORT //
} /* End of AcmCmdSimRcvTriFrame */


/*
========================================================================
Routine Description:
	Simulate to enable/disable UAPSD queue maintain mechanism.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. For QAP.
	2. Used to test Power Save management frame handle.
	3. Command Format:
		[enable: 0~1]
========================================================================
*/
static VOID AcmCmdSimUapsdQueCtrl(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef UAPSD_AP_SUPPORT
	extern UCHAR gUAPSD_FlgNotQueueMaintain;


	gUAPSD_FlgNotQueueMaintain = AcmCmdUtilNumGet(&pArgv);
#endif // UAPSD_AP_SUPPORT //
} /* End of AcmCmdSimUapsdQueCtrlpAd */
#endif // CONFIG_AP_SUPPORT //




/*
========================================================================
Routine Description:
	Receive a packet with PM=1 from a station.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	1. Command Format: 70
		[mode:0(active)1(ps)] [MAC]
========================================================================
*/
static VOID AcmCmdSimWmePSEnter(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	ACMR_STA_DB *pCdb;
	UCHAR MacPeer[6];
	UINT32 PsMode;


	PsMode = AcmCmdUtilNumGet(&pArgv);
	AcmCmdUtilMacGet(&pArgv, MacPeer);

	if (*(UINT32 *)MacPeer == 0x00)
	{
		if (MacPeer[5] == 0x00)
			ACMR_MEM_COPY(MacPeer, gMAC_STA, 6);
		else
			ACMR_MEM_COPY(MacPeer, gMAC_STA, 5);
		/* End of if */
	} /* End of if */

	pCdb = ACMR_STA_ENTRY_GET(pAd, MacPeer);
	if (pCdb == NULL)
		return;
	/* End of if */

	if (PsMode == 1)
	{
		pCdb->PsMode = PWR_SAVE;

		printk("acm_msg> %02x:%02x:%02x:%02x:%02x:%02x enters PS mode!\n",
				MacPeer[0], MacPeer[1], MacPeer[2],
				MacPeer[3], MacPeer[4], MacPeer[5]);
	}
	else
	{
		pCdb->PsMode = PWR_ACTIVE;

		printk("acm_msg> %02x:%02x:%02x:%02x:%02x:%02x enters ACTIVE mode!\n",
				MacPeer[0], MacPeer[1], MacPeer[2],
				MacPeer[3], MacPeer[4], MacPeer[5]);
	} /* End of if */
} /* End of AcmCmdSimWmePSEnter */


/*
========================================================================
Routine Description:
	Simulate a ADDTS Request frame & PS Poll frame receive event.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	For QAP.

	[sta mac:xx:xx:xx:xx:xx:xx]
	[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
	[APSD:0~1] [nom size:byte] [inact:sec] [mean data rate:bps]
	[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
========================================================================
*/
static VOID AcmCmdSimReqPsPollRcv(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
#ifdef CONFIG_AP_SUPPORT
	ACM_STREAM_INFO	StreamInfo;
	ACMR_STA_DB	*pCdb = NULL;
	ACM_TSPEC	*pTspec = &StreamInfo.Tspec;
	ACM_TS_INFO	*pInfo = &pTspec->TsInfo;
	UCHAR StreamType;
	UCHAR MAC[6], *pStaMac = MAC;
	UCHAR TclasProcessing;

	NDIS_STATUS Status;
	HEADER_802_11 HdrAction;
	UCHAR *pBufOut = NULL;
	ULONG LenFrame;


	/* precondition */
	ACM_QOS_SANITY_CHECK(pAd);

	/* get sta mac address */
	ACMR_MEM_COPY(MAC, gMAC_STA, 6);

	MAC[5] = AcmCmdUtilHexGet(&pArgv);
	if (MAC[5] == 0x00)
		ACMR_MEM_COPY(MAC, gMAC_STA, 6);
	/* End of if */

	/* get sta entry */
	pCdb = ACMR_STA_ENTRY_GET(pAd, MAC);
	if (pCdb == NULL)
	{
		printk("11e_err> the station does NOT exist!\n");
		return;
	} /* End of if */

	/* parse input command */
	if (AcmCmdInfoParse(
							pAd,
							&pArgv,
							pTspec,
							pInfo,
							&StreamType) != 0)
	{
		return;
	} /* End of if */

	TclasProcessing = AcmCmdUtilNumGet(&pArgv);

	/* vitual send a ADDTS request frame to our AP */
	Status = MlmeAllocateMemory(pAd, &pBufOut);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		printk("11e_err> allocate action buffer fail!\n");
		return;
	} /* End of if */

	/* init ADDTS request frame body */
	MgtMacHeaderInit(pAd, &HdrAction, SUBTYPE_ACTION, 0,
					pAd->ApCfg.MBSSID[BSS0].Bssid, pStaMac);
	MakeOutgoingFrame(pBufOut,			&LenFrame,
					sizeof(HEADER_802_11),	&HdrAction,
					END_OF_ARGS);

	LenFrame += ACM_CMD_WME_Action_Make(pAd, &StreamInfo,
										&pBufOut[LenFrame],
										ACM_ACTION_WME_SETUP_REQ,
										0,
										TclasProcessing);

	/* send the ADDTS request frame to ACM module */
	ACMP_ManagementHandle(pAd, pCdb, ACMR_SUBTYPE_ACTION,
							pBufOut, LenFrame, 11000000);

	/* free the frame buffer */
	MlmeFreeMemory(pAd, pBufOut);

	/* simulate a PS Poll frame is received */
	mdelay(500);
	APHandleRxPsPoll(pAd, MAC, pCdb->Aid, FALSE);
#endif // CONFIG_AP_SUPPORT //
} /* End of AcmCmdSimReqPsPollRcv */

#endif // IEEE80211E_SIMULATION //




/* =========================== Utility Function ========================== */
/*
========================================================================
Routine Description:
	Get argument number value.

Arguments:
	**ppArgv			- input parameters

Return Value:
	decimal number

Note:
========================================================================
*/
static UINT32 AcmCmdUtilHexGet(
	ACM_PARAM_IN	CHAR	**ppArgv)
{
	CHAR buf[3], *pNum;
	UINT32 ID;
	UCHAR Value;


	pNum = (*ppArgv);

	buf[0] = 0x30;
	buf[1] = 0x30;
	buf[2] = 0;

	for(ID=0; ID<sizeof(buf)-1; ID++)
	{
		if ((*pNum == '_') || (*pNum == 0x00))
			break;
		/* End of if */

		pNum ++;
	} /* End of for */

	if (ID == 0)
		return 0; /* argument length is too small */
	/* End of if */

	if (ID >= 2)
		memcpy(buf, (*ppArgv), 2);
	else
		buf[1] = (**ppArgv);
	/* End of if */

	(*ppArgv) += ID;
	if ((**ppArgv) == '_')
		(*ppArgv) ++; /* skip _ */
	/* End of if */

	ACMR_ARG_ATOH(buf, &Value);
	return (UINT32)Value;
} /* End of AcmCmdUtilHexGet */


/*
========================================================================
Routine Description:
	Get argument number value.

Arguments:
	*pArgv			- input parameters

Return Value:
	decimal number

Note:
========================================================================
*/
static UINT32 AcmCmdUtilNumGet(
	ACM_PARAM_IN	CHAR	**ppArgv)
{
	CHAR buf[20], *pNum;
	UINT32 ID;


	pNum = (*ppArgv);

	for(ID=0; ID<sizeof(buf)-1; ID++)
	{
		if ((*pNum == '_') || (*pNum == 0x00))
			break;
		/* End of if */

		pNum ++;
	} /* End of for */

	if (ID == sizeof(buf)-1)
		return 0; /* argument length is too large */
	/* End of if */

	memcpy(buf, (*ppArgv), ID);
	buf[ID] = 0x00;

	*ppArgv += ID+1; /* skip _ */

	return ACMR_ARG_ATOI(buf);
} /* End of AcmCmdUtilNumGet */


/*
========================================================================
Routine Description:
	Get argument MAC value.

Arguments:
	**ppArgv			- input parameters
	*pDevMac			- MAC address

Return Value:
	None

Note:
========================================================================
*/
static VOID AcmCmdUtilMacGet(
	ACM_PARAM_IN	CHAR	**ppArgv,
	ACM_PARAM_IN	UCHAR	*pDevMac)
{
	CHAR Buf[3];
	CHAR *pMAC = (CHAR *)(*ppArgv);
	UINT32 ID;


	if ((pMAC[0] == '0') && (pMAC[1] == '_'))
	{
		*ppArgv = (&pMAC[2]);
		return;
	} /* End of if */

	ACMR_MEM_ZERO(pDevMac, 6);

	/* must exist 18 octets */
	for(ID=0; ID<18; ID+=2)
	{
		if ((pMAC[ID] == '_') || (pMAC[ID] == 0x00))
		{
			*ppArgv = (&pMAC[ID]+1);
			return;
		} /* End of if */
	} /* End of for */

	/* get mac */
	for(ID=0; ID<18; ID+=3)
	{
		Buf[0] = pMAC[0];
		Buf[1] = pMAC[1];
		Buf[2] = 0x00;

		ACMR_ARG_ATOH(Buf, pDevMac);
		pMAC += 3;
		pDevMac ++;
	} /* End of for */

	*ppArgv += 17+1; /* skip _ */
} /* End of AcmCmdUtilMacGet */


/*
========================================================================
Routine Description:
	Display the stream status.

Arguments:
	pAd				- WLAN control block pointer
	*pStream		- the stream

Return Value:
	None

Note:
	For QAP & QSTA.
========================================================================
*/
static VOID AcmCmdStreamDisplayOne(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	ACM_STREAM_INFO		*pStream)
{
	ACM_TSPEC *pTspec;
	UINT16 SBA_Temp;


	pTspec = &pStream->Tspec;

	printk("\n=== QSTA MAC = %02x:%02x:%02x:%02x:%02x:%02x",
			pStream->DevMac[0],
			pStream->DevMac[1],
			pStream->DevMac[2],
			pStream->DevMac[3],
			pStream->DevMac[4],
			pStream->DevMac[5]);

	if (ACMR_CB->EdcaCtrlParam.FlgAcmStatus[pStream->AcmAcId])
		printk(" (NORMAL TSPEC)\n");
	else
		printk(" (NULL TSPEC)\n");
	/* End of if */

	if (pTspec->TsInfo.AccessPolicy == ACM_ACCESS_POLICY_EDCA)
	{
		if (pStream->StreamType == ACM_STREAM_TYPE_11E)
			printk("    Stream Type: EDCA");
		else
			printk("    Stream Type: WME");
		/* End of if */
	} /* End of if */

	switch(pStream->Status)
	{
		case TSPEC_STATUS_REQUEST:
			printk("\tStatus: Requesting...\n");
			break;

		case TSPEC_STATUS_ACTIVE:
			printk("\tStatus: Active\n");
			break;

		case TSPEC_STATUS_ACTIVE_SUSPENSION:
			printk("\tStatus: Active but suspended\n");
			break;

		case TSPEC_STATUS_REQ_DELETING:
			printk("\tStatus: Requesting & deleting...\n");
			break;

		case TSPEC_STATUS_ACT_DELETING:
			printk("\tStatus: Active & deleting...\n");
			break;

		case TSPEC_STATUS_RENEGOTIATING:
			printk("\tStatus: Renegotiation...\n");
			break;

		case TSPEC_STATUS_HANDLING:
			printk("\tStatus: Request Handling...\n");
			break;

		case TSPEC_STATUS_FAIL:
			switch(pStream->Cause)
			{
				case TSPEC_CAUSE_UNKNOWN:
					printk("\tStatus: (ERR) Internal Error!\n");
					break;

				case TSPEC_CAUSE_REQ_TIMEOUT:
					printk("\tStatus: (ERR) Request (ADDTS) timeout!\n");
					break;

				case TSPEC_CAUSE_SUGGESTED_TSPEC:
					printk("\tStatus: (ERR) Suggested TSPEC is provided!\n");
					break;

				case TSPEC_CAUSE_REJECTED:
					printk("\tStatus: (ERR) Rejected by QAP!\n");
					break;

				case TSPEC_CAUSE_UNKNOWN_STATUS:
					printk("\tStatus: (ERR) Unknown response status code!\n");
					break;

				case TSPEC_CAUSE_INACTIVITY_TIMEOUT:
					printk("\tStatus: (ERR) Inactivity timeout!\n");
					break;

				case TSPEC_CAUSE_DELETED_BY_QAP:
					printk("\tStatus: (ERR) Deleted by QAP!\n");
					break;

				case TSPEC_CAUSE_DELETED_BY_QSTA:
					printk("\tStatus: (ERR) Deleted by QSTA!\n");
					break;

				case TSPEC_CAUSE_BANDWIDTH:
					printk("\tStatus: (ERR) In order to increase bandwidth!\n");
					break;

				case TSPEC_CAUSE_REJ_MANY_TS:
					printk("\tStatus: (ERR) Reject due to too many TS in a AC!\n");
					break;

				case TSPEC_CASUE_REJ_INVALID_PARAM:
					printk("\tStatus: (ERR) Reject due to invalid parameters!\n");
					break;

				case TSPEC_CAUSE_REJ_INVALID_TOKEN:
					printk("\tStatus: (ERR) Reject due to invalid Dialog Token!\n");
					break;

				default:
					printk("\tStatus: Fatal error, unknown cause!\n");
					break;
			} /* End of switch */
			break;

		default:
			printk("\tStatus: Fatal error, unknown status!\n");
			break;
	} /* End of switch */

	printk("    TSID = %d", pTspec->TsInfo.TSID);

	printk("\tUP = %d", pTspec->TsInfo.UP);
	printk("\tAC ID = %d", pStream->AcmAcId);
	printk("\tUAPSD = %d", pTspec->TsInfo.APSD);

	switch(pTspec->TsInfo.Direction)
	{
		case ACM_DIRECTION_UP_LINK:
			printk("\tDirection = UP LINK\n");
			break;

		case ACM_DIRECTION_DOWN_LINK:
			printk("\tDirection = DOWN LINK\n");
			break;

		case ACM_DIRECTION_DIRECT_LINK:
			printk("\tDirection = DIRECT LINK\n");
			break;

		case ACM_DIRECTION_BIDIREC_LINK:
			printk("\tDirection = BIDIRECTIONAL LINK\n");
			break;
	} /* End of switch */

	if (ACMR_CB->EdcaCtrlParam.FlgAcmStatus[pStream->AcmAcId])
		printk("    Current Inactivity timeout = %u us\n", pStream->InactivityCur);
	else
		printk("    No Inactivity timeout!\n");
	/* End of if */

	if (pTspec->NominalMsduSize & ACM_NOM_MSDU_SIZE_CHECK_BIT)
	{
		printk("    Norminal MSDU Size (Fixed) = %d B\n",
				(pTspec->NominalMsduSize & (~ACM_NOM_MSDU_SIZE_CHECK_BIT)));
	}
	else
	{
		printk("    Norminal MSDU Size (Variable) = %d B\n",
			(pTspec->NominalMsduSize & (~ACM_NOM_MSDU_SIZE_CHECK_BIT)));
	} /* End of if */

	printk("    Inactivity Interval = %u us\n", pTspec->InactivityInt);


	printk("    Mean Data Rate = %d bps\n", pTspec->MeanDataRate);
	printk("    Min Physical Rate = %d bps (%d %d)\n",
			pTspec->MinPhyRate, pStream->PhyModeMin, pStream->McsMin);

	if (pTspec->TsInfo.AccessPolicy != ACM_ACCESS_POLICY_HCCA)
	{
		/* only for EDCA or HCCA + EDCA */
		SBA_Temp = pTspec->SurplusBandwidthAllowance;
		SBA_Temp = (UINT16)(SBA_Temp << ACM_SURPLUS_INT_BIT_NUM);
		SBA_Temp = (UINT16)(SBA_Temp >> ACM_SURPLUS_INT_BIT_NUM);
		SBA_Temp = ACM_SurplusFactorDecimalBin2Dec(SBA_Temp);

		printk("    Surplus factor = %d.%02d",
			(pTspec->SurplusBandwidthAllowance >> ACM_SURPLUS_DEC_BIT_NUM),
			SBA_Temp);

		printk("\t\tMedium Time = %d us\n", (pTspec->MediumTime << 5));
	} /* End of if */
} /* End of AcmCmdStreamDisplayOne */


/*
========================================================================
Routine Description:
	Parse WME input parameters.

Arguments:
	pAd				- WLAN control block pointer
	**ppArgv		- input parameters
	*pTspec			- the output TSPEC
	*pInfo			- the output TS Info
	*pStreamType	- the stream type
	*pTclasProcessing - the TCLAS Processing

Return Value:
	0				- parse successfully
	non 0			- parse fail

Note:
	For QAP & QSTA.

	[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
	[APSD:0~1] [nom size:byte] [inact:sec] [mean data rate:bps]
	[min phy rate:Mbps] [surp factor:>=1]

	where SBA is in 10 ~ 80, i.e. 1.0 ~ 8.0
========================================================================
*/
UCHAR AcmCmdInfoParse(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	CHAR				**ppArgv,
	ACM_PARAM_IN	ACM_TSPEC			*pTspec,
	ACM_PARAM_IN	ACM_TS_INFO			*pInfo,
	ACM_PARAM_IN	UCHAR				*pStreamType)
{
	UINT32 SBA_Integer, SBA_Decimal;
	CHAR *pArgv;


	/* init */
	pArgv = *ppArgv;

	/* get stream type: 11e or WME/WSM */
	ACM_NIN_DEC_GET((*pStreamType), 1, ("err> error type!\n"));

	/* init TS Info */
	pInfo->TrafficType = ACM_TRAFFIC_TYPE_APERIODIC;

	ACM_NIN_DEC_GET(pInfo->TSID, 7, ("err> error TID!\n"));
	ACM_NIN_DEC_GET(pInfo->Direction, 3, ("err> error dir!\n"));
	ACM_NIN_DEC_MGET(pInfo->AccessPolicy, 1, 3, ("err> error access!\n"));

	pInfo->Aggregation = ACM_AGGREGATION_DISABLE;

	ACM_NIN_DEC_GET(pInfo->UP, 7, ("err> error UP!\n"));
	pInfo->AckPolicy = ACM_ACK_POLICY_NORMAL;
	ACM_NIN_DEC_GET(pInfo->APSD, 1, ("err> error APSD!\n"));
	pInfo->Schedule = ACM_SCHEDULE_NO;

	ACMR_DEBUG(ACMR_DEBUG_TRACE,
				("acm_msg> TSInfo : "
				"TID=%d, DIR=%d, ACCESS=%d, UP=%d, APSD=%d\n",
				pInfo->TSID,
				pInfo->Direction,
				pInfo->AccessPolicy,
				pInfo->UP,
				pInfo->APSD));

	/* init TSPEC */
	pTspec->NominalMsduSize = AcmCmdUtilNumGet(&pArgv);
 	pTspec->MaxMsduSize = 0;
	pTspec->MinServInt = 0;
	pTspec->MaxServInt = 0;
	ACM_NIN_DEC_GET(pTspec->InactivityInt, 3600, ("err> error inact!\n"));
	pTspec->InactivityInt *= 1000000; /* unit: microseconds */
	pTspec->SuspensionInt = 0; /* use 0 in EDCA mode, not 0xFFFFFFFF */
	pTspec->ServiceStartTime = 0;
	ACM_NIN_DEC_GET(pTspec->MeanDataRate, ACM_RATE_MAX, ("err> error mean rate!\n"));
	pTspec->MinDataRate = pTspec->MeanDataRate;
	pTspec->PeakDataRate = pTspec->MeanDataRate;
	pTspec->MaxBurstSize = 0;
	pTspec->DelayBound = 0;
	ACM_NIN_DEC_GET(pTspec->MinPhyRate, ACM_RATE_MAX, ("err> error phy rate!\n"));
	ACM_NIN_DEC_MGET(pTspec->SurplusBandwidthAllowance, 10, 80,
					("err> error surp factor!\n"));
	if (pTspec->SurplusBandwidthAllowance < 80)
	{
		SBA_Integer = pTspec->SurplusBandwidthAllowance / 10;
		SBA_Decimal = pTspec->SurplusBandwidthAllowance - SBA_Integer*10;
		SBA_Decimal = ACM_SurplusFactorDecimalDec2Bin(SBA_Decimal*10);
		pTspec->SurplusBandwidthAllowance = \
					(SBA_Integer << ACM_SURPLUS_DEC_BIT_NUM) | SBA_Decimal;
	}
	else
		pTspec->SurplusBandwidthAllowance = 0xffff;
	/* End of if */
	pTspec->MediumTime = 0;

	ACMR_DEBUG(ACMR_DEBUG_TRACE,
				("acm_msg> TSPEC : "
				"NOM SIZE=%d, INACT=%d, MEAN(MIN/MAX) RATE=%d, MIN PHY=%d, "
				"SBA=0x%x\n",
				pTspec->NominalMsduSize,
				pTspec->InactivityInt,
				pTspec->MeanDataRate,
				pTspec->MinPhyRate,
				pTspec->SurplusBandwidthAllowance));

	*ppArgv = pArgv;
	return 0;

LabelErr:
	return 1;
} /* End of AcmCmdInfoParse */


/*
========================================================================
Routine Description:
	Parse WME input parameters.

Arguments:
	pAd				- WLAN control block pointer
	**ppArgv		- input parameters
	*pTspec			- the output TSPEC
	*pInfo			- the output TS Info
	*pStreamType	- the stream type
	*pTclasProcessing - the TCLAS Processing

Return Value:
	0				- parse successfully
	non 0			- parse fail

Note:
	For QAP & QSTA.

	[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
	[Ack Policy: 0~3] [APSD:0~1] [max size:byte] [nom size:byte]
	[burst size:byte] [inact:sec]
	[peak data rate:bps] [mean data rate:bps] [min data rate:bps]
	[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]

	where SBA is in 100 ~ 800, i.e. 1.00 ~ 8.00
========================================================================
*/
UCHAR AcmCmdInfoParseAdvance(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	CHAR				**ppArgv,
	ACM_PARAM_IN	ACM_TSPEC			*pTspec,
	ACM_PARAM_IN	ACM_TS_INFO			*pInfo,
	ACM_PARAM_IN	UCHAR				*pStreamType)
{
	UINT32 SBA_Integer, SBA_Decimal;
	CHAR *pArgv;


	/* init */
	pArgv = *ppArgv;

	/* get stream type: 11e or WME/WSM */
	ACM_NIN_DEC_GET((*pStreamType), 1, ("err> error type!\n"));

	/* init TS Info */
	pInfo->TrafficType = ACM_TRAFFIC_TYPE_APERIODIC;

	ACM_NIN_DEC_GET(pInfo->TSID, 7, ("err> error TID!\n"));
	ACM_NIN_DEC_GET(pInfo->Direction, 3, ("err> error dir!\n"));
	ACM_NIN_DEC_MGET(pInfo->AccessPolicy, 1, 3, ("err> error access!\n"));

	pInfo->Aggregation = ACM_AGGREGATION_DISABLE;

	ACM_NIN_DEC_GET(pInfo->UP, 7, ("err> error UP!\n"));
	ACM_NIN_DEC_GET(pInfo->AckPolicy, 3, ("err> error Ack Policy!\n"));
	ACM_NIN_DEC_GET(pInfo->APSD, 1, ("err> error APSD!\n"));
	pInfo->Schedule = ACM_SCHEDULE_NO;

	ACMR_DEBUG(ACMR_DEBUG_TRACE,
				("acm_msg> TSInfo : "
				"TID=%d, DIR=%d, ACCESS=%d, UP=%d, APSD=%d\n",
				pInfo->TSID,
				pInfo->Direction,
				pInfo->AccessPolicy,
				pInfo->UP,
				pInfo->APSD));

	/* init TSPEC */
 	pTspec->MaxMsduSize = AcmCmdUtilNumGet(&pArgv);
	pTspec->NominalMsduSize = AcmCmdUtilNumGet(&pArgv);
	pTspec->MaxBurstSize = AcmCmdUtilNumGet(&pArgv);
	pTspec->MinServInt = 0;
	pTspec->MaxServInt = 0;
	ACM_NIN_DEC_GET(pTspec->InactivityInt, 3600, ("err> error inact!\n"));
	pTspec->InactivityInt *= 1000000; /* unit: microseconds */
	pTspec->SuspensionInt = 0; /* use 0 in EDCA mode, not 0xFFFFFFFF */
	pTspec->ServiceStartTime = 0;
	ACM_NIN_DEC_GET(pTspec->PeakDataRate, ACM_RATE_MAX, ("err> error peak rate!\n"));
	ACM_NIN_DEC_GET(pTspec->MeanDataRate, ACM_RATE_MAX, ("err> error mean rate!\n"));
	ACM_NIN_DEC_GET(pTspec->MinDataRate, ACM_RATE_MAX, ("err> error min rate!\n"));
	pTspec->DelayBound = 0;
	ACM_NIN_DEC_GET(pTspec->MinPhyRate, ACM_RATE_MAX, ("err> error phy rate!\n"));
	ACM_NIN_DEC_MGET(pTspec->SurplusBandwidthAllowance, 100, 800,
					("err> error surp factor!\n"));
	if (pTspec->SurplusBandwidthAllowance < 800)
	{
		SBA_Integer = pTspec->SurplusBandwidthAllowance / 100;
		SBA_Decimal = pTspec->SurplusBandwidthAllowance - SBA_Integer*100;
		SBA_Decimal = ACM_SurplusFactorDecimalDec2Bin(SBA_Decimal);
		pTspec->SurplusBandwidthAllowance = \
					(SBA_Integer << ACM_SURPLUS_DEC_BIT_NUM) | SBA_Decimal;
	}
	else
		pTspec->SurplusBandwidthAllowance = 0xffff;
	/* End of if */
	pTspec->MediumTime = 0;

	ACMR_DEBUG(ACMR_DEBUG_TRACE,
				("acm_msg> TSPEC : "
				"MAX/NOM SIZE=%d %d, INACT=%d, PEAK/MEAN/MIN RATE=%d %d %d, MIN PHY=%d, "
				"SBA=0x%x\n",
				pTspec->MaxMsduSize,
				pTspec->NominalMsduSize,
				pTspec->InactivityInt,
				pTspec->PeakDataRate,
				pTspec->MeanDataRate,
				pTspec->MinDataRate,
				pTspec->MinPhyRate,
				pTspec->SurplusBandwidthAllowance));

	*ppArgv = pArgv;
	return 0;

LabelErr:
	return 1;
} /* End of AcmCmdInfoParseAdvance */


#ifdef IEEE80211E_SIMULATION


#ifdef CONFIG_AP_SUPPORT
/*
========================================================================
Routine Description:
	Make a WME action frame body.

Arguments:
	pAd				- WLAN control block pointer
	*pStream		- the stream
	*pPkt			- the frame body pointer
	Action			- action
	StatusCode		- status code, used when action = response
	TclasProcessing - the TCLAS PROCESSING value

Return Value:
	ACM_RTN_OK		- insert ok
	ACM_RTN_FAIL	- insert fail

Note:
========================================================================
*/
static UINT32 ACM_CMD_WME_Action_Make(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	ACM_STREAM_INFO		*pStream,
	ACM_PARAM_IN	UCHAR				*pPkt,
	ACM_PARAM_IN	UCHAR				Action,
	ACM_PARAM_IN	UCHAR				StatusCode,
	ACM_PARAM_IN	UCHAR				TclasProcessing)
{
	ACM_WME_NOT_FRAME *pFrameBody;
	ACM_ELM_WME_TSPEC *pElmTspec;
	ACM_WME_TS_INFO *pInfo;
	ACM_WME_TSPEC *pTspec;
	ACM_ELM_WME_TCLAS *pElmTclas;
	ACM_ELM_WME_TCLAS_PROCESSING *pElmTclasProcessing;
	UINT32 BodyLen, IdTclasNum;


	/* init */
	pFrameBody = (ACM_WME_NOT_FRAME *)pPkt;

	/* sanity check for type */
	if (Action > ACM_ACTION_WME_TEAR_DOWN)
		return 0;
	/* End of if */

	/* init frame body */
	pFrameBody->Category = ACM_CATEGORY_WME;
	pFrameBody->Action   = Action;

	if (gDialogToken == 0)
		gDialogToken ++; /* can not be 0 */
	/* End of if */

	if (Action != ACM_ACTION_WME_TEAR_DOWN)
		pFrameBody->DialogToken = gDialogToken++;
	else
		pFrameBody->DialogToken = 0;
	/* End of if */

	pFrameBody->StatusCode = StatusCode;
	BodyLen = 4;

	/* TSPEC element */
	pElmTspec = &pFrameBody->ElmTspec;
	pElmTspec->ElementId = ACM_ELM_WME_ID;
	pElmTspec->Length = ACM_ELM_WME_TSPEC_LEN;

	/* init OUI field */
	pElmTspec->OUI[0] = ACM_WME_OUI_0;
	pElmTspec->OUI[1] = ACM_WME_OUI_1;
	pElmTspec->OUI[2] = ACM_WME_OUI_2;
	pElmTspec->OUI_Type = ACM_WME_OUI_TYPE;
	pElmTspec->OUI_SubType = ACM_WME_OUI_SUBTYPE_TSPEC;
	pElmTspec->Version = ACM_WME_OUI_VERSION;

	/* init TS Info field */
	pTspec = &pFrameBody->ElmTspec.Tspec;
	ACMR_MEM_ZERO(pTspec, sizeof(ACM_WME_TSPEC));
	pInfo = &pFrameBody->ElmTspec.Tspec.TsInfo;

	pInfo->UP = pStream->Tspec.TsInfo.UP;
	pInfo->Direction = pStream->Tspec.TsInfo.Direction;
	pInfo->TID = pStream->Tspec.TsInfo.TSID;
	pInfo->PSB = pStream->Tspec.TsInfo.APSD;
	pInfo->One = 1;

#ifdef ACM_CC_FUNC_11N
	pInfo->Reserved2 = pStream->Tspec.TsInfo.AckPolicy;
#endif // ACM_CC_FUNC_11N //

	/* init TSPEC parameters */
	pTspec->NominalMsduSize = pStream->Tspec.NominalMsduSize;
	pTspec->InactivityInt = pStream->Tspec.InactivityInt;
	pTspec->SuspensionInt = pStream->Tspec.SuspensionInt;
	pTspec->MeanDataRate = pStream->Tspec.MeanDataRate;
	pTspec->MinPhyRate = pStream->Tspec.MinPhyRate;
	pTspec->SurplusBandwidthAllowance = \
							pStream->Tspec.SurplusBandwidthAllowance;

	BodyLen += (2+pElmTspec->Length);

	/* TCLAS element */
	pElmTclas = (ACM_ELM_WME_TCLAS *)pFrameBody->Tclas;

	/* init OUI field */
	pElmTclas->OUI[0] = ACM_WME_OUI_0;
	pElmTclas->OUI[1] = ACM_WME_OUI_1;
	pElmTclas->OUI[2] = ACM_WME_OUI_2;
	pElmTclas->OUI_Type = ACM_WME_OUI_TYPE;
	pElmTclas->OUI_SubType = ACM_WSM_OUI_SUBTYPE_TCLAS;
	pElmTclas->Version = ACM_WME_OUI_VERSION;

	/* init TCLAS parameters */
	for(IdTclasNum=0; IdTclasNum<gTLS_Grp_ID; IdTclasNum++)
	{
		pElmTclas->ElementId = ACM_ELM_WME_ID;

		switch(gCMD_TCLAS_Group[IdTclasNum].ClassifierType)
		{
			case ACM_TCLAS_TYPE_ETHERNET:
				pElmTclas->Length = ACM_TCLAS_TYPE_WME_ETHERNET_LEN;
				break;

			case ACM_TCLAS_TYPE_IP_V4:
				pElmTclas->Length = ACM_TCLAS_TYPE_WME_IP_V4_LEN;
				break;

			case ACM_TCLAS_TYPE_8021DQ:
				pElmTclas->Length = ACM_TCLAS_TYPE_WME_8021DQ_LEN;
				break;
		} /* End of switch */

		memcpy(&pElmTclas->Tclas,
				&gCMD_TCLAS_Group[IdTclasNum],
				pElmTclas->Length);

		BodyLen += (2+pElmTclas->Length);

		pElmTclas = (ACM_ELM_WME_TCLAS *)\
						((UCHAR *)pElmTclas + (2 + pElmTclas->Length));
	} /* End of for */

	/* TCLAS Processing element */
	if (gTLS_Grp_ID > 0)
	{
		pElmTclasProcessing = (ACM_ELM_WME_TCLAS_PROCESSING *)pElmTclas;

		/* init OUI field */
		pElmTclasProcessing->OUI[0] = ACM_WME_OUI_0;
		pElmTclasProcessing->OUI[1] = ACM_WME_OUI_1;
		pElmTclasProcessing->OUI[2] = ACM_WME_OUI_2;
		pElmTclasProcessing->OUI_Type = ACM_WME_OUI_TYPE;
		pElmTclasProcessing->OUI_SubType = ACM_WSM_OUI_SUBTYPE_TCLAS_PROCESSING;
		pElmTclasProcessing->Version = ACM_WME_OUI_VERSION;

		/* init TCLAS Processing parameters */
		pElmTclasProcessing->ElementId = ACM_ELM_WME_ID;
		pElmTclasProcessing->Length = ACM_ELM_WME_TCLAS_PROCESSING_LEN;
		pElmTclasProcessing->Processing = TclasProcessing;
		BodyLen += (2+pElmTclasProcessing->Length);
	} /* End of if */

	return BodyLen;
} /* End of ACM_CMD_WME_Action_Make */
#endif // CONFIG_AP_SUPPORT //


/*
========================================================================
Routine Description:
	QoS Data simulation task.

Arguments:
	Data			- WLAN control block pointer

Return Value:
	None

Note:
	For QAP.
========================================================================
*/
static VOID ACM_CMD_Task_Data_Simulation(ULONG Data)
{
	ACMR_PWLAN_STRUC pAd = (ACMR_PWLAN_STRUC)Data;
	UINT32 IdFlowNum;


	if (gTaskDataSleep == 1)
		goto LabelExit; /* sleeping, nothing to do */
	/* End of while */

	if (gSimDelayCount == 0)
	{
		for(IdFlowNum=0; IdFlowNum<ACM_MAX_NUM_OF_SIM_DATA_FLOW; IdFlowNum++)
		{
			if (gDATA_Sim[IdFlowNum].FlgIsValidEntry == 1)
			{
				switch(gDATA_Sim[IdFlowNum].Direction)
				{
					case 0: /* QoS receive */
						ACM_CMD_Sim_Data_Rv(pAd, &gDATA_Sim[IdFlowNum]);
						break;

					case 1: /* QoS transmission */
						ACM_CMD_Sim_Data_Tx(pAd, &gDATA_Sim[IdFlowNum]);
						break;

					case 2: /* non-QoS receive */
						ACM_CMD_Sim_nonQoS_Data_Rv(pAd, &gDATA_Sim[IdFlowNum]);
						break;
				} /* End of switch */
			} /* End of if */
		} /* End of for */

		gSimDelayCount = gSimDelay;
	}
	else
		gSimDelayCount --;
	/* End of if */

LabelExit:
#ifdef ACMR_HANDLE_IN_TIMER
	ACMR_TIMER_ENABLE(FlgIsTimerEnabled, gTimerSim, ACM_STREAM_CHECK_OFFSET);
#endif // ACMR_HANDLE_IN_TIMER //
} /* End of ACM_CMD_Task_Data_Simulation */


/*
========================================================================
Routine Description:
	Simulation periodical timer.

Arguments:
	Data			- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID ACM_CMD_Timer_Data_Simulation(ULONG Data)
{
#ifndef ACMR_HANDLE_IN_TIMER
	ACMR_TASK_ACTIVATE(gTaskletSim, gTimerSim, 10); /* 100ms */

#else

	ACM_CMD_Task_Data_Simulation(Data);
#endif // ACMR_HANDLE_IN_TIMER //
} /* End of ACM_CMD_Timer_Data_Simulation */


/*
========================================================================
Routine Description:
	Send a QoS data frame to the QAP.

Arguments:
	pAd				- WLAN control block pointer
	*pInfo			- the data flow information

Return Value:
	None

Note:
	For QAP.
========================================================================
*/
static VOID ACM_CMD_Sim_Data_Rv(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	ACM_DATA_SIM		*pInfo)
{
	/* not support */
} /* End of ACM_CMD_Sim_Data_Rv */


/*
========================================================================
Routine Description:
	Send a non-QoS data frame to the QAP.

Arguments:
	pAd				- WLAN control block pointer
	*pInfo			- the data flow information

Return Value:
	None

Note:
	For QAP.
========================================================================
*/
static VOID ACM_CMD_Sim_nonQoS_Data_Rv(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	ACM_DATA_SIM		*pInfo)
{
	/* not support */
} /* End of ACM_CMD_Sim_nonQoS_Data_Rv */


/*
========================================================================
Routine Description:
	Transmit a QoS data frame from the upper layer.

Arguments:
	pAd			- WLAN control block pointer
	*pInfo			- the data flow information

Return Value:
	None

Note:
	For QAP.
========================================================================
*/
static VOID ACM_CMD_Sim_Data_Tx(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	ACM_DATA_SIM		*pInfo)
{
	UCHAR			*pBufFrame;
	NDIS_STATUS		Status;
	ACMR_MBUF		*pMblk;
	UCHAR			*pFrameBody;
	UINT32			DataIndex;


	/* init */
	pBufFrame = NULL;
	DataIndex = 0;

	/* get an unused nonpaged memory */
	Status = MlmeAllocateMemory(pAd, &pBufFrame);
	if (Status != NDIS_STATUS_SUCCESS)
		return;
	/* End of if */

	/* allocate action request frame */
	ACMR_PKT_ALLOCATE(pAd, pMblk);
	if (pMblk == NULL)
	{
		printk("11e_err> allocate action frame fail!\n");
		goto LabelErr;
	} /* End of if */

	/* init frame body */
	ACMR_MEM_COPY(pBufFrame, pInfo->MacDst, 6);
	ACMR_MEM_COPY(pBufFrame+6, pInfo->MacSrc, 6);
	*(UINT16 *)(pBufFrame+12) = 0x0800;

	pFrameBody = (UCHAR *)(pBufFrame+14);
	ACMR_MEM_SET(pFrameBody, 'c', pInfo->FrameSize); /* data */

	/* check whether the packet is TCP test packet */
	if (gSimTCPFlag != 0)
	{
#define DATA_SET(value)     pFrameBody[DataIndex++] = value;

		/* prepare TCP packet */
		DATA_SET(0x08); DATA_SET(0x00); /* type/len */
		DATA_SET(0x45); DATA_SET(gSimTCPDSCP); /* version, len, DSCP */
		DATA_SET(0x00); DATA_SET(0x28); /* total length */
		DATA_SET(0x4d); DATA_SET(0x78); /* identifier */
		DATA_SET(0x00); DATA_SET(0x00); DATA_SET(0x38); DATA_SET(0x06);
		DATA_SET(0x97); DATA_SET(0xd9);
		DATA_SET(0xac); DATA_SET(0x14); DATA_SET(0x40); DATA_SET(0xf9);
		DATA_SET(0xac); DATA_SET(0x14); DATA_SET(0x04); DATA_SET(0x5d);

		DATA_SET(0x00); DATA_SET(0x51); DATA_SET(0x06); DATA_SET(0x81);
		DATA_SET(0xdd); DATA_SET(0x56); DATA_SET(0x9d); DATA_SET(0xeb);
		DATA_SET(0xa5); DATA_SET(0xa9); DATA_SET(0xa0); DATA_SET(0x9b);
		DATA_SET(0x50); DATA_SET(0x10); DATA_SET(0x3d); DATA_SET(0x57);
		DATA_SET(0x0c); DATA_SET(0xa5); DATA_SET(0x00); DATA_SET(0x00);
	}
	else
	{
		/* use VLAN */
		pFrameBody = (UCHAR *)(pBufFrame+12);
		DATA_SET(0x81); DATA_SET(0x00); /* type/len */
		DATA_SET(pInfo->TID<<5); DATA_SET(0x00);
	} /* End of if */

	/* send the packet */
	ACMR_PKT_COPY(pMblk, pBufFrame, pInfo->FrameSize);

	rt28xx_packet_xmit(RTPKT_TO_OSPKT(pMblk));

LabelErr:
	/* free the frame buffer */
	MlmeFreeMemory(pAd, pBufFrame);
} /* End of ACM_CMD_Sim_Data_Tx */

#else

/*
========================================================================
Routine Description:
	Simulation periodical timer.

Arguments:
	Data			- WLAN control block pointer

Return Value:
	None

Note:
========================================================================
*/
VOID ACM_CMD_Timer_Data_Simulation(ULONG Data)
{
} /* End of ACM_CMD_Timer_Data_Simulation */
#endif // IEEE80211E_SIMULATION //


/*
========================================================================
Routine Description:
	Enable or disable test flag.

Arguments:
	pAd				- WLAN control block pointer
	Argc			- the number of input parameters
	*pArgv			- input parameters

Return Value:
	None

Note:
	Test flag is used in various test situation.
========================================================================
*/
static VOID AcmCmdTestFlagCtrl(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC	pAd,
	ACM_PARAM_IN	INT32				Argc,
	ACM_PARAM_IN	CHAR				*pArgv)
{
	gAcmTestFlag = AcmCmdUtilNumGet(&pArgv);
} /* End of AcmCmdTestFlagCtrl */




#define ACM_CMD_TCLAS_RESET			1	/* wmm tclas reset */
#define ACM_CMD_TCLAS_ADD			2	/* wmm tclas add */
#define ACM_CMD_EDCA_TS_ADD			3	/* wmm edca ts add */
#define ACM_CMD_EDCA_CHG			4	/* wmm edca change */
#define ACM_CMD_AVAL_SHOW			5	/* wmm available bw show */
#define ACM_CMD_SHOW				6	/* wmm ts show */
#define ACM_CMD_FSHOW				7	/* wmm fail show */
#define ACM_CMD_EDCA_SHOW			8	/* wmm edca info show */
#define ACM_CMD_WME_DATL			9	/* wmm datl */
#define ACM_CMD_WME_ACM_CTRL		10	/* wmm acm ctrl */
#define ACM_CMD_DELTS				11	/* wmm delts */
#define ACM_CMD_FCLEAR				12	/* wmm fclr */
#define ACM_CMD_EDCA_TS_NEG			13	/* wmm edca ts negotiate */
#define ACM_CMD_UAPSD_SHOW			14	/* wmm uapsd show */
#define ACM_CMD_TSPEC_REJECT		15	/* wmm tspec reject */
#define ACM_CMD_EDCA_TS_ADD_ADV		16	/* wmm edca ts add advcance */
#define ACM_CMD_STATS				17	/* wmm statistics */
#define ACM_CMD_REASSOCIATE			18	/* wmm reassociate */

#define ACM_CMD_SM_ASSOC			50	/* wmm sim assoc */
#define ACM_CMD_SM_REQ				51	/* wmm sim req */
#define ACM_CMD_SM_DEL_FRM_QSTA		52	/* wmm sim del */
#define ACM_CMD_SM_DATRV			53	/* wmm sim datrv */
#define ACM_CMD_SM_DATTX			54	/* wmm sim dattx */
#define ACM_CMD_SM_STP				55	/* wmm sim stp */
#define ACM_CMD_SM_SUS				56	/* wmm sim sus */
#define ACM_CMD_SM_RES				57	/* wmm sim res */
#define ACM_CMD_SM_REASSOC			58	/* wmm sim reassoc */
#define ACM_CMD_SM_NASSOC			59	/* wmm sim nassoc */
#define ACM_CMD_SM_NDATRV			60	/* wmm sim ndatarv */
#define ACM_CMD_SM_RATE				61	/* wmm sim rate */
#define ACM_CMD_SM_TCP				62	/* wmm sim tcp */
#define ACM_CMD_SM_STA_MAC_SET		63	/* wmm sim staset */
#define ACM_CMD_SM_STA_ASSOC		64	/* wmm sim sta auth/assoc */
#define ACM_CMD_SM_WME_REQTX		65	/* wmm sim req tx */
#define ACM_CMD_SM_WME_NEQTX		66	/* wmm sim neq tx */
#define ACM_CMD_SM_WME_REQ_FAIL		67	/* wmm sim req fail */
#define ACM_CMD_SM_WME_NEG_FAIL		68	/* wmm sim neg fail */
#define ACM_CMD_SM_ACM_RESET		69	/* wmm sim acm reset */
#define ACM_CMD_SM_STA_PS			70	/* wmm sim station enters PS or ACT */
#define ACM_CMD_SM_REQ_PS_POLL		71	/* wmm sim req & ps poll */
#define ACM_CMD_SM_DEL_FRM_QAP		72	/* wmm sim del */
#define ACM_CMD_SM_TRI_FRM_RCV		73	/* wmm sim trigger frame receive */
#define ACM_CMD_SM_UAPSD_QUE_MAIN	74	/* wmm sim uapsd queue maintain ctrl */
#define ACM_TEST_FLAG				90	/* wmm test flag */

/*
========================================================================
Routine Description:
	Test command.

Arguments:
	pAd				- WLAN control block pointer
	*pArgvIn		- the data flow information

Return Value:
	None

Note:
	All test commands are listed as follows:
		iwpriv ra0 set acm=[cmd id]_[arg1]_[arg2]_......_[argn]
		[cmd id] = xx, such as 00, 01, 02, 03, ...

	1.  Reset all TCLAS elements
	2.  Create a TCLAS element
			[up:0~7] [type:0~2] [mask:hex] [classifier:max 16B]
	3.  Create a QoS EDCA stream
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[APSD:0~1] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
			(ack policy:0~3)
	4.  Change EDCA parameters
			[CPnu] [CPde] [BEnu] [BEde]
	5.  Show current bandwidth status
	6.  Show current streams status
			[1:EDCA] (Client MAC: xx:xx:xx:xx:xx:xx)
	7.  Show fail streams status
	8.  Show current EDCA parameters
	9.  Set DATL parameters
			[enable: 0~1] [minimum bw threshold for AC0~AC3]
			[maximum bw threshold for AC0~AC3]
	10. Enable/disable ACM flag for 4 AC
			[AC0] [AC1] [AC2] [AC3]
	11. Send out a DELTS request frame
			[Peer MAC] [TID:0~7]
	12. Clear the failed list
	13. Negotiate a QoS EDCA stream
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[APSD:0~1] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
	14. Show UAPSD information
			[Device MAC]
	15. Reject all new TSPEC requests
			[Enable/Disable 1/0]
	16. Create a advanced QoS EDCA stream
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[APSD:0~1] [max size:byte] [nom size:byte] [burst size:byte]
			[inact:sec]
			[peak data rate:bps] [mean data rate:bps] [min data rate:bps]
			[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
	17. Show statistics count
	18. Send a reassociate frame to the associated AP


	50. Simulate authentication & assocaition req event
			(Source Client MAC)
	51. Simulate a request frame receive event
			[sta mac:xx:xx:xx:xx:xx:xx]
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[UAPSD:0/1] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
	52. Simulate to delete a actived stream from QSTA
			[Client MAC] [type:0-11e, 1-WME] [TID:0~7] [dir:0~3]
	53. Simulate to continue to receive packets from a QSTA
			[Src Client MAC] [Dst Client MAC] [type:0-11e, 1-WME] [TID:0~7]
			[size] (ack: 0~1) (fragment: 0~1) (rts/cts: 0~1)
	54. Simulate to continue to transmit packets from upper layer
			[Dst Client MAC] [type:0-11e, 1-WME] [TID:0~7] [size] [ack:0~1]
	55. Simulate to stop to continue to send packets to a QSTA
			(Client MAC)
	56. Simulate to suspend to send packets
	57. Simulate to resume to send packets
			(delay)
	58. Clear all failed records
	59. Simulate authentication & assocaition req event (non-QoS)
			[Source Client MAC]
	60. Simulate to continue to receive non-QoS packets from a QSTA
			[Src Client MAC] [Dst Client MAC] [TID:0~7]
	61. Simulate to force AP to transmit packets using the rate
			[Dst Client MAC] [rate]
	62. Simulate to enable or disable TCP tx packet
			[enable: 0~1] [DSCP]
	63. Simulate to reset station MAC address
			[MAC]
	64. Simulate to authenticate and associate a AP
			[AP MAC]
	65. Simulate to send a ADDTS request and receive a ADDTS response
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[ack:0] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1] [tclas processing:0~1]
	66. Simulate to send a negotiated ADDTS request and receive a ADDTS response
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[ack:0] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1]
	67. Simulate to send a ADDTS request and receive a ADDTS fail response
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[ack:0] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1]
	68. Simulate to send a negotiated ADDTS req and receive a failed ADDTS rsp
			[type:1-WME] [TID:0~7] [dir:0~3] [access:1] [UP:0~7]
			[ack:0] [nom size:byte] [inact:sec] [mean data rate:bps]
			[min phy rate:bps] [surp factor:>=1]
	69. Simulate the ACM flag set
			[ACM0] [ACM1] [ACM2] [ACM3]
	70. Simulate the station enters PS mode or ACTIVE mode
			[mode:0~1] [MAC]
	71. Simulate to receive a PS Poll frame
			[MAC]
	72. Simulate to delete a actived stream from QAP
			[type:1-WME] [TID:0~7] [dir:0~3]
	73. Simulate to receive a trigger frame from QSTA
			[MAC]
	74. Simulate to enable/disable UAPSD queue maintain
			[enable: 0~1]

	90. Test Flag
			[ON/OFF]
========================================================================
*/
INT ACM_Ioctl(
	ACM_PARAM_IN	ACMR_PWLAN_STRUC 	pAd,
	ACM_PARAM_IN	PSTRING				pArgvIn)
{
	CHAR BufCmd[3] = { 0, 0, 0 };
	CHAR *pArgv, *pParam;
	UINT32 Command;
	INT32 Argc;


	/* init */
	pArgv = (CHAR *)pArgvIn;

	/* get command type */
	/* command format is iwpriv ra0 set acm=[cmd id]_[arg1]_......_[argn] */
	ACMR_MEM_COPY(BufCmd, pArgv, 2);
	Command = ACMR_ARG_ATOI(BufCmd);
	pArgv += 2; /* skip command field */

	/* get Argc number */
	Argc = 0;
	pParam = pArgv;

	while(1)
	{
		if (*pParam == '_')
			Argc ++;
		/* End of if */

		if ((*pParam == 0x00) || (Argc > 20))
			break;
		/* End of if */

		pParam++;
	} /* End of while */

	pArgv++; /* skip _ points to arg1 */


	/* handle the command */
	switch(Command)
	{
		case ACM_CMD_TCLAS_RESET: /* 1 wmm tclas reset */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> reset TCLAS\n"));
			AcmCmdTclasReset(pAd, Argc, pArgv);
			break;

		case ACM_CMD_TCLAS_ADD: /* 2 wmm tclas add */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> add a TCLAS\n"));
			AcmCmdTclasCreate(pAd, Argc, pArgv);
			break;


		case ACM_CMD_EDCA_CHG: /* 4 wmm edca change */
			break;

		case ACM_CMD_AVAL_SHOW: /* 5 wmm available bw show */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> show available bw\n"));
			AcmCmdBandwidthDisplay(pAd, 0, pArgv);
			break;

		case ACM_CMD_SHOW: /* 6 wmm ts show */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> show ACM status\n"));
			AcmCmdStreamDisplay(pAd, Argc, pArgv);
			break;

		case ACM_CMD_FSHOW: /* 7 wmm fail show */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> show failed TS info\n"));
			AcmCmdStreamFailDisplay(pAd, 0, pArgv);
			break;

		case ACM_CMD_EDCA_SHOW: /* 8 wmm edca info show */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> show WMM TS info\n"));
			AcmCmdEDCAParamDisplay(pAd, 0, pArgv);
			break;

		case ACM_CMD_WME_DATL: /* 9 wmm datl */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> enable/disable Dynamic ATL\n"));
			AcmCmdDATLEnable(pAd, Argc, pArgv);
			break;

#ifdef CONFIG_AP_SUPPORT
		case ACM_CMD_WME_ACM_CTRL: /* 10 wmm acm ctrl */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> enable/disable ACM Flag for each AC\n"));
			AcmCmdAcmFlagCtrl(pAd, Argc, pArgv);
			break;
#endif // CONFIG_AP_SUPPORT //

		case ACM_CMD_DELTS: /* 11 wmm delts */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> send a DELTS request frame\n"));
			AcmCmdDeltsSend(pAd, Argc, pArgv);
			break;

		case ACM_CMD_FCLEAR: /* 12 wmm fclr */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> clear failed TS info\n"));
			AcmCmdStreamFailClear(pAd, Argc, pArgv);
			break;


		case ACM_CMD_UAPSD_SHOW: /* 14 wmm uapsd show */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> show UAPSD info\n"));
			AcmCmdUapsdDisplay(pAd, Argc, pArgv);
			break;

#ifdef CONFIG_AP_SUPPORT
		case ACM_CMD_TSPEC_REJECT: /* 15 wmm tspec reject */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> reject all TSPEC\n"));
			AcmCmdTspecReject(pAd, Argc, pArgv);
			break;
#endif // CONFIG_AP_SUPPORT //


		case ACM_CMD_STATS: /* 17 wmm statistics */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> show statistics\n"));
			AcmCmdStatistics(pAd, Argc, pArgv);
			break;



#ifdef IEEE80211E_SIMULATION
		case ACM_CMD_SM_ASSOC: /* 50 wmm sim assoc */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate STA auth/assoc\n"));
			AcmCmdSimAssocBuild(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_REQ: /* 51 wmm sim req */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate STA ADDTS req\n"));
			AcmCmdSimReqRcv(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_DEL_FRM_QSTA: /* 52 wmm sim del */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate STA DELTS\n"));
			AcmCmdSimDel(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_DATRV: /* 53 wmm sim datrv */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate DATA RCV\n"));
			AcmCmdSimDataRv(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_DATTX: /* 54 wmm sim dattx */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate DATA TX\n"));
			AcmCmdSimDataTx(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_STP: /* 55 wmm sim stp */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate DATA STOP\n"));
			AcmCmdSimDataStop(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_SUS: /* 56 wmm sim sus */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate DATA SUSPEND\n"));
			AcmCmdSimDataSuspend(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_RES: /* 57 wmm sim res */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate DATA RESUME\n"));
			AcmCmdSimDataResume(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_REASSOC: /* 58 wmm sim reassoc */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate STA reassoc\n"));
			AcmCmdSimReAssocBuild(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_NASSOC: /* 59 wmm sim nassoc */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
						("11e_cmd> simulate STA non-WMM auth/assoc\n"));
			AcmCmdSimNonQoSAssocBuild(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_NDATRV: /* 60 wmm sim ndatrv */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
						("11e_cmd> simulate non-WMM DATA RV\n"));
			AcmCmdSimNonQoSDataRv(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_RATE: /* 61 wmm sim rate */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> reset TX rate for a STA\n"));
			AcmCmdSimRateSet(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_TCP: /* 62 wmm sim tcp */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
						("11e_cmd> enable/disable to tx simulated TCP PKT\n"));
			AcmCmdSimTcpTxEnable(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_STA_MAC_SET: /* 63 wmm sim staset */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> reset simulated STA MAC\n"));
			AcmCmdSimStaMacSet(pAd, Argc, pArgv);
			break;


		case ACM_CMD_SM_STA_PS: /* 70 wmm sim station enters PS */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> simulate to receive a packet with PM=0/1\n"));
			AcmCmdSimWmePSEnter(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_REQ_PS_POLL: /* 71 wmm ps poll rv */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> simulate STA ADDTS req & Ps Poll\n"));
			AcmCmdSimReqPsPollRcv(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_DEL_FRM_QAP: /* 72 wmm sim del */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> simulate AP DELTS\n"));
			AcmCmdSimDel(pAd, Argc, pArgv);
			break;

#ifdef CONFIG_AP_SUPPORT
		case ACM_CMD_SM_TRI_FRM_RCV: /* 73 wmm sim trigger frame receive */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> simulate AP RCV a trigger frame\n"));
			AcmCmdSimRcvTriFrame(pAd, Argc, pArgv);
			break;

		case ACM_CMD_SM_UAPSD_QUE_MAIN: /* 74 wmm sim uapsd queue maintain */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> simulate to enable/disable UAPSD queue maintain\n"));
			AcmCmdSimUapsdQueCtrl(pAd, Argc, pArgv);
			break;
#endif // CONFIG_AP_SUPPORT //
#endif // IEEE80211E_SIMULATION //

		case ACM_TEST_FLAG: /* 90 wmm test flag */
			ACMR_DEBUG(ACMR_DEBUG_TRACE,
					("11e_cmd> ON/OFF test flag\n"));
			AcmCmdTestFlagCtrl(pAd, Argc, pArgv);
			break;

		default: /* error command type */
			ACMR_DEBUG(ACMR_DEBUG_TRACE, ("11e_cmd> ERROR! No such command!\n"));
			return -EINVAL; /* input error */
	} /* End of switch */

	return TRUE;
} /* End of ACM_Ioctl */

#endif // WMM_ACM_SUPPORT //

/* End of acm_iocl.c */

