#pragma once

#ifdef GL_ASSERTS
#define glAssertSkip(expr, expected) if (expr != expected) { throw std::runtime_error(std::format("GL expected {} got {:x} \nGL: {}", #expected, expr, (const char*)gluErrorString(glGetError()))); }
#define glCheckErrors() if (glGetError()) { gluErrorString(glGetError()); }
#else
#define glAssertSkip(expr, expected)
#define glCheckErrors()
#endif // GL_ASSERTS