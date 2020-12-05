/*

	GL_Tuple_Introsopect code provided by Aleksandras Sevcenko

*/

#ifndef GL_ENGINE_GL_TUPLE_INTROSPECT_H
#define GL_ENGINE_GL_TUPLE_INTROSPECT_H

#include "GL_Type_Traits.h"

namespace glen
{
	/*
	
		Helper struct used for automatically generating the correct VAO type
	
	*/
	struct member_info_type
	{
		std::uint32_t gl_type_enum;
		std::uint32_t storage_size;
		std::uint32_t alignment_size;
		std::uint32_t component_count;
		std::uint32_t attribute_slot;
		std::uint32_t slot_occupancy;
		std::size_t   slot_size_in_bytes;
		std::uint8_t* offset_from_start;
		std::uint32_t tuple_stride;
	};

	// Base case
	template <typename _Tuple>
	struct gl_introspect_tuple;

	template <typename... _Arg_type>
	struct gl_introspect_tuple<std::tuple<_Arg_type...>>
	{
		// MEMBER VARIABLES
		using tuple_type = std::tuple<_Arg_type...>;
		static constexpr auto tuple_size = std::tuple_size_v<tuple_type>;
		using result_type = std::array<member_info_type, tuple_size>;

	private:
		template <std::size_t _Idx>
		using traits_for_index = gl_type_traits < std::decay_t<decltype (std::get<_Idx>(*(tuple_type*)nullptr)) >>;

		template <std::size_t... I>
		static constexpr auto auxilary_get(std::index_sequence<I...>)
		{
			std::uint32_t current_slot = 0u;
			std::uint32_t next_slot = 0u;
			return result_type
			{
				member_info_type
				{
					traits_for_index<I>::gl_type_enum,
					traits_for_index<I>::storage_size,
					traits_for_index<I>::alignment_size,
					traits_for_index<I>::component_count,
					(current_slot = next_slot, next_slot += traits_for_index<I>::slot_occupancy, current_slot),
					traits_for_index<I>::slot_occupancy,
					traits_for_index<I>::component_count * traits_for_index<I>::storage_size,
					(std::uint8_t*)std::addressof(std::get<I>(*(tuple_type*)nullptr)),
					sizeof(tuple_type)
				}...
			};
		}


	public:
		static constexpr auto get() -> result_type
		{
			return auxilary_get(std::make_index_sequence<tuple_size>{});
		}
	};
}
#endif