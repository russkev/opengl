#include "pch.h"
#include "PointLight.h"

namespace glen
{
	// // ----- CONSTANTS ----- // //
	const std::string PointLight::TYPE = "pointLight";

	// // ----- CONSTRUCTORS ----- // //
	PointLight::PointLight() :
		PointLight(1.0f, { 1.0f, 1.0f, 1.0f })
	{}

	PointLight::PointLight(const GLfloat brightness, const glm::vec3 color) :
		m_light_mesh(Sphere::create_sphere(m_radius)),
		m_material(LightMaterial("lightShader"))
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

	Material* PointLight::material()
	{
		return &m_material;
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
}