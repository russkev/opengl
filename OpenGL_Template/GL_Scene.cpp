#include "GL_Scene.h"

void GL_Scene::init(const GLuint width, const GLuint height)
{
	m_width		= width;
	m_height	= height;

	printGLProperties();
	initVerboseDebug();
	initSettings();
	initTimer();
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

void GL_Scene::initTimer()
{
	m_timer.init();
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

	// // BMP Tecture // //
	loadBMP_custom bmp_01 = loadBMP_custom("C:\\Users\\kev_k\\Documents\\_DOCS\\PROGRAMMING\\OpenGL\\repos\\data\\uvtemplate.bmp");

	// // TEST
	auto T_01 = Utilities::minMaxLoop(0.5f);
	auto T_02 = Utilities::minMaxLoop(1.5f);
	auto T_03 = Utilities::minMaxLoop(3.5f);
	auto T_04 = Utilities::minMaxLoop(-0.1f);
	auto T_05 = Utilities::minMaxLoop(-2.8f);
	auto T_06 = Utilities::minMaxLoop(1.2f, 0.0f, 10.0f);
	auto T_07 = Utilities::minMaxLoop(33.6f, 0.0f, 10.0f);
	auto T_08 = Utilities::minMaxLoop(-27.6f, 0.0f, 10.0f);
	auto T_09 = Utilities::minMaxLoop(1.6f, 10.0f, 20.0f);
	auto T_10 = Utilities::minMaxLoop(13.5f, 10.0f, 20.0f);
	auto T_11 = Utilities::minMaxLoop(-29.2f, 10.0f, 20.0f);
	auto T_12 = Utilities::minMaxLoop(-29.2, 10.0, 20.0);
	auto T_13 = Utilities::minMaxLoop(3, 10, 20);
	auto T_14 = Utilities::minMaxLoop(13, 10, 20);
	auto T_15 = Utilities::minMaxLoop(33, 10, 20);
	auto T_16 = Utilities::minMaxLoop(-2, 10, 20);
	auto T_17 = Utilities::minMaxLoop(0.0f);
	auto T_18 = Utilities::minMaxLoop(1.0f);

	// // END TEST


	// // Create Geo		T_02	0.500000000	float

	m_sh.appendShape(m_shapes.makePlane(4), "plane_01");
	m_sh.appendShape(m_shapes.makePlane(4), "plane_02");
	m_sh.appendShape(m_shapes.makePlane(4), "plane_03");

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
	static const auto shape_info = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec2, GLint>>::get();
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
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indxBuffer.size(), GL_UNSIGNED_SHORT, 0);
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
	//glm::vec3 lightPosition = { 0.0f, 2.0f, 0.0f };
	//glUniform3fv(_State.lightPositionID, 1, &lightPosition.x);
}

void GL_Scene::updateCam()
{
	// // Cam position // //
	//glm::vec3 camPositionVec = _State.cam.getPosition();
	//glUniform3fv(_State.camPositionID, 1, &camPositionVec.x);
}

void GL_Scene::updateGeo()
{
	auto transform_01	= m_sh.getTransformPtr("transform_01");
	auto time			= m_timer.time();
	*transform_01		= *transform_01 * Utilities::trs(glm::mat3({ 0,0,0 }, { 0, time/100000, 0 }, { 1,1,1 }));
	
	m_sh.uploadTransforms(m_program_id);
	m_matBuffer.Upload(m_projection * m_cam.getWorldToViewMatrix());
	m_indxBuffer.Upload(m_sh.depthSort(m_cam.getPosition()));
}

GL_Scene::~GL_Scene() {
	glUseProgram(0);
	glDeleteProgram(m_program_id);
}
