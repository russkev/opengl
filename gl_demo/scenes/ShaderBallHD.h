#ifndef GL_DEMO_SHADER_BALL_HD
#define GL_DEMO_SHADER_BALL_HD

#include "gl_engine.h"

namespace gl_demo
{
	struct ShaderBallHD
	{
		static constexpr GLuint k_width = 800u;
		static constexpr GLuint k_height = 600u;


	public:
		ShaderBallHD();

		void reset_materials();

		void add_member_nodes_to_renderer(glen::Renderer& render);

		void set_render_settings(glen::Renderer& render);

		void add_lights_to_renderer(glen::Renderer& render);

		void add_geometry_to_renderer(glen::Renderer& render);

		void demo_01();

		void demo_02();

		void demo_03();

		void demo_04();

		void demo_05();

		void demo_06();

		void demo_07();

		void demo_08();


	private:
		glen::Window m_window{ "GL_Engine", k_width, k_height };
		glen::Timer m_timer;


		glen::TargetCamera m_target_cam{};
		glen::CameraNode m_target_cam_node{ "Target Camera 1", &m_target_cam };

		glen::BlinnMaterial m_white_blinn_material{ "Blinn Material" };
		glen::BlinnMaterial m_white_diffuse_material{ "White Diffuse Material" };
		glen::BlinnMaterial m_rubber_material{ "Rubber Material" };
		glen::BlinnMaterial m_grid_material{ "Grid Material" };
		glen::Texture m_grid_texture{ "grey_grid.tga" };
		glen::Texture m_checker_texture{ "checker_02.tga" };


		glen::Mesh m_ball_mesh{ glen::OBJ_Loader::load_obj("shaderball_hd_ball.obj") };
		glen::Mesh m_ball_inside_mesh{ glen::OBJ_Loader::load_obj("shaderball_hd_ball_inside.obj") };
		glen::Mesh m_ball_trim_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball_trim.obj");
		glen::Mesh m_base_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_base.obj");
		glen::Mesh m_cyc_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_cyc.obj");


		glen::MeshNode m_ball_node{ "Ball Node", &m_ball_mesh, &m_white_blinn_material };
		glen::MeshNode m_ball_inside_node{ "Ball Inside Node", &m_ball_inside_mesh, &m_white_diffuse_material };
		glen::MeshNode m_ball_trim_node{ "Ball Trim Node", &m_ball_trim_mesh, &m_rubber_material };
		glen::MeshNode m_base_node{ "Base Node", &m_base_mesh, &m_white_blinn_material };
		glen::MeshNode m_cyc_node{ "Cyc Node", &m_cyc_mesh, &m_grid_material };

		glen::SpotLight m_spot_light_key;
		glen::LightNode m_spot_light_key_node{ "Spot Light Key 1", &m_spot_light_key };
		glen::ShadowMap m_spot_light_key_shadow{ &m_spot_light_key_node, 4096 };

		glen::SpotLight m_spot_light_fill;
		glen::LightNode m_spot_light_fill_node{ "Spot Light Fill 1", &m_spot_light_fill };
		glen::ShadowMap m_spot_light_fill_shadow{ &m_spot_light_fill_node, 4096 };

		glen::SpotLight m_spot_light_rim;
		glen::LightNode m_spot_light_rim_node{ "Spot Light Rim 1", &m_spot_light_rim };
		glen::ShadowMap m_spot_light_rim_shadow{ &m_spot_light_rim_node, 4096 };

		glen::PointLight m_point_light_fill;
		glen::LightNode m_point_light_fill_node{ "Point Light Fill 1", &m_point_light_fill };

		glen::PointLight m_point_light_fill_2;
		glen::LightNode m_point_light_fill_2_node{ "Point Light Fill 2", &m_point_light_fill_2 };

		glen::SpotLight m_spot_light_bg_01;
		glen::LightNode m_spot_light_bg_01_node{ "Spot Light BG 1", &m_spot_light_bg_01 };

		glen::SpotLight m_spot_light_bg_02;
		glen::LightNode m_spot_light_bg_02_node{ "Spot Light BG 2", &m_spot_light_bg_02 };

	};
}
#endif
