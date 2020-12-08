#ifndef GL_ENGINE_LIGHT_DIRECTIONAL_H
#define GL_ENGINE_LIGHT_DIRECTIONAL_H

#include "Light.h"
#include "shading/LoadShader.h"
#include "camera/OrthoCamera.h"

namespace glen
{
	/*

		Emits light in a single direction. Rays are parallel like the sun.

	*/
	struct DirectionalLight : public Light
	{
		// // ----- CONSTANTS ----- // //
		static const std::string TYPE;

		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		DirectionalLight();

		/*!
		 * @brief Constructor
		 * @param brightness Set the brightness of the light.
		 * @param color Set the color of the light with an r, g, b vector.
		 * Color values should be in the range 0 to 1.
		*/
		DirectionalLight(GLfloat brightness, glm::vec3 color);

		
		// // ----- GETTERS ----- // //

		/*!
		 * @brief Get the type of the light. Useful for partitioning lights when rendering.
		 * @return A string representing the type of the light.
		*/
		const std::string& type() const override;

		/*!
		 * @return The mesh object of the representation of the light.
		*/
		Mesh* mesh() override;

		/*!
		 * @return The material object of the representation of the light.
		*/
		Material* material() override;

		/*!
		 * @return The camera object used for shadow casting.
		*/
		Camera* camera() override;

		// // ----- MEMBER VARIABLES ----- // //
	private:

		// @cond
		
		Mesh m_light_mesh;
		LightMaterial m_material;
		OrthoCamera m_camera{ -10.0f, 10.0f, -10.0f, 10.0f };

		// @endcond
	};
}
#endif