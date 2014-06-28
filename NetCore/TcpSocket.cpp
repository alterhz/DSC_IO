#include "TcpSocket.h"
#include "LogCore.h"

CTcpSocket::CTcpSocket(CQueue &q) : m_qInIO(q)
{
	m_bClosed = false;
}

CTcpSocket::~CTcpSocket()
{
	m_bClosed = false;
}

bool CTcpSocket::DoRecv()
{
	m_pSock->async_read_some(buffer(m_szRecvBuffer), 
		bind(&CTcpSocket::OnRecv, this, placeholders::error));

	return true;
}

void CTcpSocket::OnRecv( const system::error_code& ec )
{
	if (ec)
	{
		DoClose();
		return ;
	}

	LOGInfo(m_szRecvBuffer);

	{
		// ��������
		MsgData msgData;
		msgData.eMsgType = MsgData::EMsgType_Message;
		msgData.nSockId = m_nSockId;

		m_qInIO.PushMsgData(msgData);
	}
	
	DoRecv();
}

bool CTcpSocket::DoClose()
{
	if (m_bClosed)
	{
		return true;
	}

	// �Ͽ�����
	m_pSock->close();

	m_bClosed = true;
	return true;
}

void CTcpSocket::OnClose()
{
	// ��������
	MsgData msgData;
	msgData.eMsgType = MsgData::EMsgType_Closed;
	msgData.nSockId = m_nSockId;

	m_qInIO.PushMsgData(msgData);
}

bool CTcpSocket::DoSend()
{
	// ����Ƿ��д���������
	bool bNeedSend = false;



	if (bNeedSend)
	{
		// ��������
		m_pSock->async_write_some(buffer(m_szSendBuffer), 
			bind(&CTcpSocket::OnSend, this, placeholders::error));
	}

	return true;
}

void CTcpSocket::OnSend( const system::error_code &ec )
{
	if (ec)
	{
		DoClose();
		return ;
	}


	DoSend();
}
