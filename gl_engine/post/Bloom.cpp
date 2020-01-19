#include "Bloom.h"

#include "../shading/Framebuffer.h"

namespace gl_engine
{
	Bloom::Bloom(Framebuffer* backbuffer, const glm::uvec2* dimensions, ToneMap* tone_map) :
		PostEffect::PostEffect(backbuffer)
	{
		m_material.set_texture("color", tone_map->beauty());
		m_material.set_texture("threshold", tone_map->bright());
	}

	void Bloom::draw()
	{
		m_mesh_node.draw();
	}
}
