#pragma once

#include "RenderTarget.h"
#include "ShaderProgram.h"

class GlInstance
{
private:
	RenderTarget m_leftEyeFramebuffer;
	RenderTarget m_rightEyeFramebuffer;

	GLuint m_vertexArray;
	GLuint m_vertexBuffer;

	ShaderProgram m_fallbackShaderProgram;

public:
	void Init();
	void Cleanup();
	void RenderFrame();

private:
	void RenderScene(vr::EVREye eye);
};

inline GlInstance& Gl()
{
	static GlInstance gl;
	return gl;
}