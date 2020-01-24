#include "DepthMap.h"

#include "../shading/Framebuffer.h"

namespace gl_engine
{
	DepthMap::DepthMap(Framebuffer* backbuffer, const glm::uvec2* dimensions) :
		PostEffect::PostEffect(backbuffer),
		m_depth{ Texture::create_depth_null_texture(GL_TEXTURE_2D, dimensions) }
	{
		PostEffect::backbuffer_fbo()->bind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth.id(), 0);

		PostEffect::backbuffer_fbo()->check_bound_framebuffer();
		PostEffect::backbuffer_fbo()->unbind();
	}
}