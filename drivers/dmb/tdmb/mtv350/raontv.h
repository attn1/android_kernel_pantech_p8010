/******************************************************************************** 
* (c) COPYRIGHT 2010 RAONTECH, Inc. ALL RIGHTS RESERVED.
* 
* This software is the property of RAONTECH and is furnished under license by RAONTECH.                
* This software may be used only in accordance with the terms of said license.                         
* This copyright noitce may not be remoced, modified or obliterated without the prior                  
* written permission of RAONTECH, Inc.                                                                 
*                                                                                                      
* This software may not be copied, transmitted, provided to or otherwise made available                
* to any other person, company, corporation or other entity except as specified in the                 
* terms of said license.                                                                               
*                                                                                                      
* No right, title, ownership or other interest in the software is hereby granted or transferred.       
*                                                                                                      
* The information contained herein is subject to change without notice and should 
* not be construed as a commitment by RAONTECH, Inc.                                                                    
* 
* TITLE 	  : RAONTECH TV device driver API header file. 
*
* FILENAME    : raontv.h
*
* DESCRIPTION : 
*		This file contains types and declarations associated with the RAONTECH
*		TV Services.
*
********************************************************************************/

/******************************************************************************** 
* REVISION HISTORY
*
*    DATE	  	  NAME				REMARKS
* ----------  -------------    --------------------------------------------------
* 10/14/2010  Ko, Kevin        Added the RTV_FM_CH_STEP_FREQ_KHz defintion.
* 10/06/2010  Ko, Kevin        Added RTV_ISDBT_FREQ2CHNUM macro for ISDB-T.
* 09/27/2010  Ko, Kevin        Creat for CS Realease
*             /Yang, Maverick  1.Reformating for CS API
*                              2.pll table, ADC clock switching, SCAN function, 
*								 FM function added..
********************************************************************************/

#ifndef __RAONTV_H__
#define __RAONTV_H__

#ifdef __cplusplus 
extern "C"{ 
#endif  


#include "raontv_port.h"

#define RAONTV_CHIP_ID		0x8A

/*==============================================================================
 *
 * Common definitions and types.
 *
 *============================================================================*/
#ifndef NULL
	#define NULL    	0
#endif

#ifndef FALSE
	#define FALSE		0
#endif

#ifndef TRUE
	#define TRUE		1
#endif

#ifndef MAX
	#define MAX(a, b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
	#define MIN(a, b)    (((a) < (b)) ? (a) : (b))
#endif

#ifndef ABS
	#define ABS(x) 		 (((x) < 0) ? -(x) : (x))
#endif


#define	RTV_TS_PACKET_SIZE		188



/* Error codes. */
#define RTV_SUCCESS							0
#define RTV_INVAILD_COUNTRY_BAND			-1
#define RTV_UNSUPPORT_ADC_CLK				-2
#define RTV_INVAILD_TV_MODE				-3
#define RTV_CHANNEL_NOT_DETECTED			-4
#define RTV_INSUFFICIENT_CHANNEL_BUF		-5
#define RTV_INVAILD_FREQ					-6
#define RTV_INVAILD_SUB_CHANNEL_ID		-7 // for T-DMB and DAB
#define RTV_NO_MORE_SUB_CHANNEL		        -8 // for T-DMB and DAB
#define RTV_INVAILD_THRESHOLD_SIZE		-9 
#define RTV_POWER_ON_CHECK_ERROR			-10 



typedef enum
{
	RTV_COUNTRY_BAND_JAPAN = 0,
	RTV_COUNTRY_BAND_KOREA,		
	RTV_COUNTRY_BAND_BRAZIL,
	RTV_COUNTRY_BAND_ARGENTINA 
} E_RTV_COUNTRY_BAND_TYPE;


// Do not modify the order!
typedef enum
{
	RTV_ADC_CLK_FREQ_8_MHz = 0,
	RTV_ADC_CLK_FREQ_8_192_MHz,
	RTV_ADC_CLK_FREQ_9_MHz,
	RTV_ADC_CLK_FREQ_9_6_MHz,
	MAX_NUM_RTV_ADC_CLK_FREQ_TYPE
} E_RTV_ADC_CLK_FREQ_TYPE;


// Modulation
typedef enum
{
	RTV_MOD_DQPSK = 0,
	RTV_MOD_QPSK,
	RTV_MOD_16QAM,
	RTV_MOD_64QAM
} E_RTV_MODULATION_TYPE;

typedef enum
{
	RTV_CODE_RATE_1_2 = 0,
	RTV_CODE_RATE_2_3,
	RTV_CODE_RATE_3_4,
	RTV_CODE_RATE_5_6,
	RTV_CODE_RATE_7_8
} E_RTV_CODE_RATE_TYPE;



/*==============================================================================
 *
 * ISDB-T definitions, types and APIs.
 *
 *============================================================================*/
static INLINE UINT RTV_ISDBT_FREQ2CHNUM(E_RTV_COUNTRY_BAND_TYPE eRtvCountryBandType, U32 dwFreqKHz)
{
	switch( eRtvCountryBandType )
	{
		case RTV_COUNTRY_BAND_JAPAN:
			return ((dwFreqKHz - 395143) / 6000);
			
		case RTV_COUNTRY_BAND_BRAZIL:
		case RTV_COUNTRY_BAND_ARGENTINA: 
			return (((dwFreqKHz - 395143) / 6000) + 1);
			
		default:
			return 0xFFFF;
	}
}


#define RTV_ISDBT_OFDM_LOCK_MASK	0x1
#define RTV_ISDBT_TMCC_LOCK_MASK	0x2
#define RTV_ISDBT_CHANNEL_LOCK_OK	(RTV_ISDBT_OFDM_LOCK_MASK|RTV_ISDBT_TMCC_LOCK_MASK)

#define RTV_ISDBT_BER_DIVIDER		100000
#define RTV_ISDBT_CNR_DIVIDER		10000
#define RTV_ISDBT_RSSI_DIVIDER		10


typedef enum
{
	RTV_ISDBT_SEG_1 = 0,
	RTV_ISDBT_SEG_3
} E_RTV_ISDBT_SEG_TYPE;

typedef enum
{
	RTV_ISDBT_MODE_1 = 0, // 2048
	RTV_ISDBT_MODE_2,	  // 4096
	RTV_ISDBT_MODE_3      // 8192 fft
} E_RTV_ISDBT_MODE_TYPE;

typedef enum
{
	RTV_ISDBT_GUARD_1_32 = 0, /* 1/32 */
	RTV_ISDBT_GUARD_1_16,     /* 1/16 */
	RTV_ISDBT_GUARD_1_8,      /* 1/8 */
	RTV_ISDBT_GUARD_1_4       /* 1/4 */
} E_RTV_ISDBT_GUARD_TYPE;


typedef enum
{
	RTV_ISDBT_INTERLV_0 = 0,
	RTV_ISDBT_INTERLV_1,
	RTV_ISDBT_INTERLV_2,
	RTV_ISDBT_INTERLV_4,
	RTV_ISDBT_INTERLV_8,
	RTV_ISDBT_INTERLV_16,
	RTV_ISDBT_INTERLV_32
} E_RTV_ISDBT_INTERLV_TYPE;


// for Layer A.
typedef struct
{
	E_RTV_ISDBT_SEG_TYPE		eSeg;
	E_RTV_ISDBT_MODE_TYPE		eTvMode;
	E_RTV_ISDBT_GUARD_TYPE		eGuard;
	E_RTV_MODULATION_TYPE		eModulation;
	E_RTV_CODE_RATE_TYPE		eCodeRate;
	E_RTV_ISDBT_INTERLV_TYPE	eInterlv;
	int						fEWS;	
} RTV_ISDBT_TMCC_INFO;

void rtvISDBT_StandbyMode(int on);
UINT rtvISDBT_GetLockStatus(void); 
U8   rtvISDBT_GetAGC(void);
S32  rtvISDBT_GetRSSI(void);
U32  rtvISDBT_GetPER(void);
U32  rtvISDBT_GetCNR(void);
U32  rtvISDBT_GetBER(void);
void rtvISDBT_GetTMCC(RTV_ISDBT_TMCC_INFO *ptTmccInfo);
void rtvISDBT_DisableStreamOut(void);
INT  rtvISDBT_SetFrequency(UINT nChNum);
INT  rtvISDBT_ScanFrequency(UINT nChNum);
void rtvISDBT_SwReset(void);
INT  rtvISDBT_Initialize(E_RTV_COUNTRY_BAND_TYPE eRtvCountryBandType, UINT nThresholdSize);


/*==============================================================================
 *
 * FM definitions, types and APIs.
 *
 *============================================================================*/
#define RTV_FM_PILOT_LOCK_MASK	0x1
#define RTV_FM_RDS_LOCK_MASK		0x2
#define RTV_FM_CHANNEL_LOCK_OK      (RTV_FM_PILOT_LOCK_MASK|RTV_FM_RDS_LOCK_MASK)

typedef enum
{
	RTV_FM_OUTPUT_MODE_AUTO = 0,
	RTV_FM_OUTPUT_MODE_MONO = 1,
	RTV_FM_OUTPUT_MODE_STEREO = 2
} E_RTV_FM_OUTPUT_MODE_TYPE;

void rtvFM_StandbyMode(int on);
void rtvFM_GetLockStatus(UINT *pLockVal, UINT *pLockCnt);
void rtvFM_SetOutputMode(E_RTV_FM_OUTPUT_MODE_TYPE eOutputMode);
void rtvFM_DisableStreamOut(void);
INT  rtvFM_SetFrequency(U32 dwChFreqKHz);
INT  rtvFM_SearchFrequency(U32 *pDetectedFreqKHz, U32 dwStartFreqKHz, U32 dwEndFreqKHz);
INT  rtvFM_ScanFrequency(U32 *pChBuf, UINT nNumChBuf, U32 dwStartFreqKHz, U32 dwEndFreqKHz);
INT  rtvFM_Initialize(E_RTV_ADC_CLK_FREQ_TYPE eAdcClkFreqType, UINT nThresholdSize); 


/*==============================================================================
 *
 * TDMB definitions, types and APIs.
 *
 *============================================================================*/
#define RTV_TDMB_OFDM_LOCK_MASK		0x1
#define RTV_TDMB_FEC_LOCK_MASK		0x2
#define RTV_TDMB_CHANNEL_LOCK_OK    (RTV_TDMB_OFDM_LOCK_MASK|RTV_TDMB_FEC_LOCK_MASK)

#define RTV_TDMB_BER_DIVIDER		100000
#define RTV_TDMB_CER_DIVIDER		10000
#define RTV_TDMB_CNR_DIVIDER		1000
#define RTV_TDMB_RSSI_DIVIDER		10

typedef enum
{
  RTV_TDMB_SERVICE_NONE = 0,
  RTV_TDMB_SERVICE_AUDIO,
  RTV_TDMB_SERVICE_VIDEO,
  RTV_TDMB_SERVICE_DATA  
} E_RTV_TDMB_SERVICE_TYPE;

typedef struct
{
	int tii_combo;
	int tii_pattern;
	int tii_tower;
	int tii_strength;
} RTV_TDMB_TII_INFO;


#if defined(RTV_IF_SPI) || defined(RTV_IF_EBI2)  
	#define RTV_CIF_HEADER_SIZE    4 /* bytes */ 
#else            
	#define RTV_CIF_HEADER_SIZE    16 /* bytes */ 
#endif 


typedef struct
{
#if defined(RTV_IF_MPEG2_SERIAL_TSIF) || defined(RTV_IF_SPI_SLAVE) || defined(RTV_IF_QUALCOMM_TSIF)
	UINT fic_size; /* Result size. */
	U8   *fic_buf_ptr; /* Destination buffer address. */
#endif	

	UINT msc_size[RTV_MAX_NUM_SUB_CHANNEL_USED];  /* Result size. */
	UINT msc_subch_id[RTV_MAX_NUM_SUB_CHANNEL_USED]; /* Result sub channel ID. */	
	U8   *msc_buf_ptr[RTV_MAX_NUM_SUB_CHANNEL_USED]; /* Destination buffer address. */
} RTV_CIF_DEC_INFO;
void rtvCIFDEC_Decode(RTV_CIF_DEC_INFO *ptDecInfo, const U8 *pbTsBuf, UINT nTsLen); 

void rtvTDMB_SetPlayState(BOOL fPlayState);
void rtvTDMB_StandbyMode(int on);
UINT rtvTDMB_GetLockStatus(void);
U32  rtvTDMB_GetPER(void);
S32  rtvTDMB_GetRSSI(void);
U32  rtvTDMB_GetCNR(void);
U32  rtvTDMB_GetCER(void);
U32  rtvTDMB_GetBER(void);
U32  rtvTDMB_GetPreviousFrequency(void);
void rtvTDMB_DisableStreamOut(void);
INT  rtvTDMB_OpenSubChannel(U32 dwChFreqKHz, UINT nSubChID, E_RTV_TDMB_SERVICE_TYPE eServiceType, UINT nThresholdSize);
INT  rtvTDMB_CloseSubChannel(UINT nSubChID);
INT  rtvTDMB_ScanFrequency(U32 dwChFreqKHz);
UINT rtvTDMB_ReadFIC(U8 *pbBuf);
void rtvTDMB_CloseFIC(void);
void rtvTDMB_OpenFIC(void);
INT  rtvTDMB_Initialize(E_RTV_COUNTRY_BAND_TYPE eRtvCountryBandType); 
void mtv350_isr_handler(void);


/*==============================================================================
 *
 * DAB definitions, types and APIs.
 *
 *============================================================================*/
#define RTV_DAB_OFDM_LOCK_MASK		0x1
#define RTV_DAB_FEC_LOCK_MASK		0x2
#define RTV_DAB_CHANNEL_LOCK_OK    (RTV_TDMB_OFDM_LOCK_MASK|RTV_TDMB_FEC_LOCK_MASK)

#define RTV_DAB_BER_DIVIDER		100000
#define RTV_DAB_CER_DIVIDER		10000
#define RTV_DAB_CNR_DIVIDER		1000
#define RTV_DAB_RSSI_DIVIDER		10

typedef enum
{
	RTV_DAB_SERVICE_VIDEO = 0,
	RTV_DAB_SERVICE_AUDIO,
	RTV_DAB_SERVICE_DATA	
} E_RTV_DAB_SERVICE_TYPE;

typedef struct
{
	int tii_combo;
	int tii_pattern;
	int tii_tower;
	int tii_strength;
} RTV_DAB_TII_INFO;


typedef enum
{
	RTV_DAB_TRANSMISSION_MODE1 = 0,
	RTV_DAB_TRANSMISSION_MODE2,
	RTV_DAB_TRANSMISSION_MODE3,	
	RTV_DAB_TRANSMISSION_MODE4,
	RTV_DAB_INVALID_TRANSMISSION_MODE
} E_RTV_DAB_TRANSMISSION_MODE;


void rtvDAB_StandbyMode(int on);
UINT rtvDAB_GetLockStatus(void);
U32  rtvDAB_GetPER(void);
S32  rtvDAB_GetRSSI(void);
U32  rtvDAB_GetCNR(void);
U32  rtvDAB_GetCER(void);
U32  rtvDAB_GetBER(void);
U32  rtvDAB_GetPreviousFrequency(void);
void rtvDAB_DisableStreamOut(void);
INT  rtvDAB_OpenSubChannel(U32 dwChFreqKHz, UINT nSubChID, E_RTV_DAB_SERVICE_TYPE eServiceType, UINT nThresholdSize);
INT  rtvDAB_CloseSubChannel(UINT nSubChID);
INT  rtvDAB_ScanFrequency(U32 dwChFreqKHz);
UINT rtvDAB_ReadFIC(U8 *pbBuf, UINT nFicSize);
UINT rtvDAB_GetFicSize(void);
void rtvDAB_CloseFIC(void);
void rtvDAB_OpenFIC(void);
INT  rtvDAB_Initialize(void); 

 
#ifdef __cplusplus 
} 
#endif 

#endif /* __RAONTV_H__ */

