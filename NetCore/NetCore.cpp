// NetCore.cpp : �������̨Ӧ�ó������ڵ㡣
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
	INetEngine *pNetEngine = INetEngine::CreateNetEngine();
	if (!pNetEngine)
	{
		return -1;
	}


	pNetEngine->SetPort(8000);
	pNetEngine->Start();

	while(1);

	return 0;
}

