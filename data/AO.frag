#version 440 core
#define NUM_SAMPLES 64

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D noise;

uniform vec3 samples[NUM_SAMPLES];

uniform float radius;
uniform float bias;

uniform uvec2 screen_dimensions;
uniform uvec2 noise_tile_dimensions;

uniform mat4 cam_to_projection;

// // ----- OUTS ----- // //
out vec4 frag_color;

// // ----- LOCALS ----- // //
const vec2 noise_scale = vec2(
	screen_dimensions.x / noise_tile_dimensions.x,
	screen_dimensions.y / noise_tile_dimensions.y);

// // ----- MAIN ----- // //
void main()
{
	frag_color = vec4(texture(g_normal, uv).xyz, 1.0);
}

