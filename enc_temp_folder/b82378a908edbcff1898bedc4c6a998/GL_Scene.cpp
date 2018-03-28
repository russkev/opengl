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
	m_program_id		= LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");
	m_text_program_id	= LoadShaders("Text2D.vert", "Text2D.frag");

	// // Texture
	Texture texture1("uvtemplate.tga");
	Texture texture2("two.tga");

	texture1.upload_to_shader(m_program_id, "textureA", 0);
	texture2.upload_to_shader(m_program_id, "textureB", 1);

	Text2D text1("uvtemplate.tga");
	//Text2D text1("font_calibri_01.tga");
	text1.print("Hello world! Hello world! Hello world! Hello world! Hello world! Hello world! Hello world! Hello world! Hello world! ", 10, 500, 60, m_textBuffer, m_text_program_id);
	static const auto text2D_info = gl_introspect_tuple<std::tuple<glm::vec2, glm::vec2>>::get();
	m_vao_text.GenerateVAO(m_textBuffer, 0, text2D_info.data(), text2D_info.data() + text2D_info.size());
	// // Create Geo

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
	*transform_01		= *transform_01 * Utilities::trs(glm::mat3({ 0,0,0 }, { 0, time/1000000, 0 }, { 1,1,1 }));
	
	m_sh.uploadTransforms(m_program_id);
	m_matBuffer.Upload(m_projection * m_cam.getWorldToViewMatrix());
	m_indxBuffer.Upload(m_sh.depthSort(m_cam.getPosition()));
}

GL_Scene::~GL_Scene() {
	glUseProgram(0);
	glDeleteProgram(m_program_id);
}
