#include "MyApp.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ProgramBuilder.h"
#include "ObjParser.h"

#include <imgui.h>
#include <iostream>
#include <array>
#include <random>

CMyApp::CMyApp()
{
	GLint max_uniform_block_size;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &max_uniform_block_size);
	std::cout << "Maximum uniform block size in bytes (must be at least 16384) " << max_uniform_block_size << std::endl;
}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	// Enable and set the debug callback function if we are in debug context
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void CMyApp::InitShaders()
{
	m_programNoInstanceID = glCreateProgram();
	ProgramBuilder{ m_programNoInstanceID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_PosNormTex.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_Lighting.frag")
		.Link();

	m_programUboInstanceID = glCreateProgram();
	ProgramBuilder{ m_programUboInstanceID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_InstancedUBO.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_Lighting.frag")
		.Link();

	// Get the index of a named uniform block
	GLuint blockIndex = glGetUniformBlockIndex(	m_programUboInstanceID, // Program ID
												"m_ubo_buffer");		// Uniform block name
	// We assign a binding point to an active uniform block
	glUniformBlockBinding(	m_programUboInstanceID,	// Program ID
							blockIndex,				// The index of the active uniform block within program whose binding to assign.
							uniformBlockBinding);	// Specifies the binding point to which to bind the uniform block with index uniformBlockIndex within program.

	m_programArrayAttrInstanceID = glCreateProgram();
	ProgramBuilder{ m_programArrayAttrInstanceID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_InstancedAttr.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_Lighting.frag")
		.Link();

	InitAxesShader();
	InitSkyboxShaders();
}

void CMyApp::CleanShaders()
{
	glDeleteProgram(m_programArrayAttrInstanceID);
	glDeleteProgram(m_programNoInstanceID);
	glDeleteProgram(m_programUboInstanceID);

	CleanAxesShader();
	CleanSkyboxShaders();
}

void CMyApp::InitAxesShader()
{
	m_programAxesID = glCreateProgram();
	ProgramBuilder{ m_programAxesID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_axes.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_PosCol.frag")
		.Link();
}

void CMyApp::CleanAxesShader()
{
	glDeleteProgram(m_programAxesID);
}

void CMyApp::InitSkyboxShaders()
{
	m_programSkyboxID = glCreateProgram();
	ProgramBuilder{ m_programSkyboxID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_skybox.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_skybox.frag")
		.Link();
}

void CMyApp::CleanSkyboxShaders()
{
	glDeleteProgram(m_programSkyboxID);
}

void CMyApp::InitGeometry()
{
	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof(Vertex, position), 3, GL_FLOAT },
		{ 1, offsetof(Vertex, normal), 3, GL_FLOAT },
		{ 2, offsetof(Vertex, texcoord), 2, GL_FLOAT },
	};

	// Suzanne
	// MeshObject<Vertex> suzanneMeshCPU = ObjParser::parse("Assets/SuzanneLowPoly.obj"); // Ha nagyon szaggat, hasznald ezt!
	MeshObject<Vertex> suzanneMeshCPU = ObjParser::parse("Assets/Suzanne.obj");
	m_Suzanne = CreateGLObjectFromMesh(suzanneMeshCPU, vertexAttribList);

	InitSkyboxGeometry();
}

void CMyApp::CleanGeometry()
{
	CleanOGLObject(m_Suzanne);
	CleanSkyboxGeometry();
}

void CMyApp::InitSkyboxGeometry()
{
	MeshObject<glm::vec3> skyboxCPU
	{
		std::vector<glm::vec3>
		{
			// Back side
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1,  1, -1),
			glm::vec3(-1,  1, -1),
			// Front side
			glm::vec3(-1, -1,  1),
			glm::vec3(1, -1,  1),
			glm::vec3(1,  1,  1),
			glm::vec3(-1,  1,  1),
		}, // 8 corner of the cube

		std::vector<GLuint>
		{
			// Back side
			0, 1, 2,
			2, 3, 0,
			// Front side
			4, 6, 5,
			6, 4, 7,
			// Left side
			0, 3, 4,
			4, 3, 7,
			// Right side
			1, 5, 2,
			5, 6, 2,
			// Bottom side
			1, 0, 4,
			1, 4, 5,
			// Top side
			3, 2, 6,
			3, 6, 7,
		}
	};

	m_Skybox = CreateGLObjectFromMesh(skyboxCPU, { { 0, offsetof(glm::vec3, x), 3, GL_FLOAT } });
}

void CMyApp::CleanSkyboxGeometry()
{
	CleanOGLObject(m_Skybox);
}

void CMyApp::InitTextures()
{
	glCreateSamplers( 1, &m_SamplerID );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	ImageRGBA woodImage = ImageFromFile( "Assets/wood.jpg" );

	glCreateTextures( GL_TEXTURE_2D, 1, &m_woodTextureID );
	glTextureStorage2D( m_woodTextureID, NumberOfMIPLevels( woodImage ), GL_RGBA8, woodImage.width, woodImage.height );
	glTextureSubImage2D( m_woodTextureID, 0, 0, 0, woodImage.width, woodImage.height, GL_RGBA, GL_UNSIGNED_BYTE, woodImage.data() );

	glGenerateTextureMipmap( m_woodTextureID );


	InitSkyboxTextures();
}

void CMyApp::CleanTextures()
{
	glDeleteTextures(1, &m_woodTextureID);
	CleanSkyboxTextures();
}

void CMyApp::InitSkyboxTextures()
{
	// skybox texture
	static const char* skyboxFiles[6] = {
		"Assets/xpos.png",
		"Assets/xneg.png",
		"Assets/ypos.png",
		"Assets/yneg.png",
		"Assets/zpos.png",
		"Assets/zneg.png",
	};

	ImageRGBA images[ 6 ];
	for ( int i = 0; i < 6; ++i )
	{
		images[ i ] = ImageFromFile( skyboxFiles[ i ], false );
	}

	glCreateTextures( GL_TEXTURE_CUBE_MAP, 1, &m_SkyboxTextureID );
	glTextureStorage2D( m_SkyboxTextureID, 1, GL_RGBA8, images[ 0 ].width, images[ 0 ].height );

	for ( int face = 0; face < 6; ++face )
	{
		glTextureSubImage3D( m_SkyboxTextureID, 0, 0, 0, face, images[ face ].width, images[ face ].height, 1, GL_RGBA, GL_UNSIGNED_BYTE, images[ face ].data() );
	}

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void CMyApp::CleanSkyboxTextures()
{
	glDeleteTextures(1, &m_SkyboxTextureID);
}

void CMyApp::InitPositions()
{
	// Initializing the Suzanne positions and rotations 
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> rand0_2Pi(0.0f, 2.0f * glm::pi<float>());
	std::uniform_real_distribution<float> randomRadius(10.0f, 150.0f);
	std::uniform_real_distribution<float> random_N10_10(-10.0f, 10.0f);
	
	m_world_matricies.reserve(INST_NUM);
	m_worldIT_matricies.reserve(INST_NUM);
	for (int i = 0; i < INST_NUM; ++i)
	{
		glm::mat4 world = glm::rotate(rand0_2Pi(rd), glm::vec3(0, 1, 0)) *
			glm::translate(glm::vec3(randomRadius(rd), random_N10_10(rd), 0)) *
			glm::rotate(rand0_2Pi(rd), glm::vec3(random_N10_10(rd), random_N10_10(rd), random_N10_10(rd)));

		m_world_matricies.push_back(world);
		m_worldIT_matricies.push_back(glm::transpose(glm::inverse(world)));
	}

	// We create one buffer id
	glCreateBuffers(1, &m_uboID);
    glNamedBufferData( m_uboID, uboSizeBytes, nullptr, GL_DYNAMIC_DRAW );
	// Bind range within a buffer object to an indexed buffer target
	glBindBufferRange(	GL_UNIFORM_BUFFER,	// Target
						uniformBlockBinding,		// Index
						m_uboID,			// Buffer ID
						0,					// Offset
						uboSizeBytes);		// Size in bytes
}

void CMyApp::InitAttributeMode()
{
	static constexpr int vec4Size = sizeof(glm::vec4);
	static constexpr int mat4Size = sizeof(glm::mat4);

	// To help setup the new vao attributes
	const auto addAttrib = [&](int binding, int attr)
	{
		// We cant put our matrix into one attribute, because only max 4 component allowed per attribute,
		// so we need four attribute per 4x4matrix
		for ( int col_i = 0; col_i < 4; col_i++ )
		{

			glEnableVertexArrayAttrib( m_Suzanne.vaoID, attr + col_i );
			glVertexArrayAttribBinding( m_Suzanne.vaoID, attr + col_i, 1 ); // melyik VBO-b�l olvassa az adatokat
			glVertexArrayAttribFormat( m_Suzanne.vaoID, attr + col_i, 4, GL_FLOAT, GL_FALSE, col_i * vec4Size );

		}
	};

	// We add another buffer to our VAO

	glCreateBuffers(1, &m_matrixBufferID);
    glNamedBufferData( m_matrixBufferID, INST_NUM * 2 * mat4Size, nullptr, GL_STATIC_DRAW ); // This allocates the memory on GPU
	glNamedBufferSubData(m_matrixBufferID, 0, INST_NUM * mat4Size, m_world_matricies.data());
	glNamedBufferSubData(m_matrixBufferID, INST_NUM * mat4Size, INST_NUM * mat4Size, m_worldIT_matricies.data());

	glVertexArrayVertexBuffer( m_Suzanne.vaoID, 1, m_matrixBufferID, 0, mat4Size );
	glVertexArrayVertexBuffer( m_Suzanne.vaoID, 2, m_matrixBufferID, INST_NUM * mat4Size, mat4Size );

	// If divisor is zero, the attributes in binding indexed VBO advances once per vertex. If divisor is non-zero,
	// the attribute advances once per divisor instances of the set(s) of vertices being rendered
	glVertexArrayBindingDivisor(m_Suzanne.vaoID, // VAO
								 1,	// Index
								 1 );// Divisor
	glVertexArrayBindingDivisor(m_Suzanne.vaoID, // VAO
								 2,	// Index
								 1 );// Divisor

	addAttrib(1,3);
	addAttrib(2,7);
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	// Set a bluish clear color
	// glClear() will use this for clearing the color buffer.
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();
	InitPositions();
	InitAttributeMode();

	//
	// Other
	//

	glEnable(GL_CULL_FACE);	 // Enable discarding the back-facing faces.
	glCullFace(GL_BACK);     // GL_BACK: facets facing away from camera, GL_FRONT: facets facing towards the camera
	glEnable(GL_DEPTH_TEST); // Enable depth testing. (for overlapping geometry)

	// Camera
	m_camera.SetView(
		glm::vec3(5, 5, 5),	// From where we look at the scene - eye
		glm::vec3(0, 0, 0),	// Which point of the scene we are looking at - at
		glm::vec3(0, 1, 0)	// Upwards direction - up
	);
	m_cameraManipulator.SetCamera(&m_camera);

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_uboID);
	glDeleteBuffers(1, &m_matrixBufferID);

	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

void CMyApp::Update(const SUpdateInfo& updateInfo)
{
	m_cameraManipulator.Update(updateInfo.DeltaTimeInSec);
	m_delta_time = updateInfo.DeltaTimeInSec;
}

void CMyApp::DrawAxes()
{
	// We always want to see it, regardless of whether there is an object in front of it
	glDisable(GL_DEPTH_TEST);

	glm::mat4 axisWorld = glm::translate(m_camera.GetAt());
	glProgramUniformMatrix4fv( m_programAxesID, ul(m_programAxesID, "viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));
	glProgramUniformMatrix4fv( m_programAxesID, ul(m_programAxesID, "world"), 1, GL_FALSE, glm::value_ptr(axisWorld));
	
	glUseProgram(m_programAxesID);

	glDrawArrays(GL_LINES, 0, 6);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void CMyApp::DrawSkybox()
{
	glProgramUniform1i( m_programSkyboxID, ul( m_programSkyboxID, "skyboxTexture"), 1 );
	glProgramUniformMatrix4fv( m_programSkyboxID, ul( m_programSkyboxID, "viewProj"),	1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()) );
	glProgramUniformMatrix4fv( m_programSkyboxID, ul( m_programSkyboxID, "world"),	1, GL_FALSE, glm::value_ptr(glm::translate(m_camera.GetEye())) );

	// Save the last Z-test, namely the relation by which we update the pixel.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// Now we use less-then-or-equal, because we push everything to the far clipping plane
	glDepthFunc(GL_LEQUAL);

	glBindTextureUnit( 1, m_SkyboxTextureID );
	glBindSampler( 1, m_SamplerID );
	
	glBindVertexArray(m_Skybox.vaoID);
	
	glUseProgram(m_programSkyboxID);

	glDrawElements(GL_TRIANGLES, m_Skybox.count, GL_UNSIGNED_INT, nullptr);

	glDepthFunc(prevDepthFnc);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindTextureUnit( 0, 0 );
	glBindSampler( 0, 0 );

}

void CMyApp::DrawNoInstance()
{
	// Nothing unusal here

	glProgramUniformMatrix4fv( m_programNoInstanceID, ul( m_programNoInstanceID, "viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));
	glProgramUniform1i( m_programNoInstanceID, ul( m_programNoInstanceID, "texImage"), 0);

	glBindVertexArray(m_Suzanne.vaoID);

	glBindTextureUnit( 0, m_woodTextureID );
	glBindSampler( 0, m_SamplerID );

	glUseProgram(m_programNoInstanceID);

	for (int i = 0; i < INST_NUM; ++i) {
		glProgramUniformMatrix4fv( m_programNoInstanceID, ul( m_programNoInstanceID, "world"), 1, GL_FALSE, glm::value_ptr(m_world_matricies[i]));
		glProgramUniformMatrix4fv( m_programNoInstanceID, ul( m_programNoInstanceID, "worldIT"), 1, GL_FALSE, glm::value_ptr(m_worldIT_matricies[i]));
		glDrawElements(GL_TRIANGLES, m_Suzanne.count, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindTextureUnit( 0, 0 );
	glBindSampler( 0, 0 );
	glUseProgram(0);
}

void CMyApp::DrawUboInstance()
{
	glBindVertexArray(m_Suzanne.vaoID);

	glBindTextureUnit( 0, m_woodTextureID );
	glBindSampler( 0, m_SamplerID );

	glProgramUniformMatrix4fv( m_programUboInstanceID, ul( m_programUboInstanceID, "viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));
	glProgramUniform1i( m_programUboInstanceID, ul( m_programUboInstanceID, "texImage"), 0);

	glBindBuffer(GL_UNIFORM_BUFFER, m_uboID);
	glUseProgram(m_programUboInstanceID);
	

	// Fill the UBO with data then draw, and repeat until every object is drawn 
	for (int rendered_total = 0; rendered_total < INST_NUM;)
	{
		int to_render = std::min(uboSize, INST_NUM - rendered_total);


		std::vector<glm::mat4> data(2 * uboSize);
		// Similar to memcpy
		std::copy(m_world_matricies.begin() + rendered_total,	m_world_matricies.begin() + rendered_total + to_render,		data.begin());
		std::copy(m_worldIT_matricies.begin() + rendered_total, m_worldIT_matricies.begin() + rendered_total + to_render,	data.begin() + uboSize);

		// Set buffer data, we use subdata because we don't want to reallocate the buffer, we just want to set the data
		glBufferSubData(/*m_uboID*/GL_UNIFORM_BUFFER,  				// Target
						0,							// Offset
						uboSizeBytes,				// Size in bytes
						(const void*)data.data());	// Pointer to the data

		
		// We draw multiple instances of Suzzanne with one call
		glDrawElementsInstanced(GL_TRIANGLES,		// Primitive type
								m_Suzanne.count,	// Count
								GL_UNSIGNED_INT,	// Index buffer data type
								0,					// Offset in the index buffer
								to_render);			// How many instance do we draw? (Only new compared to glDrawElements)

		rendered_total += to_render;
	}
	// We can unbind them
	glBindVertexArray(0);

	glBindTextureUnit( 0, 0 );
	glBindSampler( 0, 0 );

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glUseProgram(0);
}

void CMyApp::DrawArrayAttrInstanced()
{
	glBindVertexArray(m_Suzanne.vaoID);

	// We don't have to set out World and WorldIT matrices, 
	// because they are shipped by the VAO
	glBindTextureUnit( 0, m_woodTextureID );
	glBindSampler( 0, m_SamplerID );


	glUseProgram(m_programArrayAttrInstanceID);
	glProgramUniformMatrix4fv( m_programArrayAttrInstanceID, ul( m_programArrayAttrInstanceID, "viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));
	glProgramUniform1i( m_programArrayAttrInstanceID, ul(m_programArrayAttrInstanceID, "texImage"), 0);

	// We draw multiple instances of Suzzanne with one call
	glDrawElementsInstanced(GL_TRIANGLES,	// Primitive type
		m_Suzanne.count,	// Count
		GL_UNSIGNED_INT,	// Index buffer data type
		0,					// Offset in the index buffer
		INST_NUM);			// How many instance do we draw? (Only new compared to glDrawElements)

	glBindVertexArray(0);
	glBindTextureUnit( 0, m_woodTextureID );
	glBindSampler( 0, 0 );
}

void CMyApp::Render()
{
	// Clear the framebuffer (GL_COLOR_BUFFER_BIT) and the depth buffer (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (m_drawState)
	{
	case 0:DrawNoInstance(); break;
	case 1:DrawUboInstance(); break;
	case 2:DrawArrayAttrInstanced(); break;
	}

	DrawSkybox();
	DrawAxes();
}

void CMyApp::RenderGUI()
{
	// ImGui::ShowDemoWindow();
	if (ImGui::Begin("Instancing"))
	{
		ImGui::Text("1: No instancing 2: UBO instancing 3: AttribArr instancing");

		const float refresh_time = 0.5f;
		static float timer = 0;
		static int   frameCount = 0;
		static float fps = 0;
		static float avgFrameTime = 0.0f;

		timer += static_cast<float>(m_delta_time);
		++frameCount;
		if (timer > refresh_time) {
			avgFrameTime = timer / frameCount;
			fps = frameCount / timer;
			timer = 0;
			frameCount = 0;
		}
		ImGui::Text("FPS: %d", static_cast<int>(fps));
		ImGui::Text("ms %f", avgFrameTime);

		switch (m_drawState)
		{
		case 0:ImGui::Text("No instancing"); break;
		case 1:ImGui::Text("UBO"); break;
		case 2:ImGui::Text("Attribute"); break;
		}
	}
	ImGui::End();
}

// https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
// https://wiki.libsdl.org/SDL2/SDL_Keysym
// https://wiki.libsdl.org/SDL2/SDL_Keycode
// https://wiki.libsdl.org/SDL2/SDL_Keymod

void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{
	if (key.repeat == 0) // Triggers only once when held
	{
		if (key.keysym.sym == SDLK_F5 && key.keysym.mod & KMOD_CTRL) // CTRL + F5
		{
			CleanShaders();
			InitShaders();
		}
		if (key.keysym.sym == SDLK_F1) // F1
		{
			GLint polygonModeFrontAndBack[2] = {};
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
			glGetIntegerv(GL_POLYGON_MODE, polygonModeFrontAndBack); // Query the current polygon mode. It gives the front and back modes separately.
			GLenum polygonMode = (polygonModeFrontAndBack[0] != GL_FILL ? GL_FILL : GL_LINE); // Switch between FILL and LINE
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml
			glPolygonMode(GL_FRONT_AND_BACK, polygonMode); // Set the new polygon mode
		}
	}
	m_cameraManipulator.KeyboardDown(key);
	switch (key.keysym.sym)
	{
	case SDLK_1:m_drawState = 0; break;
	case SDLK_2:m_drawState = 1; break;
	case SDLK_3:m_drawState = 2; break;
	}
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp(key);
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove(mouse);
}

// https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

void CMyApp::MouseDown(const SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(const SDL_MouseButtonEvent& mouse)
{
}

// https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent

void CMyApp::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_cameraManipulator.MouseWheel(wheel);
}

// New window size
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_camera.SetAspect(static_cast<float>(_w) / _h);
}

// Other SDL events
// https://wiki.libsdl.org/SDL2/SDL_Event

void CMyApp::OtherEvent(const SDL_Event& ev)
{
}