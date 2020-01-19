#version 440 core
#define BLUR_SIZE 5;

// // ----- INS ----- // //
in vec2 uv;

// // ----- UNIFORMS ----- // //
uniform sampler2D image;
uniform bool is_horizontal;
float weights[5] = float[]( 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 );

// // ----- OUTS ----- // //
out layout (location = 0) vec4 out_color;

// // ----- MAIN ----- // //
void main()
{
	vec2 texel_size = 1.0 / textureSize(image, 0);
	vec3 frag_contribution = texture(image, uv).rgb * weights[0];
	
	if(is_horizontal)
	{
		for (int i = 0; i < 5; ++i)
		{
			vec2 right_uv = uv + vec2(texel_size.x * i, 0.0);
			vec2 left_uv = uv - vec2(texel_size.x * i, 0.0);

			frag_contribution +=
				texture(image, right_uv).rgb * weights[i];
			frag_contribution +=
				texture(image, left_uv).rgb * weights[i];
		}
	}
	else
	{
		for (int i = 0; i < 5; ++i)
		{
			vec2 up_uv = uv + vec2(0.0, texel_size.y * i);
			vec2 down_uv = uv - vec2(0.0, texel_size.y * i);

			frag_contribution += 
				texture(image, up_uv).rgb * weights[i];
			frag_contribution += 
				texture(image, down_uv).rgb * weights[i];
		}
	}
	out_color = vec4(frag_contribution, 1.0);
}