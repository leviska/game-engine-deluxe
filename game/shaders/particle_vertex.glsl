#version 330 core
layout (location = 0) in vec4 Parameters;

out vec4 ColorFrag;

uniform mat4 Projection;
uniform vec4 Transform;
uniform float Time;
uniform float MaxX;
uniform int CoordMod = 1;

float CutDistance() {
	return 0.6 * MaxX * MaxX;
}

float LowCut(float x) {
	return x * x * 0.1 - CutDistance();
}

float HighCut(float x) {
	return -x * x + CutDistance();
}

float Clump(float x) {
	return -pow(x * 2 - 1, 6) + 1;
}

float Interval(float l, float x, float h) {
	return mod(x - l, h - l) + l;
}

const vec3 ColA = vec3(139, 109, 156) / 255;
const vec3 ColB = vec3(242, 211, 171) / 255;

void main()
{
	float x = Parameters.x;
	float l = LowCut(x);
	float h = HighCut(x);
	float tl = -log(exp(-l)+exp(-h));
	float th = log(exp(l)+exp(h));
	l = tl;
	h = th;
	float y = Interval(l, Time + Parameters.w * 10, h);
	float frac = (y - l) / (h - l);
	x = x + Parameters.y * sin(y + Parameters.z);
	vec2 pos = vec2(x, -y) * Transform.zw;
	pos -= mod(pos, CoordMod);
	pos += Transform.xy;
	vec3 color = ColA;
	if (Parameters.z >= 0) {
		color = ColB;
	}
	ColorFrag = vec4(color, Clump(frac) * 0.5);
	gl_Position = Projection * vec4(pos, 0, 1.0);
}
