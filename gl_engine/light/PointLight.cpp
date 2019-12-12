#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "PointLight.h"


// // ----- STATICS ----- // //
const std::string PointLight::TYPE = "light";

const std::string PointLight::LIGHT_COLOR = std::string(TYPE + ".color");
const std::string PointLight::LIGHT_BRIGHTNESS = std::string(TYPE + ".brightness");

//Shader PointLight::m_shader = Shader("lightShader", "LightMesh.vert", "LightMesh.frag");

// // ----- CONSTRUCTORS ----- // //

PointLight::PointLight(const GLfloat brightness, const glm::vec3 color) :
	m_brightness(brightness),
	m_color(brightness),
	m_light_mesh(Sphere::createSphere(m_radius)),
	m_shader(Shader("lightShader", "lightShader.vert", "lightShader.frag"))
{}


// // ----- GETTERS ----- // //
const GLfloat& PointLight::brightness() const
{
	return m_brightness;
}

const glm::vec3& PointLight::color() const
{
	return m_color;
}


const float& PointLight::radius() const
{
	return m_radius;
}

const std::string& PointLight::type() const
{
	return TYPE;
}

//std::vector<std::string> PointLight::shader_commands()
//{
//	//std::map<std::string, void*> to_send;
//	//to_send.insert("point_light.brightness", (void*)Light::brightness());
//	//return
//	//{
//	//	std::string(TYPE + ".brightness"
//	//	"sdsds"
//	//};
//}

Mesh* PointLight::mesh()
{
	return &m_light_mesh;
}
Shader* PointLight::shader()
{
	return &m_shader;
}

// // ----- SETTERS ----- // //
void PointLight::setRadius(const GLfloat radius)
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

void PointLight::setBrightness(const GLfloat brightness)
{
	m_brightness = brightness;
	if (shader() != NULL)
	{
		shader()->setUniform(LIGHT_BRIGHTNESS, m_brightness);
	}
}

void PointLight::setColor(const glm::vec3 color)
{
	m_color = color;
	if (shader() != NULL)
	{
		shader()->setUniform(LIGHT_COLOR, m_color);
	}
}