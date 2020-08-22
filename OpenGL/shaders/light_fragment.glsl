#version 330 core
#extension GL_ARB_shading_language_420pack: enable
in vec2 TextCoordsFrag;
out vec4 Result;

layout (binding = 0) uniform sampler2DRect Texture;
layout (binding = 1) uniform sampler2DRect LightingTexture;

const ivec2 shift[4] = { ivec2(1, 0), ivec2(0, 1), ivec2(-1, 0), ivec2(0, -1) };

void main()
{
	float alpha = texture(LightingTexture, TextCoordsFrag).a;
	/*int cnt = 0;
	for (int i = 0; i < 4; i++) {
		float a = texture(LightingTexture, TextCoordsFrag + shift[i]).a;
		float r = texture(LightingTexture, TextCoordsFrag + shift[i]).r;
		if (r < 1) {
			cnt++;
			alpha += a;
		}
	}
	alpha /= cnt;*/
	//alpha = smoothstep(0, 1, alpha);
	Result = vec4(texture(Texture, TextCoordsFrag).xyz, alpha);
}
