#define STB_IMAGE_IMPLEMENTATION  
#include <iostream>
#include <stb_image.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TOOLS/shader.h"
#include "TOOLS/camera.h"
#include "TOOLS/keyboard.h"
#include "TOOLS/mouse.h"
#include "TOOLS/file_loader.h"

// Main declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// window settings
const unsigned int sWidth = 1200;
const unsigned int sHeight = 1200;
float x, y, z;

// camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
// mouse settings
float lastX = sWidth / 2.0f;
float lastY = sHeight / 2.0f;
bool firstMouse = true;

double dt = 0.0f;
double lastFrame = 0.0f; 

// Positions
glm::vec3 lightPos(0.0f, 0.0f, 4.0f);
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);
// Define the range of movement along axis
float moveRange = 1.2f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window creation and making context to window
    GLFWwindow* window = glfwCreateWindow(sWidth, sHeight, "GRATS", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Callbacks
    // window callback
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // keyboard callback
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    // mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // glad loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // loading shaders
    Shader shader;

    std::string lightVextexShaderText= shader.LoadingShaders("src/SHADERS/light_vs.glsl");
    std::string lightFragmentShaderText = shader.LoadingShaders("src/SHADERS/light_fs.glsl");
    unsigned int light = shader.CreateShaderProgram(lightVextexShaderText.c_str(), lightFragmentShaderText.c_str());

    std::string cubeVextexShaderText = shader.LoadingShaders("src/SHADERS/cube_vs.glsl");
    std::string cubeFragmentShaderText = shader.LoadingShaders("src/SHADERS/cube_fs.glsl");
    unsigned int cube = shader.CreateShaderProgram(cubeVextexShaderText.c_str(), cubeFragmentShaderText.c_str());

    // vertices: vertex position, normal position, texture position, etc.
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    //glm::vec3 circlePositions[15];
    //float radius = 10.0f;
    //float angleIncrement = glm::radians(360.0f / 15.0f);
    //for (int i = 0; i < 15; i++) {
    //    float angle = angleIncrement * i;
    //    float x = radius * cos(angle);
    //    float y = radius * sin(angle);
    //    circlePositions[i] = glm::vec3(x, y, -15.0f);
    //}

    //  ~~~~~~~~~~~~~~~~~~~~~~~~~~ TEXTURES ~~~~~~~~~~~~~~~~~~~~~~~~~~
     
    // diffuse map
    unsigned int diffuseMap = loadTexture(FileSystem::getPath("src/ASSETS/images/abstr.png").c_str());

    // specular map
    unsigned int specularMap = loadTexture(FileSystem::getPath("src/ASSETS/images/abstr_specular.png").c_str());

    unsigned int VBO, cubeVAO; //EBO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
    glBindVertexArray(cubeVAO);
    
    //glGenBuffers(1, &EBO);  
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); 
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~ LIGHTNING ~~~~~~~~~~~~~~~~~~~~~~~~~~
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // camera's points for orientation
    x = 0.0f;
    y = 0.0f;
    z = 10.0f;

    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        // time for frames
        double currentTime = glfwGetTime();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window, dt);

        // refreshing and z-buffer handling
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind texture
        //glBindTexture(GL_TEXTURE_2D, texture1);
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        //trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 200.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        // Calculate the x-axis position based on time and moveRange
        float pos_change = moveRange * sin(glfwGetTime());
        float pos_change2 = moveRange * cos(glfwGetTime());
        float pos_change3 = moveRange * -cos(glfwGetTime());


        // Create a translation vector with the new x-axis position
        glm::vec3 translation(pos_change, pos_change2, 0.0f);
        glm::vec3 translation2(pos_change, pos_change3, 0.0f);



        // Light specifier
        glUseProgram(light);
        //int objectLocation = glGetUniformLocation(light, "objectColor");
        //glUniform3f(objectLocation, 0.4f, 0.8f, 0.4f);
        int lightColorLocation = glGetUniformLocation(light, "lightColor");
        glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
        int lightPositionLocation = glGetUniformLocation(light, "lightPos");
        glUniform3fv(lightPositionLocation, 1, &lightPos[0]);
        int viewPositionLocation = glGetUniformLocation(light, "viewPos");
        glUniform3fv(viewPositionLocation, 1, &camera.cameraPos[0]);

        // create transformations: MVP Matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, circlePositions[i]);
        model = glm::translate(glm::mat4(1.0f), cubePos + translation2);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(60.0f), (float)sWidth / (float)sHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(light, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(light, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(light, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // draw triangles points for cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //view = glm::translate(view, glm::vec3(-x, -y, -z));
        //view = camera.getViewMatrix();
        //projection = glm::perspective(glm::radians(60.0f), (float)sWidth / (float)sHeight, 0.1f, 100.0f);

        //glUniformMatrix4fv(glGetUniformLocation(light, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glUniformMatrix4fv(glGetUniformLocation(light, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //glUniformMatrix4fv(glGetUniformLocation(light, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


        // draw triangles points for cube
        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        /*
        float aTime = glfwGetTime();
        float color1 = sin(aTime);
        float color2 = cos(aTime);

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "newColor");
        glUniform4f(vertexColorLocation, color1, color2,  1.0f, 1.0f);
        */

        // LIGHT CUBE : create transformations : MVP Matrix - second

        glm::mat4 model2 = glm::mat4(0.5f);
        glm::mat4 view2 = glm::mat4(1.0f);
        glm::mat4 projection2 = glm::mat4(1.0f);

        //model2 = glm::translate(model2, (float)glfwGetTime() * -lightPos);
        // Update the model2's position
        model2 = glm::translate(glm::mat4(1.0f), lightPos + translation);

        model2 = glm::scale(model2, glm::vec3(0.4f));


        model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f));
        view2 = camera.getViewMatrix();
        projection2 = glm::perspective(glm::radians(40.0f), (float)sWidth / (float)sHeight, 0.1f, 100.0f);


        glUseProgram(cube);
        unsigned int timeLoc = glGetUniformLocation(cube, "time");
        glUniform1f(timeLoc, currentTime);

        glUniformMatrix4fv(glGetUniformLocation(cube, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniformMatrix4fv(glGetUniformLocation(cube, "view"), 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(glGetUniformLocation(cube, "projection"), 1, GL_FALSE, glm::value_ptr(projection2));

        // draw triangles points for light cube
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // send new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleaning time!
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, double dt)
{

    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
  
    // camera movement
    if (Keyboard::key(GLFW_KEY_W)) {
        camera.updateCameraPos(CameraDirection::FORWARD, dt);
    }

    if (Keyboard::key(GLFW_KEY_S)) {
        camera.updateCameraPos(CameraDirection::BACKWARD, dt);
    }

    if (Keyboard::key(GLFW_KEY_D)) {
        camera.updateCameraPos(CameraDirection::RIGHT, dt);
    }

    if (Keyboard::key(GLFW_KEY_A)) {
        camera.updateCameraPos(CameraDirection::LEFT, dt);
    }

    if (Keyboard::key(GLFW_KEY_E)) {
        camera.updateCameraPos(CameraDirection::UP, dt);
    }

    if (Keyboard::key(GLFW_KEY_Q)) {
        camera.updateCameraPos(CameraDirection::DOWN, dt);
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.08f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;


    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

    camera.cameraFront = glm::normalize(direction);
    camera.cameraRight = glm::normalize(glm::cross(camera.cameraFront, camera.worldUp));
    camera.cameraUp = glm::normalize(glm::cross(camera.cameraRight, camera.cameraFront));

 }

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (camera.zoom >= 1.0f && camera.zoom <= 45.0f)
        camera.zoom -= yoffset;
    if (camera.zoom <= 1.0f)
        camera.zoom = 1.0f;
    if (camera.zoom >= 45.0f)
        camera.zoom = 45.0f;
}


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}