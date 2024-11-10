#include "fbx_render.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Mesh loadFBXMesh(FbxNode* node) {
    Mesh meshData;

    // Process the node's mesh
    FbxMesh* fbxMesh = node->GetMesh();
    if (fbxMesh) {
        int vertexCount = fbxMesh->GetNode()->GetMesh()->GetControlPointsCount();

        // Loop through the vertices
        for (int i = 0; i < vertexCount; ++i) {
            FbxVector4 vertex = fbxMesh->GetControlPointAt(i);
            meshData.vertices.push_back(glm::vec3(vertex[0], vertex[1], vertex[2]));
        }

        // Process normals (if available)
        if (fbxMesh->GetElementNormalCount() > 0) {
            FbxGeometryElementNormal* normalElement = fbxMesh->GetElementNormal(0);
            for (int i = 0; i < vertexCount; ++i) {
                FbxVector4 normal = normalElement->GetDirectArray().GetAt(i);
                meshData.normals.push_back(glm::vec3(normal[0], normal[1], normal[2]));
            }
        }

        // Process texture coordinates (if available)
        if (fbxMesh->GetElementUVCount() > 0) {
            FbxGeometryElementUV* uvElement = fbxMesh->GetElementUV(0);
            for (int i = 0; i < vertexCount; ++i) {
                FbxVector2 uv = uvElement->GetDirectArray().GetAt(i);
                meshData.texCoords.push_back(glm::vec2(uv[0], uv[1]));
            }
        }

        // Process indices (if using an EBO)
        int polygonCount = fbxMesh->GetPolygonCount();
        for (int i = 0; i < polygonCount; ++i) {
            for (int j = 0; j < fbxMesh->GetPolygonSize(i); ++j) {
                meshData.indices.push_back(fbxMesh->GetPolygonVertex(i, j));
            }
        }
    }

    return meshData;
}


GLuint createVAO(const Mesh& mesh) {
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind VBO for vertex data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3), mesh.vertices.data(), GL_STATIC_DRAW);

    // Vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Create and bind VBO for normal data
    GLuint normalVBO;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(glm::vec3), mesh.normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // Create and bind VBO for texture coordinates
    GLuint texCoordVBO;
    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.texCoords.size() * sizeof(glm::vec2), mesh.texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(2);

    // If you have indices, use an EBO
    if (!mesh.indices.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    return VAO;
}


GLuint loadTexture(const char* texturePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "Failed to load texture!" << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return textureID;
}


// Function to render the model
void renderModel(const GLuint VAO, const GLuint texture, GLuint shaderProgram) {
    glUseProgram(shaderProgram);  // Use the shader program

    // Set up the model, view, and projection matrices (you can use GLM for transformations)
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix (for now, modify if needed)

    // View matrix: Camera position, looking at the origin, with the up direction as Y
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),  // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),  // Looking at the origin
        glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector

    // Projection matrix: Perspective projection (field of view, aspect ratio, near and far planes)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),  // Field of view angle in radians
        800.0f / 600.0f,    // Aspect ratio (width / height)
        0.1f,               // Near plane
        100.0f);            // Far plane

    // Set the model, view, and projection matrices in the shader program
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // Pass the matrices to the shader program
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);   // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture);  // Bind the texture
    GLint textureLoc = glGetUniformLocation(shaderProgram, "texture1");
    glUniform1i(textureLoc, 0);  // Set the texture sampler to texture unit 0

    // Draw the model
    glBindVertexArray(VAO);  // Bind the VAO (which stores vertex attributes)
    glDrawArrays(GL_TRIANGLES, 0, 36);  // Assuming the model is made of 36 vertices (modify this based on your model)
}
