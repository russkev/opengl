#include <algorithm>

#include "PointLight.h"

namespace gl_engine
{
	// // ----- CONSTANTS ----- // //
	const std::string PointLight::TYPE = "pointLight";

	// // ----- CONSTRUCTORS ----- // //

	PointLight::PointLight(const GLfloat brightness, const glm::vec3 color) :
		m_light_mesh(Sphere::create_sphere(m_radius)),
		m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
	{
		set_brightness(brightness);
		set_color(color);
		m_camera.set_angle_of_view(CAMERA_ANGLE);
	}


	// // ----- GETTERS ----- // //
	const float& PointLight::radius() const
	{
		return m_radius;
	}

	const std::string& PointLight::type() const
	{
		return TYPE;
	}

	Mesh* PointLight::mesh()
	{
		return &m_light_mesh;
	}

	Shader* PointLight::shader()
	{
		return &m_shader;
	}

	Camera* PointLight::camera()
	{
		return &m_camera;
	}

	// // ----- SETTERS ----- // //
	void PointLight::set_radius(const GLfloat radius)
	{
		if (radius < 0)
		{
			std::printf("WARNING: unable to set radius to %.3f, can't set to negative", radius);
		}
		else
		{
			m_radius = radius;
		}
	}
} //namespace gl_engine