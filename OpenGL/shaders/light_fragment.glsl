#version 330 core
in vec2 TextCoordsFrag;
out vec4 Result;

uniform float Time;

layout(binding=0) uniform sampler2DRect Texture;
layout(binding=1) uniform sampler2DRect ObstructTexture;

vec2 Light;

const ivec2 shift[4] = { ivec2(-1, 0), ivec2(0, 1), ivec2(1, 0), ivec2(0, -1) };

const vec2 parts[2] = { vec2(0, 1), vec2(1, 0) };

bool obstruct(ivec2 pos) {
	return length(texture(ObstructTexture, pos).xyz) > 0.01;
}

float sendRay(vec2 start) {
	vec2 diff = Light - start;
	diff /= length(diff) * 2;
	vec2 cur = start;
	for (int i = 0; i < 170; i++) {
		if (length(cur - Light) < 1) {
			return 1;
		}
		vec2 tcur = cur + diff;
		ivec2 tmp = ivec2(tcur);
		if (obstruct(tmp)) {
			bool find = false;
			for (int j = 0; j < 2; j++) {
				tcur = cur + diff * parts[j];
				tmp = ivec2(tcur);
				if (!obstruct(tmp)) {
					cur = tcur;
					find = true;
					diff = Light - cur;
					diff /= length(diff) * 2;
					break;
				}
			}
			if (!find)
				return 0;
		}
		else {
			cur = tcur;
		}
	}
	return 0;
}

void main()
{
	Light = vec2(8 * 16 + 32 * sin(Time * 10), 6 * 16 + 8 * cos(Time * 5));
	vec2 tmp = TextCoordsFrag - Light;
    if (length(tmp) < 1) {
		Result = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else {
		float fv = 1.0; 
		if (!obstruct(ivec2(TextCoordsFrag))) {
			float cnt = sendRay(TextCoordsFrag);
			
			fv = cnt * 26 + 80;
			fv /= 106;
		}
		Result = vec4(1.0, 1.0, 1.0, fv) * texture(Texture, TextCoordsFrag);
	}
}
