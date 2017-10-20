#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

// // Base case 
template <typename T>
struct gl_type_traits {};

#define DEFINE_GL_TYPE_TRAITS(T, EN, CC, SO)\
template <>\
struct gl_type_traits<T>\
{\
	static constexpr auto gl_enum_type		= EN;\
	static constexpr auto component_count	= CC;\
	static constexpr auto slot_occupancy    = SO;\
	static constexpr auto storage_size		= sizeof(T);\
	static constexpr auto alignment_size	= alignof(T);\
}

DEFINE_GL_TYPE_TRAITS(float,		 GL_FLOAT,				1u, 1u);
DEFINE_GL_TYPE_TRAITS(double,		 GL_DOUBLE,				1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::int64_t,  GL_INT64_NV,			1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::uint64_t, GL_UNSIGNED_INT64_NV,	1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::int32_t,	 GL_INT,				1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::uint32_t, GL_DOUBLE,				1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::int16_t,	 GL_SHORT,				1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::uint16_t, GL_UNSIGNED_SHORT,		1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::int8_t,	 GL_BYTE,				1u, 1u);
DEFINE_GL_TYPE_TRAITS(std::uint8_t,  GL_UNSIGNED_BYTE,		1u, 1u);