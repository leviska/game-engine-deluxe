#version 330 core
in vec2 TextCoordsFrag;
out vec4 Result;

layout(binding=0) uniform sampler2DRect Texture;

void main()
{
	Result = texture(Texture, TextCoordsFrag);
}
