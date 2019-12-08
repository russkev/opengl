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
struct PointLight : public Light
{
	// // ----- MEMBER VARIABLES ----- // //
private:
	float m_radius = 1.0f;
	Mesh m_light_mesh;
	Shader m_shader;

	// // ----- CONSTRUCTORS ----- // //
public:
	PointLight(const GLfloat brightness, const glm::vec3 color, const GLfloat radius);

	// // ----- GETTERS ----- // //
	const float& radius() const;
	Mesh* mesh_ptr() override;
	//Shader* shader_ptr() override;

	// // ----- SETTERS ----- // //
	void setRadius(const GLfloat radius);
};