#ifndef GL_ENGINE_LIGHT_POINT_H
#define GL_ENGINE_LIGHT_POINT_H
						
#include <glm/glm.hpp>

#include "Light.h"

#include "../shading/LoadShader.h"
#include "../mesh/Sphere.h"
#include "../camera/FreeCamera.h"

namespace gl_engine
{
	/*

		Emits light in all directions.
		Uses sphere mesh to display location and size in scene

	*/
	struct PointLight : public Light
	{
		// // ----- CONSTANTS ----- // //
		static const std::string TYPE;
		static constexpr GLfloat CAMERA_ANGLE = 90.0f;

		// // ----- CONSTRUCTORS ----- // //
	public:
		PointLight(const GLfloat brightness, const glm::vec3 color);

		// // ----- GETTERS ----- // //
		const float& radius() const;

		const std::string& type() const override;

		Mesh* mesh() override;
		Shader* shader() override;
		Camera* camera() override;

		// // ----- SETTERS ----- // //	
		void set_radius(const GLfloat radius);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		float m_radius = 0.5f;
		Mesh m_light_mesh;
		Shader m_shader;
		FreeCamera m_camera;
	};
} // namespace gl_engine
#endif