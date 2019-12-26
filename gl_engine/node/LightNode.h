#ifndef GL_ENGINE_NODE_LIGHT_H
#define GL_ENGINE_NODE_LIGHT_H

#include <memory>

#include "Node.h"
#include "../VAO.h"
#include "../shading/Shader.h"

namespace gl_engine
{
	// Forward declare light
	struct Light;
	struct ShadowMap;
	struct CameraNode;


	/*

		Light node type.

	*/
	struct LightNode : public Node
	{
		static const std::string LIGHT_POSITION;
		static const std::string LIGHT_DIRECTION;
	private:
		Light* m_light = NULL;
		ShadowMap* m_shadowMap = NULL;
		GLuint m_shaderIndex;

		VAO m_vao;
		Buffer m_vertexBuffer = { GL_ARRAY_BUFFER, 0 };
		Buffer m_indexBuffer = { GL_ARRAY_BUFFER, 0 };

		glm::mat4 m_modelToPerspective_matrix;
		bool m_shader_warned = false;

	public:
		LightNode() {};
		LightNode(const std::string name, Light* light);

		// // ----- GENERAL ----- // //

		void update_view(CameraNode* cameraNode) override;
		void draw(const Pass& pass = rgb) override;

		// // ----- GETTERS ----- // //
		Light* light();
		ShadowMap* shadowMap();
		const GLuint shaderIndex() const;

		// // ----- SETTERS ----- // //
		void set_shadowMap(ShadowMap* shadowMap);
		void set_shaderIndex(const GLuint index);
	};
} // namespace gl_engine

#endif