#include "Shader.h"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

// default
Shader::Shader() {}

// activate shader
void Shader::activate() {
    glUseProgram(id);
}

// cleanup
void Shader::cleanup() {
    glDeleteProgram(id);
}

void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat3(const std::string& name, glm::mat3 val) {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setMat4(const std::string& name, glm::mat4 val) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

std::string Shader::LoadingShaders(const std::string file) {
    std::ifstream inFile(file);
    std::stringstream buffer;

    if (!inFile.is_open())
    {
        std::cout << "Can't open the file!" << file << std::endl;
    }

    buffer << inFile.rdbuf();
    return buffer.str();
}

unsigned int Shader::CreateShaderProgram(const GLchar* vertexShaderText, const GLchar* fragmentShaderText) {
    unsigned int vertexS = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentS = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = glCreateProgram();

    glShaderSource(vertexS, 1, &vertexShaderText, NULL);
    glCompileShader(vertexS);

    glShaderSource(fragmentS, 1, &fragmentShaderText, NULL);
    glCompileShader(fragmentS);

    glAttachShader(shaderProgram, vertexS);
    glAttachShader(shaderProgram, fragmentS);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexS);
    glDeleteShader(fragmentS);

    return shaderProgram;
}
