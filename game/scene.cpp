#include "scene.h"

#include "game.h"
#include "shaders.h"

void DrawFramebufferToScreen(const FrameBuffer& buffer) {
	FrameBuffer::SelectWindow();
	buffer.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), Shaders().Shaders[to_ui32(ShadersId::Buffer)]);
}