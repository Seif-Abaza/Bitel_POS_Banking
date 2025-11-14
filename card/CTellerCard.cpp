//---------------------------------------------------------------------------
/****************************************************************************
* История изменений:
* $Log: CTellerCard.cpp,v $
* Revision 1.3  2010/06/09 06:04:42  rakhmatulin
* no message
*
*
***************************************************************************/
//---------------------------------------------------------------------------

#include "CTellerCard.h"
#include "../internal/tools.h"

#include <infra/config.h>
#include <infra/error.h>
#include <hal/usip/hal_apm.h>
#include <hal/usip/hal_scs.h>
#include <devs/sc_at83c26.h>
#include <libemv.h>
