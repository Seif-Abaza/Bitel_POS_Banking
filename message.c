/*! 
* \file    $Source: /resources/message.c $
* \date    $Date: 2013/05/11  $
* \version $Revision: 1.1 $
* \author  $Author: HAhmed $
*/
/*!
 * *  Implementation of messages handler
*  
* (c) Copyright Bitel Emea 2013 unpublished work.
* This computer program includes confidential, proprietary 
* information and is a trade secret of Bitel. All use, 
* disclosure and/or reproduction is prohibited unless
* authorized in writing.
* All rights reserved.
*/
/*@{*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../BF_SDK/include/BTEMEA_LIB/message.h"
#include "extern_emea.h"
 
//width of array in "EnglishMsgTable" and "ArabicMsgTable"
#define WIDTH  50;

//Temp array to load active language messages to it
char currentMsgTable[][50];

APP_LANG udtAppLang = L_ENGLISH;
const char * currentLanguage="English";

/*
 * \brief  Message table loader based on selected language
 * 
 * * This function loads the language file into the POS memory so that 
 * a fast look-up is possible.  
 * Each language file must follow a specific naming convention . 
 * The name of the language Array should be 
 * <pre>
 * [language]MsgTable     
 * </pre>
 * 
 * For example, the English messages should be stored into EnglishMsgTable,
 * Arabic messages into ArabicMsgTable.... etc.
 *  
 * \param  language [in] The language code.
 * \return  0 failure
 *          1 success
 * \code
 *  //This is an example of how to use the API
 *  SetCurrentLanguage("English");
	v("Successful means %s\r\n",GetMessage(M_SUCCESSFUL));
	
 */
int SetCurrentLanguage(APP_LANG language)
{
	unsigned long numOfMessages;
	unsigned long j=0;
	
	//check current language 
	if (language == L_ARABIC)
	{
	  udtAppLang = L_ARABIC;
	}
	else if (language == L_ENGLISH)
	{
	   udtAppLang = L_ENGLISH;
	}
	else
	{
		Rprintf("Unknow language\r\n");
		return 0;
	}

	return 1;
	
}
 /*
  * \brief  Looks up a message in the mesage table
  *  
  * The function returns a message from the  message table  based 
  * on the message Index
  * 
  * \param   index [in] the numeric id of the message to retrieve. 
  *          It must be greater than 0  
  * 
  * \return  the message that corresponds to the requested Id
  */
char* GetMessage(unsigned int index)
{

  if (udtAppLang == L_ARABIC)
  {
    //return ArabicMsgTable[index];
  }
  else if (udtAppLang == L_ENGLISH)
  {
    //return EnglishMsgTable[index];
  }
}
