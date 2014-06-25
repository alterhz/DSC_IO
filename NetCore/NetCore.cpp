// NetCore.cpp : 定义控制台应用程序的入口点。
//

#include "NetServer.h"



INetEngine * INetEngine::CreateNetEngine()
{
	CNetServer *pNetServer = new CNetServer;

	return pNetServer;
}

void INetEngine::Release()
{
	CNetServer *pNetServer = dynamic_cast<CNetServer *>(this);

	delete pNetServer;
	pNetServer = NULL;
}

int main(int argc, char* argv[])
{
	return 0;
}

