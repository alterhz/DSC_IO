#include "NetServer.h"

CNetServer::CNetServer()
{
	m_wPort = 0;
	m_pDispatcher = NULL;
}

CNetServer::~CNetServer()
{
	m_wPort = 0;
	m_pDispatcher = NULL;
}

bool CNetServer::SetPort( unsigned short wPort )
{
	m_wPort = wPort;
	return true;
}

bool CNetServer::SetDispatcher( IDispatcher *pDispatcher )
{
	if (!pDispatcher)
	{
		return false;
	}

	m_pDispatcher = pDispatcher;

	return true;
}

bool CNetServer::Start()
{
	return true;
}

bool CNetServer::ShutDown()
{
	return true;
}

bool CNetServer::SendData(int nSockId, const char *pData, unsigned int nLength)
{
	return true;
}

bool CNetServer::Disconnect(int nSockId)
{
	return true;
}