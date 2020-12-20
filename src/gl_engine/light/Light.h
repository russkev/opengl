#ifndef GL_ENGINE_LIGHT_LIGHT_H
#define GL_ENGINE_LIGHT_LIGHT_H

#include "mesh/Mesh.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "camera/Camera.h"

namespace glen
{
	/*!

		Base light class. Designed to be extended by specific light types

	*/
	struct Light
	{
		// // ----- CONSTANTS ----- // //
		static inline const std::string k_light_shader_type = "light";
		static inline const std::string k_light_brightness = "brightness";
		static inline const std::string k_light_color = "color";
		/*
		
		!!! These constants should really be restricted to MaterialLibrary

		*/

		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Light constructor
		*/
		Light() {};

		
		// // ----- GETTERS ----- // //

		/*!
		 * @return The brightness of the light
		*/
		const GLfloat& brightness() const;

		/*!
		 * @return a vector with the r, g, b color values of the light.
		*/
		const glm::vec3& color() const;

		/*!
		 * @return True if mesh representing the light is renderable.
		*/
		const bool& mesh_enabled() const;

		/*!
		 * @return The mesh object of the representation of the light.
		*/
		virtual Mesh* mesh();

		/*!
		 * @return The material object of the representation of the light.
		*/
		virtual Material* material();

		/*!
		 * @return The camera object used for shadow casting.
		*/
		virtual Camera* camera();

		/*!
		 * @return True if light casts a specular reflection.
		*/
		const bool& specular_enabled() const;

		/*!
		 * @return True if light illuminates diffuse componenet of meshes.
		*/
		const bool& diffuse_enabled() const;
		
		
		// // ----- SHADER COMMANDS ----- // //

		/*!
		 * @brief Get the type of the light. Useful for partitioning lights when rendering.
		 * @return A string representing the type of the light.
		*/
		virtual const std::string& type() const = 0;

		
		// // ----- SETTERS ----- // //

		/*!
		 * @param brightness Set the brightness of the light.
		*/
		void set_brightness(const GLfloat brightness);

		/*!
		 * @param color Set the color of the light with an r, g, b vector.
		 * Color values should be in the range 0 to 1.
		*/
		void set_color(const glm::vec3 color);

		/*!
		 * @brief Make the light itself renderable.
		*/
		void enable_mesh();

		/*!
		 * @brief Make the light itself not renderable.
		*/
		void disable_mesh();

		/*!
		 * @brief Make the light illuminate the diffuse component of materials.
		*/
		void enable_diffuse();

		/*!
		 * @brief Stop the light from illuminating the diffuse component of materials.
		*/
		void disable_diffuse();

		/*!
		 * @brief Make the light cast specular reflections.
		*/
		void enable_specular();

		/*!
		 * @brief Prevent the light from casting specular reflections.
		*/
		void disable_specular();

		// // ----- MEMBER VARIABLES ----- // //
	private:

		// @cond

		bool m_mesh_enabled = true;
		bool m_specular_enabled = true;
		bool m_diffuse_enabled = true;
		GLfloat m_brightness = 0.0;
		glm::vec3 m_color = { 0.0f, 0.0f, 0.0f };

		// @endcond

	};
}
#endif