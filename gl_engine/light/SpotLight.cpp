#include "SpotLight.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../mesh/Cylinder.h"


namespace gl_engine
{

	const std::string SpotLight::TYPE = "spot_light";
	const std::string SpotLight::INNER = ".inner";
	const std::string SpotLight::OUTER = ".outer";


	SpotLight::SpotLight(GLfloat brightness, glm::vec3 color) :
		m_light_mesh(Cylinder::createCylinder()),
		m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
	{
		setBrightness(brightness);
		setColor(color);

		// Rotate mesh so that it points the right direction
		m_light_mesh.transform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.1f, 1.0f)));
		m_light_mesh.transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	}

	// // ----- GETTERS ----- // //
	const std::string& SpotLight::type() const
	{
		return TYPE;
	}

	Mesh* SpotLight::mesh()
	{
		return &m_light_mesh;
	}
	Shader* SpotLight::shader()
	{
		return &m_shader;
	}

	const float SpotLight::cos_innerAngle() const
	{
		return std::cos(glm::radians(m_innerAngle));
	}

	const float SpotLight::cos_outerAngle() const
	{
		return std::cos(glm::radians(m_outerAngle));
	}

	// // ----- SETTERS ----- // //
	void SpotLight::set_innerAngle(const GLfloat theta)
	{
		m_innerAngle = theta;
		if (m_outerAngle < m_innerAngle)
		{
			m_outerAngle = m_innerAngle;
		}
	}

	void SpotLight::set_outerAngle(const GLfloat theta)
	{
		m_outerAngle = theta;
		if (m_outerAngle < m_innerAngle)
		{
			m_innerAngle = m_outerAngle;
		}
	}
} // namespace gl_engine