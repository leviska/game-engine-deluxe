#version 330 core
in vec2 TextCoords;
out vec4 color;

uniform sampler2DRect sprite;
uniform vec3 spriteColor;

void main()
{
    color = texture(sprite, TextCoords);
}
