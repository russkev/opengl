#ifndef GL_ENGINE_UTILS_VECTOR_UTILS_H
#define GL_ENGINE_UTILS_VECTOR_UTILS_H

#include <vector>
#include <utility>
#include <math.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace gl_engine
{
	/*
	
		Various helpot methods
	
	*/
	namespace VectorUtils
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


		//// // ----- DISTANCE SQUARED ----- // //
		GLfloat distanceSquared(glm::vec2, glm::vec2);
		GLfloat distanceSquared(glm::vec3, glm::vec3);

		// // ----- IS NEAR ----- // //
		template <typename T>
		bool isNear(T s_point_1, T s_point_2, float threshold)
		{
			return distanceSquared(s_point_1, s_point_2) < (threshold * threshold);
		}

		// // ----- IS LESS THAN ----- // //
		bool isLessThan(const glm::vec2&, const glm::vec2&, const float threshold);
		bool isLessThan(const glm::vec3&, const glm::vec3&, float threshold);


		// // ----- QUICK SORT VECTOR OF PAIRS ----- // //
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

		// // ----- TRANSLATE ROTATE SCALE ----- // //
		/*

			Creates a 4x4 transform matrix for a 3x3 translate rotate scale matrix
			Input matrix should be of the form {translate vector, rotate vector, scale vector}
			Optionally take a string specifying the rotate order.

		*/
		template<typename T>
		glm::tmat4x4<T> trs(const glm::tmat3x3<T>& s_trs, std::string s_rotate_order = "xyz")//, const char[]& s_rotate_order = 'xyz')
		{
			assert(s_rotate_order.size() == 3 && "Rotate order must be exactly three letters");

			std::vector<int> rotate_order;
			glm::tvec3<T> rotate_axes[] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };

			// Break out the various vectors
			const glm::tvec3<T>* translate_vec = &s_trs[0];
			glm::tvec3<T> rotate_vec{ glm::radians(s_trs[1][0]), glm::radians(s_trs[1][1]), glm::radians(s_trs[1][2]) };
			const glm::tvec3<T>* scale_vec = &s_trs[2];

			// Parse rotate order
			for (auto & i : s_rotate_order)
			{
				assert(i >= 'x' && i <= 'z' && "Rotate order letters must be 'x', 'y' or 'z'");
				rotate_order.push_back(i - 120);
			}

			// Do transformation all at once
			return
				glm::scale
				(
					glm::rotate
					(
						glm::rotate
						(
							glm::rotate
							(
								glm::translate
								(
									glm::tmat4x4<T>(T(1.0f)), *translate_vec
								),
								rotate_vec[rotate_order[2]], rotate_axes[rotate_order[2]]
							),
							rotate_vec[rotate_order[1]], rotate_axes[rotate_order[1]]
						),
						rotate_vec[rotate_order[0]], rotate_axes[rotate_order[0]]
					),
					*scale_vec
				);
			//return
			//	glm::translate
			//	(
			//		glm::rotate
			//		(
			//			glm::rotate
			//			(
			//				glm::rotate
			//				(
			//					glm::scale
			//					(
			//						glm::tmat4x4<T>(T(1.0f)), *scale_vec
			//					),
			//					rotate_vec[rotate_order[2]], rotate_axes[rotate_order[2]]
			//				),
			//				rotate_vec[rotate_order[1]], rotate_axes[rotate_order[1]]
			//			),
			//			rotate_vec[rotate_order[0]], rotate_axes[rotate_order[0]]
			//		),
			//		*translate_vec
			//	);
		}
		// // ----- MIN MAX LOOP ----- // //
		template<typename T>
		T minMaxLoop(T num, T min = 0, T max = 1)
		{
			//T p_min = (T)0;
			T p_max = max - min;
			T p_num = num - min;
			//T p_num = num < 0 ? 0 - num : num;


			//T p_num = num < min ? (T)fmod(num, max) + p_max : num;
			//return fmod(abs(p_num), p_max*2) == p_max?max:(T)fmod(p_num, p_max);

			//auto test = p_max + fmod(p_num, p_max);
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
	};
} // namespace gl_engine
#endif