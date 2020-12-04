#include "pch.h"
#include "SpotLight.h"

#include <glm/gtc/matrix_transform.hpp>

#include "mesh/Cylinder.h"


namespace glen
{
	// // ----- CONSTANTS ----- // //
	const std::string SpotLight::TYPE = "spotLight";
	const std::string SpotLight::INNER = "inner";
	const std::string SpotLight::OUTER = "outer";

	// // ----- CONSTRUCTORS ----- // //
	SpotLight::SpotLight() :
		SpotLight(1.0f, { 1.0f, 1.0f, 1.0f })
	{}

	SpotLight::SpotLight(GLfloat brightness, glm::vec3 color) :
		m_light_mesh{ Cylinder::create_cylinder() },
		m_material{ LightMaterial{ "lightShader" } }
	{
		set_brightness(brightness);
		set_color(color);

		// Rotate mesh so that it points the right direction
		m_light_mesh.transform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.1f, 1.0f)));
		m_light_mesh.transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		// Adjust camera angle of view
		set_camera_angle();
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
	Material* SpotLight::material()
	{
		return &m_material;
	}
	
	Camera* SpotLight::camera()
	{
		return &m_camera;
	}

	const float SpotLight::cos_inner_angle() const
	{
		return std::cos(glm::radians(m_inner_angle));
	}

	const float SpotLight::cos_outer_angle() const
	{
		return std::cos(glm::radians(m_outer_angle));
	}

	// // ----- SETTERS ----- // //
	void SpotLight::set_inner_angle(const GLfloat theta)
	{
		m_inner_angle = theta;
		if (m_outer_angle < m_inner_angle)
		{
			m_outer_angle = m_inner_angle;
		}
		set_camera_angle();

	}

	void SpotLight::set_outer_angle(const GLfloat theta)
	{
		m_outer_angle = theta;
		if (m_outer_angle < m_inner_angle)
		{
			m_inner_angle = m_outer_angle;
		}
		set_camera_angle();
	}

	void SpotLight::set_camera_angle()
	{
		m_camera.set_angle_of_view(m_outer_angle * 2);
	}
} 