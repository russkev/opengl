#ifndef GL_DEMO_SPECULARITY_H
#define GL_DEMO_SPECULARITY_H

#include "gl_engine.h"

namespace gl_demo
{
	static void specularity_demo()
	{
		GLuint width = 800u;
		GLuint height = 600u;
		glen::Window window{ "GL Engine", width, height };

		GLfloat shader_ball_scale = 0.6f;
		GLfloat shader_ball_offset = 3.5f;
		glm::vec3 shader_ball_rotation{ 0.0f, -35.0f, 0.0f };

		// Target Camera
		glen::TargetCamera targetCam{};
		glen::CameraNode targetCam_node{ "Target Camera 1", &targetCam };
		targetCam.set_position({ 0.0f, 8.0f, 8.0f });
		targetCam.set_focus_target({ 0.0f, 0.0f, 0.0f });
		targetCam.focus(glm::vec3{ 0.0f, 0.0f, 0.0f });
		targetCam.set_clip_far(1000.0f);

		// Colors
		glen::Texture white_tex{ glm::vec3{1.00f, 1.00f, 1.00f} };
		glen::Texture lighter_grey_tex{ glm::vec3{0.95f, 0.95f, 0.95f} };
		glen::Texture light_grey_tex{ glm::vec3{0.85f, 0.85f, 0.85f} };
		glen::Texture grey_tex{ glm::vec3{0.50f, 0.50f, 0.50f} };
		glen::Texture dark_grey_tex{ glm::vec3{0.25f, 0.25f, 0.25f} };
		glen::Texture darker_grey_tex{ glm::vec3{0.10f, 0.10f, 0.10f} };
		glen::Texture red_tex{ glm::vec3{1.00f, 0.00f, 0.00f} };
		glen::Texture green_tex{ glm::vec3{0.00f, 1.00f, 0.00f} };
		glen::Texture blue_tex{ glm::vec3{0.00f, 0.00f, 1.00f} };
		glen::Texture normal_up{ glm::vec3{0.50f, 0.50f, 1.00f} };
		glen::Texture grid_tex{ "greyGrid_01.tga" };
		glen::Texture brick_normals{ "bricks2_normal.tga" };
		glen::Texture brick_displacement{ "bricks2_disp.tga" };



		// Shader Grey
		glen::BlinnMaterial grey_material_rough{ "grey material rough" };
		glen::BlinnMaterial grey_material_mid{ "grey material mid" };
		glen::BlinnMaterial grey_material_shiny{ "grey material shiny" };
		glen::BlinnMaterial grey_material_rough_mapped{ "grey material rough" };
		glen::BlinnMaterial grey_material_mid_mapped{ "grey material mid" };
		glen::BlinnMaterial grey_material_shiny_mapped{ "grey material shiny" };

		grey_material_rough.set_sampler_value("material.glossiness", 0.01f);
		grey_material_mid.set_sampler_value("material.glossiness", 0.50f);
		grey_material_shiny.set_sampler_value("material.glossiness", 1.00f);
		grey_material_rough_mapped.set_sampler_value("material.glossiness", 0.01f);
		grey_material_mid_mapped.set_sampler_value("material.glossiness", 0.50f);
		grey_material_shiny_mapped.set_sampler_value("material.glossiness", 1.00f);

		grey_material_rough.set_sampler_value("material.specular", 1.0f);
		grey_material_mid.set_sampler_value("material.specular", 1.0f);
		grey_material_shiny.set_sampler_value("material.specular", 1.0f);
		grey_material_rough_mapped.set_sampler_value("material.specular", 1.0f);
		grey_material_mid_mapped.set_sampler_value("material.specular", 1.0f);
		grey_material_shiny_mapped.set_sampler_value("material.specular", 1.0f);

		grey_material_rough.set_texture("material.diffuse", &grid_tex);
		grey_material_mid.set_texture("material.diffuse", &grid_tex);
		grey_material_shiny.set_texture("material.diffuse", &grid_tex);
		grey_material_rough_mapped.set_texture("material.diffuse", &grid_tex);
		grey_material_mid_mapped.set_texture("material.diffuse", &grid_tex);
		grey_material_shiny_mapped.set_texture("material.diffuse", &grid_tex);

		grey_material_rough.set_texture("material.normal", &brick_normals);
		grey_material_mid.set_texture("material.normal", &brick_normals);
		grey_material_shiny.set_texture("material.normal", &brick_normals);
		grey_material_rough_mapped.set_texture("material.normal", &brick_normals);
		grey_material_mid_mapped.set_texture("material.normal", &brick_normals);
		grey_material_shiny_mapped.set_texture("material.normal", &brick_normals);


		grey_material_rough.set_sampler_value("material.displacement", 0.0f);
		grey_material_mid.set_sampler_value("material.displacement", 0.0f);
		grey_material_shiny.set_sampler_value("material.displacement", 0.0f);
		grey_material_rough_mapped.set_texture("material.displacement", &brick_displacement);
		grey_material_mid_mapped.set_texture("material.displacement", &brick_displacement);
		grey_material_shiny_mapped.set_texture("material.displacement", &brick_displacement);

		grey_material_rough.set_uniform("material.displacement_amount", 0.0f);
		grey_material_mid.set_uniform("material.displacement_amount", 0.0f);
		grey_material_shiny.set_uniform("material.displacement_amount", 0.0f);
		grey_material_rough_mapped.set_uniform("material.displacement_amount", 0.03f);
		grey_material_mid_mapped.set_uniform("material.displacement_amount", 0.03f);
		grey_material_shiny_mapped.set_uniform("material.displacement_amount", 0.03f);

		// Shader ball mesh
		glen::Mesh shaderBall = glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
		//glen::Mesh shaderBall = glen::Sphere::create_sphere(2.5f);

		// Spec test shader ball nodes
		glen::MeshNode shaderBall_rough_node{ "shaderBall rough",			&shaderBall, &grey_material_rough };
		glen::MeshNode shaderBall_mid_node{ "shaderBall mid",			&shaderBall, &grey_material_mid };
		glen::MeshNode shaderBall_shiny_node{ "shaderBall shiny",			&shaderBall, &grey_material_shiny };
		glen::MeshNode shaderBall_rough_mapped_node{ "shaderBall blinn rough",	&shaderBall, &grey_material_rough_mapped };
		glen::MeshNode shaderBall_mid_mapped_node{ "shaderBall blinn mid",	&shaderBall, &grey_material_mid_mapped };
		glen::MeshNode shaderBall_shiny_mapped_node{ "shaderBall blinn shiny",	&shaderBall, &grey_material_shiny_mapped };

		std::vector<glen::MeshNode*> shaderBalls;
		shaderBalls.push_back(&shaderBall_rough_node);
		shaderBalls.push_back(&shaderBall_mid_node);
		shaderBalls.push_back(&shaderBall_shiny_node);
		shaderBalls.push_back(&shaderBall_rough_mapped_node);
		shaderBalls.push_back(&shaderBall_mid_mapped_node);
		shaderBalls.push_back(&shaderBall_shiny_mapped_node);

		for (auto & shad_ball : shaderBalls)
		{
			shad_ball->set_scale(shader_ball_scale);
			shad_ball->set_rotation(shader_ball_rotation);
		}

		// Position shader balls
		shaderBall_rough_node.set_position({ -shader_ball_offset, shader_ball_offset / 2, 0.0f });
		shaderBall_mid_node.set_position({ 0.0f, shader_ball_offset / 2, 0.0f });
		shaderBall_shiny_node.set_position({ shader_ball_offset, shader_ball_offset / 2, 0.0f });
		shaderBall_rough_mapped_node.set_position({ -shader_ball_offset, -shader_ball_offset / 2, 0.0f });
		shaderBall_mid_mapped_node.set_position({ 0.0f, -shader_ball_offset / 2, 0.0f });
		shaderBall_shiny_mapped_node.set_position({ shader_ball_offset, -shader_ball_offset / 2, 0.0f });

		// Directional light 1
		glen::DirectionalLight directionalLight1{ 0.2f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
		glen::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
		directionalLight_node1.set_rotation({ 33.0f, 225.0f, 0.0f });
		directionalLight_node1.set_position({ 16.0f, 16.0f, 16.0f });

		// Shadow map
		glen::ShadowMap shadowMap_directional1{ &directionalLight_node1 };


		// Renderer
		glen::Renderer render{ &targetCam_node, glm::uvec2(window.width(), window.height()) };
		render.disable_post_effects();
		render.disable_post_effects();

		render.add_node(&shaderBall_rough_node);
		render.add_node(&shaderBall_mid_node);
		render.add_node(&shaderBall_shiny_node);
		render.add_node(&shaderBall_rough_mapped_node);
		render.add_node(&shaderBall_mid_mapped_node);
		render.add_node(&shaderBall_shiny_mapped_node);

		render.add_node(&directionalLight_node1);

		glen::Timer timer;

		while (render.poll_events())
		{
			render.update(&window, &timer);

			//shaderBall_rough_node.set_position(glm::vec3{ std::cos(timer.total_time_s()) * 5.0f, 0.0f, 0.0f });
		}
	}

}
#endif