#version 430

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
// textúra mintavételező objektum
uniform sampler2D texImage;

// multiple outputs are directed into different color textures by the FBO
layout(location=0) out vec4 fs_out_diffuse;
layout(location=1) out vec4 fs_out_normal;
layout(location=2) out vec4 fs_out_position;

void main()
{
	// A fragment normálvektora
	// MINDIG normalizáljuk!
	vec3 normal = normalize( vs_out_norm );
	
	fs_out_diffuse = vec4(texture(texImage, vs_out_tex).xyz, 1);
	fs_out_position = vec4(vs_out_pos,1);
	fs_out_normal = vec4(normal,1.0);
}