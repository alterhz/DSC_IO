#include "TcpSocket.h"
#include "LogCore.h"

CTcpSocket::CTcpSocket(CQueue &q) : m_qInIO(q)
{
	m_bClosed = false;
	m_nSockId = 0;
	m_wHaveRecvLength = 0;
	m_wHaveSendLength = 0;
}

CTcpSocket::~CTcpSocket()
{
	m_bClosed = false;
	m_nSockId = 0;
	m_wHaveRecvLength = 0;
	m_wHaveSendLength = 0;
}

bool CTcpSocket::DoRecv()
{
	char *pRecvBuffer = m_szRecvBuffer + m_wHaveRecvLength;
	USHORT wFreeRecvLength = MAX_RECV_BUFFER_LENGTH-m_wHaveRecvLength;

	if (0 == wFreeRecvLength)
	{
		LOGError("������Ϣ���������Ͽ����ӡ�");

		// ������Ϣ���������Ͽ�����
		DoClose();
		
		return false;
	}

	m_pSock->async_read_some(buffer(pRecvBuffer, wFreeRecvLength), 
		bind(&CTcpSocket::OnRecv, this, placeholders::error, placeholders::bytes_transferred));

	return true;
}

void CTcpSocket::OnRecv( const system::error_code& ec, size_t nByteTransferred )
{
	if (ec)
	{
		LOGError(ec.message().c_str());

		DoClose();
		return ;
	}

	m_wHaveRecvLength += nByteTransferred;

	// ������
	USHORT wReadLength = 0;	//���������޸Ķ���������Ϣ��

	while (true)
	{
		const char *pPacketHead = m_szRecvBuffer + wReadLength;
		USHORT wRemainLength = m_wHaveRecvLength - wReadLength;

		USHORT wReadPacketLength = ReadPacket(pPacketHead, wRemainLength);
		if (wReadPacketLength > 0)
		{
			wReadLength += wReadPacketLength;
		}
		else
		{
			if ((wRemainLength > 0) 
				&& (wRemainLength != m_wHaveRecvLength))
			{
				memcpy(m_szRecvBuffer, m_szRecvBuffer+wReadLength, wRemainLength);
			}
			
			m_wHaveRecvLength = wRemainLength;
			break;
		}
	}

	DoRecv();
}

USHORT CTcpSocket::ReadPacket( const char *pPacketHead, unsigned short wLength )
{
	if (wLength < sizeof(USHORT))
	{
		// ��Ϣ��ͷ���Ȳ���
		return 0;
	}

	USHORT wPacketLength = static_cast<USHORT>(*(USHORT*)(pPacketHead));
	
	if (sizeof(USHORT) + wPacketLength > wLength)
	{
		// ����һ��������Ϣ��
		return 0;
	}

	// һ��������Ϣ��
	const char *pPacketData = (pPacketHead + sizeof(USHORT));

	// ��ӡ��Ϣ����
	LOGDebug(pPacketData);

	// ��������
	MsgData msgData;
	msgData.eMsgType = MsgData::EMsgType_Message;
	msgData.nSockId = m_nSockId;
	memcpy(msgData.szData, pPacketData, wPacketLength);
	msgData.wLength = wPacketLength;

	m_qInIO.PushMsgData(msgData);

	return sizeof(USHORT) + wPacketLength;
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
			bind(&CTcpSocket::OnSend, this, placeholders::error, placeholders::bytes_transferred));
	}

	return true;
}

void CTcpSocket::OnSend( const system::error_code &ec, size_t nByteTransferred )
{
	if (ec)
	{
		LOGError(ec.message().c_str());

		DoClose();
		return ;
	}


	DoSend();
}

