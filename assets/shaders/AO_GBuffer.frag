#version 440 core
// // ----- INS ----- // //
in vec2 uv;

in Out_Frag
{
	vec3 cam_space_position;
	vec3 cam_space_normal;
} in_frag;

// // ----- OUTS ----- // //
out layout (location = 0) vec3 g_position;
out layout (location = 1) vec3 g_normal;
out layout (location = 2) vec3 g_diffuse;

// // ----- MAIN ----- // //
void main ()
{
	g_position = in_frag.cam_space_position;
	g_normal = normalize(in_frag.cam_space_normal);
	g_diffuse = vec3(0.95);
}