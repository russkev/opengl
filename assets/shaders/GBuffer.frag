#version 440 core
// // ----- INS ----- // //
in vec2 uv;

in Out_Frag
{
	vec3 world_space_position;
	vec3 world_space_normal;
} in_frag;

// // ----- OUTS ----- // //
out layout (location = 0) vec3 g_position;
out layout (location = 1) vec3 g_normal;
out layout (location = 2) vec4 g_diffuse_spec;


// // ----- UNIFORMS ----- // //
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
};
uniform Material material;

// // ----- MAIN ----- // //
void main ()
{
	g_position = in_frag.world_space_position;
	g_normal = normalize(in_frag.world_space_normal);
	g_diffuse_spec.rgb =  texture(material.diffuse, uv).rgb;
	g_diffuse_spec.a =  texture(material.specular, uv).r;
}