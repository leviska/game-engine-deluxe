#include <scene.h>

#include <utility.h>

#include <resources/shaders.h>
#include <resources/consts.h>

void DrawFramebufferToScreen(const FrameBuffer& buffer) {
	FrameBuffer::SelectWindow();
	const Shader& bufShader = Shaders().Shaders[to_ui32(ShadersId::Buffer)];
	bufShader.Select();
	bufShader.UpdateProjection(Consts().WindowSize, true);
	buffer.Draw(Consts().WindowSize / 2u, Consts().Scale, bufShader);
}
