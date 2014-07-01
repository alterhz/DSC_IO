//  [6/28/2014 Administrator]

#ifndef _LOGCORE_
#define	_LOGCORE_

#include "Header.h"

#include <assert.h>

class CLogCore
{
public:
	void Log(const char *pTypeName, const char *pLog)
	{
		std::cout << "[" << pTypeName << "] - " << pLog << std::endl;
	}
};

#define LOGDebug(log) {	\
	CLogCore logCore;	\
	logCore.Log("DEBUG", log);	\
}

#define LOGError(log) {	\
	CLogCore logCore;	\
	logCore.Log("ERROR", log);	\
	assert(0);	\
}

#define LOGInfo(log) {	\
	CLogCore logCore;	\
	logCore.Log("INFO", log);	\
}

#endif