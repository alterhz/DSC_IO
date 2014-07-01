//  [7/1/2014 Administrator]

#ifndef _INETADAPTER_
#define _INETADAPTER_

#define DllClassExport	__declspec(dllexport)

typedef unsigned short PORT;

// 通信对象
class IPlayer
{
public:
	virtual ~IPlayer();

public:	//功能
	virtual bool Send(const void *pData, unsigned int nLength) = 0;
	virtual bool Close() = 0;
	virtual const char * GetRemoteIP() = 0;
	virtual PORT GetRemotePort() = 0;


public:	//事件
	virtual bool OnConnected() = 0;
	virtual bool OnRecvPacket(const void *pData, unsigned int nLength) = 0;
	virtual bool OnClosed() = 0;

};

class IPlayerManager
{
public:
	virtual ~IPlayerManager();

public:	//事件
	virtual IPlayer * OnCreatePlayer() = 0;
	virtual void OnDestory() = 0;

};

class INetAdapter
{
public:
	static INetAdapter * CreateNetAdapter();
	void Release();

public:
	virtual bool RegPlayerManager(IPlayerManager *pPlayerManager) = 0;
	virtual bool Start(PORT port, unsigned short wMaxPlayer = 64) = 0;
	virtual bool ShutDown() = 0;
	virtual bool DoTick(unsigned long nElapsed) = 0;

};

#endif