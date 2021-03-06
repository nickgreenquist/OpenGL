#version 330

layout (location = 0) in vec2 position;
uniform mat4 worldMatrix;

void main()
{
	gl_Position = worldMatrix * vec4(position,0,1);
}