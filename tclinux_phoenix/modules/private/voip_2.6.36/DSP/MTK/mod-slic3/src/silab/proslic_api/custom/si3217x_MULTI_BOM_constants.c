/*
** Copyright (c) 2016 Silicon Laboratories, Inc.
** 2016-01-13 01:25:26
**
** Si3217x ProSLIC API Configuration Tool Version 4.0.3
** Last Updated in API Release: 8.0.0
** Auto generated file from configuration tool
*/


#include "proslic.h"
#include "si3217x.h"

Si3217x_General_Cfg Si3217x_General_Configuration  = {
0x73,                       /* DEVICE_KEY */
BO_DCDC_FLYBACK,            /* BOM_OPT */
BO_GDRV_INSTALLED,          /* GDRV_OPTION */
VDC_7P0_20P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
};


/* Start of MULTI BOM section */
#ifdef SIVOICE_MULTI_BOM_SUPPORT

#include "si32xxx_multibom_constants.h"
int si3217x_genconf_multi_max_preset = SI3217X_GEN_PARAM_LAST;

Si3217x_General_Cfg Si3217x_General_Configuration_MultiBOM[SI3217X_GEN_PARAM_LAST] = {
{ /*  SI3217X_GEN_PARAM_FLYBACK_GDRV */ 
0x73,                       /* DEVICE_KEY */
BO_DCDC_FLYBACK,            /* BOM_OPT */
BO_GDRV_INSTALLED,          /* GDRV_OPTION */
VDC_7P0_20P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
},
{ /*  SI3217X_GEN_PARAM_FLYBACK_NO_GDRV */ 
0x71,                       /* DEVICE_KEY */
BO_DCDC_FLYBACK,            /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_7P0_20P0,               /* VDC_RANGE_OPT */
VDAA_DISABLED,              /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x00,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
},
{ /*  SI3217X_GEN_PARAM_BUCK_BOOST_NO_GDRV */ 
0x73,                       /* DEVICE_KEY */
BO_DCDC_BUCK_BOOST,         /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_8P0_16P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1400L,                      /* I_OITHRESH (1400mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1400L,                      /* I_OITHRESH_HI (1400mA) */
94000L,                     /* V_OVTHRESH (94000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x06147AB2L,                /* VBATR_EXPECT (95.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00000000L,                /* DCDC_RNGTYPE */
0x00600000L,                /* DCDC_ANA_TOFF */
0x00400000L,                /* DCDC_ANA_TONMIN */
0x01500000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
},
{ /*  SI3217X_GEN_PARAM_LCQC3W */ 
0x71,                       /* DEVICE_KEY */
BO_DCDC_LCQC_3W,            /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_4P5_16P0,               /* VDC_RANGE_OPT */
VDAA_DISABLED,              /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1850L,                      /* I_OITHRESH (1850mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1850L,                      /* I_OITHRESH_HI (1850mA) */
128000L,                    /* V_OVTHRESH (128000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x00,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
},
{ /*  SI3217X_GEN_PARAM_LCQC6W */ 
0x71,                       /* DEVICE_KEY */
BO_DCDC_LCQC_6W,            /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_4P5_16P0,               /* VDC_RANGE_OPT */
VDAA_DISABLED,              /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1750L,                      /* I_OITHRESH (1750mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1750L,                      /* I_OITHRESH_HI (1750mA) */
128000L,                    /* V_OVTHRESH (128000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x00,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
}
};
#endif


/* End of MULTI BOM section */

Si3217x_GPIO_Cfg Si3217x_GPIO_Configuration = {
0x00,     /* GPIO_OE */
0x06,     /* GPIO_ANA */
0x00,     /* GPIO_DIR */
0x00,     /* GPIO_MAN */
0x00,     /* GPIO_POL */
0x00,     /* GPIO_OD */
0x00     /* BATSELMAP */
};
Si3217x_CI_Cfg Si3217x_CI_Presets [] = {
{0}
};
Si3217x_audioGain_Cfg Si3217x_audioGain_Presets [] = {
{0x1377080L,0, 0x0L, 0x0L, 0x0L, 0x0L},
{0x80C3180L,0, 0x0L, 0x0L, 0x0L, 0x0L}
};

Si3217x_Ring_Cfg Si3217x_Ring_Presets[] ={
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 3, Rcpe = 600 ohms
	Rprot = 30 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x0025D5BFL,	/* RINGAMP (64.490 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) */
0x00608636L,	/* RTACTH (53.304 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x070A0AA8L,	/* VBATR_EXPECT (109.988 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x01,			/* USERSTAT */
0x03850554L,	/* VCM_RING (51.994 v) */
0x03850554L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
},  /* RING_MAX_VBAT_PROVISIONING */
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 30 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001B9F2EL,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) */
0x0068E9B4L,	/* RTACTH (57.936 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x0558ABFCL,	/* VBATR_EXPECT (83.537 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x01,			/* USERSTAT */
0x02AC55FEL,	/* VCM_RING (38.769 v) */
0x02AC55FEL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
},  /* RING_F20_45VRMS_0VDC_LPR */
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 30 ohms, Type = BALANCED, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001B9F2EL,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) */
0x0068E9B4L,	/* RTACTH (57.936 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x0558ABFCL,	/* VBATR_EXPECT (83.537 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02AC55FEL,	/* VCM_RING (38.769 v) */
0x02AC55FEL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}   /* RING_F20_45VRMS_0VDC_BAL */
};

Si3217x_DCfeed_Cfg Si3217x_DCfeed_Presets[] = {
{
0x1C8A024CL,	/* SLOPE_VLIM */
0x1F909679L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1D5B21A9L,	/* SLOPE_DELTA1 */
0x1DD87A3EL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x050D2839L,	/* V_RFEED (43.000 v) */
0x03FE7F0FL,	/* V_ILIM  (34.000 v) */
0x00B4F3C3L,	/* CONST_RFEED (15.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
},  /* DCFEED_48V_20MA */
{
0x1C8A024CL,	/* SLOPE_VLIM */
0x1EE08C11L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1C940D71L,	/* SLOPE_DELTA1 */
0x1DD87A3EL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x050D2839L,	/* V_RFEED (43.000 v) */
0x03FE7F0FL,	/* V_ILIM  (34.000 v) */
0x01241BC9L,	/* CONST_RFEED (15.000 mA) */
0x0074538FL,	/* CONST_ILIM (25.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
},  /* DCFEED_48V_25MA */
{
0x1E655196L,	/* SLOPE_VLIM */
0x001904EFL,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1B4CAD9EL,	/* SLOPE_DELTA1 */
0x1BB0F47CL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x043AA4A6L,	/* V_RFEED (36.000 v) */
0x025977EAL,	/* V_ILIM  (20.000 v) */
0x0068B19AL,	/* CONST_RFEED (18.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
},  /* DCFEED_PSTN_DET_1 */
{
0x1A10433FL,	/* SLOPE_VLIM */
0x1C206275L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1C1F426FL,	/* SLOPE_DELTA1 */
0x1EB51625L,	/* SLOPE_DELTA2 */
0x041C91DBL,	/* V_VLIM (35.000 v) */
0x03E06C43L,	/* V_RFEED (33.000 v) */
0x038633E0L,	/* V_ILIM  (30.000 v) */
0x022E5DE5L,	/* CONST_RFEED (10.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
0x0021373DL,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
}   /* DCFEED_PSTN_DET_2 */
};

Si3217x_Impedance_Cfg Si3217x_Impedance_Presets[] ={
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=30 rfuse=0 emi_cap=10*/
{
{0x07F46C00L, 0x000E4600L, 0x00008580L, 0x1FFD6100L,    /* TXACEQ */
 0x07EF5000L, 0x0013F580L, 0x1FFDE000L, 0x1FFCB280L},   /* RXACEQ */
{0x0027CB00L, 0x1F8A8880L, 0x02801180L, 0x1F625C80L,    /* ECFIR/ECIIR */
 0x0314FB00L, 0x1E6B8E80L, 0x00C5FF00L, 0x1FC96F00L,
 0x1FFD1200L, 0x00023C00L, 0x0ED29D00L, 0x192A9400L},
{0x00810E00L, 0x1EFEBE80L, 0x00803500L, 0x0FF66D00L,    /* ZSYNTH */
 0x18099080L, 0x59}, 
 0x088E0D80L,   /* TXACGAIN */
 0x01456D80L,   /* RXACGAIN */
 0x07ABE580L, 0x18541B00L, 0x0757CB00L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_600_0_0_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=0 emi_cap=10*/
{
{0x0715CB80L, 0x1FD56880L, 0x000D4480L, 0x1FFEAE00L,    /* TXACEQ */
 0x0A834F80L, 0x1BA7E500L, 0x0080D300L, 0x1FDC1580L},   /* RXACEQ */
{0x0017A080L, 0x1FDE3600L, 0x0129F900L, 0x01915280L,    /* ECFIR/ECIIR */
 0x01434280L, 0x018E9E00L, 0x1FFE1200L, 0x0085E000L,
 0x001ECE80L, 0x1FDF3C80L, 0x0CC9EA00L, 0x1B2E1180L},
{0x00C66800L, 0x1DD9CB80L, 0x015F8F00L, 0x0CB97F00L,    /* ZSYNTH */
 0x1B44F480L, 0x94}, 
 0x08000000L,   /* TXACGAIN */
 0x0108FB80L,   /* RXACGAIN */
 0x07BB6980L, 0x18449700L, 0x0776D380L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_270_750_150_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=370_620_310 rprot=30 rfuse=0 emi_cap=10*/
{
{0x07E59E80L, 0x1FD33400L, 0x1FFDF800L, 0x1FFD8300L,    /* TXACEQ */
 0x09F38000L, 0x1C1C5A00L, 0x1F94D700L, 0x1FDE5800L},   /* RXACEQ */
{0x00234480L, 0x1F9CDD00L, 0x01F5D580L, 0x1FF39000L,    /* ECFIR/ECIIR */
 0x02C17180L, 0x1FBE2500L, 0x00DFFE80L, 0x00441A80L,
 0x003BF800L, 0x1FC42400L, 0x0D9EB380L, 0x1A514580L},
{0x003ED200L, 0x1F5D6B80L, 0x0063B100L, 0x0F12E200L,    /* ZSYNTH */
 0x18EC9380L, 0x8B}, 
 0x08000000L,   /* TXACGAIN */
 0x0127C700L,   /* RXACGAIN */
 0x07B51200L, 0x184AEE80L, 0x076A2480L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_370_620_310_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=220_820_120 rprot=30 rfuse=0 emi_cap=10*/
{
{0x06E2A580L, 0x1FD1DF80L, 0x00068880L, 0x1FFCE200L,    /* TXACEQ */
 0x0A7AFB00L, 0x1BC11F80L, 0x009C4E80L, 0x1FD60300L},   /* RXACEQ */
{0x002C9880L, 0x1F530400L, 0x02CF4D80L, 0x1E895880L,    /* ECFIR/ECIIR */
 0x055F7200L, 0x1E034600L, 0x023B9080L, 0x1FB01780L,
 0x00339380L, 0x1FC98F80L, 0x0B7EA900L, 0x1C760400L},
{0x022C8200L, 0x1A9F3E80L, 0x03332100L, 0x0A0D4700L,    /* ZSYNTH */
 0x1DEBC480L, 0x8D}, 
 0x08000000L,   /* TXACGAIN */
 0x01013A80L,   /* RXACGAIN */
 0x07BEF980L, 0x18410700L, 0x077DF280L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_220_820_120_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=30 rfuse=0 emi_cap=10*/
{
{0x07F46C00L, 0x000E4600L, 0x00008580L, 0x1FFD6100L,    /* TXACEQ */
 0x07EF5000L, 0x0013F580L, 0x1FFDE000L, 0x1FFCB280L},   /* RXACEQ */
{0x0027CB00L, 0x1F8A8880L, 0x02801180L, 0x1F625C80L,    /* ECFIR/ECIIR */
 0x0314FB00L, 0x1E6B8E80L, 0x00C5FF00L, 0x1FC96F00L,
 0x1FFD1200L, 0x00023C00L, 0x0ED29D00L, 0x192A9400L},
{0x00810E00L, 0x1EFEBE80L, 0x00803500L, 0x0FF66D00L,    /* ZSYNTH */
 0x18099080L, 0x59}, 
 0x088E0D80L,   /* TXACGAIN */
 0x01456D80L,   /* RXACGAIN */
 0x07ABE580L, 0x18541B00L, 0x0757CB00L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_600_0_1000_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=200_680_100 rprot=30 rfuse=0 emi_cap=10*/
{
{0x07365D80L, 0x1FC64180L, 0x00022980L, 0x1FFCE300L,    /* TXACEQ */
 0x09C28580L, 0x1D1FD880L, 0x0071A280L, 0x1FDF7500L},   /* RXACEQ */
{0x1FF15A00L, 0x005C0600L, 0x00828200L, 0x01B11D00L,    /* ECFIR/ECIIR */
 0x027BB800L, 0x1EE9F200L, 0x028BAB80L, 0x1E57BE80L,
 0x01007580L, 0x1EF8B780L, 0x0556EE80L, 0x028DFB80L},
{0x014F2C00L, 0x1C7A1180L, 0x02369A00L, 0x0A138100L,    /* ZSYNTH */
 0x1DEA2280L, 0x8E}, 
 0x08000000L,   /* TXACGAIN */
 0x010C7E80L,   /* RXACGAIN */
 0x07BB2500L, 0x1844DB80L, 0x07764980L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_200_680_100_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=220_820_115 rprot=30 rfuse=0 emi_cap=10*/
{
{0x06D56400L, 0x1FDF1780L, 0x00095A80L, 0x1FFDA880L,    /* TXACEQ */
 0x0A596300L, 0x1C067880L, 0x0095EF00L, 0x1FD7AF00L},   /* RXACEQ */
{0x00164300L, 0x1FD81880L, 0x0150CC80L, 0x0151BB80L,    /* ECFIR/ECIIR */
 0x01DA1A00L, 0x0142CB80L, 0x0027DE80L, 0x0076A180L,
 0x0012F980L, 0x1FEAE000L, 0x0CC70C80L, 0x1B2DF000L},
{0x00246300L, 0x1E5E0580L, 0x017D2300L, 0x0A138100L,    /* ZSYNTH */
 0x1DEA2280L, 0xA7}, 
 0x08000000L,   /* TXACGAIN */
 0x01009500L,   /* RXACGAIN */
 0x07BBEE80L, 0x18441200L, 0x0777DD80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_220_820_115_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=20 rfuse=0 emi_cap=0*/
{
{0x0817F080L, 0x1FE39600L, 0x0005A280L, 0x1FFC7600L,    /* TXACEQ */
 0x07FA6E80L, 0x1FFD4F80L, 0x1FF80500L, 0x1FFDE780L},   /* RXACEQ */
{0x00538E00L, 0x1F47A800L, 0x02957D00L, 0x00128700L,    /* ECFIR/ECIIR */
 0x0194DD80L, 0x000E7100L, 0x1FC5B180L, 0x00061600L,
 0x0021B480L, 0x1FE88600L, 0x1FCAD600L, 0x1FD00E80L},
{0x006A0F00L, 0x1F499F80L, 0x004C8900L, 0x0A152400L,    /* ZSYNTH */
 0x1DE9B580L, 0x68}, 
 0x08419500L,   /* TXACGAIN */
 0x01365F00L,   /* RXACGAIN */
 0x07BB5700L, 0x1844A980L, 0x0776AE80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 }   /* WB_ZSYN_600_0_0_20_0 */
};

Si3217x_FSK_Cfg Si3217x_FSK_Presets[] ={
{
0x02232000L,	 /* FSK01 */
0x077C2000L,	 /* FSK10 */
0x003C0000L,	 /* FSKAMP0 (0.220 vrms )*/
0x00200000L,	 /* FSKAMP1 (0.220 vrms) */
0x06B60000L,	 /* FSKFREQ0 (2200.0 Hz space) */
0x079C0000L,	 /* FSKFREQ1 (1200.0 Hz mark) */
0x00,			 /* FSK8 */
0x00,			 /* FSKDEPTH (1 deep fifo) */
}   /* DEFAULT_FSK */
};

Si3217x_PulseMeter_Cfg Si3217x_PulseMeter_Presets[] ={
{
0x007A2B6AL,  /* PM_AMP_THRESH (1.000) */
0,            /* Freq (12kHz) */ 
0,            /* PM_RAMP (24kHz)*/
0,            /* PM_FORCE (First)*/
0,            /* PWR_SAVE (off)*/
0,            /* PM_AUTO (off)*/
0x07D00000L,  /* PM_active (2000 ms) */
0x07D00000L   /* PM_inactive (2000 ms) */
 }   /* DEFAULT_PULSE_METERING */
};

Si3217x_Tone_Cfg Si3217x_Tone_Presets[] = {
{
	{
	0x07B30000L,	 /* OSC1FREQ (350.000 Hz) */
	0x000C6000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x00,			 /* O1TALO (0 ms) */
	0x00,			 /* O1TAHI */
	0x00,			 /* O1TILO (0 ms) */
	0x00			 /* O1TIHI */
	},
	{
	0x07870000L,	 /* OSC2FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x00,			 /* O2TALO (0 ms) */
	0x00,			 /* O2TAHI */
	0x00,			 /* O2TILO (0 ms) */
	0x00 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_DIAL */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0xA0,			 /* O1TALO (500 ms) */
	0x0F,			 /* O1TAHI */
	0xA0,			 /* O1TILO (500 ms) */
	0x0F			 /* O1TIHI */
	},
	{
	0x07120000L,	 /* OSC2FREQ (620.000 Hz) */
	0x00164000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0xA0,			 /* O2TALO (500 ms) */
	0x0F,			 /* O2TAHI */
	0xA0,			 /* O2TILO (500 ms) */
	0x0F 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_BUSY */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x80,			 /* O1TALO (2000 ms) */
	0x3E,			 /* O1TAHI */
	0x00,			 /* O1TILO (4000 ms) */
	0x7D			 /* O1TIHI */
	},
	{
	0x07870000L,	 /* OSC2FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x80,			 /* O2TALO (2000 ms) */
	0x3E,			 /* O2TAHI */
	0x00,			 /* O2TILO (4000 ms) */
	0x7D 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_RINGBACK */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x60,			 /* O1TALO (300 ms) */
	0x09,			 /* O1TAHI */
	0x60,			 /* O1TILO (300 ms) */
	0x09			 /* O1TIHI */
	},
	{
	0x07120000L,	 /* OSC2FREQ (620.000 Hz) */
	0x00164000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x60,			 /* O2TALO (300 ms) */
	0x09,			 /* O2TAHI */
	0x40,			 /* O2TILO (200 ms) */
	0x06 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_REORDER */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x40,			 /* O1TALO (200 ms) */
	0x06,			 /* O1TAHI */
	0x40,			 /* O1TILO (200 ms) */
	0x06			 /* O1TIHI */
	},
	{
	0x07120000L,	 /* OSC2FREQ (620.000 Hz) */
	0x00164000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x40,			 /* O2TALO (200 ms) */
	0x06,			 /* O2TAHI */
	0x40,			 /* O2TILO (200 ms) */
	0x06 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
}   /* TONEGEN_FCC_CONGESTION */
};

Si3217x_PCM_Cfg Si3217x_PCM_Presets[] ={
	{
	0x01, 	 /* PCM_FMT - u-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_8ULAW */
	{
	0x00, 	 /* PCM_FMT - A-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_8ALAW */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_16LIN */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x01, 	 /* WIDEBAND - ENABLED (7kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	}   /* PCM_16LIN_WB */
};

