#pragma once

#include <glm/glm.hpp>

#include "../mesh/Sphere.h"

/*
	Emits light in all directions.
	Uses sphere mesh to display location and size in scene

*/

struct Light
{
	Light(const float brightness, const float radius, const glm::vec3& position);

	// // ----- GETTERS ----- // //
	const float& brightness() const;
	const float& radius() const;
	const glm::vec3& position() const;
	const glm::vec4& transformMatrix() const;

	// // ----- SETTERS ----- // //
	float brightness();
	float radius();
	glm::vec3& position();


private:
	float m_brightness;
	float m_radius;
	glm::vec3 m_position;
	Mesh m_lightMesh;
};