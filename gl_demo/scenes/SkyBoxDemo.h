#ifndef GL_DEMO_SKY_BOX
#define GL_DEMO_SKY_BOX

#include "gl_engine.h"

namespace gl_demo
{
	static void skybox_demo()
	{
		GLuint width = 800u;
		GLuint height = 600u;

		glen::Window window{ "GL Engine", width, height };

		// Target Camera
		glen::TargetCamera targetCam{};
		glen::CameraNode targetCam_node{ "Target Camera 1", &targetCam };
		targetCam.set_position({ 0.0f, 8.0f, 8.0f });
		targetCam.set_focus_target({ 0.0f, 0.0f, 0.0f });
		targetCam.focus(glm::vec3{ 0.0f, 0.0f, 0.0f });
		targetCam.set_clip_far(1000.0f);

		//glen::Texture uv_template_texture{ "leather_09_diffuse.tga" };

		glen::CubeMapMaterial skybox_material{ "Skybox Material" };
		glen::Texture skybox_texture{ glen::Texture::create_cubemap_texture({
										"pond/posx.tga",
										"pond/negx.tga",
										"pond/posy.tga",
										"pond/negy.tga",
										"pond/posz.tga",
										"pond/negz.tga"
										})};
		glen::Mesh skybox_mesh = glen::Cube::create_cube();
		skybox_mesh.reverse_triangles();
		glm::mat4 skybox_transform = glm::translate(glm::mat4{ 1.0f  }, glm::vec3{ 0.0f, -1.0f, 0.0f });
		skybox_mesh.transform(skybox_transform);
		skybox_material.set_texture(skybox_material.k_skybox, &skybox_texture);
		glen::MeshNode skybox_mesh_node{ "Skybox node", &skybox_mesh, &skybox_material };

		glen::Renderer render{ &targetCam_node, glm::uvec2{ width, height } };
		render.add_node(&skybox_mesh_node);
		render.disable_ao();
		render.disable_deferred_render();
		render.disable_post_effects();

		glen::Timer timer;

		while (render.poll_events())
		{
			render.update(&window, &timer);
		}
	}
}



#endif