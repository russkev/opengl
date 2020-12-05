#pragma once

#include "gl_engine.h"

namespace gl_demo
{
	static void first_scene()
	{
		// Set up the Window
		GLuint width = 800u;
		GLuint height = 600u;
		glen::Window window{ "First Window", width, height };

		// Set up the camera
		glen::TargetCamera targetCam{};
		glen::CameraNode targetCam_node{ "Target Camera", &targetCam };
		targetCam.set_position({ 0.0f, 3.0f, 5.0f });
		targetCam.focus({ 0.0f, 0.0f, 0.0f });

		// Set up the sphere model
		glen::Mesh sphere_mesh{ glen::Sphere::create_sphere(1.0f) };
		glen::BlinnMaterial blinn_white{ "White Blinn" };
		blinn_white.set_sampler_value(blinn_white.k_material_diffuse, 0.5f);
		glen::MeshNode sphere_mesh_node{ "Sphere Node", &sphere_mesh, &blinn_white };
		sphere_mesh_node.set_position({ 0.0f, 1.0f, 0.0f });

		//Set up a ground plane
		glen::Mesh ground{ glen::Plane::create_plane(5.0f, 5.0f) };
		glen::MeshNode ground_node{ "Ground Plane Node", &ground, &blinn_white };
		
		// Set up the light
		glen::PointLight light;
		glen::LightNode light_node{ "Main Light Node", &light };
		light_node.set_position({ 5.0f, 6.0f, 0.0f });
		light.set_brightness(5.0f);

		// Set up shadow map
		glen::ShadowMap shadow{ &light_node, 2048 };
		shadow.set_bias(0.2f);
		shadow.set_clip_far(40);

		// Set up the renderer
		glen::Renderer renderer{ &targetCam_node, glm::uvec2{ width, height } };

		// Add the nodes
		renderer.add_node(&sphere_mesh_node);
		renderer.add_node(&ground_node);
		renderer.add_node(&light_node);
		renderer.enable_ao(); //Turn on ambient occlusion

		// Set up the timer and window
		glen::Timer timer;

		// Main render loop
		while (renderer.poll_events())
		{
			renderer.update(&window, &timer);
		}
	}
}