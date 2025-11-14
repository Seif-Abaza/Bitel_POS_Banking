/******************************************************************************/
/* COMPANY  :  Bitel EMEA Co.                                                 */
/*                                                                            */
/* PROJECT  :                                                                 */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/* !Component       :                                                         */
/* !Description     :                                                         */
/*                                                                            */
/* !Module          :                                                         */
/* !Description     :                                                         */
/*                                                                            */
/* !File            :                                                         */
/*                                                                            */
/* !Target          :                                                         */
/*                                                                            */
/* Coding language  : C                                                       */
/*                                                                            */
/* !COPYRIGHT 2013 Bitel EMEA Co.                                             */
/* All rights reserved                                                        */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/* EVOLUTIONS                                                                 */
/******************************************************************************/
/* $Log: $
*******************************************************************************/

/******************************************************************************/
/* INCLUDE FILES                                                              */
/******************************************************************************/
#include "AppGlobal.h"
//#include "XDATA.h"

/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
#define PORTBASE 0x302B0100// 0x300F0000


#define  DATASAVEPAGE0BaseAddress 	PORTBASE
//#define  DATASAVEPAGE1BaseAddress 	0x500A0000
//#define  DATASAVEPAGE3BaseAddress 	0x500B0000
//#define  DATASAVEPAGE2BaseAddress 	0x500B0800
//#define  TMSBaseAddress 	        0x50100000

#define  DATASAVEPAGE1BaseAddress 	0x500A0100
#define  DATASAVEPAGE3BaseAddress 	0x50200000
#define  DATASAVEPAGE2BaseAddress 	0x50300000
#define  TMSBaseAddress 	        0x50400000


RUNDATA   udtRunData;
strRunData   udtBrdRunData;
strMiniStatment udtMiniStatment;
strBillInqInfo udtBillInqInfo;
strNECInfo udtNECInfo ;
int iSgnlStrength;

volatile  DATASAVEPAGE0 * const DataSave0 = ( DATASAVEPAGE0 *)DATASAVEPAGE0BaseAddress;
volatile  DATASAVEPAGE1 * const DataSave1 = ( DATASAVEPAGE1 *)DATASAVEPAGE1BaseAddress;
volatile  DATASAVEPAGE2 * const DataSave2 = ( DATASAVEPAGE2 *)DATASAVEPAGE2BaseAddress;
volatile  DATASAVEPAGE3 * const DataSave3 = ( DATASAVEPAGE3 *)DATASAVEPAGE3BaseAddress;


/******************************************************************************/
/* PRIVATE VARIABLES                                                          */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                               */
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/******************************************************************************/
/* EXPORTED FUNCTIONS                                                         */
/******************************************************************************/




