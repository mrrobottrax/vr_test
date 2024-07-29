#include "pch.h"
#include "RenderTarget.h"

#include "GlDebug.h"

void RenderTarget::Init()
{
	// create frame buffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// create render buffer
	vr::VRSystem()->GetRecommendedRenderTargetSize(&m_width, &m_height);

	glGenRenderbuffers(1, &m_renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, m_width, m_height);

	// bind the two
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderbuffer);

	// check errors
	glCheckErrors();
	glAssertSkip(glCheckFramebufferStatus(GL_FRAMEBUFFER), GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}