#include "DirectionalLight.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../mesh/Arrow.h"
#include "../Buffer.h"
#include "../camera/OrthoCamera.h"

namespace gl_engine
{
	// // ----- CONSTANTS ----- // //
	const std::string DirectionalLight::TYPE = "directionalLight";

	// // ----- CONSTRUCTORS ----- // //
	DirectionalLight::DirectionalLight(GLfloat brightness, glm::vec3 color) :
		m_light_mesh(Arrow::create_arrow(10.0)),
		m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
	{
		set_brightness(brightness);
		set_color(color);
	}

	// // ----- GETTERS ----- // //
	const std::string& DirectionalLight::type() const
	{
		return TYPE;
	}

	Camera* DirectionalLight::camera()
	{
		return &m_camera;
	}

	Mesh* DirectionalLight::mesh()
	{
		return &m_light_mesh;
	}
	Shader* DirectionalLight::shader()
	{
		return &m_shader;
	}
} // namespace gl_engine