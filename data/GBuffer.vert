#version 440 core

// // ----- INS ----- // //
in layout(location = 0 ) vec3 vertex_position;
in layout(location = 2 ) vec3 vertex_normal;
in layout(location = 3 ) vec2 vertex_uv;

// // ----- OUTS ----- // //
out vec3 test;

out vec2 uv;

out Out_Frag
{
	vec3 world_space_position;
	vec3 world_space_normal;
} out_frag;

// // ----- UNIFORMS ----- // //
struct Transform
{
	mat4 model_to_projection;
	mat4 model_to_world;
	mat3 model_to_world_normal;
};

uniform Transform transform;

// // ----- SEND FUNCTIONS ----- // //
void send_uv_coordinates()
{
	uv = vertex_uv;
}

void send_world_space_coordinates()
{
	out_frag.world_space_position	= (transform.model_to_world * vec4(vertex_position, 1.0)).xyz;
	out_frag.world_space_normal		= transform.model_to_world_normal * vertex_normal;
}

// // ----- MAIN ----- // //
void main()
{
	send_uv_coordinates();
	send_world_space_coordinates();
	gl_Position = transform.model_to_projection * vec4(vertex_position, 1.0);
}



