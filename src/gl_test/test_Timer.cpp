#include "pch.h"
#include "CppUnitTest.h"

#include "utils/Timer.h"
#include "utils/Timer.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Timer)
	{

	public:
		TEST_METHOD(normalSecond)
		{
			glen::Timer timer;
			Sleep(1000);
			timer.update();
			Assert::AreEqual(1000.0, timer.delta_time_ms(), 1000*0.1);
			Assert::AreEqual(1.0, timer.delta_time_s(), 0.1);
		}

		TEST_METHOD(nonNormalSpeed)
		{
			glen::Timer timer(0.1);
			Sleep(1000);
			timer.update();
			Assert::AreEqual(100.0, timer.delta_time_ms(), 100*0.1);
			Assert::AreEqual(0.1, timer.delta_time_s(), 0.1*0.1);
			timer.set_multiplier(10.0);
			Sleep(1000);
			timer.update();
			Assert::AreEqual(10000.0, timer.delta_time_ms(), 10000.0*0.1);
			Assert::AreEqual(10.0, timer.delta_time_s(), 10.0*0.1);
		}

		TEST_METHOD(totalTime)
		{
			glen::Timer timer;
			Sleep(500);
			timer.update();
			Sleep(500);
			timer.update();
			Sleep(500);
			timer.update();
			Sleep(500);
			timer.update();
			Assert::AreEqual(2000.0, timer.total_time_ms(), 2000*0.1);
			Assert::AreEqual(2.0, timer.total_time_s(), 2.0*0.1);
		}

		TEST_METHOD(fps)
		{
			glen::Timer timer;
			Sleep(100);
			timer.update();
			Sleep(100);
			timer.update();
			std::string fps_string = std::string(timer.fps());
			float fps_float = std::stof(fps_string);
			Assert::AreEqual(10.0f, fps_float, 10.0f*0.1f);
		}
	};
}