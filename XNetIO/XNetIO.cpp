// XNetIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_ALL_NO_LIB

// header
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// source
#include <libs/system/src/error_code.cpp>

using namespace boost;
using namespace boost::asio;

#include <iostream>

void Time_handler(const boost::system::error_code &ec)
{
	std::cout << "hello world" << std::endl;


}

int _tmain(int argc, _TCHAR* argv[])
{
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, posix_time::seconds(5));

	t.async_wait(Time_handler);

	io.run();


	return 0;
}

