#ifndef GL_ENGINE_NODE_LIGHT_H
#define GL_ENGINE_NODE_LIGHT_H

#include "Node.h"
#include "resources/VAO.h"

namespace glen
{
	// // ----- FORWARD DECLARATION ----- // //
	struct Light;
	struct ShadowMap;
	struct CameraNode;


	/*!

		Light node type. Inherits from `Node`.
		Required for adding a light to the scene

	*/
	struct LightNode : public Node
	{
		// @cond

		// // ----- CONSTANTS ----- // //
		static const std::string LIGHT_POSITION;
		static const std::string LIGHT_DIRECTION;

		// @endcond

		// // ----- CONSTRUCTORS ----- // //
	public:
		/*!
		 * @brief Constructor
		*/
		LightNode(const std::string name, Light* light);

		// // ----- GENERAL ----- // //

		void update_view(CameraNode* camera_node) override;

		/*!
		 * @brief Render the light mesh (if enabled).
		*/
		void draw() override;

		// // ----- GETTERS ----- // //

		/*!
		 * @return The light object.
		*/
		const Light* light() const;

		/*!
		 * @return The light object (mutable).
		*/
		Light* light();

		/*!
		 * @return The shadow map object (mutable).
		*/
		ShadowMap* shadowMap();

		/*!
		 * @return The index used in some shaders (`BlinnShader` for 
		 * instance) to identify this light
		*/
		const GLuint shader_pos() const;

		// // ----- SETTERS ----- // //

		/*!
		 * @brief Add a `shadowMap` object to the node so the light 
		 * can cast shadows
		 * @param shadowMap 
		*/
		void set_shadowMap(ShadowMap* shadowMap);

		/*!
		 * @param index The index used in some shaders (`BlinnShader` for 
		 * instance) to identify this light
		*/
		void set_shader_pos(const GLuint index);

		// // ----- MEMBER VARIABLES ----- // //
	private:
		Light* m_light = NULL;
		ShadowMap* m_shadow_map = NULL;
		GLuint m_shader_pos;

		VAO m_vao;
		Buffer m_vertex_buffer{ GL_ARRAY_BUFFER, 0 };
		Buffer m_index_buffer{ GL_ARRAY_BUFFER, 0 };

		glm::mat4 m_model_to_perspective{ 1.0 };
		bool m_shader_warned{ false };
	};
}
#endif