
#ifndef _HEADER_
#define _HEADER_

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif

#define BOOST_ALL_NO_LIB
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace boost;
using namespace boost::asio;

#include <iostream>

// end
#endif