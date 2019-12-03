#include "GL_Scene.h"
#include "mesh/OBJ.h"
#include "mesh/Sphere.h"
#include "mesh/Plane.h"
#include "mesh/Cylinder.h"
#include "mesh/Cube.h"
#include "mesh/Arrow.h"

void GL_Scene::init(const GLuint width, const GLuint height)
{
	m_width		= width;
	m_height	= height;

	printGLProperties();
	initVerboseDebug();
	initSettings();
	initTimer();
	initCam();
	initText();
	initGeo();
	initBuffers();
	initLights();
}

void GL_Scene::printGLProperties()
{
	// // Check OpenGL properties // //
	printf("OpenGL loaded\n");
	//gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
}

void GL_Scene::initVerboseDebug()
{
#ifdef DEBUG
	// // Enable the debug callback // //
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE
	);
#endif 
}

void GL_Scene::initSettings()
{
	// // Dark blue background // //
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// // Enable depth test // //
	glEnable(GL_DEPTH_TEST);
	// // Enable backface culling // //
	glEnable(GL_CULL_FACE);
	// // Set winding direction // // 
	glFrontFace(GL_CCW);
	// // Accept fragment shader if it closer to the camera than the previous one
	glDepthFunc(GL_LESS);
	//glDepthFunc(GL_ALWAYS);
	// // Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GL_Scene::initTimer()
{
	m_timer.init();
}

void GL_Scene::initCam()
{
	GLfloat near	= 0.1f;
	GLfloat far		= 200.0f;
	m_projection	= glm::perspective(glm::radians(50.0f), float(m_width) / float(m_height), near, far);
	glUseProgram(m_program_id);
	const GLuint camPositionLocation = glGetUniformLocation(m_program_id, "camPosition");
	glm::vec3 cam_position = m_cam.getPosition();
	glUniform3fv(camPositionLocation, 1, &cam_position.x);
}

void GL_Scene::initLights()
{
	m_light = PointLight{ { 20.0f, 1.0f, 0.0f } };
	//m_light.programID() = LoadShaders::load("LightShader.vert", "LightShader.frag");
	//m_light.programID() = LoadShaders::load("LightShader.vert", "LightShader.frag");
	m_light_program_id = LoadShaders::load("LightShader.vert", "LightShader.frag");

	//m_light_sh.appendShape(Sphere::createSphere(5));

	std::vector<Vertex> vertices;
	for (Vertex vertex : m_light.mesh().vertices())
	{
		vertices.push_back(vertex);
	}

	m_light_geoBuffer.Append(vertices);

	//auto depthSortedIndices = m_light_sh.depthSort(m_cam.getPosition());
	m_light_indexBuffer.Append(m_light.mesh().indices());

	//m_light_matBuffer.Append(sizeof(glm::mat4), &(m_projection * m_cam.getWorldToViewMatrix()[0][0]));

	auto lightProgramID = m_light_program_id;
	glUseProgram(m_light_program_id);

	const ShapeContainer::intType transfromLocation = glGetUniformLocation(m_light_program_id, "mat_modelToProjection");
	auto matBufferMatrix = m_projection * m_cam.getWorldToViewMatrix() * m_light.transformMatrix();
	glUniformMatrix4fv(transfromLocation, 1, GL_FALSE, &matBufferMatrix[0][0]);

	// // Set up standard information for the VAO
	static const auto shape_info = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();
	static const auto matrix_info = gl_introspect_tuple<std::tuple<glm::mat4>>::get();

	// // Upload the VAO information
	m_vao_light.GenerateVAO(m_light_geoBuffer, 0, shape_info.data(), shape_info.data() + shape_info.size(), POSITION_ATTR);
	//m_vao_light.GenerateVAO(m_light_matBuffer, 1, matrix_info.data(), matrix_info.data() + matrix_info.size(), MODEL_ATTR);


	glUseProgram(m_program_id);
	const ShapeContainer::intType lightPositionLocation = glGetUniformLocation(m_program_id, "lightPosition");
	glUniform3fv(lightPositionLocation, 1, &m_light.position()[0]);
	const ShapeContainer::intType ambientLightLocation = glGetUniformLocation(m_program_id, "ambientLight");
	glUniform3f(ambientLightLocation, 0.2f, 0.2f, 0.4f);
}

void GL_Scene::initText()
{

	m_text.init(5, m_height - 25, 20, m_width, m_height);
	
	m_text.print("HELLO THERE");
}

void GL_Scene::initGeo()
{
	// // Create and compile our GLSL program from the shaders // //
	m_program_id		= LoadShaders::load("SimpleVertexShader.vert", "SimpleFragmentShader.frag");
	
	// // Create Texture
	Texture texture1("uvtemplate.tga");
	Texture texture2("two.tga");
	Texture normalTexture("brickNormalMap_02.tga");
	
	// // Upload textures 
	texture1.upload_to_shader(m_program_id, "textureA");
	texture2.upload_to_shader(m_program_id, "textureB");
	normalTexture.upload_to_shader(m_program_id, "normalMap");
	
	// // Create Geo
	m_sh.appendShape(OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj"), "shader_ball");
	//ShapeData newSphere = Sphere::createSphere(3.0, 20, 20);
	//m_sh.appendShape(Sphere::createSphere(3.0, 50), "sphere");
	m_sh.appendShape(Plane::createPlane(10.0f, 10.0f), "plane");
	//m_sh.appendShape(Cylinder::createCylinder(), "cylinder");
	//m_sh.appendShape(Cube::createCube(4.0f));
	//m_sh.appendShape(Arrow::createArrow());


	// // Create transforms
	glm::vec3 planeScale = glm::vec3(1.0f);

	// // Append transforms
	m_sh.appendTransform(VectorUtils::trs(glm::mat3({ 0.0f, -1.5f, 0.0 }, { 0.0f, 0.0f, 0.0f }, { 1.0, 1.0, 1.0 })), "transform_01");
	m_sh.appendTransform(VectorUtils::trs(glm::mat3({ 0.0f, -2.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, { 1.0, 1.0, 1.0 })), "transform_02");


	// // Transform Geo 
	//m_sh.connect("transform_01", "shader_ball");
	//m_sh.connect("transform_02", "sphere");
}

void GL_Scene::initBuffers()
{
	// // Send information to graphics card
	auto vertices = m_sh.vertices();
	//auto meshes = m_sh.meshes();
	m_geoBuffer.Append(vertices);

	auto depthSortedIndices = m_sh.depthSort(m_cam.getPosition());
	m_indxBuffer.Append(depthSortedIndices);

	auto matBufferMatrix = m_cam.getWorldToViewMatrix();
	m_matBuffer.Append(sizeof(glm::mat4), &(m_projection * m_cam.getWorldToViewMatrix()[0][0]));
	m_wldBuffer.Append(sizeof(glm::mat4), &glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));

	//!!! Create uniform upload function
	glUseProgram(m_program_id);
	const ShapeContainer::intType viewMatrixLocation = glGetUniformLocation(m_program_id, "mat_view");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &matBufferMatrix[0][0]);


	// // Set up standard information for the VAO
	static const auto shape_info = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint, glm::vec3, glm::vec3>>::get();
	static const auto matrix_info = gl_introspect_tuple<std::tuple<glm::mat4>>::get();

	// // Upload the VAO information
	m_vao_main.GenerateVAO(m_geoBuffer, 0, shape_info.data(), shape_info.data() + shape_info.size(), POSITION_ATTR);
	m_vao_main.GenerateVAO(m_matBuffer, 1, matrix_info.data(), matrix_info.data() + matrix_info.size(), MODEL_ATTR);
	m_vao_main.GenerateVAO(m_wldBuffer, 1, matrix_info.data(), matrix_info.data() + matrix_info.size(), WORLD_ATTR);


	m_sh.uploadTransforms(m_program_id);
	m_sh.uploadConnections(m_program_id);

#ifdef DEBUG
	// Debug matrix array
	std::vector<glm::mat4> testTransformsUniform;
	testTransformsUniform = m_sh.readUniform<glm::mat4>(.m_program_id, "transforms", 4);
	std::vector<glm::ivec3> testConnectionsUniform;
	testConnectionsUniform = m_sh.readUniform<glm::ivec3>(.m_program_id, "connections", 7);
#endif
}

bool GL_Scene::pollEvents()
{
	SDL_Event loc_event;
	static bool mouseDown = false;
	static bool altDown = false;
	while (SDL_PollEvent(&loc_event))
	{

		if (loc_event.type == SDL_QUIT)
		{
			return false;
		}
		if (loc_event.type == SDL_KEYUP) {
			if (loc_event.key.keysym.scancode == SDL_SCANCODE_F)
			{
				glm::mat4 wldBuffer = glm::mat4(1.0f);
				m_cam.focus(wldBuffer);
			}
			if (loc_event.key.keysym.scancode == SDL_SCANCODE_4)
			{
				m_displayWireframe = true;
			}
			if (loc_event.key.keysym.scancode == SDL_SCANCODE_5)
			{
				m_displayWireframe = false;
			}
			
		}
	}
	return true;
}

void GL_Scene::renderFrame()
{
	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	// // Clear both the colour buffer and the depth buffer at the same time // //
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat cmAmplitude = 0.5f;
	GLfloat cmFreq = 0.1f;
	GLfloat cmOffset = 0.5f;

	updateTimer();
	updateLights();
	updateCam();
	updateGeo();

	m_vao_main.Bind();
	m_indxBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);

	glUseProgram(m_program_id);
	if (m_displayWireframe)
	{
		glDrawElements(GL_LINE_STRIP, (GLsizei)m_indxBuffer.size(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indxBuffer.size(), GL_UNSIGNED_SHORT, 0);
	}

	glUseProgram(m_light_program_id);
	m_vao_light.Bind();
	m_light_indexBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_light_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);


	m_text.draw();
}

void GL_Scene::updateTimer()
{
	m_timer.update();
}

void GL_Scene::updateLights()
{
	// // Ambient Lighting // //
	//glm::vec4 ambientLight = { 0.0f, 0.34f, 0.6f, 1.0f };
	//glUniform4fv(_State.ambientLightID, 1, &ambientLight.r);
	// // Diffuse Lighting // // 
	//glm::vec3 lightPosition = { 2.0f, -20.0f, 0.0f };
	//const auto lightPositionLocation = glGetUniformLocation(m_program_id, "lightPosition");
	//glUniform3fv(lightPositionLocation, 1, &lightPosition.x);
}

void GL_Scene::updateCam()
{
	// Update camera position
	glUseProgram(m_program_id);
	const GLuint camPositionLocation = glGetUniformLocation(m_program_id, "camPosition");
	glm::vec3 cam_position = m_cam.getPosition();
	glUniform3fv(camPositionLocation, 1, &cam_position.x);

	// Update view matrix
	auto matBufferMatrix = m_cam.getWorldToViewMatrix();
	glUseProgram(m_program_id);
	const ShapeContainer::intType viewMatrixLocation = glGetUniformLocation(m_program_id, "mat_view");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &matBufferMatrix[0][0]);

	// LIGHT
	glUseProgram(m_light_program_id);
	const ShapeContainer::intType transfromLocation = glGetUniformLocation(m_light_program_id, "mat_modelToProjection");
	auto lightMatrix = m_projection * m_cam.getWorldToViewMatrix() * m_light.transformMatrix();
	glUniformMatrix4fv(transfromLocation, 1, GL_FALSE, &lightMatrix[0][0]);

	// // Cam position // //
	//glm::vec3 camPositionVec = _State.cam.getPosition();
	//glUniform3fv(_State.camPositionID, 1, &camPositionVec.x);
}

void GL_Scene::updateGeo()
{
	//auto transform_01	= m_sh.getTransformPtr("transform_01");
	auto time			= m_timer.time();
	//*transform_01		= *transform_01 * VectorUtils::trs(glm::mat3({ 0,0,0 }, { 0, time/1000000, 0 }, { 1,1,1 }));
	
	m_sh.uploadTransforms(m_program_id);
	m_matBuffer.Upload(m_projection * m_cam.getWorldToViewMatrix());
	m_indxBuffer.Upload(m_sh.depthSort(m_cam.getPosition()));



	//m_light_matBuffer.Upload(m_projection * m_cam.getWorldToViewMatrix());
	m_light_indexBuffer.Upload(m_light.mesh().indices());
}

GL_Scene::~GL_Scene() {
	glUseProgram(0);
	glDeleteProgram(m_program_id);
}
