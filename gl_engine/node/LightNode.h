#ifndef GL_ENGINE_NODE_LIGHT_H
#define GL_ENGINE_NODE_LIGHT_H

#include <memory>

#include "Node.h"
#include "../VAO.h"
#include "../shading/Shader.h"

namespace gl_engine
{
	// // ----- FORWARD DECLARATION ----- // //
	struct Light;
	struct ShadowMap;
	struct CameraNode;


	/*

		Light node type.

	*/
	struct LightNode : public Node
	{
		// // ----- STATICS ----- // //
		static const std::string LIGHT_POSITION;
		static const std::string LIGHT_DIRECTION;

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Light* m_light = NULL;
		ShadowMap* m_shadow_map = NULL;
		GLuint m_shader_pos;

		VAO m_vao;
		Buffer m_vertex_buffer = { GL_ARRAY_BUFFER, 0 };
		Buffer m_index_buffer = { GL_ARRAY_BUFFER, 0 };

		glm::mat4 m_model_to_perspective;
		bool m_shader_warned = false;

		// // ----- CONSTRUCTORS ----- // //
	public:
		LightNode() {};
		LightNode(const std::string name, Light* light);

		// // ----- GENERAL ----- // //

		void update_view(CameraNode* camera_node) override;
		void draw(const Pass& pass = rgb) override;

		// // ----- GETTERS ----- // //
		Light* light();
		ShadowMap* shadowMap();
		const GLuint shader_pos() const;

		// // ----- SETTERS ----- // //
		void set_shadowMap(ShadowMap* shadowMap);
		void set_shader_pos(const GLuint index);
	};
} // namespace gl_engine

#endif