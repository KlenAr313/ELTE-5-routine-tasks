#version 430

// VBO-ból érkező változók
layout( location = 0 ) in vec3 vs_in_pos;
//layout( location = 1 ) in vec3 vs_in_norm;
layout( location = 1 ) in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 viewProj;

//uniform sampler2D texImage;
uniform sampler2D colorTexImage;

float r = 0.3;

// CIRCLE
void main()
{
	float u = vs_in_pos.x * 3.1415 * 2;
	float v = vs_in_pos.y;

	vs_out_pos = vec3( 
			r * cos(u) * v,
			0.5,
			r * sin(u) * v);

	vs_out_tex = vs_in_tex;

	vs_out_norm = vec3(.0,1.0,.0);

	gl_Position = viewProj * world * vec4( vs_out_pos, 1 );
	vs_out_pos  = (world   * vec4(vs_out_pos,  1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_out_norm, 0)).xyz;

}