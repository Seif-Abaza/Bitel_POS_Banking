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
#include "CurrencyCodes.h"

/******************************************************************************/
/* TYPES                                                                      */
/******************************************************************************/


/******************************************************************************/
/* CONSTANTS, MACROS                                                          */
/******************************************************************************/
const strCurrencyCode udtCurrencyTable[MAX_CODES] = 
{
 {"AED\0",	"784\0"},     // United Arab Emirates dirham                                
 {"AFN\0",	"971\0"},     // Afghan afghani                                             
 {"ALL\0",	"008\0"},     // Albanian lek                                               
 {"AMD\0",	"051\0"},     // Armenian dram                                              
 {"ANG\0",	"532\0"},     // Netherlands Antillean guilder                              
 {"AOA\0",	"973\0"},     // Angolan kwanza                                             
 {"ARS\0",	"032\0"},     // Argentine peso                                             
 {"AUD\0",	"036\0"},     // Australian dollar                                          
 {"AWG\0",	"533\0"},     // Aruban florin                                              
 {"AZN\0",	"944\0"},     // Azerbaijani manat                                          
 {"BAM\0",	"977\0"},     // Bosnia and Herzegovina convertible mark                    
 {"BBD\0",	"052\0"},     // Barbados dollar                                            
 {"BDT\0",	"050\0"},     // Bangladeshi taka                                           
 {"BGN\0",	"975\0"},     // Bulgarian lev                                              
 {"BHD\0",	"048\0"},     // Bahraini dinar                                             
 {"BIF\0",	"108\0"},     // Burundian franc                                            
 {"BMD\0",	"060\0"},     // Bermudian dollar                                           
 {"BND\0",	"096\0"},     // Brunei dollar                                              
 {"BOB\0",	"068\0"},     // Boliviano                                                  
 {"BOV\0",	"984\0"},     // Bolivian Mvdol (funds code)                                
 {"BRL\0",	"986\0"},     // Brazilian real                                             
 {"BSD\0",	"044\0"},     // Bahamian dollar                                            
 {"BTN\0",	"064\0"},     // Bhutanese ngultrum                                         
 {"BWP\0",	"072\0"},     // Botswana pula                                              
 {"BYR\0",	"974\0"},     // Belarusian ruble                                           
 {"BZD\0",	"084\0"},     // Belize dollar                                              
 {"CAD\0",	"124\0"},     // Canadian dollar                                            
 {"CDF\0",	"976\0"},     // Congolese franc                                            
 {"CHE\0",	"947\0"},     // WIR Euro (complementary currency)                          
 {"CHF\0",	"756\0"},     // Swiss franc                                                
 {"CHW\0",	"948\0"},     // WIR Franc (complementary currency)                         
 {"CLF\0",	"990\0"},     // Unidad de Fomento (funds code)                             
 {"CLP\0",	"152\0"},     // Chilean peso                                               
 {"CNY\0",	"156\0"},     // Chinese yuan                                               
 {"COP\0",	"170\0"},     // Colombian peso                                             
 {"COU\0",	"970\0"},     // Unidad de Valor Real                                       
 {"CRC\0",	"188\0"},     // Costa Rican colon                                          
 {"CUC\0",	"931\0"},     // Cuban convertible peso                                     
 {"CUP\0",	"192\0"},     // Cuban peso                                                 
 {"CVE\0",	"132\0"},     // Cape Verde escudo                                          
 {"CZK\0",	"203\0"},     // Czech koruna                                               
 {"DJF\0",	"262\0"},     // Djiboutian franc                                           
 {"DKK\0",	"208\0"},     // Danish krone                                               
 {"DOP\0",	"214\0"},     // Dominican peso                                             
 {"DZD\0",	"012\0"},     // Algerian dinar                                             
 {"EGP\0",	"818\0"},     // Egyptian pound                                             
 {"ERN\0",	"232\0"},     // Eritrean nakfa                                             
 {"ETB\0",	"230\0"},     // Ethiopian birr                                             
 {"EUR\0",	"978\0"},     // Euro                                                       
 {"FJD\0",	"242\0"},     // Fiji dollar                                                
 {"FKP\0",	"238\0"},     // Falkland Islands pound                                     
 {"GBP\0",	"826\0"},     // Pound sterling                                             
 {"GEL\0",	"981\0"},     // Georgian lari                                              
 {"GHS\0",	"936\0"},     // Ghanaian cedi                                              
 {"GIP\0",	"292\0"},     // Gibraltar pound                                            
 {"GMD\0",	"270\0"},     // Gambian dalasi                                             
 {"GNF\0",	"324\0"},     // Guinean franc                                              
 {"GTQ\0",	"320\0"},     // Guatemalan quetzal                                         
 {"GYD\0",	"328\0"},     // Guyanese dollar                                            
 {"HKD\0",	"344\0"},     // Hong Kong dollar                                           
 {"HNL\0",	"340\0"},     // Honduran lempira                                           
 {"HRK\0",	"191\0"},     // Croatian kuna                                              
 {"HTG\0",	"332\0"},     // Haitian gourde                                             
 {"HUF\0",	"348\0"},     // Hungarian forint                                           
 {"IDR\0",	"360\0"},     // Indonesian rupiah                                          
 {"ILS\0",	"376\0"},     // Israeli new shekel                                         
 {"INR\0",	"356\0"},     // Indian rupee                                               
 {"IQD\0",	"368\0"},     // Iraqi dinar                                                
 {"IRR\0",	"364\0"},     // Iranian rial                                               
 {"ISK\0",	"352\0"},     // Icelandic króna                                            
 {"JMD\0",	"388\0"},     // Jamaican dollar                                            
 {"JOD\0",	"400\0"},     // Jordanian dinar                                            
 {"JPY\0",	"392\0"},     // Japanese yen                                               
 {"KES\0",	"404\0"},     // Kenyan shilling                                            
 {"KGS\0",	"417\0"},     // Kyrgyzstani som                                            
 {"KHR\0",	"116\0"},     // Cambodian riel                                             
 {"KMF\0",	"174\0"},     // Comoro franc                                               
 {"KPW\0",	"408\0"},     // North Korean won                                           
 {"KRW\0",	"410\0"},     // South Korean won                                           
 {"KWD\0",	"414\0"},     // Kuwaiti dinar                                              
 {"KYD\0",	"136\0"},     // Cayman Islands dollar                                      
 {"KZT\0",	"398\0"},     // Kazakhstani tenge                                          
 {"LAK\0",	"418\0"},     // Lao kip                                                    
 {"LBP\0",	"422\0"},     // Lebanese pound                                             
 {"LKR\0",	"144\0"},     // Sri Lankan rupee                                           
 {"LRD\0",	"430\0"},     // Liberian dollar                                            
 {"LSL\0",	"426\0"},     // Lesotho loti                                               
 {"LTL\0",	"440\0"},     // Lithuanian litas                                           
 {"LVL\0",	"428\0"},     // Latvian lats                                               
 {"LYD\0",	"434\0"},     // Libyan dinar                                               
 {"MAD\0",	"504\0"},     // Moroccan dirham                                            
 {"MDL\0",	"498\0"},     // Moldovan leu                                               
 {"MGA\0",	"969\0"},     // Malagasy ariary                                            
 {"MKD\0",	"807\0"},     // Macedonian denar                                           
 {"MMK\0",	"104\0"},     // Myanma kyat                                                
 {"MNT\0",	"496\0"},     // Mongolian tugrik                                           
 {"MOP\0",	"446\0"},     // Macanese pataca                                            
 {"MRO\0",	"478\0"},     // Mauritanian ouguiya                                        
 {"MUR\0",	"480\0"},     // Mauritian rupee                                            
 {"MVR\0",	"462\0"},     // Maldivian rufiyaa                                          
 {"MWK\0",	"454\0"},     // Malawian kwacha                                            
 {"MXN\0",	"484\0"},     // Mexican peso                                               
 {"MXV\0",	"979\0"},     // Mexican Unidad de Inversion (UDI) (funds code)             
 {"MYR\0",	"458\0"},     // Malaysian ringgit                                          
 {"MZN\0",	"943\0"},     // Mozambican metical                                         
 {"NAD\0",	"516\0"},     // Namibian dollar                                            
 {"NGN\0",	"566\0"},     // Nigerian naira                                             
 {"NIO\0",	"558\0"},     // Nicaraguan córdoba                                         
 {"NOK\0",	"578\0"},     // Norwegian krone                                            
 {"NPR\0",	"524\0"},     // Nepalese rupee                                             
 {"NZD\0",	"554\0"},     // New Zealand dollar                                         
 {"OMR\0",	"512\0"},     // Omani rial                                                 
 {"PAB\0",	"590\0"},     // Panamanian balboa                                          
 {"PEN\0",	"604\0"},     // Peruvian nuevo sol                                         
 {"PGK\0",	"598\0"},     // Papua New Guinean kina                                     
 {"PHP\0",	"608\0"},     // Philippine peso                                            
 {"PKR\0",	"586\0"},     // Pakistani rupee                                            
 {"PLN\0",	"985\0"},     // Polish z?oty                                               
 {"PYG\0",	"600\0"},     // Paraguayan guaraní                                         
 {"QAR\0",	"634\0"},     // Qatari riyal                                               
 {"RON\0",	"946\0"},     // Romanian new leu                                           
 {"RSD\0",	"941\0"},     // Serbian dinar                                              
 {"RUB\0",	"643\0"},     // Russian rouble                                             
 {"RWF\0",	"646\0"},     // Rwandan franc                                              
 {"SAR\0",	"682\0"},     // Saudi riyal                                                
 {"SBD\0",	"090\0"},     // Solomon Islands dollar                                     
 {"SCR\0",	"690\0"},     // Seychelles rupee                                           
 {"SDG\0",	"938\0"},     // Sudanese pound                                             
 {"SEK\0",	"752\0"},     // Swedish krona/kronor                                       
 {"SGD\0",	"702\0"},     // Singapore dollar                                           
 {"SHP\0",	"654\0"},     // Saint Helena pound                                         
 {"SLL\0",	"694\0"},     // Sierra Leonean leone                                       
 {"SOS\0",	"706\0"},     // Somali shilling                                            
 {"SRD\0",	"968\0"},     // Surinamese dollar                                          
 {"SSP\0",	"728\0"},     // South Sudanese pound                                       
 {"STD\0",	"678\0"},     // São Tomé and Príncipe dobra                                
 {"SYP\0",	"760\0"},     // Syrian pound                                               
 {"SZL\0",	"748\0"},     // Swazi lilangeni                                            
 {"THB\0",	"764\0"},     // Thai baht                                                  
 {"TJS\0",	"972\0"},     // Tajikistani somoni                                         
 {"TMT\0",	"934\0"},     // Turkmenistani manat                                        
 {"TND\0",	"788\0"},     // Tunisian dinar                                             
 {"TOP\0",	"776\0"},     // Tongan pa?anga                                             
 {"TRY\0",	"949\0"},     // Turkish lira                                               
 {"TTD\0",	"780\0"},     // Trinidad and Tobago dollar                                 
 {"TWD\0",	"901\0"},     // New Taiwan dollar                                          
 {"TZS\0",	"834\0"},     // Tanzanian shilling                                         
 {"UAH\0",	"980\0"},     // Ukrainian hryvnia                                          
 {"UGX\0",	"800\0"},     // Ugandan shilling                                           
 {"USD\0",	"840\0"},     // United States dollar                                       
 {"USN\0",	"997\0"},     // United States dollar (next day) (funds code)               
 {"USS\0",	"998\0"},     // United States dollar (same day) (funds code) (one source[wh
 {"UYI\0",	"940\0"},     // Uruguay Peso en Unidades Indexadas (URUIURUI) (funds code) 
 {"UYU\0",	"858\0"},     // Uruguayan peso                                             
 {"UZS\0",	"860\0"},     // Uzbekistan som                                             
 {"VEF\0",	"937\0"},     // Venezuelan bolívar                                         
 {"VND\0",	"704\0"},     // Vietnamese dong                                            
 {"VUV\0",	"548\0"},     // Vanuatu vatu                                               
 {"WST\0",	"882\0"},     // Samoan tala                                                
 {"XAF\0",	"950\0"},     // CFA franc BEAC                                             
 {"XAG\0",	"961\0"},     // Silver (one troy ounce)                                    
 {"XAU\0",	"959\0"},     // Gold (one troy ounce)                                      
 {"XBA\0",	"955\0"},     // European Composite Unit (EURCO) (bond market unit)         
 {"XBB\0",	"956\0"},     // European Monetary Unit (E.M.U.-6) (bond market unit)       
 {"XBC\0",	"957\0"},     // European Unit of Account 9 (E.U.A.-9) (bond market unit)   
 {"XBD\0",	"958\0"},     // European Unit of Account 17 (E.U.A.-17) (bond market unit) 
 {"XBT\0",	".  \0"},     // Bitcoin (Internet currency)                                
 {"XCD\0",	"951\0"},     // East Caribbean dollar                                      
 {"XDR\0",	"960\0"},     // Special drawing rights                                     
 {"XFU\0",	"Nil\0"},     // UIC franc (special settlement currency)                    
 {"XOF\0",	"952\0"},     // CFA franc BCEAO                                            
 {"XPD\0",	"964\0"},     // Palladium (one troy ounce)                                 
 {"XPF\0",	"953\0"},     // CFP franc (Franc du Pacifique)                             
 {"XPT\0",	"962\0"},     // Platinum (one troy ounce)                                  
 {"XTS\0",	"963\0"},     // Code reserved for testing purposes                         
 {"XVN\0",	".  \0"},     // Ven (Internet currency)                                    
 {"XXX\0",	"999\0"},     // No currency                                                
 {"YER\0",	"886\0"},     // Yemeni rial                                                
 {"ZAR\0",	"710\0"},     // South African rand                                         
 {"ZMW\0",	"967\0"}      // Zambian kwacha                                             

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



