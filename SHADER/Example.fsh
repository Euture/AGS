#version 440 core

uniform	vec4 Offset;
uniform	vec4 Color;
uniform vec4 Color1;

in float gradient;

void main (void)
{
	gl_FragColor = vec4(Color) * gradient +  vec4(Color1) * (1.0 - gradient);
}
