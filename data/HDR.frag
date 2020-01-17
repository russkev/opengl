#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D hdr_buffer;
uniform bool is_hdr;
uniform float exposure;


// // ----- OUTS ----- // //
out vec4 frag_color;

// // ----- GENERAL ----- // //
float rgb_to_luminance(vec3 color)
{
	return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

vec3 gamma(vec3 color, float gamma_value)
{
	return vec3(pow(color.x, gamma_value), pow(color.y, gamma_value), pow(color.z, gamma_value));
}

vec3 reinhard_tonemap(vec3 color)
{
	return color / (color + 1);
}

vec3 exposure_tonemap(vec3 color, float exposure_value)
{
	return vec3(1.0) - exp(-color * exposure_value);
}


// // ----- MAIN ----- // //
void main()
{
	const float inverted_gamma = 1 / 2.2;
	const float luminance = 0.1;

	vec3 hdr_color = texture(hdr_buffer, uv).rgb;

	hdr_color = gamma(hdr_color, inverted_gamma);


	if (uv.x < 0.5)
	{

		hdr_color = exposure_tonemap(hdr_color, exposure);
	}

//	hdr_color = reinhard_tonemap(hdr_color);

	frag_color = vec4(hdr_color, 1.0);
}
