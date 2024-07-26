#include "pch.h"
#include "ShaderProgram.h"

void ShaderProgram::Compile(const char *vertSource, const char *fragSource)
{
	GLuint vert = CompileShader(vertSource, GL_VERTEX_SHADER);
	GLuint frag = CompileShader(fragSource, GL_FRAGMENT_SHADER);

	m_program = glCreateProgram();
	glAttachShader(m_program, vert);
	glAttachShader(m_program, frag);
	glLinkProgram(m_program);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

GLuint ShaderProgram::CompileShader(const char *source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		throw std::runtime_error(std::format("Shader compilation failed {}", infoLog));
	}

	return shader;
}