#include "DirectionalLight.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../mesh/Arrow.h"
#include "../Buffer.h"

namespace gl_engine
{
	const std::string DirectionalLight::TYPE = "directional_light";

	DirectionalLight::DirectionalLight(GLfloat brightness, glm::vec3 color) :
		m_light_mesh(Arrow::createArrow(10.0)),
		m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
	{
		setBrightness(brightness);
		setColor(color);
		m_light_mesh.transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.05, 0.2)));
	}

	// // ----- GETTERS ----- // //
	const std::string& DirectionalLight::type() const
	{
		return TYPE;
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