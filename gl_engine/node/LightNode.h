#ifndef GL_ENGINE_NODE_LIGHT_H
#define GL_ENGINE_NODE_LIGHT_H

#include "Node.h"
#include "../VAO.h"
#include "../shading/Shader.h"

namespace gl_engine
{
	// Forward declare light
	struct Light;


	/*

		Light node type.

	*/
	struct LightNode : public Node
	{
		static const std::string LIGHT_POSITION;
		static const std::string LIGHT_DIRECTION;
	private:
		Light* m_light = NULL;
		VAO m_vao;
		Buffer m_vertexBuffer = { GL_ARRAY_BUFFER, 0 };
		Buffer m_indexBuffer = { GL_ARRAY_BUFFER, 0 };
		glm::mat4 m_modelToPerspectiveMatrix;
		bool m_shader_warned = false;

	public:
		LightNode() {};
		LightNode(const std::string name, Light* light);

		// // ----- GENERAL ----- // //
		void update_view(CameraNode* cameraNode) override;
		void draw() override;

		// // ----- GETTERS ----- // //
		Light* light();


	};
} // namespace gl_engine

#endif