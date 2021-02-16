#version 330 core
in vec2 TextCoordsFrag;
in vec4 ColorFrag;
out vec4 Result;

uniform sampler2DRect Texture;

void main()
{
    Result = ColorFrag; //* texture(Texture, TextCoordsFrag);
}
