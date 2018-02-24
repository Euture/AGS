#version 440 core

uniform	vec4 Offset;
uniform	vec4 Color;

in vec2 vPosition;

out float gradient;

void main ()
{
	gradient = vPosition.x + 0.5;
	gl_Position = vec4 (vPosition, 0, 1) + Offset;
}
