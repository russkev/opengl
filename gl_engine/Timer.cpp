#include "Timer.h"

#include <stdio.h>


Timer::Timer() :
	m_start(SDL_GetPerformanceCounter())
{}

Timer::Timer(double multiplier) :
	m_start(SDL_GetPerformanceCounter()),
	m_multiplier(multiplier)
{}

void Timer::update()
{
	// PerformanceCounter provides number of ticks since start
	// PerfomanceFrequency provides the number of ticks per second
	Uint64 previous = m_current;
	m_current = SDL_GetPerformanceCounter() - m_start;
	m_delta_time_ms = (double)((m_current - previous) * 1000) / SDL_GetPerformanceFrequency();
}

void Timer::debug_update()
{
	update();
	if (delta_time_s() > 0.1)
	{
		__debugbreak();
	}
}

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

void Timer::set_multiplier(const double multiplier)
{
	m_multiplier = multiplier;
}

const char* Timer::fps()
{
	sprintf_s(m_fps, "%.3f", 1.0 / delta_time_s());
	return m_fps;
}
