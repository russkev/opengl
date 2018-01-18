// shadertype=glsl

#version 330 core

const int numElements = 100;
const int transformSourceLoc	= 0;
const int transformDestLoc		= 1;
const int shapeDestLoc			= 2;

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec4 model_vertexPosition;
in layout(location = 1) vec3 model_vertexColor;
in layout(location = 2) vec3 model_vertexNormal;
in layout(location = 3) int model_id;
in layout(location = 4) mat4 mat_modelToProjection;
in layout(location = 8) mat4 mat_modelToWorld;

// // Array of transforms
// // Numbers in the transform slots of the connections array
// // indicte the nth matrix in this array
uniform mat4[numElements] transforms;

// // Connections is an ivec3 of the form:
// // <source transform, destination transform, destination shape>
// // It should have a number in slot 0 and either a number in slot 1 or slot 2
// // -1 indicates that that slot is empty
// // <0,0,0> indicates that the connection is empty
uniform ivec3[numElements] connections;


// // Output data ; will be interpolated for each fragment
out vec3 f_world_vertexNormal;
out vec3 f_world_vertexPosition;

// // Uniforms ; values that stay constant for whole mesh

int incomingConnection(int a, int loc)
{
	int destination			= -1;
	bool emptyConnection	= true;

	for(int i = 0; i < numElements; ++i)
	{
		destination		= connections[i][loc];
		emptyConnection	= (connections[i][transformSourceLoc] == 0 && connections[i][transformDestLoc] == 0 && connections[1][shapeDestLoc] == 0);
		if (emptyConnection)	{ continue; }
		if (destination == a)	{ return connections[i][transformSourceLoc]; }
	}
	return -1;
}



void transformGlPosition()
{
	int incoming = incomingConnection(model_id, shapeDestLoc);
	if (incoming > -1)
	{ 
		gl_Position	= mat_modelToProjection * transforms[incoming] * model_vertexPosition;
	}
	else
	{
		gl_Position	= mat_modelToProjection * model_vertexPosition;
	}
}
	


void main()
{
	transformGlPosition();

	// // The colour of each vertex will be interpolated to produce the colour of each fragment
	
	f_world_vertexPosition	= vec3(mat_modelToWorld * model_vertexPosition);
	fragmentColor			= model_vertexColor;
	f_world_vertexNormal	= normalize(vec3(mat_modelToWorld * vec4(model_vertexNormal, 0)));
}

