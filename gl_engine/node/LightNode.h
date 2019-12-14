#ifndef GL_ENGINE_NODE_LIGHT_H
#define GL_ENGINE_NODE_LIGHT_H

#include "Node.h"
#include "../VAO.h"


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

	GLuint m_depthMap_FBO;
	GLuint m_depthMap_ID;

public:
	LightNode() {};
	LightNode(const std::string name, Light* light);

	// // ----- SHADOW MAP ----- // //
public:
	static constexpr GLuint SHADOW_WIDTH = 1024;
	static constexpr GLuint SHADOW_HEIGHT = 1024;

	void init_shadowMap();
	void render_shadowMap();

	// // ----- GENERAL ----- // //
	void update_view(Camera* camera) override;
	void draw() override;

	// // ----- GETTERS ----- // //
	Light* light();


};

#endif