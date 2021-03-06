#include "TcpSocket.h"
#include "LogCore.h"

CTcpSocket::CTcpSocket(CQueue &q) : m_qInIO(q)
{
	m_bClosed = false;
	m_nSockId = 0;
	m_wHaveRecvLength = 0;
	m_wSendLength = 0;
}

CTcpSocket::~CTcpSocket()
{
	m_bClosed = false;
	m_nSockId = 0;
	m_wHaveRecvLength = 0;
	m_wSendLength = 0;
}

bool CTcpSocket::DoRecv()
{
	char *pRecvBuffer = m_szRecvBuffer + m_wHaveRecvLength;
	USHORT wFreeRecvLength = MAX_RECV_BUFFER_LENGTH - m_wHaveRecvLength;

	if (0 == wFreeRecvLength)
	{
		LOGError("单个消息包过长，断开连接。");

		// 单个消息包过长，断开连接
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

	// 黏包解析
	USHORT wReadLength = 0;	//解析长度修改都是完整消息包

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
		// 消息包头长度不足
		return 0;
	}

	USHORT wPacketLength = static_cast<USHORT>(*(USHORT*)(pPacketHead));
	
	if (sizeof(USHORT) + wPacketLength > wLength)
	{
		// 不足一个完整消息包
		return 0;
	}

	// 一个完整消息包
	const char *pPacketData = (pPacketHead + sizeof(USHORT));

	// 打印消息内容
	LOGDebug(pPacketData);

	// 接收数据
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

	// 断开连接
	m_pSock->close();

	m_bClosed = true;
	return true;
}

void CTcpSocket::OnClose()
{
	// 接收数据
	MsgData msgData;
	msgData.eMsgType = MsgData::EMsgType_Closed;
	msgData.nSockId = m_nSockId;

	m_qInIO.PushMsgData(msgData);
}

//bool CTcpSocket::DoSend()
//{
//	// 拷贝数据
//	std::list<MsgData>::iterator it = m_vtMsgData.begin();
//	for (; it!=m_vtMsgData.end(); )
//	{
//		MsgData &msgData = (*it);
//
//		if (MAX_SEND_BUFFER_LENGTH - m_wSendLength >= msgData.wLength)
//		{
//			USHORT *pPacketLength = (USHORT *)m_szSendBuffer;
//			*pPacketLength = msgData.wLength;
//			m_wSendLength += sizeof(USHORT);
//			memcpy(m_szSendBuffer + m_wSendLength, msgData.szData, msgData.wLength);
//			m_wSendLength += msgData.wLength;
//
//			it = m_vtMsgData.erase(it);
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	if (m_wSendLength > 0)
//	{
//		// 发送数据
//		m_pSock->async_write_some(buffer(m_szSendBuffer, m_wSendLength), 
//			bind(&CTcpSocket::OnSend, this, placeholders::error, placeholders::bytes_transferred));
//	}
//
//	return true;
//}

bool CTcpSocket::DoSend( MsgData msgData )
{
	if (m_wSendLength > 0)
	{
		// 正在发送数据
		m_listMsgData.push_back(msgData);

		return true;
	}
	else
	{
		// 拷贝数据并发送
		if (msgData.wLength > MAX_SEND_BUFFER_LENGTH)
		{
			LOGError("发送数据长度越界。");
			return false;
		}

		USHORT *pPacketLength = (USHORT *)m_szSendBuffer;
		*pPacketLength = msgData.wLength;
		m_wSendLength += sizeof(USHORT);
		memcpy(m_szSendBuffer + m_wSendLength, msgData.szData, msgData.wLength);
		m_wSendLength += msgData.wLength;

		// 发送数据
		m_pSock->async_write_some(buffer(m_szSendBuffer, m_wSendLength), 
			bind(&CTcpSocket::OnSend, this, placeholders::error, placeholders::bytes_transferred));

		return true;
	}
}

void CTcpSocket::OnSend( const system::error_code &ec, size_t nByteTransferred )
{
	if (ec)
	{
		LOGError(ec.message().c_str());

		DoClose();
		return ;
	}

	m_wSendLength -= nByteTransferred;

	if (m_wSendLength > 0)
	{
		LOGError("理论来讲，这里是进不来的！");

		// 理论来讲，这里是进不来的。
		memcpy(m_szSendBuffer, m_szSendBuffer + nByteTransferred, m_wSendLength);

		// 发送数据
		m_pSock->async_write_some(buffer(m_szSendBuffer, m_wSendLength), 
			bind(&CTcpSocket::OnSend, this, placeholders::error, placeholders::bytes_transferred));
	}
	else if (0 == m_wSendLength)
	{
		// 拷贝数据
		std::list<MsgData>::iterator it = m_listMsgData.begin();
		for (; it!=m_listMsgData.end(); )
		{
			MsgData &msgData = (*it);

			if (MAX_SEND_BUFFER_LENGTH - m_wSendLength >= msgData.wLength)
			{
				USHORT *pPacketLength = (USHORT *)m_szSendBuffer;
				*pPacketLength = msgData.wLength;
				m_wSendLength += sizeof(USHORT);
				memcpy(m_szSendBuffer + m_wSendLength, msgData.szData, msgData.wLength);
				m_wSendLength += msgData.wLength;

				it = m_listMsgData.erase(it);
			}
			else
			{
				break;
			}
		}

		// 发送数据
		m_pSock->async_write_some(buffer(m_szSendBuffer, m_wSendLength), 
			bind(&CTcpSocket::OnSend, this, placeholders::error, placeholders::bytes_transferred));
	}
	else
	{
		LOGError("异常错误！");
	}
}

//bool CTcpSocket::PushMsgData( MsgData msgData )
//{
//	m_vtMsgData.push_back(msgData);
//
//	return true;
//}

