
#ifndef CREADKEY_H
#define CREADKEY_H

#include "../internal/CParam.h"
#include "CTaskDisplay.h"

class CTaskReadKey
{
public:
	void	run()	
	{	g_pParam->readKey();	
	}
};

#endif
