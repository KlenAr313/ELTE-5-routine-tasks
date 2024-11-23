#version 430

// Per fragment variables coming from the pipeline
in vec2 vs_out_tex;

// Outgoing values - fragment color
out vec4 fs_out_col;

// External parameters of the shader
uniform sampler2D frameTex;
uniform float filter_weight;
uniform mat3 color_matrix = mat3( 0.393f, 0.349f, 0.272f, 
								  0.769f, 0.686f, 0.534f,
								  0.189f, 0.168f, 0.131f );

uniform vec2 res = vec2(800,600);
uniform int k = 5;

void main()
{
	//vec4 originalColor = texture(frameTex, vs_out_tex);
	//originalColor += texture(frameTex, vs_out_tex+vec2(0.1,0));

	vec3 col = vec3(0);
	for(int i=-k; i<=k;++i)
	for(int j=-k; j<=k;++j)
	{
		col+=texture(frameTex,vs_out_tex+vec2(i,j)/res).rgb;
	}
	col /= float((2*k+1)*(2*k+1));

	vec4 originalColor = vec4(col,1);

	vec4 sepiaColored = vec4(
								color_matrix*originalColor.xyz, 
								1
							);
	fs_out_col = mix(originalColor, sepiaColored, filter_weight);
}