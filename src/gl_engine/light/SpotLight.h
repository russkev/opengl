#ifndef GL_ENGINE_LIGHT_SPOT_H
#define GL_ENGINE_LIGHT_SPOT_H
#include "Light.h"
#include "shading/LoadShader.h"
#include "camera/FreeCamera.h"

namespace glen
{
	/*!

		Emits light in a cone shape like it's coming from a torch.

	*/
	struct SpotLight : public Light
	{
		// // ----- CONSTANTS ----- // //
	public:

		// @cond

		static const std::string INNER;
		static const std::string OUTER;
		static const std::string TYPE;

		// @endcond

		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		SpotLight();

		/*!
		 * @brief Constructor
		 * @param brightness Set the brightness of the light.
		 * @param color Set the color of the light with an r, g, b vector.
		 * Color values should be in the range 0 to 1.
		*/
		SpotLight(GLfloat brightness, glm::vec3 color);


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

		/*!
		 * @return Number between 0 and 1 represnting the inner angle of the 
		 * [penumbra](https://en.wikipedia.org/wiki/Umbra,_penumbra_and_antumbra)
		*/
		const float cos_inner_angle() const;

		/*!
		 * @return Number between 0 and 1 represnting the outer angle of the
		 * [penumbra](https://en.wikipedia.org/wiki/Umbra,_penumbra_and_antumbra)
		*/
		const float cos_outer_angle() const;

		
		// // ----- SETTERS ----- // //

		/*!
		 * @param theta The inner angle (in radians) of the 
		 * [penumbra](https://en.wikipedia.org/wiki/Umbra,_penumbra_and_antumbra)
		*/
		void set_inner_angle(const GLfloat theta);

		/*!
		 * @param theta The outer angle (in radians) of the
		 * [penumbra](https://en.wikipedia.org/wiki/Umbra,_penumbra_and_antumbra)
		*/
		void set_outer_angle(const GLfloat theta);

	private:
		void set_camera_angle();

		// // ----- MEMBER VARIABLES ----- // //
	private:

		// @cond 

		GLfloat m_inner_angle = 30.0f;
		GLfloat m_outer_angle = 35.0f;
		Mesh m_light_mesh;
		LightMaterial m_material;
		FreeCamera m_camera;

		// @endcond
	};
} 
#endif