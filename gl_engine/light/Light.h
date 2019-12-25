#ifndef GL_ENGINE_LIGHT_LIGHT_H
#define GL_ENGINE_LIGHT_LIGHT_H

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <../mesh/Mesh.h>
#include <../shading/Shader.h>
#include <../shading/Material.h>
#include <../camera/Camera.h>

namespace gl_engine
{
	/*

		Base light class. Designed to be extended by specific light types

	*/
	struct Light
	{
		static const std::string LIGHT_SHADER_TYPE;
		static const std::string LIGHT_COLOR;
		static const std::string LIGHT_BRIGHTNESS;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		bool m_mesh_enabled = true;
		GLfloat m_brightness = 0.0;
		glm::vec3 m_color = { 0.0f, 0.0f, 0.0f };
		

		// // ----- CONSTRUCTORS ----- // //
	public:
		Light() {};

		// // ----- GETTERS ----- // //
		const GLfloat& brightness() const;
		const glm::vec3& color() const;
		const bool& isEnabled() const;
		virtual Mesh* mesh();
		virtual Shader* shader();
		virtual Camera* camera();
		

		// // ----- SHADER COMMANDS ----- // //
		virtual const std::string& type() const = 0;

		// // ----- SETTERS ----- // //
		void setBrightness(const GLfloat brightness);
		void setColor(const glm::vec3 color);
		void enable_mesh();
		void disable_mesh();
	};
} // namespace gl_engine
#endif