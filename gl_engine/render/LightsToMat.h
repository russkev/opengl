#ifndef GL_ENGINE_RENDER_LIGHT_TO_MAT
#define GL_ENGINE_RENDER_LIGHT_TO_MAT

#include <set>

#include "../node/lightNode.h"
#include "../shading/Material.h"

namespace gl_engine
{
	void lights_to_mat(const std::vector<LightNode*>* light_nodes, const Material* material)
	{
		for (LightNode* light_node : *light_nodes)
		{
			//
		}
	}
}

#endif