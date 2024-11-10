#ifndef H_FBX_RENDER
#define H_FBX_RENDER


#include "stb_image.h"

#include <fbxsdk.h>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>   // GLEW must be included first
#include <GLFW/glfw3.h> // or SDL, or another OpenGL context library
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // For glm::perspective and glm::lookAt
#include <glm/gtc/type_ptr.hpp>  // For glm::value_ptr (to send matrices to shaders)


// Struct to hold mesh data
struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;  // If you're using an EBO (Element Buffer Object)
};

Mesh loadFBXMesh(FbxNode* node);
GLuint createVAO(const Mesh& mesh);
GLuint loadTexture(const char* texturePath);
void renderModel(const GLuint VAO, const GLuint texture, GLuint shaderProgram);
#endif
