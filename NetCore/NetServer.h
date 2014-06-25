//////////////////////////////////////////////////////////////////////////
// 服务器端网络实现

#ifndef _NETSERVER_
#define _NETSERVER_

#include "Header.h"
#include "INetWork.h"

class CNetServer : public INetEngine
{
public:
	CNetServer();
	~CNetServer();

public:
	virtual bool RegDispatcher(IDispatcher *pDispatcher);
	virtual bool SetPort(unsigned short wPort);
	virtual bool SetMaxPlayer(unsigned short wMaxPlayer);
	virtual bool Start();
	virtual bool ShutDown();

private:
	IDispatcher *m_pDispatcher;

};

#endif