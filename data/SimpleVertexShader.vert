// shadertype=glsl

#version 440 core

const int numElements			= 6;
const int transformSourceLoc	= 0;
const int transformDestLoc		= 1;
const int shapeDestLoc			= 2;

// // Input vertex data, different for all executions of this shader.
in layout(location = 0) vec4 model_vertexPosition;
in layout(location = 1) vec3 model_vertexColor;
in layout(location = 2) vec3 model_vertexNormal;
in layout(location = 3) vec2 model_uv;
in layout(location = 4) int  model_id;
in layout(location = 5) mat4 mat_modelToProjection;
in layout(location = 9) mat4 mat_modelToWorld;

// // Array of transforms
// // Numbers in the transform slots of the connections array
// // indicte the nth matrix in this array
uniform mat4[numElements] transforms;
mat4[numElements] transformsMoved = transforms;

// // Connections is an ivec3 of the form:
// // <source transform, destination transform, destination shape>
// // It should have a number in slot 0 and either a number in slot 1 or slot 2
// // -1 indicates that that slot is empty
// // <0,0,0> indicates that the connection is empty
uniform ivec3[numElements] connections;
uniform int test_int;

// // Output data ; will be interpolated for each fragment
out vec3 f_world_vertexPosition;
out vec3 fragmentColor;
out vec3 f_world_vertexNormal;
out vec2 uv;


bool isEmptyConnection(int index)
{
	return
		connections[index][0] == 0 &&
		connections[index][1] == 0 &&
		connections[index][2] == 0;
}

int incomingConnection(int a, int loc)
{
	if (a == -1) { return -1; }

	int destination			= -1;

	for(int i = 0; i < numElements; ++i)
	{
		destination		= connections[i][loc];
		if (isEmptyConnection(i))	{ continue; }
		if (destination == -1)		{ continue; }
		if (destination == a)	
		{ 
			return connections[i][transformSourceLoc]; 
		}
	}
	return -1;
}



mat4 transformTransform(int endTransformId)
{
	mat4 outTransform = transforms[endTransformId];
	int incomingTransformId = incomingConnection(endTransformId, transformDestLoc);

	while (incomingTransformId != -1)
	{
		outTransform			= transforms[incomingTransformId] * transforms[endTransformId];
		incomingTransformId		= incomingConnection(incomingTransformId, transformDestLoc);
	}
	return outTransform;
}

void transformGlPosition()
{
	int incoming = incomingConnection(model_id, shapeDestLoc);

	mat4 transform = mat4(
		vec4(1.0f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 1.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 1.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	if (incoming > -1)
	{
		transform = transformTransform(incoming);
	}
	gl_Position	= mat_modelToProjection * transform * model_vertexPosition;
	f_world_vertexPosition	= vec3(mat_modelToWorld * transform * model_vertexPosition);
	f_world_vertexNormal	= normalize(vec3(mat_modelToWorld * transform * vec4(model_vertexNormal, 0)));
}
	
vec3 colorFromIndex(int a)
{
	vec3 colArray[6] = vec3[6]
	(
		vec3(1.0f, 0.0f, 0.0f), //red
		vec3(0.0f, 1.0f, 0.0f), //green
		vec3(0.0f, 0.0f, 1.0f), //blue
		vec3(1.0f, 1.0f, 0.0f), //yellow
		vec3(0.0f, 1.0f, 1.0f), //cyan
		vec3(1.0f, 0.0f, 1.0f)  //magenta
	);
	return colArray[a%6];
}

vec4 redColor()
{
	return vec4(1.0, 0.0, 0.0, 1.0);
}
vec4 blueColor()
{
	return vec4(0.0, 0.0, 1.0, 1.0);
}

void main()
{
	transformGlPosition();
	fragmentColor			= model_vertexColor;
	uv						= model_uv;

	
}

