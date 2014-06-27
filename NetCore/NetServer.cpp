#include "NetServer.h"

CNetServer::CNetServer()
{
	m_pDispatcher = NULL;
	m_pIos = NULL;
	m_pAcceptor = NULL;
	m_wPort = 0;
	m_wMaxPlayer = 0;
}

CNetServer::~CNetServer()
{
	m_pDispatcher = NULL;
	m_pIos = NULL;
	m_pAcceptor = NULL;
	m_wPort = 0;
	m_wMaxPlayer = 0;
}

bool CNetServer::RegDispatcher( IDispatcher *pDispatcher )
{
	if (!pDispatcher)
	{
		return false;
	}

	m_pDispatcher = pDispatcher;

	return true;
}

bool CNetServer::SetPort( unsigned short wPort )
{
	m_wPort = wPort;

	return true;
}

bool CNetServer::SetMaxPlayer( unsigned short wMaxPlayer )
{
	m_wMaxPlayer = wMaxPlayer;

	return true;
}

bool CNetServer::Start()
{
	

	// 启动线程
	thread t(bind(&CNetServer::ThreadNetRun, this));

	t.detach();

	return true;
}

bool CNetServer::ShutDown()
{
	return true;
}

bool CNetServer::SendData( const void *pData, unsigned int nLength )
{
	return true;
}

bool CNetServer::Disconnect( int nSockId )
{
	return true;
}

bool CNetServer::DoTick( unsigned long nElapsedTime )
{
	return true;
}

void CNetServer::ThreadNetRun()
{
	m_pIos = new io_service();

	m_pAcceptor = new ip::tcp::acceptor(*m_pIos, ip::tcp::endpoint(ip::tcp::v4(), m_wPort));

	DoAccept();

	m_pIos->run();
}

bool CNetServer::DoAccept()
{
	sock_pt pNewSock(new ip::tcp::socket(*m_pIos));
	
	m_pAcceptor->async_accept(*pNewSock,
		bind(&CNetServer::OnAccept, this, placeholders::error, pNewSock));

	return true;
}

void CNetServer::OnAccept( const system::error_code& ec, sock_pt pSock )
{
	if (ec)
	{
		return ;
	}

	// 再次接收连接
	DoAccept();

	std::cout << "接收连接" << std::endl;

	static int sTcpSocketIndex = 0;

	CTcpSocket_pt pNewTcpSocket(new CTcpSocket());
	pNewTcpSocket->SetSockId(sTcpSocketIndex);
	pNewTcpSocket->SetSock(pSock);

	m_mapTcpSocket.insert(std::make_pair(pNewTcpSocket->GetSockId(), pNewTcpSocket));

	// 投递接收连接事件


	// 投递异步接收数据
	pNewTcpSocket->DoRecv();
}

