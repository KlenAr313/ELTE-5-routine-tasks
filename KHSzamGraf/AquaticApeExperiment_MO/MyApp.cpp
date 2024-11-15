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
		.ShaderStage( GL_FRAGMENT_SHADER, "Shaders/Frag_LightingNoFaceCull.frag" )
		.Link();

	m_programWaterID = glCreateProgram();
	ProgramBuilder{ m_programWaterID }
		.ShaderStage( GL_VERTEX_SHADER, "Shaders/Vert_Water.vert" )
		.ShaderStage( GL_FRAGMENT_SHADER, "Shaders/Frag_LightingNoFaceCull.frag" )
		.Link();

	m_programAxesID = glCreateProgram();
	ProgramBuilder{ m_programAxesID }
		.ShaderStage( GL_VERTEX_SHADER, "Shaders/Vert_axes.vert" )
		.ShaderStage( GL_FRAGMENT_SHADER, "Shaders/Frag_PosCol.frag" )
		.Link();

	m_programTrajectoryID = glCreateProgram();
	ProgramBuilder{ m_programTrajectoryID }
		.ShaderStage( GL_VERTEX_SHADER, "Shaders/Vert_traj.vert" )
		.ShaderStage( GL_FRAGMENT_SHADER, "Shaders/Frag_PosCol.frag" )
		.Link();


	InitSkyboxShaders();
}

void CMyApp::InitSkyboxShaders()
{
	m_programSkyboxID = glCreateProgram();
	ProgramBuilder{ m_programSkyboxID }
		.ShaderStage(GL_VERTEX_SHADER, "Shaders/Vert_skybox.vert")
		.ShaderStage(GL_FRAGMENT_SHADER, "Shaders/Frag_skybox.frag")
		.Link();
}

void CMyApp::CleanShaders()
{
	glDeleteProgram( m_programID );
	glDeleteProgram( m_programWaterID );
	glDeleteProgram( m_programAxesID );
	glDeleteProgram( m_programTrajectoryID );

	CleanSkyboxShaders();
}

void CMyApp::CleanSkyboxShaders()
{
	glDeleteProgram( m_programSkyboxID );
}

struct Param
{
	glm::vec3 GetPos(float u, float v) const noexcept
	{
        return glm::vec3(u, v, 0.0);
    }

	glm::vec3 GetNorm(float u, float v) const noexcept
	{
        return glm::vec3(0.0, 0.0, 1.0);
    }

	glm::vec2 GetTex( float u, float v ) const noexcept
	{
        return glm::vec2(u, v);
    }
};

struct Water
{
	glm::vec3 GetPos(float u, float v) const noexcept
	{
		glm::vec3 pos = glm::vec3(-10.0, 0.0, 10.0) + glm::vec3( 20.0, 0.0, -20.0) * glm::vec3(u, 0.0, v);
		pos.y = sinf(pos.z);

		return pos;
	}

	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du, dv));
	}

	glm::vec2 GetTex( float u, float v ) const noexcept
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

	MeshObject<Vertex> suzanneMeshCPU = ObjParser::parse("Assets/Suzanne.obj");

	m_SuzanneGPU = CreateGLObjectFromMesh( suzanneMeshCPU, vertexAttribList );

	// quad
	MeshObject<Vertex> quadMeshCPU;
    quadMeshCPU.vertexArray =
	{
        { glm::vec3(-1.0, -1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(0.0,0.0) }, // első lap
        { glm::vec3( 1.0, -1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(1.0,0.0) },
        { glm::vec3( 1.0,  1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(1.0,1.0) },
        { glm::vec3(-1.0,  1.0, 0.0), glm::vec3(0.0, 0.0,  1.0), glm::vec2(0.0,1.0) }
    };

    quadMeshCPU.indexArray =
	{
        0, 1, 2, // első lap
        2, 3, 0
    };
    
	m_quadGPU = CreateGLObjectFromMesh( quadMeshCPU, vertexAttribList );

	// Skybox
	InitSkyboxGeometry();

	// Water
	MeshObject<glm::vec2> waterCPU;
	{
		MeshObject<Vertex> surfaceMeshCPU = GetParamSurfMesh( Param(), 160, 80 );
		for ( const Vertex& v : surfaceMeshCPU.vertexArray )
		{
			waterCPU.vertexArray.emplace_back( glm::vec2( v.position.x, v.position.y ) );
		}
		waterCPU.indexArray = surfaceMeshCPU.indexArray;
	}
	m_waterGPU = CreateGLObjectFromMesh( waterCPU, { { 0, offsetof( glm::vec2,x), 2, GL_FLOAT}});
}

void CMyApp::CleanGeometry()
{
	CleanOGLObject( m_SuzanneGPU );
	CleanSkyboxGeometry();
}

void CMyApp::InitSkyboxGeometry()
{
	// skybox geo
	MeshObject<glm::vec3> skyboxCPU =
	{
		std::vector<glm::vec3>
		{
			// hátsó lap
			glm::vec3(-1, -1, -1),
			glm::vec3( 1, -1, -1),
			glm::vec3( 1,  1, -1),
			glm::vec3(-1,  1, -1),
			// elülső lap
			glm::vec3(-1, -1, 1),
			glm::vec3( 1, -1, 1),
			glm::vec3( 1,  1, 1),
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

	m_SkyboxGPU = CreateGLObjectFromMesh( skyboxCPU, { { 0, offsetof( glm::vec3,x ), 3, GL_FLOAT } } );
}

void CMyApp::CleanSkyboxGeometry()
{
	CleanOGLObject( m_SkyboxGPU );
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

	ImageRGBA SuzanneImage = ImageFromFile( "Assets/wood.jpg" );

	glCreateTextures( GL_TEXTURE_2D, 1, &m_SuzanneTextureID );
	glTextureStorage2D( m_SuzanneTextureID, NumberOfMIPLevels( SuzanneImage ), GL_RGBA8, SuzanneImage.width, SuzanneImage.height );
	glTextureSubImage2D( m_SuzanneTextureID, 0, 0, 0, SuzanneImage.width, SuzanneImage.height, GL_RGBA, GL_UNSIGNED_BYTE, SuzanneImage.data() );

	glGenerateTextureMipmap( m_SuzanneTextureID );


	ImageRGBA waterImage = ImageFromFile( "Assets/water_texture.jpg" );

	glCreateTextures( GL_TEXTURE_2D, 1, &m_waterTextureID );
	glTextureStorage2D( m_waterTextureID, NumberOfMIPLevels( waterImage ), GL_RGBA8, waterImage.width, waterImage.height );
	glTextureSubImage2D( m_waterTextureID, 0, 0, 0, waterImage.width, waterImage.height, GL_RGBA, GL_UNSIGNED_BYTE, waterImage.data() );

	glGenerateTextureMipmap( m_waterTextureID );


	ImageRGBA glassImage = ImageFromFile( "Assets/danger_glass.png" );

	glCreateTextures( GL_TEXTURE_2D, 1, &m_glassTextureID );
	glTextureStorage2D( m_glassTextureID, NumberOfMIPLevels( glassImage ), GL_RGBA8, glassImage.width, glassImage.height );
	glTextureSubImage2D( m_glassTextureID, 0, 0, 0, glassImage.width, glassImage.height, GL_RGBA, GL_UNSIGNED_BYTE, glassImage.data() );

	glGenerateTextureMipmap( m_glassTextureID );


	InitSkyboxTextures();
}

void CMyApp::CleanTextures()
{
	glDeleteTextures( 1, &m_SuzanneTextureID );
	glDeleteTextures( 1, &m_waterTextureID );
	glDeleteTextures( 1, &m_glassTextureID );

	CleanSkyboxTextures();
}

void CMyApp::InitSkyboxTextures()
{
	// skybox texture
	static const char* skyboxFiles[6] = {
		"Assets/lab_xpos.png",
		"Assets/lab_xneg.png",
		"Assets/lab_ypos.png",
		"Assets/lab_yneg.png",
		"Assets/lab_zpos.png",
		"Assets/lab_zneg.png",
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
	glDeleteTextures( 1, &m_SkyboxTextureID );
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

	// kontrolpontok
	m_controlPoints.push_back(glm::vec3(-1.0f, 0.0, -1.0f));
	m_controlPoints.push_back(glm::vec3( 1.0f, 0.0,  1.0f));

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
}

void CMyApp::SetLightingUniforms( GLuint program, float Shininess, glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks )
{
	// - Fényforrások beállítása
	glProgramUniform3fv( program, ul( program, "cameraPos" ), 1, glm::value_ptr( m_camera.GetEye() ) );
	glProgramUniform4fv( program, ul( program, "lightPos" ),  1, glm::value_ptr( m_lightPos ) );

	glProgramUniform3fv( program, ul( program, "La" ),		 1, glm::value_ptr( m_La ) );
	glProgramUniform3fv( program, ul( program, "Ld" ),		 1, glm::value_ptr( m_Ld ) );
	glProgramUniform3fv( program, ul( program, "Ls" ),		 1, glm::value_ptr( m_Ls ) );

	glProgramUniform1f( program, ul( program, "lightConstantAttenuation"	 ), m_lightConstantAttenuation );
	glProgramUniform1f( program, ul( program, "lightLinearAttenuation"	 ), m_lightLinearAttenuation   );
	glProgramUniform1f( program, ul( program, "lightQuadraticAttenuation" ), m_lightQuadraticAttenuation);

	// - Anyagjellemzők beállítása
	glProgramUniform3fv( program, ul( program, "Ka" ),		 1, glm::value_ptr( Ka ) );
	glProgramUniform3fv( program, ul( program, "Kd" ),		 1, glm::value_ptr( Kd ) );
	glProgramUniform3fv( program, ul( program, "Ks" ),		 1, glm::value_ptr( Ks ) );

	glProgramUniform1f( program, ul( program, "Shininess" ),	Shininess );
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
	glProgramUniformMatrix4fv( m_programID, ul( m_programID,"viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	// Transformációs mátrixok
	glm::vec3 suzanneForward = EvaluatePathTangent(); // Merre nézzen a Suzanne?
	glm::vec3 suzanneWorldUp = glm::vec3(0.0, 1.0, 0.0); // Milyen irány a felfelé?
	if ( fabsf( suzanneForward.y ) > 0.99 ) // Ha a Suzanne felfelé néz, akkor a worldUp irányt nem tudjuk használni, mert akkor a jobbra vektor null vektor lesz
	{
		suzanneWorldUp = glm::vec3( -1.0, 0.0, 0.0); // Ezért ha felfelé néz, akkor a worldUp legyen egy tetszőleges [0,1,0] vektorra merőleges irány
	}
	glm::vec3 suzanneRight = glm::normalize(glm::cross(suzanneForward, suzanneWorldUp)); // Jobbra nézése
	glm::vec3 suzanneUp = glm::cross(suzanneRight, suzanneForward); // Felfelé nézése

	// A három vektorból álló bázisvektorokat egy mátrixba rendezzük, hogy tudjuk velük forgatni a Suzanne-t
	glm::mat4 suzanneRot(1.0f);
	suzanneRot[0] = glm::vec4(suzanneForward, 0.0f);
	suzanneRot[1] = glm::vec4(     suzanneUp, 0.0f);
	suzanneRot[2] = glm::vec4(  suzanneRight, 0.0f);

	// A Suzanne alapállásban a Z tengelyre néz, de nekünk az X tengelyre kell, ezért elforgatjuk
	static const glm::mat4 suzanneTowardX = glm::rotate(glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 matWorld = glm::translate(EvaluatePathPosition()) * suzanneRot * suzanneTowardX;

	glProgramUniformMatrix4fv( m_programID, ul( m_programID,"world" ),    1, GL_FALSE, glm::value_ptr( matWorld ) );
	glProgramUniformMatrix4fv( m_programID, ul( m_programID,"worldIT" ),  1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

	SetLightingUniforms( m_programID, m_Shininess, m_Ka, m_Kd, m_Ks );

	// - textúraegységek beállítása
	glProgramUniform1i( m_programID, ul( m_programID,"texImage" ), 0 );

	// - Textúrák beállítása, minden egységre külön
	glBindTextureUnit( 0, m_SuzanneTextureID );
	glBindSampler( 0, m_SamplerID );

    // - VAO
	glBindVertexArray( m_SuzanneGPU.vaoID );

    // - Program
	glUseProgram( m_programID );

	// Rajzolási parancs kiadása
	glDrawElements( GL_TRIANGLES,    
					m_SuzanneGPU.count,			 
					GL_UNSIGNED_INT,
					nullptr );

	//
	// Viz
	//

	// Mivel másik shader-t használunk, ezért újra be kell állítani a uniform paramétereket
	
	glProgramUniformMatrix4fv( m_programWaterID, ul( m_programWaterID,"viewProj" ), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );
	
	matWorld = glm::translate(glm::vec3(0.0,-2.0,0.0)); // toljuk lejjebb a vizet
	glProgramUniformMatrix4fv( m_programWaterID, ul( m_programWaterID,"world" ),    1, GL_FALSE, glm::value_ptr( matWorld ) );
	glProgramUniformMatrix4fv( m_programWaterID, ul( m_programWaterID,"worldIT" ),  1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

	SetLightingUniforms( m_programWaterID, m_Shininess, m_Ka, m_Kd, m_Ks );

	glProgramUniform1f( m_programWaterID, ul( m_programWaterID,"ElapsedTimeInSec" ),	m_ElapsedTimeInSec );

	// - Textúrák beállítása, minden egységre külön
	glBindTextureUnit( 0, m_waterTextureID );
	glBindSampler( 0, m_SamplerID );

	glBindVertexArray( m_waterGPU.vaoID );

	glUseProgram( m_programWaterID );


	glDrawElements( GL_TRIANGLES,    
					m_waterGPU.count,			 
					GL_UNSIGNED_INT,
					nullptr );
	//
	// Tengelyek
	//

	glBindVertexArray( 0 );

	glProgramUniformMatrix4fv( m_programAxesID, ul(m_programAxesID, "world"),    1, GL_FALSE, glm::value_ptr( glm::identity<glm::mat4>() ) );
	glProgramUniformMatrix4fv( m_programAxesID, ul(m_programAxesID, "viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	glUseProgram( m_programAxesID );

	glDrawArrays( GL_LINES, 0, 6 );

	// Trajektória
	glProgramUniformMatrix4fv( m_programTrajectoryID, ul( m_programTrajectoryID,"viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );
	glProgramUniformMatrix4fv( m_programTrajectoryID, ul( m_programTrajectoryID,"world"),    1, GL_FALSE, glm::value_ptr( glm::identity<glm::mat4>() ) );
	GLsizei controlPointCount = static_cast<GLsizei>(m_controlPoints.size());
	glProgramUniform3fv( m_programTrajectoryID, ul( m_programTrajectoryID,"positions"), controlPointCount, glm::value_ptr(m_controlPoints[0]));
	glProgramUniform3fv( m_programTrajectoryID, ul( m_programTrajectoryID,"color"), 1, glm::value_ptr( glm::vec3(1.0, 0.0, 0.0) ) );
	
	glUseProgram( m_programTrajectoryID );
	glDrawArrays( GL_LINE_STRIP, 0, controlPointCount );
	
	glProgramUniform3fv( m_programTrajectoryID, ul( m_programTrajectoryID,"color"), 1, glm::value_ptr( glm::vec3(1.0, 0.0, 1.0) ) );
	glDrawArrays( GL_POINTS, 0, controlPointCount );

	//
	// skybox
	//

	// mentsük el az előző Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlőt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	// - uniform parameterek
	glProgramUniformMatrix4fv( m_programSkyboxID, ul( m_programSkyboxID,"viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );
	glProgramUniformMatrix4fv( m_programSkyboxID, ul( m_programSkyboxID,"world"),    1, GL_FALSE, glm::value_ptr( glm::translate( m_camera.GetEye() ) ) );

	// - textúraegységek beállítása
	glProgramUniform1i( m_programSkyboxID, ul( m_programSkyboxID,"skyboxTexture" ), 2 );


	// - Textura
	glBindTextureUnit( 2, m_SkyboxTextureID );
	glBindSampler( 2, m_SamplerID );

	// - VAO
	glBindVertexArray( m_SkyboxGPU.vaoID );

	// - Program
	glUseProgram( m_programSkyboxID );

	// - Rajzolas
	glDrawElements( GL_TRIANGLES, m_SkyboxGPU.count, GL_UNSIGNED_INT, nullptr );

	glDepthFunc(prevDepthFnc);

	//
	// vedő üveg
	//

	glDisable(GL_CULL_FACE ); // kapcsoljuk ki a hátrafelé néző lapok eldobását
	glEnable(GL_BLEND); // átlátszóság engedélyezése
	// meghatározza, hogy az átlátszó objektum az adott pixelben hogyan módosítsa a korábbi fragmentekből oda lerakott színt: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	// - Textúrák beállítása, minden egységre külön
	glBindTextureUnit( 0, m_glassTextureID );
	glBindSampler( 0, m_SamplerID );

	glBindVertexArray( m_quadGPU.vaoID );

	glUseProgram( m_programID );

	glProgramUniformMatrix4fv( m_programID, ul( m_programID, "viewProj" ), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	std::sort(GLASS_TRANSFORMS.begin(), GLASS_TRANSFORMS.end(), [&](glm::vec4 a, glm::vec4 b) -> bool {
		glm::vec3 cam_pos = m_camera.GetEye();
		float da = glm::distance(cam_pos, glm::vec3(a.x, a.y, a.z));
		float db = glm::distance(cam_pos, glm::vec3(b.x, b.y, b.z));
		return da < db;
	});

	for (int i = 3; i >= 0; i--)
	{

		//matWorld = glm::translate( GLASS_POSITION ) * glm::scale( GLASS_SCALE );
		glm::vec4 d = GLASS_TRANSFORMS[i];
		glm::vec3 pos = glm::vec3(d.x, d.y, d.z);
		matWorld = glm::translate(pos) * glm::rotate(d.w, glm::vec3(0, 1, 0)) * glm::scale(GLASS_SCALE);
		glProgramUniformMatrix4fv( m_programID, ul( m_programID, "world" ), 1, GL_FALSE, glm::value_ptr( matWorld ) );
		glProgramUniformMatrix4fv( m_programID, ul( m_programID, "worldIT" ), 1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

		glDrawElements( GL_TRIANGLES,
						m_quadGPU.count,
						GL_UNSIGNED_INT,
						nullptr );
	}
	
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE ); // kapcsoljuk vissza a hátrafelé néző lapok eldobását

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

	if ( ImGui::Begin( "Experiment" ) )
	{
		// A paramétert szabályozó csúszka
		ImGui::SliderFloat("Parameter", &m_currentParam, 0, (float)(m_controlPoints.size() - 1));

		ImGui::SeparatorText("Control points");

		// A kijelölt pont indexe
		// Lehetne a CMyApp tagváltozója is, de mivel csak a GUI-hoz kell, ezért elégséges lokális, de statikus változónak lennie
		static int currentItem = -1;

		// A listboxban megjelenítjük a pontokat
		// Legyen a magasssága annyi, hogy MAX_POINT_COUNT elem férjen bele
		// ImGui::GetTextLineHeightWithSpacing segítségével lekérhető egy sor magassága
		if (ImGui::BeginListBox("Control Points", ImVec2(0.0, MAX_POINT_COUNT * ImGui::GetTextLineHeightWithSpacing())))
		{
			for ( int i = 0; i < static_cast<const int>( m_controlPoints.size() ); ++i )
			{
				const bool is_seleceted = ( currentItem == i ); // épp ki van-e jelölve?
				if ( ImGui::Selectable( std::to_string( i ).c_str(), is_seleceted ) )
				{
					if ( i == currentItem ) currentItem = -1; // Ha rákattintottunk, akkor szedjük le a kijelölést
					else currentItem = i; // Különben jelöljük ki
				}

				// technikai apróság, nem baj ha lemarad.
				if ( is_seleceted )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox(); 
		}

		// Gombnyomásra új pontot adunk a végére
		if (ImGui::Button("Add")) // Akkor tér vissza true-val, ha rákattintottunk
		{
			if ( m_controlPoints.size() < MAX_POINT_COUNT )
			{
				m_controlPoints.push_back( glm::vec3( 0.0f ) );
				currentItem = static_cast<const int>( m_controlPoints.size() - 1 ); // Az új pontot állítjuk be aktuálisnak
			}
		}

		ImGui::SameLine();

		// Gombnyomásra töröljük a kijelölt pontot
		if (ImGui::Button("Delete") )
		{
			if ( !m_controlPoints.empty() && currentItem < m_controlPoints.size() && currentItem != -1 ) // currentItem valid index?
			{
				m_controlPoints.erase( m_controlPoints.begin() + currentItem ); // Iterátoron keresztül tudjuk törölni a kijelölt elemet
				currentItem = -1; // Törölve lett a kijelölés
			}
		}

		// Ha van kijelölt elem, akkor jelenítsük meg a koordinátáit
		// és lehessen szerkeszteni
		if ( currentItem < m_controlPoints.size() && currentItem != -1 ) // currentItem valid index?
		{
			ImGui::SliderFloat3("Coordinates", glm::value_ptr(m_controlPoints[currentItem]), -10, 10);
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

// Pozíció kiszámítása a kontrollpontok alapján
glm::vec3 CMyApp::EvaluatePathPosition() const
{
	if (m_controlPoints.size() == 0) // Ha nincs pont, akkor visszaadjuk az origót
		return glm::vec3(0);

	const int interval = (const int)m_currentParam; // Melyik két pont között vagyunk?

	if (interval < 0) // Ha a paraméter negatív, akkor a kezdőpontot adjuk vissza
		return m_controlPoints[0];

	if (interval >= m_controlPoints.size() - 1) // Ha a paraméter nagyobb, mint a pontok száma, akkor az utolsó pontot adjuk vissza
		return m_controlPoints[m_controlPoints.size() - 1];

	float localT = m_currentParam - interval; // A paramétert normalizáljuk az aktuális intervallumra
	
	return glm::mix( m_controlPoints[interval], m_controlPoints[interval + 1], localT ); // Lineárisan interpolálunk a két kontrollpont között
}

// Tangens kiszámítása a kontrollpontok alapján
glm::vec3 CMyApp::EvaluatePathTangent() const
{
	if (m_controlPoints.size() < 2) // Ha nincs elég pont az interpolációhoy, akkor visszaadjuk az x tengelyt
		return glm::vec3(1.0,0.0,0.0);

	int interval = (int)m_currentParam; // Melyik két pont között vagyunk?

	if (interval < 0) // Ha a paraméter negatív, akkor a kezdő intervallumot adjuk vissza
		interval = 0;

	if (interval >= m_controlPoints.size() - 1) // Ha a paraméter nagyobb, mint az intervallumok száma, akkor az utolsót adjuk vissza
		interval = static_cast<int>( m_controlPoints.size() - 2 );

	return glm::normalize(m_controlPoints[interval + 1] - m_controlPoints[interval]);
}
