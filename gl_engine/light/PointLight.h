#pragma once

#include <glm/glm.hpp>

#include "Light.h"

#include "../shading/LoadShader.h"
#include "../mesh/Sphere.h"
#include "../mesh/Cube.h"

/*
	Emits light in all directions.
	Uses sphere mesh to display location and size in scene
*/
struct PointLight : Light
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	float m_radius = 1.0f;
	Mesh m_lightMesh = Sphere::createSphere(1.0f);
	const static Shader m_shader;

	// // ----- CONSTRUCTORS ----- // //
	PointLight(const float brightness, const glm::vec3 color, const GLfloat radius);

	// // ----- GETTERS ----- // //
	const float& radius() const;
	const Mesh* mesh_ptr() const;
	const Shader* shader_ptr() const;

	// // ----- SETTERS ----- // //
	void setRadius(const GLfloat radius);
};