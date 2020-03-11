#include "pch.h"
#include "Light.h"

#include "camera/FreeCamera.h"

namespace glen
{
	// // ----- GETTERS ----- // //
	const GLfloat& Light::brightness() const
	{
		return m_brightness;
	}

	const glm::vec3& Light::color() const
	{
		return m_color;
	}

	Mesh* Light::mesh()
	{
		return NULL;
	}

	Material* Light::material()
	{
		return NULL;
	}

	const bool& Light::mesh_enabled() const
	{
		return m_mesh_enabled;
	}

	Camera* Light::camera()
	{
		return NULL;
	}

	const bool& Light::diffuse_enabled() const
	{
		return m_diffuse_enabled;
	}

	const bool& Light::specular_enabled() const
	{
		return m_specular_enabled;
	}


	// // ----- SETTERS ----- // //
	void Light::set_brightness(const GLfloat brightness)
	{
		m_brightness = brightness;
		if (material() != NULL)
		{
			material()->set_uniform(std::string(k_light_shader_type + "." + k_light_brightness), m_brightness);
		}
	}

	void Light::set_color(const glm::vec3 color)
	{
		m_color = color;
		if (material() != NULL)
		{
			material()->set_uniform(std::string(k_light_shader_type + "." + k_light_color), m_color);
		}
	}

	void Light::enable_mesh()
	{
		m_mesh_enabled = true;
	}

	void Light::disable_mesh()
	{
		m_mesh_enabled = false;
	}

	void Light::enable_diffuse()
	{
		m_diffuse_enabled = true;
	}

	void Light::disable_diffuse()
	{
		m_diffuse_enabled = false;
	}

	void Light::enable_specular()
	{
		m_specular_enabled = true;
	}

	void Light::disable_specular()
	{
		m_specular_enabled = false;
	}
}