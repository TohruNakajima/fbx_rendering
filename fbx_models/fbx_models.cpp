// fbx_models.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "fbx_models.h"
#include <fstream>
#include <string>
#include <sstream>
// Function prototypes (add these to the top of your .cpp file)
GLFWwindow* initWindow(int width, int height, const char* title);

/* Tab character ("\t") counter */
int numTabs = 0;

/**
 * Print the required number of tabs.
 */
void PrintTabs() {
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}

/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch (type) {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}
/**
 * Print a node, its attributes, and all its children recursively.
 */
 /**
  * Print an attribute.
  */
void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}


void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}


// Function definitions

const char* readShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << filePath << std::endl;
        return nullptr;  // Return nullptr or handle the error as appropriate
    }

    std::stringstream shaderStream;
    shaderStream << file.rdbuf();  // Read the entire file into the stringstream
    file.close();

    std::string shaderCode = shaderStream.str();  // Convert stringstream to string
    std::cout << shaderCode;
    return shaderCode.c_str();  // Return the C-style string (const char*)
}

// Initializes GLFW and creates a windowed context
GLFWwindow* initWindow(int width, int height, const char* title) {
    // Set OpenGL version hints (GL 3.3 core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed context with the specified dimensions
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(-1); // Terminate if window creation fails
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Set the framebuffer size callback to adjust the viewport when resizing the window
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });

    return window;
}
// Helper function to check shader compilation errors
void checkShaderCompileErrors(GLuint shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
// Helper function to check program linking errors
void checkProgramLinkingErrors(GLuint program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}


// Create a basic shader program
GLuint createBasicShaderProgram() {
    std::string vertex_shader_path = "C:/Users/a/source/repos/fbx_models/fbx_models/vertex_shader.glsl";
    std::string fragment_shader_path = "C:/Users/a/source/repos/fbx_models/fbx_models/fragment_shader.glsl";

    const char* vertexShaderSource = readShaderSource(vertex_shader_path);
    const char* fragmentShaderSource = readShaderSource(fragment_shader_path);

    // Compile and link the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    std::cout << "compile vertex shader" << std::endl;
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader);

    // Compile and link the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    std::cout << "compile fragment shader" << std::endl;

    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader);

    // Link shaders into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinkingErrors(shaderProgram);

    // Cleanup shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}



// Main rendering function
void renderModel(const GLuint VAO, GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    // Set up the model, view, and projection matrices (using GLM for transformations)
    glm::mat4 model = glm::mat4(1.0f);  // Identity matrix (no transformation)
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Camera at (0, 0, 3)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f); // Perspective projection

    // Pass matrices to shaders
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set light position (simple directional light)
    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPosLoc, 0.0f, 1.0f, 2.0f);  // Light at position (0, 1, 2)

    // Set object color (for rendering without textures)
    GLuint objColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(objColorLoc, 1.0f, 0.5f, 0.31f); // Orange color

    // Render the model
    glBindVertexArray(VAO);  // Bind the VAO containing the model data
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // Draw using the index buffer
    glBindVertexArray(0);  // Unbind the VAO
}

int main()
{
	std::cout << "Hello CMake." << std::endl;
	const char* lFilename = "C:/Users/a/source/repos/fbx_models/fbx_models/models/rin.fbx";
	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();
    
    
    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }


    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported, so get rid of the importer.
    lImporter->Destroy();
	
    std::cout << "fbx file " << lFilename << " is imported" << std::endl;

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode) {
        for (int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintNode(lRootNode->GetChild(i));
    }


    // Step 1: Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create a GLFW windowed context
    GLFWwindow* window = initWindow(800, 600, "FBX Model Renderer");


    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    GLuint VAO = 0; // Vertex Array Object for the FBX model
    GLuint texture = 0; // Texture ID

    while (!glfwWindowShouldClose(window)) {
        // Poll for events
        glfwPollEvents();



        // Step 6: Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Set background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear both color and depth buffers
        Mesh mesh = loadFBXMesh(lRootNode);
        GLuint shaderProgram = createBasicShaderProgram();
        VAO = createVAO(mesh);
        // Step 7: Render the FBX model
        renderModel(VAO, shaderProgram);

        // Step 8: Swap buffers
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();

    return 0;
}