#include "Timer.h"

#include <stdio.h>

namespace gl_engine_exe
{
	// // ----- CONSTRUCTORS ----- // //
	Timer::Timer() :
		m_start(SDL_GetPerformanceCounter())
	{}

	Timer::Timer(double multiplier) :
		m_start(SDL_GetPerformanceCounter()),
		m_multiplier(multiplier)
	{}

	// // ----- GENERAL METHODS ----- // //
	void Timer::update()
	{
		// PerformanceCounter provides number of ticks since start
		// PerfomanceFrequency provides the number of ticks per second
		Uint64 previous = m_current;
		m_current = SDL_GetPerformanceCounter() - m_start;
		m_delta_time_ms = (double)((m_current - previous) * 1000) / SDL_GetPerformanceFrequency();

		// FPS stuff
		m_fps_counter++;
		m_delta_fps_ms += m_delta_time_ms;
		if (m_delta_fps_ms > m_fps_update_time_ms)
		{
			m_fps = (float)(m_fps_counter * 1000) / ((float)m_delta_fps_ms);
			m_fps_counter = 0;
			m_delta_fps_ms = 0.0;
		}
	}

	void Timer::debug_update()
	{
		update();
		if (delta_time_s() > 0.1)
		{
			__debugbreak();
		}
	}

	// // ----- GETTERS ----- // //
	const double Timer::total_time_s() const
	{
		return (double)m_current / SDL_GetPerformanceFrequency();
	}

	const double Timer::total_time_ms() const
	{
		return (double)(m_current * 1000) / SDL_GetPerformanceFrequency();
	}

	const double Timer::delta_time_s() const
	{
		return m_delta_time_ms * 0.001 * m_multiplier;
	}

	const double Timer::delta_time_ms() const
	{
		return m_delta_time_ms * m_multiplier;
	}

	const char* Timer::fps()
	{
		sprintf_s(m_fps_chars, "%.3f", m_fps);
		return m_fps_chars;
	}

	// // ----- SETTERS ----- // //
	void Timer::set_multiplier(const double multiplier)
	{
		m_multiplier = multiplier;
	}

	void Timer::set_fps_update_time(const float update_time_s)
	{
		m_fps_update_time_ms = update_time_s * 1000;
	}
} // namespace gl_engine