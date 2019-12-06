#ifndef GL_ENGINE_TIMER_H
#define GL_ENGINE_TIMER_H
#include <SDL.h>

/*

	Basic high accuracy timer.
	Gives change in time and total time since initialisation
	Optional multiplier to adjust overall time

*/
struct Timer
{
private:
	Uint64 m_start = 0;
	Uint64 m_current = 0;
	double m_delta_time_ms = 0.0;
	double m_multiplier = 1.0;
	char m_fps[16];

public:
	Timer();
	Timer(const double multiplier);
	void update();
	void debug_update();

	const double total_time_s() const;
	const double total_time_ms() const;

	const double delta_time_s() const;
	const double delta_time_ms() const;

	void set_multiplier(const double multiplier);

	const char* fps();
};

#endif
