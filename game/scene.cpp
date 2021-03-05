#include "scene.h"

#include "game.h"
#include "shaders.h"

void DrawFramebufferToScreen(const FrameBuffer& buffer) {
	FrameBuffer::SelectWindow();
	const Shader& bufShader = Shaders().Shaders[to_ui32(ShadersId::Buffer)];
	bufShader.Select();
	bufShader.UpdateProjection(Game().GetWindow().GetSize(), true);
	buffer.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), bufShader);
}
