//////////////////////////////////////////////////////////////////////////
// ÍøÂç¿â

#ifndef _INETWORK_
#define _INETWORK_

class IDispatcher
{
public:
	bool OnAccept(int nSockId);
	bool OnRecvPackage(int nSockId, const void *pData, unsigned int nLength);
	bool OnClose(int nSockId);

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

};

#endif