//////////////////////////////////////////////////////////////////////////
// 服务器端网络实现

#ifndef _NETSERVER_
#define _NETSERVER_


#include "TcpSocket.h"
#include "Queue.h"

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
	virtual bool SendData(int nSockId, const void *pData, unsigned short wLength);
	virtual bool Disconnect(int nSockId);
	virtual bool DoTick(unsigned long nElapsedTime);

private:
	void ThreadNetRun();

private:
	bool DoAccept();
	void OnAccept(const system::error_code& ec, sock_pt pSock);

	// 处理断开连接
	bool DoClose(int nSockId);
	void OnClose();

	bool DoTimerRun();
	void OnTimerRun(const system::error_code& ec);

	void OnDispatchSendData();
	void OnRunSend();
	void OnRunClose();

	CTcpSocket_pt FindTcpSocket(int nSockId);

private:
	IDispatcher *m_pDispatcher;
	io_service *m_pIos;
	unsigned short m_wPort;
	ip::tcp::acceptor *m_pAcceptor;
	unsigned short m_wMaxPlayer;

	deadline_timer *m_pTimerSend;

	typedef std::map<int, CTcpSocket_pt> TcpSocketList;
	TcpSocketList m_mapTcpSocket;

	CQueue m_qInIO;
	CQueue m_qOutIO;

};

#endif