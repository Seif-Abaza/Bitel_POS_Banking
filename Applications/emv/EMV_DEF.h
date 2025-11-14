/*---------------------------------------------------------------------------+
| Author:    RSBON
| File:      EMV_DEF.H
| Purpose:   EMV_DEF (Card Holder Activated Terminal) header file
| Changes:
|
| Date:      Author  Comment:
| ---------  ------  --------
| 26-Feb-02  RSBON   Created.
+---------------------------------------------------------------------------*/

#ifndef _EMV_DEF_H_
#define _EMV_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIPS_LITTLE
#define MIPS_LITTLE
#endif
/*===============Start of Structure Definitions ===========================*/
    // Tag;     // 0x9B Transaction Status Information (TSI)
    // U08 Len; // 2 Bytes Length(16 bits)
typedef struct EMV_16BIT_tag{
    // Tag;     // 0x9B Transaction Status Information (TSI)
    // U08 Len; // 2 Bytes Length(16 bits)
#ifdef MIPS_LITTLE
    //Byte 1:
    U08 B1b1:1; // 1 = Offline data authentication was performed
    U08 B1b2:1; // 1 = Cardholder verification was performed
    U08 B1b3:1; // 1 = Card risk management was performed
    U08 B1b4:1; // 1 = Issuer authentication was performed
    U08 B1b5:1; // 1 = Terminal risk management was performed
    U08 B1b6:1; // 1 = Issuer Script processing was performed
    U08 B1b7:1; // 0 RFU
    U08 B1b8:1; // 0 RFU
    //Byte 2:
    U08 B2b1:1; // 0 RFU
    U08 B2b2:1; // 0 RFU
    U08 B2b3:1; // 0 RFU
    U08 B2b4:1; // 0 RFU
    U08 B2b5:1; // 0 RFU
    U08 B2b6:1; // 0 RFU
    U08 B2b7:1; // 0 RFU
    U08 B2b8:1; // 0 RFU
#else
    //Byte 1:
    U08 B1b8:1; // 1 = Offline data authentication was performed
    U08 B1b7:1; // 1 = Cardholder verification was performed
    U08 B1b6:1; // 1 = Card risk management was performed
    U08 B1b5:1; // 1 = Issuer authentication was performed
    U08 B1b4:1; // 1 = Terminal risk management was performed
    U08 B1b3:1; // 1 = Issuer Script processing was performed
    U08 B1b2:1; // 0 RFU
    U08 B1b1:1; // 0 RFU
    //Byte 2:
    U08 B2b8:1; // 0 RFU
    U08 B2b7:1; // 0 RFU
    U08 B2b6:1; // 0 RFU
    U08 B2b5:1; // 0 RFU
    U08 B2b4:1; // 0 RFU
    U08 B2b3:1; // 0 RFU
    U08 B2b2:1; // 0 RFU
    U08 B2b1:1; // 0 RFU
#endif
}EMV_16BIT_TAG;

    // Tag;     // 0x9F33 Terminal Capabilities
    // U08 Len; // 3 Bytes Length(24 bits)
typedef struct EMV_24BIT_tag{
    // Tag;     // 0x9F33 Terminal Capabilities
    // U08 Len; // 3 Bytes Length(24 bits)
#ifdef MIPS_LITTLE
    //Byte 1 (Card Data Input Capability):
    U08 B1b1:1; // 1 = Manual key entry
    U08 B1b2:1; // 1 = Magnetic stripe
    U08 B1b3:1; // 1 = IC with contacts
    U08 B1b4:1; // 0 RFU
    U08 B1b5:1; // 0 RFU
    U08 B1b6:1; // 0 RFU
    U08 B1b7:1; // 0 RFU
    U08 B1b8:1; // 0 RFU
    //Byte 2 (CVM Capability):
    U08 B2b1:1; // 1 = Plaintext PIN for offline ICC verification
    U08 B2b2:1; // 1 = Enciphered PIN for online verification
    U08 B2b3:1; // 1 = Signature (paper)
    U08 B2b4:1; // 1 = Enciphered PIN for offline verification
    U08 B2b5:1; // 1 = No CVM Required //SU10
    U08 B2b6:1; // 0 RFU
    U08 B2b7:1; // 0 RFU
    U08 B2b8:1; // 0 RFU
    //Byte 3 (Security Capability):
    U08 B3b1:1; // 1 = Offline static data authentication
    U08 B3b2:1; // 1 = Offline dynamic data authentication
    U08 B3b3:1; // 1 = Card capture
    U08 B3b4:1; // 0 RFU                          //SU09
    U08 B3b5:1; // 1 = Combined DDA/AC generation //SU09
    U08 B3b6:1; // 0 RFU
    U08 B3b7:1; // 0 RFU
    U08 B3b8:1; // 0 RFU
#else
    //Byte 1 (Card Data Input Capability):
    U08 B1b8:1; // 1 = Manual key entry
    U08 B1b7:1; // 1 = Magnetic stripe
    U08 B1b6:1; // 1 = IC with contacts
    U08 B1b5:1; // 0 RFU
    U08 B1b4:1; // 0 RFU
    U08 B1b3:1; // 0 RFU
    U08 B1b2:1; // 0 RFU
    U08 B1b1:1; // 0 RFU
    //Byte 2 (CVM Capability):
    U08 B2b8:1; // 1 = Plaintext PIN for offline ICC verification
    U08 B2b7:1; // 1 = Enciphered PIN for online verification
    U08 B2b6:1; // 1 = Signature (paper)
    U08 B2b5:1; // 1 = Enciphered PIN for offline verification
    U08 B2b4:1; // 1 = No CVM Required //SU10
    U08 B2b3:1; // 0 RFU
    U08 B2b2:1; // 0 RFU
    U08 B2b1:1; // 0 RFU
    //Byte 3 (Security Capability):
    U08 B3b8:1; // 1 = Offline static data authentication
    U08 B3b7:1; // 1 = Offline dynamic data authentication
    U08 B3b6:1; // 1 = Card capture
    U08 B3b5:1; // 0 RFU                          //SU09
    U08 B3b4:1; // 1 = Combined DDA/AC generation //SU09
    U08 B3b3:1; // 0 RFU
    U08 B3b2:1; // 0 RFU
    U08 B3b1:1; // 0 RFU
#endif
}EMV_24BIT_TAG;

    // Tag;     // 0x95   TerminalVerificationResults(TVR)                          // 0x9F40 AdditionalTerminalCapabilities(ATC)
    // U08 Len; // 5 Bytes Length(40 bits)
typedef struct EMV_40BIT_tag{
    // Tag;     // 0x95   TerminalVerificationResults(TVR)                          // 0x9F40 AdditionalTerminalCapabilities(ATC)
    // U08 Len; // 5 Bytes Length(40 bits)
#ifdef MIPS_LITTLE
    //Byte 1                                                                        //Byte 1 (Transaction Type Capability):
    U08 B1b1:1; // 1 = Offline data authentication was not performed                 1 = Cash
    U08 B1b2:1; // 1 = Offline static data authentication(SDA) failed                1 = Goods
    U08 B1b3:1; // 1 = ICC data missing                                              1 = Services
    U08 B1b4:1; // 1 = Card appears on terminal exception file                       1 = Cashback
    U08 B1b5:1; // 1 = Offline dynamic data authentication(DDA) failed               1 = Inquiry
    U08 B1b6:1; // 1 = Combined DDA/AC Generation failed                             1 = Transfer
    U08 B1b7:1; // 0 RFU                                                             1 = Payment
    U08 B1b8:1; // 0 RFU                                                             1 = Administrative
    //Byte 2                                                                        //Byte 2 (Transaction Type Capability):
    U08 B2b1:1; // 1 = ICC and terminal have different application versions          0 RFU
    U08 B2b2:1; // 1 = Expired application                                           0 RFU
    U08 B2b3:1; // 1 = Application not yet effective                                 0 RFU
    U08 B2b4:1; // 1 = Requested service not allowed for card product                0 RFU
    U08 B2b5:1; // 1 = New card                                                      0 RFU
    U08 B2b6:1; // 0 RFU                                                             0 RFU
    U08 B2b7:1; // 0 RFU                                                             0 RFU
    U08 B2b8:1; // 0 RFU                                                             0 RFU
    //Byte 3                                                                        //Byte 3 (Terminal Data Input Capability):
    U08 B3b1:1; // 1 = Cardholder verification was not successful                    1 = Numeric keys
    U08 B3b2:1; // 1 = Unrecognized Cardholder verification Method(CVM)              1 = Alphabetic and special characters keys
    U08 B3b3:1; // 1 = PIN Try Limit exceeded                                        1 = Command keys
    U08 B3b4:1; // 1 = PIN entry required and PIN pad not present/working            1 = Function keys
    U08 B3b5:1; // 1 = PIN entry required, PIN pad present, but PIN was not entered  0 RFU
    U08 B3b6:1; // 1 = Online PIN entered                                            0 RFU
    U08 B3b7:1; // 0 RFU                                                             0 RFU
    U08 B3b8:1; // 0 RFU                                                             0 RFU
    //Byte 4                                                                        //Byte 4 (Terminal Data Output Capability):
    U08 B4b1:1; // 1 = Transaction exceeds floor limit                               1 = Print, attendant
    U08 B4b2:1; // 1 = Lower consecutive offline limit(LCOL)("9F14") exceeded        1 = Print, cardholder
    U08 B4b3:1; // 1 = Upper consecutive offline limit(UCOL)("9F23") exceeded        1 = Display, attendant
    U08 B4b4:1; // 1 = Transaction selected randomly for online processing           1 = Display, cardholder
    U08 B4b5:1; // 1 = Merchant forced transaction online                            0 RFU
    U08 B4b6:1; // 0 RFU                                                             0 RFU
    U08 B4b7:1; // 0 RFU                                                             1 = Code table 10
    U08 B4b8:1; // 0 RFU                                                             1 = Code table 9
    //Byte 5                                                                        //Byte 5 (Terminal Data Output Capability):
    U08 B5b1:1; // 1 = Default TDOL used                                             1 = Code table 8
    U08 B5b2:1; // 1 = Issuer authentication was unsuccessful                        1 = Code table 7
    U08 B5b3:1; // 1 = Issuer Script processing failed before final GENERATE AC      1 = Code table 6
    U08 B5b4:1; // 1 = Issuer Script processing failed after final GENERATE AC       1 = Code table 5
    U08 B5b5:1; // 0 RFU                                                             1 = Code table 4
    U08 B5b6:1; // 0 RFU                                                             1 = Code table 3
    U08 B5b7:1; // 0 RFU                                                             1 = Code table 2
    U08 B5b8:1; // 0 RFU                                                             1 = Code table 1
#else
    //Byte 1                                                                        //Byte 1 (Transaction Type Capability):
    U08 B1b8:1; // 1 = Offline data authentication was not performed                 1 = Cash
    U08 B1b7:1; // 1 = Offline static data authentication(SDA) failed                1 = Goods
    U08 B1b6:1; // 1 = ICC data missing                                              1 = Services
    U08 B1b5:1; // 1 = Card appears on terminal exception file                       1 = Cashback
    U08 B1b4:1; // 1 = Offline dynamic data authentication(DDA) failed               1 = Inquiry
    U08 B1b3:1; // 1 = Combined DDA/AC Generation failed                             1 = Transfer
    U08 B1b2:1; // 0 RFU                                                             1 = Payment
    U08 B1b1:1; // 0 RFU                                                             1 = Administrative
    //Byte 2                                                                        //Byte 2 (Transaction Type Capability):
    U08 B2b8:1; // 1 = ICC and terminal have different application versions          0 RFU
    U08 B2b7:1; // 1 = Expired application                                           0 RFU
    U08 B2b6:1; // 1 = Application not yet effective                                 0 RFU
    U08 B2b5:1; // 1 = Requested service not allowed for card product                0 RFU
    U08 B2b4:1; // 1 = New card                                                      0 RFU
    U08 B2b3:1; // 0 RFU                                                             0 RFU
    U08 B2b2:1; // 0 RFU                                                             0 RFU
    U08 B2b1:1; // 0 RFU                                                             0 RFU
    //Byte 3                                                                        //Byte 3 (Terminal Data Input Capability):
    U08 B3b8:1; // 1 = Cardholder verification was not successful                    1 = Numeric keys
    U08 B3b7:1; // 1 = Unrecognized Cardholder verification Method(CVM)              1 = Alphabetic and special characters keys
    U08 B3b6:1; // 1 = PIN Try Limit exceeded                                        1 = Command keys
    U08 B3b5:1; // 1 = PIN entry required and PIN pad not present/working            1 = Function keys
    U08 B3b4:1; // 1 = PIN entry required, PIN pad present, but PIN was not entered  0 RFU
    U08 B3b3:1; // 1 = Online PIN entered                                            0 RFU
    U08 B3b2:1; // 0 RFU                                                             0 RFU
    U08 B3b1:1; // 0 RFU                                                             0 RFU
    //Byte 4                                                                        //Byte 4 (Terminal Data Output Capability):
    U08 B4b8:1; // 1 = Transaction exceeds floor limit                               1 = Print, attendant
    U08 B4b7:1; // 1 = Lower consecutive offline limit(LCOL)("9F14") exceeded        1 = Print, cardholder
    U08 B4b6:1; // 1 = Upper consecutive offline limit(UCOL)("9F23") exceeded        1 = Display, attendant
    U08 B4b5:1; // 1 = Transaction selected randomly for online processing           1 = Display, cardholder
    U08 B4b4:1; // 1 = Merchant forced transaction online                            0 RFU
    U08 B4b3:1; // 0 RFU                                                             0 RFU
    U08 B4b2:1; // 0 RFU                                                             1 = Code table 10
    U08 B4b1:1; // 0 RFU                                                             1 = Code table 9
    //Byte 5                                                                        //Byte 5 (Terminal Data Output Capability):
    U08 B5b8:1; // 1 = Default TDOL used                                             1 = Code table 8
    U08 B5b7:1; // 1 = Issuer authentication was unsuccessful                        1 = Code table 7
    U08 B5b6:1; // 1 = Issuer Script processing failed before final GENERATE AC      1 = Code table 6
    U08 B5b5:1; // 1 = Issuer Script processing failed after final GENERATE AC       1 = Code table 5
    U08 B5b4:1; // 0 RFU                                                             1 = Code table 4
    U08 B5b3:1; // 0 RFU                                                             1 = Code table 3
    U08 B5b2:1; // 0 RFU                                                             1 = Code table 2
    U08 B5b1:1; // 0 RFU                                                             1 = Code table 1
#endif
}EMV_40BIT_TAG;

    // Tag;     // 0x82 Application Interchange Profile(AIP)
    // U08 Len; // 2 Bytes Length(16 bits)
typedef struct {
    // Tag;     // 0x82 Application Interchange Profile(AIP)
    // U08 Len; // 2 Bytes Length(16 bits)
#ifdef MIPS_LITTLE
    U08 CDA :1; // 1 Combined DDA/AC Generation //SU09
    U08 B1b2:1; // 0 RFU                        //SU09
    U08 IAC :1; // 1 Issuer Authentication
    U08 TRM :1; // 1 Terminal risk management performed
    U08 CVM :1; // 1 Cardholder Verification
    U08 DDA :1; // 1 Offline Dynamic Data Authentication
    U08 SDA :1; // 1 Offline Static  Data Authentication
    U08 B1b8:1; // 0 RFU
    U08 B2b1:1; // 0 RFU
    U08 B2b2:1; // 0 RFU
    U08 B2b3:1; // 0 RFU
    U08 B2b4:1; // 0 RFU
    U08 B2b5:1; // 0 RFU
    U08 B2b6:1; // 0 RFU
    U08 B2b7:1; // 0 RFU
    U08 B2b8:1; // 0 RFU
#else
    U08 B1b8:1; // 0 RFU
    U08 SDA :1; // 1 Offline Static  Data Authentication
    U08 DDA :1; // 1 Offline Dynamic Data Authentication
    U08 CVM :1; // 1 Cardholder Verification
    U08 TRM :1; // 1 Terminal risk management performed
    U08 IAC :1; // 1 Issuer Authentication
    U08 B1b2:1; // 0 RFU                        //SU09
    U08 CDA :1; // 1 Combined DDA/AC Generation //SU09
    U08 B2b8:1; // 0 RFU
    U08 B2b7:1; // 0 RFU
    U08 B2b6:1; // 0 RFU
    U08 B2b5:1; // 0 RFU
    U08 B2b4:1; // 0 RFU
    U08 B2b3:1; // 0 RFU
    U08 B2b2:1; // 0 RFU
    U08 B2b1:1; // 0 RFU
#endif
}EMV_AIP;

typedef struct {
    U08 SFI; //Short File Identifier (a numeric file label)
    U08 frn; //Record number of the first record to be read
    U08 lrn; //Record number of the last record to be read
    U08 rns; //Number of consecutive records with frn...
}EMV_AFL;

typedef struct {
    #define CodeFail          0     //000000 = Fail CVM processing
    #define CodeTxtPIN        1     //000001 = Plaintext PIN verification performed by ICC
    #define CodeEncPINonLine  2     //000010 = Enciphered PIN verified online
    #define CodeTxtPIN_Sign   3     //000011 = Plaintext PIN verification performed by ICC and signature(paper)
    #define CodeEncPIN        4     //000100 = Enciphered PIN verification performed by ICC
    #define CodeEncPIN_Sign   5     //000101 = Enciphered PIN verification performed by ICC and signature(paper)
    #define CodeSign          0x1E  //011110 = Signature (paper)
    #define CodeNoCVM         0x1F  //011111 = No CVM required
    #define NoCVMperformed    0x3F  //111111 = No CVM performed

    #define CC_Always         0 	//00 = Always
    #define CC_Cash           1 	//01 = If cash or cashback (includes quasi-cash)
    #define CC_NotCash        2 	//02 = If not cash or cashback (includes quasi-cash)
    #define CC_SupportCVM     3 	//03 = If terminal supports the CVM
    #define CC_TrxnUnderX     6 	//06 = If transaction is in Application Currency Code and is under X value
    #define CC_TrxnOverX      7 	//07 = If transaction is in Application Currency Code and is over X value
    #define CC_TrxnUnderY     8 	//08 = If transaction is in Application Currency Code and is under Y value
    #define CC_TrxnOverY      9 	//09 = If transaction is in Application Currency Code and is over Y value
#ifdef MIPS_LITTLE
    U08 Code :6;
    U08 bit7 :1;     // 1 Apply succeeding CVM field if this CVM is unsuccessful
    U08 bit8 :1;     // 0 RFU
#else
    U08 bit8 :1;     // 0 RFU
    U08 bit7 :1;     // 1 Apply succeeding CVM field if this CVM is unsuccessful
    U08 Code :6;
#endif
	U08 B2CC;        // (CVM Condition Code):
}EMV_CVMCodeCC;

typedef struct TagNameDef {
    int  Tag;
    char *TagName;
}EMV_TagNAME;

typedef struct {
	#define EMV_RSA		(U08)0x01 //Digital signature algorithm
	#define EMV_SHA1	(U08)0x01 //Hash algorithm indicator
    U08 *RID;      /* RID 5 bytes     */
    U08 index;     /* Key Index       */
    U08 *CSN;      /* Certificate Serial Number = 3 bytes */
    U32 exponent;  /* public exponent */
    U16 bits;      /* length in bits of modulus */
    U08 *modulus;  /* modulus */
}EMV_RSA_PK;

#ifdef __cplusplus
}
#endif

#endif
