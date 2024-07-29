#pragma once
//
//#include "RenderTarget.h"
//#include "ShaderProgram.h"

class GlInstance
{
private:
	SDL_GLContext m_sdlGlContext;

	//RenderTarget m_leftEyeFramebuffer;
	//RenderTarget m_rightEyeFramebuffer;

	//GLuint m_vertexArray;
	//GLuint m_vertexBuffer;

	//ShaderProgram m_fallbackShaderProgram;

public:
	void Init();
	void Cleanup();
	void RenderFrame();

private:
	//void RenderScene(vr::EVREye eye, vr::TrackedDevicePose_t renderPose);
};

inline GlInstance& Gl()
{
	static GlInstance gl;
	return gl;
}