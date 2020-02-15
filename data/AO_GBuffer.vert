#version 440 core

// // ----- INS ----- // //
in layout(location = 0 ) vec3 vertex_position;
in layout(location = 2 ) vec3 vertex_normal;
in layout(location = 3 ) vec2 vertex_uv;

// // ----- OUTS ----- // //
out vec2 uv;

out Out_Frag
{
	vec3 cam_space_position;
	vec3 cam_space_normal;
} out_frag;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_world;
	mat4 world_to_cam;
	mat4 model_to_projection;
};
uniform Transform transform;

// // ----- SEND FUNCTIONS ----- // //
void send_cam_space_coordinates()
{
	mat4 model_to_cam = transform.world_to_cam * transform.model_to_world;
	vec4 cam_position = model_to_cam * vec4(vertex_position, 1.0);
	mat3 normal_model_to_cam = transpose(inverse(mat3(model_to_cam)));

	out_frag.cam_space_position = cam_position.xyz;
	out_frag.cam_space_normal = normal_model_to_cam * vertex_normal;
	gl_Position = transform.model_to_projection * cam_position;
}

// // ----- MAIN ----- // //
void main()
{
	send_cam_space_coordinates();
	uv = vertex_uv;
}