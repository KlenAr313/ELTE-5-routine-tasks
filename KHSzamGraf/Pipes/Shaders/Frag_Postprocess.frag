#version 430

// Per fragment variables coming from the pipeline
in vec2 vs_out_tex;

// Outgoing values - fragment color
out vec4 fs_out_col;

// External parameters of the shader
uniform sampler2D frameTex;

void main()
{
	fs_out_col = texture(frameTex, vs_out_tex);
}