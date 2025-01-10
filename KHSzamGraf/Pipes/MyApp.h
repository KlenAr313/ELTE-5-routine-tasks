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
#include "GLUtils.hpp"
#include "Camera.h"
#include "CameraManipulator.h"

#include "PipeSystem/System.h"

struct SUpdateInfo
{
	float ElapsedTimeInSec = 0.0f; // Program indulása óta eltelt idő
	float DeltaTimeInSec   = 0.0f; // Előző Update óta eltelt idő
};

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update( const SUpdateInfo& );
	void Render();
	void RenderGUI();

	void RenderSphere(glm::mat4& matWorld);
	void RenderCylinder(glm::mat4& matWorld);
	void RenderCircle(glm::mat4& matWorld);

	void RenderSkyBox();

	void KeyboardDown(const SDL_KeyboardEvent&);
	void KeyboardUp(const SDL_KeyboardEvent&);
	void MouseMove(const SDL_MouseMotionEvent&);
	void MouseDown(const SDL_MouseButtonEvent&);
	void MouseUp(const SDL_MouseButtonEvent&);
	void MouseWheel(const SDL_MouseWheelEvent&);
	void Resize(int, int);

	void OtherEvent( const SDL_Event& );
protected:
	void SetupDebugCallback();

	//
	// Adat változók
	//

	float m_ElapsedTimeInSec = 0.0f;

	// Kamera
	Camera m_camera;
	CameraManipulator m_cameraManipulator;

	//
	// OpenGL-es dolgok
	//
	
	// uniform location lekérdezése
	static GLint ul( const char* uniformName ) noexcept;

	// shaderekhez szükséges változók
	GLuint m_Sphere_programID = 0; // Gömbök programja
	GLuint m_Cylinder_programID = 0; // Hengerek programja
	GLuint m_Circle_programID = 0; // Körlap programja
	GLuint m_programSkyboxID = 0;

	// Fényforrás- ...
	glm::vec4 m_lightPos = glm::vec4( 0.48f, 1.1f, 0.9f, 0.0f );

	glm::vec3 m_La = glm::vec3(0.0, 0.0, 0.0 );
	glm::vec3 m_Ld = glm::vec3(1.0, 1.0, 1.0 );
	glm::vec3 m_Ls = glm::vec3(1.0, 1.0, 1.0 );

	float m_lightConstantAttenuation    = 1.0;
	float m_lightLinearAttenuation      = 0.0;
	float m_lightQuadraticAttenuation   = 0.0;

	// ... és anyagjellemzők
	glm::vec3 m_Ka = glm::vec3( 1.0 );
	glm::vec3 m_Kd = glm::vec3( 1.0 );
	glm::vec3 m_Ks = glm::vec3( 0.0 );

	float m_Shininess = 16.0;

	// Shaderek inicializálása, és törtlése
	void InitShaders();
	void CleanShaders();

	// Geometriával kapcsolatos változók

	OGLObject m_surfaceGPU = {};
	OGLObject m_SkyboxGPU = {};

	// Geometria inicializálása, és törtlése
	void InitGeometry();
	void CleanGeometry();

	// Textúrázás, és változói

	GLuint m_TextureID = 0;
	GLuint m_ColorTextureID = 0;
	GLuint m_skyboxTextureID = 0;

	void InitTextures();
	void CleanTextures();

	//Pipe System
	System pipeSystem = System(5, 4);
};

