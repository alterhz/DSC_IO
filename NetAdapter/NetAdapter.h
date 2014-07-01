//  [7/1/2014 Administrator]

#ifndef _NETADAPTER_
#define _NETADAPTER_

#include "INetWork.h"

class CNetAdapater : public INetAdapter
{
public:
	CNetAdapater();
	~CNetAdapater();

	virtual bool RegPlayerManager(IPlayerManager *pPlayerManager);
	virtual bool Start(PORT port, unsigned short wMaxPlayer = 64);
	virtual bool ShutDown();
	virtual bool DoTick(unsigned long nElapsed);

private:
	INetEngine *m_pNetEngine;


};

#endif