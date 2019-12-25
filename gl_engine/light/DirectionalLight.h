#ifndef GL_ENGINE_LIGHT_DIRECTIONAL_H
#define GL_ENGINE_LIGHT_DIRECTIONAL_H

#include <glm/glm.hpp>

#include "Light.h"

#include "../shading/LoadShader.h"
#include "../camera/OrthoCamera.h"

namespace gl_engine
{
	/*

		Emits light in a single direction. Rays are parallel like the sun.

	*/
	struct DirectionalLight : public Light
	{
	private:
		static const std::string TYPE;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Mesh m_light_mesh;
		Shader m_shader;
		OrthoCamera m_camera{ -10.0f, 10.0f, -10.0f, 10.0f };

		// // ----- CONSTRUCTORS ----- // //
	public:
		DirectionalLight(GLfloat brightness, glm::vec3 color);

		// // ----- GETTERS ----- // //
		const std::string& type() const override;
		Mesh* mesh() override;
		Shader* shader() override;
		Camera* camera() override;
	};
} // namespace gl_engine
#endif