#include "Shader.h"

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
	ShaderProgram = glCreateProgram();
	CompileShader(GL_VERTEX_SHADER, vertexSrc);
	CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	
}
Shader::~Shader() {
	glDeleteProgram(ShaderProgram);
}

void Shader::Bind() const {
	glUseProgram(ShaderProgram);
}
void Shader::Unbind() const {
	glUseProgram(0);
}
void Shader::UploadUniformFloat2(const std::string& name,
    const glm::vec2& vector) {
	glUniform2f(glGetUniformLocation(ShaderProgram, name.c_str()), vector.x, vector.y);
}
void Shader::UploadUniformFloat3(const std::string& name,
	const glm::vec3& vector) {
	glUniform3f(glGetUniformLocation(ShaderProgram, name.c_str()), vector.x, vector.y,vector.z);
}
void Shader::UploadUniformFloat4(const std::string& name,
	const glm::vec4& vector) {
	glUniform4f(glGetUniformLocation(ShaderProgram, name.c_str()), vector[0], vector[1],vector[2],vector[3]);
}

void Shader::UploadUniformMat4x4(const std::string& name,
	const glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);

}
void Shader::UploadUniformInt(const std::string& name,
	const int integer) {
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), integer);
}
void Shader::UploadUniformFloat(const std::string& name,
	const float flpt) {
	glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), flpt);
}

void Shader::CompileShader(GLenum shaderType, const std::string& shaderSrc) {

	if (shaderType == GL_VERTEX_SHADER) {
		VertexShader = glCreateShader(shaderType);
		const GLchar* src = shaderSrc.c_str();
		glShaderSource(VertexShader, 1, &src, nullptr);
		glCompileShader(VertexShader);
	}
	else {
		FragmentShader = glCreateShader(shaderType);
		const GLchar* src = shaderSrc.c_str();
		glShaderSource(FragmentShader, 1, &src, nullptr);
		glCompileShader(FragmentShader);
	}
}