#include "MyApp.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ObjParser.h"
#include "ParametricSurfaceMesh.hpp"
#include "ProgramBuilder.h"

#include <imgui.h>

#include <string>
#include <array>
#include <algorithm>

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
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void CMyApp::InitShaders()
{
	m_programID = glCreateProgram();
	ProgramBuilder{ m_programID }
		.ShaderStage( GL_VERTEX_SHADER, "Shaders/Vert_PosNormTex.vert" )
		.ShaderStage( GL_FRAGMENT_SHADER, "Shaders/Frag_Lighting.frag" )
		.Link();

}

void CMyApp::CleanShaders()
{
	glDeleteProgram( m_programID );
}


// Nyers parameterek
struct Param
{
	glm::vec3 GetPos( float u, float v ) const noexcept
	{
		/**/
		u *=  glm::two_pi<float>(); //fi
		v *= glm::pi<float>();  //theta

		float r = 1.0;
		return glm::vec3( r * sin(v)*cos(u), r*sin(v)*sin(u), r*cos(v));
	}
	glm::vec3 GetNorm( float u, float v ) const noexcept
	{
		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du, dv));
	}
	glm::vec2 GetTex( float u, float v ) const noexcept
	{
		return glm::vec2( u, v );
	}
};


struct Bezier
{

	std::array<std::array<glm::vec3, 3>, 3> b;

	Bezier(std::array<std::array<glm::vec3, 3>, 3> &b) {
		this->b = b;
	}

	glm::vec3 GetPos(float u, float v) const noexcept
	{
		float Bu[3] = { (1 - u) * (1 - u), 2.f * (1 - u) * u, u * u };
		float Bv[3] = { (1 - v) * (1 - v), 2.f * (1 - v) * v, v * v };
		glm::vec3 p = glm::vec3(0, 0, 0);

		for (int i = 0; i < 3; ++i) 
		{
			for (int j = 0; j < 3; ++j) 
			{
				p += b[i][j] * Bu[i] * Bv[j];
			}
		}

		return glm::vec3(p.x, p.y, p.z);
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

void CMyApp::InitGeometry()
{

	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof( Vertex, position ), 3, GL_FLOAT },
		{ 1, offsetof( Vertex, normal   ), 3, GL_FLOAT },
		{ 2, offsetof( Vertex, texcoord ), 2, GL_FLOAT },
	};

	// Suzanne
	std::array<std::array<glm::vec3, 3>, 3> b;
	b[0] = {glm::vec3(0,0,0),glm::vec3(1,0,0),glm::vec3(2,0,0)};
	b[1] = {glm::vec3(0,0,1),glm::vec3(1,2,1),glm::vec3(2,0,1)},
	b[2] ={glm::vec3(0,0,2),glm::vec3(1,-1,2),glm::vec3(2,0,2)};
	MeshObject<Vertex> SurfaceMeshCPU = GetParamSurfMesh( Bezier(b) );
	m_SurfaceGPU = CreateGLObjectFromMesh( SurfaceMeshCPU, vertexAttribList );
	
	std::array<std::array<glm::vec3, 3>, 3> c;
	c[0] = {glm::vec3(0,0,2),glm::vec3(1,-1,2),glm::vec3(2,0,2)};
	c[1] = {glm::vec3(0,0,3),glm::vec3(1,-3,3),glm::vec3(2,0,3)},
	c[2] ={glm::vec3(0,0,4),glm::vec3(1,0,4),glm::vec3(2,0,4)};
	MeshObject<Vertex> SurfaceMeshCPU2 = GetParamSurfMesh( Bezier(c) );
	m_SurfaceGPU2 = CreateGLObjectFromMesh( SurfaceMeshCPU2, vertexAttribList );
}

void CMyApp::CleanGeometry()
{
	CleanOGLObject( m_SurfaceGPU );
	CleanOGLObject( m_SurfaceGPU2);
}

void CMyApp::InitTextures()
{
	// sampler

	glCreateSamplers( 1, &m_SamplerID );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// diffuse texture

	ImageRGBA Image = ImageFromFile( "Assets/color_checkerboard.png" );

	glCreateTextures( GL_TEXTURE_2D, 1, &m_TextureID );
	glTextureStorage2D( m_TextureID, NumberOfMIPLevels( Image ), GL_RGBA8, Image.width, Image.height );
	glTextureSubImage2D( m_TextureID, 0, 0, 0, Image.width, Image.height, GL_RGBA, GL_UNSIGNED_BYTE, Image.data() );

	glGenerateTextureMipmap( m_TextureID );
}

void CMyApp::CleanTextures()
{
	glDeleteTextures( 1, &m_TextureID );
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glPointSize( 16.0f ); // nagyobb pontok
	glLineWidth( 4.0f ); // vastagabb vonalak

	InitShaders();
	InitGeometry();
	InitTextures();

	//
	// egyéb inicializálás
	//

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glCullFace(GL_BACK);    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	// kamera
	m_camera.SetView(
		glm::vec3(0.0, 7.0, 7.0),	// honnan nézzük a színteret	   - eye
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
}

void CMyApp::Update( const SUpdateInfo& updateInfo )
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;

	m_cameraManipulator.Update( updateInfo.DeltaTimeInSec );
	
	// kivetelesen a fényforrás a kamera pozíciója legyen, hogy mindig lássuk a feluletet,
	// es ne keljen allitgatni a fenyforrast
    //m_lightPos = glm::vec4( m_camera.GetEye(), 1.0 );
    m_lightPos = glm::vec4( 5,5,1, 1.0 );
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT)...
	// ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	// Suzanne
	//

    // - Uniform paraméterek
    // view és projekciós mátrix
    glProgramUniformMatrix4fv( m_programID, ul( m_programID, "viewProj" ), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );


    glm::mat4 matWorld = glm::translate( SUZANNE_POS );

    glProgramUniformMatrix4fv( m_programID, ul( m_programID, "world" ), 1, GL_FALSE, glm::value_ptr( matWorld ) );
    glProgramUniformMatrix4fv( m_programID, ul( m_programID, "worldIT" ), 1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

    // - Fényforrások beállítása
    glProgramUniform3fv( m_programID, ul( m_programID, "cameraPos" ), 1, glm::value_ptr( m_camera.GetEye() ) );
    glProgramUniform4fv( m_programID, ul( m_programID, "lightPos" ), 1, glm::value_ptr( m_lightPos ) );

    glProgramUniform3fv( m_programID, ul( m_programID, "La" ), 1, glm::value_ptr( m_La ) );
    glProgramUniform3fv( m_programID, ul( m_programID, "Ld" ), 1, glm::value_ptr( m_Ld ) );
    glProgramUniform3fv( m_programID, ul( m_programID, "Ls" ), 1, glm::value_ptr( m_Ls ) );

    glProgramUniform1f( m_programID, ul( m_programID, "lightConstantAttenuation" ), m_lightConstantAttenuation );
    glProgramUniform1f( m_programID, ul( m_programID, "lightLinearAttenuation" ), m_lightLinearAttenuation );
    glProgramUniform1f( m_programID, ul( m_programID, "lightQuadraticAttenuation" ), m_lightQuadraticAttenuation );

    // - Anyagjellemzők beállítása
    glProgramUniform3fv( m_programID, ul( m_programID, "Ka" ), 1, glm::value_ptr( m_Ka ) );
    glProgramUniform3fv( m_programID, ul( m_programID, "Kd" ), 1, glm::value_ptr( m_Kd ) );
    glProgramUniform3fv( m_programID, ul( m_programID, "Ks" ), 1, glm::value_ptr( m_Ks ) );

    glProgramUniform1f( m_programID, ul( m_programID, "Shininess" ), m_Shininess );

    // - textúraegységek beállítása
    glProgramUniform1i( m_programID, ul( m_programID, "texImage" ), 0 );

	// - Textúrák beállítása, minden egységre külön
	glBindTextureUnit( 0, m_TextureID );
	glBindSampler( 0, m_SamplerID );

    // - VAO
	glBindVertexArray( m_SurfaceGPU.vaoID );

    // - Program
	glUseProgram( m_programID );

	// Rajzolási parancs kiadása
	glDrawElements( GL_TRIANGLES,    
					m_SurfaceGPU.count,			 
					GL_UNSIGNED_INT,
					nullptr );

	// - VAO
	glBindVertexArray(m_SurfaceGPU2.vaoID);

	glDrawElements(GL_TRIANGLES,
		m_SurfaceGPU.count,
		GL_UNSIGNED_INT,
		nullptr);

	// shader kikapcsolasa
	glUseProgram( 0 );

	// - Textúrák kikapcsolása, minden egységre külön
	glBindTextureUnit( 0, 0 );
	glBindSampler( 0, 0 );

	// VAO kikapcsolása
	glBindVertexArray( 0 );
}

void CMyApp::RenderGUI()
{
	//ImGui::ShowDemoWindow();
	if ( ImGui::Begin( "Lighting settings" ) )
	{		
		ImGui::InputFloat("Shininess", &m_Shininess, 0.1f, 1.0f, "%.1f" );
		static float Kaf = 1.0f;
		static float Kdf = 1.0f;
		static float Ksf = 1.0f;
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

		{
			static glm::vec2 lightPosXZ = glm::vec2( 0.0f );
			lightPosXZ = glm::vec2( m_lightPos.x, m_lightPos.z );
			if ( ImGui::SliderFloat2( "Light Position XZ", glm::value_ptr( lightPosXZ ), -1.0f, 1.0f ) )
			{
				float lightPosL2 = lightPosXZ.x * lightPosXZ.x + lightPosXZ.y * lightPosXZ.y;
				if ( lightPosL2 > 1.0f ) // Ha kívülre esne a körön, akkor normalizáljuk
				{
					lightPosXZ /= sqrtf( lightPosL2 );
					lightPosL2 = 1.0f;
				}

				m_lightPos.x = lightPosXZ.x;
				m_lightPos.z = lightPosXZ.y;
				m_lightPos.y = sqrtf( 1.0f - lightPosL2 );
			}
			ImGui::LabelText( "Light Position Y", "%f", m_lightPos.y );
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
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp( key );
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove( mouse );
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
}

// Le nem kezelt, egzotikus esemény kezelése
// https://wiki.libsdl.org/SDL2/SDL_Event

void CMyApp::OtherEvent( const SDL_Event& ev )
{

}

