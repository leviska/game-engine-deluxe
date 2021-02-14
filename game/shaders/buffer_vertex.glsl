#version 330 core
layout (location = 0) in vec2 Vertex;

out vec2 TextCoordsFrag;

uniform mat4 Projection;

uniform vec2 Position;
uniform vec3 Transform;

void main()
{
	TextCoordsFrag = abs(Transform.xy) * (Vertex + vec2(0.5, 0.5));
    gl_Position = Projection * vec4(Vertex * Transform.xy * Transform.z + Position, 0, 1.0);
}