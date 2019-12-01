#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "PointLight.h"



// // ----- CONSTRUCTORS ----- // //
PointLight::PointLight()
{}

PointLight::PointLight(const glm::vec3& position) :
	m_position(position)
{}

PointLight::PointLight(const float brightness, const glm::vec3& position) :
	m_brightness(brightness > 0 ? brightness : 0),
	m_position(position)
{}

PointLight::PointLight(const float brightness, const float radius, const glm::vec3& position) :
	m_brightness(brightness > 0 ? brightness : 0),
	m_radius(radius > 0 ? radius : 0.1f),
	m_position(position)
{}

PointLight::PointLight(const float brightness, const float radius, const glm::vec3& position, const bool displayMesh) :
	m_brightness(brightness > 0? brightness : 0), 
	m_radius(radius > 0? radius : 0.1f), 
	m_position(position),
	m_displayMesh(displayMesh)
{}


// // ----- GETTERS ----- // //
const float& PointLight::brightness() const
{
	return m_brightness;
}

const float& PointLight::radius() const
{
	return m_brightness;
}

const glm::vec3& PointLight::position() const
{
	return m_position;
}
const glm::mat4& PointLight::transformMatrix() const
{
	return glm::translate(glm::mat4(), m_position);
}

// // ----- SETTERS ----- // //
float PointLight::brightness()
{
	return m_brightness;
}

float PointLight::radius()
{
	return m_radius;
}

glm::vec3& PointLight::position()
{
	return m_position;
}

void PointLight::transform(const glm::mat4& transformMatrix)
{
	m_position = { transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2] };
}