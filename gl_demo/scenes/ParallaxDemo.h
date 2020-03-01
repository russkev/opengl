#ifndef GL_DEMO_PARALLAX_H
#define GL_DEMO_PARALLAX_H

#include "gl_engine.h"

namespace gl_demo
{
	static void parallax_demo()
	{
		GLuint width = 800u;
		GLuint height = 600u;

		glen::Window window{ "GL Engine", width, height };

		// Target Camera
		glen::TargetCamera targetCam{};
		glen::CameraNode targetCam_node{ "Target Camera 1", &targetCam };
		targetCam.set_position({ 0.0f, 8.0f, 8.0f });
		targetCam.set_clip_far(1000.0f);

		// Orthogonal Camera
		glen::OrthoCamera orthoCam{};
		glen::CameraNode orthoCam_node{ "Ortho Cam 1", &orthoCam };
		orthoCam.set_clip_near(0.001f);
		orthoCam.set_clip_far(100.0f);
		orthoCam.set_sides(-30.0f, 30.0f, -30.0f, 30.0f);

		// Free Camera
		glen::FreeCamera freeCam{};
		glen::CameraNode freeCam_node{ "Free Cam 1", &freeCam };
		freeCam.set_clip_near(0.001f);
		freeCam.set_clip_far(1000.0f);

		// Colors
		glen::Texture white_tex{ glm::vec3{1.00f, 1.00f, 1.00f} };
		glen::Texture light_grey_tex{ glm::vec3{0.85f, 0.85f, 0.85f} };
		glen::Texture grey_tex{ glm::vec3{0.50f, 0.50f, 0.50f} };
		glen::Texture dark_grey_tex{ glm::vec3{0.25f, 0.25f, 0.25f} };
		glen::Texture blue_tex{ glm::vec3{0.25f, 0.25f, 1.00f} };
		glen::Texture normal_up{ glm::vec3{0.50f, 0.50f, 1.00f} };

		// Textures
		glen::Texture uv_template_b_tex{ "uvtemplateB.tga" };
		glen::Texture grey_grid_tex{ "greyGrid_01.tga" };
		glen::Texture brick_normals{ "bricks2_normal.tga" };
		glen::Texture brick_displacement{ "bricks2_disp.tga" };

		// Shader 1
		glen::BlinnMaterial shaderBall_material{ "Shaderball Blinn" };
		shaderBall_material.set_sampler_value(shaderBall_material.k_material_glossiness, 0.5f);
		shaderBall_material.set_sampler_value(shaderBall_material.k_material_specular, 0.8f);
		shaderBall_material.set_texture(shaderBall_material.k_material_diffuse, &uv_template_b_tex);
		shaderBall_material.set_texture(shaderBall_material.k_material_normal, &brick_normals);
		shaderBall_material.set_uniform(shaderBall_material.k_material_normal_directx_mode, false);
		shaderBall_material.set_uniform(shaderBall_material.k_material_displacement_enabled, true);
		shaderBall_material.set_texture(shaderBall_material.k_material_displacement, &brick_displacement);
		shaderBall_material.set_uniform(shaderBall_material.k_material_displacement_amount, 0.06f);


		// Shader 2
		glen::BlinnMaterial floor_material{ "Floor Blinn" };
		floor_material.set_sampler_value(floor_material.k_material_glossiness, 0.03f);
		floor_material.set_sampler_value(floor_material.k_material_specular, 0.30f);
		floor_material.set_texture(floor_material.k_material_diffuse, &grey_grid_tex);

		// Mesh 1
		glen::Mesh shaderBall = glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
		glen::MeshNode shaderBall_node{ "shader ball", &shaderBall, &shaderBall_material };

		// Mesh 2
		glen::Mesh plane = glen::Plane::create_plane(100.0f, 100.0f, 1, 1);
		plane.scale_uvs(10.0f);
		glen::MeshNode plane_node{ "Plane 1", &plane, &floor_material };


		// Point Light 1
		glen::PointLight pointLight{ 1.0f, { 0.0f, 0.0f, 0.0f } };
		pointLight.set_brightness(4.1f);
		pointLight.set_color(glm::vec3(1.0, 0.7, 0.2));
		pointLight.set_radius(0.1f);
		//pointLight.disable_mesh();
		glen::LightNode pointLight_node{ "Point Light 1", &pointLight };
		pointLight_node.set_position({ -4.0f, 1.2f, 0.0f });

		// Point Light 2
		glen::PointLight pointLight2{ 9.0f, { 0.2f, 0.1f, 1.0f } };
		glen::LightNode pointLight_node2{ "Point Light 2", &pointLight2 };
		pointLight_node2.set_position({ 0.0f, 4.0f, -5.0f });

		// Directional Light 1
		glen::DirectionalLight directionalLight1{ 0.1f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
		glen::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
		directionalLight_node1.set_rotation({ 33.0f, 225.0f, 0.0f });
		directionalLight_node1.set_position({ 16.0f, 16.0f, 16.0f });

		directionalLight_node1.add_child(&freeCam_node);
		directionalLight_node1.add_child(&orthoCam_node);


		// Spot Light 1
		glen::SpotLight spotLight1{ 10.0f, { 1.0f, 0.0f, 0.0f } };
		glen::LightNode spotLight_node1{ "Spot Light 1", &spotLight1 };
		spotLight_node1.set_rotation({ 60.0f, 0.0f, 0.0f });
		spotLight_node1.set_position({ -3.0f, 10.0f, -2.0f });
		spotLight1.set_inner_angle(30.0f);
		spotLight1.set_outer_angle(33.0f);

		// Spot Light 2
		glen::SpotLight spotLight2{ 14.0f, {0.0f, 0.0f, 1.0f } };
		glen::LightNode spotLight_node2{ "Spot Light 2", &spotLight2 };
		spotLight_node2.set_position({ 6.0f, 12.0f, 2.0f });
		spotLight_node2.set_rotation({ 50.0f, -100.0f, 0.0f });
		spotLight2.set_inner_angle(25.0f);
		spotLight2.set_outer_angle(70.0f);

		// Shadow map
		//glen::ShadowMap shadowMap_directional1{ &directionalLight_node1 };
		//glen::ShadowMap shadowMap_point1{ &pointLight_node };
		//glen::ShadowMap shadowMap_point2{ &pointLight_node2 };
		//glen::ShadowMap shadowMap_spot1{ &spotLight_node1 };
		glen::ShadowMap shadowMap_spot2{ &spotLight_node2 };

		// Null node 1
		glen::Node lightRotate1{ "light rotate 01" };
		//lightRotate1.add_child(&pointLight_node);

		// Null node 2
		glen::Node lightRotate2{ "light rotate 02" };
		lightRotate2.add_child(&lightRotate1);
		lightRotate2.set_position({ 3.0, 0.0, 0.0 });

		// Axis arrows
		glen::Mesh axis_arrow{ glen::Arrow::create_arrow(20) };
		glen::Material axis_mat_x{ "axis arrow x", "Light.vert", "Light.frag" };
		glen::Material axis_mat_y{ "axis arrow x", "Light.vert", "Light.frag" };
		glen::Material axis_mat_z{ "axis arrow x", "Light.vert", "Light.frag" };
		axis_mat_x.set_uniform("light.color", glm::vec3{ 1.0f, 0.0f, 0.0f });
		axis_mat_y.set_uniform("light.color", glm::vec3{ 0.0f, 1.0f, 0.0f });
		axis_mat_z.set_uniform("light.color", glm::vec3{ 0.0f, 0.0f, 1.0f });
		glen::MeshNode axis_arrow_x_node{ "axis arrow x node", &axis_arrow, &axis_mat_x };
		glen::MeshNode axis_arrow_y_node{ "axis arrow y node", &axis_arrow, &axis_mat_y };
		glen::MeshNode axis_arrow_z_node{ "axis arrow z node", &axis_arrow, &axis_mat_z };
		axis_arrow_x_node.set_scale(0.05f);
		axis_arrow_y_node.set_scale(0.05f);
		axis_arrow_z_node.set_scale(0.05f);
		axis_arrow_x_node.set_position({ 1.0f, 0.0f, 0.0f });
		axis_arrow_y_node.set_position({ 0.0f, 1.0f, 0.05f });
		axis_arrow_z_node.set_position({ 0.0f, 0.0f, 1.0f });
		axis_arrow_x_node.set_rotation({ 0.0f, 90.0f, 0.0f });
		axis_arrow_y_node.set_rotation({ -90.0f, 0.0f, 0.0f });


		// Renderer
		glen::Renderer render{ &targetCam_node, glm::uvec2(window.width(), window.height()) };
		render.enable_post_effects();
		render.disable_deferred_render();
		render.disable_ao();

		render.add_node(&shaderBall_node);
		render.add_node(&plane_node);

		//render.add_node(&directionalLight_node1);
		//render.add_node(&pointLight_node);
		//render.add_node(&pointLight_node2);
		//render.add_node(&spotLight_node1);
		render.add_node(&spotLight_node2);

		//render.add_node(&lightRotate1);


		//render.add_node(&axis_arrow_x_node);
		//render.add_node(&axis_arrow_y_node);
		//render.add_node(&axis_arrow_z_node);


		glen::Timer timer;

		while (render.poll_events())
		{
			float new_y_rotation = (float)shaderBall_node.local_rotation().y + (float)timer.delta_time_s() * 10.0f;
			shaderBall_node.set_rotation({ 0.0, new_y_rotation, 0.0 });

			float l1_rotate = lightRotate1.local_rotation().z - (float)timer.delta_time_s() * 10;
			lightRotate1.set_rotation({ 0.0 , 0.0, l1_rotate });

			float l2_rotate = lightRotate2.local_rotation().y - (float)timer.delta_time_s() * 60;
			lightRotate2.set_rotation({ 0.0, l2_rotate, 0.0 });

			render.update(&window, &timer);
		}
	}
}
#endif