#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 textCoords;
layout (location = 2) in mat4 model;

out vec2 TextCoords;

uniform mat4 projection;

void main()
{
	TextCoords = textCoords.xy + textCoords.zw * position;
    gl_Position = projection * model * vec4(position.xy, 0, 1.0);
}
