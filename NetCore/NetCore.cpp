// NetCore.cpp : 定义控制台应用程序的入口点。
//

#include "NetServer.h"
#include "ServerTest.h"


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
	CServerTest *pServerTest = new CServerTest;

	INetEngine *pNetEngine = INetEngine::CreateNetEngine();
	if (!pNetEngine)
	{
		return -1;
	}

	pServerTest->SetNetEngine(pNetEngine);

	pNetEngine->RegDispatcher(pServerTest);
	pNetEngine->SetPort(9000);
	pNetEngine->Start();

	while(1)
	{
		system_time &tNow = boost::get_system_time();
		boost::thread::sleep( tNow + boost::posix_time::microsec(100)); 

		pNetEngine->DoTick(1);
	}

	return 0;
}

