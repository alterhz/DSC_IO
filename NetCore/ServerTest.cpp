#include "ServerTest.h"

bool CServerTest::OnConnected( int nSockId )
{
	LOGDebug("[main]建立socket连接。");


	return true;
}

bool CServerTest::OnRecvData( int nSockId, const void *pData, unsigned short wLength )
{
	LOGDebug("[main]接收socket数据。");

	// 回发数据测试。
	char szSend[0x2000] = "very good!";
	m_pNetEngine->SendData(nSockId, szSend, strlen(szSend)+1);

	return true;
}

bool CServerTest::OnClosed( int nSockId )
{
	LOGDebug("[main]断开socket连接。");

	return true;
}