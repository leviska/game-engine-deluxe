#version 330 core
layout (location = 0) in vec2 Vertex;
layout (location = 1) in vec2 Position;
layout (location = 2) in float Layer;
layout (location = 3) in vec2 TextPos;
layout (location = 4) in vec2 TextSize;
layout (location = 5) in vec2 Scale;
layout (location = 6) in vec4 Color;
layout (location = 7) in float Rotation;

out vec2 TextCoordsFrag;
out vec4 ColorFrag;

uniform mat4 Projection;

void main()
{
	TextCoordsFrag = TextPos + TextSize * (Vertex + vec2(0.5, 0.5));
	ColorFrag = Color;
	// rotate
	// then scale
	// then move
	float cs = cos(Rotation);
	float sn = sin(Rotation);
    gl_Position = Projection * vec4(
		(Vertex * mat2(cs, sn, -sn, cs))
		* Scale * TextSize
		+ Position, 
		Layer, 1.0);
}
