#ifndef GL_ENGINE_MATERIAL_SHADER_TYPES_H
#define GL_ENGINE_MATERIAL_SHADER_TYPES_H

#include <GL/glew.h>
#include <glm/glm.hpp>


namespace glen
{
	namespace ShaderType
	{
		/*

			A mapping of cpp types to shader type enum values
			Not included are all the sampler and image types

		*/
		const static std::map<const type_info*, GLenum> gl_types
		{
			{&typeid(GLfloat), GL_FLOAT},
			{&typeid(glm::vec2), GL_FLOAT_VEC2},
			{&typeid(glm::vec3), GL_FLOAT_VEC3},
			{&typeid(glm::vec4), GL_FLOAT_VEC4},
			{&typeid(GLdouble), GL_DOUBLE},
			{&typeid(glm::dvec2), GL_DOUBLE_VEC2},
			{&typeid(glm::dvec3), GL_DOUBLE_VEC3},
			{&typeid(glm::dvec4), GL_DOUBLE_VEC4},
			{&typeid(GLint), GL_INT},
			{&typeid(glm::ivec2), GL_INT_VEC2},
			{&typeid(glm::ivec3), GL_INT_VEC2},
			{&typeid(glm::ivec4), GL_INT_VEC4},
			{&typeid(GLuint), GL_UNSIGNED_INT},
			{&typeid(glm::uvec2), GL_UNSIGNED_INT_VEC2},
			{&typeid(glm::uvec3), GL_UNSIGNED_INT_VEC3},
			{&typeid(glm::uvec4), GL_UNSIGNED_INT_VEC4},
			{&typeid(GLboolean), GL_BOOL},
			{&typeid(glm::bvec2), GL_BOOL_VEC2},
			{&typeid(glm::bvec3), GL_BOOL_VEC3},
			{&typeid(glm::bvec4), GL_BOOL_VEC4},
			{&typeid(glm::mat2), GL_FLOAT_MAT2},
			{&typeid(glm::mat3), GL_FLOAT_MAT3},
			{&typeid(glm::mat4), GL_FLOAT_MAT4},
			{&typeid(glm::mat2x3), GL_FLOAT_MAT2x3},
			{&typeid(glm::mat2x4), GL_FLOAT_MAT2x4},
			{&typeid(glm::mat3x2), GL_FLOAT_MAT3x2},
			{&typeid(glm::mat3x4), GL_FLOAT_MAT3x4},
			{&typeid(glm::mat4x2), GL_FLOAT_MAT4x2},
			{&typeid(glm::mat4x3), GL_FLOAT_MAT4x3},
			{&typeid(glm::dmat2), GL_DOUBLE_MAT2},
			{&typeid(glm::dmat3), GL_DOUBLE_MAT3},
			{&typeid(glm::dmat4), GL_DOUBLE_MAT4},
			{&typeid(glm::dmat2x3), GL_DOUBLE_MAT2x3},
			{&typeid(glm::dmat2x4), GL_DOUBLE_MAT2x4},
			{&typeid(glm::dmat3x2), GL_DOUBLE_MAT3x2},
			{&typeid(glm::dmat3x4), GL_DOUBLE_MAT3x4},
			{&typeid(glm::dmat4x2), GL_DOUBLE_MAT4x2},
			{&typeid(glm::dmat4x3), GL_DOUBLE_MAT4x3},
			{&typeid(float), GL_FLOAT},
			{&typeid(double), GL_DOUBLE},
			{&typeid(int), GL_INT},
			{&typeid(uint16_t), GL_UNSIGNED_INT},
			{&typeid(bool), GL_BOOL}
		};

		const static std::set<GLenum> gl_float_samplerTypes
		{
			GL_SAMPLER,
			GL_SAMPLER_1D,
			GL_SAMPLER_2D,
			GL_SAMPLER_3D,
			GL_SAMPLER_CUBE,
			GL_SAMPLER_1D_SHADOW,
			GL_SAMPLER_2D_SHADOW,
			GL_SAMPLER_1D_ARRAY,
			GL_SAMPLER_2D_ARRAY,
			GL_SAMPLER_1D_ARRAY_SHADOW,
			GL_SAMPLER_2D_ARRAY_SHADOW,
			GL_SAMPLER_2D_MULTISAMPLE,
			GL_SAMPLER_2D_MULTISAMPLE_ARRAY,
			GL_SAMPLER_CUBE_SHADOW,
			GL_SAMPLER_BUFFER,
			GL_SAMPLER_2D_RECT,
			GL_SAMPLER_2D_RECT_SHADOW
		};

		const static std::set<GLenum> gl_int_samplerTypes
		{
			GL_INT_SAMPLER_1D,
			GL_INT_SAMPLER_2D,
			GL_INT_SAMPLER_3D,
			GL_INT_SAMPLER_CUBE,
			GL_INT_SAMPLER_1D_ARRAY,
			GL_INT_SAMPLER_2D_ARRAY,
			GL_INT_SAMPLER_2D_MULTISAMPLE,
			GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
			GL_INT_SAMPLER_BUFFER,
			GL_INT_SAMPLER_2D_RECT,
		};

		const static std::set<GLenum> gl_uint_samplerTypes
		{
			GL_UNSIGNED_INT_SAMPLER_1D,
			GL_UNSIGNED_INT_SAMPLER_2D,
			GL_UNSIGNED_INT_SAMPLER_3D,
			GL_UNSIGNED_INT_SAMPLER_CUBE,
			GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,
			GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
			GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
			GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
			GL_UNSIGNED_INT_SAMPLER_BUFFER,
			GL_UNSIGNED_INT_SAMPLER_2D_RECT,
		};
	}
}


#endif