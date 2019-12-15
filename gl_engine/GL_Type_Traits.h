#ifndef GL_ENGINE_GL_TYPE_TRAITS_H
#define GL_ENGINE_GL_TYPE_TRAITS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

namespace gl_engine
{
	// // Base case 
	template <typename T>
	struct gl_type_traits;

	// Build a macro to make it easier to do verious template specializations
	// For example, for float, the first one would read:
	//
	//		template <>
	//		struct gl_type_traits<float>
	//		{
	//			static constexpr auto gl_enum_type		= GL_FLOAT;
	//			static constexpr auto component_count	= 1u;
	//			static constexpr auto slot_occupancy    = 1u;
	//			static constexpr auto storage_size		= sizeof(float);
	//			static constexpr auto alignment_size	= alignof(float);
	//		{
	//

#define DEFINE_GL_TYPE_TRAITS(T, EN, CC, SO)\
template <> struct gl_type_traits<T>\
{\
	static constexpr auto gl_type_enum		= EN;\
	static constexpr auto component_count	= CC;\
	static constexpr auto slot_occupancy    = SO;\
	static constexpr auto storage_size		= sizeof(T);\
	static constexpr auto alignment_size	= alignof(T);\
}

	DEFINE_GL_TYPE_TRAITS(float, GL_FLOAT, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(double, GL_DOUBLE, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::int64_t, GL_INT64_NV, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::uint64_t, GL_UNSIGNED_INT64_NV, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::int32_t, GL_INT, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::uint32_t, GL_UNSIGNED_INT, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::int16_t, GL_SHORT, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::uint16_t, GL_UNSIGNED_SHORT, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::int8_t, GL_BYTE, 1u, 1u);
	DEFINE_GL_TYPE_TRAITS(std::uint8_t, GL_UNSIGNED_BYTE, 1u, 1u);

	// Get rid of macro to let it be redifined
#undef DEFINE_GL_TYPE_TRAITS

// Specialize for vector and matrix types
#define DEFINE_GL_TYPE_TRAITS(TM, CC_Mul, SO_Mul)\
template <typename T>\
struct gl_type_traits<TM<T>>\
{\
	static constexpr auto gl_type_enum		= gl_type_traits<T>::gl_type_enum; \
	static constexpr auto component_count	= gl_type_traits<T>::component_count * CC_Mul; \
	static constexpr auto slot_occupancy	= gl_type_traits<T>::slot_occupancy * SO_Mul; \
	static constexpr auto storage_size		= sizeof(T); \
	static constexpr auto alignment_size		= alignof (T); \
}

// Vectors:
	DEFINE_GL_TYPE_TRAITS(glm::tvec2, 2u, 1u);
	DEFINE_GL_TYPE_TRAITS(glm::tvec3, 3u, 1u);
	DEFINE_GL_TYPE_TRAITS(glm::tvec4, 4u, 1u);

	// Matrices:
	DEFINE_GL_TYPE_TRAITS(glm::tmat2x2, 2u, 2u);
	DEFINE_GL_TYPE_TRAITS(glm::tmat2x3, 2u, 3u);
	DEFINE_GL_TYPE_TRAITS(glm::tmat2x4, 2u, 4u);

	DEFINE_GL_TYPE_TRAITS(glm::tmat3x2, 3u, 2u);
	DEFINE_GL_TYPE_TRAITS(glm::tmat3x3, 3u, 3u);
	DEFINE_GL_TYPE_TRAITS(glm::tmat3x4, 3u, 4u);

	DEFINE_GL_TYPE_TRAITS(glm::tmat4x2, 4u, 2u);
	DEFINE_GL_TYPE_TRAITS(glm::tmat4x3, 4u, 3u);
	DEFINE_GL_TYPE_TRAITS(glm::tmat4x4, 4u, 4u);

	// Get rid of macro so it doesn't pollute namespace
#undef DEFINE_GL_TYPE_TRAITS
} // namespace gl_engine
#endif