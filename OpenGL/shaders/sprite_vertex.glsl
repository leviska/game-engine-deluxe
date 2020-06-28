#version 330 core
layout (location = 0) in vec2 position;

out vec2 TextCoords;

uniform vec4 textCoords;
uniform mat4 model;
uniform mat4 projection;

void main()
{
	TextCoords = textCoords.xy + textCoords.zw * position;
    gl_Position = projection * model * vec4(position.xy, 0, 1.0);
}