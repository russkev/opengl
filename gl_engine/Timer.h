#pragma once
#include <SDL.h>

struct Timer
{
	Timer();
	void init(const float multiplier = 1.0f);
	void update();
	double time();
	

private:
	double m_time;
	double m_freq_multiplier;
};
