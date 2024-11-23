#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Utils
#include "Camera.h"
#include "CameraManipulator.h"
#include "GLUtils.hpp"

struct SUpdateInfo
{
	float ElapsedTimeInSec = 0.0f;	// Elapsed time since start of the program
	float DeltaTimeInSec = 0.0f;	// Elapsed time since last update
};

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update(const SUpdateInfo&);
	void Render();
	void RenderGUI();

	void KeyboardDown(const SDL_KeyboardEvent&);
	void KeyboardUp(const SDL_KeyboardEvent&);
	void MouseMove(const SDL_MouseMotionEvent&);
	void MouseDown(const SDL_MouseButtonEvent&);
	void MouseUp(const SDL_MouseButtonEvent&);
	void MouseWheel(const SDL_MouseWheelEvent&);
	void Resize(int, int);

	void OtherEvent(const SDL_Event&);
protected:
	void SetupDebugCallback();
	void DrawNoInstance();
	void DrawUboInstance();
	void DrawArrayAttrInstanced();

	//
	// Variables
	//

	float m_delta_time = 0.0f;

	// Camera
	Camera m_camera;
	CameraManipulator m_cameraManipulator;

	//
	// OpenGL
	//

	// We can switch between rendering modes by pressing 1,2 or 3
	int m_drawState = 2;

	void DrawAxes();
	void DrawSkybox();

	// Shader variables
	static constexpr int uboSize = 100;	// How many objects we draw with one draw call
	static constexpr int uboSizeBytes = uboSize * 2 * sizeof(glm::mat4);
	GLuint m_programAxesID = 0;		// Axes program
	GLuint m_programSkyboxID = 0;	// Skybox program
	GLuint m_programNoInstanceID = 0;
	GLuint m_programUboInstanceID = 0;
	GLuint m_programArrayAttrInstanceID = 0;

	// Shader initialization and termination
	void InitShaders();
	void CleanShaders();
	void InitAxesShader();
	void CleanAxesShader();
	void InitSkyboxShaders();
	void CleanSkyboxShaders();

	// Geometry variables
	OGLObject m_Suzanne = {};
	OGLObject m_Skybox = {};
	GLuint m_uboID = 0;
	GLuint m_matrixBufferID = 0;
	static constexpr GLuint uniformBlockBinding = 0;

	static constexpr int INST_NUM = 10000;		// How many heads we draw
	std::vector<glm::mat4> m_world_matricies;	// World matrices
	std::vector<glm::mat4> m_worldIT_matricies; // WorldIT matrices

	// Geometry initialization and termination
	void InitPositions();
	void InitGeometry();
	void InitAttributeMode();
	void CleanGeometry();
	void InitSkyboxGeometry();
	void CleanSkyboxGeometry();

	// Texture variables
	GLuint m_SamplerID = 0;

	GLuint m_SkyboxTextureID = 0;
	GLuint m_woodTextureID = 0;


	// Texture initialization and termination
	void InitTextures();
	void CleanTextures();
	void InitSkyboxTextures();
	void CleanSkyboxTextures();
};