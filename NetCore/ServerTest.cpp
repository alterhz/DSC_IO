#include "ServerTest.h"

bool CServerTest::OnConnected( int nSockId )
{
	LOGDebug("[main]����socket���ӡ�");


	return true;
}

bool CServerTest::OnRecvData( int nSockId, const char *pRecvData, unsigned short wLength )
{
	LOGDebug("[main]����socket���ݡ�");
	LOGDebug(pRecvData);

	// �ط����ݲ��ԡ�
	char szSend[0x2000] = "very good!";
	m_pNetEngine->SendData(nSockId, szSend, strlen(szSend)+1);

	return true;
}

bool CServerTest::OnClosed( int nSockId )
{
	LOGDebug("[main]�Ͽ�socket���ӡ�");

	return true;
}