#version 430 core
layout (location = 0) in vec2 Vertex;

layout (binding = 0, rgba8ui) uniform restrict readonly uimage2D Texture;
layout (binding = 1, r32f) uniform restrict image2D Lighting;

uniform int LightsSize = 0;
uniform ivec2 Lights[16];

const ivec2 shift[8] = { 
	ivec2(1, 0), ivec2(0, 1), ivec2(-1, 0), ivec2(0, -1),
	ivec2(1, 1), ivec2(-1, 1), ivec2(-1, -1), ivec2(1, -1)
};

layout (local_size_x = 8, local_size_y = 8) in;

void main() {
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	if (LightsSize > 0) {
		bool obst = imageLoad(Texture, ivec2(storePos)).a == 255;
		bool isLight = false;
		for (int i = 0; i < LightsSize; i++) {
			if (ivec2(storePos) == Lights[i]) {
				isLight = true;
				break;
			}
		}
		float v = (isLight ? 150 : 0) + (obst ? 1000 : 0);
		imageStore(Lighting, ivec2(storePos), vec4(v));
	}
	else {
		memoryBarrierImage();
		float oldV = imageLoad(Lighting, ivec2(storePos)).x;
		bool obst = oldV >= 1000;
		float v = oldV;
		if (obst) {
			v -= 1000;
		}
		for (int j = 0; j < 4; j++) {
			float im = imageLoad(Lighting, ivec2(storePos + shift[j])).x;
			bool imobst = im >= 1000;
			if (imobst) {
				im -= 1000;
			}
			if (obst || !imobst) {
				v = max(im - 1, v);
			}
		}
		for (int j = 4; j < 8; j++) {
			float im = imageLoad(Lighting, ivec2(storePos + shift[j])).x;
			bool imobst = im >= 1000;
			if (imobst) {
				im -= 1000;
			}
			if (obst || !imobst) {
				v = max(im - sqrt(2), v);
			}
		}
		if (obst) {
			v += 1000;
		}
		if (oldV != v) {
			imageStore(Lighting, ivec2(storePos), vec4(v, 0, 0, 1));
		}
	}
}
