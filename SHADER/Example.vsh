#version 440 core

uniform mat4 ProjectionMatrix; 
uniform mat4 ModelViewMatrix; 
uniform vec4 Color;  
in vec3 vPosition;  
void main () 
{  
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (vPosition, 1); 
}