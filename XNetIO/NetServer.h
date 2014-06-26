//  [6/26/2014 Administrator]

#ifndef _NETSERVER_
#define _NETSERVER_

#include "Header.h"
#include "INetIO.h"

class CNetServer : public INetIO
{
public:
	CNetServer();
	~CNetServer();

public:
	virtual bool SetPort(unsigned short wPort);
	virtual bool SetDispatcher(IDispatcher *pDispatcher);
	virtual bool Start();
	virtual bool ShutDown();
	virtual bool SendData(int nSockId, const char *pData, unsigned int nLength);
	virtual bool Disconnect(int nSockId);

private:
	unsigned short m_wPort;
	IDispatcher *m_pDispatcher;

};

#endif