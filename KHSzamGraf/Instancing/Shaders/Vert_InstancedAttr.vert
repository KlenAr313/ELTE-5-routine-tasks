#version 430

// Variables coming from the VBO
layout( location = 0 ) in vec3 vs_in_pos;
layout( location = 1 ) in vec3 vs_in_norm;
layout( location = 2 ) in vec2 vs_in_tex;
layout( location = 3 ) in mat4 vs_in_world;
layout( location = 7 ) in mat4 vs_in_worldIT;

// Values to be passed on the pipeline
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

uniform mat4 viewProj;

void main()
{
	gl_Position = viewProj * vs_in_world * vec4( vs_in_pos, 1 );

	vs_out_pos  = (vs_in_world   * vec4(vs_in_pos,  1)).xyz;
	vs_out_norm = (vs_in_worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}