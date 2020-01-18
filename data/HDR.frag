#version 440 core
#pragma optionNV unroll all

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D hdr_buffer;
uniform bool is_hdr;
uniform float exposure;


// // ----- OUTS ----- // //
out layout (location = 0) vec4 frag_color;
out layout (location = 1) vec4 bright_color;

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

vec3 threshold_color(vec3 color, float threshold)
{
	if (rgb_to_luminance(color) > threshold)
	{
		return vec3(1.0);
//		return color;
	}
	else
	{
		return vec3(0.0);
	}
}


// // ----- MAIN ----- // //
void main()
{
	const float inverted_gamma = 1 / 2.2;

	vec3 raw_color = texture(hdr_buffer, uv).rgb;

	vec3 gamma_corrected_color = gamma(raw_color, inverted_gamma);

	vec3 tonemapped_color = exposure_tonemap(gamma_corrected_color, exposure);


	frag_color = vec4(tonemapped_color, 1.0);
	bright_color = vec4(threshold_color(raw_color, 1.0), 1.0);
//	frag_color = vec4(0.1, 0.8, 0.2, 1.0)  + vec4(tonemapped_color, 1.0) * 0.01;
//	bright_color = vec4(0.9, 0.1, 0.2, 1.0);


}
