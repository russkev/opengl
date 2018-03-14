#include "GL_Scene.h"



void GL_Scene::init(const GLuint width, const GLuint height)
{
	m_width		= width;
	m_height	= height;

	printGLProperties();
	initVerboseDebug();
	initSettings();
	initCam();
	initLights();
	initGeo();
	initBuffers();
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
	//glEnable(GL_CULL_FACE);
	// // Set winding direction // // 
	glFrontFace(GL_CCW);
	// // Accept fragment shader if it closer to the camera than the previous one
	//glDepthFunc(GL_LESS);
	glDepthFunc(GL_ALWAYS);
	// // Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GL_Scene::initCam()
{
	GLfloat near	= 0.1f;
	GLfloat far		= 100.0f;
	m_projection	= glm::perspective(glm::radians(50.0f), float(m_width) / float(m_height), near, far);
}

void GL_Scene::initLights()
{
	// Initialize lights
}

void GL_Scene::initGeo()
{
	// // Create and compile our GLSL program from the shaders // //
	m_program_id = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	// // Create Geo
	m_sh.appendShape(m_shapes.makePlane(2), "plane_01");
	m_sh.appendShape(m_shapes.makePlane(2), "plane_02");
	m_sh.appendShape(m_shapes.makePlane(2), "plane_03");

	// // Create transforms
	glm::vec3 planeScale = glm::vec3(3.0f);

	m_sh.appendTransform(Utilities::trs(glm::mat3({ 0.0f, -1.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, planeScale)), "transform_01");
	m_sh.appendTransform(Utilities::trs(glm::mat3({ 0.0f, -3.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, planeScale)), "transform_02");
	m_sh.appendTransform(Utilities::trs(glm::mat3({ 0.0f, -5.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, planeScale)), "transform_03");


	// // Transform Geo 
	m_sh.connect("transform_01", "plane_01");
	m_sh.connect("transform_02", "plane_02");
	m_sh.connect("transform_03", "plane_03");
}

void GL_Scene::initBuffers()
{
	// // Send information to graphics card
	auto vertices = m_sh.vertices();
	m_geoBuffer.Append(vertices);

	auto depthSortedIndices = m_sh.depthSort(m_cam.getPosition());
	m_indxBuffer.Append(depthSortedIndices);


	//_State.matBuffer.Append(sizeof(glm::mat4), &(_State.projection * _State.cam.getWorldToViewMatrix()[0][0]));
	auto matBufferMatrix = m_cam.getWorldToViewMatrix();
	m_matBuffer.Append(sizeof(glm::mat4), &(m_projection * m_cam.getWorldToViewMatrix()[0][0]));
	m_wldBuffer.Append(sizeof(glm::mat4), &glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));


	// // Set up standard information for the VAO
	static const auto shape_info = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, GLint>>::get();
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

void GL_Scene::render_frame()
{
	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	// // Clear both the colour buffer and the depth buffer at the same time // //
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Get time
	//_State.time = _State.freqMultiplier * SDL_GetPerformanceCounter();

	GLfloat cmAmplitude = 0.5f;
	GLfloat cmFreq = 0.1f;
	GLfloat cmOffset = 0.5f;

	prepareLights();
	prepareCam();
	prepareGeo();

	m_vao_main.Bind();
	m_indxBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indxBuffer.size(), GL_UNSIGNED_SHORT, 0);
}

void GL_Scene::prepareLights()
{
	// // Ambient Lighting // //
	//glm::vec4 ambientLight = { 0.0f, 0.34f, 0.6f, 1.0f };
	//glUniform4fv(_State.ambientLightID, 1, &ambientLight.r);
	// // Diffuse Lighting // // 
	//glm::vec3 lightPosition = { 0.0f, 2.0f, 0.0f };
	//glUniform3fv(_State.lightPositionID, 1, &lightPosition.x);
}

void GL_Scene::prepareCam()
{
	// // Cam position // //
	//glm::vec3 camPositionVec = _State.cam.getPosition();
	//glUniform3fv(_State.camPositionID, 1, &camPositionVec.x);
}

void GL_Scene::prepareGeo()
{
	m_matBuffer.Upload(m_projection * m_cam.getWorldToViewMatrix());
	m_indxBuffer.Upload(m_sh.depthSort(m_cam.getPosition()));
}
