#version 330 core
#extension GL_ARB_shader_image_load_store: require
#extension GL_ARB_shading_language_420pack: require
in vec2 TextCoordsFrag;
out vec4 Result;

//uniform layout(binding=0, rgba8ui) restrict coherent uimage2D Lighting;

layout (binding = 0) uniform sampler2DRect Texture;
layout (binding = 1) uniform sampler2DRect Obstructs;

const ivec2 shift[8] = { 
	ivec2(1, 0), ivec2(0, 1), ivec2(-1, 0), ivec2(0, -1),
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

void main()
{
	float alpha = 1;
	Result = vec4(vec3(texture(Texture, TextCoordsFrag).xyz), alpha);	
}

//	float red = texture(LightingTexture, TextCoordsFrag).r;
//	float alpha = texture(LightingTexture, TextCoordsFrag).a;
//	if (red < 1) {
//		int cnt = 1;
//		for (int i = 0; i < 8; i++) {
//			vec2 cur = TextCoordsFrag;
//			for (int j = 0; j < 10; j++) {
//				cur += shift[i];
//				float a = texture(LightingTexture, cur).a;
//				float r = texture(LightingTexture, cur).r;
//				if (r < 1) {
//					cnt++;
//					alpha += a;
//				}
//				else {
//					break;
//				}
//			}
//		}
//		alpha /= cnt;
//	}
//	alpha = clamp(alpha, 0, 1);
//	//alpha = smoothstep(0, 1, alpha);
//	const float t = 80;
//	alpha = customClamp(alpha*255, t, 255-t)/255;
//	alpha = round(alpha * 5) / 5;
//	Result = vec4(vec3(texture(Texture, TextCoordsFrag).xyz) * alpha + vec3(0, 0, 0) * (1-alpha), 1);