#pragma once

class ShaderProgram
{
private:
	GLuint m_program;

public:
	inline GLuint Id() const { return m_program; }

public:
	void Compile(const char *vertSource, const char *fragSource);

private:
	GLuint CompileShader(const char *source, GLenum type);
};