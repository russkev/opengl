#ifndef GL_ENGINE_CAMERA_CAMERA_H
#define GL_ENGINE_CAMERA_CAMERA_H

#include "pch.h"

namespace glen
{
	/*
	
		Abstract camera node. Provides camera basic funcrionality
	
	*/
	struct Camera
	{
	public:
		// // ----- CONSTRUCTOR ----- // //
		/*!
		* Construct a basic camera
		*/
		Camera() {};

		// // ----- CAMERA MOVEMENT ----- // //
		virtual void update() = 0;

		// // ----- GETTERS ----- // //
		const glm::uvec2& dimensions() const;
		const GLfloat& clip_near() const;
		const GLfloat& clip_far() const;
		virtual const glm::vec3 position() const;
		const glm::mat4 transform() const;

		const glm::mat4 object_to_cam() const;
		const glm::mat4 object_to_projection() const;
		virtual const glm::mat4 cam_to_projection() const = 0;
		virtual const glm::mat4 transform_to_cam(const glm::mat4& transform) const = 0;
		const glm::mat4 transform_to_projection(const glm::mat4& transform) const;

		// // ----- SETTERS ----- // //
		void set_dimensions(glm::uvec2);
		void set_clip_near(GLfloat);
		void set_clip_far(GLfloat);
		void set_position(const glm::vec3& position);
		void add_to_position(const glm::vec3& position);
		void register_transform(glm::mat4* transform);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		glm::uvec2 m_dimensions = { 800u, 600u };
		GLfloat m_clip_near = 0.1f;
		GLfloat m_clip_far = 1000.0f;

		glm::mat4 m_default_transform = glm::mat4{ 1.0f };
		glm::mat4* m_parent_transform = &m_default_transform;
	};
}
#endif