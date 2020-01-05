#ifndef GL_ENGINE_CAMERA_CAMERA_H
#define GL_ENGINE_CAMERA_CAMERA_H

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace gl_engine
{
	/*
	
		Abstract camera node. Provides camera basic funcrionality
	
	*/
	struct Camera
	{
	private:

	private:
		// // ----- MEMBER VARIABLES ----- // //
		glm::uvec2 m_dimensions = { 800u, 600u };
		GLfloat m_clip_near = 0.1f;
		GLfloat m_clip_far = 10.0f;

		glm::mat4 m_default_transform = glm::mat4{ 1.0f };
		glm::mat4* m_transform = &m_default_transform;

	public:
		// // ----- CONSTRUCTOR ----- // //
		Camera() {};

		// // ----- CAMERA MOVEMENT ----- // //
		virtual void update() = 0;

		// // ----- GETTERS ----- // //
		const glm::uvec2& dimensions() const;
		const GLfloat& clip_near() const;
		const GLfloat& clip_far() const;
		virtual const glm::vec3 position() const;
		glm::mat4* transform();

		glm::mat4 object_to_cam();
		glm::mat4 object_to_projection();
		virtual glm::mat4 cam_to_projection() = 0;
		virtual glm::mat4 transform_to_cam(const glm::mat4& transform) = 0;
		glm::mat4 transform_to_projection(const glm::mat4& transform);

		// // ----- SETTERS ----- // //
		void set_dimensions(glm::uvec2);
		void set_clip_near(GLfloat);
		void set_clip_far(GLfloat);
		void set_position(const glm::vec3& position);
		void add_to_position(const glm::vec3& position);
		void register_transform(glm::mat4* transform);

	};
} // namespace gl_engine

#endif