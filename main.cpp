#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

double accumulatedTime = 0;

//blue intensity own var
float blueIntensity = 0.2f;
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //on button press own func
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        blueIntensity = std::clamp(blueIntensity + 0.00025f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        blueIntensity = std::clamp(blueIntensity - 0.00025f, 0.0f, 1.0f);
    }
}

void framebuffer_size_callback(GLFWwindow *window,
                               int width, int height) {
    glViewport(0, 0, width, height);
}

std::string readFileToString(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filePath.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
    printf("Loading shader:\n%s\n", shaderPath.c_str());
    const std::string shaderText = readFileToString(shaderPath);
    const GLuint shader = glCreateShader(shaderType);
    const char *s_str = shaderText.c_str();
    glShaderSource(shader, 1, &s_str, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
    }
    return shader;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    const GLuint vertexShader = generateShader("shaders/vertex.vs", GL_VERTEX_SHADER);
    const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);

    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Error! Making Shader Program: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ///////////

    GLuint uniformPaintColorLocation = glGetUniformLocation(shaderProgram, "paintColor");
    GLuint uniformOffsetLocation = glGetUniformLocation(shaderProgram, "offset");
    GLuint uniformBlueIntensityLocation = glGetUniformLocation(shaderProgram, "blueIntensity");
    GLuint uniformTimeLocation = glGetUniformLocation(shaderProgram, "time");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Vertices
    const float vertices[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,

            0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,
                          GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //UVS
    const float UVs[] = {
        1, 1,
        1, 0,
        0, 0,

        1, 1,
        0, 0,
        0, 1
    };
    GLuint UvBO;
    glGenBuffers(1, &UvBO);
    glBindBuffer(GL_ARRAY_BUFFER, UvBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UVs),
                 UVs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT,
                          GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //set paintColor
    glUseProgram(shaderProgram);
    glUniform4f(uniformPaintColorLocation,  0.0f,0.0f,1.0f,1.0f);
    glUseProgram(0);

    //set offset
    // glUseProgram(shaderProgram);
    // glUniform2f(uniformOffsetLocation,  0.2f,0.2f);
    // glUseProgram(0);

    // glm::vec4 clearColor = glm::vec4(0.6f, 0.4f, 0.2f, 1.0f);
    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);

    double elapsedSecs;
    float changingOffset = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        clock_t begin = clock();
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // glUniform4f(uniformPaintColorLocation,  0.0f,0.0f,1.0f,1.0f);
        //continuously change offset
        changingOffset += 0.0001f;
        glUniform2f(uniformOffsetLocation,  sin(changingOffset * 3) / 2,sin(changingOffset) / 2);

        //set blueIntensity
        glUniform1f(uniformBlueIntensityLocation, blueIntensity);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
        clock_t end = clock();
        elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
        accumulatedTime += elapsedSecs;

        glUniform1f(uniformTimeLocation,  accumulatedTime);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &UvBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}