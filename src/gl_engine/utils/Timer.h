#ifndef GL_ENGINE_TIMER_H
#define GL_ENGINE_TIMER_H

namespace glen
{
	/*!

		Basic high accuracy timer.
		Gives change in time and total time since initialisation
		Optional multiplier to adjust overall timegit s

	*/
	struct Timer
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		Timer();

		/*!
		 * @brief Constructor
		 * @param multiplier Set the apparant speed of time. Lower values will slow everything down.
		*/
		Timer(const double multiplier);

		// // ----- GENERAL METHODS ----- // //
		
		/*!
		 * @brief Mark the passage of another frame
		*/
		void update();

		/*!
		 * @brief Mark the passage of another frame (with debug info printed to terminal).
		*/
		void debug_update();

		
		// // ----- GETTERS ----- // //

		/*!
		 * @return The total number of seconds passed since the timer was started.
		*/
		const double total_time_s() const;

		/*!
		 * @return The total number of milliseconds passed since the timer was started.
		*/
		const double total_time_ms() const;

		/*!
		 * @return The number of seconds passed since the last update.
		*/
		const double delta_time_s() const;

		/*!
		 * @return The number of milliseconds passed since the last update.
		*/
		const double delta_time_ms() const;

		/*!
		 * @return The current frame rate.
		*/
		const char* fps();

		// // ----- SETTERS ----- // //
		
		/*!
		 * @brief Change the multiplier used to either speed up or slow down time.
		*/
		void set_multiplier(const double multiplier);

		/*!
		 * @brief How frequently (in seconds) to update the frame rate output.
		*/
		void set_fps_update_time(const float update_time);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Uint64 m_start = 0, m_current = 0;
		double m_delta_time_ms = 0.0;
		double m_multiplier = 1.0;

		float m_fps = 0.0f;
		char m_fps_chars[16];
		int m_fps_counter = 0;
		double m_delta_fps_ms = 0.0;
		float m_fps_update_time_ms = 200.0f;

	};
}
#endif
