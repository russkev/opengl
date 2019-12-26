#ifndef GL_ENGINE_LIGHT_SPOT_H
#define GL_ENGINE_LIGHT_SPOT_H

#include <glm/glm.hpp>

#include "Light.h"

#include "../shading/LoadShader.h"
#include "../camera/FreeCamera.h"

namespace gl_engine
{
	/*

		Emits light in a cone shape like it's coming from a torch.

	*/
	struct SpotLight : public Light
	{
	public:
		static const std::string INNER;
		static const std::string OUTER;
		static const std::string TYPE;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		GLfloat m_innerAngle = 30.0f;
		GLfloat m_outerAngle = 35.0f;
		Mesh m_light_mesh;
		Shader m_shader;
		FreeCamera m_camera;

		// // ----- CONSTRUCTORS ----- // //
	public:
		SpotLight(GLfloat brightness, glm::vec3 color);

		// // ----- GETTERS ----- // //
		const std::string& type() const override;

		Mesh* mesh() override;
		Shader* shader() override;
		Camera* camera() override;

		const float cos_innerAngle() const;
		const float cos_outerAngle() const;

		// // ----- SETTERS ----- // //
		void set_innerAngle(const GLfloat theta);
		void set_outerAngle(const GLfloat theta);

	private:
		void set_cameraAngle();
	};
} // namespace gl_engine
#endif