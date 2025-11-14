/*---------------------------------------------------------------------------+
| Author:    RSBON
| File:      EMV_CAT.H
| Purpose:   EMV_CAT (Card Holder Activated Terminal) header file
| Changes:
|
| Date:      Author  Comment:
| ---------  ------  --------
| 26-Feb-02  RSBON   Created.
+---------------------------------------------------------------------------*/

#ifndef _EMV_CAT_H_
#define _EMV_CAT_H_

#include "EMV_DEF.H"
#include "EMV_TAG.H"

int app_pgsTrmRsaKeyMake(U08 *RID, U08 index);
int retrive_CAPK(void);
int retrive_IssuerPK(void);
int retrive_ICCardPK(void);
int retrive_ICCpinEncPK(void);
int app_checkYYMMDD(U08 *cn6YMD);
int app_compareYYMMDD(U08 *cn6YMD);
int app_compareMMYY(U08 *n4MMYY);
int app_comparePKE(U08 *pPKE, U08 eLen);
int app_comparePAN(U08 *cPAN, U08 *aPAN, U08 cLen);
int app_compareCSN(U08 *pCSN);

void SetTrmARC(U16 Y1, U16 Z1, U16 Y2, U16 Z2, U16 Y3, U16 Z3);
void SetTrmCAPK(U08 *pCAPK);
void SetTrmRevocList(U08 *pTrmRevocList);
void SetTrmExcepList(U08 *pTrmExcepList);

extern U08 *pgbTrmRevocList, *pgbTrmExcepList;

//Authorisation Response Code
//T2a_ARC=0x8A
extern U16 TrmARC_Y1; // Offline approved
extern U16 TrmARC_Z1; // Offline declined
extern U16 TrmARC_Y2; // Approval (after card-initiated referral)
extern U16 TrmARC_Z2; // Decline (after card-initiated referral)
extern U16 TrmARC_Y3; // Unable to go online, offline approved
extern U16 TrmARC_Z3; // Unable to go online, offline declined

//Transaction Category Codes(EUROPAY only)
//T1a_TrmTCC=0x9F53
extern const U08 TrmTCC_C; // Cash Disbursement
extern const U08 TrmTCC_Z; // ATM Cash Disbursement
extern const U08 TrmTCC_O; // College/School Expanse Hospitalization
extern const U08 TrmTCC_H; // Hotel/Motel and Cruise Ship Services
extern const U08 TrmTCC_X; // Transportation
extern const U08 TrmTCC_A; // Automobile/Vehicle Rental
extern const U08 TrmTCC_F; // Restaurant
extern const U08 TrmTCC_T; // Mail/Teliphone Order Preauthorized Order
extern const U08 TrmTCC_U; // Unique Transation
extern const U08 TrmTCC_R; // Retail/All Other Transactions
//Merchant Category Codes
//T2n_TrmMCC=0x9F15
//see ISO+8583:1993 A.4 Card Acceptor Business Code.

#endif
