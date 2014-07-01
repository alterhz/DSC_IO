// XNetIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_ALL_NO_LIB
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace boost;
using namespace boost::asio;

#include <iostream>


#include <libs/system/src/error_code.cpp>

#ifdef _MSC_VER
namespace boost { void tss_cleanup_implemented(void) {}} 
#include <libs/thread/src/win32/thread.cpp>
#include <libs/thread/src/win32/tss_dll.cpp>
#include <libs/thread/src/win32/tss_pe.cpp>
#else
#include <libs/thread/src/pthread/thread.cpp>
#include <libs/thread/src/pthread/once.cpp>
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	io_service ios;
	ip::tcp::socket sock(ios);

	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 9000);

	sock.connect(ep);

	char szSendBuffer[0x2000] = {0};
	unsigned short wSendLength = 0;

	// 第一个包
	char *pPacketHead = (szSendBuffer + wSendLength);
	unsigned short *pPacketLength = (unsigned short *)pPacketHead;
	*pPacketLength = strlen("hello world") + 1;
	memcpy(pPacketHead+sizeof(unsigned short), "hello world", strlen("hello world")+1);

	wSendLength += sizeof(unsigned short) + strlen("hello world") + 1;

	// 第二个包
	pPacketHead = (szSendBuffer + wSendLength);
	pPacketLength = (unsigned short *)pPacketHead;
	*pPacketLength = strlen("good bye") + 1;
	memcpy(pPacketHead+sizeof(unsigned short), "good bye", strlen("good bye")+1);

	wSendLength += sizeof(unsigned short) + strlen("good bye") + 1;


	unsigned short wHaveSendLength = 0;

	char *pSendData = szSendBuffer + wHaveSendLength;
	sock.write_some(buffer(pSendData, 1));
	wHaveSendLength += 1;

	for (int i=0; i<10; ++i)
	{
		ios.run_one();
	}
	
	
	pSendData = szSendBuffer + wHaveSendLength;
	sock.write_some(buffer(pSendData, 5));
	wHaveSendLength += 5;

	for (int i=0; i<10; ++i)
	{
		ios.run_one();
	}

	pSendData = szSendBuffer + wHaveSendLength;
	sock.write_some(buffer(pSendData, wSendLength-wHaveSendLength));

	for (int i=0; i<10; ++i)
	{
		ios.run_one();
	}

	return 0;
}

