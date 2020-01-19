#ifndef GL_ENGINE_SHADING_MATERIAL_H
#define GL_ENGINE_SHADING_MATERIAL_H

#include <map>
#include <string>
#include <list>
#include <set>

#include <GL/glew.h>

#include "Shader.h"



namespace gl_engine
{
	// // ----- FORWARD DECLERATIONS ----- // //
	struct Texture;
	struct Framebuffer;

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
	private:
		void bind_texture(const std::string uniform_name, Texture* texture);
	public:
		void unbind_textures();
		void update_texture_id(const std::string uniform_name, const GLuint id);

		// // ----- SETTERS ----- // //
	public:
		//void set_sampler_color(const std::string& uniform_name, glm::vec4& color);
		void set_sampler_color(const std::string& uniform_name, glm::vec3& color);
		void set_sampler_value(const std::string& uniform_name, GLfloat value);
		void set_texture(const std::string& uniform_name, Texture* texture);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		std::map<std::string, Texture*> m_textures;
		std::list<Texture> m_colors;
		std::set<std::string> m_failed_uniforms;
	};
} // namespace gl_engine
#endif
