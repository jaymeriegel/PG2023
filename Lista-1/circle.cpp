#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Largura e altura da janela
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Função para inicializar o GLFW e a janela
bool initGLFW(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar o GLFW" << std::endl;
        return false;
    }

    // Configurações do GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Círculo OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

// Função para criar um círculo usando a equação paramétrica
std::vector<float> createCircleVertices(float radius, int numSegments) {
    const float PI = 3.14159265359f;
    std::vector<float> vertices;

    for (int i = 0; i < numSegments; ++i) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);

        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}

// Função para criar um programa de shader
GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Compilar e vincular shaders
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

    // Verificar erros de ligação
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar o shader program: " << infoLog << std::endl;
        return 0;
    }

    // Limpar shaders intermediários
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    GLFWwindow* window = nullptr;

    if (!initGLFW(window)) {
        return -1;
    }

    // Inicializar o GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar o GLAD" << std::endl;
        return -1;
    }

    // Configuração do OpenGL
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Criar um círculo com equação paramétrica
    float radius = 0.5f;
    int numSegments = 8;
    std::vector<float> circleVertices = createCircleVertices(radius, numSegments);

    // Vertex Shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        }
    )";

    // Fragment Shader
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 circleColor; // Cor do círculo (RGB)
        void main() {
            FragColor = vec4(circleColor, 1.0);
        }
    )";

    // Criar e vincular o programa de shader
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // Criar VAO e VBO para o círculo
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    // Atributos de vértice
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Cor do círculo
    GLint circleColorLocation = glGetUniformLocation(shaderProgram, "circleColor");
    if (circleColorLocation != -1) {
        glUniform3f(circleColorLocation, 1.0f, 0.0f, 0.0f); // Vermelho (RGB)
    }

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
