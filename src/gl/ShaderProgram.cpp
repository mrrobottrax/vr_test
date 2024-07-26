#include "pch.h"
#include "ShaderProgram.h"

void ShaderProgram::Compile(const char *vertSource, const char *fragSource, const char *debugName = "unnamed")
{
	GLuint vert = CompileShader(vertSource, GL_VERTEX_SHADER, debugName);
	GLuint frag = CompileShader(fragSource, GL_FRAGMENT_SHADER, debugName);

	m_program = glCreateProgram();
	glAttachShader(m_program, vert);
	glAttachShader(m_program, frag);
	glLinkProgram(m_program);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

GLuint ShaderProgram::CompileShader(const char *source, GLenum type, const char *debugName)
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
		const char *typeName = "unknown";
		switch (type)
		{
		case GL_VERTEX_SHADER:
			typeName = "Vertex";
			break;
		case GL_FRAGMENT_SHADER:
			typeName = "Fragment";
			break;
		}
		throw std::runtime_error(std::format("Shader compilation of {} : {} failed!\n{}", debugName, typeName, infoLog));
	}

	return shader;
}