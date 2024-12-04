#version 420 core

#ifdef GL_ES
precision mediump float;
#endif

uniform float iTime;
uniform vec2 iResolution;
in  vec2 gl_FragCoord;
out vec4 fragColor;

#define R iResolution.xy
void main() 
{
    vec2 uv = ( 2.* gl_FragCoord - R ) / R.y, z = 0./R; 
	
    float i = 0.;
    for(; ++i <= 64. && dot(z,z) < 4. ;)
        z = mat2( z, -z.y, z.x ) * z + uv;

    fragColor = vec4(vec3( i / 64. ), 1.0);
}

