#include "TcpSocket.h"

CTcpSocket::CTcpSocket()
{

}

CTcpSocket::~CTcpSocket()
{

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
		return ;
	}

	

	DoRecv();
}
