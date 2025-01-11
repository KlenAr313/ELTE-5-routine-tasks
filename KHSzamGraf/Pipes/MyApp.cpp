#include "MyApp.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ObjParser.h"
#include "ParametricSurfaceMesh.hpp"
#include "ProgramBuilder.h"

#include <imgui.h>

CMyApp::CMyApp()
{

}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	// engedélyezzük és állítsuk be a debug callback függvényt ha debug context-ben vagyunk 
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void CMyApp::InitShaders()
{
	m_Sphere_programID = glCreateProgram();
	AssembleProgram( m_Sphere_programID, "Shaders/Vert_PosNormTex.vert", "Shaders/Frag_Lighting.frag" );

	m_Cylinder_programID = glCreateProgram();
	AssembleProgram(m_Cylinder_programID, "Shaders/Vert_Cylinder_PosNormTex.vert", "Shaders/Frag_Lighting.frag" );

	m_Circle_programID = glCreateProgram();
	AssembleProgram(m_Circle_programID, "Shaders/Vert_Circle_PosNormTex.vert", "Shaders/Frag_Lighting.frag" );

	m_programSkyboxID = glCreateProgram();
	AssembleProgram(m_programSkyboxID, "Shaders/Vert_skybox.vert", "Shaders/Frag_skybox_skeleton.frag");

	m_programPostprocessID = glCreateProgram();
	ProgramBuilder{ m_programPostprocessID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_FullScreen.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_Postprocess.frag")
		.Link();
}

void CMyApp::CleanShaders()
{
	glDeleteProgram( m_Sphere_programID );
	glDeleteProgram(m_Cylinder_programID);
	glDeleteProgram(m_programSkyboxID);
}

// Nyers parameterek
struct Param
{
	glm::vec3 GetPos( float u, float v ) const noexcept
	{
		return glm::vec3( u, v, 0.0f );
	}
	glm::vec3 GetNorm( float u, float v ) const noexcept
	{
		return glm::vec3( 0.0,0.0,1.0 );
	}
	glm::vec2 GetTex( float u, float v ) const noexcept
	{
		return glm::vec2( u, v );
	}
};

struct Sphere
{
	float r;

	Sphere(float _r = 1.0f) : r(_r)
	{ }

	glm::vec3 GetPos(float u, float v) const noexcept
	{
		u *= glm::two_pi<float>();
		v *= glm::pi<float>();

		return glm::vec3( 
			r * sinf(v) * cosf(u),
			r * cosf(v),
			r * sinf(v) * sinf(u));
	}
	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		/*u *= glm::two_pi<float>();
		v *= glm::pi<float>();

		return glm::vec3(
			 sinf(v) * cosf(u),
			 cosf(v),
			 sinf(v) * sinf(u));*/

		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du, dv));
	}
	glm::vec2 GetTex(float u, float v) const noexcept
	{
		return glm::vec2(u, v);
	}
};

struct Torus
{
	float r;
	float R;

	Torus(float _r = 2.0f, float _R = 2.0f) : r(_r), R(_R)
	{ }

	glm::vec3 GetPos(float u, float v) const noexcept
	{
		u *= glm::two_pi<float>();
		v *= glm::two_pi<float>();

		return glm::vec3(
			(R + r * cosf(u)) * cosf(v),
			r * sinf(u),
			(R + r * cosf(u)) * sinf(v));
	}
	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		/*u *= glm::two_pi<float>();
		v *= glm::two_pi<float>();

		glm::vec3 du = glm::vec3(-sinf(u) *cosf(v),cosf(u), -sinf(u)*sinf(v));
		glm::vec3 dv = glm::vec3(-sinf(v), 0.f, cosf(v));*/
		
		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du,dv));
	}
	glm::vec2 GetTex(float u, float v) const noexcept
	{
		return glm::vec2(u, v);
	}
};

struct SurfaceOfRevolution
{
	float ProfileFunc(float t) const noexcept
	{
		return  t * t;
	}

	glm::vec3 GetPos(float u, float v) const noexcept
	{
		u *= glm::two_pi<float>();

		float r = ProfileFunc(v);

		return glm::vec3(
			r * cosf(u),
			v,
			-r * sinf(u)
		);
	}
	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du, dv));
	}
	glm::vec2 GetTex(float u, float v) const noexcept
	{
		return glm::vec2(u, v);
	}
};

void CMyApp::InitFrameBufferObject()
{
	// FBO létrehozása
	if (m_frameBufferCreated)
		return;
	glCreateFramebuffers(1, &m_frameBuffer);
	m_frameBufferCreated = true;
}

void CMyApp::CleanFrameBufferObject()
{
	glDeleteFramebuffers(1, &m_frameBuffer);
}

void CMyApp::InitResolutionDependentResources(int width, int height)
{
	// Setup the texture
	// We use texture because we will sample it later in the shader
	glCreateTextures(GL_TEXTURE_2D, 1, &m_colorBuffer);
	glTextureStorage2D(m_colorBuffer, 1, GL_RGBA32F, width, height);

	glNamedFramebufferTexture(m_frameBuffer, GL_COLOR_ATTACHMENT0, m_colorBuffer, 0);

	// Setup renderbuffer
	// We use renderbuffer because it's more optimized for drawing
	// and we won't sample it later
	glCreateRenderbuffers(1, &m_depthBuffer);
	glNamedRenderbufferStorage(m_depthBuffer, GL_DEPTH_COMPONENT24, width, height);

	glNamedFramebufferRenderbuffer(m_frameBuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	// Completeness check
	GLenum status = glCheckNamedFramebufferStatus(m_frameBuffer, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[InitFramebuffer] Incomplete framebuffer GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[InitFramebuffer] Incomplete framebuffer GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT!");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[InitFramebuffer] Incomplete framebuffer GL_FRAMEBUFFER_UNSUPPORTED!");
			break;
		}
	}
}

void CMyApp::CleanResolutionDependentResources()
{
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteTextures(1, &m_colorBuffer);
}

void CMyApp::InitGeometry()
{

	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof( Vertex, position ), 3, GL_FLOAT },
		//{ 1, offsetof( Vertex, normal   ), 3, GL_FLOAT },
		{ 1, offsetof( Vertex, texcoord ), 2, GL_FLOAT },
	};

	MeshObject<Vertex> surfaceMeshCPU = GetParamSurfMesh(Param() );
	m_surfaceGPU = CreateGLObjectFromMesh( surfaceMeshCPU, vertexAttribList );


	// skybox geo
	MeshObject<glm::vec3> skyboxCPU =
	{
		std::vector<glm::vec3>
		{
			// hátsó lap
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1,  1, -1),
			glm::vec3(-1,  1, -1),
			// elülső lap
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1,  1, 1),
			glm::vec3(-1,  1, 1),
		},

		std::vector<GLuint>
		{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
		}
	};

	m_SkyboxGPU = CreateGLObjectFromMesh(skyboxCPU, { { 0, offsetof(glm::vec3,x), 3, GL_FLOAT } });
}

void CMyApp::CleanGeometry()
{
	CleanOGLObject( m_surfaceGPU );
	CleanOGLObject(m_SkyboxGPU);
}

void CMyApp::InitTextures()
{
	// diffuse texture
	glGenTextures( 1, &m_TextureID );
	//TextureFromFile( m_TextureID, "Assets/Approximate_Earth_Heigh_Map.png" );
	TextureFromFile( m_TextureID, "Assets/earth-height-map.png" );
	//TextureFromFile( m_TextureID, "Assets/color_checkerboard.png" );
	SetupTextureSampling( GL_TEXTURE_2D, m_TextureID );

	glGenTextures(1, &m_ColorTextureID);
	TextureFromFile(m_ColorTextureID, "Assets/colorTex.jpg" );
	SetupTextureSampling(GL_TEXTURE_2D, m_ColorTextureID);

	glGenTextures(1, &m_skyboxTextureID);
	TextureFromFile(m_skyboxTextureID, "Assets/SkyBox2/xpos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFile(m_skyboxTextureID, "Assets/SkyBox2/xneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFile(m_skyboxTextureID, "Assets/SkyBox2/ypos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFile(m_skyboxTextureID, "Assets/SkyBox2/yneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFile(m_skyboxTextureID, "Assets/SkyBox2/zpos.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFile(m_skyboxTextureID, "Assets/SkyBox2/zneg.png", GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	SetupTextureSampling(GL_TEXTURE_CUBE_MAP, m_skyboxTextureID, false);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void CMyApp::CleanTextures()
{
	glDeleteTextures( 1, &m_TextureID );
	glDeleteTextures( 1, &m_ColorTextureID);
	glDeleteTextures(1, &m_skyboxTextureID);
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();
	InitFrameBufferObject();

	//
	// egyéb inicializálás
	//

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glCullFace(GL_BACK);    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	// kamera
	m_camera.SetView(
		glm::vec3(15.0, 0.0, 15.0),// honnan nézzük a színteret	   - eye
		glm::vec3(0.0, 0.0, 0.0),   // a színtér melyik pontját nézzük - at
		glm::vec3(0.0, 1.0, 0.0));  // felfelé mutató irány a világban - up

	m_cameraManipulator.SetCamera( &m_camera );

	return true;
}

void CMyApp::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
	CleanResolutionDependentResources();
	CleanFrameBufferObject();
}

void CMyApp::Update( const SUpdateInfo& updateInfo )
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;

	m_cameraManipulator.Update( updateInfo.DeltaTimeInSec );
	//m_lightPos = glm::vec4( m_camera.GetEye(), 1.0 );

	if (m_ElapsedTimeInSec - prevActionTime > generationTime)
	{
		pipeSystem.next();
		prevActionTime = m_ElapsedTimeInSec;
	}

	if (m_needNewSystem)
	{
		pipeSystem = System(m_gridSize, 2);
		m_needNewSystem = false;
}
}

void CMyApp::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	if (m_needFreshFboByMouse || m_needFreshFboByKey || m_needFreshFboByLight)
	{
		// töröljük a frampuffert (GL_COLOR_BUFFER_BIT)...
		// ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto element : pipeSystem.elements)
		{
			if (element->isSphere)
			{
				RenderSphere(element->posRot, element->color);

				if (!element->isEnd)
					RenderCylinder(element->posRot, element->color, true);

				if (!element->isBegin)
					RenderCylinder(element->prevPosRot, element->color, true);
			}
			else
				RenderCylinder(element->posRot, element->color, false);
		}

		RenderSkyBox();
		m_needFreshFboByMouse = false;
		m_needFreshFboByLight = false; 
		m_needFreshFboByKey = length(m_cameraManipulator.GetSpeed()) != 0;
	}


	for (auto element : pipeSystem.freshElements)
	{
		if (element->isSphere)
		{
			RenderSphere(element->posRot, element->color);

			if(!element->isEnd)
				RenderCylinder(element->posRot, element->color, true);

			if(!element->isBegin)
				RenderCylinder(element->prevPosRot, element->color, true);
		}
		else
			RenderCylinder(element->posRot, element->color, false);
	}

	// VAO kikapcsolása
	glBindVertexArray( 0 );

	//Bac to default fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0);

	glBindTextureUnit(0, m_colorBuffer);

	glUseProgram(m_programPostprocessID);
	glUniform1i(ul("frameTex"), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void CMyApp::RenderSphere(glm::mat4& matWorld, glm::vec3& color)
{
	// - VAO beállítása
	glBindVertexArray(m_surfaceGPU.vaoID);

	// - Textúrák beállítása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);

	glUseProgram(m_Sphere_programID);

	// - Uniform paraméterek
	glUniform3fv(ul("color"), 1, glm::value_ptr(color));

	// view és projekciós mátrix
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));

	// Transzformációs mátrixok

	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));

	// - Fényforrások beállítása
	glUniform3fv(ul("cameraPos"), 1, glm::value_ptr(m_camera.GetEye()));
	glUniform4fv(ul("lightPos"), 1, glm::value_ptr(m_lightPos));

	glUniform3fv(ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv(ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv(ul("Ls"), 1, glm::value_ptr(m_Ls));

	glUniform1f(ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f(ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f(ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);

	// - Anyagjellemzők beállítása
	glUniform3fv(ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv(ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv(ul("Ks"), 1, glm::value_ptr(m_Ks));

	glUniform1f(ul("Shininess"), m_Shininess);


	// - textúraegységek beállítása
	glUniform1i(ul("texImage"), 0);
	glUniform1i(ul("colorTexImage"), 1);

	// Rajzolási parancs kiadása
	glDrawElements(GL_TRIANGLES,
		m_surfaceGPU.count,
		GL_UNSIGNED_INT,
		nullptr);

	// shader kikapcsolasa
	glUseProgram(0);

	// - Textúrák kikapcsolása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CMyApp::RenderCylinder(glm::mat4 matWorld, glm::vec3& color, bool sphereExtend)
{
	RenderCircle(matWorld, color);
	// - VAO beállítása
	glBindVertexArray(m_surfaceGPU.vaoID);

	// - Textúrák beállítása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);

	glUseProgram(m_Cylinder_programID);

	// - Uniform paraméterek
	glUniform3fv(ul("color"), 1, glm::value_ptr(color));

	// view és projekciós mátrix
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));

	// Transzformációs mátrixok
	if(sphereExtend)
		matWorld *= glm::scale(glm::vec3(1.0,0.5,1.0));
	else
		matWorld *= glm::translate(glm::vec3(0, -0.5, 0));
	
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));

	// - Fényforrások beállítása
	glUniform3fv(ul("cameraPos"), 1, glm::value_ptr(m_camera.GetEye()));
	glUniform4fv(ul("lightPos"), 1, glm::value_ptr(m_lightPos));

	glUniform3fv(ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv(ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv(ul("Ls"), 1, glm::value_ptr(m_Ls));

	glUniform1f(ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f(ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f(ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);

	// - Anyagjellemzők beállítása
	glUniform3fv(ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv(ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv(ul("Ks"), 1, glm::value_ptr(m_Ks));

	glUniform1f(ul("Shininess"), m_Shininess);


	// - textúraegységek beállítása
	glUniform1i(ul("texImage"), 0);
	glUniform1i(ul("colorTexImage"), 1);

	// Rajzolási parancs kiadása
	glDrawElements(GL_TRIANGLES,
		m_surfaceGPU.count,
		GL_UNSIGNED_INT,
		nullptr);

	// shader kikapcsolasa
	glUseProgram(0);

	// - Textúrák kikapcsolása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CMyApp::RenderCircle(glm::mat4& matWorld, glm::vec3& color)
{
	// - VAO beállítása
	glBindVertexArray(m_surfaceGPU.vaoID);

	// - Textúrák beállítása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);

	glUseProgram(m_Circle_programID);

	// - Uniform paraméterek
	glUniform3fv(ul("color"), 1, glm::value_ptr(color));

	// view és projekciós mátrix
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));

	// Transzformációs mátrixok

	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));

	// - Fényforrások beállítása
	glUniform3fv(ul("cameraPos"), 1, glm::value_ptr(m_camera.GetEye()));
	glUniform4fv(ul("lightPos"), 1, glm::value_ptr(m_lightPos));

	glUniform3fv(ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv(ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv(ul("Ls"), 1, glm::value_ptr(m_Ls));

	glUniform1f(ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f(ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f(ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);

	// - Anyagjellemzők beállítása
	glUniform3fv(ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv(ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv(ul("Ks"), 1, glm::value_ptr(m_Ks));

	glUniform1f(ul("Shininess"), m_Shininess);


	// - textúraegységek beállítása
	glUniform1i(ul("texImage"), 0);
	glUniform1i(ul("colorTexImage"), 1);

	// Rajzolási parancs kiadása
	glDrawElements(GL_TRIANGLES,
		m_surfaceGPU.count,
		GL_UNSIGNED_INT,
		nullptr);

	// shader kikapcsolasa
	glUseProgram(0);

	// - Textúrák kikapcsolása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CMyApp::RenderSkyBox()
{
	// - VAO
	glBindVertexArray(m_SkyboxGPU.vaoID);

	// - Textura

	// - Program
	glUseProgram(m_programSkyboxID);

	// - uniform parameterek
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(glm::translate(m_camera.GetEye())));

	// - textúraegységek beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTextureID);
	glUniform1i(ul("skyboxTexture"), 0);

	// mentsük el az előző Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlőt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	// Rajzolási parancs kiadása
	glDrawElements(GL_TRIANGLES,
		m_SkyboxGPU.count,
		GL_UNSIGNED_INT,
		nullptr);

	glDepthFunc(prevDepthFnc);

	// shader kikapcsolasa
	glUseProgram(0);

	// - Textúrák kikapcsolása, minden egységre külön
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CMyApp::RenderGUI()
{
	// ImGui::ShowDemoWindow();
	if ( ImGui::Begin( "Lighting settings" ) )
	{
		//ImGui::SeparatorText("Settings");
		
		//ImGui::SliderFloat("Shininess", &m_Shininess, 0.0001f, 10.0f );
		//ImGui::InputFloat("Shininess", &m_Shininess, 0.1f, 1.0f, "%.1f" );
		/*static float Kaf = 1.0f;
		static float Kdf = 1.0f;
		static float Ksf = 0.0f;
		if ( ImGui::SliderFloat( "Ka", &Kaf, 0.0f, 1.0f ) )
		{
			m_Ka = glm::vec3( Kaf );
		}
		if ( ImGui::SliderFloat( "Kd", &Kdf, 0.0f, 1.0f ) )
		{
			m_Kd = glm::vec3( Kdf );
		}
		if ( ImGui::SliderFloat( "Ks", &Ksf, 0.0f, 1.0f ) )
		{
			m_Ks = glm::vec3( Ksf );
		}

		ImGui::SliderFloat( "Constant Attenuation", &m_lightConstantAttenuation, 0.001f, 2.0f );
		ImGui::SliderFloat( "Linear Attenuation", &m_lightLinearAttenuation, 0.001f, 2.0f );
		ImGui::SliderFloat( "Quadratic Attenuation", &m_lightQuadraticAttenuation, 0.001f, 2.0f );*/

		static glm::vec2 lightPosXZ = glm::vec2(0.0f);
		lightPosXZ = glm::vec2(m_lightPos.x, m_lightPos.z);
		if (ImGui::SliderFloat2("Light Position XZ", glm::value_ptr(lightPosXZ), -1.0f, 1.0f))
		{
			float lightPosL2 = lightPosXZ.x * lightPosXZ.x + lightPosXZ.y * lightPosXZ.y;
			if (lightPosL2 > 1.0f) // Ha kívülre esne a körön, akkor normalizáljuk
			{
				lightPosXZ /= sqrtf(lightPosL2);
				lightPosL2 = 1.0f;
			}

			m_lightPos.x = lightPosXZ.x;
			m_lightPos.z = lightPosXZ.y;
			m_lightPos.y = sqrtf(1.0f - lightPosL2);
			m_needFreshFboByLight = true;
		}
		ImGui::LabelText("Light Position Y", "%f", m_lightPos.y);

		ImGui::SliderFloat("Generation Time", &generationTime, 0.1f, 5.0f);

		ImGui::DragInt("GridSize", &m_gridSize, 1.0F, 5, 128);

		if (ImGui::Button("Reset"))
		{
			m_needNewSystem = true;
		}
	}

	ImGui::End();
}

GLint CMyApp::ul( const char* uniformName ) noexcept
{
	GLuint programID = 0;

	// Kérdezzük le az aktuális programot!
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
	glGetIntegerv( GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>( &programID ) );
	// A program és a uniform név ismeretében kérdezzük le a location-t!
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
	return glGetUniformLocation( programID, uniformName );
}

// https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
// https://wiki.libsdl.org/SDL2/SDL_Keysym
// https://wiki.libsdl.org/SDL2/SDL_Keycode
// https://wiki.libsdl.org/SDL2/SDL_Keymod

void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{	
	if ( key.repeat == 0 ) // Először lett megnyomva
	{
		if ( key.keysym.sym == SDLK_F5 && key.keysym.mod & KMOD_CTRL )
		{
			CleanShaders();
			InitShaders();
		}
		if ( key.keysym.sym == SDLK_F1 )
		{
			GLint polygonModeFrontAndBack[ 2 ] = {};
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
			glGetIntegerv( GL_POLYGON_MODE, polygonModeFrontAndBack ); // Kérdezzük le a jelenlegi polygon módot! Külön adja a front és back módokat.
			GLenum polygonMode = ( polygonModeFrontAndBack[ 0 ] != GL_FILL ? GL_FILL : GL_LINE ); // Váltogassuk FILL és LINE között!
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml
			glPolygonMode( GL_FRONT_AND_BACK, polygonMode ); // Állítsuk be az újat!
		}
	}
	m_cameraManipulator.KeyboardDown( key );
	m_needFreshFboByKey = true;
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp( key );
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove( mouse );
	m_needFreshFboByMouse = true;
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
	m_cameraManipulator.MouseWheel( wheel );
}


// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_camera.SetAspect( static_cast<float>(_w) / _h );


	// When we resize we need to remake the framebuffer with the new size,
	// because now we want the two framebuffer (default,ours) to be the same resolution
	CleanResolutionDependentResources();
	InitResolutionDependentResources(_w, _h);
}

// Le nem kezelt, egzotikus esemény kezelése
// https://wiki.libsdl.org/SDL2/SDL_Event

void CMyApp::OtherEvent( const SDL_Event& ev )
{

}