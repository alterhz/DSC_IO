
#ifndef _INETIO_
#define _INETIO_

class IDispatcher
{
public:
	virtual bool OnConnect(int nSockId) = 0;
	virtual bool OnRecvData(int nSockId, const char *pData, unsigned int nLength) = 0;
	virtual bool OnClose(int nSockId) = 0;
};

class INetIO
{
public:
	static INetIO * CreateNetIO();
	void Release();

public:
	virtual bool SetPort(unsigned short wPort) = 0;
	virtual bool SetDispatcher(IDispatcher *pDispatcher) = 0;
	virtual bool Start() = 0;
	virtual bool ShutDown() = 0;
	virtual bool SendData(int nSockId, const char *pData, unsigned int nLength) = 0;
	virtual bool Disconnect(int nSockId) = 0;

};

#endif