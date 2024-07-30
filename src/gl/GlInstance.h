#pragma once
//
//#include "RenderTarget.h"
//#include "ShaderProgram.h"

class GlInstance
{
private:
	SDL_GLContext m_sdlGlContext;
	ImGuiContext *m_pImguiContext;

	//RenderTarget m_leftEyeFramebuffer;
	//RenderTarget m_rightEyeFramebuffer;

	//GLuint m_vertexArray;
	//GLuint m_vertexBuffer;

	//ShaderProgram m_fallbackShaderProgram;

public:
	void Init(SDL_Window *pWindow);
	void Cleanup();
	void RenderFrame();

private:
	//void RenderScene(vr::EVREye eye, vr::TrackedDevicePose_t renderPose);
};

inline GlInstance &Gl()
{
	static GlInstance gl;
	return gl;
}