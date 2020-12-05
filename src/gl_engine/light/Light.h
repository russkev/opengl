#ifndef GL_ENGINE_LIGHT_LIGHT_H
#define GL_ENGINE_LIGHT_LIGHT_H

#include "mesh/Mesh.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "camera/Camera.h"

namespace glen
{
	/*

		Base light class. Designed to be extended by specific light types

	*/
	struct Light
	{
		// // ----- CONSTANTS ----- // //
		static inline const std::string k_light_shader_type = "light";
		static inline const std::string k_light_brightness = "brightness";
		static inline const std::string k_light_color = "color";
		/*
		
		!!! These should constants should really be restricted to MaterialLibrary

		*/

		// // ----- CONSTRUCTORS ----- // //
	public:
		Light() {};

		// // ----- GETTERS ----- // //
		const GLfloat& brightness() const;
		const glm::vec3& color() const;
		const bool& mesh_enabled() const;
		virtual Mesh* mesh();
		virtual Material* material();
		virtual Camera* camera();
		const bool& specular_enabled() const;
		const bool& diffuse_enabled() const;
		
		// // ----- SHADER COMMANDS ----- // //
		virtual const std::string& type() const = 0;

		// // ----- SETTERS ----- // //
		void set_brightness(const GLfloat brightness);
		void set_color(const glm::vec3 color);
		void enable_mesh();
		void disable_mesh();
		void enable_diffuse();
		void disable_diffuse();
		void enable_specular();
		void disable_specular();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		bool m_mesh_enabled = true;
		bool m_specular_enabled = true;
		bool m_diffuse_enabled = true;
		GLfloat m_brightness = 0.0;
		glm::vec3 m_color = { 0.0f, 0.0f, 0.0f };

	};
}
#endif