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

float compare(float old, float new, float stp) {
	if (old < 0 || new > 0) {
		if (mod(abs(new), 1000) - stp > mod(abs(old), 1000)) {
			return sign(old) * (abs(new) - stp);
		}
	}
	return old;
}

void main() {
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	if (LightsSize > 0) {
		bool obst = imageLoad(Texture, ivec2(storePos)).a == 255;
		int lightIndex = -1;
		for (int i = 0; i < LightsSize; i++) {
			if (ivec2(storePos) == Lights[i]) {
				lightIndex = i;
				break;
			}
		}
		float v = (lightIndex >= 0 ? 151 + (lightIndex + 1) * 1000 : 1) * (obst ? -1 : 1);
		imageStore(Lighting, ivec2(storePos), vec4(v));
	}
	else {
		memoryBarrierImage();
		float oldV = imageLoad(Lighting, ivec2(storePos)).x;
		float v = oldV;
		for (int j = 0; j < 4; j++) {
			float im = imageLoad(Lighting, ivec2(storePos + shift[j])).x;
			v = compare(v, im, 1);
		}
		for (int j = 4; j < 8; j++) {
			float im = imageLoad(Lighting, ivec2(storePos + shift[j])).x;
			v = compare(v, im, sqrt(2));
		}
		if (oldV != v) {
			imageStore(Lighting, ivec2(storePos), vec4(v, 0, 0, 1));
		}
	}
}
