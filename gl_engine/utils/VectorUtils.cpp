#include "pch.h"
#ifndef GL_ENGINE_UTILS_VECTOR_UTILS_H
#define GL_ENGINE_UTILS_VECTOR_UTILS_H

#include <vector>
#include <utility>
#include <math.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace glen
{
	/*

		Various helpot methods

	*/
	namespace VectorUtils
	{
		// // ----- COMBINE VECTORS ----- // //
		template<typename T, typename ...Args>
		std::vector<T> combine_vectors(std::vector<T>& s_first, Args...args)
		{
			std::vector<T> second = combine_vectors(args...);
			std::vector<T> outDistances;

			outDistances.reserve(s_first.size() + second.size());
			outDistances.insert(outDistances.end(), s_first.begin(), s_first.end());
			outDistances.insert(outDistances.end(), second.begin(), second.end());

			return outDistances;
		}

		template<typename T>
		std::vector<T> combine_vectors(std::vector<T>& s_first, std::vector<T>& s_second)
		{
			std::vector<T> outDistances;

			outDistances.reserve(s_first.size() + s_second.size());
			outDistances.insert(outDistances.end(), s_first.begin(), s_first.end());
			outDistances.insert(outDistances.end(), s_second.begin(), s_second.end());

			return outDistances;
		}

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

		// // ----- AVERAGE VECTOR ----- // //
		template <typename ... Args>
		glm::vec3 vector_average(Args... args)
		{
			glm::vec3 outVec;

			auto min_x = min_value(args.x...);
			auto max_x = max_value(args.x...);
			auto min_y = min_value(args.y...);
			auto max_y = max_value(args.y...);
			auto min_z = min_value(args.z...);
			auto max_z = max_value(args.z...);

			outVec.x = min_x + (max_x - min_x) / 2;
			outVec.y = min_y + (max_y - min_y) / 2;
			outVec.z = min_z + (max_z - min_z) / 2;

			return outVec;
		}


		//// // ----- DISTANCE SQUARED ----- // //
		GLfloat distance_squared(glm::vec2, glm::vec2);
		GLfloat distance_squared(glm::vec3, glm::vec3);

		// // ----- IS NEAR ----- // //
		template <typename T>
		bool is_near(T s_point_1, T s_point_2, float threshold)
		{
			return distance_squared(s_point_1, s_point_2) < (threshold * threshold);
		}

		// // ----- IS LESS THAN ----- // //
		bool is_less_than(const glm::vec2&, const glm::vec2&, const float threshold);
		bool is_less_than(const glm::vec3&, const glm::vec3&, float threshold);


		// // ----- QUICK SORT VECTOR OF PAIRS ----- // //
		template<typename T1, typename T2>
		void quick_sort_pair_vector(std::vector<std::pair<T1, T2>>& s_pairs)
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
			quick_sort_pair_vector(less);
			quick_sort_pair_vector(greater);

			s_pairs = combine_vectors(greater, pivots, less);
			return;
		}


		template<typename T>
		glm::tvec3<T> extract_position(const glm::tmat4x4<T>& transform)
		{
			return glm::tvec3<T>{ transform[3][0], transform[3][1], transform[3][2] };
		}

		template<typename T>
		void set_position(glm::tmat4x4<T>& transform, const glm::tvec3<T>& position)
		{
			transform[3][0] = position[0];
			transform[3][1] = position[1];
			transform[3][2] = position[2];
		}

		template<typename T>
		void add_position_to_transform(glm::tmat4x4<T>& transform, const glm::tvec3<T>& position)
		{
			glm::tvec3<T> newPosition = extract_position(transform) + position;
			set_position(transform, newPosition);
		}


		// // ----- TRANSLATE ROTATE SCALE ----- // //
		/*

			Creates a 4x4 transform matrix for a 3x3 translate rotate scale matrix
			Input matrix should be of the form {translate vector, rotate vector, scale vector}
			Optionally take a string specifying the rotate order.

		*/
		template<typename T>
		glm::tmat4x4<T> translate_rotate_scale(const glm::tmat3x3<T>& trans_rot_scale, std::string s_rotate_order = "xyz")//, const char[]& s_rotate_order = 'xyz')
		{
			assert(s_rotate_order.size() == 3 && "Rotate order must be exactly three letters");

			std::vector<int> rotate_order;
			glm::tvec3<T> rotate_axes[] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };

			// Break out the various vectors
			const glm::tvec3<T>* translate_vec = &trans_rot_scale[0];
			glm::tvec3<T> rotate_vec{ glm::radians(trans_rot_scale[1][0]), glm::radians(trans_rot_scale[1][1]), glm::radians(trans_rot_scale[1][2]) };
			const glm::tvec3<T>* scale_vec = &trans_rot_scale[2];

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
	};
}


#endif