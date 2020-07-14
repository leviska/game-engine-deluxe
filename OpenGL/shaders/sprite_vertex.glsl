#version 330 core
layout (location = 0) in vec2 Vertex;

out vec2 TextCoordsFrag;

uniform mat4 Projection;

uniform vec2 Position;
uniform vec4 TextCoords;
// x,y - scale; z = rotation
uniform vec3 Transform;

void main()
{
	TextCoordsFrag = TextCoords.xy + TextCoords.zw * Vertex;
	// rotate
	// then scale
	// then move
	float cs = cos(Transform.z);
	float sn = sin(Transform.z);
    gl_Position = Projection * vec4(
		(Vertex * mat2(cs, sn, -sn, cs))
		* Transform.xy
		+ Position, 
		0, 1.0);
}