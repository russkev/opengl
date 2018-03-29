#version 440 core

in layout(location = 0) vec2 model_vertexPosition;
in layout(location = 1) vec2 model_uv;
in layout(location = 2) int vertex_id;

uniform int width;
uniform int height;

int half_width		= width / 2;
int half_height		= height / 2;
const int num_length		= 8;

out vec2 uv;
out vec3 vertex_color;

vec2 text_uv(int[num_length] s_int)
{
	int i			= vertex_id/4;
	int offset		= 32;
	int num			= s_int[i] + 48 - offset;
	int corner		= vertex_id%4;

	float uv_size	= 1.0f / 16.0f;
	float uv_x		= (num % 16) * uv_size;
	float uv_y		= (num / 16) * uv_size;
	
	if (corner == 0) { return vec2( uv_x,			1.0 - uv_y			);	}
	if (corner == 1) { return vec2( uv_x + uv_size,	1.0 - uv_y			);	}
	if (corner == 2) { return vec2( uv_x,			1.0 - uv_y - uv_size);	}
	if (corner == 3) { return vec2( uv_x + uv_size, 1.0 - uv_y - uv_size);	}

	return vec2( 0.0f, 0.0f );
}

void main()
{
	vec2 outPosition	= model_vertexPosition - vec2(half_width, half_height);
	outPosition			/= vec2(half_width, half_height);

	vertex_color		= vec3(1.0f, 0.0f, 0.0f);

	gl_Position			= vec4(outPosition, 0, 1);
	//uv					= model_uv;
	uv					= text_uv(int[num_length](1, 2, 3, 4, 5, 6, 7, 8) );
}