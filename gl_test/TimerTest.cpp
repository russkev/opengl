#pragma once
#include "pch.h"
#include <boost/test/unit_test.hpp>
#include <windows.h> 
#include <string>
#include "../gl_engine/Timer.h"
#include "../gl_engine/Timer.cpp"

namespace tt = boost::test_tools;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(timerTests)

BOOST_AUTO_TEST_CASE(normalSecond, * utf::tolerance(0.01))
{
	Timer timer;
	Sleep(1000);
	timer.update();
	BOOST_TEST(timer.delta_time_ms() == 1000.0);
	BOOST_TEST(timer.delta_time_s() == 1.0);
}

BOOST_AUTO_TEST_CASE(nonNormalSpeed, * utf::tolerance(0.01))
{
	Timer timer(0.1);
	Sleep(1000);
	timer.update();
	BOOST_TEST(timer.delta_time_ms() == 100.0);
	BOOST_TEST(timer.delta_time_s() == 0.1);
	timer.setMultiplier(10.0);
	Sleep(1000);
	timer.update();
	BOOST_TEST(timer.delta_time_ms() == 10000.0);
	BOOST_TEST(timer.delta_time_s() == 10.0);
}

BOOST_AUTO_TEST_CASE(totalTime, * utf::tolerance(0.01))
{
	Timer timer;
	Sleep(500);
	timer.update();
	Sleep(500);
	timer.update();
	Sleep(500);
	timer.update();
	Sleep(500);	
	timer.update();
	BOOST_TEST(timer.total_time_ms() == 2000.0);
	BOOST_TEST(timer.total_time_s() == 2.0);
}

BOOST_AUTO_TEST_CASE(fps, *utf::tolerance(0.01))
{
	Timer timer;
	Sleep(100);
	timer.update();
	Sleep(100);
	timer.update();
	std::string fps_string = std::string(timer.fps());
	float fps_float = std::stof(fps_string);
	BOOST_TEST(fps_float == 10.0);
}


BOOST_AUTO_TEST_SUITE_END()
