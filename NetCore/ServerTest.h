//  [7/1/2014 Administrator]
// ≤‚ ‘

#ifndef _SERVERTEST_
#define _SERVERTEST_

#include "INetWork.h"
#include "LogCore.h"

class CServerTest : public IDispatcher
{
public:
	virtual bool OnConnected(int nSockId);
	virtual bool OnRecvData(int nSockId, const void *pData, unsigned short wLength);
	virtual bool OnClosed(int nSockId);

	void SetNetEngine(INetEngine *pNetEngine) { m_pNetEngine = pNetEngine; }

private:
	INetEngine *m_pNetEngine;
};

#endif