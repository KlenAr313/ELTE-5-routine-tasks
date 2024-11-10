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

	// Mozgás trajektória
	float m_currentParam = 0.0;
	static constexpr int MAX_POINT_COUNT = 20;
	std::vector<glm::vec3> m_controlPoints;

	glm::vec3 EvaluatePathPosition() const;
	glm::vec3 EvaluatePathTangent() const;

	// biztonsági üveg
	static constexpr glm::vec3 GLASS_POSITION = glm::vec3( 0.0f, 0.0f, 21.0f );
	static constexpr glm::vec4 GLASS_TRANSFORMS[4]=
	{
		// pozíció, forgatási szög
		glm::vec4(  0.0f, 0.0f, 21.0f, 0.0f),
		glm::vec4( 21.0f, 0.0f,  0.0f, glm::half_pi<float>()),
		glm::vec4(  0.0f, 0.0f,-21.0f, glm::pi<float>()),
		glm::vec4(-21.0f, 0.0f,  0.0f, glm::pi<float>() * 3.0f / 2.0f ),
	};
	static constexpr glm::vec3 GLASS_SCALE    = glm::vec3( 21.0, 10.5, 10.5 );

	// Kamera
	Camera m_camera;
	CameraManipulator m_cameraManipulator;

	//
	// OpenGL-es dolgok
	//
	
	// shaderekhez szükséges változók
	GLuint m_programID = 0;		  // shaderek programja
	GLuint m_programSkyboxID = 0; // skybox programja
	GLuint m_programAxis = 0; // skybox programja

	// Fényforrás- ...
	glm::vec4 m_lightPos = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );

	glm::vec3 m_La = glm::vec3( 0.125f );
	glm::vec3 m_Ld = glm::vec3(1.0, 1.0, 1.0 );
	glm::vec3 m_Ls = glm::vec3(1.0, 1.0, 1.0 );

	float m_lightConstantAttenuation    = 1.0;
	float m_lightLinearAttenuation      = 0.0;
	float m_lightQuadraticAttenuation   = 0.0;

	// ... és anyagjellemzők
	glm::vec3 m_Ka = glm::vec3( 1.0 );
	glm::vec3 m_Kd = glm::vec3( 1.0 );
	glm::vec3 m_Ks = glm::vec3( 1.0 );

	float m_Shininess = 1.0;

	// Shaderek inicializálása, és törtlése
	void InitShaders();
	void CleanShaders();
	void InitSkyboxShaders();
	void CleanSkyboxShaders();

	// Geometriával kapcsolatos változók

	OGLObject m_SuzanneGPU = {};
	OGLObject m_SkyboxGPU = {};

	// Geometria inicializálása, és törtlése
	void InitGeometry();
	void CleanGeometry();
	void InitSkyboxGeometry();
	void CleanSkyboxGeometry();

	// Textúrázás, és változói
    GLuint m_SamplerID = 0;

	GLuint m_SuzanneTextureID = 0;
	GLuint m_SkyboxTextureID = 0;

	void InitTextures();
	void CleanTextures();
	void InitSkyboxTextures();
	void CleanSkyboxTextures();

	void SetLightingUniforms( GLuint program, float Shininess, glm::vec3 Ka = glm::vec3( 1.0 ), glm::vec3 Kd  = glm::vec3( 1.0 ), glm::vec3 Ks  = glm::vec3( 1.0 ) );
};

