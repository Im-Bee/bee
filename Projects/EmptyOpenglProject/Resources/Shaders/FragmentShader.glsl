#version 420 core

#ifdef GL_ES
precision mediump float;
#endif

uniform float iTime;
uniform vec2 iResolution;
in  vec2 gl_FragCoord;
out vec4 fragColor;

#define R iResolution.xy

#define N 1024
#define B 32
#define S 8

float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12519.0,78345.2))) * 43758.5);
}

vec2 random2(in vec2 st) {
    return vec2(random(st), random(st));    
}

vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d ) {
    return a + b*cos(6.28318 * (c*t + d));
}

float iterate(vec2 p) {

    vec2 z = vec2(0), c = p;
    float i;

    for(i=0.; i < N; i++) {
        z = abs(z);
        z = mat2(z, -z.y, z.x) * z + c;
        if(dot(z, z) > B*B) break;
    }

    return i - log(log(dot(z, z)) / log(B)) / log(2.);;     
}

void main() 
{

    vec3 col = vec3(0);
    for(float i=0.; i < S; i++) {

        vec2 uv = (.09 + sin(iTime / 46) - cos(iTime / 24)) * (1. * gl_FragCoord + random2(R+i) - R) / R.y 
            - vec2(1.50, -.4);

        uv.y = -uv.y;

        float sn = iterate(uv) / N;   

        col += pal(fract(( iTime / 0.84 )*sn + 0.5), 
            vec3(.731, -1.098, .192),
            vec3(.608, -.512, .657), 
            vec3(-.542, -1.458, .328),
            vec3(-0.389, 2.365, 0.837));
    }

    fragColor = vec4(col / S, 1.0);
}
