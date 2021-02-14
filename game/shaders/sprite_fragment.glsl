#version 330 core
in vec2 TextCoordsFrag;
out vec4 Result;

uniform sampler2DRect Texture;
uniform vec4 Color;

void main()
{
    Result = Color * texture(Texture, TextCoordsFrag);
}
