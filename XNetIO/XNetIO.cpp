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

	sock.write_some(buffer("hello world!"));

	ios.run();

	while(1);

	return 0;
}

