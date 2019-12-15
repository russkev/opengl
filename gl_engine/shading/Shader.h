#ifndef GL_ENGINE_SHADING_SHADER_H
#define GL_ENGINE_SHADING_SHADER_H


#include <map>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <set>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../shading/Texture.h"

namespace gl_engine
{
	// Forward decleration
	struct LightNode;


	/*

		Helper struct to store uniform information

	*/
	struct Uniform
	{
		GLint location;
		GLenum type;
		GLsizei dataSize;
	};


	/*

		Main shader class that stores relevent information and functions about a particular shader

	*/
	struct Shader
	{
		// // ----- MEMBER ATTRIBUTES ----- // //
	private:
		std::string m_name;
		GLuint m_programID;
		std::map<std::string, Uniform> m_uniforms;
		std::set<std::string> m_hasBeenWarned;

		// // ----- CONSTRUCTOR ----- // //
	public:
		Shader() {};
		Shader(const std::string& name, const char* vertexShader, const char* fragmentShader);

		// // ----- GENERAL METHODS ----- // //

		// Set uniform of shader.
		// Automatically choose correct upload path based on data type
		// Defined in header because of template usage
		template<typename T>
		void setUniform(const std::string& name, T data)
		{
			GLenum dataEnumType = ShaderType::glTypes.at(&typeid(T));
			Uniform* thisUniform;
			std::string name_s = name;

			if (m_uniforms.size() == 0)
			{
				if (m_hasBeenWarned.find(name_s) == m_hasBeenWarned.end())
				{
					printf("WARNING: No uniforms stored in \"%s\". \"%s\" will not be set\n", m_name.c_str(), name.c_str());
					m_hasBeenWarned.insert(name_s);
				}
				return;
			}
			try
			{
				thisUniform = &m_uniforms.at(name);
			}
			catch (const std::out_of_range& e)
			{
				if (m_hasBeenWarned.find(name_s) == m_hasBeenWarned.end())
				{
					printf("WARNING: Setting uniform \"%s\" for shader \"%s\" failed. Uniform not found (%s)\n", name_s.c_str(), m_name.c_str(), e.what());
					m_hasBeenWarned.insert(name_s);
				}
				return;
			}
			if (thisUniform->type != dataEnumType)
			{
				if (m_hasBeenWarned.find(name_s) == m_hasBeenWarned.end())
				{
					printf("WARNING: Setting uniform \"%s\" for shader \"%s\" failed. Incorrect data type\n", name.c_str(), m_name.c_str());
					m_hasBeenWarned.insert(name_s);
				}
				return;
			}

			use();
			uploadUniform(thisUniform, data);
		}

		void setTexture(const std::string& location_name, Texture& texture);

		void use();
		bool containsUniform(const std::string uniform_name);
		void updateLights(const std::vector<LightNode*>& light_nodes);


	private:
		void fetchUniforms();

	public:
		// // ----- GETTERS ----- // //
		const GLuint programID() const;
		const std::string name() const;
		const std::map<std::string, Uniform>& uniforms() const;

		// // ----- SETTERS ----- // //
		std::string& name();

		// // ----- UNIFORM UPLOAD METHODS ----- // //
	private:
		void uploadUniform(Uniform* uniform, const GLfloat& data) { glUniform1f(uniform->location, data); };
		void uploadUniform(Uniform* uniform, const glm::vec2& data) { glUniform2fv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::vec3& data) { glUniform3fv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::vec4& data) { glUniform4fv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const GLdouble& data) { glUniform1d(uniform->location, data); };
		void uploadUniform(Uniform* uniform, const glm::dvec2& data) { glUniform2dv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::dvec3& data) { glUniform3dv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::dvec4& data) { glUniform4dv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const GLint& data) { glUniform1i(uniform->location, data); };
		void uploadUniform(Uniform* uniform, const glm::ivec2& data) { glUniform2iv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::ivec3& data) { glUniform3iv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::ivec4& data) { glUniform4iv(uniform->location, 1, &data[0]); };
		void uploadUniform(Uniform* uniform, const glm::mat2& data) { glUniformMatrix2fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat3& data) { glUniformMatrix3fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat4& data) { glUniformMatrix4fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat2x3& data) { glUniformMatrix2x3fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat2x4& data) { glUniformMatrix2x4fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat3x2& data) { glUniformMatrix3x2fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat3x4& data) { glUniformMatrix3x4fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat4x2& data) { glUniformMatrix4x2fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::mat4x3& data) { glUniformMatrix4x3fv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat2& data) { glUniformMatrix2dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat3& data) { glUniformMatrix3dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat4& data) { glUniformMatrix4dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat2x3& data) { glUniformMatrix2x3dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat2x4& data) { glUniformMatrix2x4dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat3x2& data) { glUniformMatrix3x2dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat3x4& data) { glUniformMatrix3x4dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat4x2& data) { glUniformMatrix4x2dv(uniform->location, 1, false, &data[0][0]); };
		void uploadUniform(Uniform* uniform, const glm::dmat4x3& data) { glUniformMatrix4x3dv(uniform->location, 1, false, &data[0][0]); };
	};


	namespace ShaderType
	{
		/*

			A mapping of cpp types to shader type enum values
			Not included are all the sampler and image types

		*/
		const static std::map<const type_info*, GLenum> glTypes =
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
	}
} // namespace gl_engine
#endif