#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Função para inicializar o GLFW e criar a janela
bool initGLFW(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar o GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jayme-Lista1", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

// Função para inicializar o GLAD
bool initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar o GLAD" << std::endl;
        return false;
    }
    return true;
}

// Função para criar um programa de shader
GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void renderShape(GLuint shaderProgram, GLenum primitiveType, float red, float green, float blue) {
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.0f,
         0.0f,  0.0f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    glDrawArrays(primitiveType, 0, 6);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void exA(GLuint shaderProgram, float red, float green, float blue) {
    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f);
}

void exB(GLuint shaderProgram, float red, float green, float blue) {
    renderShape(shaderProgram, GL_LINE_LOOP, 1.0f, 0.0f, 0.0f);
}

void exC(GLuint shaderProgram, float red, float green, float blue) {
    renderShape(shaderProgram, GL_POINTS, 1.0f, 0.0f, 0.0f);
}

void exD(GLuint shaderProgram, float red, float green, float blue) {
    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f); // Vermelho como GL_TRIANGLES
    renderShape(shaderProgram, GL_LINE_LOOP, 0.0f, 1.0f, 0.0f); // Verde como GL_LINE_LOOP
    renderShape(shaderProgram, GL_POINTS, 0.0f, 0.0f, 1.0f);    // Azul como GL_POINTS
}

int main() {
    GLFWwindow* window = nullptr;

    if (!initGLFW(window)) {
        return -1;
    }

    if (!initGLAD()) {
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 shapeColor;
        void main() {
            FragColor = vec4(shapeColor, 1.0);
        }
    )";

    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //exA(shaderProgram, 1.0f, 0.0f, 0.0f);
        //exB(shaderProgram, 1.0f, 0.0f, 0.0f);
        //exC(shaderProgram, 1.0f, 0.0f, 0.0f);
        exD(shaderProgram, 1.0f, 0.0f, 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}