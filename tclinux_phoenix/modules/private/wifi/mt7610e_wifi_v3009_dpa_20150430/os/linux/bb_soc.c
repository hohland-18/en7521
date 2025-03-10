#include "rt_config.h"

#include "os/bb_soc.h"
//#include "rtmp_timer.h"
//#include "rt_config.h"
#if 0 //def RT5392
#ifdef WSC_AP_SUPPORT
extern int wscTimerRunning;
extern int wscStatus;
extern int statusprobe;
extern unsigned short wsc_done;
#define TC3162_LED_TIMMER_INTERVAL 5
#define TC3262_LED_TIMMER_INTERVAL 10
#if 0//def LED_WPSSPEC_COMPLY
#define 	WPS_INPROGRESS	1
#define	WPS_ERROR			2
#define	WPS_OVERLAP		3
#define	WPS_SUCCESS		4
#define	WPS_INPUT			0
#define	WPS_TIMEOUT	1200
extern void SetWPSLedMode(unsigned char LedMode);
#endif
#endif
#endif
#define isRT63365   (((*(volatile unsigned long int *)(0xbfb00064)&0xffff0000))==0x00040000)

#if defined(TCSUPPORT_WLAN_GPIO)
#define WLAN_LED_OFF 0
#define WLAN_LED_ON  1
#define WLAN_LED_OEN  2
#define WLAN_LED_IEN  3
RTMP_ADAPTER	*pAd_save = NULL;
extern int (*hook_wlan_led_action)(int action, int gpio);
extern unsigned int (*hook_wlan_led_get_data)(int gpio);
extern void set_wlan_gpio_mode(void);
#endif
/*
    ========================================================================

    Routine Description:
       Trendchip DMT Trainning status detect

    Arguments:
        data                     Point to RTMP_ADAPTER

    Return Value:
        NONE

    Note: 

    ========================================================================
*/

static UCHAR dslStateChg=0;
VOID PeriodicPollingModeDetect(
	IN PVOID SystemSpecific1, 
    IN PVOID FunctionContext, 
    IN PVOID SystemSpecific2, 
    IN PVOID SystemSpecific3){
  unsigned long irqFlags;
  UCHAR modem_status=0; 
  RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)FunctionContext;
  POS_COOKIE _pObj = (POS_COOKIE)(pAd->OS_Cookie);

  os_TCIfQuery(0x0002, &modem_status, NULL);
  
  if ((modem_status==0x08)||(modem_status==0x10))
  {
  	if(!(dslStateChg & (1<<0))){
//		printk("Detect DSL status:0x%02x into polling mode!!\r\n", modem_status);
		RTMP_INT_LOCK(&pAd->irq_lock, irqFlags);
                dslStateChg|=(1<<0);
		RTMP_INT_UNLOCK(&pAd->irq_lock, irqFlags);
	// disable enterrupt
	tc3162_disable_irq(_pObj->pci_dev->irq);
	}
	schedule_work(&pAd->Pollingmode.PollingDataBH);
	//slow down the POLLING MODE DETECT while the dmt in wait init state
  	//pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;

	RTMPModTimer(&pAd->Pollingmode.PollingModeDetect, 10);
  	pAd->Pollingmode.PollingModeDetectRunning = TRUE;
	return;
  }
  else{
	if(dslStateChg & (1<<0)){
 	tc3162_enable_irq(_pObj->pci_dev->irq);
 // 		printk("Detect DSL status:0x%02x into INT mode!!\r\n", modem_status);
  		RTMP_INT_LOCK(&pAd->irq_lock, irqFlags);
  		dslStateChg &=~(1<<0);
  		RTMP_INT_UNLOCK(&pAd->irq_lock, irqFlags);
	}
  }
  RTMPModTimer(&pAd->Pollingmode.PollingModeDetect, POLLING_MODE_DETECT_INTV);
  pAd->Pollingmode.PollingModeDetectRunning = TRUE;

#if 0 //def RT5392
#ifdef WSC_AP_SUPPORT
{
	static int lastWscStatus=0;
	PWSC_CTRL  pWscControl= &pAd->ApCfg.MBSSID[MAIN_MBSSID].WscControl;
	
	wscTimerRunning=pWscControl->Wsc2MinsTimerRunning;
	wscStatus=pWscControl->WscStatus;

	if((wscStatus==STATUS_WSC_CONFIGURED)&&(lastWscStatus!=STATUS_WSC_CONFIGURED)){
	#ifndef LED_WPSSPEC_COMPLY
		wsc_done = 10;
	#else
		wsc_done = 600;
	#endif
	#ifdef CONFIG_MIPS_TC3262
		wsc_done /= (TC3262_LED_TIMMER_INTERVAL/TC3162_LED_TIMMER_INTERVAL);
	#endif
		//ledTurnOff(LED_WLAN_WPS_ACT_STATUS);//xyyou???
		//ledTurnOn(LED_WLAN_WPS_STATUS);
		//ledTurnOn(LED_WLAN_WPS_NOACT_STATUS);

	}
	lastWscStatus=wscStatus;
}
#endif
#endif
  return;
}


VOID PollingModeIsr(struct work_struct *work)
{
  	
	PBBUPollingMode pPollingmode=container_of(work, BBUPollingMode, PollingDataBH);
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pPollingmode->pAd_va;
	struct net_device *net_dev = pAd->net_dev;
	rt2860_interrupt(0, net_dev);
}


VOID BBUPollingModeClose(IN RTMP_ADAPTER *pAd){
	BOOLEAN 		Cancelled;
	
	pAd->Pollingmode.PollingModeDetectRunning = FALSE;
	RTMPCancelTimer(&pAd->Pollingmode.PollingModeDetect, &Cancelled);
}

BUILD_TIMER_FUNCTION(PeriodicPollingModeDetect);


VOID BBUPollingModeInit(IN RTMP_ADAPTER *pAd){

	NdisAllocateSpinLock(pAd,&pAd->Pollingmode.PollingModeLock);//for polling mode
	
	RTMPInitTimer(pAd, &pAd->Pollingmode.PollingModeDetect, GET_TIMER_FUNCTION(PeriodicPollingModeDetect), pAd, FALSE);	
	pAd->Pollingmode.PollingModeDetectRunning = FALSE;	
}

VOID BBUPollingModeStart(IN RTMP_ADAPTER *pAd){

	if (pAd->Pollingmode.PollingModeDetectRunning == FALSE)
	{
	    printk("jiffies=%08lx, POLLING_MODE_DETECT_INTV=%d\r\n", jiffies, POLLING_MODE_DETECT_INTV);
	    RTMPSetTimer(&pAd->Pollingmode.PollingModeDetect, POLLING_MODE_DETECT_INTV);	
	}
	// init a BH task here
	INIT_WORK(&(pAd->Pollingmode.PollingDataBH), PollingModeIsr);
#if 0 //defined(TCSUPPORT_WLAN_GPIO)
	BBUSetWlanGpio(pAd);
#endif	

}

VOID BBU_PCIE_Init(){
	pcie_init();
}

VOID BBUPrepareMAC(IN RTMP_ADAPTER *pAd, PUCHAR macaddr ){
	UCHAR FourByteOffset = 0;			
	UCHAR NWlanExt = 0;
	
	FourByteOffset = macaddr[5]%4;		
	DBGPRINT(RT_DEBUG_WARN, ("\r\nFourByteOffset is %d", FourByteOffset));		
	NWlanExt = pAd->ApCfg.BssidNum;		
	DBGPRINT(RT_DEBUG_WARN, ("\r\nNWlanExt is %d", NWlanExt));
	

	switch(NWlanExt){			
		case 1:				
			break;			
		case 2:				
			switch(FourByteOffset){					
				case 1:					
				case 3:						
					macaddr[5]--;
					break;					
				case 0:					
				case 2:								
					break;				
			}		
			break;			
		case 3:		
		case 4:
			switch(FourByteOffset){					
				case 0:							
					break;					
				case 1:						
					macaddr[5]--;				
					break;					
				case 2:						
					macaddr[5] -= 2;											
					break;					
				case 3:						
					macaddr[5] -= 3;											
					break;				
			}			
			break;			
		default:
			break;
	}
	
		DBGPRINT(RT_DEBUG_WARN,("current MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", 	        
			macaddr[0], macaddr[1],     
			macaddr[2], macaddr[3],     
			macaddr[4], macaddr[5]));		
	/*generate bssid from cpe mac address end, merge from linos, 20100208*/
}
#if 0 //defined(TCSUPPORT_WLAN_GPIO)
int BBUWlanLedAct(int action ,int gpio){
	PRTMP_ADAPTER	pAd = pAd_save;
	UINT32 		gpioctl;
	
	if(pAd == NULL)
		return 0;
	if(pAd->CSRBaseAddress == NULL)	
		return 0;
		
	RTMP_IO_READ32(pAd, GPIO_CTRL_CFG, &gpioctl);
	RTMP_IO_READ32(pAd, GPIO_CTRL_CFG, &gpioctl); //read twice for rt63365

	switch(action){
		case	WLAN_LED_OFF:
			if(gpio > 7){
				gpioctl |= ((1<<(gpio-8))<<16);
			}else{
			gpioctl |= (1<<gpio);
			}	
			break;
		case 	WLAN_LED_ON:
			if(gpio > 7){
				gpioctl &= ~((1<<(gpio-8))<<16);
			}else{	
			gpioctl &= ~(1<<gpio);
			}	
			break;	
		case 	WLAN_LED_OEN:
			if(gpio > 7){
				gpioctl &= ~((1<<(gpio-8))<<24);
			}else{	
			gpioctl &= ~((1<<gpio)<<8);
			}	
			break;
		case 	WLAN_LED_IEN:
			if(gpio > 7){
				gpioctl |=  ((1<<(gpio-8))<<24);
			}else{	
				gpioctl |=  ((1<<gpio)<<8);
			}	
			break;
		default:
			break;		
	}	
	
	RTMP_IO_WRITE32(pAd, GPIO_CTRL_CFG, gpioctl);
	
	return 0;
}

unsigned int BBUWlanGetLed(int gpio){
	PRTMP_ADAPTER	pAd = pAd_save;
	UINT32		gpioctl;
	
	if(pAd == NULL)
		return (1<<gpio);
	if(pAd->CSRBaseAddress == NULL)	
		return (1<<gpio);
	
	RTMP_IO_READ32(pAd, GPIO_CTRL_CFG, &gpioctl);
	RTMP_IO_READ32(pAd, GPIO_CTRL_CFG, &gpioctl); //read twice for rt63365
	
	if(gpio > 7){
		return (gpioctl & ((1<<(gpio-8))<<16) );
	}
	else{
		return (gpioctl & (1<<gpio));
	}
}
void BBUSetWlanGpio(IN RTMP_ADAPTER *pAd){
	set_wlan_gpio_mode();
	return;
}

int BBUAfterInsMod(IN RTMP_ADAPTER *pAd){
	pAd_save = pAd;
	rcu_assign_pointer(hook_wlan_led_action, BBUWlanLedAct);
	rcu_assign_pointer(hook_wlan_led_get_data, BBUWlanGetLed);
	BBUSetWlanGpio(pAd);
	return 0;
}
int BBUBeforeRmMod(IN RTMP_ADAPTER *pAd){
	pAd_save = NULL;
	
	rcu_assign_pointer(hook_wlan_led_action, NULL);
	rcu_assign_pointer(hook_wlan_led_get_data, NULL);
	
	return 0;
}
#endif
