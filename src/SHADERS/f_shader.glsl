#version 330 core

out vec4 fColor;
//in vec2 TexCoord;

uniform vec4 newColor;
//uniform sampler2D texture1;

void main()
{
	//fColor = texture(texture1, TexCoord);
	fColor = newColor;
}

