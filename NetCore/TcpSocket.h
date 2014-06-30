//  [6/27/2014 Administrator]

#ifndef _TCPSOCKET_
#define _TCPSOCKET_

#include "Header.h"
#include "INetWork.h"
#include "Queue.h"

typedef shared_ptr<ip::tcp::socket> sock_pt;

#define MAX_RECV_BUFFER_LENGTH		(0x2000)
#define MAX_SEND_BUFFER_LENGTH		(0x2000)

typedef unsigned short USHORT;

class CTcpSocket
{
public:
	CTcpSocket(CQueue &q);
	~CTcpSocket();

	void SetSock(sock_pt pSock) { m_pSock = pSock; }
	void SetSockId(int nSockId) { m_nSockId = nSockId; }
	int GetSockId() const { return m_nSockId; }
	bool IsClosed() const { return m_bClosed; }

public:
	bool DoRecv();
	bool DoSend();
	bool DoClose();
	
public:
	// 事件
	void OnRecv(const system::error_code& ec, size_t nByteTransferred);
	void OnSend(const system::error_code &ec, size_t nByteTransferred);
	void OnClose();

private:
	// 解析消息包
	USHORT ReadPacket(const char *pPacketHead, unsigned short wLength);

private:
	bool m_bClosed;
	int	m_nSockId;
	sock_pt m_pSock;
	char m_szRecvBuffer[MAX_RECV_BUFFER_LENGTH];
	USHORT m_wHaveRecvLength;
	char m_szSendBuffer[MAX_SEND_BUFFER_LENGTH];
	USHORT m_wHaveSendLength;

	CQueue &m_qInIO;

};

#endif