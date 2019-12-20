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
		GLfloat m_clipNear = 0.1f;
		GLfloat m_clipFar = 10.0f;

		glm::mat4 m_default_transform = glm::mat4{ 1.0f };
		glm::mat4* m_transform = &m_default_transform;

	public:
		// // ----- CONSTRUCTOR ----- // //
		Camera() {};

		// // ----- CAMERA MOVEMENT ----- // //
		virtual void update() = 0;

		// // ----- GETTERS ----- // //
		const glm::uvec2& dimensions() const;
		const GLfloat& clipNear() const;
		const GLfloat& clipFar() const;
		virtual const glm::vec3 position() const;
		glm::mat4* transform();

		glm::mat4 objectToCam_matrix();
		glm::mat4 objectToProjection_matrix();

		virtual glm::mat4 camToProjection_matrix() = 0;

		virtual glm::mat4 transformToCam_matrix(const glm::mat4& transform) = 0;
		glm::mat4 transformToProjection_matrix(const glm::mat4& transform);

		// // ----- SETTERS ----- // //
		void setDimensions(glm::uvec2);
		void setClipNear(GLfloat);
		void setClipFar(GLfloat);
		void setPosition(const glm::vec3& position);
		void addPosition(const glm::vec3& position);
		void registerTransform(glm::mat4* transform);

	};
} // namespace gl_engine

#endif