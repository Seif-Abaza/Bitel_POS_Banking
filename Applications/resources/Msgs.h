/******************************************************************************/
/* COMPANY  : Bitel EMEA Co.                                                  */
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
#ifndef MSGS_H
  #define MSGS_H

/*****************************************************************************/
/* INCLUDE FILES                                                             */
/*****************************************************************************/
#include "../../../BF_SDK/include/BTEMEA_LIB/message.h"

/*****************************************************************************/
/* CONSTANTS, MACROS                                                         */
/*****************************************************************************/



/*****************************************************************************/
/* TYPES                                                                     */
/*****************************************************************************/
typedef enum 
{

  //Bread Msgs Start
  S_INSP_DISP    = 0       ,
  S_FAMLY_NO            ,
  S_PART_DISP           ,
  S_BRD_NO              ,
  S_INCODE              ,
  S_QUOTA              ,
  S_DISP_SUCSS          ,
  S_DISP_ERR            ,
  //Bread Msgs End
  S_BAKERY_MENU_HDR    ,
  S_LOGIN    ,
  S_SALES    ,
  S_SETTLMNT    ,
  S_LOGOUT    ,
  S_PSW_CHNG,

  S_LOGIN_SUCC,
  S_LOGOUT_SUCC,
  S_LOGIN_FIRST,


  S_MAIN_MENU_HDR    ,
  S_PURCH               ,
 // S_BL_PAY              ,		//heba
//  S_TP_PAY              ,
//  S_ELC                 ,
//  S_CARD_TRANS          ,
//  S_ACNT_TRANS          ,
 // S_MNY_DEPOS           ,
//  S_MNY_TRANS           ,
//  S_MNY_RCV             ,
//  S_BLNC_INQ            ,
//  S_PN_CHNG             ,
  S_VOID                ,
//  S_PURCH_CASH_BK       ,
//  S_MIN_STATE           ,
  S_RFND                ,
  S_BTCH                ,
  S_SERV                ,

  S_RVRSL                ,
  S_ATTMP_END,

  S_REPRNT               ,
  S_RPRT                ,
  S_LANG                ,
  S_EK_TST              ,
  S_SETUP               ,

  S_RPRNT_LAST          ,
  S_RPRNT_BY_NUM        ,
  S_RPRNT_FAILD         ,
  S_RPRNT_NOT_EXST      ,

  S_SUM_RPRT            ,
  S_DTLD_RPRT           ,
  S_CONFIG_RPRT         ,
  S_NO_TRANS            ,

  S_ENT_ADMIN_PAS       ,
  S_ENT_SERIAL_NO       ,

  S_DT_TM               ,
  S_PS_NO               ,
  S_NETWRK_SETNGS       ,
  S_CHNG_KEYS           ,
  S_SETL_TIME           ,
 S_CHNG_PSWS           ,
// S_DIST_SET              ,
 S_BTCH_NO              ,
 S_EMPTY_BATCH         ,
  S_RST                 ,
  S_VERSION             ,

  S_VEND_INSTIT_CODE    ,
  S_SWIP_ENT_PIN,
  S_MAN_ENT         ,
  S_CLR_RVRSL           ,


  S_ENT_PAS           ,
  S_WRNG_PAS,
  S_WRNG_SERL,

  S_GP_PRM              ,
  S_BNK_IP              ,

  S_CHNG_TMK            ,
  S_CHNG_TRK            ,

  S_MERCH_PSW           ,
  S_ADMIN_PSW           ,

  S_DATE_FRMT           ,
  S_TIME_FRMT           ,
  S_TIME_HHMM_FRMT      ,
  S_INVLD_TIME          ,

  S_ENG                 ,
  S_ARA                 ,

  S_POS_PARAM_HDR       ,
  S_BANK_NAME           ,
  S_POS_ID              ,
  S_POS_NAME            ,
  S_MRECH_ID            ,
  S_MERCH_NAME          ,
  S_MERCH_ADDR          ,

  S_GPRS_PRM_HDR        ,
  S_SIM1_APN            ,
  S_SIM1_USER_NAME      ,
  S_SIM1_PSW            ,
  S_SIM2_APN            ,
  S_SIM2_USER_NAME      ,
  S_SIM2_PSW            ,

  S_VEND_CODE,
  S_INSTIT_CODE,

  S_BNK_PORT            ,
  S_BNK_URL            ,
  S_HOST_TIMOUT            ,
  S_TIMOUT_SEC          ,

  S_RST_Q               ,
  S_PIN_CAPA            ,
  S_ACTIV_Q             ,
  S_RETRY_Q             ,
  S_PLZ_WAIT        ,
  S_RESPO_CODE,
  S_BTCH_ERR,

  S_ADD_NEW             ,
  S_EDIT                ,
  S_DEL                 ,
       
  S_ZAIN                ,
  S_MTN                 ,
  S_SDN                 ,
  S_CNR                 ,
  S_NEC                 ,

  S_PURCHASE_HDR        ,
  //S_BL_PAY_HDR          ,		//heba
  //S_TP_UP_HDR           ,
 // S_ELC_NEC_HDR         ,
 // S_CRD_TRANS_HDR       ,
 // S_ACNT_TRANS_HDR      ,
 // S_MNY_DEPOS_HDR       ,
 // S_MNY_TRANS_HDR       ,
 // S_MNY_RCV_HDR         ,
 // S_BLNC_INQ_HDR        ,
 // S_PN_CHNG_HDR         ,
  S_VOID_HDR            ,
 // S_PURCH_CASH_BK_HDR   ,
 // S_MNI_STAT_HDR        ,
  S_REF_HDR             ,
  S_BATCH_HDR           ,
  S_OTHR_SERV_HDR         ,

  //S_PURCHASE_HDR        ,
  S_ENT_AMNT            ,
  S_SWP_CRD             ,
  S_ENT_CRD_PIN         ,
  S_EXP_DATE,
  S_CVC2,				//Heba
  S_AUTH,				//Heba
  S_INST_CRD,			//Heba
  S_READ_CRD,			//Heba
  S_REMOVE_CRD,			//Heba
  S_BATCH_MISMATCH,		//Heba
  S_BATCH_MATCH,		//Heba

  //S_BL_PAY_HDR          ,
  S_ENT_FN_NO           ,
  S_RENT_FN_NO          ,
  S_BL_INQ_HDR          ,
  S_MOB_NO              ,
  S_CNT_NO              ,
  S_BL_AMNT             ,
  S_UN_BL_AMNT          ,
  S_INVC_DT             ,
  S_PY_NOW_Q            ,

  //S_TP_UP_HDR           ,
  S_TP_UP_VAL_Q         ,
  S_5_SDG               ,
  S_10_SDG              ,
  S_25_SDG              ,
  S_50_SDG              ,
  S_OTH_AMNT            ,

  //S_ELC_NEC_HDR         ,
  S_ENT_MTR_NO          ,
  S_RENT_MTR_NO         ,
  S_PURCH_VAL_Q         ,

  //S_CRD_TRANS_HDR       ,
  S_ENT_CRD_NO          ,
  S_RENT_CRD_NO         ,

  //S_ACNT_TRANS_HDR      ,
  S_ENT_ACNT_NO         ,
  S_RENT_ACNT_NO        ,

  //S_MNY_DEPOS_HDR       ,
  S_ENT_MERCH_PAS       ,

  //S_MNY_TRANS_HDR       ,

  //S_MNY_RCV_HDR         ,
  S_ENT_APRV_CD         ,

  //S_BLNC_INQ_HDR        ,
  S_PRNT_RCT            ,
  S_YS                  ,
  S_NO                  ,

  //S_PN_CHNG_HDR         ,
  S_ENT_OLD_PN          ,
  S_ENT_NW_PN           ,
  S_RENT_NW_PN          ,

  //S_VOID_HDR            ,
  S_ENT_RCT_NO          ,
  S_VOID_AMNT_Q         ,
  S_VOID_SALE_Q         ,

  //S_PURCH_CASH_BK_HDR   ,
  S_ENT_PURCH_AMNT      ,
  S_ENT_CASH_BK_AMNT    ,

  //S_BATCH_HDR           ,
  S_MEM_FULL,
  S_BTCH_NOT_EMPTY          ,
  S_PEND_BATCH          ,
  S_AUTO_BATCH          ,
  S_BATCH_UPLD_HDR      ,

  //S_MNI_STAT_HDR        ,

  //S_REF_HDR             ,

  S_NUM_DNT_MTCH        ,

  S_SLCT_SIM1           ,
  S_SLCT_SIM2           ,
  S_NO_SIM              ,
  S_GPRS_INIT           ,
  S_GPRS_ERROR          ,
  S_MDM_NO_RSPO_ERR     ,
  S_MDM_ERR             ,
  S_MDM_CM_ERR          ,
  S_MDM_NO_CONN_ERR     ,

  S_TRY_CNCT                  ,
  S_CNCT                  ,
  S_NOT_CNCT              ,
  S_REQ                 ,
  S_RESP                ,
  S_DISCON                ,

  S_RCT_PREP,
  S_PRNT,
  S_DBG                 ,

  INS_PAPR,

  EMPTY_MSG             ,

  R_PURCHASE_HDR        ,
//  R_BL_PAY_HDR          ,
//  R_TP_UP_HDR           ,
//  R_S_ELC_NEC_HDR       ,
//  R_CRD_TRANS_HDR       ,
//  R_ACNT_TRANS_HDR      ,
//  R_MNY_DEPOS_HDR       ,
//  R_MNY_TRANS_HDR       ,
//  R_MNY_RCV_HDR         ,
//  R_BLNC_INQ_HDR        ,
//  R_PN_CHNG_HDR         ,
  R_VOID_HDR            ,
  R_REF_HDR             ,
//  R_PURCH_CASH_BK_HDR   ,
  R_BATCH_HDR           ,
  R_MNI_STAT_HDR        ,

  R_OTHR_SERV_HDR       ,

  R_DATE                ,
  R_TIME                ,
  R_MRCH_ID             ,
  R_TRM_ID              ,
  R_CRD_NO              ,
  R_CRD_HLD_NAME          ,
  R_EXP_DATE            ,

  //R_PURCH_HDR           ,
  R_APRV_HDR            ,
  R_FLD_TRANS_HDR       ,
  R_TRANS_ID            ,
  R_RCT_NO              ,
  R_AMNT                ,
  R_REF_NO              ,
  R_ATH_NO              ,
  R_CUST_CPY_HDR       ,
  R_MERCH_CPY_HDR       ,

  //R_BL_PAY_HDR          ,
  R_MOB_NO              ,
  R_CONT_NO             ,
  R_BLD_AMNT            ,
  R_PAD_AMNT            ,

  //R_TOP_UP_PAY_HDR      ,
  R_VCH_AMNT            ,
  R_CUST_SIG            ,
  R_RES_CD              ,
  R_FLD_MSG             ,

  //R_ELC_PAY_HDR         ,
  R_RNN_NO              ,
  R_UNT                 ,
  R_KWH                 ,
  R_SL_AMNT             ,
  R_FEES                ,
  R_SDG                 ,
  R_CUST_NM             ,
  R_ACNT_NO             ,
  R_MTR_NO              ,
  R_REF                 ,
  R_OPR_MSG             ,
  
  //R_CRD_TRNS_HDR          ,
  //R_ACC_TRNS_HDR          ,
  //R_MNY_DEPOS_HDR         ,
  //R_MNY_TRANS_HDR         ,
  R_APPRVL_CODE           ,
  //R_MNY_RCV_HDR           ,
  //R_BAL_INQ_HDR           ,
  R_CURR_BAL              ,
  //R_CHNG_PIN_HDR          ,
  //R_VOID_HDR              ,
  //R_PURCH_CASH_BK_HDR     ,
  R_TOT_AMNT              ,
  R_CSH_BCK_AMNT          ,

  //R_BATCH_HDR             ,
  R_BATCH_HDR1            ,

  //R_MINI_STAT_HDR         ,
  R_MINI_STAT_HDR1        ,
  //R_REFUND_HDR            ,
  R_CONFIG_RPRT_HDR       ,
  R_APP_VER               ,
  R_APN1                   ,
  R_APN2                   ,
  R_TERM_SERIAL_NO        ,

  R_BTCH_NO               ,
  R_DTLD_RPRT_HDR         ,
  R_DTLD_RPRT_HDR1        ,
  R_DTLD_RPRT_HDR2        ,
  R_DTLD_RPRT_HDR3        ,
  R_SUMM_RPRT_HDR         ,
  R_SUMM_RPRT_HDR1        ,
  R_SUMM_RPRT_HDR2        ,
  R_TOTAL                 ,
  R_REPRINT_CPY           ,
  R_PRINT_CPY_Q			 ,
  R_AID				,

  MAX_MSGS_NO           ,

}APP_MSGS;


/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/
extern char* GetMsgs(unsigned int index);
extern char* GetLangMsgs(unsigned int index, APP_LANG udtSelectedLang);

#endif /*MSGS_H*/





