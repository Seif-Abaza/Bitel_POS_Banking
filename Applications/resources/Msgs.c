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
#include "Msgs.h"
/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/
typedef struct
{
  const unsigned char ErrCode;
  const char ErrMsg[MAX_NUM_LANG][50];
}udtErrMsgRaw;

/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/

static const char MsgsTable[MAX_MSGS_NO][MAX_NUM_LANG][60]=
{

//Bread Msgs Start
//{"INSP Dispence\0",                             "   wvt hgltja   "},                            //S_INSP_DISP	//heba
{" \0",                             "     "},                            //S_INSP_DISP		//heba
{"Family Members\0",                            "u]] hgHtvh]"},                                 //S_FAMLY_NO
{"Part Dispence\0",                             "    wvt [.zd    "},                                                       //DATE_FRMT
{"Bread NO\0",                                  "u]] hghvytm :"},                                                       //TIME_FRMT
{"Inside Code\0",                               "hgvrl hg]hogd"},                                                       //TIME_FRMT
{"Quota\0",                                     "hgpwm"},                                                       //TIME_FRMT

{"Dispense Done\0",                             "jl hgwvt fk[hp"},                                                       //TIME_FRMT
{"Dispense Error\0",                             "gl djl hgwvt"},                                                       //TIME_FRMT

//Bread Msgs End
{"***MAIN MENU***\0",                      "hgrhzlm hgvzdsdm\0"},
{"Log In\0",                                "]o,g ggk/hl\0"},
{"Sales\0",                                  "hglfduhj\0"},
{"Settlement\0",                            "hgjs,dm\0"},
{"Log Out\0",                               "ov,[ lk hgk/hl\0"},
{"Change Password\0",                       "qf' ;glm hgsv\0"},

{"Login Successful\0",                       "jl hg]o,g fk[hp\0"},
{"Logout Successful\0",                       "jl hgov,[ fk[hp\0"},
{"Please Login\0",                            "fv[hx hg]o,g ggk/hl\0"},


{"***MAIN MENU***\0",                      "hgrhzlm hgvzdsdm\0"},                               //MAIN_MENU_HDR     
{"Sale\0",                            "hgavhx\0"},                                               //PURCH
//{"Bill Payment.\0",                        "s]h] hgthj,vm\0"},                                        //BL_PAY	heba
//{"Top-up Payment.\0",                      "Yuh]m hgapk\0"},                                          //TP_PAY
//{"Electricity (NEC).\0",                   "avhx ;ivfhx\0"},                                          //ELC
//{"Card Transfer.\0",                       "hgjp,dg gf'hrm\0"},                                       //CARD_TRANS
//{"Account Transfer.\0",                    "hgjp,dg gpshf\0"},                                        //ACNT_TRANS
//{"Money Deposit.\0",                       "jy`dm hgpshf\0"},                                         //MNY_DEPOS
//{"Money Transfer.\0",                      "Yvshg p,hgm\0"},                                          //MNY_TRANS
//{"Money Receive.\0",                       "Ysjghl p,hgm\0"},                                         //MNY_RCV
//{"Balance Inquiry\0",                     "Ysjughl uk hgvwd]\0"},                                    //BLNC_INQ
//{"PIN Change.\0",                          "jyddv hgvrl hgsvd\0"},                                    //PN_CHNG
{"Void\0",                                "Ygyhx\0"},                                                //VOID
//{"Purchase with Cash Back.\0",             "kr]dm lu hgavhx\0"},                                      //PURCH_CASH_BK
//{"Mini Statement.\0",                      ";at pshf lwyv\0"},                                        //MIN_STATE
{"Refund\0",                              "Ygyhx fu] hgjs,dm\0"},                                    //RFND
{"Batch\0",                               "Yvshg js,dm\0"},                                          //BTCH
{"Other Services\0",                      "o]lhj Hovn\0"},                                           //SERV

{"REVERSAL\0",                             ""},                                                        //RVRSL
{"Number of Attempts Ended\0",             "u]] hglph,ghj hkjin"},                                      //ATTMP_END

{"Reprint\0",                              "Yuh]m 'fu\0"},                                            //REPRNT            
{"Report\0",                               "jrvdv\0"},                                                //RPRT              
{"Language\0",                             "hggym\0"},                                                //LANG              
{"Echo Test\0",                            "hojfhv hjwhg\0"},                                         //EK_TST            
{"Setup\0",                                "Yu]h]hj\0"},                                              //SETUP             

{"Last\0",                                 "Hov luhlgm\0"},                                            //S_RPRNT_LAST
{"By Number\0",                            "fvrl hghdwhg\0"},                                         //S_RPRNT_BY_NUM
{"No RePrint for Failed Transaction\0",    "ghd,[] hdwhg gulgdm thagm\0"},                            //S_RPRNT_FAILD
{"Transaction Not Found\0",           	   "luhlgm ydv l,[,]m\0"},                                    //S_RPRNT_NOT_EXST

{"Summary\0",                                 "H[lhgd\0"},                                             //S_SUM_RPRT
{"Detailed\0",                                 "jtwdgd\0"},                                            //S_DTLD_RPRT 
{"Configuration\0",                            "jrvdv hghu]h]hj\0"},                                   //S_CONFIG_RPRT
{"No Transactions\0",                          "ghj,[] ulgdhj\0"},                                     //S_NO_TRANS 
 
{"Password:\0",                            ";glm hgsv :\0"},                                           //S_ENT_ADMIN_PAS    
{"Serial No:\0",                           "h]og hgsdcdhg :\0"},                                        //S_ENT_SERIAL_NO    
            
{"Date\Time\0",                            "jhvdo - ,rj\0"},                                          //DT_TM             
{"POS Parameters\0",                       "fdhkhj lh;dkm\0"},                                        //PS_NO             
{"Network Settings\0",                     "hu]h]hj hgaf;m"},                                         //NETWRK_SETNGS
{"Change Keys\0",                          "lthjdp hgjatdv"},                                         //CHNG_KEYS                                                                                                      
{"Settl Time\0",                           ",rj hgjs,dm\0"},                                          //SETL_TIME

{"Change Paswords\0",                       ";glhj hgsv"},                                            //CHNG_PSWS
//{"Distib Setting\0",                         "hgltja"},                                                       //S_DIST_SET
{"Batch NO\0",                                  "vrl hgfhja"},                                                       //TIME_FRMT
{"Empty Batch\0",                          "lsp hgulgdhj\0"},                                         //EMPTY_BATCH               
{"Reset POS\0",                            "qf' hglwku\0"},                                           //RST               
{"Version\0",                              "hghw]hv\0"},                                              //VERSION               

{"Vend\Instit Code\0",                     ""},                                                       //VEND_INSTIT_CODE
{"Swipe Entry Pin\0",           ""},                                                       //SWIP_ENT_PIN
{"Manual Entry \0",           ""},                                                       //MAN_ENT
{"Clear Reversal\0",                          ""},                                                    //CLR_RVRSL


{"Enter Password:\0",                      "H]og ;glm hgsv :\0"},                                     //S_ENT_PAS
{"Wrong Password:\0",                      ";glm hgsv o'H\0"},                                       //S_WRNG_PAS
{"Wrong Serial:\0",                        "hgsdvdhg o'H\0"},                                       //S_WRNG_PAS
                                                                                                      //
{"GPRS PARAM\0",                           "qf' hgaf;m\0"},                                           //GP_PRM            
{"Bank IP\0",                              ""},                                                       //BNK_IP            

{"Change TMK\0",                           ""},                                                       //CHNG_TMK
{"New Wrk Key\0",                           ""},                                                       //CHNG_TRK          
                                                                                                       //    
{"Merchant Password\0",                    ";glm hgsv hgjh[v\0"},                                      //MERCH_PSW               
{"Admin Password\0",                       ";glm hgsv hghu]h]j\0"},                                   //ADMIN_PSW               

{"YYYYMMDD\0",                             ""},                                                       //DATE_FRMT
{"HHMMSS\0",                               ""},                                                       //TIME_FRMT
{"HHMM\0",                                 ""},                                                       //TIME_HHMM_FRMT
{"Invalid Time\0",                         "h,grj o'H"},                                               //S_INVLD_TIME

{"English\0",                              "hgHk[gd.dm\0"},                                           //ENG
{"Arabic\0",                               "hguvfdm\0"},                                              //ARA

{"POS PARAM\0",                            "fdhkhj lh;dkm"},                                          //POS_PARAM_HDR
{"Bank Name\0",                            "Hsl hgfk; \0"},                                            //BANK_NAME
{"POS ID\0",                               "vrl hglh;dkm\0"},                                         //POS_ID
{"POS Name\0",                             "hsl lh;dkm\0"},                                           //POS_NAME
{"Merchant ID\0",                          "vrl hgjh[v\0"},                                           //MRECH_ID
{"Merchant Name\0",                        "hsl hgjh[v\0"},                                           //MERCH_NAME
{"Merchant address\0",                     "uk,hk hglof.\0"},                                         //MERCH_ADDR

{"GPRS PARAM\0",                           "qf' hgaf;m\0"},                                           //GPRS_PRM_HDR
{"SIM1 APN\0",                             ""},                                                       //SIM1_APN
{"SIM1 User Name\0",                       ""},                                                       //SIM1_USER_NAME
{"SIM1 Password\0",                        ""},                                                       //SIM1_PSW
{"SIM2 APN\0",                             ""},                                                       //SIM2_APN
{"SIM2 User Name\0",                       ""},                                                       //SIM2_USER_NAME
{"SIM2 Password\0",                        ""},                                                       //SIM2_PSW

{"Vendor Code\0",                          ""},                                                       //VEND_CODE
{"Institution Code\0",                     ""},                                                       //INSTIT_CODE

{"Bank Port\0",                            ""},                                                       //BNK_PORT
{"Bank URL\0",                            ""},                                                       //BNK_PORT
{"Host TimeOut\0",                            ""},                                                     //S_HOST_TIMOUT
{"TimeOut in Sec\0",                            ""},                                                   //S_TIMOUT_SEC

{"Are you sure?\0",                        "ig hkj ljH;]?\0"},                                         //RST_Q
{" Pin Capability \0",                     ""},                                                        //PIN_CAPA
{"   Activate ?   \0",                     "    jtudg ?    \0"},                                       //ACTIV_Q
{"   Retry ?   \0",                        "Huh]m hglph,gm ?\0"},                                       //RETRY_Q
{" Please Wait...\0",                      " hkj/v lk tqg; >\0"},                                         //PLZ_WAIT
{"ResponseCode\0",                      "vl. hsj[hfm\0"},                                         //RESPO_CODE
{"Batch Error\0",                         "o'H td hgjs,dm\0"} ,                                       //BTCH_ERR
                                                                                                                 
{"Add New\0",                              "Hqt []d]\0"},                                              //ADD_NEW
{"Edit\0",                                 "ju]dg\0"},                                                 //EDIT
{"delete\0",                               "p`t\0"},                                                   //DEL

{"ZAIN\0",                                ".dk\0"},                                                  //ZAIN
{"MTN\0",                                 "Yl jd Yk\0"},                                             //MTN               
{"SUDANI\0",                              "s,]hkd\0"},                                               //SDN               
{"CANAR\0",                               ";khv\0"},                                                 //CNR               
{"NEC\0",                                 ""},                                                       //NEC               

{"  * Sale *  \0",                     "   * hgavhx *   \0"},                               //PURCHASE_HDR      	//Heba
//{"* Bill Payment *\0",                     "* s]h]  thj,vm *\0"},                               //BL_PAY_HDR
//{"*Top-up Payment*\0",                     "*Yuh]m apk hgvwd]*\0"},                             //TP_UP_HDR
//{"*Electricity (NEC)*\0",                  "* avhx ;ivfhx *\0"},                                //S_ELC_NEC_HDR
//{"*Card Transfer*\0",                      "*hgjp,dg gf'hrm*\0"},                               //CRD_TRANS_HDR
//{"*Account Transfer*\0",                   "*hgjp,dg gpshf *\0"},                                //ACNT_TRANS_HDR
//{"*Money Deposit*\0",                      "* jy`dm hgf'hrm*\0"},                                //MNY_DEPOS_HDR
//{"*Money Transfer*\0",                     "* Yvshg p,hgm *\0"},                                //MNY_TRANS_HDR
//{"*Money Receive*\0",                      "* Ysjbl p,hgm *\0"},                                //MNY_RCV_HDR
//{"*Balance Inquiry*\0",                    "Ysjubl uk hgvwd]\0"},                           //BLNC_INQ_HDR
//{" * PIN Change * \0",                     "* jyddv hgvrl hgsvd *\0"},                          //PN_CHNG_HDR
{"    * Void *    \0",                     "   * Ygyhx *   \0"},                                //VOID_HDR          
//{"*Purchase with Cash Back*\0",            "kr]dm lu hgavhx\0"},                              //S_PURCH_CASH_BK_HDR

//{"*Mini Statement*\0",                     "* ;at pshf lwyv*\0"},                                //MNI_STAT_HDR                                                                                                            //
{"   * Refund *   \0",                     "* Ygyhx hgavhx *\0"},                                 //REF_HDR     
{"   * Batch *   \0",                      "* Yvshg hgjs,dm*\0"},                                //BATCH_HDR
{"*Other Services*\0",                     " * o]lhj Hovn * \0"},                                      //OTHR_SERV_HDR              

                                                                                                      //                  
{"Enter Amount:\0",                        "H]og hglfgy:\0"},                                         //ENT_AMNT        
{"Please Swipe Your Card:\0",              "hgv[hx jlvdv hgf'hrm:\0"},                                //SWP_CRD           
{"Enter Card PIN Code:\0",                 "H]og hgvrl hgsvd ggf'hrm:\0"},                            //ENT_CRD_PIN       
{"Expiry Date:\0",                         "wbpdm hgf'hrm:\0"},                                		  //S_EXP_DATE
{"CVC2\0",                         		   "\0"},													  //S_CVC2  			Heba
{"Authorisation number\0",				   "vrl hgjprr\0"	},										  //S_AUTH				Heba
{"Please Insert Card\0",				   "lk tqg; h]og hgf'hri\0"},								  //S_INST_CRD			Heba
{"Please Read Card\0",					   "lk tqg; hgf'hri\0"},									  //S_READ_CRD,			//Heba
{"Remove Card",								""},//S_REMOVE_CRD,			//Heba
{"Batch Mismatch",""},//S_BATCH_MISMATCH
{"Batch Match",""},//S_BATCH_MATCH,		//Heba


{"Enter Phone NO:\0",                      "H]og vrl hgjgdt,k\0"},                                    //ENT_FN_NO         
{"ReEnter Phone NO:\0",                    "Hu] h]ohg vrl hgjgdt,k:\0"},                              //RENT_FN_NO        
{"* Bill Inquiry *\0",                     "hgYsjubl thj,vm\0"},                                      //BL_INQ_HDR        
{"Mobile Number:\0",                       "vrl hgjgt,k:\0"},                                         //MOB_NO            
{"Contract Number:\0",                     "vrl hgjuhr]:\0"},                                         //CNT_NO            
{"Bill Amount:\0",                         "lfgy hgthj,vm\0"},                                        //BL_AMNT           
{"Un Bill Amount:\0",                      "hglfgy hgydv lt,jv:\0"},                                  //UN_BL_AMNT        
{"Invoice Date:\0",                        "jhvdo hgthj,vm :\0"},                                     //INVC_DT           
{"Do you want to pay now?\0",              "ig jvd] Hk js]] hgthj,vm hghk ?\0"},                       //PY_NOW_Q          
                                                                                                      //                  
{"Choose Top-Up Value?\0",                 "Yojv hglfgy ?\0"},                                         //TP_UP_VAL_Q       
{"5 SDG\0",                                "{5}[\0"},                                                   //5_SDG             
{"10 SDG\0",                               "{10}[\0"},                                                  //10_SDG            
{"25 SDG\0",                               "{25}[\0"},                                           //25_SDG            
{"50 SDG\0",                               "{50}[\0"},                                                  //50_SDG            
{"Other Amount\0",                         "lfgy hov\0"},                                             //OTH_AMNT          
                                                                                                      //                  
{"Enter Meter NO:\0",                      "H]og vrl hgu]h]:\0"},                                     //S_ENT_MTR_NO        
{"Re Enter Meter NO:\0",                   "Hu] Y]ohg vrl hgu]h]:\0"},                                //S_RENT_MTR_NO       
{"Please choose Sale Value?\0",        "hgv[hx Yojdhv lfgy hgavhx?\0"},                           //S_PURCH_VAL_Q
                                                                                                      //                  
{"Enter Card Number:\0",                   "H]og vrl hgf'hrm:\0"},                                    //ENT_CRD_NO        
{"Re Enter Card Number:\0",                "Hu] Y]ohg vrl hgf'hrm:\0"},                               //RENT_CRD_NO       

{"Enter Account No:\0",                    "H]og vrl hgpshf:\0"},                                     //ENT_ACNT_NO      
{"ReEnter Account No:\0",                  "Hu] Y]ohg hgpshf:\0"},                                     //RENT_ACNT_NO      

{"Merchant Password:\0",                   "hgvrl hgsvd ggjh[v:\0"},                                  //ENT_MERCH_PAS     


{"Enter Approval Code:\0",                 "H]og vrl hgp,hgm:\0"},                                    //ENT_APRV_CD       

{"Print Receipt?\0",                       "'fhum hgYdwhg?\0"},                                       //PRNT_RCT          
{"1 Yes\0",                                 "1 kul\0"},                                                  //YS                
{"2 No\0",                                  "2 gh\0"},                                                   //NO                

{"Enter Old PIN Code.\0",                  "H]og hgvrl hgsvd hgr]dl:\0"},                             //ENT_OLD_PN        
{"Enter New PIN Code:\0",                  "H]og hgvrl hgsvd hg[]d]:\0"},                             //ENT_NW_PN         
{"Re Enter New PIN Code:\0",               "Hu] Y]ohg hgvrl hgsvd hg[]d]:\0"},                        //RENT_NW_PN        
                                                                                                      //                  
{"Enter Receipt Number:\0",                "H]og vrl hgYdwhg:\0"},                                    //ENT_RCT_NO        
{"Correct Amount?\0",                      "hglfgy wpdp ?\0"},                                         //VOID_AMNT_Q       
{"Are you Sure void Sale?\0",              "ig Hkj ljH;] lk Ygyhx hgluhlgm?\0"},                      //VOID_SALE_Q       
                                                                                                      //                  
{"Enter Purchase Amount:\0",                "H]og lfgy hgavhx:\0"},                                   //S_ENT_PURCH_AMNT    
{"Enter Cash Back Amount:\0",               "H]og lfgy hgkr]dm:\0"},                                  //S_ENT_CASH_BK_AMNT  

{"Memory Full Please Send the Batch.\0",     "hg`h;vm lljgzm lk tqg; hvsg hgjs,dm\0"},               //MEM_FULL//                  
{"Access denied! Batch is not empty.\0",     "d[f hvshg hgjs,dm h,gh\0"},                           //BTCH_NOT_EMPTY//                  
{"Batch Pending, No Transactions Allowed\0",              "js,dm td hkj/hv hghvshg ydv lsl,p fulgdhj Hovn\0"},//PEND_BATCH
{"Automatic Batch Sending Started\0",        "hgf]x td hvshg hgjs,dm jgrhzdh\0"},                   //AUTO_BATCH
{"* Batch Uplaod *\0",                       ""},                                                    //BATCH_UPLD_HDR         
                                                                                                      //                  
{"Numbers Don't Matched\0",                 " hghvrhl ydv lj,htrm\0"},                               //S_NUM_DNT_MTCH
                                                                                                          
{"Selecting SIM1\0",                        "Hojdhv hgavdpm 1\0"},                                   //S_SLCT_SIM1
{"Selecting SIM2\0",                        "Hojdhv hgavdpm 2\0"},                                   //S_SLCT_SIM2
{"NO SIM\0",                                "gh j,[] avdpm\0"},                                      //S_NO_SIM
{"GPRS INITIALIZATION\0",                   "jkad' o]lm hgfdhkhj\0"},                    //S_GPRS_INIT
{"GPRS ERROR\0",                            "o'H td o]lm hgfdhkhj\0"},                               //S_GPRS_ERROR
{"MODEM NO RESP\0",                         ""},                                                     //S_MDM_NO_RSPO_ERR
{"MODEM ERROR\0",                           ""},                                                     //S_MDM_ERR         
{"MODEM CM ERROR\0",                        ""},                                                     //S_MDM_CM_ERR      
{"GPRS No Connection\0",                    ""},                                                     //S_MDM_NO_CONN_ERR 
                                                                                                     //
{"Connect to server\0",                     "hghjwhg fhgoh]l\0"},                                    //S_TRY_CNCT
{"Connected\0",                             "jl hgj,wdg fhgoh]l\0"},                                 //S_CNCT
{"Not Connected\0",                         "tag hgj,wdg fhgoh]l\0"},                                //S_NOT_CNCT
{"REQUEST\0",                               "hvshg\0"},                                               //S_REQ
{"RESPONSE\0",                              "hsjrfhg\0"},                                             //S_RESP
{"Disconnect\0",                             "Ykihx hgYjwhg\0"},                                             //S_RESP
                                                                                                      //
{"Preparing Receipt\0",                     "j;,dk hghdwhg"},                                                   //S_RCT_PREP
{"Printing...\0",                           "[hvd hg'fhum >>>"},                                                   //S_PRNT
{"DEBUG\0",                                 ""},                                                     //S_DBG
{"Insert Paper\0",                         "h]og ,vr\0"},                                            //INS_PAPR
                                                                                                               
{"\0",              "\0"},

{"*** Sale ***\0",                     "**** hgavhx ****\0"},                               //PURCHASE_HDR
//{"**BILL PAYMENT**\0",                     "* s]h]  thj,vm *\0"},                               //BL_PAY_HDR
//{"*TOP-UP PAYMENT*\0",                     "*Yuh]m apk hgvwd]*\0"},                             //TP_UP_HDR
//{"*ELECTRICITY (NEC)*\0",                  "* avhx ;ivfhx *\0"},                                //S_ELC_NEC_HDR
//{"*CARD TRANSFER*\0",                      "*hgjp,dg gf'hrm*\0"},                               //CRD_TRANS_HDR
//{"*ACCOUNT TRANSFER*\0",                   "*hgjp,dg gpshf *\0"},                                //ACNT_TRANS_HDR
//{"*MONEY DEPOSIT*\0",                      "*jy`dm hgf'hrm *\0"},                                //MNY_DEPOS_HDR
//{"*MONEY TRANSFER*\0",                     "* Yvshg p,hgm *\0"},                                //MNY_TRANS_HDR
//{"*MONEY RECEIVE*\0",                      "* Ysjbl p,hgm *\0"},                                //MNY_RCV_HDR
//{"*BALANCE INQUIRY*\0",                    "Ysjubl uk hgvwd]\0"},                               //BLNC_INQ_HDR
//{"***PIN CHANGE***\0",                     "* jyddv hgvrl hgsvd *\0"},                              //PN_CHNG_HDR
{"***** VOID *****\0",                     "**** Ygyhx ****\0"},                                //VOID_HDR
{"**** REFUND ****\0",                     "**** hv[hu ****\0"},                                 //REF_HDR
//{"*PURCHASE WITH CASH BACK*\0",            "kr]dm lu hgavhx\0"},                                //S_PURCH_CASH_BK_HDR
{"**** BATCH ****\0",                      "*Yvshg hgjs,dm *\0"},                                //BATCH_HDR         
{"*MINI STATEMENT*\0",                     "*;at pshf lwyv *\0"},                                //MNI_STAT_HDR                                                                                                            //
{"*OTHER SERVICES*\0",                     "** o]lhj Hovn **\0"},                                      //OTHR_SERV_HDR              


{"Date:\0",                                 "jhvdo:\0"},                                            //R_DATE
{"Time:\0",                                 ",rj:\0"},                                              //R_TIME
{"Merchant ID:\0",                          "vrl hgjh[v:\0"},                                         //R_MRCH_ID
{"Terminal ID:\0",                          "vrl hglh;dki:\0"},                                         //R_TRM_ID
{"Card Number:\0",                          "vrl hgf'hrm:\0"},                                        //R_CRD_NO            
{"Card Holder Name:\0",                     "Ysl whpf hgf'hrm:\0"},                                   //R_CRD_HLD_NAME        
{"Expiry Date:\0",                          "jhvdo wbpdm hgf'hrm:\0"},                                //R_EXP_DATE          

//{"PURCHASE\0",                                     "avhx\0"},                                         //R_PURCH_HDR
{"*** APPROVED ***\0",                               "*hgluhlgm kh[pm*\0"},                               //R_APRV_HDR           
{"Failed Transaction\0",                           "**luhlgm thagm**\0"},                                //R_FLD_TRANS_HDR      
{"STAN:\0",                              "vrl hgluhlgm:\0"},                                //R_TRANS_ID
{"Receipt No:\0",                                  "vrl hghdwhg:\0"},                                 //R_RCT_NO             
{"Amount:\0",                                      "hglfgy:\0"},                                      //R_AMNT               
{"Reference No:\0",                                "vrl hglv[u:\0"},                                  //R_REF_NO             
{"AUTH No:\0",                                     "vrl hgjprr:\0"},                                  //R_ATH_NO             
{"******** Customer Copy ********\0",            "********* ksom hg.f,k *********\0"},           //R_CUST_CPY_HDR      
{"******** Merchant Copy ********\0",            "********* ksom hgjh[v *********\0"},            //R_MERCH_CPY_HDR      

//{"********* Bill Payment ********\0",            "******** s]h] hgthj,vm ********\0"},       //R_BL_PAY_HDR
{"Mobile Number:\0",                               "vrl hgjgt,k:\0"},                                 //R_MOB_NO             
{"Contract Number:\0",                             "vrl hgjuhr]:\0"},                                 //R_CONT_NO            
{"Billed Amount:\0",                               "lfgy hgthj,vm:\0"},                               //R_BLD_AMNT           
{"Paid Amount:\0",                                 "hglfgy hgl]t,u:\0"},                              //R_PAD_AMNT           

//{"********** Top-Up Payment **********\0",         "********** Yuh]m apk hgvwd] **********\0"},       //R_TOP_UP_PAY_HDR
{"Voucher Amount:\0",                              "lfgy hgapk:\0"},                                  //R_VCH_AMNT           
{"Customer Signature:\0",                          "j,rdu hg.f,k:\0"},                                //R_CUST_SIG           
{"Response Code:\0",                               "vl. hgHsj[hfm:\0"},                               //R_RES_CD             
{"Failed Message:\0",                              "vshgm hgtag:\0"},                                 //R_FLD_MSG            

//{"********* Electricity Payment **********\0",     "********* avhx hg;ivfhx **********\0"},           //R_ELC_PAY_HDR
{"RRN No:\0",                                      "vrl hglv[u:\0"},                                  //R_RNN_NO             
{"Units:\0",                                       "hg,p]hj:\0"},                                     //R_UNT                
{"Kwh\0",                                          "; , s \0"},                                             //R_KWH                
{"Sale Amount:\0",                                 "lfgy hgavhx:\0"},                                 //R_SL_AMNT            
{"Fees:\0",                                        "hgul,gm:\0"},                                     //R_FEES               
{"LE\0",                                          "[ \0"},                                             //R_SDG
{"Customer Name:\0",                               "Ysl hguldg:\0"},                                  //R_CUST_NM            
{"Account No:\0",                                  "vrl hgpshf:\0"},                                  //R_ACNT_NO            
{"Meter No:\0",                                    "vrl hgu]h]:\0"},                                  //R_MTR_NO             
{"Reference:\0",                                   "vrl hglv[u:\0"},                                  //R_REF                
{"Operator Message:\0",                            "vshgm hglayg:\0"},                                //R_OPR_MSG            

//{"************ Card Transfer  *********** \0",     "hgjp,dg gf'hrm\0"},                               //CRD_TRNS_HDR
//{"********** Account Transfer  ********** \0",     "hgjp,dg gpshf\0"},                                //ACC_TRNS_HDR
//{"*********** MONEY DEPOSIT **********\0",         "jy`dm hgf'hrm\0"},                                //MNY_DEPOS_HDR
//{"*********** MONEY TRANSFER *********\0",         "hvshg p,hgm\0"},                                  //MNY_TRANS_HDR
{"Approval Code:\0",                               "vrl hgp,hgm\0"},                                  //APPRVL_CODE
//{"*********** Money Receive ***********\0",        "hsjghl p,hgm\0"},                                 //MNY_RCV_HDR
//{"********** Balance Inquiry ***********\0",       "Ysjughl uk vwd]\0"},                              //BAL_INQ_HDR
{"Current Balance:\0",                             "hgvwd] hgphgd\0"},                                //CURR_BAL
//{"************* Change PIN ************\0",        "jyddv hgvrl hgsvd\0"},                            //CHNG_PIN_HDR
//{"*************** Void ***************\0",         "hgyhx\0"},                                        //VOID_HDR
//{"***** PURCHASE WITH CASHBACK *****\0",           "kr]dm lu avhx\0"},                                //PURCH_CASH_BK_HDR
{"Total Amount:\0",                                "hglfgy hg;gd\0"},                                 //TOT_AMNT
{"Cash Back Amount\0",                             "lfgy hgkr]dm\0"},                                 //CSH_BCK_AMNT
                                                                                                      
//{" Batch \0",                                      "js,dm\0"},                                        //BATCH_HDR
{"Date    Operation Name    Amount\0",             "jhvdo   k,u hgluhlgm      hglfgy\0"},                    //BATCH_HDR1
                                                                                                      
//{"********** MINI STATEMENT **********\0",         ";at pshf lwyv\0"},                                //MINI_STAT_HDR
{"Date  Operation Code   Amount\0",                "jhvdo  vl. hgluhlgm   hglfgy\0"},                    //MINI_STAT_HDR1
//{"************** Refund**************\0",          "Yv[hu\0"},                                        //REFUND_HDR
{"Configuration Report\0",                         "jrvdv hghu]h]hj\0"},                                             //CONFIG_RPRT_HDR
{"Application Version:\0",                         "hghw]hv\0"},                                             //APP_VER
{"Static APN1:\0",                                  "\0"},                                             //APN1
{"Static APN2:\0",                                  "\0"},                                             //APN2
{"Term Serial No:\0",                         "\0"},                                             //TERM_SERIAL_NO
                                                                                                      
{"Batch No\0",                                     "vrl hgjs,dm:\0"},                                 //BTCH_NO
{"*Details Report*\0",                             "* jrvdv jtwdgd *\0"} ,                                 //DTLD_RPRT_HDR
{"Receipt No.      Transaction Name\0",           "vrl hghdwhg          hsl hgulgdm\0"},           //DTLD_RPRT_HDR1
{"Date Time        Amount \0",                    "jhvdo .lk             hglfgy\0"},                //DTLD_RPRT_HDR2
{"PAN              Auth No.\0",                   "vrl hgf'hrm           vrl hgjprr\0"},           //DTLD_RPRT_HDR3
//{"Transaction Name PAN Auth No.\0",                "hsl hgulgdm vrl hgf'hrm vrl hgjprr\0"},           //DTLD_RPRT_HDR1
//{"Amount Date Time Receipt No.\0",                 "hglfgy jhvdo .lk vrl hghdwhg\0"},                 //DTLD_RPRT_HDR2
{"*Summary Report*\0",                               "* jrvdv Y[lhgd *\0"},                                  //SUMM_RPRT_HDR
//{"Transaction Name          Accounts\0",           "hsl hgulgdm     l[l,u hgluhlghj\0"},            //SUMM_RPRT_HDR1
{"Transaction Name          \0",           "hsl hgulgdm     \0"},            //SUMM_RPRT_HDR1

{"Total Amounts\0",                                "l[l,u hglfhgy\0"},                                  //SUMM_RPRT_HDR2
{"TOTAL\0",                                        "hgl[l,u \0"},                                       //R_TOTAL
//{"Transaction Name Accounts Total Amounts\0",      "hsl hgulgdm l[l,u hgluhlghj l[l,u hglfhgy\0"},    //SUMM_RPRT_HDR1
{"******** Re print Copy ********\0",              "******* ksom Yuh]m 'fhum *******\0"},           //REPRINT_CPY
{"Print Merchant Copy?\0",                                   "'fhum ksom hgjh[v ?\0"},                                //R_PRINT_CPY_Q
{ "AID:",											""},//R_AID
{"\0",              "\0"}
};
const udtErrMsgRaw arrudtErrMsgsTable[] =
{
  {00, {"Approved\0",                                            "luhlgm kh[pm\0"}},
  {10, {"Contact Card Issuer\0",                                 "lvh[um lw]v hgf'hrm\0"}},
  {03, {"Format Error\0",                                        "o'H td hgluhlgm\0"}},
  {05, {"External Decline\0",                                    "luhlgm thagm\0"}},
  {11, {"Please change the Default PIN\0",                       "d[f jyddv hgvrl hgsvd\0"}},
  {12, {"Invalid Transaction\0",                                 "hgluhlgm ydv luv,tm\0"}},
  {13, {"Merchant Limit Exceeded\0",                             "j[h,. hglfgy hglsl,p fi ggluhlgm\0"}},
  {14, {"Invalid Track 2\0",                                     "o'H td Yw]hv hgf'hrm\0"}},
  {30, {"Invalid Format\0",                                      "hgluhlgm ydv wpdpm\0"}},
  {63, {"Restricted Card\0",                                     "f'hrm l,r,tm\0"}},
  {63, {"PIN tries Limit was Exceeded\0",                        "jl Yyghr hgf'hrm\0"}},
  {41, {"Lost Card\0",                                           "hgf'hrm ltr,]m\0"}},
  {43, {"Stolen Card\0",                                         "hgf'hrm lsv,rm\0"}},
  {50, {"Not Active Card, Closed Card\0",                        "f'hrm ydv lka'm f'hrm lygrm\0"}},
  {51, {"Insufficient Funds\0",                                  "hgvwd] ydv ;htd\0"}},
  {52, {"Declared Card\0",                                       "f'hrm ydv l,[,]m\0"}},
  {54, {"Expired Card\0",                                        "hgf'hrm lkjidm hgwghpdm\0"}},
  {55, {"Invalid PIN\0",                                         "o'H td hgvrl hgsvd\0"}},
  {58, {"Invalid Processing Code\0",                             "o'H td vrl hgulgdm\0"}},
  {30, {"Withdrawal Limit Would be Exceeded\0",                  "j[h,. hglfgy hglsl,p fi ggspf\0"}},
  {62, {"Encryption Error1\0",                                   "o'H td hgjatdv 1\0"}},
  {63, {"Encryption Error2\0",                                   "o'H td hgjatdv 2\0"}},
  {64, {"Maximum Balance is Exceeded.\0",                        "j[h,. hglfgy hglsl,p fi ggf'hrm\0"}},
  {67, {"Invalid Amount\0",                                      "o'H td hglfgy\0"}},
  {75, {"PIN Tries Limit was Reached\0",                         "kth` lph,ghj hgvrl hgsvd\0"}},
  {78, {"Original Request not Found\0",                          "o'H td hglw]v\0"}},
  {81, {"Wrong Format of Customer Information Field\0",          "o'H td fdhkhj hgf'hrm\0"}},
  {82, {"Prepaid Code not Found\0",                              "vrl hglo]l ydv l,[,]\0"}},
  {89, {"Invalid Terminal ID\0",                                 "vrl hglh;dkm ydv luvt\0"}},
  {91, {"Destination not Available\0",                           "lw]v hgf'hrm ydv ljhp\0"}},
  {94, {"duplicate Transaction\0",                               "vrl hgluhlgm l;vv\0"}},
  {95, {"Batch Error\0",                                         "o'H td hgjs,dm\0"}},
  {96, {"system Error\0",                                        "o'H td hglw]v\0"}},
  {255,{" UnKnown Error\0",                                      "o'H ydv luvt"}}
};

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

char* GetMsgs(unsigned int index)
{
  return(MsgsTable[index][udtAppLang]);
}
char* GetLangMsgs(unsigned int index, APP_LANG udtSelectedLang)
{
  return(MsgsTable[index][udtSelectedLang]);
}

char* GetErrMsgs(unsigned char ucErrCode)
{
  unsigned char ucIdx;
  unsigned char ucErrTableSize = sizeof(arrudtErrMsgsTable)/sizeof(udtErrMsgRaw);
  for (ucIdx = 0; ucIdx < (ucErrTableSize - 1); ucIdx++) {
    if (arrudtErrMsgsTable[ucIdx].ErrCode == ucErrCode) {
      return arrudtErrMsgsTable[ucIdx].ErrMsg[udtAppLang];
    }
  }
  return arrudtErrMsgsTable[ucIdx].ErrMsg[udtAppLang];

}
