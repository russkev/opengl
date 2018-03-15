#include "Timer.h"

Timer::Timer() :
	m_time(0),
	m_freq_multiplier(0)
{}

void Timer::init(const float multiplier)
{
	m_freq_multiplier	= multiplier / SDL_GetPerformanceFrequency();
	m_time				= m_freq_multiplier * SDL_GetPerformanceCounter();
}

void Timer::update()
{
	m_time = m_freq_multiplier * SDL_GetPerformanceCounter();
}

double Timer::time()
{
	return m_time;
}