/*
 * Disp.h
 *
 *  Created on: Sep 9, 2014
 *      Author: Mahmoud Swedan
 */

#ifndef DISP_H_
#define DISP_H_


extern char gbLangCode;
#define LsDialingNow        21
#define LsLineConnected     22
#define LsDataReceiving     23
#define LsDataSending       24
#define LsNoDialTone        25
#define LsLineBusy          26
#define LsNoCarrier         27
#define LsLineError         28
#define LsTimeoutS7         29

#define LsEnterPAN          44
#define LsEnterYYMM         45
#define LsSwipeCard         46
#define LsInsertCard        47
#define LsAmount            48
#define LsEnterPIN          49
#define LsTrxnResult        50
#define LsEMVuseMSR         51
#define LsEMVuseICC         52
#define LsEMVselect         53
#define LsEMVreading        54
#define LsEMVreferral       55
#define LsEMVOn_Line        56
#define LsEMVOffLine        57
#define LsPINtryLast        58
#define LsPINincorrect      59
#define LsPIN_OK            60
#define LsPINchange         61
#define LsPINconfirm        62
#define LsISSreferral       63
#define LsLCDcontrast       64
#define LsTrxnF5INQ         65
//#define LsTrxnF5INQ_1       66
#define LsRetrievalRefNr    67
#define LsTrxnF6SET         68
#define LsSectorErase       69
#define LsEtherNET          70 // upto ..76
#define LsMID_TID           77
#define LsRetryTrxn         78

#endif /* DISP_H_ */
