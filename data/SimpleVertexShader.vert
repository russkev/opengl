// shadertype=glsl

#version 330 core

const int numElements = 100;

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec4 model_vertexPosition;
in layout(location = 1) vec3 model_vertexColor;
in layout(location = 2) vec3 model_vertexNormal;
in layout(location = 3) uint model_id;
in layout(location = 4) mat4 mat_modelToProjection;
in layout(location = 8) mat4 mat_modelToWorld;

uniform mat4[numElements] transforms;
uniform ivec3[numElements] connections;

// // Output data ; will be interpolated for each fragment
out vec3 f_world_vertexNormal;
out vec3 f_world_vertexPosition;

// // Uniforms ; values that stay constant for whole mesh

bool hasInput(int a)
{
	for(int i = 0; i < numElements; ++i)
	{
		if(connections[i][1] == a || connections[i][2] == a)
		{
			return true;
		}
	}
}

//void commitTransform


void main()
{
	// // Output position of the vertex, in clip space : MVP * position
	gl_Position				= mat_modelToProjection * model_vertexPosition * transforms[0] * (model_id+500u);

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	f_world_vertexNormal	= normalize(vec3(mat_modelToWorld * vec4(model_vertexNormal, 0)));
	f_world_vertexPosition	= vec3(mat_modelToWorld * model_vertexPosition);
}

