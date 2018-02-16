#pragma once
#include <vector>
#include <GL/glew.h>

namespace Utilities
{
	// // ----- COMBINE VECTORS ----- // //
	template<typename T, typename ...Args>
	std::vector<T> combineVectors(std::vector<T>& s_first, Args...args)
	{
		std::vector<T> second = combineVectors(args...);
		std::vector<T> outDistances;

		outDistances.reserve(s_first.size() + second.size());
		outDistances.insert(outDistances.end(), s_first.begin(), s_first.end());
		outDistances.insert(outDistances.end(), second.begin(), second.end());

		return outDistances;
	}

	template<typename T>
	std::vector<T> combineVectors(std::vector<T>& s_first, std::vector<T>& s_second)
	{
		std::vector<T> outDistances;

		outDistances.reserve(s_first.size() + s_second.size());
		outDistances.insert(outDistances.end(), s_first.begin(), s_first.end());
		outDistances.insert(outDistances.end(), s_second.begin(), s_second.end());

		return outDistances;
	}

	// // ----- MIN/MAX VALUES ----- // //
	template <typename T1, typename T2>
	T1 minValue(T1 first, T2 second)
	{
		return (first < second) ? first : second;
	}
	template <typename T1, typename T2, typename ...Args>
	T1 minValue(T1 first, T2 second, Args ...args)
	{
		return (first < second) ? minValue(first, args...) : minValue(second, args...);
	}
	template<typename T1, typename T2>
	T1 maxValue(T1 first, T2 second)
	{
		return (first > second) ? first : second;
	}
	template<typename T1, typename T2, typename ...Args>
	T1 maxValue(T1 first, T2 second, Args... args)
	{
		return (first > second) ? maxValue(first, args...) : maxValue(second, args...);
	}

	// // ----- DISTANCE SQUARED ----- // //
	template <typename T>
	GLfloat distanceSquared(T s_point_1, T s_point_2)
	{
		if (typeid(T) == typeid(glm::vec2))
		{
			return
				(
				(s_point_2.x - s_point_1.x) * (s_point_2.x - s_point_1.x) +
				(s_point_2.y - s_point_1.y) * (s_point_2.y - s_point_1.y));
		}
		else
		{
			return
				(
				(s_point_2.x - s_point_1.x) * (s_point_2.x - s_point_1.x) +
				(s_point_2.y - s_point_1.y) * (s_point_2.y - s_point_1.y) +
				(s_point_2.z - s_point_1.z) * (s_point_2.z - s_point_1.z));
		}
	}
};