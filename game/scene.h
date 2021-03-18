#pragma once

#include <framebuffer.h>

class Scene {
public:
	virtual void Reset() = 0;

	virtual void Update() = 0;
	virtual void Clear() = 0;
	virtual void Draw() = 0;
};

void DrawFramebufferToScreen(const FrameBuffer& buffer);