#version 330 core
layout (location = 0) in vec2 Vertex;
layout (location = 1) in vec2 Position;
layout (location = 2) in vec4 TextCoords;
layout (location = 3) in vec4 Color;
// x,y - scale; z = rotation
layout (location = 4) in vec3 Transforms;

out vec2 TextCoordsFrag;
out vec4 ColorFrag;

uniform mat4 Projection;

void main()
{
	TextCoordsFrag = TextCoords.xy + TextCoords.zw * (Vertex + vec2(0.5, 0.5));
	ColorFrag = Color;
	// rotate
	// then scale
	// then move
	float cs = cos(Transforms.z);
	float sn = sin(Transforms.z);
    gl_Position = Projection * vec4(
		(Vertex * mat2(cs, sn, -sn, cs))
		* Transforms.xy
		+ Position, 
		0, 1.0);
}
