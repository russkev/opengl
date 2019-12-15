#ifndef GL_ENGINE_TIMER_H
#define GL_ENGINE_TIMER_H
#include <SDL.h>

namespace gl_engine
{
	/*

		Basic high accuracy timer.
		Gives change in time and total time since initialisation
		Optional multiplier to adjust overall time

	*/
	struct Timer
	{
	private:
		Uint64 m_start = 0, m_current = 0;
		double m_delta_time_ms = 0.0;
		double m_multiplier = 1.0;

		float m_fps = 0.0f;
		char m_fps_chars[16];
		int m_fps_counter = 0;
		double m_delta_fps_ms = 0.0;
		float m_fps_update_time_ms = 200.0f;

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
		void set_fps_update_time(const float update_time);

		const char* fps();
	};
} // namespace gl_engine
#endif
