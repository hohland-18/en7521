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
	cmm_profile.c
 
    Abstract:
 
    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
 */
 
#include "rt_config.h"


CHAR End_Mark='\0';

#define ETH_MAC_ADDR_STR_LEN 17  // in format of xx:xx:xx:xx:xx:xx

// We assume the s1 is a sting, s2 is a memory space with 6 bytes. and content of s1 will be changed.
BOOLEAN rtstrmactohex(PSTRING s1, PSTRING s2)
{
	int i = 0;
	PSTRING ptokS = s1, ptokE = s1;

	if (strlen(s1) != ETH_MAC_ADDR_STR_LEN)
		return FALSE;

	while((*ptokS) != End_Mark)	
	{
		if((ptokE = strchr(ptokS, ':')) != NULL)
			*ptokE++ = End_Mark;

		if ((strlen(ptokS) != 2) || (!isxdigit(*ptokS)) || (!isxdigit(*(ptokS+1))))
			break; // fail
		AtoH(ptokS, (PUCHAR)&s2[i++], 1);
		ptokS = ptokE;
		if (i == 6)
			break; // parsing finished
	}

	return ( i == 6 ? TRUE : FALSE);

}


// we assume the s1 and s2 both are strings.
BOOLEAN rtstrcasecmp(PSTRING s1, PSTRING s2)
{
	PSTRING p1 = s1, p2 = s2;
	
	if (strlen(s1) != strlen(s2))
		return FALSE;
	
	while(*p1 != End_Mark)
	{
		if((*p1 != *p2) && ((*p1 ^ *p2) != 0x20))
			return FALSE;
		p1++;
		p2++;
	}
	
	return TRUE;
}

// we assume the s1 (buffer) and s2 (key) both are strings.
PSTRING rtstrstruncasecmp(PSTRING s1, PSTRING s2)
{
	INT l1, l2, i;
	char temp1, temp2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;

	l1 = strlen(s1);

	while (l1 >= l2)
	{
		l1--;

		for(i=0; i<l2; i++)
		{
			temp1 = *(s1+i);
			temp2 = *(s2+i);

			if (('a' <= temp1) && (temp1 <= 'z'))
				temp1 = 'A'+(temp1-'a');
			if (('a' <= temp2) && (temp2 <= 'z'))
				temp2 = 'A'+(temp2-'a');

			if (temp1 != temp2)
				break;
		}

		if (i == l2)
			return (char *) s1;

		s1++;
	}
	
	return NULL; // not found
}

//add by kathy

 /**
  * strstr - Find the first substring in a %NUL terminated string
  * @s1: The string to be searched
  * @s2: The string to search for
  */
PSTRING rtstrstr(PSTRING s1,const PSTRING s2)
{
	INT l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return s1;
	
	l1 = strlen(s1);
	
	while (l1 >= l2)
	{
		l1--;
		if (!memcmp(s1,s2,l2))
			return s1;
		s1++;
	}
	
	return NULL;
}
 
/**
 * rstrtok - Split a string into tokens
 * @s: The string to be searched
 * @ct: The characters to search for
 * * WARNING: strtok is deprecated, use strsep instead. However strsep is not compatible with old architecture.
 */
PSTRING __rstrtok;
PSTRING rstrtok(PSTRING s,const PSTRING ct)
{
	PSTRING sbegin, send;

	sbegin  = s ? s : __rstrtok;
	if (!sbegin)
	{
		return NULL;
	}

	sbegin += strspn(sbegin,ct);
	
	if (*sbegin == End_Mark)	
	{
		__rstrtok = NULL;
		return( NULL );
	}

	send = strpbrk( sbegin, ct);

	if (send && *send != End_Mark)
		*send++ = End_Mark;

	__rstrtok = send;

	return (sbegin);
}

/**
 * delimitcnt - return the count of a given delimiter in a given string.
 * @s: The string to be searched.
 * @ct: The delimiter to search for.
 * Notice : We suppose the delimiter is a single-char string(for example : ";").
 */
INT delimitcnt(PSTRING s,PSTRING ct)
{
	INT count = 0;
	/* point to the beginning of the line */
	PSTRING token = s; 

	for ( ;; )
	{
		token = strpbrk(token, ct); /* search for delimiters */

        if ( token == NULL )
		{
			/* advanced to the terminating null character */
			break; 
		}
		/* skip the delimiter */
	    ++token; 

		/*
		 * Print the found text: use len with %.*s to specify field width.
		 */
        
		/* accumulate delimiter count */
	    ++count; 
	}
    return count;
}

/*
  * converts the Internet host address from the standard numbers-and-dots notation
  * into binary data.
  * returns nonzero if the address is valid, zero if not.	
  */
int rtinet_aton(PSTRING cp, unsigned int *addr)
{
	unsigned int 	val;
	int         	base, n;
	STRING        	c;
	unsigned int    parts[4];
	unsigned int    *pp = parts;

	for (;;)
    {
         /*
          * Collect number up to ``.''. 
          * Values are specified as for C: 
          *	0x=hex, 0=octal, other=decimal.
          */
         val = 0;
         base = 10;
         if (*cp == '0')
         {
             if (*++cp == 'x' || *cp == 'X')
                 base = 16, cp++;
             else
                 base = 8;
         }

	while ((c = *cp) != End_Mark)	 	
         {
             if (isdigit((unsigned char) c))
             {
                 val = (val * base) + (c - '0');
                 cp++;
                 continue;
             }
             if (base == 16 && isxdigit((unsigned char) c))
             {
                 val = (val << 4) +
                     (c + 10 - (islower((unsigned char) c) ? 'a' : 'A'));
                 cp++;
                 continue;
             }
             break;
         }
         if (*cp == '.')
         {
             /*
              * Internet format: a.b.c.d a.b.c   (with c treated as 16-bits)
              * a.b     (with b treated as 24 bits)
              */
             if (pp >= parts + 3 || val > 0xff)
                 return 0;
             *pp++ = val, cp++;
         }
         else
             break;
     }
 
     /*
      * Check for trailing junk.
      */
     while (*cp)
         if (!isspace((unsigned char) *cp++))
             return 0;
 
     /*
      * Concoct the address according to the number of parts specified.
      */
     n = pp - parts + 1;
     switch (n)
     {
 
         case 1:         /* a -- 32 bits */
             break;
 
         case 2:         /* a.b -- 8.24 bits */
             if (val > 0xffffff)
                 return 0;
             val |= parts[0] << 24;
             break;
 
         case 3:         /* a.b.c -- 8.8.16 bits */
             if (val > 0xffff)
                 return 0;
             val |= (parts[0] << 24) | (parts[1] << 16);
             break;
 
         case 4:         /* a.b.c.d -- 8.8.8.8 bits */
             if (val > 0xff)
                 return 0;
             val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
             break;
     }
	      
     *addr = htonl(val);
     return 1;

}

/*
    ========================================================================

    Routine Description:
        Find key section for Get key parameter.

    Arguments:
        buffer                      Pointer to the buffer to start find the key section
        section                     the key of the secion to be find

    Return Value:
        NULL                        Fail
        Others                      Success
    ========================================================================
*/
PSTRING RTMPFindSection(
    IN  PSTRING   buffer)
{
    STRING temp_buf[32];
    PSTRING  ptr;

    strcpy(temp_buf, "Default");

    if((ptr = rtstrstr(buffer, temp_buf)) != NULL)
            return (ptr+strlen("\n"));
        else
            return NULL;
}

/*
    ========================================================================

    Routine Description:
        Get key parameter.

    Arguments:
	key			Pointer to key string
	dest			Pointer to destination      
	destsize		The datasize of the destination
	buffer		Pointer to the buffer to start find the key
	bTrimSpace	Set true if you want to strip the space character of the result pattern
	
    Return Value:
        TRUE                        Success
        FALSE                       Fail

    Note:
	This routine get the value with the matched key (case case-sensitive)
	For SSID and security key related parameters, we SHALL NOT trim the space(' ') character.
    ========================================================================
*/
INT RTMPGetKeyParameter(
    IN PSTRING key,
    OUT PSTRING dest,
    IN INT destsize,
    IN PSTRING buffer,
    IN BOOLEAN bTrimSpace)
{
	PSTRING pMemBuf, temp_buf1 = NULL, temp_buf2 = NULL;
	PSTRING start_ptr, end_ptr;
	PSTRING ptr;
	PSTRING offset = NULL;
	INT  len, keyLen;


	keyLen = strlen(key);
	os_alloc_mem(NULL, (PUCHAR *)&pMemBuf, MAX_PARAM_BUFFER_SIZE  * 2);
	if (pMemBuf == NULL)
		return (FALSE);
	
	memset(pMemBuf, 0, MAX_PARAM_BUFFER_SIZE * 2);
	temp_buf1 = pMemBuf;
	temp_buf2 = (PSTRING)(pMemBuf + MAX_PARAM_BUFFER_SIZE);


	//find section
	if((offset = RTMPFindSection(buffer)) == NULL)
	{
		os_free_mem(NULL, (PUCHAR)pMemBuf);
		return (FALSE);
	}

	strcpy(temp_buf1, "\n");
	strcat(temp_buf1, key);
	strcat(temp_buf1, "=");

	//search key
	if((start_ptr=rtstrstr(offset, temp_buf1)) == NULL)
	{
		os_free_mem(NULL, (PUCHAR)pMemBuf);
		return (FALSE);
	}

	start_ptr += strlen("\n");
	if((end_ptr = rtstrstr(start_ptr, "\n"))==NULL)
		end_ptr = start_ptr+strlen(start_ptr);

	if (end_ptr<start_ptr)
	{
		os_free_mem(NULL, (PUCHAR)pMemBuf);
		return (FALSE);
	}

	NdisMoveMemory(temp_buf2, start_ptr, end_ptr-start_ptr);
	temp_buf2[end_ptr-start_ptr]=End_Mark;

	if((start_ptr=rtstrstr(temp_buf2, "=")) == NULL)
	{
		os_free_mem(NULL, (PUCHAR)pMemBuf);
		return (FALSE);
	}
	ptr = (start_ptr +1);
	//trim special characters, i.e.,  TAB or space
	while(*start_ptr != 0x00)
	{
		if( ((*ptr == ' ') && bTrimSpace) || (*ptr == '\t') )
			ptr++;
		else
			break;
	}
	len = strlen(start_ptr);

	memset(dest, 0x00, destsize);
	strncpy(dest, ptr, ((len >= destsize) ? destsize: len));

	os_free_mem(NULL, (PUCHAR)pMemBuf);
	
	return TRUE;
}


/*
    ========================================================================

    Routine Description:
        Get multiple key parameter.

    Arguments:
        key                         Pointer to key string
        dest                        Pointer to destination      
        destsize                    The datasize of the destination
        buffer                      Pointer to the buffer to start find the key

    Return Value:
        TRUE                        Success
        FALSE                       Fail

    Note:
        This routine get the value with the matched key (case case-sensitive)
    ========================================================================
*/
INT RTMPGetKeyParameterWithOffset(
    IN  PSTRING   key,
    OUT PSTRING   dest,   
    OUT	USHORT	*end_offset,		
    IN  INT     destsize,
    IN  PSTRING   buffer,
    IN	BOOLEAN	bTrimSpace)
{
    PSTRING temp_buf1 = NULL;
    PSTRING temp_buf2 = NULL;
    PSTRING start_ptr;
    PSTRING end_ptr;
    PSTRING ptr;
    PSTRING offset = 0;
    INT  len;

	if (*end_offset >= MAX_INI_BUFFER_SIZE)
		return (FALSE);
	
	os_alloc_mem(NULL, (PUCHAR *)&temp_buf1, MAX_PARAM_BUFFER_SIZE);

	if(temp_buf1 == NULL)
        return (FALSE);
		
	os_alloc_mem(NULL, (PUCHAR *)&temp_buf2, MAX_PARAM_BUFFER_SIZE);
	if(temp_buf2 == NULL)
	{
		os_free_mem(NULL, (PUCHAR)temp_buf1);
        return (FALSE);
	}
	
    //find section		
	if(*end_offset == 0)
    {
		if ((offset = RTMPFindSection(buffer)) == NULL)
		{
			os_free_mem(NULL, (PUCHAR)temp_buf1);
	    	os_free_mem(NULL, (PUCHAR)temp_buf2);
    	    return (FALSE);
		}
    }
	else
		offset = buffer + (*end_offset);	
		
    strcpy(temp_buf1, "\n");
    strcat(temp_buf1, key);
    strcat(temp_buf1, "=");

    //search key
    if((start_ptr=rtstrstr(offset, temp_buf1))==NULL)
    {
		os_free_mem(NULL, (PUCHAR)temp_buf1);
    	os_free_mem(NULL, (PUCHAR)temp_buf2);
        return (FALSE);
    }

    start_ptr+=strlen("\n");
    if((end_ptr=rtstrstr(start_ptr, "\n"))==NULL)
       end_ptr=start_ptr+strlen(start_ptr);
	
    if (end_ptr<start_ptr)
    {
		os_free_mem(NULL, (PUCHAR)temp_buf1);
    	os_free_mem(NULL, (PUCHAR)temp_buf2);
        return (FALSE);
    }

	*end_offset = end_ptr - buffer;

    NdisMoveMemory(temp_buf2, start_ptr, end_ptr-start_ptr);

   temp_buf2[end_ptr-start_ptr]=End_Mark;

    len = strlen(temp_buf2);
    strcpy(temp_buf1, temp_buf2);
    if((start_ptr=rtstrstr(temp_buf1, "=")) == NULL)
    {
		os_free_mem(NULL, (PUCHAR)temp_buf1);
    	os_free_mem(NULL, (PUCHAR)temp_buf2);
        return (FALSE);
    }

    strcpy(temp_buf2, start_ptr+1);
    ptr = temp_buf2;
    //trim space or tab
    while(*ptr != 0x00)
    {
        if((bTrimSpace && (*ptr == ' ')) || (*ptr == '\t') )
            ptr++;
        else
           break;
    }

    len = strlen(ptr);    
    memset(dest, 0x00, destsize);
    strncpy(dest, ptr, len >= destsize ?  destsize: len);

	os_free_mem(NULL, (PUCHAR)temp_buf1);
    os_free_mem(NULL, (PUCHAR)temp_buf2);
    return TRUE;
}


static int rtmp_parse_key_buffer_from_file(IN  PRTMP_ADAPTER pAd,IN  PSTRING buffer,IN  ULONG KeyType,IN  INT BSSIdx,IN  INT KeyIdx)
{
	PSTRING		keybuff;
	//INT			i = BSSIdx, idx = KeyIdx, retVal;
	ULONG		KeyLen;
	//UCHAR		CipherAlg = CIPHER_WEP64;
	CIPHER_KEY	*pSharedKey;
	
	keybuff = buffer;
	KeyLen = strlen(keybuff);
	pSharedKey = &pAd->SharedKey[BSSIdx][KeyIdx];

	if(((KeyType != 0) && (KeyType != 1)) ||
	    ((KeyType == 0) && (KeyLen != 10) && (KeyLen != 26)) ||
	    ((KeyType== 1) && (KeyLen != 5) && (KeyLen != 13)))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Key%dStr is Invalid key length(%ld) or Type(%ld)\n", 
								KeyIdx+1, KeyLen, KeyType));
		return FALSE;
	}
	else
	{
		return RT_CfgSetWepKey(pAd, buffer, pSharedKey, KeyIdx);
	}
	
}


static void rtmp_read_key_parms_from_file(IN  PRTMP_ADAPTER pAd, PSTRING tmpbuf, PSTRING buffer)
{
	STRING		tok_str[16];
	PSTRING		macptr;						
	INT			i = 0, idx;
	ULONG		KeyType[MAX_MBSSID_NUM];
	ULONG		KeyIdx;

	NdisZeroMemory(KeyType, sizeof(KeyType));

	//DefaultKeyID
	if(RTMPGetKeyParameter("DefaultKeyID", tmpbuf, 25, buffer, TRUE))
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			{
				if (i >= pAd->ApCfg.BssidNum)
				{
					break;
				}

				KeyIdx = simple_strtol(macptr, 0, 10);
				if((KeyIdx >= 1 ) && (KeyIdx <= 4))
					pAd->ApCfg.MBSSID[i].DefaultKeyId = (UCHAR) (KeyIdx - 1 );
				else
					pAd->ApCfg.MBSSID[i].DefaultKeyId = 0;

				DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) DefaultKeyID(0~3)=%d\n", i, pAd->ApCfg.MBSSID[i].DefaultKeyId));
			}
		}
#endif // CONFIG_AP_SUPPORT //

	}	   


	for (idx = 0; idx < 4; idx++)
	{
		sprintf(tok_str, "Key%dType", idx + 1);
		//Key1Type
		if (RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer, TRUE))
		{
		    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
		    {
				/*
					do sanity check for KeyType length;
					or in station mode, the KeyType length > 1,
					the code will overwrite the stack of caller
					(RTMPSetProfileParameters) and cause srcbuf = NULL
				*/
				if (i < MAX_MBSSID_NUM)
					KeyType[i] = simple_strtol(macptr, 0, 10);
		    }
#ifdef CONFIG_AP_SUPPORT
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			{
				if (TRUE)
				{
					BOOLEAN bKeyxStryIsUsed = FALSE;
					DBGPRINT(RT_DEBUG_TRACE, ("pAd->ApCfg.BssidNum=%d\n", pAd->ApCfg.BssidNum));

					for (i = 0; i < pAd->ApCfg.BssidNum; i++)
			        	{
						sprintf(tok_str, "Key%dStr%d", idx + 1, i + 1);
					if (RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer, FALSE))
						{
							rtmp_parse_key_buffer_from_file(pAd, tmpbuf, KeyType[i], i, idx);

							if (bKeyxStryIsUsed == FALSE)
							{
								bKeyxStryIsUsed = TRUE;
							}						
						}
					}

					if (bKeyxStryIsUsed == FALSE)
					{
						sprintf(tok_str, "Key%dStr", idx + 1);
					if (RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer, FALSE))
						{
							if (pAd->ApCfg.BssidNum == 1)
							{
								rtmp_parse_key_buffer_from_file(pAd, tmpbuf, KeyType[BSS0], BSS0, idx);
							}
							else
							{
								// Anyway, we still do the legacy dissection of the whole KeyxStr string.
							    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
							    {
									rtmp_parse_key_buffer_from_file(pAd, macptr, KeyType[i], i, idx);
							    }
							}
						}
					}
				}
			}
#endif // CONFIG_AP_SUPPORT //

		}
	}
}

#ifdef CONFIG_AP_SUPPORT 

#ifdef APCLI_SUPPORT
static void rtmp_read_ap_client_from_file(
	IN PRTMP_ADAPTER pAd,
	IN PSTRING tmpbuf,
	IN PSTRING buffer)
{
	PSTRING		macptr;
	INT			i=0, j=0, idx;
	UCHAR		macAddress[MAC_ADDR_LEN];
	UCHAR		keyMaterial[40];
	PAPCLI_STRUCT   pApCliEntry = NULL;
	ULONG		KeyIdx;
	STRING		tok_str[16];
	ULONG		KeyType[MAX_APCLI_NUM];
	ULONG		KeyLen;
	UCHAR		CipherAlg = CIPHER_WEP64;

	//ApCliEnable
	if(RTMPGetKeyParameter("ApCliEnable", tmpbuf, 128, buffer, TRUE))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
		{
			pApCliEntry = &pAd->ApCfg.ApCliTab[i];
			if ((strncmp(macptr, "0", 1) == 0))
				pApCliEntry->Enable = FALSE;
			else if ((strncmp(macptr, "1", 1) == 0))
				pApCliEntry->Enable = TRUE;
	        else
				pApCliEntry->Enable = FALSE;

			if (pApCliEntry->Enable)
			{
				//pApCliEntry->WpaState = SS_NOTUSE;
				//pApCliEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
				//NdisZeroMemory(pApCliEntry->ReplayCounter, LEN_KEY_DESC_REPLAY); 
			}
			DBGPRINT(RT_DEBUG_TRACE, ("ApCliEntry[%d].Enable=%d\n", i, pApCliEntry->Enable));
	    }
	}

	//ApCliSsid
	if(RTMPGetKeyParameter("ApCliSsid", tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer, FALSE))
	{
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++) 
		{
			pApCliEntry = &pAd->ApCfg.ApCliTab[i];

			//Ssid acceptable strlen must be less than 32 and bigger than 0.
			if((strlen(macptr) < 0) || (strlen(macptr) > 32))
				continue; 

			pApCliEntry->CfgSsidLen = strlen(macptr);
			if(pApCliEntry->CfgSsidLen > 0)
			{
				memcpy(&pApCliEntry->CfgSsid, macptr, pApCliEntry->CfgSsidLen);
				pApCliEntry->Valid = FALSE;// it should be set when successfuley association
			} else
			{
				NdisZeroMemory(&(pApCliEntry->CfgSsid), MAX_LEN_OF_SSID);
				continue;
			}
			DBGPRINT(RT_DEBUG_TRACE, ("ApCliEntry[%d].CfgSsidLen=%d, CfgSsid=%s\n", i, pApCliEntry->CfgSsidLen, pApCliEntry->CfgSsid));
		}
	}

	//ApCliBssid
	if(RTMPGetKeyParameter("ApCliBssid", tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer, TRUE))
	{
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++) 
		{
			pApCliEntry = &pAd->ApCfg.ApCliTab[i];

			if(strlen(macptr) != 17)  //Mac address acceptable format 01:02:03:04:05:06 length 17
				continue; 
			if(strcmp(macptr,"00:00:00:00:00:00") == 0)
				continue; 
			if(i >= MAX_APCLI_NUM)
				break; 
			for (j=0; j<ETH_LENGTH_OF_ADDRESS; j++)
			{
				AtoH(macptr, &macAddress[j], 1);
				macptr=macptr+3;
			}	
			memcpy(pApCliEntry->CfgApCliBssid, &macAddress, ETH_LENGTH_OF_ADDRESS);
			pApCliEntry->Valid = FALSE;// it should be set when successfuley association
		}
	}

	//ApCliAuthMode
	if (RTMPGetKeyParameter("ApCliAuthMode", tmpbuf, 255, buffer, TRUE))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
		{
			pApCliEntry = &pAd->ApCfg.ApCliTab[i];
			
			if ((strncmp(macptr, "WEPAUTO", 7) == 0) || (strncmp(macptr, "wepauto", 7) == 0))
				pApCliEntry->AuthMode = Ndis802_11AuthModeAutoSwitch;
			else if ((strncmp(macptr, "SHARED", 6) == 0) || (strncmp(macptr, "shared", 6) == 0))
				pApCliEntry->AuthMode = Ndis802_11AuthModeShared;
			else if ((strncmp(macptr, "WPAPSK", 6) == 0) || (strncmp(macptr, "wpapsk", 6) == 0))
				pApCliEntry->AuthMode = Ndis802_11AuthModeWPAPSK;
			else if ((strncmp(macptr, "WPA2PSK", 7) == 0) || (strncmp(macptr, "wpa2psk", 7) == 0))
				pApCliEntry->AuthMode = Ndis802_11AuthModeWPA2PSK;
			else
				pApCliEntry->AuthMode = Ndis802_11AuthModeOpen;

			//pApCliEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;

			DBGPRINT(RT_DEBUG_TRACE, ("I/F(apcli%d) ApCli_AuthMode=%d \n", i, pApCliEntry->AuthMode));
			RTMPMakeRSNIE(pAd, pApCliEntry->AuthMode, pApCliEntry->WepStatus, (i + MIN_NET_DEVICE_FOR_APCLI));
		}

	}

	//ApCliEncrypType
	if (RTMPGetKeyParameter("ApCliEncrypType", tmpbuf, 255, buffer, TRUE))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
		{
			pApCliEntry = &pAd->ApCfg.ApCliTab[i];

			pApCliEntry->WepStatus = Ndis802_11WEPDisabled;
			if ((strncmp(macptr, "WEP", 3) == 0) || (strncmp(macptr, "wep", 3) == 0))
            {
				if (pApCliEntry->AuthMode < Ndis802_11AuthModeWPA)
					pApCliEntry->WepStatus = Ndis802_11WEPEnabled;				  
			}
			else if ((strncmp(macptr, "TKIP", 4) == 0) || (strncmp(macptr, "tkip", 4) == 0))
			{
				if (pApCliEntry->AuthMode >= Ndis802_11AuthModeWPA)
					pApCliEntry->WepStatus = Ndis802_11Encryption2Enabled;                       
            }
			else if ((strncmp(macptr, "AES", 3) == 0) || (strncmp(macptr, "aes", 3) == 0))
			{
				if (pApCliEntry->AuthMode >= Ndis802_11AuthModeWPA)
					pApCliEntry->WepStatus = Ndis802_11Encryption3Enabled;                            
			}    
			else
			{
				pApCliEntry->WepStatus      = Ndis802_11WEPDisabled;                 
			}

			pApCliEntry->PairCipher     = pApCliEntry->WepStatus;
			pApCliEntry->GroupCipher    = pApCliEntry->WepStatus;
			pApCliEntry->bMixCipher		= FALSE;
			
			//pApCliEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;

			DBGPRINT(RT_DEBUG_TRACE, ("I/F(apcli%d) APCli_EncrypType = %d \n", i, pApCliEntry->WepStatus));
			RTMPMakeRSNIE(pAd, pApCliEntry->AuthMode, pApCliEntry->WepStatus, (i + MIN_NET_DEVICE_FOR_APCLI));
		}

	}
	
	//ApCliWPAPSK
	if (RTMPGetKeyParameter("ApCliWPAPSK", tmpbuf, 255, buffer, FALSE))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
		{
			int retval = TRUE;

			pApCliEntry = &pAd->ApCfg.ApCliTab[i];

			if((strlen(macptr) < 8) || (strlen(macptr) > 64))
			{
				DBGPRINT(RT_DEBUG_ERROR, ("APCli_WPAPSK_KEY, key string required 8 ~ 64 characters!!!\n"));
				continue; 
			}
			
			NdisMoveMemory(pApCliEntry->PSK, macptr, strlen(macptr));
			pApCliEntry->PSKLen = strlen(macptr);
			DBGPRINT(RT_DEBUG_TRACE, ("I/F(apcli%d) APCli_WPAPSK_KEY=%s, Len=%d\n", i, pApCliEntry->PSK, pApCliEntry->PSKLen));

			if ((pApCliEntry->AuthMode != Ndis802_11AuthModeWPAPSK) &&
				(pApCliEntry->AuthMode != Ndis802_11AuthModeWPA2PSK))
			{
				retval = FALSE;
			}

			{
				retval = RT_CfgSetWPAPSKKey(pAd, macptr, (PUCHAR)pApCliEntry->CfgSsid, (INT)pApCliEntry->CfgSsidLen, pApCliEntry->PMK);
			}
			if (retval == TRUE)
			{
				// Start STA supplicant WPA state machine
				DBGPRINT(RT_DEBUG_TRACE, ("Start AP-client WPAPSK state machine \n"));
				//pApCliEntry->WpaState = SS_START;				
			}

			//RTMPMakeRSNIE(pAd, pApCliEntry->AuthMode, pApCliEntry->WepStatus, (i + MIN_NET_DEVICE_FOR_APCLI));			
#ifdef DBG
			DBGPRINT(RT_DEBUG_TRACE, ("I/F(apcli%d) PMK Material => \n", i));
			
			for (j = 0; j < 32; j++)
			{
				DBGPRINT(RT_DEBUG_OFF, ("%02x:", pApCliEntry->PMK[j]));
				if ((j%16) == 15)
					DBGPRINT(RT_DEBUG_OFF, ("\n"));
			}
			DBGPRINT(RT_DEBUG_OFF,("\n"));
#endif
		}
	}

	//ApCliDefaultKeyID
	if (RTMPGetKeyParameter("ApCliDefaultKeyID", tmpbuf, 255, buffer, TRUE))
	{
		for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
		{
			pApCliEntry = &pAd->ApCfg.ApCliTab[i];
			
			KeyIdx = simple_strtol(macptr, 0, 10);
			if((KeyIdx >= 1 ) && (KeyIdx <= 4))
				pApCliEntry->DefaultKeyId = (UCHAR) (KeyIdx - 1);
			else
				pApCliEntry->DefaultKeyId = 0;

			DBGPRINT(RT_DEBUG_TRACE, ("I/F(apcli%d) DefaultKeyID(0~3)=%d\n", i, pApCliEntry->DefaultKeyId));
		}
	}

	//ApCliKeyXType, ApCliKeyXStr
	for (idx=0; idx<4; idx++)
	{
		sprintf(tok_str, "ApCliKey%dType", idx+1);
		//ApCliKey1Type
		if(RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer, TRUE))
		{
			for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
			{
			    KeyType[i] = simple_strtol(macptr, 0, 10);
			}

			sprintf(tok_str, "ApCliKey%dStr", idx+1);
			//ApCliKey1Str
			if(RTMPGetKeyParameter(tok_str, tmpbuf, 512, buffer, FALSE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_APCLI_NUM); macptr = rstrtok(NULL,";"), i++)
				{
					pApCliEntry = &pAd->ApCfg.ApCliTab[i];
					KeyLen = strlen(macptr);
					if(((KeyType[i] == 0) && (KeyLen != 10) && (KeyLen != 26)) ||
					    ((KeyType[i] != 0) && (KeyLen != 5) && (KeyLen != 13)))
					{
						DBGPRINT(RT_DEBUG_ERROR, ("I/F(apcli%d) Key%dStr is Invalid key length!\n", i, idx+1));
					}
					else
					{
						RT_CfgSetWepKey(pAd, macptr, &pApCliEntry->SharedKey[idx], idx);
					}
				}
			}
		}
	}
	
#ifdef WSC_AP_SUPPORT
#endif // WSC_AP_SUPPORT //	
}
#endif // APCLI_SUPPORT //

static void rtmp_read_acl_parms_from_file(IN  PRTMP_ADAPTER pAd, PSTRING tmpbuf, PSTRING buffer)
{
	STRING		tok_str[32];
	PSTRING		macptr;						
	INT			i=0, j=0, idx;
	UCHAR		macAddress[MAC_ADDR_LEN];
											  
	for (idx=0; idx<MAX_MBSSID_NUM; idx++)
	{
		memset(&pAd->ApCfg.MBSSID[idx].AccessControlList, 0, sizeof(RT_802_11_ACL));
		// AccessPolicyX
		sprintf(tok_str, "AccessPolicy%d", idx);
		if (RTMPGetKeyParameter(tok_str, tmpbuf, 10, buffer, TRUE))
		{
			switch (simple_strtol(tmpbuf, 0, 10))
			{
				case 1: // Allow All, and the AccessControlList is positive now.
					pAd->ApCfg.MBSSID[idx].AccessControlList.Policy = 1;
					break;
				case 2: // Reject All, and the AccessControlList is negative now.
					pAd->ApCfg.MBSSID[idx].AccessControlList.Policy = 2;
					break;
				case 0: // Disable, don't care the AccessControlList.
				default:
					pAd->ApCfg.MBSSID[idx].AccessControlList.Policy = 0;
					break;
			}
			DBGPRINT(RT_DEBUG_TRACE, ("%s=%ld\n", tok_str, pAd->ApCfg.MBSSID[idx].AccessControlList.Policy));
		}
		// AccessControlListX
		sprintf(tok_str, "AccessControlList%d", idx);
		if (RTMPGetKeyParameter(tok_str, tmpbuf, MAX_PARAM_BUFFER_SIZE, buffer, TRUE))
		{
			for (i=0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++) 
			{
				if (strlen(macptr) != 17)  // Mac address acceptable format 01:02:03:04:05:06 length 17
					continue;

				ASSERT(pAd->ApCfg.MBSSID[idx].AccessControlList.Num <= MAX_NUM_OF_ACL_LIST);
				
				if (pAd->ApCfg.MBSSID[idx].AccessControlList.Num == MAX_NUM_OF_ACL_LIST)
				{
					DBGPRINT(RT_DEBUG_WARN, ("The AccessControlList is full, and no more entry can join the list!\n"));
        			DBGPRINT(RT_DEBUG_WARN, ("The last entry of ACL is %02x:%02x:%02x:%02x:%02x:%02x\n",
        				macAddress[0],macAddress[1],macAddress[2],macAddress[3],macAddress[4],macAddress[5]));

				    break;
				}
				for (j=0; j<ETH_LENGTH_OF_ADDRESS; j++)
				{
					AtoH(macptr, &macAddress[j], 1);
					macptr=macptr+3;
				}
				
				pAd->ApCfg.MBSSID[idx].AccessControlList.Num++;
				NdisMoveMemory(pAd->ApCfg.MBSSID[idx].AccessControlList.Entry[(pAd->ApCfg.MBSSID[idx].AccessControlList.Num - 1)].Addr, macAddress, ETH_LENGTH_OF_ADDRESS);				
			}
			DBGPRINT(RT_DEBUG_TRACE, ("%s=Get %ld Mac Address\n", tok_str, pAd->ApCfg.MBSSID[idx].AccessControlList.Num));
 		}
	}
}

/*
    ========================================================================

    Routine Description:
        In kernel mode read parameters from file

    Arguments:
        src                     the location of the file.
        dest                        put the parameters to the destination.
        Length                  size to read.

    Return Value:
        None

    Note:

    ========================================================================
*/
static void rtmp_read_ap_wmm_parms_from_file(IN  PRTMP_ADAPTER pAd, PSTRING tmpbuf, PSTRING buffer)
{
	PSTRING					macptr;						
	INT						i=0;

	//WmmCapable
	if(RTMPGetKeyParameter("WmmCapable", tmpbuf, 32, buffer, TRUE))
	{
	    BOOLEAN bEnableWmm = FALSE;
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			if (i >= pAd->ApCfg.BssidNum)
			{
				break;
			}

			if(simple_strtol(macptr, 0, 10) != 0)  //Enable
			{
				pAd->ApCfg.MBSSID[i].bWmmCapable = TRUE;
				bEnableWmm = TRUE;
			}
			else //Disable
			{
				pAd->ApCfg.MBSSID[i].bWmmCapable = FALSE;
			}
			if (bEnableWmm)
			{
				pAd->CommonCfg.APEdcaParm.bValid = TRUE;
				pAd->ApCfg.BssEdcaParm.bValid = TRUE;
			}
			else
				{
				pAd->CommonCfg.APEdcaParm.bValid = FALSE;
				pAd->ApCfg.BssEdcaParm.bValid = FALSE;
			}

			pAd->ApCfg.MBSSID[i].bWmmCapableOrg = \
										pAd->ApCfg.MBSSID[i].bWmmCapable;
										
			DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) WmmCapable=%d\n", i, pAd->ApCfg.MBSSID[i].bWmmCapable));
	    }
	}
	//DLSCapable
	if(RTMPGetKeyParameter("DLSCapable", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			if (i >= pAd->ApCfg.BssidNum)
			{
				break;
			}

			if(simple_strtol(macptr, 0, 10) != 0)  //Enable
			{
				pAd->ApCfg.MBSSID[i].bDLSCapable = TRUE;
			}
			else //Disable
			{
				pAd->ApCfg.MBSSID[i].bDLSCapable = FALSE;
			}

			DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) DLSCapable=%d\n", i, pAd->ApCfg.MBSSID[i].bDLSCapable));
	    }
	}
	//APAifsn
	if(RTMPGetKeyParameter("APAifsn", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->CommonCfg.APEdcaParm.Aifsn[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("APAifsn[%d]=%d\n", i, pAd->CommonCfg.APEdcaParm.Aifsn[i]));
	    }
	}
	//APCwmin
	if(RTMPGetKeyParameter("APCwmin", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->CommonCfg.APEdcaParm.Cwmin[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("APCwmin[%d]=%d\n", i, pAd->CommonCfg.APEdcaParm.Cwmin[i]));
	    }
	}
	//APCwmax
	if(RTMPGetKeyParameter("APCwmax", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->CommonCfg.APEdcaParm.Cwmax[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("APCwmax[%d]=%d\n", i, pAd->CommonCfg.APEdcaParm.Cwmax[i]));
	    }
	}
	//APTxop
	if(RTMPGetKeyParameter("APTxop", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->CommonCfg.APEdcaParm.Txop[i] = (USHORT) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("APTxop[%d]=%d\n", i, pAd->CommonCfg.APEdcaParm.Txop[i]));
	    }
	}
	//APACM
	if(RTMPGetKeyParameter("APACM", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->CommonCfg.APEdcaParm.bACM[i] = (BOOLEAN) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("APACM[%d]=%d\n", i, pAd->CommonCfg.APEdcaParm.bACM[i]));
	    }
	}
	//BSSAifsn
	if(RTMPGetKeyParameter("BSSAifsn", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->ApCfg.BssEdcaParm.Aifsn[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("BSSAifsn[%d]=%d\n", i, pAd->ApCfg.BssEdcaParm.Aifsn[i]));
	    }
	}
	//BSSCwmin
	if(RTMPGetKeyParameter("BSSCwmin", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->ApCfg.BssEdcaParm.Cwmin[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("BSSCwmin[%d]=%d\n", i, pAd->ApCfg.BssEdcaParm.Cwmin[i]));
	    }
	}
	//BSSCwmax
	if(RTMPGetKeyParameter("BSSCwmax", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->ApCfg.BssEdcaParm.Cwmax[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("BSSCwmax[%d]=%d\n", i, pAd->ApCfg.BssEdcaParm.Cwmax[i]));
	    }
	}
	//BSSTxop
	if(RTMPGetKeyParameter("BSSTxop", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->ApCfg.BssEdcaParm.Txop[i] = (USHORT) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("BSSTxop[%d]=%d\n", i, pAd->ApCfg.BssEdcaParm.Txop[i]));
	    }
	}
	//BSSACM
	if(RTMPGetKeyParameter("BSSACM", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->ApCfg.BssEdcaParm.bACM[i] = (BOOLEAN) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("BSSACM[%d]=%d\n", i, pAd->ApCfg.BssEdcaParm.bACM[i]));
	    }
	}
	//AckPolicy
	if(RTMPGetKeyParameter("AckPolicy", tmpbuf, 32, buffer, TRUE))
	{
	    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
	    {
			pAd->CommonCfg.AckPolicy[i] = (UCHAR) simple_strtol(macptr, 0, 10);;

			DBGPRINT(RT_DEBUG_TRACE, ("AckPolicy[%d]=%d\n", i, pAd->CommonCfg.AckPolicy[i]));
	    }
	}
#ifdef UAPSD_AP_SUPPORT
	//APSDCapable
	if(RTMPGetKeyParameter("APSDCapable", tmpbuf, 10, buffer, TRUE))
	{
		if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
			pAd->CommonCfg.bAPSDCapable = TRUE;
		else
			pAd->CommonCfg.bAPSDCapable = FALSE;

		DBGPRINT(RT_DEBUG_TRACE, ("APSDCapable=%d\n", pAd->CommonCfg.bAPSDCapable));
	}
#endif // UAPSD_AP_SUPPORT //
#ifdef RTL865X_SOC
	//EthWithVLANTag
	if(RTMPGetKeyParameter("EthWithVLANTag", tmpbuf, 10, buffer, TRUE))
	{
		if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
			pAd->CommonCfg.bEthWithVLANTag = TRUE;
		else
			pAd->CommonCfg.bEthWithVLANTag = FALSE;

		DBGPRINT(RT_DEBUG_TRACE, ("bEthWithVLANTag=%d\n", pAd->CommonCfg.bEthWithVLANTag));
	}
#endif // RTL865X_SOC //
}

#ifdef DOT1X_SUPPORT
/*
    ========================================================================

    Routine Description:
        In kernel mode read parameters from file

    Arguments:
        src                     the location of the file.
        dest                        put the parameters to the destination.
        Length                  size to read.

    Return Value:
        None

    Note:

    ========================================================================
*/
static void rtmp_read_radius_parms_from_file(IN  PRTMP_ADAPTER pAd, PSTRING tmpbuf, PSTRING buffer)
{
	STRING					tok_str[16];
	PSTRING					macptr;		
	UINT32					ip_addr;
	INT						i=0;
	BOOLEAN					bUsePrevFormat = FALSE;
	USHORT					offset;
	INT						count[MAX_MBSSID_NUM];

	// own_ip_addr
	if (RTMPGetKeyParameter("own_ip_addr", tmpbuf, 32, buffer, TRUE))
	{
		Set_OwnIPAddr_Proc(pAd, tmpbuf);
	}

	// radius_retry_primary_interval
	if (RTMPGetKeyParameter("radius_retry_primary_interval", tmpbuf, 32, buffer, TRUE))
	{
		pAd->ApCfg.retry_interval = simple_strtol(tmpbuf, 0, 10); 
		DBGPRINT(RT_DEBUG_TRACE, ("radius_retry_primary_interval=%d\n", pAd->ApCfg.retry_interval));
	} 

	// session_timeout_interval
	if (RTMPGetKeyParameter("session_timeout_interval", tmpbuf, 32, buffer, TRUE))
	{
		pAd->ApCfg.session_timeout_interval = simple_strtol(tmpbuf, 0, 10); 
		DBGPRINT(RT_DEBUG_TRACE, ("session_timeout_interval=%d\n", pAd->ApCfg.session_timeout_interval));
	} 

	// quiet_interval
	if (RTMPGetKeyParameter("quiet_interval", tmpbuf, 32, buffer, TRUE))
	{
		pAd->ApCfg.quiet_interval = simple_strtol(tmpbuf, 0, 10); 
		DBGPRINT(RT_DEBUG_TRACE, ("quiet_interval=%d\n", pAd->ApCfg.quiet_interval));
	} 

	// EAPifname
	if (RTMPGetKeyParameter("EAPifname", tmpbuf, 256, buffer, TRUE))
	{
		Set_EAPIfName_Proc(pAd, tmpbuf);
	}
	
	// PreAuthifname
	if (RTMPGetKeyParameter("PreAuthifname", tmpbuf, 256, buffer, TRUE))
	{
		Set_PreAuthIfName_Proc(pAd, tmpbuf);
	}
	
	// RADIUS_Server
	offset = 0;
	//if (RTMPGetKeyParameter("RADIUS_Server", tmpbuf, 256, buffer, TRUE))
	while (RTMPGetKeyParameterWithOffset("RADIUS_Server", tmpbuf, &offset, 256, buffer, TRUE))	
	{
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_MBSSID_NUM); macptr = rstrtok(NULL,";"), i++) 
		{
			if (rtinet_aton(macptr, &ip_addr) && pAd->ApCfg.MBSSID[i].radius_srv_num < MAX_RADIUS_SRV_NUM)
			{
				INT	srv_idx = pAd->ApCfg.MBSSID[i].radius_srv_num;

				pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_ip = ip_addr;
				pAd->ApCfg.MBSSID[i].radius_srv_num++;
				DBGPRINT(RT_DEBUG_TRACE, ("IF(ra%d), radius_ip(seq-%d)=%s(%x)\n", i, pAd->ApCfg.MBSSID[i].radius_srv_num, macptr, pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_ip));
			}	    
		}
	}
	// RADIUS_Port
	//if (RTMPGetKeyParameter("RADIUS_Port", tmpbuf, 128, buffer, TRUE))
	offset = 0;
	memset(&count[0], 0, sizeof(count));
	while (RTMPGetKeyParameterWithOffset("RADIUS_Port", tmpbuf, &offset, 128, buffer, TRUE))
	{
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_MBSSID_NUM); macptr = rstrtok(NULL,";"), i++) 
		{	  
			if (count[i] < pAd->ApCfg.MBSSID[i].radius_srv_num)
			{		
				INT		srv_idx = count[i];
				
            	pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_port = (UINT32) simple_strtol(macptr, 0, 10); 
				count[i] ++;
				DBGPRINT(RT_DEBUG_TRACE, ("IF(ra%d), radius_port(seq-%d)=%d\n", i, count[i], pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_port));
			}
		}
	}
	// RADIUS_Key
	//if (RTMPGetKeyParameter("RADIUS_Key", tmpbuf, 640, buffer, FALSE))
	offset = 0;
	memset(&count[0], 0, sizeof(count));
	while (RTMPGetKeyParameterWithOffset("RADIUS_Key", tmpbuf, &offset, 640, buffer, FALSE))
	{
		if (strlen(tmpbuf) > 0)
			bUsePrevFormat = TRUE;
	
		for (i=0, macptr = rstrtok(tmpbuf,";"); (macptr && i < MAX_MBSSID_NUM); macptr = rstrtok(NULL,";"), i++) 
		{	  
			if (strlen(macptr) > 0 && (count[i] < pAd->ApCfg.MBSSID[i].radius_srv_num))
			{
				INT		srv_idx = count[i];
			
				pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key_len = strlen(macptr); 
				NdisMoveMemory(pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key, macptr, strlen(macptr));
				count[i] ++;
				DBGPRINT(RT_DEBUG_TRACE, ("IF(ra%d), radius_key(seq-%d)=%s, len=%d\n", i, 
															count[i],
															pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key, 
															pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key_len));
			}
		}
	}

	/* NasIdX, X indicate the interface index(1~8) */		
	for (i = 0; i < pAd->ApCfg.BssidNum; i++)
	{
		sprintf(tok_str, "NasId%d", i + 1);
		if (RTMPGetKeyParameter(tok_str, tmpbuf, 33, buffer, FALSE))
		{
			if (strlen(tmpbuf) > 0)
			{
				pAd->ApCfg.MBSSID[i].NasIdLen = strlen(tmpbuf); 
				NdisMoveMemory(pAd->ApCfg.MBSSID[i].NasId, tmpbuf, strlen(tmpbuf));
				DBGPRINT(RT_DEBUG_TRACE, ("IF-ra%d NAS-ID=%s, len=%d\n", i, 
												pAd->ApCfg.MBSSID[i].NasId, 
												pAd->ApCfg.MBSSID[i].NasIdLen));
			}					
		}
	}
	
	if (!bUsePrevFormat)
	{
		for (i = 0; i < MAX_MBSSID_NUM; i++)
		{
			INT	srv_idx = 0;
			
			sprintf(tok_str, "RADIUS_Key%d", i + 1);
			
			// RADIUS_KeyX (X=1~MAX_MBSSID_NUM)
			//if (RTMPGetKeyParameter(tok_str, tmpbuf, 128, buffer, FALSE))			
			offset = 0;
			while (RTMPGetKeyParameterWithOffset(tok_str, tmpbuf, &offset, 128, buffer, FALSE))
			{
				if (strlen(tmpbuf) > 0 && (srv_idx < pAd->ApCfg.MBSSID[i].radius_srv_num))
				{
					pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key_len = strlen(tmpbuf); 
					NdisMoveMemory(pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key, tmpbuf, strlen(tmpbuf));					
					DBGPRINT(RT_DEBUG_TRACE, ("IF(ra%d), update radius_key(seq-%d)=%s, len=%d\n", i, srv_idx+1,
																pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key, 
																pAd->ApCfg.MBSSID[i].radius_srv_info[srv_idx].radius_key_len));
					srv_idx ++;
				}	
			}
		}
	}
}
#endif // DOT1X_SUPPORT //

static int rtmp_parse_wpapsk_buffer_from_file(IN  PRTMP_ADAPTER pAd,IN  PSTRING buffer,IN  INT BSSIdx)
{
	PSTRING		tmpbuf = buffer;
	INT			i = BSSIdx;
	//UCHAR		keyMaterial[40];
#ifdef WSC_AP_SUPPORT	
	ULONG		len = strlen(tmpbuf);
#endif // WSC_AP_SUPPORT //
	int         ret = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) WPAPSK_KEY=%s\n", i, tmpbuf));

	ret = RT_CfgSetWPAPSKKey(pAd, tmpbuf, (PUCHAR)pAd->ApCfg.MBSSID[i].Ssid, pAd->ApCfg.MBSSID[i].SsidLen, pAd->ApCfg.MBSSID[i].PMK);
	if (ret == FALSE)
		return FALSE;

#ifdef WSC_AP_SUPPORT
	NdisZeroMemory(pAd->ApCfg.MBSSID[i].WscControl.WpaPsk, 64);
	pAd->ApCfg.MBSSID[i].WscControl.WpaPskLen = 0;
	if ((len >= 8) && (len <= 64))
	{                                    
		NdisMoveMemory(pAd->ApCfg.MBSSID[i].WscControl.WpaPsk, tmpbuf, len);
		pAd->ApCfg.MBSSID[i].WscControl.WpaPskLen = len;
	}
#endif // WSC_AP_SUPPORT //
	return ret;
}
#endif // CONFIG_AP_SUPPORT //




#ifdef DOT11_N_SUPPORT
static void HTParametersHook(
	IN	PRTMP_ADAPTER pAd, 
	IN	PSTRING		  pValueStr,
	IN	PSTRING		  pInput)
{

	long Value;
#ifdef CONFIG_AP_SUPPORT	
	INT			i=0;
	PSTRING		Bufptr;
#endif // CONFIG_AP_SUPPORT //

    if (RTMPGetKeyParameter("HT_PROTECT", pValueStr, 25, pInput, TRUE))
    {
        Value = simple_strtol(pValueStr, 0, 10);
        if (Value == 0)
        {
            pAd->CommonCfg.bHTProtect = FALSE;
        }
        else
        {
            pAd->CommonCfg.bHTProtect = TRUE;
        }
        DBGPRINT(RT_DEBUG_TRACE, ("HT: Protection  = %s\n", (Value==0) ? "Disable" : "Enable"));
    }

    if (RTMPGetKeyParameter("HT_MIMOPSEnable", pValueStr, 25, pInput, TRUE))
    {
        Value = simple_strtol(pValueStr, 0, 10);
        if (Value == 0)
        {
            pAd->CommonCfg.bMIMOPSEnable = FALSE;
        }
        else
        {
            pAd->CommonCfg.bMIMOPSEnable = TRUE;
        }
        DBGPRINT(RT_DEBUG_TRACE, ("HT: MIMOPSEnable  = %s\n", (Value==0) ? "Disable" : "Enable"));
    }


    if (RTMPGetKeyParameter("HT_MIMOPSMode", pValueStr, 25, pInput, TRUE))
    {
        Value = simple_strtol(pValueStr, 0, 10);
        if (Value > MMPS_ENABLE)
        {
			pAd->CommonCfg.BACapability.field.MMPSmode = MMPS_ENABLE;
        }
        else
        {
            //TODO: add mimo power saving mechanism
            pAd->CommonCfg.BACapability.field.MMPSmode = MMPS_ENABLE;
			//pAd->CommonCfg.BACapability.field.MMPSmode = Value;
        }
        DBGPRINT(RT_DEBUG_TRACE, ("HT: MIMOPS Mode  = %d\n", (INT) Value));
    }

    if (RTMPGetKeyParameter("HT_BADecline", pValueStr, 25, pInput, TRUE))
    {
        Value = simple_strtol(pValueStr, 0, 10);
        if (Value == 0)
        {
            pAd->CommonCfg.bBADecline = FALSE;
        }
        else
        {
            pAd->CommonCfg.bBADecline = TRUE;
        }
        DBGPRINT(RT_DEBUG_TRACE, ("HT: BA Decline  = %s\n", (Value==0) ? "Disable" : "Enable"));
    }


    if (RTMPGetKeyParameter("HT_DisableReordering", pValueStr, 25, pInput, TRUE))
    {
        Value = simple_strtol(pValueStr, 0, 10);
        if (Value == 0)
        {
            pAd->CommonCfg.bDisableReordering = FALSE;
        }
        else
        {
            pAd->CommonCfg.bDisableReordering = TRUE;
        }
        DBGPRINT(RT_DEBUG_TRACE, ("HT: DisableReordering  = %s\n", (Value==0) ? "Disable" : "Enable"));
    }

    if (RTMPGetKeyParameter("HT_AutoBA", pValueStr, 25, pInput, TRUE))
    {
        Value = simple_strtol(pValueStr, 0, 10);
        if (Value == 0)
        {
            pAd->CommonCfg.BACapability.field.AutoBA = FALSE;
			pAd->CommonCfg.BACapability.field.Policy = BA_NOTUSE;
        }
        else
        {
            pAd->CommonCfg.BACapability.field.AutoBA = TRUE;
			pAd->CommonCfg.BACapability.field.Policy = IMMED_BA;
        }
        pAd->CommonCfg.REGBACapability.field.AutoBA = pAd->CommonCfg.BACapability.field.AutoBA;
		pAd->CommonCfg.REGBACapability.field.Policy = pAd->CommonCfg.BACapability.field.Policy;
        DBGPRINT(RT_DEBUG_TRACE, ("HT: Auto BA  = %s\n", (Value==0) ? "Disable" : "Enable"));
    }

	// Tx_+HTC frame
    if (RTMPGetKeyParameter("HT_HTC", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == 0)
		{
			pAd->HTCEnable = FALSE;
		}
		else
		{
            pAd->HTCEnable = TRUE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Tx +HTC frame = %s\n", (Value==0) ? "Disable" : "Enable"));
	}

	// Enable HT Link Adaptation Control
	if (RTMPGetKeyParameter("HT_LinkAdapt", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == 0)
		{
			pAd->bLinkAdapt = FALSE;
		}
		else
		{
			pAd->HTCEnable = TRUE;
			pAd->bLinkAdapt = TRUE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Link Adaptation Control = %s\n", (Value==0) ? "Disable" : "Enable(+HTC)"));
	}

	// Reverse Direction Mechanism
    if (RTMPGetKeyParameter("HT_RDG", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == 0)
		{			
			pAd->CommonCfg.bRdg = FALSE;
		}
		else
		{
			pAd->HTCEnable = TRUE;
            pAd->CommonCfg.bRdg = TRUE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: RDG = %s\n", (Value==0) ? "Disable" : "Enable(+HTC)"));
	}




	// Tx A-MSUD ?
    if (RTMPGetKeyParameter("HT_AMSDU", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == 0)
		{
			pAd->CommonCfg.BACapability.field.AmsduEnable = FALSE;
		}
		else
		{
            pAd->CommonCfg.BACapability.field.AmsduEnable = TRUE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Tx A-MSDU = %s\n", (Value==0) ? "Disable" : "Enable"));
	}

	// MPDU Density
    if (RTMPGetKeyParameter("HT_MpduDensity", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value <=7 && Value >= 0)
		{		
			pAd->CommonCfg.BACapability.field.MpduDensity = Value;
			DBGPRINT(RT_DEBUG_TRACE, ("HT: MPDU Density = %d\n", (INT) Value));
		}
		else
		{
			pAd->CommonCfg.BACapability.field.MpduDensity = 4;
			DBGPRINT(RT_DEBUG_TRACE, ("HT: MPDU Density = %d (Default)\n", 4));
		}
	}

	// Max Rx BA Window Size
    if (RTMPGetKeyParameter("HT_BAWinSize", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);

#ifdef CONFIG_AP_SUPPORT
		// Intel IOT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		Value = 64;
#endif // CONFIG_AP_SUPPORT //
		if (Value >=1 && Value <= 64)
		{		
			pAd->CommonCfg.REGBACapability.field.RxBAWinLimit = Value;
			pAd->CommonCfg.BACapability.field.RxBAWinLimit = Value;
			DBGPRINT(RT_DEBUG_TRACE, ("HT: BA Windw Size = %d\n", (INT) Value));
		}
		else
		{
            pAd->CommonCfg.REGBACapability.field.RxBAWinLimit = 64;
			pAd->CommonCfg.BACapability.field.RxBAWinLimit = 64;
			DBGPRINT(RT_DEBUG_TRACE, ("HT: BA Windw Size = 64 (Defualt)\n"));
		}

	}

	// Guard Interval
	if (RTMPGetKeyParameter("HT_GI", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);

		if (Value == GI_400)
		{
			pAd->CommonCfg.RegTransmitSetting.field.ShortGI = GI_400;
		}
		else
		{
			pAd->CommonCfg.RegTransmitSetting.field.ShortGI = GI_800;
		}
		
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Guard Interval = %s\n", (Value==GI_400) ? "400" : "800" ));
	}

	// HT Operation Mode : Mixed Mode , Green Field
	if (RTMPGetKeyParameter("HT_OpMode", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);

		if (Value == HTMODE_GF)
		{

			pAd->CommonCfg.RegTransmitSetting.field.HTMODE  = HTMODE_GF;
		}
		else
		{
			pAd->CommonCfg.RegTransmitSetting.field.HTMODE  = HTMODE_MM;
		}		

		DBGPRINT(RT_DEBUG_TRACE, ("HT: Operate Mode = %s\n", (Value==HTMODE_GF) ? "Green Field" : "Mixed Mode" ));
	}

	// Fixed Tx mode : CCK, OFDM
	if (RTMPGetKeyParameter("FixedTxMode", pValueStr, 25, pInput, TRUE))
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			for (i = 0, Bufptr = rstrtok(pValueStr,";"); (Bufptr && i < MAX_MBSSID_NUM); Bufptr = rstrtok(NULL,";"), i++) 	
			{
				pAd->ApCfg.MBSSID[i].DesiredTransmitSetting.field.FixedTxMode = 
														RT_CfgSetFixedTxPhyMode(Bufptr);																	
				DBGPRINT(RT_DEBUG_TRACE, ("(IF-ra%d) Fixed Tx Mode = %d\n", i, 
											pAd->ApCfg.MBSSID[i].DesiredTransmitSetting.field.FixedTxMode));							
			}
		}
#endif // CONFIG_AP_SUPPORT //
	}


	// Channel Width
	if (RTMPGetKeyParameter("HT_BW", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);

		if (Value == BW_40)
		{
			pAd->CommonCfg.RegTransmitSetting.field.BW  = BW_40;
		}
		else
		{
            pAd->CommonCfg.RegTransmitSetting.field.BW  = BW_20;
		}		

#ifdef MCAST_RATE_SPECIFIC
		pAd->CommonCfg.MCastPhyMode.field.BW = pAd->CommonCfg.RegTransmitSetting.field.BW;
#endif // MCAST_RATE_SPECIFIC //

		DBGPRINT(RT_DEBUG_TRACE, ("HT: Channel Width = %s\n", (Value==BW_40) ? "40 MHz" : "20 MHz" ));
	}

	if (RTMPGetKeyParameter("HT_EXTCHA", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);

		if (Value == 0)
		{
			
			pAd->CommonCfg.RegTransmitSetting.field.EXTCHA  = EXTCHA_BELOW;
		}
		else
		{
            pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_ABOVE;
		}		

		DBGPRINT(RT_DEBUG_TRACE, ("HT: Ext Channel = %s\n", (Value==0) ? "BELOW" : "ABOVE" ));
	}

	// MSC
	if (RTMPGetKeyParameter("HT_MCS", pValueStr, 50, pInput, TRUE))
	{
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			for (i = 0, Bufptr = rstrtok(pValueStr,";"); (Bufptr && i < MAX_MBSSID_NUM); Bufptr = rstrtok(NULL,";"), i++) 	
			{
				Value = simple_strtol(Bufptr, 0, 10);			
//				if ((Value >= 0 && Value <= 15) || (Value == 32))
				if ((Value >= 0 && Value <= 23) || (Value == 32)) // 3*3
				{
					pAd->ApCfg.MBSSID[i].DesiredTransmitSetting.field.MCS  = Value;
					DBGPRINT(RT_DEBUG_TRACE, ("(IF-ra%d) HT: MCS = %d\n", i, pAd->ApCfg.MBSSID[i].DesiredTransmitSetting.field.MCS));							
				}
				else
				{
					pAd->ApCfg.MBSSID[i].DesiredTransmitSetting.field.MCS  = MCS_AUTO;
					DBGPRINT(RT_DEBUG_TRACE, ("(IF-ra%d) HT: MCS is AUTO\n", i));							
				}		
			}		
		}
#endif // CONFIG_AP_SUPPORT //

	}

	// STBC 
    if (RTMPGetKeyParameter("HT_STBC", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == STBC_USE)
		{		
			pAd->CommonCfg.RegTransmitSetting.field.STBC = STBC_USE;
		}
		else
		{
			pAd->CommonCfg.RegTransmitSetting.field.STBC = STBC_NONE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: STBC = %d\n", pAd->CommonCfg.RegTransmitSetting.field.STBC));
	}

	// 40_Mhz_Intolerant
	if (RTMPGetKeyParameter("HT_40MHZ_INTOLERANT", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == 0)
		{		
			pAd->CommonCfg.bForty_Mhz_Intolerant = FALSE;
		}
		else
		{
			pAd->CommonCfg.bForty_Mhz_Intolerant = TRUE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: 40MHZ INTOLERANT = %d\n", pAd->CommonCfg.bForty_Mhz_Intolerant));
	}
	//HT_TxStream
	if(RTMPGetKeyParameter("HT_TxStream", pValueStr, 10, pInput, TRUE))
	{
		switch (simple_strtol(pValueStr, 0, 10))
		{
			case 1:
				pAd->CommonCfg.TxStream = 1;
				break;
			case 2:
				pAd->CommonCfg.TxStream = 2;
				break;
			case 3: // 3*3
			default:
				pAd->CommonCfg.TxStream = 3;

				if (pAd->MACVersion < RALINK_2883_VERSION)
					pAd->CommonCfg.TxStream = 2; // only 2 tx streams for RT2860 series
				break;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Tx Stream = %d\n", pAd->CommonCfg.TxStream));
	}
	//HT_RxStream
	if(RTMPGetKeyParameter("HT_RxStream", pValueStr, 10, pInput, TRUE))
	{
		switch (simple_strtol(pValueStr, 0, 10))
		{
			case 1:
				pAd->CommonCfg.RxStream = 1;
				break;
			case 2:
				pAd->CommonCfg.RxStream = 2;
				break;
			case 3:
			default:
				pAd->CommonCfg.RxStream = 3;

				if (pAd->MACVersion < RALINK_2883_VERSION)
					pAd->CommonCfg.RxStream = 2; // only 2 rx streams for RT2860 series
				break;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Rx Stream = %d\n", pAd->CommonCfg.RxStream));
	}
	//2008/11/05: KH add to support Antenna power-saving of AP<--
	//Green AP
	if(RTMPGetKeyParameter("GreenAP", pValueStr, 10, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);
		if (Value == 0)
		{		
			pAd->CommonCfg.bGreenAPEnable = FALSE;
		}
		else
		{
			pAd->CommonCfg.bGreenAPEnable = TRUE;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("HT: Green AP= %d\n", pAd->CommonCfg.bGreenAPEnable));
	}

	// HT_DisallowTKIP
	if (RTMPGetKeyParameter("HT_DisallowTKIP", pValueStr, 25, pInput, TRUE))
	{
		Value = simple_strtol(pValueStr, 0, 10);

		if (Value == 1)
		{
			pAd->CommonCfg.HT_DisallowTKIP = TRUE;
		}
		else
		{
			pAd->CommonCfg.HT_DisallowTKIP = FALSE;
		}		

		DBGPRINT(RT_DEBUG_TRACE, ("HT: Disallow TKIP mode = %s\n", (pAd->CommonCfg.HT_DisallowTKIP == TRUE) ? "ON" : "OFF" ));
	}

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
			if (RTMPGetKeyParameter("OBSSScanParam", pValueStr, 32, pInput, TRUE))
			{	int ObssScanValue, idx;
				PSTRING	macptr;	
				for (idx = 0, macptr = rstrtok(pValueStr,";"); macptr; macptr = rstrtok(NULL,";"), idx++)
				{
					ObssScanValue = simple_strtol(macptr, 0, 10);
					switch (idx)
					{
						case 0:
							if (ObssScanValue < 5 || ObssScanValue > 1000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanPassiveDwell(%d), should in range 5~1000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanPassiveDwell = ObssScanValue;	// Unit : TU. 5~1000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanPassiveDwell=%d\n", ObssScanValue));
							}
							break;
						case 1:
							if (ObssScanValue < 10 || ObssScanValue > 1000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanActiveDwell(%d), should in range 10~1000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanActiveDwell = ObssScanValue;	// Unit : TU. 10~1000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanActiveDwell=%d\n", ObssScanValue));
							}
							break;
						case 2:
							pAd->CommonCfg.Dot11BssWidthTriggerScanInt = ObssScanValue;	// Unit : Second
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthTriggerScanInt=%d\n", ObssScanValue));
							break;
						case 3:
							if (ObssScanValue < 200 || ObssScanValue > 10000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanPassiveTotalPerChannel(%d), should in range 200~10000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = ObssScanValue;	// Unit : TU. 200~10000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanPassiveTotalPerChannel=%d\n", ObssScanValue));
							}
							break;
						case 4:
							if (ObssScanValue < 20 || ObssScanValue > 10000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanActiveTotalPerChannel(%d), should in range 20~10000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = ObssScanValue;	// Unit : TU. 20~10000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanActiveTotalPerChannel=%d\n", ObssScanValue));
							}
							break;
						case 5:
							pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = ObssScanValue;
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelayFactor=%d\n", ObssScanValue));
							break;
						case 6:
							pAd->CommonCfg.Dot11OBssScanActivityThre = ObssScanValue;	// Unit : percentage
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelayFactor=%d\n", ObssScanValue));
							break;
					}			
				}

				if (idx != 7)
				{
					DBGPRINT(RT_DEBUG_ERROR, ("Wrong OBSSScanParamtetrs format in dat file!!!!! Use default value.\n"));

					pAd->CommonCfg.Dot11OBssScanPassiveDwell = dot11OBSSScanPassiveDwell;	// Unit : TU. 5~1000
					pAd->CommonCfg.Dot11OBssScanActiveDwell = dot11OBSSScanActiveDwell;	// Unit : TU. 10~1000
					pAd->CommonCfg.Dot11BssWidthTriggerScanInt = dot11BSSWidthTriggerScanInterval;	// Unit : Second	
					pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = dot11OBSSScanPassiveTotalPerChannel;	// Unit : TU. 200~10000
					pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = dot11OBSSScanActiveTotalPerChannel;	// Unit : TU. 20~10000
					pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = dot11BSSWidthChannelTransactionDelayFactor;
					pAd->CommonCfg.Dot11OBssScanActivityThre = dot11BSSScanActivityThreshold;	// Unit : percentage
				}
				pAd->CommonCfg.Dot11BssWidthChanTranDelay = (pAd->CommonCfg.Dot11BssWidthTriggerScanInt * pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor);
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelay=%ld\n", pAd->CommonCfg.Dot11BssWidthChanTranDelay));
			}

			//Modified by fredli 11-12-1. 3390(2.3.2.17) driver fail to connect 11n client with COEX enable, and hence should avoid bandwith adjust. The value will be forced set with FALSE when initialize in rtmp_init.c as well.
			#if 0
			if (RTMPGetKeyParameter("HT_BSSCoexistence", pValueStr, 25, pInput, TRUE))
			{
				Value = simple_strtol(pValueStr, 0, 10);
				pAd->CommonCfg.bBssCoexEnable = ((Value == 1) ? TRUE : FALSE);

				DBGPRINT(RT_DEBUG_TRACE, ("HT: 20/40 BssCoexSupport = %s\n", (pAd->CommonCfg.bBssCoexEnable == TRUE) ? "ON" : "OFF" ));
			}
			#endif
			
			pAd->CommonCfg.bBssCoexEnable = FALSE;
			DBGPRINT(RT_DEBUG_TRACE, ("HT: 20/40 BssCoexSupport = %s\n", (pAd->CommonCfg.bBssCoexEnable == TRUE) ? "ON" : "OFF"
));
			//end Modification of fredli 11-12-1

#endif // DOT11N_DRAFT3 //

#endif // DOT11_N_SUPPORT //

	//2008/11/05:KH add to support Antenna power-saving of AP-->
}
#endif // DOT11_N_SUPPORT //


NDIS_STATUS	RTMPSetProfileParameters(
	IN RTMP_ADAPTER *pAd,
	IN PSTRING	pBuffer)
{
	PSTRING					tmpbuf;
	ULONG					RtsThresh;
	ULONG					FragThresh;
	PSTRING					macptr;							
	INT						i = 0, retval;
	tmpbuf = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(tmpbuf == NULL)
		return NDIS_STATUS_FAILURE;
	
	do
	{
		// set file parameter to portcfg
		//CountryRegion
		if (RTMPGetKeyParameter("MacAddress", tmpbuf, 25, pBuffer, TRUE))
		{					
			retval = RT_CfgSetMacAddress(pAd, tmpbuf);
			if (retval)
				DBGPRINT(RT_DEBUG_TRACE, ("MacAddress = %02x:%02x:%02x:%02x:%02x:%02x\n", 
											PRINT_MAC(pAd->CurrentAddress)));
		}
		if(RTMPGetKeyParameter("CountryRegion", tmpbuf, 25, pBuffer, TRUE))
		{
			retval = RT_CfgSetCountryRegion(pAd, tmpbuf, BAND_24G);
			DBGPRINT(RT_DEBUG_TRACE, ("CountryRegion=%d\n", pAd->CommonCfg.CountryRegion));
		}
		//CountryRegionABand
		if(RTMPGetKeyParameter("CountryRegionABand", tmpbuf, 25, pBuffer, TRUE))
		{
			retval = RT_CfgSetCountryRegion(pAd, tmpbuf, BAND_5G);
			DBGPRINT(RT_DEBUG_TRACE, ("CountryRegionABand=%d\n", pAd->CommonCfg.CountryRegionForABand));
		}
#ifdef RTMP_EFUSE_SUPPORT
#ifdef RT30xx
#ifdef RALINK_ATE
		//EfuseBufferMode
		if(RTMPGetKeyParameter("EfuseBufferMode", tmpbuf, 25, pBuffer, TRUE))
		{
			pAd->bEEPROMFile = (UCHAR) simple_strtol(tmpbuf, 0, 10);
			DBGPRINT(RT_DEBUG_TRACE, ("EfuseBufferMode=%d\n", pAd->bUseEfuse));
		}
#endif // RALINK_ATE //
#endif // RT30xx //
#endif // RTMP_EFUSE_SUPPORT //
		//CountryCode
		if(RTMPGetKeyParameter("CountryCode", tmpbuf, 25, pBuffer, TRUE))
		{
			NdisMoveMemory(pAd->CommonCfg.CountryCode, tmpbuf , 2);
			if (strlen((PSTRING) pAd->CommonCfg.CountryCode) != 0)
			{
				pAd->CommonCfg.bCountryFlag = TRUE;
			}
			DBGPRINT(RT_DEBUG_TRACE, ("CountryCode=%s\n", pAd->CommonCfg.CountryCode));
		}
		//ChannelGeography
		if(RTMPGetKeyParameter("ChannelGeography", tmpbuf, 25, pBuffer, TRUE))
		{
			UCHAR Geography = (UCHAR) simple_strtol(tmpbuf, 0, 10);
			if (Geography <= BOTH)
			{
				pAd->CommonCfg.Geography = Geography;
				pAd->CommonCfg.CountryCode[2] =
					(pAd->CommonCfg.Geography == BOTH) ? ' ' : ((pAd->CommonCfg.Geography == IDOR) ? 'I' : 'O');
				DBGPRINT(RT_DEBUG_TRACE, ("ChannelGeography=%d\n", pAd->CommonCfg.Geography));
			}
		}
		else
		{
			pAd->CommonCfg.Geography = BOTH;
			pAd->CommonCfg.CountryCode[2] = ' ';
		}
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
#ifdef MBSS_SUPPORT
			//BSSIDNum; This must read first of other multiSSID field, so list this field first in configuration file
			if(RTMPGetKeyParameter("BssidNum", tmpbuf, 25, pBuffer, TRUE))
			{
				pAd->ApCfg.BssidNum = (UCHAR) simple_strtol(tmpbuf, 0, 10);
				if(pAd->ApCfg.BssidNum > MAX_MBSSID_NUM)
				{
					pAd->ApCfg.BssidNum = MAX_MBSSID_NUM;
					DBGPRINT(RT_DEBUG_TRACE, ("BssidNum=%d(MAX_MBSSID_NUM is %d)\n", pAd->ApCfg.BssidNum,MAX_MBSSID_NUM));
				}
				else
				DBGPRINT(RT_DEBUG_TRACE, ("BssidNum=%d\n", pAd->ApCfg.BssidNum));
			}

			if (HW_BEACON_OFFSET > (HW_BEACON_MAX_SIZE / pAd->ApCfg.BssidNum))
			{
				DBGPRINT(RT_DEBUG_OFF, ("mbss> fatal error! beacon offset is error in driver! "
						"Please re-assign HW_BEACON_OFFSET!\n"));
			} /* End of if */
#else
			pAd->ApCfg.BssidNum = 1;
#endif // MBSS_SUPPORT //
		}
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			// SSID
			if (TRUE)
			{
				STRING tok_str[16];
				UCHAR BssidCountSupposed = 0;
				BOOLEAN bSSIDxIsUsed = FALSE;

				DBGPRINT(RT_DEBUG_TRACE, ("pAd->ApCfg.BssidNum=%d\n", pAd->ApCfg.BssidNum));

				for (i = 0; i < pAd->ApCfg.BssidNum; i++)
				{
					sprintf(tok_str, "SSID%d", i + 1);
					if(RTMPGetKeyParameter(tok_str, tmpbuf, 33, pBuffer, FALSE))
						{
							NdisMoveMemory(pAd->ApCfg.MBSSID[i].Ssid, tmpbuf , strlen(tmpbuf));

						pAd->ApCfg.MBSSID[i].Ssid[strlen(tmpbuf)] = End_Mark;

								    	pAd->ApCfg.MBSSID[i].SsidLen = strlen((PSTRING) pAd->ApCfg.MBSSID[i].Ssid);
							if (bSSIDxIsUsed == FALSE)
							{
								bSSIDxIsUsed = TRUE;
							}
					    	DBGPRINT(RT_DEBUG_TRACE, ("SSID[%d]=%s\n", i, pAd->ApCfg.MBSSID[i].Ssid));
						}
					}
				if (bSSIDxIsUsed == FALSE)
				{
					if(RTMPGetKeyParameter("SSID", tmpbuf, 256, pBuffer, FALSE))
					{			
						BssidCountSupposed = delimitcnt(tmpbuf, ";") + 1;
						if (pAd->ApCfg.BssidNum != BssidCountSupposed)
						{
							DBGPRINT_ERR(("Your no. of SSIDs( = %d) does not match your BssidNum( = %d)!\n", BssidCountSupposed, pAd->ApCfg.BssidNum));
						}
						if (pAd->ApCfg.BssidNum > 1)
						{
							// Anyway, we still do the legacy dissection of the whole SSID string.
							for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
							{
								int apidx = 0;

								if (i < pAd->ApCfg.BssidNum)
								{
									apidx = i;
								} 
								else
								{
									break;
								}

								NdisMoveMemory(pAd->ApCfg.MBSSID[apidx].Ssid, macptr , strlen(macptr));

							pAd->ApCfg.MBSSID[apidx].Ssid[strlen(macptr)] = End_Mark;

							   pAd->ApCfg.MBSSID[apidx].SsidLen = strlen((PSTRING) pAd->ApCfg.MBSSID[apidx].Ssid);

				    			DBGPRINT(RT_DEBUG_TRACE, ("SSID[%d]=%s\n", i, pAd->ApCfg.MBSSID[apidx].Ssid));
							}
						}
						else
						{
							if ((strlen(tmpbuf) > 0) && (strlen(tmpbuf) <= 32))
							{
								NdisMoveMemory(pAd->ApCfg.MBSSID[BSS0].Ssid, tmpbuf , strlen(tmpbuf));

							pAd->ApCfg.MBSSID[BSS0].Ssid[strlen(tmpbuf)] = End_Mark;
									    	pAd->ApCfg.MBSSID[BSS0].SsidLen = strlen((PSTRING) pAd->ApCfg.MBSSID[BSS0].Ssid);
								DBGPRINT(RT_DEBUG_TRACE, ("SSID=%s\n", pAd->ApCfg.MBSSID[BSS0].Ssid));
							}
						}
					}
				}
			}
		}
#endif // CONFIG_AP_SUPPORT //


		//Channel
		if(RTMPGetKeyParameter("Channel", tmpbuf, 10, pBuffer, TRUE))
		{
			pAd->CommonCfg.Channel = (UCHAR) simple_strtol(tmpbuf, 0, 10);
			DBGPRINT(RT_DEBUG_TRACE, ("Channel=%d\n", pAd->CommonCfg.Channel));
		}
		//WirelessMode
		if(RTMPGetKeyParameter("WirelessMode", tmpbuf, 10, pBuffer, TRUE))
		{
			RT_CfgSetWirelessMode(pAd, tmpbuf);
			DBGPRINT(RT_DEBUG_TRACE, ("PhyMode=%d\n", pAd->CommonCfg.PhyMode));
		}
	    //BasicRate
		if(RTMPGetKeyParameter("BasicRate", tmpbuf, 10, pBuffer, TRUE))
		{
			pAd->CommonCfg.BasicRateBitmap = (ULONG) simple_strtol(tmpbuf, 0, 10);
			DBGPRINT(RT_DEBUG_TRACE, ("BasicRate=%ld\n", pAd->CommonCfg.BasicRateBitmap));
		}
		//BeaconPeriod
		if(RTMPGetKeyParameter("BeaconPeriod", tmpbuf, 10, pBuffer, TRUE))
		{
			pAd->CommonCfg.BeaconPeriod = (USHORT) simple_strtol(tmpbuf, 0, 10);
			DBGPRINT(RT_DEBUG_TRACE, ("BeaconPeriod=%d\n", pAd->CommonCfg.BeaconPeriod));
		}
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			//DtimPeriod
			if(RTMPGetKeyParameter("DtimPeriod", tmpbuf, 10, pBuffer, TRUE))
			{
				pAd->ApCfg.DtimPeriod = (UCHAR) simple_strtol(tmpbuf, 0, 10);
				DBGPRINT(RT_DEBUG_TRACE, ("DtimPeriod=%d\n", pAd->ApCfg.DtimPeriod));
			}
		}
#endif // CONFIG_AP_SUPPORT //					
	    //TxPower
		if(RTMPGetKeyParameter("TxPower", tmpbuf, 10, pBuffer, TRUE))
		{
			pAd->CommonCfg.TxPowerPercentage = (ULONG) simple_strtol(tmpbuf, 0, 10);
			DBGPRINT(RT_DEBUG_TRACE, ("TxPower=%ld\n", pAd->CommonCfg.TxPowerPercentage));
		}
		//BGProtection
		if(RTMPGetKeyParameter("BGProtection", tmpbuf, 10, pBuffer, TRUE))
		{
	//#if 0	//#ifndef WIFI_TEST
	//		pAd->CommonCfg.UseBGProtection = 2;// disable b/g protection for throughput test
	//#else
			switch (simple_strtol(tmpbuf, 0, 10))
			{
				case 1: //Always On
					pAd->CommonCfg.UseBGProtection = 1;
					break;
				case 2: //Always OFF
					pAd->CommonCfg.UseBGProtection = 2;
					break;
				case 0: //AUTO
				default:
					pAd->CommonCfg.UseBGProtection = 0;
					break;
			}
	//#endif
			DBGPRINT(RT_DEBUG_TRACE, ("BGProtection=%ld\n", pAd->CommonCfg.UseBGProtection));
		}
		//OLBCDetection
		if(RTMPGetKeyParameter("DisableOLBC", tmpbuf, 10, pBuffer, TRUE))
		{
			switch (simple_strtol(tmpbuf, 0, 10))
			{
				case 1: //disable OLBC Detection
					pAd->CommonCfg.DisableOLBCDetect = 1;
					break;
				case 0: //enable OLBC Detection
					pAd->CommonCfg.DisableOLBCDetect = 0;
					break;
				default:
					pAd->CommonCfg.DisableOLBCDetect= 0;
					break;
			}
			DBGPRINT(RT_DEBUG_TRACE, ("OLBCDetection=%ld\n", pAd->CommonCfg.DisableOLBCDetect));
		}
		//TxPreamble
		if(RTMPGetKeyParameter("TxPreamble", tmpbuf, 10, pBuffer, TRUE))
		{
			switch (simple_strtol(tmpbuf, 0, 10))
			{
				case Rt802_11PreambleShort:
					pAd->CommonCfg.TxPreamble = Rt802_11PreambleShort;
					break;
				case Rt802_11PreambleLong:
				default:
					pAd->CommonCfg.TxPreamble = Rt802_11PreambleLong;
					break;
			}
			DBGPRINT(RT_DEBUG_TRACE, ("TxPreamble=%ld\n", pAd->CommonCfg.TxPreamble));
		}
		//RTSThreshold
		if(RTMPGetKeyParameter("RTSThreshold", tmpbuf, 10, pBuffer, TRUE))
		{
			RtsThresh = simple_strtol(tmpbuf, 0, 10);
			if( (RtsThresh >= 1) && (RtsThresh <= MAX_RTS_THRESHOLD) )
				pAd->CommonCfg.RtsThreshold  = (USHORT)RtsThresh;
			else
				pAd->CommonCfg.RtsThreshold = MAX_RTS_THRESHOLD;
			
			DBGPRINT(RT_DEBUG_TRACE, ("RTSThreshold=%d\n", pAd->CommonCfg.RtsThreshold));
		}
		//FragThreshold
		if(RTMPGetKeyParameter("FragThreshold", tmpbuf, 10, pBuffer, TRUE))
		{		
			FragThresh = simple_strtol(tmpbuf, 0, 10);
			pAd->CommonCfg.bUseZeroToDisableFragment = FALSE;

			if (FragThresh > MAX_FRAG_THRESHOLD || FragThresh < MIN_FRAG_THRESHOLD)
			{ //illegal FragThresh so we set it to default
				pAd->CommonCfg.FragmentThreshold = MAX_FRAG_THRESHOLD;
				pAd->CommonCfg.bUseZeroToDisableFragment = TRUE;
			}
			else if (FragThresh % 2 == 1)
			{
				// The length of each fragment shall always be an even number of octets, except for the last fragment
				// of an MSDU or MMPDU, which may be either an even or an odd number of octets.
				pAd->CommonCfg.FragmentThreshold = (USHORT)(FragThresh - 1);
			}
			else
			{
				pAd->CommonCfg.FragmentThreshold = (USHORT)FragThresh;
			}
			//pAd->CommonCfg.AllowFragSize = (pAd->CommonCfg.FragmentThreshold) - LENGTH_802_11 - LENGTH_CRC;
			DBGPRINT(RT_DEBUG_TRACE, ("FragThreshold=%d\n", pAd->CommonCfg.FragmentThreshold));
		}
		//TxBurst
		if(RTMPGetKeyParameter("TxBurst", tmpbuf, 10, pBuffer, TRUE))
		{
	//#ifdef WIFI_TEST
	//						pAd->CommonCfg.bEnableTxBurst = FALSE;
	//#else
			if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
				pAd->CommonCfg.bEnableTxBurst = TRUE;
			else //Disable
				pAd->CommonCfg.bEnableTxBurst = FALSE;
	//#endif
			DBGPRINT(RT_DEBUG_TRACE, ("TxBurst=%d\n", pAd->CommonCfg.bEnableTxBurst));
		}

#ifdef AGGREGATION_SUPPORT
		//PktAggregate
		if(RTMPGetKeyParameter("PktAggregate", tmpbuf, 10, pBuffer, TRUE))
		{
			if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
				pAd->CommonCfg.bAggregationCapable = TRUE;
			else //Disable
				pAd->CommonCfg.bAggregationCapable = FALSE;
#ifdef PIGGYBACK_SUPPORT
			pAd->CommonCfg.bPiggyBackCapable = pAd->CommonCfg.bAggregationCapable;
#endif // PIGGYBACK_SUPPORT //
			DBGPRINT(RT_DEBUG_TRACE, ("PktAggregate=%d\n", pAd->CommonCfg.bAggregationCapable));
		}
#else
		pAd->CommonCfg.bAggregationCapable = FALSE;
		pAd->CommonCfg.bPiggyBackCapable = FALSE;
#endif // AGGREGATION_SUPPORT //

		// WmmCapable
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			rtmp_read_ap_wmm_parms_from_file(pAd, tmpbuf, pBuffer);
#endif // CONFIG_AP_SUPPORT //


#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			// MaxStaNum
			if (RTMPGetKeyParameter("MaxStaNum", tmpbuf, 32, pBuffer, TRUE))
			{
			    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					if (i >= pAd->ApCfg.BssidNum)
						break;
					
					ApCfg_Set_MaxStaNum_Proc(pAd, i, macptr);					
			    }
			}
		
			// IdleTimeout
			if(RTMPGetKeyParameter("IdleTimeout", tmpbuf, 10, pBuffer, TRUE))
			{
				ApCfg_Set_IdleTimeout_Proc(pAd, tmpbuf);
			}
		
			//NoForwarding
			if(RTMPGetKeyParameter("NoForwarding", tmpbuf, 32, pBuffer, TRUE))
			{
			    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					if (i >= pAd->ApCfg.BssidNum)
						break;

					if(simple_strtol(macptr, 0, 10) != 0)  //Enable
						pAd->ApCfg.MBSSID[i].IsolateInterStaTraffic = TRUE;
					else //Disable
						pAd->ApCfg.MBSSID[i].IsolateInterStaTraffic = FALSE;

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) NoForwarding=%ld\n", i, pAd->ApCfg.MBSSID[i].IsolateInterStaTraffic));
			    }
			}
			//NoForwardingBTNBSSID
			if(RTMPGetKeyParameter("NoForwardingBTNBSSID", tmpbuf, 10, pBuffer, TRUE))
			{
				if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
					pAd->ApCfg.IsolateInterStaTrafficBTNBSSID = TRUE;
				else //Disable
					pAd->ApCfg.IsolateInterStaTrafficBTNBSSID = FALSE;

				DBGPRINT(RT_DEBUG_TRACE, ("NoForwardingBTNBSSID=%ld\n", pAd->ApCfg.IsolateInterStaTrafficBTNBSSID));
			}
			//HideSSID
			if(RTMPGetKeyParameter("HideSSID", tmpbuf, 32, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					int apidx = i;

					if (i >= pAd->ApCfg.BssidNum)
						break;

					if(simple_strtol(macptr, 0, 10) != 0)  //Enable
						pAd->ApCfg.MBSSID[apidx].bHideSsid = TRUE;								
					else //Disable
						pAd->ApCfg.MBSSID[apidx].bHideSsid = FALSE;								

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) HideSSID=%d\n", i, pAd->ApCfg.MBSSID[apidx].bHideSsid));
				}
			}

			//StationKeepAlive
			if(RTMPGetKeyParameter("StationKeepAlive", tmpbuf, 32, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					int apidx = i;

					if (i >= pAd->ApCfg.BssidNum)
						break;

					pAd->ApCfg.MBSSID[apidx].StationKeepAliveTime = simple_strtol(macptr, 0, 10);
					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) StationKeepAliveTime=%d\n", i, pAd->ApCfg.MBSSID[apidx].StationKeepAliveTime));
				}
			}
		}
#endif // CONFIG_AP_SUPPORT //
		//ShortSlot
		if(RTMPGetKeyParameter("ShortSlot", tmpbuf, 10, pBuffer, TRUE))
		{
			RT_CfgSetShortSlot(pAd, tmpbuf);
			DBGPRINT(RT_DEBUG_TRACE, ("ShortSlot=%d\n", pAd->CommonCfg.bUseShortSlotTime));
		}
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		//if (pAd->OpMode == OPMODE_AP)
		{
			//AutoChannelSelect
			if(RTMPGetKeyParameter("AutoChannelSelect", tmpbuf, 10, pBuffer, TRUE))
			{
				if(simple_strtol(tmpbuf, 0, 10) != 0)  //Enable
					pAd->ApCfg.bAutoChannelAtBootup = TRUE;
				else
					pAd->ApCfg.bAutoChannelAtBootup = FALSE;

				DBGPRINT(RT_DEBUG_TRACE, ("AutoChannelAtBootup=%d\n", pAd->ApCfg.bAutoChannelAtBootup));
			}
			//IEEE8021X
			if(RTMPGetKeyParameter("IEEE8021X", tmpbuf, 10, pBuffer, TRUE))
			{
			    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					if (i >= pAd->ApCfg.BssidNum)
						break;

					if(simple_strtol(macptr, 0, 10) != 0)  //Enable
						pAd->ApCfg.MBSSID[i].IEEE8021X = TRUE;
					else //Disable
						pAd->ApCfg.MBSSID[i].IEEE8021X = FALSE;

					DBGPRINT(RT_DEBUG_TRACE, ("IEEE8021X=%d\n", pAd->ApCfg.MBSSID[i].IEEE8021X));
			    }
			}
		}
#endif // CONFIG_AP_SUPPORT //
		//IEEE80211H
		if(RTMPGetKeyParameter("IEEE80211H", tmpbuf, 10, pBuffer, TRUE))
		{
		    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
		    {
				if(simple_strtol(macptr, 0, 10) != 0)  //Enable
					pAd->CommonCfg.bIEEE80211H = TRUE;
				else //Disable
					pAd->CommonCfg.bIEEE80211H = FALSE;

				DBGPRINT(RT_DEBUG_TRACE, ("IEEE80211H=%d\n", pAd->CommonCfg.bIEEE80211H));
		    }
		}
		//CSPeriod
		if(RTMPGetKeyParameter("CSPeriod", tmpbuf, 10, pBuffer, TRUE))
		{
		    if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.RadarDetect.CSPeriod = simple_strtol(tmpbuf, 0, 10);
			else
				pAd->CommonCfg.RadarDetect.CSPeriod = 0;

				DBGPRINT(RT_DEBUG_TRACE, ("CSPeriod=%d\n", pAd->CommonCfg.RadarDetect.CSPeriod));
		}

#ifdef MERGE_ARCH_TEAM
		// DfsLowerLimit
		if(RTMPGetKeyParameter("DfsLowerLimit", tmpbuf, 10, pBuffer, TRUE))
		{
			if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.RadarDetect.DfsLowerLimit = simple_strtol(tmpbuf, 0, 10);

			DBGPRINT(RT_DEBUG_TRACE, ("DfsLowerLimit=%ld\n", pAd->CommonCfg.RadarDetect.DfsLowerLimit));
		}
		
		// DfsUpperLimit
		if(RTMPGetKeyParameter("DfsUpperLimit", tmpbuf, 10, pBuffer, TRUE))
		{
			if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.RadarDetect.DfsUpperLimit = simple_strtol(tmpbuf, 0, 10);

			DBGPRINT(RT_DEBUG_TRACE, ("DfsUpperLimit=%ld\n", pAd->CommonCfg.RadarDetect.DfsUpperLimit));
		}

		// FixDfsLimit
		if(RTMPGetKeyParameter("FixDfsLimit", tmpbuf, 10, pBuffer, TRUE))
		{
		    if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.RadarDetect.FixDfsLimit = TRUE;
			else
				pAd->CommonCfg.RadarDetect.FixDfsLimit = FALSE;

			DBGPRINT(RT_DEBUG_TRACE, ("FixDfsLimit=%d\n", pAd->CommonCfg.RadarDetect.FixDfsLimit));
		}

		// LongPulseRadarTh
		if(RTMPGetKeyParameter("LongPulseRadarTh", tmpbuf, 10, pBuffer, TRUE))
		{
		    if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.RadarDetect.LongPulseRadarTh = simple_strtol(tmpbuf, 0, 10);

			DBGPRINT(RT_DEBUG_TRACE, ("LongPulseRadarTh=%d\n", pAd->CommonCfg.RadarDetect.LongPulseRadarTh));
		}

		// AvgRssiReq
		if(RTMPGetKeyParameter("AvgRssiReq", tmpbuf, 10, pBuffer, TRUE))
		{
			if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.RadarDetect.AvgRssiReq = simple_strtol(tmpbuf, 0, 10);

			DBGPRINT(RT_DEBUG_TRACE, ("AvgRssiReq=%d\n", pAd->CommonCfg.RadarDetect.AvgRssiReq));
		}

#endif // MERGE_ARCH_TEAM //

		//RDRegion
		if(RTMPGetKeyParameter("RDRegion", tmpbuf, 128, pBuffer, TRUE))
		{
						RADAR_DETECT_STRUCT	*pRadarDetect = &pAd->CommonCfg.RadarDetect;
			if ((strncmp(tmpbuf, "JAP_W53", 7) == 0) || (strncmp(tmpbuf, "jap_w53", 7) == 0))
			{
							pRadarDetect->RDDurRegion = JAP_W53;
							pRadarDetect->DfsSessionTime = 15;
			}
			else if ((strncmp(tmpbuf, "JAP_W56", 7) == 0) || (strncmp(tmpbuf, "jap_w56", 7) == 0))
			{
							pRadarDetect->RDDurRegion = JAP_W56;
							pRadarDetect->DfsSessionTime = 13;
			}
			else if ((strncmp(tmpbuf, "JAP", 3) == 0) || (strncmp(tmpbuf, "jap", 3) == 0))
			{
							pRadarDetect->RDDurRegion = JAP;
							pRadarDetect->DfsSessionTime = 5;
			}
			else  if ((strncmp(tmpbuf, "FCC", 3) == 0) || (strncmp(tmpbuf, "fcc", 3) == 0))
			{
							pRadarDetect->RDDurRegion = FCC;
							pRadarDetect->DfsSessionTime = 5;
#ifdef DFS_FCC_BW40_FIX
							pRadarDetect->DfsSessionFccOff = 0;
#endif // DFS_FCC_BW40_FIX //
			}
			else if ((strncmp(tmpbuf, "CE", 2) == 0) || (strncmp(tmpbuf, "ce", 2) == 0))
			{
							pRadarDetect->RDDurRegion = CE;
							pRadarDetect->DfsSessionTime = 13;
			}
			else
			{
							pRadarDetect->RDDurRegion = CE;
							pRadarDetect->DfsSessionTime = 13;
			}

						DBGPRINT(RT_DEBUG_TRACE, ("RDRegion=%d\n", pRadarDetect->RDDurRegion));
		}
		else
		{
			pAd->CommonCfg.RadarDetect.RDDurRegion = CE;
			pAd->CommonCfg.RadarDetect.DfsSessionTime = 13;
		}

		//WirelessEvent
		if(RTMPGetKeyParameter("WirelessEvent", tmpbuf, 10, pBuffer, TRUE))
		{				
#if WIRELESS_EXT >= 15
		    if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.bWirelessEvent = simple_strtol(tmpbuf, 0, 10);
			else
				pAd->CommonCfg.bWirelessEvent = 0;	// disable
#else
			pAd->CommonCfg.bWirelessEvent = 0;	// disable
#endif
				DBGPRINT(RT_DEBUG_TRACE, ("WirelessEvent=%d\n", pAd->CommonCfg.bWirelessEvent));
		}
		if(RTMPGetKeyParameter("WiFiTest", tmpbuf, 10, pBuffer, TRUE))
		{				
		    if(simple_strtol(tmpbuf, 0, 10) != 0)
				pAd->CommonCfg.bWiFiTest= simple_strtol(tmpbuf, 0, 10);
			else
				pAd->CommonCfg.bWiFiTest = 0;	// disable

				DBGPRINT(RT_DEBUG_TRACE, ("WiFiTest=%d\n", pAd->CommonCfg.bWiFiTest));
		}
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			//PreAuth
			if(RTMPGetKeyParameter("PreAuth", tmpbuf, 10, pBuffer, TRUE))
			{
			    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					if (i >= pAd->ApCfg.BssidNum)
						break;

					if(simple_strtol(macptr, 0, 10) != 0)  //Enable
						pAd->ApCfg.MBSSID[i].PreAuth = TRUE;
					else //Disable
						pAd->ApCfg.MBSSID[i].PreAuth = FALSE;

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) PreAuth=%d\n", i, pAd->ApCfg.MBSSID[i].PreAuth));
			    }
			}
		}
#endif // CONFIG_AP_SUPPORT //					
		//AuthMode
		if(RTMPGetKeyParameter("AuthMode", tmpbuf, 128, pBuffer, TRUE))
		{
#ifdef CONFIG_AP_SUPPORT
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			{
		   		 for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
		    		{
					int apidx;

					if (i < pAd->ApCfg.BssidNum)
					{							
						apidx = i;
					}
					else
					{
						break;
					}

					if ((strncmp(macptr, "WEPAUTO", 4) == 0) || (strncmp(macptr, "wepauto", 4) == 0))
		    				pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeAutoSwitch;
						else if ((strncmp(macptr, "OPEN", 4) == 0) || (strncmp(macptr, "open", 4) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeOpen;
						else if ((strncmp(macptr, "SHARED", 6) == 0) || (strncmp(macptr, "shared", 6) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeShared;
						else if ((strncmp(macptr, "WPA2PSK", 7) == 0) || (strncmp(macptr, "wpa2psk", 7) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWPA2PSK;
						else if ((strncmp(macptr, "WPA2", 4) == 0) || (strncmp(macptr, "wpa2", 4) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWPA2;
						else if ((strncmp(macptr, "WPA1WPA2", 8) == 0) || (strncmp(macptr, "wpa1wpa2", 8) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWPA1WPA2;
						else if ((strncmp(macptr, "WPAPSKWPA2PSK", 13) == 0) || (strncmp(macptr, "wpapskwpa2psk", 13) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWPA1PSKWPA2PSK;
						else if ((strncmp(macptr, "WPAPSK", 6) == 0) || (strncmp(macptr, "wpapsk", 6) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWPAPSK;
						else if ((strncmp(macptr, "WPA", 3) == 0) || (strncmp(macptr, "wpa", 3) == 0))
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWPA;
#ifdef WAPI_SUPPORT
									else if ((strncmp(macptr, "WAICERT", 7) == 0) || (strncmp(macptr, "waicert", 7) == 0))
    									pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWAICERT;
									else if ((strncmp(macptr, "WAIPSK", 6) == 0) || (strncmp(macptr, "waipsk", 6) == 0))
    									pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeWAIPSK;
#endif // WAPI_SUPPORT //
						else //Default
							pAd->ApCfg.MBSSID[apidx].AuthMode = Ndis802_11AuthModeOpen;

						// move to ap.c::APStartUp to process
						//RTMPMakeRSNIE(pAd, pAd->ApCfg.MBSSID[apidx].AuthMode, pAd->ApCfg.MBSSID[apidx].WepStatus, apidx);
						DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) AuthMode=%d\n", i, pAd->ApCfg.MBSSID[apidx].AuthMode));
				}
			}
#endif // CONFIG_AP_SUPPORT //
		}
		//EncrypType
		if(RTMPGetKeyParameter("EncrypType", tmpbuf, 128, pBuffer, TRUE))
		{
#ifdef CONFIG_AP_SUPPORT
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			{
				//We need to reset the WepStatus of all interfaces as 1 (Ndis802_11WEPDisabled) first. 
				//		Or it may have problem when some interface enabled but didn't configure it.
				for ( i= 0; i<pAd->ApCfg.BssidNum; i++)
					pAd->ApCfg.MBSSID[i].WepStatus = Ndis802_11WEPDisabled;
		    		for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
		    		{
					int apidx;

					if (i<pAd->ApCfg.BssidNum)
					{							
						apidx = i;
					}
		        		else
					{
						break;
					}

					if ((strncmp(macptr, "NONE", 4) == 0) || (strncmp(macptr, "none", 4) == 0))
		            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11WEPDisabled;
		        		else if ((strncmp(macptr, "WEP", 3) == 0) || (strncmp(macptr, "wep", 3) == 0))
		            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11WEPEnabled;
		        		else if ((strncmp(macptr, "TKIPAES", 7) == 0) || (strncmp(macptr, "tkipaes", 7) == 0))
		            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11Encryption4Enabled;
		        		else if ((strncmp(macptr, "TKIP", 4) == 0) || (strncmp(macptr, "tkip", 4) == 0))
		            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11Encryption2Enabled;
		        		else if ((strncmp(macptr, "AES", 3) == 0) || (strncmp(macptr, "aes", 3) == 0))
		            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11Encryption3Enabled;
#ifdef WAPI_SUPPORT
									else if ((strncmp(macptr, "SMS4", 4) == 0) || (strncmp(macptr, "sms4", 4) == 0))
					            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11EncryptionSMS4Enabled;									
#endif // WAPI_SUPPORT //
		        		else
		            			pAd->ApCfg.MBSSID[apidx].WepStatus = Ndis802_11WEPDisabled;

						// decide the group key encryption type
						if (pAd->ApCfg.MBSSID[apidx].WepStatus == Ndis802_11Encryption4Enabled)	
							pAd->ApCfg.MBSSID[apidx].GroupKeyWepStatus = Ndis802_11Encryption2Enabled;		
						else
							pAd->ApCfg.MBSSID[apidx].GroupKeyWepStatus = pAd->ApCfg.MBSSID[apidx].WepStatus;

						// move to ap.c::APStartUp to process
	        			//RTMPMakeRSNIE(pAd, pAd->ApCfg.MBSSID[apidx].AuthMode, pAd->ApCfg.MBSSID[apidx].WepStatus, apidx);
		        		DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) EncrypType=%d\n", i, pAd->ApCfg.MBSSID[apidx].WepStatus));
		    		}
			}
#endif // CONFIG_AP_SUPPORT //

		}

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			// WpaMixPairCipher
			if(RTMPGetKeyParameter("WpaMixPairCipher", tmpbuf, 256, pBuffer, TRUE))
			{
				/*
					In WPA-WPA2 mix mode, it provides a more flexible cipher combination. 
					-	WPA-AES and WPA2-TKIP
					-	WPA-AES and WPA2-TKIPAES
					-	WPA-TKIP and WPA2-AES
					-	WPA-TKIP and WPA2-TKIPAES
					-	WPA-TKIPAES and WPA2-AES
					-	WPA-TKIPAES and WPA2-TKIP
					-	WPA-TKIPAES and WPA2-TKIPAES (default)																 																	
				 */							
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{
					if (pAd->ApCfg.MBSSID[i].AuthMode != Ndis802_11AuthModeWPA1WPA2 && 
						pAd->ApCfg.MBSSID[i].AuthMode != Ndis802_11AuthModeWPA1PSKWPA2PSK)
						continue;
													
					if (pAd->ApCfg.MBSSID[i].WepStatus != Ndis802_11Encryption4Enabled)
						continue;

					if ((strncmp(macptr, "WPA_AES_WPA2_TKIPAES", 20) == 0) || (strncmp(macptr, "wpa_aes_wpa2_tkipaes", 20) == 0))
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_AES_WPA2_TKIPAES;																			
					else if ((strncmp(macptr, "WPA_AES_WPA2_TKIP", 17) == 0) || (strncmp(macptr, "wpa_aes_wpa2_tkip", 17) == 0))
	    				pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_AES_WPA2_TKIP;								 						
					else if ((strncmp(macptr, "WPA_TKIP_WPA2_AES", 17) == 0) || (strncmp(macptr, "wpa_tkip_wpa2_aes", 17) == 0))
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_TKIP_WPA2_AES;								
					else if ((strncmp(macptr, "WPA_TKIP_WPA2_TKIPAES", 21) == 0) || (strncmp(macptr, "wpa_tkip_wpa2_tkipaes", 21) == 0))
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_TKIP_WPA2_TKIPAES;
					else if ((strncmp(macptr, "WPA_TKIPAES_WPA2_AES", 20) == 0) || (strncmp(macptr, "wpa_tkipaes_wpa2_aes", 20) == 0))
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_TKIPAES_WPA2_AES;
					else if ((strncmp(macptr, "WPA_TKIPAES_WPA2_TKIPAES", 24) == 0) || (strncmp(macptr, "wpa_tkipaes_wpa2_tkipaes", 24) == 0))
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_TKIPAES_WPA2_TKIPAES;
					else if ((strncmp(macptr, "WPA_TKIPAES_WPA2_TKIP", 21) == 0) || (strncmp(macptr, "wpa_tkipaes_wpa2_tkip", 21) == 0))
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_TKIPAES_WPA2_TKIP;
					else //Default
						pAd->ApCfg.MBSSID[i].WpaMixPairCipher = WPA_TKIPAES_WPA2_TKIPAES;

					DBGPRINT(RT_DEBUG_OFF, ("I/F(ra%d) MixWPACipher=0x%02x\n", i, pAd->ApCfg.MBSSID[i].WpaMixPairCipher));
				}																													
			}			
		
			//RekeyMethod
			if(RTMPGetKeyParameter("RekeyMethod", tmpbuf, 128, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{				
					PRT_WPA_REKEY pRekeyInfo = &pAd->ApCfg.MBSSID[i].WPAREKEY;
				
					if ((strcmp(macptr, "TIME") == 0) || (strcmp(macptr, "time") == 0))
						pRekeyInfo->ReKeyMethod = TIME_REKEY;
					else if ((strcmp(macptr, "PKT") == 0) || (strcmp(macptr, "pkt") == 0))
						pRekeyInfo->ReKeyMethod = PKT_REKEY;
					else if ((strcmp(macptr, "DISABLE") == 0) || (strcmp(macptr, "disable") == 0))
						pRekeyInfo->ReKeyMethod = DISABLE_REKEY;
					else
						pRekeyInfo->ReKeyMethod = DISABLE_REKEY;

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) ReKeyMethod=%ld\n", i, pRekeyInfo->ReKeyMethod));
				}

				// Apply to remaining MBSS
				if (i == 1)
				{
					for (i = 1; i < pAd->ApCfg.BssidNum; i++)
					{
						pAd->ApCfg.MBSSID[i].WPAREKEY.ReKeyMethod = 
								pAd->ApCfg.MBSSID[0].WPAREKEY.ReKeyMethod;
						DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) ReKeyMethod=%ld\n", 
											i, pAd->ApCfg.MBSSID[i].WPAREKEY.ReKeyMethod));
					}	
				}
			}
			//RekeyInterval
			if(RTMPGetKeyParameter("RekeyInterval", tmpbuf, 255, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{				
					ULONG	value_interval;
					PRT_WPA_REKEY pRekeyInfo = &pAd->ApCfg.MBSSID[i].WPAREKEY;

					value_interval = simple_strtol(macptr, 0, 10);
				
					if((value_interval >= 0) && (value_interval < MAX_REKEY_INTER))
						pRekeyInfo->ReKeyInterval = value_interval;
					else //Default
						pRekeyInfo->ReKeyInterval = 3600;

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) ReKeyInterval=%ld\n", 
													i, pRekeyInfo->ReKeyInterval));
				}

				// Apply to remaining MBSS
				if (i == 1)
				{
					for (i = 1; i < pAd->ApCfg.BssidNum; i++)
					{
						pAd->ApCfg.MBSSID[i].WPAREKEY.ReKeyInterval = 
								pAd->ApCfg.MBSSID[0].WPAREKEY.ReKeyInterval;
						DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) ReKeyInterval=%ld\n", 
													i, pAd->ApCfg.MBSSID[i].WPAREKEY.ReKeyInterval));
					}
				}

			}
			//PMKCachePeriod
			if(RTMPGetKeyParameter("PMKCachePeriod", tmpbuf, 255, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{									
					pAd->ApCfg.MBSSID[i].PMKCachePeriod = 
											simple_strtol(macptr, 0, 10) * 60 * OS_HZ;

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) PMKCachePeriod=%ld\n", 
												i, pAd->ApCfg.MBSSID[i].PMKCachePeriod));
				}

				// Apply to remaining MBSS
				if (i == 1)
				{
					for (i = 1; i < pAd->ApCfg.BssidNum; i++)
					{
						pAd->ApCfg.MBSSID[i].PMKCachePeriod = 
								pAd->ApCfg.MBSSID[0].PMKCachePeriod;

						DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) PMKCachePeriod=%ld\n", 
													i, pAd->ApCfg.MBSSID[i].PMKCachePeriod));					
					}
				}
			}

			//WPAPSK_KEY
			if(TRUE)
			{
							STRING tok_str[16];
				BOOLEAN bWPAPSKxIsUsed = FALSE;

				DBGPRINT(RT_DEBUG_TRACE, ("pAd->ApCfg.BssidNum=%d\n", pAd->ApCfg.BssidNum));

				for (i = 0; i < pAd->ApCfg.BssidNum; i++)
				{
					sprintf(tok_str, "WPAPSK%d", i + 1);
				if(RTMPGetKeyParameter(tok_str, tmpbuf, 65, pBuffer, FALSE))
					{
						rtmp_parse_wpapsk_buffer_from_file(pAd, tmpbuf, i);
						
						if (bWPAPSKxIsUsed == FALSE)
						{
							bWPAPSKxIsUsed = TRUE;
						}
					}
				}
				if (bWPAPSKxIsUsed == FALSE)
				{
				if (RTMPGetKeyParameter("WPAPSK", tmpbuf, 512, pBuffer, FALSE))
					{
						if (pAd->ApCfg.BssidNum == 1)
						{
							rtmp_parse_wpapsk_buffer_from_file(pAd, tmpbuf, BSS0);
						}
						else
						{
							// Anyway, we still do the legacy dissection of the whole WPAPSK passphrase.
						    for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
						    {
								rtmp_parse_wpapsk_buffer_from_file(pAd, macptr, i);
						    }

						}
					}
				}

#ifdef DBG
				for (i = 0; i < pAd->ApCfg.BssidNum; i++)
				{
					int j;
					
	                				DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) WPAPSK Key => \n", i));
	                				for (j = 0; j < 32; j++)
	                				{
	                    				DBGPRINT(RT_DEBUG_TRACE, ("%02x:", pAd->ApCfg.MBSSID[i].PMK[j]));
	                    				if ((j%16) == 15)
	                        					DBGPRINT(RT_DEBUG_TRACE, ("\n"));
	                				}
	                				DBGPRINT(RT_DEBUG_TRACE, ("\n"));
				}
#endif
			}
		}
#endif // CONFIG_AP_SUPPORT //


		//DefaultKeyID, KeyType, KeyStr
		rtmp_read_key_parms_from_file(pAd, tmpbuf, pBuffer);

#ifdef WAPI_SUPPORT
		rtmp_read_wapi_parms_from_file(pAd, tmpbuf, pBuffer);
#endif // WAPI_SUPPORT //

		//HSCounter
		/*if(RTMPGetKeyParameter("HSCounter", tmpbuf, 10, pBuffer, TRUE))
		{
			switch (simple_strtol(tmpbuf, 0, 10))
			{
				case 1: //Enable
					pAd->CommonCfg.bEnableHSCounter = TRUE;
					break;
				case 0: //Disable
				default:
					pAd->CommonCfg.bEnableHSCounter = FALSE;
					break;
			}
			DBGPRINT(RT_DEBUG_TRACE, "HSCounter=%d\n", pAd->CommonCfg.bEnableHSCounter);
		}*/
#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
			//Access Control List
			rtmp_read_acl_parms_from_file(pAd, tmpbuf, pBuffer);

#ifdef APCLI_SUPPORT					
			rtmp_read_ap_client_from_file(pAd, tmpbuf, pBuffer);
#endif // APCLI_SUPPORT //

#ifdef IGMP_SNOOP_SUPPORT
			// Igmp Snooping information
			rtmp_read_igmp_snoop_from_file(pAd, tmpbuf, pBuffer);
#endif // IGMP_SNOOP_SUPPORT //

#ifdef WDS_SUPPORT
			rtmp_read_wds_from_file(pAd, tmpbuf, pBuffer);
//#else // WDS_SUPPORT //
//			pAd->WdsTab.Mode = WDS_DISABLE_MODE;
#endif // WDS_SUPPORT //
#ifdef DOT1X_SUPPORT
			rtmp_read_radius_parms_from_file(pAd, tmpbuf, pBuffer);
#endif // DOT1X_SUPPORT //

#ifdef IDS_SUPPORT
			rtmp_read_ids_from_file(pAd, tmpbuf, pBuffer);
#endif // IDS_SUPPORT //
		}

#endif // CONFIG_AP_SUPPORT //

#ifdef DOT11_N_SUPPORT
		HTParametersHook(pAd, tmpbuf, pBuffer);
#endif // DOT11_N_SUPPORT //

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
#ifdef WSC_AP_SUPPORT
			STRING	tok_str[16] = {0};
			for (i = 0; i < pAd->ApCfg.BssidNum; i++)
			{
				sprintf(tok_str, "WscDefaultSSID%d", i + 1);
				if(RTMPGetKeyParameter(tok_str, tmpbuf, 33, pBuffer, FALSE))
				{
					NdisZeroMemory(&pAd->ApCfg.MBSSID[i].WscControl.WscDefaultSsid, sizeof(NDIS_802_11_SSID));
					NdisMoveMemory(pAd->ApCfg.MBSSID[i].WscControl.WscDefaultSsid.Ssid, tmpbuf , strlen(tmpbuf));
					pAd->ApCfg.MBSSID[i].WscControl.WscDefaultSsid.SsidLength = strlen(tmpbuf);
			    	DBGPRINT(RT_DEBUG_TRACE, ("WscDefaultSSID[%d]=%s\n", i, pAd->ApCfg.MBSSID[i].WscControl.WscDefaultSsid.Ssid));
				}
			}
			
			//WscConfMode
			if(RTMPGetKeyParameter("WscConfMode", tmpbuf, 10, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
			    {
					INT WscConfMode = simple_strtol(macptr, 0, 10);
					
					if (i >= pAd->ApCfg.BssidNum)
						break;

					if (WscConfMode > 0 && WscConfMode < 8)
					{
						pAd->ApCfg.MBSSID[i].WscControl.WscConfMode = WscConfMode;
					}
					else
					{
						pAd->ApCfg.MBSSID[i].WscControl.WscConfMode = WSC_DISABLE;
					}

					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) WscConfMode=%d\n", i, pAd->ApCfg.MBSSID[i].WscControl.WscConfMode));
				}
			}

			//WscConfStatus
			if(RTMPGetKeyParameter("WscConfStatus", tmpbuf, 10, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{
					if (i >= pAd->ApCfg.BssidNum)
						break;

					pAd->ApCfg.MBSSID[i].WscControl.WscConfStatus = (INT) simple_strtol(macptr, 0, 10);
					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) WscConfStatus=%d\n", i, pAd->ApCfg.MBSSID[i].WscControl.WscConfStatus));
				}
			}
			//WscConfMethods
			if(RTMPGetKeyParameter("WscConfMethods", tmpbuf, 32, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{
					if (i >= pAd->ApCfg.BssidNum)
						break;

					pAd->ApCfg.MBSSID[i].WscControl.WscConfigMethods = (USHORT)simple_strtol(macptr, 0, 16);
					DBGPRINT(RT_DEBUG_TRACE, ("I/F(ra%d) WscConfMethods=0x%x\n", i, pAd->ApCfg.MBSSID[i].WscControl.WscConfigMethods));
				}
			}

			//WscKeyASCII (0:Hex, 1:ASCII(random length), others: ASCII length, default 8)
			if (RTMPGetKeyParameter("WscKeyASCII", tmpbuf, 10, pBuffer, TRUE))
			{
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{
					INT Value;
					
					if (i >= pAd->ApCfg.BssidNum)
						break;

					Value = (INT) simple_strtol(tmpbuf, 0, 10);
					if(Value==0 || Value==1)
						pAd->ApCfg.MBSSID[i].WscControl.WscKeyASCII = Value;
					else if(Value >= 8 && Value <=63)
						pAd->ApCfg.MBSSID[i].WscControl.WscKeyASCII = Value;
					else
						pAd->ApCfg.MBSSID[i].WscControl.WscKeyASCII = 8;
					DBGPRINT(RT_DEBUG_WARN, ("WscKeyASCII=%d\n", pAd->ApCfg.MBSSID[i].WscControl.WscKeyASCII));
				}								
			}

			// WCNTest
			if(RTMPGetKeyParameter("WCNTest", tmpbuf, 10, pBuffer, TRUE))
			{
				BOOLEAN	bEn = FALSE;
				
				if ((strncmp(tmpbuf, "0", 1) == 0))
					bEn = FALSE;
				else
					bEn = TRUE;

				for (i = 0; i < pAd->ApCfg.BssidNum; i++)
				{
					pAd->ApCfg.MBSSID[i].WscControl.bWCNTest = bEn;
				}
				DBGPRINT(RT_DEBUG_TRACE, ("WCNTest=%d\n", bEn));
			}
#endif // WSC_AP_SUPPORT //
		}
#endif // CONFIG_AP_SUPPORT //

#ifdef CARRIER_DETECTION_SUPPORT
			//CarrierDetect
			if(RTMPGetKeyParameter("CarrierDetect", tmpbuf, 128, pBuffer, TRUE))
			{
				if ((strncmp(tmpbuf, "0", 1) == 0))
					pAd->CommonCfg.CarrierDetect.Enable = FALSE;
				else if ((strncmp(tmpbuf, "1", 1) == 0))
					pAd->CommonCfg.CarrierDetect.Enable = TRUE;
				else
					pAd->CommonCfg.CarrierDetect.Enable = FALSE;

				DBGPRINT(RT_DEBUG_TRACE, ("CarrierDetect.Enable=%d\n", pAd->CommonCfg.CarrierDetect.Enable));
			}
			else
				pAd->CommonCfg.CarrierDetect.Enable = FALSE;
#endif // CARRIER_DETECTION_SUPPORT //


#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		{
#ifdef MCAST_RATE_SPECIFIC
			// McastPhyMode
			if (RTMPGetKeyParameter("McastPhyMode", tmpbuf, 32, pBuffer, TRUE))
			{	
				UCHAR PhyMode = simple_strtol(tmpbuf, 0, 10);
							pAd->CommonCfg.MCastPhyMode.field.BW = pAd->CommonCfg.RegTransmitSetting.field.BW;
				switch (PhyMode)
				{
								case MCAST_DISABLE: // disable
									NdisMoveMemory(&pAd->CommonCfg.MCastPhyMode,
										&pAd->MacTab.Content[MCAST_WCID].HTPhyMode, sizeof(HTTRANSMIT_SETTING));
									break;

								case MCAST_CCK:	// CCK
									pAd->CommonCfg.MCastPhyMode.field.MODE = MODE_CCK;
									pAd->CommonCfg.MCastPhyMode.field.BW =  BW_20;
						break;

								case MCAST_OFDM:	// OFDM
									pAd->CommonCfg.MCastPhyMode.field.MODE = MODE_OFDM;
						break;
#ifdef DOT11_N_SUPPORT
								case MCAST_HTMIX:	// HTMIX
									pAd->CommonCfg.MCastPhyMode.field.MODE = MODE_HTMIX;
						break;
#endif // DOT11_N_SUPPORT //									

					default:
						DBGPRINT(RT_DEBUG_OFF, ("unknow Muticast PhyMode %d.\n", PhyMode));
						DBGPRINT(RT_DEBUG_OFF, ("0:Disable 1:CCK, 2:OFDM, 3:HTMIX.\n"));
						break;
				}
			}
			else
							NdisMoveMemory(&pAd->CommonCfg.MCastPhyMode,
								&pAd->MacTab.Content[MCAST_WCID].HTPhyMode, sizeof(HTTRANSMIT_SETTING));

			// McastMcs
			if (RTMPGetKeyParameter("McastMcs", tmpbuf, 32, pBuffer, TRUE))
			{
				UCHAR Mcs = simple_strtol(tmpbuf, 0, 10);
				switch(pAd->CommonCfg.MCastPhyMode.field.MODE)
				{
					case MODE_CCK:
						if ((Mcs <= 3) || (Mcs >= 8 && Mcs <= 11))
							pAd->CommonCfg.MCastPhyMode.field.MCS = Mcs;
						else
							DBGPRINT(RT_DEBUG_OFF, ("MCS must in range of 0 ~ 3 and 8 ~ 11 for CCK Mode.\n"));
						break;

					case MODE_OFDM:
						if (Mcs > 7)
							DBGPRINT(RT_DEBUG_OFF, ("MCS must in range from 0 to 7 for CCK Mode.\n"));
						else
							pAd->CommonCfg.MCastPhyMode.field.MCS = Mcs;
						break;

					default:
						pAd->CommonCfg.MCastPhyMode.field.MCS = Mcs;
						break;
				}
			}
			else
				pAd->CommonCfg.MCastPhyMode.field.MCS = 0;
#endif // MCAST_RATE_SPECIFIC //

#ifdef DOT11_N_SUPPORT
#ifdef DOT11N_DRAFT3
			if (RTMPGetKeyParameter("OBSSScanParam", tmpbuf, 32, pBuffer, TRUE))
			{	int ObssScanValue;
				for (i = 0, macptr = rstrtok(tmpbuf,";"); macptr; macptr = rstrtok(NULL,";"), i++)
				{
					ObssScanValue = simple_strtol(macptr, 0, 10);
					switch (i)
					{
						case 0:
							if (ObssScanValue < 5 || ObssScanValue > 1000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanPassiveDwell(%d), should in range 5~1000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanPassiveDwell = ObssScanValue;	// Unit : TU. 5~1000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanPassiveDwell=%d\n", ObssScanValue));
							}
							break;
						case 1:
							if (ObssScanValue < 10 || ObssScanValue > 1000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanActiveDwell(%d), should in range 10~1000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanActiveDwell = ObssScanValue;	// Unit : TU. 10~1000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanActiveDwell=%d\n", ObssScanValue));
							}
							break;
						case 2:
							pAd->CommonCfg.Dot11BssWidthTriggerScanInt = ObssScanValue;	// Unit : Second
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthTriggerScanInt=%d\n", ObssScanValue));
							break;
						case 3:
							if (ObssScanValue < 200 || ObssScanValue > 10000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanPassiveTotalPerChannel(%d), should in range 200~10000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = ObssScanValue;	// Unit : TU. 200~10000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanPassiveTotalPerChannel=%d\n", ObssScanValue));
							}
							break;
						case 4:
							if (ObssScanValue < 20 || ObssScanValue > 10000)
							{
								DBGPRINT(RT_DEBUG_ERROR, ("Invalid OBSSScanParam for Dot11OBssScanActiveTotalPerChannel(%d), should in range 20~10000\n", ObssScanValue));
							}
							else
							{
								pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = ObssScanValue;	// Unit : TU. 20~10000
								DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11OBssScanActiveTotalPerChannel=%d\n", ObssScanValue));
							}
							break;
						case 5:
							pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = ObssScanValue;
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelayFactor=%d\n", ObssScanValue));
							break;
						case 6:
							pAd->CommonCfg.Dot11OBssScanActivityThre = ObssScanValue;	// Unit : percentage
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelayFactor=%d\n", ObssScanValue));
							break;
					}			
				}

				if (i != 7)
				{
					DBGPRINT(RT_DEBUG_ERROR, ("Wrong OBSSScanParamtetrs format in dat file!!!!! Use default value.\n"));

					pAd->CommonCfg.Dot11OBssScanPassiveDwell = dot11OBSSScanPassiveDwell;	// Unit : TU. 5~1000
					pAd->CommonCfg.Dot11OBssScanActiveDwell = dot11OBSSScanActiveDwell;	// Unit : TU. 10~1000
					pAd->CommonCfg.Dot11BssWidthTriggerScanInt = dot11BSSWidthTriggerScanInterval;	// Unit : Second	
					pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = dot11OBSSScanPassiveTotalPerChannel;	// Unit : TU. 200~10000
					pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = dot11OBSSScanActiveTotalPerChannel;	// Unit : TU. 20~10000
					pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = dot11BSSWidthChannelTransactionDelayFactor;
					pAd->CommonCfg.Dot11OBssScanActivityThre = dot11BSSScanActivityThreshold;	// Unit : percentage
				}
				pAd->CommonCfg.Dot11BssWidthChanTranDelay = (pAd->CommonCfg.Dot11BssWidthTriggerScanInt * pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor);
							DBGPRINT(RT_DEBUG_TRACE, ("OBSSScanParam for Dot11BssWidthChanTranDelay=%ld\n", pAd->CommonCfg.Dot11BssWidthChanTranDelay));
			}
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //
		}
#endif // CONFIG_AP_SUPPORT //
#ifdef WSC_INCLUDED
	    if (RTMPGetKeyParameter("WscVendorPinCode", tmpbuf, 256, pBuffer, TRUE))
	    {
			PWSC_CTRL pWscContrl;
			int bSetOk;
#ifdef CONFIG_AP_SUPPORT
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
			{
				pWscContrl = &pAd->ApCfg.MBSSID[BSS0].WscControl;
			}
#endif // CONFIG_AP_SUPPORT //
			bSetOk = RT_CfgSetWscPinCode(pAd, tmpbuf, pWscContrl);
			if (bSetOk)
				DBGPRINT(RT_DEBUG_TRACE, ("%s - WscVendorPinCode= (%d)\n", __FUNCTION__, bSetOk));
		        else
		            DBGPRINT(RT_DEBUG_ERROR, ("%s - WscVendorPinCode: invalid pin code(%s)\n", __FUNCTION__, tmpbuf));
	    }
#endif // WSC_INCLUDED //

#ifdef CONFIG_AP_SUPPORT
#endif // CONFIG_AP_SUPPORT //

#ifdef RT30xx
#endif // RT30xx //

#ifdef CONFIG_AP_SUPPORT
		// EntryLifeCheck is used to check 
		if (RTMPGetKeyParameter("EntryLifeCheck", tmpbuf, 256, pBuffer, TRUE))
		{
			long LifeCheckCnt = simple_strtol(tmpbuf, 0, 10);
			if (LifeCheckCnt <= 65535)
				pAd->ApCfg.EntryLifeCheck = LifeCheckCnt;
			else
				pAd->ApCfg.EntryLifeCheck = MAC_ENTRY_LIFE_CHECK_CNT;
		}

#endif // CONFIG_AP_SUPPORT //

#ifdef SINGLE_SKU
		if(RTMPGetKeyParameter("AntGain", tmpbuf, 10, pBuffer, TRUE))
		{
			UCHAR AntGain = simple_strtol(tmpbuf, 0, 10);
			pAd->CommonCfg.AntGain= AntGain;
	
			DBGPRINT(RT_DEBUG_TRACE, ("AntGain=%d\n", pAd->CommonCfg.AntGain));
		}
		if(RTMPGetKeyParameter("BandedgeDelta", tmpbuf, 10, pBuffer, TRUE))
		{
			UCHAR Bandedge = simple_strtol(tmpbuf, 0, 10);
			pAd->CommonCfg.BandedgeDelta = Bandedge;

			DBGPRINT(RT_DEBUG_TRACE, ("BandedgeDelta=%d\n", pAd->CommonCfg.BandedgeDelta));
		}
#endif // SINGLE_SKU //
	}while(0);


	kfree(tmpbuf);
	
	return NDIS_STATUS_SUCCESS;
	
}


#ifdef MULTIPLE_CARD_SUPPORT
// record whether the card in the card list is used in the card file
UINT8  MC_CardUsed[MAX_NUM_OF_MULTIPLE_CARD];
// record used card mac address in the card list
static UINT8  MC_CardMac[MAX_NUM_OF_MULTIPLE_CARD][6];

/*
========================================================================
Routine Description:
    Get card profile path.

Arguments:
    pAd

Return Value:
    TRUE		- Find a card profile
	FALSE		- use default profile

Note:
========================================================================
*/
BOOLEAN RTMP_CardInfoRead(
	IN	PRTMP_ADAPTER pAd)
{
#define MC_SELECT_CARDID		0	/* use CARD ID (0 ~ 31) to identify different cards */
#define MC_SELECT_MAC			1	/* use CARD MAC to identify different cards */
#define MC_SELECT_CARDTYPE		2	/* use CARD type (abgn or bgn) to identify different cards */

#define LETTER_CASE_TRANSLATE(txt_p, card_id)			\
	{	UINT32 _len; char _char;						\
		for(_len=0; _len<strlen(card_id); _len++) {		\
			_char = *(txt_p + _len);					\
			if (('A' <= _char) && (_char <= 'Z'))		\
				*(txt_p+_len) = 'a'+(_char-'A');		\
		} }

	RTMP_OS_FD srcf;
	INT retval;
	PSTRING buffer, tmpbuf;
	STRING card_id_buf[30], RFIC_word[30];
	BOOLEAN flg_match_ok = FALSE;
	INT32 card_select_method;
	INT32 card_free_id, card_nouse_id, card_same_mac_id, card_match_id;
	EEPROM_ANTENNA_STRUC antenna;
	USHORT addr01, addr23, addr45;
	UINT8 mac[6];
	UINT32 data, card_index;
	UCHAR *start_ptr;
	RTMP_OS_FS_INFO osFSInfo;

	// init
	buffer = kmalloc(MAX_INI_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if (buffer == NULL)
		return FALSE;

	tmpbuf = kmalloc(MAX_PARAM_BUFFER_SIZE, MEM_ALLOC_FLAG);
	if(tmpbuf == NULL)
	{
		kfree(buffer);
		return NDIS_STATUS_FAILURE;
	}

	// get RF IC type
	RTMP_IO_READ32(pAd, E2PROM_CSR, &data);

	if ((data & 0x30) == 0)
		pAd->EEPROMAddressNum = 6;	// 93C46
	else if ((data & 0x30) == 0x10)
		pAd->EEPROMAddressNum = 8;	// 93C66
	else
		pAd->EEPROMAddressNum = 8;	// 93C86
	
	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC1_OFFSET, antenna.word);

	if ((antenna.field.RfIcType == RFIC_2850) ||
		(antenna.field.RfIcType == RFIC_2750))
	{
		/* ABGN card */
		strcpy(RFIC_word, "abgn");
	}
	else
	{
		/* BGN card */
		strcpy(RFIC_word, "bgn");
	}

	// get MAC address
	RT28xx_EEPROM_READ16(pAd, 0x04, addr01);
	RT28xx_EEPROM_READ16(pAd, 0x06, addr23);
	RT28xx_EEPROM_READ16(pAd, 0x08, addr45);

	mac[0] = (UCHAR)(addr01 & 0xff);
	mac[1] = (UCHAR)(addr01 >> 8);
	mac[2] = (UCHAR)(addr23 & 0xff);
	mac[3] = (UCHAR)(addr23 >> 8);
	mac[4] = (UCHAR)(addr45 & 0xff);
	mac[5] = (UCHAR)(addr45 >> 8);

	DBGPRINT(RT_DEBUG_TRACE, ("mac addr=%02x:%02x:%02x:%02x:%02x:%02x!\n", PRINT_MAC(mac)));
	
	RtmpOSFSInfoChange(&osFSInfo, TRUE);
	// open card information file
	srcf = RtmpOSFileOpen(CARD_INFO_PATH, O_RDONLY, 0);
	if (IS_FILE_OPEN_ERR(srcf)) 
	{
		/* card information file does not exist */
			DBGPRINT(RT_DEBUG_TRACE,
				("--> Error opening %s\n", CARD_INFO_PATH));
		goto  free_resource;
	}

	/* card information file exists so reading the card information */
	memset(buffer, 0x00, MAX_INI_BUFFER_SIZE);
	retval = RtmpOSFileRead(srcf, buffer, MAX_INI_BUFFER_SIZE);
	if (retval < 0)
	{
		/* read fail */
			DBGPRINT(RT_DEBUG_TRACE,
				("--> Read %s error %d\n", CARD_INFO_PATH, -retval));
	}
	else
	{
		/* get card selection method */
		memset(tmpbuf, 0x00, MAX_PARAM_BUFFER_SIZE);
		card_select_method = MC_SELECT_CARDTYPE; // default

		if (RTMPGetKeyParameter("SELECT", tmpbuf, 256, buffer, TRUE))
		{
			if (strcmp(tmpbuf, "CARDID") == 0)
				card_select_method = MC_SELECT_CARDID;
			else if (strcmp(tmpbuf, "MAC") == 0)
				card_select_method = MC_SELECT_MAC;
			else if (strcmp(tmpbuf, "CARDTYPE") == 0)
				card_select_method = MC_SELECT_CARDTYPE;
		}

		DBGPRINT(RT_DEBUG_TRACE,
				("MC> Card Selection = %d\n", card_select_method));

		// init
		card_free_id = -1;
		card_nouse_id = -1;
		card_same_mac_id = -1;
		card_match_id = -1;

		// search current card information records
		for(card_index=0;
			card_index<MAX_NUM_OF_MULTIPLE_CARD;
			card_index++)
		{
			if ((*(UINT32 *)&MC_CardMac[card_index][0] == 0) &&
				(*(UINT16 *)&MC_CardMac[card_index][4] == 0))
			{
				// MAC is all-0 so the entry is available
				MC_CardUsed[card_index] = 0;

				if (card_free_id < 0)
					card_free_id = card_index; // 1st free entry
			}
			else
			{
				if (memcmp(MC_CardMac[card_index], mac, 6) == 0)
				{
					// we find the entry with same MAC
					if (card_same_mac_id < 0)
						card_same_mac_id = card_index; // 1st same entry
				}
				else
				{
					// MAC is not all-0 but used flag == 0
					if ((MC_CardUsed[card_index] == 0) &&
						(card_nouse_id < 0))
					{
						card_nouse_id = card_index; // 1st available entry
					}
				}
			}
		}

		DBGPRINT(RT_DEBUG_TRACE,
				("MC> Free = %d, Same = %d, NOUSE = %d\n",
				card_free_id, card_same_mac_id, card_nouse_id));

		if ((card_same_mac_id >= 0) &&
			((card_select_method == MC_SELECT_CARDID) ||
			(card_select_method == MC_SELECT_CARDTYPE)))
		{
			// same MAC entry is found
			card_match_id = card_same_mac_id;

			if (card_select_method == MC_SELECT_CARDTYPE)
			{
				// for CARDTYPE
				sprintf(card_id_buf, "%02dCARDTYPE%s",
						card_match_id, RFIC_word);

				if ((start_ptr = (PUCHAR)rtstrstruncasecmp(buffer, card_id_buf)) != NULL)
				{
					// we found the card ID
					LETTER_CASE_TRANSLATE(start_ptr, card_id_buf);
				}
			}
		}
		else
		{
			// the card is 1st plug-in, try to find the match card profile
			switch(card_select_method)
			{
				case MC_SELECT_CARDID: // CARDID
				default:
					if (card_free_id >= 0)
						card_match_id = card_free_id;
					else
						card_match_id = card_nouse_id;
					break;

				case MC_SELECT_MAC: // MAC
					sprintf(card_id_buf, "MAC%02x:%02x:%02x:%02x:%02x:%02x",
							mac[0], mac[1], mac[2],
							mac[3], mac[4], mac[5]);

					/* try to find the key word in the card file */
					if ((start_ptr = (PUCHAR)rtstrstruncasecmp(buffer, card_id_buf)) != NULL)
					{
						LETTER_CASE_TRANSLATE(start_ptr, card_id_buf);

						/* get the row ID (2 ASCII characters) */
						start_ptr -= 2;
						card_id_buf[0] = *(start_ptr);
						card_id_buf[1] = *(start_ptr+1);
						card_id_buf[2] = 0x00;

						card_match_id = simple_strtol(card_id_buf, 0, 10);
					}
					break;

				case MC_SELECT_CARDTYPE: // CARDTYPE
					card_nouse_id = -1;

					for(card_index=0;
						card_index<MAX_NUM_OF_MULTIPLE_CARD;
						card_index++)
					{
						sprintf(card_id_buf, "%02dCARDTYPE%s",
								card_index, RFIC_word);

						if ((start_ptr = (PUCHAR)rtstrstruncasecmp(buffer,
													card_id_buf)) != NULL)
						{
							LETTER_CASE_TRANSLATE(start_ptr, card_id_buf);

							if (MC_CardUsed[card_index] == 0)
							{
								/* current the card profile is not used */
								if ((*(UINT32 *)&MC_CardMac[card_index][0] == 0) &&
									(*(UINT16 *)&MC_CardMac[card_index][4] == 0))
								{
									// find it and no previous card use it
									card_match_id = card_index;
									break;
								}
								else
								{
									// ever a card use it
									if (card_nouse_id < 0)
										card_nouse_id = card_index;
								}
							}
						}
					}

					// if not find a free one, use the available one
					if (card_match_id < 0)
						card_match_id = card_nouse_id;
					break;
			}
		}

		if (card_match_id >= 0)
		{
			// make up search keyword
			switch(card_select_method)
			{
				case MC_SELECT_CARDID: // CARDID
					sprintf(card_id_buf, "%02dCARDID", card_match_id);
					break;

				case MC_SELECT_MAC: // MAC
					sprintf(card_id_buf,
							"%02dmac%02x:%02x:%02x:%02x:%02x:%02x",
							card_match_id,
							mac[0], mac[1], mac[2],
							mac[3], mac[4], mac[5]);
					break;

				case MC_SELECT_CARDTYPE: // CARDTYPE
				default:
					sprintf(card_id_buf, "%02dcardtype%s",
							card_match_id, RFIC_word);
					break;
			}

			DBGPRINT(RT_DEBUG_TRACE, ("Search Keyword = %s\n", card_id_buf));

			// read card file path
			if (RTMPGetKeyParameter(card_id_buf, tmpbuf, 256, buffer, TRUE))
			{
				if (strlen(tmpbuf) < sizeof(pAd->MC_FileName))
				{
					// backup card information
					pAd->MC_RowID = card_match_id; /* base 0 */
					MC_CardUsed[card_match_id] = 1;
					memcpy(MC_CardMac[card_match_id], mac, sizeof(mac));

					// backup card file path
					NdisMoveMemory(pAd->MC_FileName, tmpbuf , strlen(tmpbuf));

					pAd->MC_FileName[strlen(tmpbuf)] = End_Mark;

					flg_match_ok = TRUE;

					DBGPRINT(RT_DEBUG_TRACE,
							("Card Profile Name = %s\n", pAd->MC_FileName));
				}
				else
				{
					DBGPRINT(RT_DEBUG_ERROR,
							("Card Profile Name length too large!\n"));
				}
			}
			else
			{
				DBGPRINT(RT_DEBUG_ERROR,
						("Can not find search key word in card.dat!\n"));
			}

			if ((flg_match_ok != TRUE) &&
				(card_match_id < MAX_NUM_OF_MULTIPLE_CARD))
			{
				MC_CardUsed[card_match_id] = 0;
				memset(MC_CardMac[card_match_id], 0, sizeof(mac));
			}
		} // if (card_match_id >= 0)
	}


	// close file
	retval = RtmpOSFileClose(srcf);

free_resource:
	RtmpOSFSInfoChange(&osFSInfo, FALSE);
	kfree(buffer);
	kfree(tmpbuf);

	return flg_match_ok;
}
#endif // MULTIPLE_CARD_SUPPORT //

