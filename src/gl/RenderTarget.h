#pragma once

class RenderTarget
{
private:
	GLuint m_framebuffer;
	GLuint m_renderbuffer;

	uint32_t m_width;
	uint32_t m_height;

public:
	void Init();

	inline GLuint FramebufferName() const { return m_framebuffer; }
	inline GLuint RenderbufferName() const { return m_renderbuffer; }

	inline uint32_t Width() const { return m_width; }
	inline uint32_t Height() const { return m_height; }
};