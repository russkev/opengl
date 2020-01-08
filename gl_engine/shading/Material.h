#ifndef GL_ENGINE_SHADING_MATERIAL_H
#define GL_ENGINE_SHADING_MATERIAL_H

#include <map>
#include <GL/glew.h>
#include <string>

#include "Shader.h"



namespace gl_engine
{
	// // ----- FORWARD DECLERATIONS ----- // //
	struct Texture;

	/*

		A material node links a shader to various textures and other attributs

	*/
	struct Material : public Shader
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		Material() {};
		Material(const std::string name, const char* vertex_shader, const char* fragment_shader);
		Material(const std::string name, const char* vertex_shader, const char* geometry_shader, const char* fragment_shader);

		// // ----- GENERAL METHODS ----- // //
	public:
		void bind_textures();
		void unbind_textures();

		// // ----- SETTERS ----- // //
	public:
		void set_sampler_color(const std::string& uniform_name, glm::vec4& color);
		void set_sampler_color(const std::string& uniform_name, glm::vec3& color);
		void set_sampler_value(const std::string& uniform_name, GLfloat value);
		void add_texture(const std::string& uniform_name, Texture* texture);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::map<std::string, Texture*> m_textures;
		std::vector<Texture> m_colors;
	};
} // namespace gl_engine
#endif
