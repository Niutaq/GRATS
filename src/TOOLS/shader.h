#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
    // program ID
    unsigned int id;
    
    // default
    Shader();
    // activate shader
    void activate();

    // cleanup
    void cleanup();

    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setMat3(const std::string& name, glm::mat3 val);
    void setMat4(const std::string& name, glm::mat4 val);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setVec4(const std::string& name, float x, float y, float z, float w);


    std::string LoadingShaders(const std::string file);

    unsigned int CreateShaderProgram(const GLchar* vertexShaderText, const GLchar* fragmentShaderText);
};

#endif