//////////////////////////////////////////////////////////////////////////
// ÍøÂç¿â

#ifndef _INETWORK_
#define _INETWORK_

class IDispatcher
{
public:
	virtual bool OnConnected(int nSockId) = 0;
	virtual bool OnRecvData(int nSockId, const void *pData, unsigned short nLength) = 0;
	virtual bool OnClosed(int nSockId) = 0;

};

class INetEngine
{
public:
	static INetEngine * CreateNetEngine();
	void Release();

public:
	virtual bool RegDispatcher(IDispatcher *pDispatcher) = 0;
	virtual bool SetPort(unsigned short wPort) = 0;
	virtual bool SetMaxPlayer(unsigned short wMaxPlayer) = 0;
	virtual bool Start() = 0;
	virtual bool ShutDown() = 0;
	virtual bool SendData(const void *pData, unsigned int nLength) = 0;
	virtual bool Disconnect(int nSockId) = 0;
	virtual bool DoTick(unsigned long nElapsedTime) = 0;

};

#endif