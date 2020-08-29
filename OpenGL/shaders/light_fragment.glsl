#version 330 core
#extension GL_ARB_shader_image_load_store: require
#extension GL_ARB_shading_language_420pack: require
in vec2 TextCoordsFrag;
out vec4 Result;

layout (binding = 0) uniform sampler2DRect Texture;
layout (binding = 1, r32f ) uniform restrict readonly image2D Lighting;

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
	float alpha = imageLoad(Lighting, ivec2(TextCoordsFrag)).x;
	if (alpha >= 1000) alpha -= 1000;
	alpha /= 150;
	if (obst < 1) {
		int cnt = 1;
		for (int i = 0; i < 8; i++) {
			vec2 cur = TextCoordsFrag;
			for (int j = 0; j < 10; j++) {
				cur += shift[i];
				float a = imageLoad(Lighting, ivec2(cur)).x / 150;
				float r = texture(Texture, cur).a;
				if (check(cur) && r < 1) {
					cnt++;
					alpha += a;
				}
				else {
					break;
				}
			}
		}
		alpha /= cnt;
	}
	alpha = clamp(alpha, 0, 1);
	//alpha = smoothstep(0, 1, alpha);
	const float t = 80;
	alpha = customClamp(alpha*255, t, 255-t)/255;
	alpha = round(alpha * 5) / 5;
	Result = vec4(vec3(texture(Texture, TextCoordsFrag).xyz), alpha);	
}