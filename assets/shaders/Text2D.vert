#version 440 core

// // ----- INS ----- // //
in layout(location = 0) vec2 vertex_position;
in layout(location = 1) vec2 vertex_uv;
in layout(location = 2) int vertex_id;

// // ----- UNIFORMS ----- // //
uniform int			width;
uniform int			height;
uniform int[200]	text_string;

// // ----- LOCALS ----- // //
int half_width			= width / 2;
int half_height			= height / 2;
const int max_digits	= 200;
const int max_letters	= 200;
int num_digits			= 1;

// // ----- OUTS ----- // //
out vec2 uv;
out vec3 vertex_color;

// // ----- GENERAL METHODS ----- // //
int[200] test()
{
	int[200] outArray;
	for (int i = 0; i < 200; i++)
	{
		outArray[i] = 9;
	}
	return outArray;
}

vec2 text_uv(int[max_digits] s_int)
{


	int i			= vertex_id/4;
	int offset		= 32;
	int num			= s_int[i] + 48 - offset;
	int corner		= vertex_id%4;

	if (vertex_id > max_digits * 4) num = -16;

	float uv_size	= 1.0f / 16.0f;
	float uv_x		= (num % 16) * uv_size;
	float uv_y		= (num / 16) * uv_size;
	
	if (corner == 0) { return vec2( uv_x,			1.0 - uv_y			);	}
	if (corner == 1) { return vec2( uv_x + uv_size,	1.0 - uv_y			);	}
	if (corner == 2) { return vec2( uv_x,			1.0 - uv_y - uv_size);	}
	if (corner == 3) { return vec2( uv_x + uv_size, 1.0 - uv_y - uv_size);	}	
}

 void reverse_digits(inout int[max_digits] digits, in int start, in int end)
{
	for (; start < end; ++ start, -- end)
	{
		int digit		= digits[start];
		digits[start]	= digits[end];
		digits[end]		= digit;
	}
}

int[max_letters] string_array()
{
	int[max_letters] letters;
	for (int i = 0; i < max_letters; i++)
	{
		letters[i] = text_string[i] - 48;
	}
	return letters;
}

int[max_digits] float_to_ints(float num)
{
	int[max_digits] letters;
	for (int i = 0; i < max_digits; i++)
	{
		letters[i] = text_string[i] - 48;
	}
	return letters;


	int num_sign = -16;
	if ( num < 0.0 ) num_sign = -3;

	int[max_digits]	digits;

	const float base	= 10;
	int start			= num_digits;

	num			= abs(num);
	float frac	= num;
	num			= floor(num);
	frac		-= num;

	// Integral digits
	for (; num > 0.0 && num_digits < max_digits; ++num_digits)
	{
		float new_num		= floor(num / base);
		digits[num_digits]	= int(num - base * new_num);
		num = new_num;
	}

	reverse_digits(digits, start, num_digits - 1);
	digits[num_digits] = -2; //ASCII for full stop
	num_digits++;

	// Fractional digits
	for ( ; frac > 0.0 && num_digits < max_digits; ++num_digits)
	{
		frac				*= base;
		float digit			= floor(frac);
		frac				-= digit;
		digits[num_digits]	= int(digit);
	}

	digits[0] = num_sign;

	return digits;
}

// // ----- MAIN ----- // //
void main()
{
	vec2 outPosition	= vertex_position - vec2(half_width, half_height);
	outPosition			/= vec2(half_width, half_height);

	vertex_color		= vec3(1.0f, 0.0f, 0.0f);

	gl_Position			= vec4(outPosition, 0, 1);
	//uv					= text_uv(float_to_ints(1.0) );
	uv					= text_uv(string_array());
	//uv =				text_uv(test());
}