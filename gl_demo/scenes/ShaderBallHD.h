#ifndef GL_DEMO_SHADER_BALL_HD
#define GL_DEMO_SHADER_BALL_HD

#include "gl_engine.h"

namespace gl_demo
{
	static void shader_ball_hd_demo()
	{
		GLuint width = 800u;
		GLuint height = 600u;

		glen::Window window{ "GL_Engine", width, height };

		// Target Camera
		glen::TargetCamera target_cam{};
		glen::CameraNode target_cam_node{ "Target Camera 1", &target_cam };
		glm::vec3 centre_focus{ 0.0f, 5.0f, 0.0f };
		//glm::vec3 centre_focus{ 0.0f, 110.0f, 0.0f };
		target_cam.set_position({ 0.0f, 320.0f, 600.0f });
		target_cam.set_focus_target(centre_focus);
		target_cam.focus(centre_focus);
		target_cam.set_clip_far(3000.0f);
		target_cam.set_angle_of_view(37.8f); //35mm


		// Meshes
		//glen::Mesh ball_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball.obj");
		//glen::Mesh ball_inside_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball_inside.obj");
		//glen::Mesh ball_trim_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball_trim.obj");
		glen::Mesh base_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_base.obj");
		glen::Mesh cyc_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_cyc.obj");

		// Materials
		glen::BlinnMaterial white_blinn_material{ "Blinn Material " };
		white_blinn_material.set_sampler_value(white_blinn_material.k_material_diffuse, 0.8f);
		white_blinn_material.set_uniform(white_blinn_material.k_material_specular_amount, 0.8f);
		white_blinn_material.set_sampler_value(white_blinn_material.k_material_specular, 0.9f);


		// Mesh Nodes
		//glen::MeshNode ball_node{ "Ball Node", &ball_mesh, &white_blinn_material };
		//glen::MeshNode ball_inside_node{ "Ball Inside Node", &ball_inside_mesh, &white_blinn_material };
		//glen::MeshNode ball_trim_node{ "Ball Trim Node", &ball_trim_mesh, &white_blinn_material };
		glen::MeshNode base_node{ "Base Node", &base_mesh, &white_blinn_material };
		glen::MeshNode cyc_node{ "Cyc Node", &cyc_mesh, &white_blinn_material };


		// Test Small Shader Ball
		glen::Mesh shaderBall = glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
		glen::MeshNode shaderBall_node{ "shader ball", &shaderBall, &white_blinn_material };

		// Test Spot Light Node
		glen::SpotLight spotLight2{ 100.0f, { 0.2f, 0.3f, 1.0f } };
		glen::LightNode spotLight_node2{ "Spot Light 2", &spotLight2 };
		//glm::vec3 spotLight2_start_position{ 11.0f, 5.0f, 0.0f };
		//glm::vec3 spotLight2_start_position{ 6.0f, 12.0f, 2.0f };
		//spotLight_node2.set_position(spotLight2_start_position * 25.0f);
		//spotLight_node2.set_rotation({ 20.0f, -90.0f, 0.0f });
		//spotLight2.set_inner_angle(25.0f);
		//spotLight2.set_outer_angle(35.0f);

		spotLight_node2.set_rotation({ 33.0f, 270.0f, 0.0f });
		spotLight_node2.set_position({ 300.0f, 300.0f, 0.0f });
		//spotLight2.set_inner_angle(50.0f);
		//spotLight2.set_outer_angle(90.0f);


		// Test Directional Light Node
		//glen::DirectionalLight directionalLight2{ 14.0f, {0.0f, 0.0f, 1.0f } };
		//glen::LightNode directionalLight_node2{ "Directional Light 2", &directionalLight2 };
		//glm::vec3 directinoalLight2_start_position{ 0.0f, 5.0f, -20.0f };
		////glm::vec3 spotLight2_start_position{ 6.0f, 12.0f, 2.0f };
		//directionalLight_node2.set_position(directinoalLight2_start_position * 1.0f);
		//directionalLight_node2.set_rotation({ 20.0f, 0.0f, 0.0f });


		// Test Shadow Map
		glen::ShadowMap shadowMap_spot2{ &spotLight_node2 };
		shadowMap_spot2.set_bias(0.000005f);
		shadowMap_spot2.set_radius(0.2f);
		shadowMap_spot2.set_num_samples(4);
		//shadowMap_spot2.set_clip_near(0.1f);
		//shadowMap_spot2.set_clip_far(1000.0f);


		//glen::ShadowMap shadowMap_directional2{ &directionalLight_node2 };
		//shadowMap_directional2.set_bias(0.00005f);
		//shadowMap_directional2.set_radius(0.01f);
		//shadowMap_directional2.set_num_samples(3);



		// Spot Light 1
		glen::SpotLight spot_light{ 70.0f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
		spot_light.camera()->set_clip_far(2000.0f);
		//spot_light.set_inner_angle(40.0f);
		//spot_light.set_outer_angle(45.0f);
		glen::LightNode spot_light_node{ "Spot Light 1", &spot_light };
		spot_light_node.set_rotation({ 33.0f, 270.0f, 0.0f });
		spot_light_node.set_position({ 300.0f, 300.0f, 0.0f });

		// Point Light 1
		glen::PointLight point_light{ 80.0f, glm::vec3{0.0f, 1.0f, 0.0f} };
		point_light.camera()->set_clip_far(2000.0f);
		point_light.camera()->set_clip_near(10.0f);
		glen::LightNode point_light_node{ "Point Light 1", &point_light };
		point_light_node.set_position(glm::vec3{ -300.0f, 300.0f, 100.0f });

		// Shadow Map
		glen::ShadowMap spot_light_shadow{ &spot_light_node };
		spot_light_shadow.set_bias(0.000002f);
		spot_light_shadow.set_radius(0.01f);
		spot_light_shadow.set_num_samples(1);
		//spot_light_shadow.set_clip_near(1.0f);
		spot_light_shadow.set_clip_far(50000.0f);

		glen::ShadowMap point_light_shadow{ &point_light_node };
		point_light_shadow.set_bias(10.0f);
		point_light_shadow.set_clip_far(5000.0f);
		point_light_shadow.set_radius(1.0f);

		glen::Renderer render{ &target_cam_node, glm::uvec2{ window.width(), window.height() } };
		render.enable_post_effects();
		render.disable_deferred_render();
		render.disable_ao();

		// Add mesh nodes
		//render.add_node(&ball_node);
		//render.add_node(&ball_inside_node);
		//render.add_node(&ball_trim_node);
		render.add_node(&base_node);
		render.add_node(&cyc_node);

		// Test bit
		//render.add_node(&shaderBall_node);
		//render.add_node(&spotLight_node2);
		//render.add_node(&directionalLight_node2);

		// Add light nodes
		render.add_node(&spot_light_node);
		//render.add_node(&point_light_node);

		glen::Timer timer;
		while (render.poll_events())
		{
			spotLight2.set_brightness(200.0f);
			spotLight2.set_inner_angle(39.0f);
			spotLight2.set_outer_angle(80.0f);
			render.update(&window, &timer);
		}

	}
}
#endif
