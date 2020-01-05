#version 440 core

const int numElements			= 6;
const int transformSourceLoc	= 0;
const int transformDestLoc		= 1;
const int shapeDestLoc			= 2;

// // Input vertex data, different for all executions of this shader.
in layout(location = 0 ) vec3 vertex_position;
in layout(location = 1 ) vec3 vertex_color;
in layout(location = 2 ) vec3 vertex_normal;
in layout(location = 3 ) vec2 vertex_uv;
in layout(location = 4 ) int  vertex_id;
in layout(location = 5 ) vec3 vertex_tangent;
in layout(location = 6 ) vec3 vertex_bitangent;
in layout(location = 7 ) mat4 mat_modelToProjection;
in layout(location = 11) mat4 mat_model_to_world;
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

// // Uniform for light position
uniform vec3 lightPosition;
uniform vec3 camPosition;
uniform mat4 mat_view;

// // Output data - will be interpolated for each fragment
out vec3 worldSpace_vertexPosition;
out vec3 worldSpace_vertexNormal;
//out vec3 worldSpace_lightPosition;

out vec3 fragmentColor;
out vec2 uv;

out vec3 cameraSpace_vertexPosition;
out vec3 cameraSpace_normalDirection;
out vec3 cameraSpace_eyeDirection;
out vec3 cameraSpace_lightDirection;

out mat3 mat_tangentToWorld;
out vec3 tangentSpace_lightPosition;
out vec3 tangentSpace_camPosition;
out vec3 tangentSpace_fragPosition;
out vec3 tangentSpace_cDirection;

mat4 transform = mat4(1.0);

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

// Calculate final transform matrix
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

// Calculate final vertex position
void transformGlPosition()
{
	int incoming = incomingConnection(vertex_id, shapeDestLoc);

	if (incoming > -1)
	{
		transform = transformTransform(incoming);
	}
	gl_Position					= mat_modelToProjection * transform * vec4(vertex_position, 1.0);
	worldSpace_vertexPosition	= vec3(mat_model_to_world * transform * vec4(vertex_position, 1.0));
	worldSpace_vertexNormal		= normalize(vec3(mat_model_to_world * transform * vec4(vertex_normal, 0)));
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




void sendTangendSpaceInformation()
{
	// Tangent vector
	vec3 tangentBasis	= normalize(vec3(transform * vec4(vertex_tangent, 0.0)));

	// Normal vector
	vec3 normalBasis	= normalize(vec3(transform * vec4(vertex_normal, 0.0)));

	// Orthogonalize the tangent vector use Gram Schmidt procedure 
	tangentBasis = normalize(tangentBasis - dot(tangentBasis, normalBasis) * normalBasis);

	// Calculate the bitangent (bitangent from model not required)
	vec3 bitangentBasis = cross(normalBasis, tangentBasis);
	
	// Calculate the tangent to world transition matrix
	mat_tangentToWorld = mat3(tangentBasis, bitangentBasis, normalBasis);

	// World to tangent matrix is inverse of TBN_matrix
	mat3 mat_worldToTangent = transpose(mat_tangentToWorld);

	// Convert important coordinates to their tangent space values
	tangentSpace_camPosition	= mat_worldToTangent * camPosition;
	tangentSpace_lightPosition	= mat_worldToTangent * lightPosition;
	tangentSpace_fragPosition	= mat_worldToTangent * worldSpace_vertexPosition;


	vec3 c_a		= (mat_view * vec4(worldSpace_vertexPosition, 0.0)).xyz;
	vec3 c_b		= vec3(0.0, 0.0, 0.0) - c_a;
	tangentSpace_cDirection = mat_worldToTangent * c_b;
}

void sendCameraSpaceInformation()
{
	// Convert important coordinates to their camera space values
	cameraSpace_vertexPosition		= (mat_view * vec4(worldSpace_vertexPosition, 0.0)).xyz;
	cameraSpace_eyeDirection		= vec3(0.0, 0.0, 0.0) - cameraSpace_vertexPosition;
	cameraSpace_lightDirection		= (mat_view * vec4(lightPosition, 0.0)).xyz - cameraSpace_vertexPosition;
	cameraSpace_normalDirection		= (mat_view * vec4(worldSpace_vertexNormal, 0.0)).xyz;




}

void main()
{
	transformGlPosition();
	fragmentColor				= vertex_color;
	uv							= vertex_uv;
	sendTangendSpaceInformation();		
	sendCameraSpaceInformation();

}

