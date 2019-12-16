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
		// // ----- MEMBER VARIABLES ----- // //
		glm::uvec2 m_dimensions = { 800u, 600u };
		GLfloat m_clipNear = 0.1f;
		GLfloat m_clipFar = 10.0f;

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
		virtual glm::mat4 worldToCam_matrix() = 0;
		virtual glm::mat4 viewToProjection_matrix() = 0;
		virtual glm::mat4 worldToProjection_matrix() = 0;

		// // ----- SETTERS ----- // //
		void setDimensions(glm::uvec2);
		void setClipNear(GLfloat);
		void setClipFar(GLfloat);

	};
} // namespace gl_engine

#endif