#version 430

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_color;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

uniform float mult = 1.0;

void main()
{
	fs_out_col = vec4( mult * vs_out_color, 1.0 );
}
