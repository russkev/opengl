#ifndef GL_ENGINE_UTILS_SCALAR_UTILS_H
#define GL_ENGINE_UTILS_SCALAR_UTILS_H

namespace glen
{
	/*!

		Various helper methods for working with scalar values.

	*/
	namespace ScalarUtils
	{
		// // ----- MIN/MAX VALUES ----- // //

		/*!
		 * @return The argument that is the smallest of the two.
		*/
		template <typename T1, typename T2>
		T1 min_value(T1 first, T2 second)
		{
			return (first < second) ? first : second;
		}

		/*!
		 * @return The argument out of all provided which is the smallest.
		*/
		template <typename T1, typename T2, typename ...Args>
		T1 min_value(T1 first, T2 second, Args ...args)
		{
			return (first < second) ? min_value(first, args...) : min_value(second, args...);
		}

		/*!
		 * @return The argument that is the largest of the two
		*/
		template<typename T1, typename T2>
		T1 max_value(T1 first, T2 second)
		{
			return (first > second) ? first : second;
		}

		/*!
		 * @return The argument out of all provided which is the largest.
		*/
		template<typename T1, typename T2, typename ...Args>
		T1 max_value(T1 first, T2 second, Args... args)
		{
			return (first > second) ? max_value(first, args...) : max_value(second, args...);
		}

		// // ----- LERP ----- // //

		/*!
		 * @brief linearly interpolate between `a` and `b` by `amount`.
		*/
		template<typename T>
		T lerp(const T& a, const T& b, const T& amount)
		{
			return a + amount * (b - a);
		}
	}
}
#endif