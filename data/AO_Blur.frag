#version 440 core

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D ao_input;

// // ----- OUTS ----- // //
out vec4 frag_color;

void main() {
	vec2 texel_size = 1.0 / vec2(textureSize(ao_input, 0));

	float result = 0.0;

	for (int x = -2; x < 2; ++x)
	{
		for (int y = -2; y < 2; ++y)
		{
			vec2 offset = vec2(float(x), float(y)) * texel_size;
			result += texture(ao_input, uv + offset).r;
		}
	}
//	frag_color = vec4(result / (4.0 * 4.0), 1.0);
//	frag_color = 1 - vec4(texture(ao_input, uv).xyz, 1.0);
	frag_color = vec4(vec3(result) / (4.0 * 4.0), 1.0);
}