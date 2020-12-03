#ifndef GL_ENGINE_UTILS_SCALAR_UTILS_H
#define GL_ENGINE_UTILS_SCALAR_UTILS_H

namespace glen
{
	/*

	Various helper methods

	*/
	namespace ScalarUtils
	{
		// // ----- MIN/MAX VALUES ----- // //
		template <typename T1, typename T2>
		T1 min_value(T1 first, T2 second)
		{
			return (first < second) ? first : second;
		}
		template <typename T1, typename T2, typename ...Args>
		T1 min_value(T1 first, T2 second, Args ...args)
		{
			return (first < second) ? min_value(first, args...) : min_value(second, args...);
		}
		template<typename T1, typename T2>
		T1 max_value(T1 first, T2 second)
		{
			return (first > second) ? first : second;
		}
		template<typename T1, typename T2, typename ...Args>
		T1 max_value(T1 first, T2 second, Args... args)
		{
			return (first > second) ? max_value(first, args...) : max_value(second, args...);
		}

		// // ----- MIN MAX LOOP ----- // //
		template<typename T>
		T min_max_loop(T num, T min = 0, T max = 1)
		{
			T p_max = max - min;
			T p_num = num - min;
			T out;
			if (!fmod(p_num, p_max))
			{
				out = abs(fmod(p_num, p_max * 2));
			}
			else
			{
				out = p_num < 0 ? p_max + fmod(p_num, p_max) : fmod(p_num, p_max);
			}
			return out;
		}

		// // ----- LERP ----- // //
		template<typename T>
		T lerp(const T& a, const T& b, const T& amount)
		{
			return a + amount * (b - a);
		}
	}
}
#endif