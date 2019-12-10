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

	Light* light();
	void update_view(Camera* camera) override;
	void draw() override;

};

#endif