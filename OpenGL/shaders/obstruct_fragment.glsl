#version 330 core
in vec2 TextCoordsFrag;
in vec4 ColorFrag;
out vec4 Result;

uniform sampler2DRect Texture;

void main()
{
	if (texture(Texture, TextCoordsFrag).w < 0.01) {
		Result = vec4(0.0, 0.0, 0.0, 0.0);
	}
	else {
		Result = ColorFrag;
	}
}
