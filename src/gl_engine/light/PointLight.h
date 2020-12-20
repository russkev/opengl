#ifndef GL_ENGINE_LIGHT_POINT_H
#define GL_ENGINE_LIGHT_POINT_H
						
#include "Light.h"
#include "mesh/Sphere.h"
#include "camera/FreeCamera.h"

namespace glen
{
	/*!

		Emits light in all directions.
		Uses sphere mesh to display location and size in scene

	*/
	struct PointLight : public Light
	{
		// // ----- CONSTANTS ----- // //
		static const std::string TYPE;
		static constexpr GLfloat CAMERA_ANGLE = 90.0f;

		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		PointLight();

		/*!
		 * @brief Constructor
		 * @param brightness Set the brightness of the light.
		 * @param color Set the color of the light with an r, g, b vector.
		 * Color values should be in the range 0 to 1.
		*/
		PointLight(const GLfloat brightness, const glm::vec3 color);

		// // ----- GETTERS ----- // //

		/*!
		 * @return The radius of the mesh representing the light
		*/
		const float& radius() const;

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

		
		// // ----- SETTERS ----- // //

		/*!
		 * @param radius The radius of the mesh representing the light
		 * @attention If the light has a shadow map, this value won't change the apparent
		 * softness of the shadow. You'll need to change the `ShadowMap` radius for that.
		*/
		void set_radius(const GLfloat radius);

		// // ----- MEMBER VARIABLES ----- // //
	private:

		// @cond

		float m_radius = 0.5f;
		Mesh m_light_mesh;
		LightMaterial m_material;
		FreeCamera m_camera;

		// @endcond
	};
} 
#endif