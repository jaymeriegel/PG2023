#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

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


// Função para criar um círculo usando a equação paramétrica
std::vector<float> createCircleVerticesToTriangle(float radius, int numSegments) {
    const float PI = 3.14159265359f;
    std::vector<float> vertices;

    float theta, x, y;

    for (int i = 0; i < 2; ++i) {
        theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        x = radius * std::cos(theta);
        y = radius * std::sin(theta);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);


    for (int i = 2; i < numSegments + 1; ++i) {
        theta = 2.0f * PI * static_cast<float>(i - 1) / static_cast<float>(numSegments);
        x = radius * std::cos(theta);
        y = radius * std::sin(theta);
        vertices.push_back(x);
        vertices.push_back(y);

        theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        x = radius * std::cos(theta);
        y = radius * std::sin(theta);
        vertices.push_back(x);
        vertices.push_back(y);

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
    }

    return vertices;
}

std::vector<float> createCircleVerticesToStar() {
    std::vector<float> verticesIn = createCircleVertices(0.2f, 5);
    std::vector<float> verticesOut = createCircleVerticesToTriangle(0.5f, 10);
    std::vector<float> vertices;

    float x, y;
    int init = 0;
    int initOut = 2;
    for (int i = 0; i<5; i++) {
        if (i == 4) {
            vertices.push_back(verticesIn[init]);
            vertices.push_back(verticesIn[init + 1]);
            vertices.push_back(verticesIn[0]);
            vertices.push_back(verticesIn[1]);
            init = init + 2;

            vertices.push_back(verticesOut[initOut]);
            vertices.push_back(verticesOut[initOut + 1]);
            initOut = initOut + 12;
        } else {
            vertices.push_back(verticesIn[init]);
            vertices.push_back(verticesIn[init + 1]);
            vertices.push_back(verticesIn[init + 2]);
            vertices.push_back(verticesIn[init + 3]);
            init = init + 2;

            vertices.push_back(verticesOut[initOut]);
            vertices.push_back(verticesOut[initOut + 1]);
            initOut = initOut + 12;
        }
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << vertices[i];
        
        // Adiciona um hífen após cada par de elementos (exceto no último elemento)
        if (i % 2 == 1 && i != vertices.size() - 1) {
            std::cout << " - ";
        } else {
            std::cout << " ";
        }
    }

    std::cout << "Estamos no final!" << std::endl;

    return vertices;
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


void renderShape(GLuint shaderProgram, int numSegments, float red, float green, float blue) {
    float radius = 0.2f;
    std::vector<float> circleVertices = createCircleVertices(radius, numSegments);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void renderPacman(GLuint shaderProgram, int numSegments, float red, float green, float blue) {
    float radius = 0.5f;
    std::vector<float> circleVertices = createCircleVerticesToTriangle(radius, numSegments);    

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    
    glDrawArrays(GL_TRIANGLES, 0, (numSegments * 3) - 9);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void renderPizza(GLuint shaderProgram, int numSegments, float red, float green, float blue) {
    float radius = 0.5f;
    std::vector<float> circleVertices = createCircleVerticesToTriangle(radius, numSegments);    

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    
    glDrawArrays(GL_TRIANGLES, 0, (numSegments * 3) - 45);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void renderStar(GLuint shaderProgram, int numSegments, float red, float green, float blue) {
    float radius = 0.5f;
    std::vector<float> vertices = createCircleVerticesToStar();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    
    glDrawArrays(GL_TRIANGLES, 0, 5 * 3);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void exA(GLuint shaderProgram) {
    renderShape(shaderProgram, 8, 1.0f, 0.0f, 0.0f);
}

void exB(GLuint shaderProgram) {
    renderShape(shaderProgram, 5, 1.0f, 0.0f, 0.0f);
}

void exC(GLuint shaderProgram) {
    renderPacman(shaderProgram, 16, 1.0f, 0.0f, 0.0f);
}

void exD(GLuint shaderProgram) {
    renderPizza(shaderProgram, 16, 1.0f, 0.0f, 0.0f);
}

void exE(GLuint shaderProgram) {
    renderStar(shaderProgram, 10, 1.0f, 1.0f, 1.0f);
    renderShape(shaderProgram, 5, 1.0f, 1.0f, 1.0f);
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

        //exA(shaderProgram);
        //exB(shaderProgram);
        //exC(shaderProgram);
        //exD(shaderProgram);
        exE(shaderProgram);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}