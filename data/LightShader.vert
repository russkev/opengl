in layout(location = 0) vec4 model_vertexPosition;

uniform mat4 mat_transform;
uniform mat4 mat_modelToProjection;
uniform mat4 mat_modelToWorld;

out vec3 worldSpace_vertexPosition;

void main()
{
	gl_Position = mat_modelToProjection * mat_transform * model_vertexPosition;
	worldSpace_vertexPosition = vec3(mat_modelToWorld * mat_transform * model_vertexPosition);
}

