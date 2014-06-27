//  [6/27/2014 Administrator]

#ifndef _TCPSOCKET_
#define _TCPSOCKET_

#include "Header.h"
#include "INetWork.h"

typedef shared_ptr<ip::tcp::socket> sock_pt;

class CTcpSocket
{
public:
	CTcpSocket();
	~CTcpSocket();

	void SetSock(sock_pt pSock) { m_pSock = pSock; }
	void SetSockId(int nSockId) { m_nSockId = nSockId; }
	int GetSockId() const { return m_nSockId; }

public:
	bool DoRecv();
	void OnRecv(const system::error_code& ec);

	//bool DoSend();
	//bool OnSend();

private:
	int	m_nSockId;
	sock_pt m_pSock;
	char m_szRecvBuffer[0x2000];

};

#endif