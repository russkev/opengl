#ifndef GL_ENGINE_NODE_CAMERA_H
#define GL_ENGINE_NODE_CAMERA_H

#include "Node.h"
#include "resources/Buffer.h"

namespace glen
{
	// // ----- FORWARD DECLERATION ----- // //
	struct Camera;

	/*!

		Camera node. Inherits from `Node`.
		Required for adding a camera to a scene.

	*/
	struct CameraNode : public Node
	{
		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		CameraNode(const std::string name, Camera* camera);

		// // ----- GENERAL METHODS ----- // //

		/*!
		 * @brief Run update command on camera.
		*/
		void update();

		// // ----- GETTERS ----- // //

		/*!
		 * @return The camera object.
		*/
		const Camera* camera() const;

		/*!
		 * @return The camera object (mutable).
		*/
		Camera* camera();

		const glm::vec3 world_position() const override;

		/*!
		 * @return The world to camera transform matrix.
		*/
		const glm::mat4 world_to_cam() const;

		/*!
		 * @return The world to projection transfrom matrix.
		*/
		const glm::mat4 world_to_projection() const;

		
		// // ----- SETTERS ----- // //
		void set_position(const glm::vec3& position) override;


		// // ----- MEMBER VARIABLES ----- // //
	private:
		Camera* m_camera{ NULL };
	};
}
#endif