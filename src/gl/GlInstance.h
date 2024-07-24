#pragma once

class GlInstance
{
private:
	GLuint m_leftEyeFramebuffer;
	GLuint m_leftEyeRenderbuffer;

public:
	void Init();
	void Cleanup();
	void RenderFrame();
};

inline GlInstance& Gl()
{
	static GlInstance gl;
	return gl;
}