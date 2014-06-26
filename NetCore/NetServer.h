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
	virtual bool SendData(const void *pData, unsigned int nLength);
	virtual bool Disconnect(int nSockId);
	virtual bool DoTick(unsigned long nElapsedTime);

private:
	typedef shared_ptr<ip::tcp::socket> sock_pt;
	typedef ip::tcp::socket TcpSocket;

	void ThreadNetRun();

private:
	bool DoAccept();
	void OnAccept(const system::error_code& ec, TcpSocket *pTcpSocket);

	bool DoRecv(TcpSocket *pTcpSocket);
	void OnRecv(const system::error_code& ec, TcpSocket *pTcpSocket);



private:
	IDispatcher *m_pDispatcher;
	io_service *m_pIos;
	unsigned short m_wPort;
	ip::tcp::acceptor *m_pAcceptor;
	unsigned short m_wMaxPlayer;

	std::map<int, TcpSocket *> m_mapTcpSocket;

	char m_szRecvBuffer[0x2000];
	unsigned int m_nRecvLength;


};

#endif