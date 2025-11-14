/*---------------------------------------------------------------------------+
| Author:    RSBON
| File:      EMV_TAG.H
| Purpose:   EMV_TAG header file
| Changes:
|
| Date:      Author  Comment:
| ---------  ------  --------
| 26-Feb-02  RSBON   Created.
+---------------------------------------------------------------------------*/

#ifndef _EMV_TAG_H_
#define _EMV_TAG_H_

#define Ttt_PSE                         (int)0x70
#define Ttt_App                         (int)0x61
#define Ttt_DirDiscretionary            (int)0x73
#define Ttt_IssuerScript1               (int)0x71
#define Ttt_IssuerScript2               (int)0x72
#define Ttt_FCI                         (int)0x6F
#define Ttt_FCIProprietary              (int)0xA5
#define Ttt_ResponseMsgFormat2          (int)0x77
#define Ttt_FCIIssuerDiscretionaryData  (int)0xBF0C

#define Tvv_AID                         (int)0x4F
#define Tva_AppLabel                    (int)0x50
#define T2b_FID                         (int)0x51   /*K-CASH only?*/
#define Tvb_Track2EquivalentData        (int)0x57
#define Tvc_PAN                         (int)0x5A
#define Tvb_ResponseMsgFormat1          (int)0x80
#define T4b_TrmAmountAuthorised         (int)0x81
#define T2b_AIP                         (int)0x82
#define Tvv_DFname                      (int)0x84
#define Tvb_TrmCommand                  (int)0x83
#define Tvb_IssuerScriptCommand         (int)0x86
#define T1b_AppPriorityIndicator        (int)0x87
#define T1b_SFI                         (int)0x88
#define T6a_AuxnCode                    (int)0x89
#define T2a_ARC                         (int)0x8A
#define Tvb_CDOL1                       (int)0x8C
#define Tvb_CDOL2                       (int)0x8D
#define Tvb_CVMList                     (int)0x8E
#define T1b_CAPKI                       (int)0x8F
#define Tvb_IssuerPKC                   (int)0x90
#define Tvb_IAD                         (int)0x91
#define Tvb_IssuerPKR                   (int)0x92
#define Tvb_SignedSAD                   (int)0x93
#define Tvb_AFL                         (int)0x94
#define T5b_TrmTVR                      (int)0x95
#define Tvb_TDOL                        (int)0x97
#define Tvb_TrmTC_HashValue             (int)0x98
#define Tvb_TrmTrxnPINdata              (int)0x99
#define T3n_TrmTrxnDate                 (int)0x9A
#define T2b_TrmTSI                      (int)0x9B
#define T1n_TrmTrxnType                 (int)0x9C
#define Tvb_DDFname                     (int)0x9D
#define Tvv_C1_MONETA                   (int)0xC1   /*MONETA only?*/
#define Tvv_C2_MONETA                   (int)0xC2   /*MONETA only?*/
#define Tva_CardholderName              (int)0x5F20
#define T3n_AppExpirationDate           (int)0x5F24
#define T3n_AppEffectiveDate            (int)0x5F25
#define T2n_IssuerCountryCode           (int)0x5F28
#define T2n_TrmTrxnCurrencyCode         (int)0x5F2A
#define Tva_LanguagePreference          (int)0x5F2D
#define T2n_ServiceCode                 (int)0x5F30
#define T1n_PAN_SeqNo                   (int)0x5F34
#define T1n_TrmTrxnCurrencyExp          (int)0x5F36
#define Tva_IssuerURL                   (int)0x5F50

#define T6n_TrmAcquirerID               (int)0x9F01
#define T6n_TrmAmountAuthorised         (int)0x9F02
#define T6n_TrmAmountOther              (int)0x9F03
#define T4b_TrmAmountOther              (int)0x9F04
#define Tvb_AppDiscretionaryData        (int)0x9F05
#define Tvb_TrmAID                      (int)0x9F06
#define T2b_AUC                         (int)0x9F07
#define T2b_AppVersionNo                (int)0x9F08
#define T2b_TrmAppVersionNo             (int)0x9F09
#define Tva_CardholderNameExt           (int)0x9F0B
#define T5b_IACdefault                  (int)0x9F0D
#define T5b_IAC_Denial                  (int)0x9F0E
#define T5b_IAC_Online                  (int)0x9F0F
#define Tvb_IssuerAppData               (int)0x9F10
#define T1n_IssuerCodeTableIndex        (int)0x9F11
#define Tva_AppPreferredName            (int)0x9F12
#define T2b_LastOnLineATCR              (int)0x9F13
#define T1b_LCOLimit                    (int)0x9F14
#define T2n_TrmMCC                      (int)0x9F15
#define Tva_TrmMID                      (int)0x9F16
#define T1b_PinTryCounter               (int)0x9F17
#define T4b_IssuerScriptID              (int)0x9F18
#define T2n_TrmCountryCode              (int)0x9F1A
#define T4b_TrmFloorLimit               (int)0x9F1B
#define T8a_TrmIdentification           (int)0x9F1C
#define Tvb_TrmRiskManagementData       (int)0x9F1D
#define T8a_TrmIFD_SerialNo             (int)0x9F1E
#define Tva_Track1_DiscretionaryData    (int)0x9F1F
#define Tvc_Track2_DiscretionaryData    (int)0x9F20
#define T3n_TrmTrxnTime                 (int)0x9F21
#define T1b_TrmCAPKI                    (int)0x9F22
#define T1b_UCOLimit                    (int)0x9F23
#define T8b_AppCrytogram                (int)0x9F26
#define T1b_CryptogramInfoData          (int)0x9F27
#define Tvb_ICCPIN_EPKC                 (int)0x9F2D
#define Tvb_ICCPIN_EPKE                 (int)0x9F2E
#define Tvb_ICCPIN_EPKR                 (int)0x9F2F
#define Tvb_IssuerPKE                   (int)0x9F32
#define T3b_TrmCapa                     (int)0x9F33
#define T3b_TrmCVMR                     (int)0x9F34
#define T1n_TrmType                     (int)0x9F35
#define T2b_ATC                         (int)0x9F36
#define T4b_TrmUnpredictableNo          (int)0x9F37
#define Tvb_PDOL                        (int)0x9F38
#define T1n_TrmPOSEntryMode             (int)0x9F39
#define T4b_TrmAmountRefCurrency        (int)0x9F3A
#define T2n_AppRefCurrency              (int)0x9F3B
#define T2n_TrmTrxnRefCurrencyCode      (int)0x9F3C
#define T1n_TrmTrxnRefCurrencyExp       (int)0x9F3D
#define T5b_TrmCapaAdditional           (int)0x9F40
#define Tvn_TrmTrxnSequenceCounter      (int)0x9F41
#define T2n_AppCurrencyCode             (int)0x9F42
#define T1n_AppRefCurrencyExp           (int)0x9F43
#define T1n_AppCurrencyExp              (int)0x9F44
#define T2b_DAC                         (int)0x9F45
#define Tvb_ICC_PKC                     (int)0x9F46
#define Tvb_ICC_PKE                     (int)0x9F47
#define Tvb_ICC_PKR                     (int)0x9F48
#define Tvb_DDOL                        (int)0x9F49
#define Tvb_SDA_TagList                 (int)0x9F4A
#define Tvb_SignedDAD                   (int)0x9F4B
#define Tvb_ICCDynamicNo                (int)0x9F4C
#define T2n_AppCurrencyCodeVISA         (int)0x9F51 /*VISA only*/
#define T2b_AppDefaultActionVISA        (int)0x9F52 /*VISA only*/
#define T1a_TrmTCC                      (int)0x9F53 /*EUROPAY only(TCC)*/
#define T1b_ConsecutiveTLimitIntVISA    (int)0x9F53 /*VISA only*/
#define T6n_CumulativeTTALimitVISA      (int)0x9F54 /*VISA only*/
#define T1b_GeographicIndicatorVISA     (int)0x9F55 /*VISA only*/
#define T1b_IssuerAuxnIndicatorVISA     (int)0x9F56 /*VISA only*/
#define T2n_IssuerCountryCodeVISA       (int)0x9F57 /*VISA only*/
#define T1b_LCOLimitVISA                (int)0x9F58 /*VISA only*/
#define T1b_UCOLimitVISA                (int)0x9F59 /*VISA only*/
#define Tva_IssuerURL2VISA              (int)0x9F5A /*VISA only*/
#define Tvb_TrmISR                      (int)0x9F5B
#define T6n_CumulativeTTAULimitVISA     (int)0x9F5C /*VISA only*/
#define T1b_ConsecutiveTLimitVISA       (int)0x9F72 /*VISA only*/
#define T4n_CurrencyConversionFactor    (int)0x9F73
#define T6a_VLP_IssuerAuxnCodeVISA      (int)0x9F74 /*VISA only*/
#define T6n_CumulativeTTALimitDCVISA    (int)0x9F75 /*VISA only*/
#define T2n_SecondaryAppCurrencyCode    (int)0x9F76
#define T6n_VLP_FundsLimitVISA          (int)0x9F77 /*VISA only*/
#define T6n_VLP_SingleTrxnLimitVISA     (int)0x9F78 /*VISA only*/
#define T6n_VLP_AvailableFunds          (int)0x9F79
#define T1n_TrmSupportIndicator         (int)0x9F7A
#define T6n_TrmVLP_TrmTrxnLimit         (int)0x9F7B /*Visa Low-Value Payment(VLP)*/
#define Tvb_CPLC_HistoryFidVISA         (int)0x9F7F /*VISA only*/
#define Tvb_TrmScript                   (int)0x17   /*Bitel Defined TrmScript*/
#define Tvb_TrmCAPK                     (int)0x18   /*Bitel Defined TrmCAPK  */
#define Tvb_TrmPDOL_data                (int)0x19   /*Bitel Defined PDOL_data*/
#define Tvb_TrmCDOL1data                (int)0x1A   /*Bitel Defined CDOL1data*/
#define Tvb_TrmCDOL2data                (int)0x1B   /*Bitel Defined CDOL2data*/
#define Tag_EOT4                        (int)0x04   /*Bitel Defined DataEOT  */

#endif
