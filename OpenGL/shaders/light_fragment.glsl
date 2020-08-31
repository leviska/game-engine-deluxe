#version 430 core
in vec2 TextCoordsFrag;
out vec4 Result;

layout (binding = 0) uniform sampler2DRect Texture;
layout (binding = 1, r32f ) uniform restrict readonly image2D Lighting;

uniform int LightsSize = 0;
uniform ivec2 Lights[16];

const ivec2 shift[8] = { 
	ivec2(0, 1), ivec2(0, -1), ivec2(-1, 0), ivec2(1, 0), 
	ivec2(1, 1), ivec2(-1, 1), ivec2(-1, -1), ivec2(1, -1)
};

const int MAX = 20;

float customClamp(float x, float l, float h) {
	if (x <= l) 
		return 0;
	if (x >= h)
		return 255;
	return (x-l) * 255 / (h-l);
}

bool check(vec2 pos) {
	return pos.x >= 0 && pos.y >= 0 && pos.x < 384 && pos.y < 216;
}

void main()
{
	float obst = texture(Texture, TextCoordsFrag).a;
	float value = imageLoad(Lighting, ivec2(TextCoordsFrag)).x;
	float alpha = mod(abs(value) - 1, 1000);
	int nearest = int(round((abs(value) - 1) / 1000)) - 1;
	float len = length(Lights[nearest] - TextCoordsFrag);
	if (len < 150) {
		len = 150 - len;
		if (len - alpha < 5) {
			alpha -= 5 - (len - alpha);
		}
	}
	alpha /= 150;
	alpha = clamp(alpha, 0, 1);
	//alpha = smoothstep(0, 1, alpha);
	const float t = 80;
	const int tt = 4;
	alpha = customClamp(alpha*255, t, 255-t)/255;
	alpha = round(alpha * tt) / tt;
	Result = vec4(vec3(texture(Texture, TextCoordsFrag).xyz), alpha);	
}