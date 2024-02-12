#ifndef SHADER_H_
#define SHADER_H_

#include "iostream"
#include "glad/glad.h"
#include "glm/glm.hpp"
class Shader
{
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void UploadUniformFloat2(const std::string& name,
        const glm::vec2& vector);
    void UploadUniformFloat3(const std::string& name,
        const glm::vec3& vector);
    void UploadUniformFloat4(const std::string& name,
        const glm::vec4& vector);
    void UploadUniformMat4x4(const std::string& name,
        const glm::mat4& mat);
    void UploadUniformInt(const std::string& name,
        const int integer);
    void UploadUniformFloat(const std::string& name,
        const float flpt);

private:
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;

    void CompileShader(GLenum shaderType, const std::string& shaderSrc);
};

#endif