#ifndef GL_ENGINE_LIGHT_SPOT_H
#define GL_ENGINE_LIGHT_SPOT_H

#include <glm/glm.hpp>

#include "Light.h"
#include "shading/LoadShader.h"
#include "camera/FreeCamera.h"

namespace glen
{
	/*

		Emits light in a cone shape like it's coming from a torch.

	*/
	struct SpotLight : public Light
	{
		// // ----- CONSTANTS ----- // //
	public:
		static const std::string INNER;
		static const std::string OUTER;
		static const std::string TYPE;

		// // ----- CONSTRUCTORS ----- // //
	public:
		SpotLight(GLfloat brightness, glm::vec3 color);

		// // ----- GETTERS ----- // //
		const std::string& type() const override;

		Mesh* mesh() override;
		Material* material() override;
		Camera* camera() override;

		const float cos_inner_angle() const;
		const float cos_outer_angle() const;

		// // ----- SETTERS ----- // //
		void set_inner_angle(const GLfloat theta);
		void set_outer_angle(const GLfloat theta);
		void set_light_mesh_scale(const GLfloat scale);

	private:
		void set_camera_angle();

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLfloat m_inner_angle = 30.0f;
		GLfloat m_outer_angle = 35.0f;
		Mesh m_light_mesh;
		LightMaterial m_material;
		FreeCamera m_camera;
	};
} 
#endif