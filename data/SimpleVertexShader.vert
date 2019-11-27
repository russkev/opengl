#version 440 core

const int numElements			= 6;
const int transformSourceLoc	= 0;
const int transformDestLoc		= 1;
const int shapeDestLoc			= 2;

// // Input vertex data, different for all executions of this shader.
in layout(location = 0 ) vec4 model_vertexPosition;
in layout(location = 1 ) vec3 model_vertexColor;
in layout(location = 2 ) vec3 model_vertexNormal;
in layout(location = 3 ) vec2 model_uv;
in layout(location = 4 ) int  model_id;
in layout(location = 5 ) vec3 model_vertexTangent;
in layout(location = 6 ) vec3 model_vertexBitangent;
in layout(location = 7 ) mat4 mat_modelToProjection;
in layout(location = 11) mat4 mat_modelToWorld;
in layout(location = 15) vec2 text_vertexPosition;
in layout(location = 16) vec2 text_uv;
in layout(location = 17) int  text_id;

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

// // Uniforms for working out text position
uniform int width;
uniform int height;

// // Uniform for light position
uniform vec3 lightPosition;
uniform mat4 mat_view;

// // Output data ; will be interpolated for each fragment
out vec3 f_world_vertexPosition;
out vec3 fragmentColor;
out vec3 f_world_vertexNormal;
out vec2 uv;
//out mat4 transform_matrix;

out vec3 worldSpace_lightPosition;

out vec3 cameraSpace_eyeDirection;
out vec3 cameraSpace_lightDirection;

out mat3 TBN_matrix;
out vec3 tangentSpace_eyeDirection;
out vec3 tangentSpace_lightDirection;

mat4 transform = mat4(
		vec4(1.0f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 1.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 1.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

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




mat3 tangent_bitangent_normal_matrix()
{
	vec3 T	= normalize(vec3(transform * vec4(model_vertexTangent, 0.0)));
	vec3 B	= normalize(vec3(transform * vec4(model_vertexBitangent, 0.0)));
	vec3 N	= normalize(vec3(transform * vec4(model_vertexNormal, 0.0)));

	//vec3 vertexNormal_cameraSpace		= mat3(mat_modelToProjection) * normalize(model_vertexNormal);
	//vec3 vertexTangent_cameraSpace		= mat3(mat_modelToProjection) * normalize(model_vertexTangent);
	//vec3 vertexBiTangent_cameraSpace	= mat3(mat_modelToProjection) * normalize(model_vertexBitangent);
	
	return mat3(T, B, N);
	//return transpose(mat3(
	//	vertexTangent_cameraSpace,
	//	vertexBiTangent_cameraSpace,
	//	vertexNormal_cameraSpace
	//));
}

void sendLightAndNormalMap()
{
	vec3 cameraSpace_vertexPosition	= ( mat_view * mat_modelToWorld * model_vertexPosition).xyz;
	cameraSpace_eyeDirection		= vec3(0,0,0) - cameraSpace_vertexPosition;
	vec3 cameraSpace_lightPosition	= (mat_view * vec4(worldSpace_lightPosition, 1)).xyz;
	cameraSpace_lightDirection		= cameraSpace_lightPosition + cameraSpace_eyeDirection;
	
	mat3 TBN = tangent_bitangent_normal_matrix();

	TBN_matrix = TBN;
	tangentSpace_lightDirection		= TBN * cameraSpace_lightDirection;
	tangentSpace_eyeDirection		= TBN * cameraSpace_eyeDirection;
}

void main()
{
	transformGlPosition();
	fragmentColor				= model_vertexColor;
	uv							= model_uv;
	worldSpace_lightPosition	= lightPosition;
	sendLightAndNormalMap();

}

