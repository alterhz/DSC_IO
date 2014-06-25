#include "NetServer.h"

CNetServer::CNetServer()
{
	m_pDispatcher = NULL;
}

CNetServer::~CNetServer()
{
	m_pDispatcher = NULL;
}

bool CNetServer::RegDispatcher( IDispatcher *pDispatcher )
{
	return true;
}

bool CNetServer::SetPort( unsigned short wPort )
{
	return true;
}

bool CNetServer::SetMaxPlayer( unsigned short wMaxPlayer )
{
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
