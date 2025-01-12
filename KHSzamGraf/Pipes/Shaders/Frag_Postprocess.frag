#version 430
#define PI 3.1415926538

// Per fragment variables coming from the pipeline
in vec2 vs_out_tex;

// Outgoing values - fragment color
out vec4 fs_out_col;

// External parameters of the shader
uniform sampler2D frameTex;
uniform sampler2D depthTex;

uniform vec2 res = vec2(800,600);

void main()
{
	//fs_out_col = texture(frameTex, vs_out_tex);

    float depth = texture(depthTex, vs_out_tex).r;
    int k = 1;
    if(depth < 4)
    {
        k = int( floor( 4.0 - ((depth / 2.0) * (depth / 2.0))));
    }
    else
    {
        k = int(8.0 * atan((depth-4.0)/5.0) / PI);
    }
        
	vec3 col = vec3(0);
 	for(int i=-k; i<=k;++i)
 	for(int j=-k; j<=k;++j)
 	{
 		col+=texture(frameTex,vs_out_tex+vec2(i,j)/res).rgb;
 	}
 	col /= float((2*k+1)*(2*k+1));
 
 	vec4 originalColor = vec4(col,1);

 	fs_out_col = originalColor;
}