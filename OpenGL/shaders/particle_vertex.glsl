#version 330 core
layout (location = 0) in vec4 Parameters;

out vec4 ColorFrag;

uniform mat4 Projection;
uniform vec2 Position;
uniform float Time;

const float LH = 10;

float LowCut(float x) {
	return x * x / 10 - LH;
}

float HighCut(float x) {
	return -x * x + LH;
}

float Clump(float x) {
	return -pow(x * 2 - 1, 6) + 1;
}

const vec3 ColA = vec3(139, 109, 156) / 255;
const vec3 ColB = vec3(242, 211, 171) / 255;

float GetY(float x) {
	float l = LowCut(x);
	float h = HighCut(x);
	return mod(Time - l + Parameters.y * 10, h - l) + l;
}

void main()
{
	float x = Parameters.x;
	float l = LowCut(x);
	float h = HighCut(x);
	float y = GetY(x);
	x = Parameters.y * sin(Parameters.z * y) + Parameters.w;
	vec2 pos = vec2(x, -y) * 8 + Position;
	pos = pos - mod(pos, 8);
	float frac = (y - LowCut(x)) / (HighCut(x) - LowCut(x));
	float frac2 = (y - l) / (h - l);
	vec3 col = ColA;
	if (Parameters.z >= 0.5) col = ColB;
	ColorFrag = vec4(col, Clump(frac) * Clump(frac2) * 0.7);
    gl_Position = Projection * vec4(pos, 0, 1.0);
}
