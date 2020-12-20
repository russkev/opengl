#ifndef GL_ENGINE_CAMERA_CAMERA_H
#define GL_ENGINE_CAMERA_CAMERA_H

#include "pch.h"

namespace glen
{
	/*!
	
		Abstract camera node. Provides camera basic funcrionality.

	*/
	struct Camera
	{
	public:
		// // ----- CONSTRUCTOR ----- // //
		/*!
		* Constructs a camera
		* By default cameras have a resolution of 800x600 pixels, 
		* a near and far clipping plane 0.1 and 1000 units respectively.
		*/
		Camera() {};

		// // ----- CAMERA MOVEMENT ----- // //
		/*! 
		* Update the camera position.
		* Accept input from the user to move the camera around.
		*/
		virtual void update() = 0;

		// // ----- GETTERS ----- // //
		/*!
		* \returns the dimensions of the camera.
		*/
		const glm::uvec2& dimensions() const;

		/*!
		* \returns the near clipping plane of the camera. Anything further away than this from the camera 
		* won't be rendered.		
		*/
		const GLfloat& clip_near() const;

		/*!
		* \returns the far clipping plane of the camera. Anything closer than this won't be rendered.
		*/
		const GLfloat& clip_far() const;

		/*!
		* \returns the x, y, z coordinates in local space (relative to camera node).
		*/
		virtual const glm::vec3 position() const;

		/*!
		* \returns the transform matrix in local space (relative to camera node).
		*/
		const glm::mat4 transform() const;

		/*!
		* \returns the transformation matrix to go from object space to camera space.
		*/
		const glm::mat4 object_to_cam() const;

		/*!
		* \returns the transformation matrix from object to camera to projection. 
		* The projection matrix is the matrix required to transform the scene into a perspective view.
		*/
		const glm::mat4 object_to_projection() const;

		/*!
		* \returns the transformation matrix from the camera to projection.
		* The projection matrix is the matrix required to transform the scene into a perspective view.
		*/
		virtual const glm::mat4 cam_to_projection() const = 0;

		/*!
		* \returns the transformation matrix from \a input transform to the camera transform.
		*/
		virtual const glm::mat4 transform_to_cam(const glm::mat4& transform) const = 0;

		/*!
		* \returns the transformation matrix from \a input matrix to projection.
		*/
		const glm::mat4 transform_to_projection(const glm::mat4& transform) const;

		// // ----- SETTERS ----- // //
		/*!
		* Sets the dimensions of the camera
		* Please note, this is mainly used for determining the aspect ratio of the camera view.
		* Set the actual resolution in `Window` to affect the view.
		* \param Dimensions The width and height of the camera's perspective.
		*/
		void set_dimensions(glm::uvec2 dimensions);

		/*!
		* Sets the far clipping plane. Anything beyond this won't be rendered.
		* Ideally set the near and far clipping planes just enough to encompass everything required
		* in the scene and no more. If the difference is too large, you will start to see artefacts.
		* \param distance Clipping plane distance
		*/
		void set_clip_near(GLfloat distance);

		/*!
		* Sets the near clipping plane. Anything closer to this won't be rendered.
		* Ideally set the near and far clipping planes just enough to encompass everything required
		* in the scene and no more. If the difference is too large, you will start to see artefacts.
		* \param distance Clipping plane difference
		*/
		void set_clip_far(GLfloat distance);

		/*!
		* Set the local position of the camera (relative to the camera node).
		* \param position The new position in x, y, z vector coordinates.
		*/
		void set_position(const glm::vec3& position);

		/*!
		* Take input position vector and add it to the camera's current position.
		* Useful for implementing extra camera movement.
		* \param position The offset position in x, y, z vector coordinates.
		*/
		void add_to_position(const glm::vec3& position);

		/*!
		* Used for connecting to a `CameraNode`. Allows for some useful transform matrices to be calculated.
		* \param transform The transform matrix to register.
		*/
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