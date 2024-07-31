#pragma once

class ShaderProgram
{
private:
	GLuint m_program;

public:
	inline GLuint Id() const { return m_program; }

public:
	void Compile(const void *vertSource, const void *fragSource, const char* debugName);

private:
	GLuint CompileShader(const char *source, GLenum type, const char* debugName);
};