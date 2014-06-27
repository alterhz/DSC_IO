//////////////////////////////////////////////////////////////////////////
// 服务器端网络实现

#ifndef _NETSERVER_
#define _NETSERVER_


#include "TcpSocket.h"

class CNetServer : public INetEngine
{
	typedef shared_ptr<CTcpSocket> CTcpSocket_pt;

public:
	CNetServer();
	~CNetServer();

public:
	virtual bool RegDispatcher(IDispatcher *pDispatcher);
	virtual bool SetPort(unsigned short wPort);
	virtual bool SetMaxPlayer(unsigned short wMaxPlayer);
	virtual bool Start();
	virtual bool ShutDown();
	virtual bool SendData(const void *pData, unsigned int nLength);
	virtual bool Disconnect(int nSockId);
	virtual bool DoTick(unsigned long nElapsedTime);

private:


	void ThreadNetRun();

private:
	bool DoAccept();
	void OnAccept(const system::error_code& ec, sock_pt pSock);



private:
	IDispatcher *m_pDispatcher;
	io_service *m_pIos;
	unsigned short m_wPort;
	ip::tcp::acceptor *m_pAcceptor;
	unsigned short m_wMaxPlayer;

	std::map<int, CTcpSocket_pt> m_mapTcpSocket;

	char m_szRecvBuffer[0x2000];
	unsigned int m_nRecvLength;


};

#endif