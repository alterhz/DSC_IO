#include "NetServer.h"
#include "LogCore.h"


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
	while(true)
	{
		MsgData msgData;
		if (m_qInIO.PrepareMsgData(msgData))
		{
			switch (msgData.eMsgType)
			{
			case MsgData::EMsgType_Connect:
				{
					LOGInfo("新连接接入。");
					if (m_pDispatcher)
					{
						m_pDispatcher->OnConnected(msgData.nSockId);
					}
				}
				break;
			case MsgData::EMsgType_Closed:
				{
					LOGInfo("连接关闭。");
					if (m_pDispatcher)
					{
						m_pDispatcher->OnClosed(msgData.nSockId);
					}
				}
				break;
			case MsgData::EMsgType_Message:
				{
					LOGInfo("接收到消息。");
					if (m_pDispatcher)
					{
						m_pDispatcher->OnRecvData(msgData.nSockId, msgData.szData, msgData.nLength);
					}
				}
				break;
			default:
				;
			}

			m_qInIO.RemoveFrontMsgData();
		}
		else
		{
			break;
		}
	}

	return true;
}

void CNetServer::ThreadNetRun()
{
	m_pIos = new io_service();

	m_pTimerSend = new deadline_timer(*m_pIos, posix_time::microsec(ThreadFrameTick));

	DoTimerRun();

	// tcpsocket接收器
	m_pAcceptor = new ip::tcp::acceptor(*m_pIos, ip::tcp::endpoint(ip::tcp::v4(), m_wPort));

	DoAccept();

	m_pIos->run();
	
	//while (1)
	//{
	//	m_pIos->run_one();
	//}

	LOGInfo("网络线程退出！");
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
		LOGError(ec.message().c_str());
		return ;
	}

	LOGInfo("接收连接");

	static int sTcpSocketIndex = 0;

	CTcpSocket_pt pNewTcpSocket(new CTcpSocket(m_qInIO));
	pNewTcpSocket->SetSockId(sTcpSocketIndex);
	pNewTcpSocket->SetSock(pSock);

	m_mapTcpSocket.insert(std::make_pair(pNewTcpSocket->GetSockId(), pNewTcpSocket));

	// 投递接收连接事件
	{
		MsgData msgData;
		msgData.eMsgType = MsgData::EMsgType_Connect;
		msgData.nSockId = pNewTcpSocket->GetSockId();

		m_qInIO.PushMsgData(msgData);
	}
		
	// 投递异步接收数据
	pNewTcpSocket->DoRecv();

	// 再次接收连接
	DoAccept();
}

bool CNetServer::DoTimerRun()
{
	// 发送线程计时器
	m_pTimerSend->expires_at(m_pTimerSend->expires_at() + posix_time::millisec(ThreadFrameTick));

	m_pTimerSend->async_wait(bind(&CNetServer::OnTimerRun, this, placeholders::error));

	return true;
}

void CNetServer::OnTimerRun( const system::error_code& ec )
{
	if (ec)
	{
		LOGError(ec.message().c_str());
		return ;
	}

	DoTimerRun();

	// 先执行事件投递，避免下面逻辑影响逻辑执行次数
	LOGInfo("已经过了3秒了...");

	OnRunClose();

	OnRunSend();
}

void CNetServer::OnDispatchSendData()
{
	while (1)
	{
		MsgData msgData;
		if (m_qOutIO.PrepareMsgData(msgData))
		{
			switch (msgData.eMsgType)
			{
			case MsgData::EMsgType_Connect:
				break;
			case MsgData::EMsgType_Closed:
				{
					CTcpSocket_pt pTcpSocket = FindTcpSocket(msgData.nSockId);
					if (pTcpSocket.get())
					{
						pTcpSocket->DoClose();
					}
				}
				break;
			case MsgData::EMsgType_Message:
				{
					CTcpSocket_pt pTcpSocket = FindTcpSocket(msgData.nSockId);
					if (pTcpSocket.get())
					{
						// 压入数据

					}
				}
				break;
			}

			m_qOutIO.RemoveFrontMsgData();
		}
	}
}

void CNetServer::OnRunSend()
{
	// 处理发送
	TcpSocketList::iterator it = m_mapTcpSocket.begin();
	for (; it!=m_mapTcpSocket.end(); ++it)
	{
		CTcpSocket_pt pTcpSocket = it->second;

		// 发送开始
		pTcpSocket->DoSend();
	}
}

void CNetServer::OnRunClose()
{
	TcpSocketList::iterator it = m_mapTcpSocket.begin();
	for (; it!=m_mapTcpSocket.end(); )
	{
		CTcpSocket_pt pTcpSocket = it->second;

		if (pTcpSocket->IsClosed())
		{
			pTcpSocket->OnClose();

			it = m_mapTcpSocket.erase(it);
		}
		else
		{
			++it;
		}
	}
}

CNetServer::CTcpSocket_pt CNetServer::FindTcpSocket( int nSockId )
{
	TcpSocketList::iterator it = m_mapTcpSocket.find(nSockId);
	if (it != m_mapTcpSocket.end())
	{
		return it->second;
	}

	return NULL;
}

