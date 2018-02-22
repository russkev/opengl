#pragma once
#include <vector>
#include <utility>

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

	// // ----- AVERAGE VECTOR ----- // //
	template <typename ... Args>
	glm::vec3 vectorAverage(Args... args)
	{
		glm::vec3 outVec;

		auto min_x = minValue(args.x...);
		auto max_x = maxValue(args.x...);
		auto min_y = minValue(args.y...);
		auto max_y = maxValue(args.y...);
		auto min_z = minValue(args.z...);
		auto max_z = maxValue(args.z...);

		outVec.x = min_x + (max_x - min_x) / 2;
		outVec.y = min_y + (max_y - min_y) / 2;
		outVec.z = min_z + (max_z - min_z) / 2;

		return outVec;
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


	template<typename T1, typename T2>

	void quickSortPairVector(std::vector<std::pair<T1, T2>>& s_pairs)
	{
		typedef std::vector<std::pair<T1, T2>> pairType;

		if (s_pairs.size() < 2) return;

		auto pivot = s_pairs.at(0).first;
		pairType less;
		pairType greater;
		pairType pivots;

		for (auto & i : s_pairs)
		{
			if (i.first < pivot)
			{
				less.push_back(i);
			}
			else if (i.first > pivot)
			{
				greater.push_back(i);
			}
			else
			{
				pivots.push_back(i);
			}
		}
		quickSortPairVector(less);
		quickSortPairVector(greater);

		s_pairs = combineVectors(greater, pivots, less);
		return;
	}
};