#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

void renderShape(GLuint shaderProgram, std::vector<float> vertices, GLenum primitiveType,int nOfPoints) {
    GLuint VAO, VBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "inColor");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    glDrawArrays(primitiveType, 0, nOfPoints);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

        in vec3 inPosition;  // Atributo de posição
        in vec3 inColor;     // Atributo de cor

        out vec3 fragColor;  // Saída para o fragment shader

        void main() {
            gl_Position = vec4(inPosition, 1.0);
            fragColor = inColor;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core

        in vec3 fragColor;  // Entrada do vertex shader
        out vec4 outColor;  // Saída para a tela

        void main() {
            outColor = vec4(fragColor, 1.0);
        }
    )";

    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    
    
    std::vector<float> floor = {
        -0.9, 0.0, 0.0, 1.0, 0.5, 0.0,
        0.0, 0.0, 0.0, 1.0, 0.5, 0.0,
        0.9, 0.0, 0.0, 1.0, 0.5, 0.0
    };

    std::vector<float> house = {
        -0.2, 0.0, 0.0, 0.0, 0.0, 0.0,
        -0.2, 0.5, 0.0, 0.0, 0.0, 0.0,
        0.2, 0.5, 0.0, 0.0, 0.0, 0.0,
        0.2, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    std::vector<float> roof = {
        -0.2, 0.5, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.8, 0.0, 1.0, 0.0, 0.0,
        0.2, 0.5, 0.0, 1.0, 0.0, 0.0,
    };

    std::vector<float> roofLines = {
        -0.2, 0.5, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.8, 0.0, 0.0, 0.0, 0.0,
        0.2, 0.5, 0.0, 0.0, 0.0, 0.0,
    };

    std::vector<float> door = {
        -0.05, 0.0, 0.0, 0.5f, 0.2f, 0.0f,
        -0.05, 0.2, 0.0, 0.5f, 0.2f, 0.0f,
        0.05, 0.0, 0.0, 0.5f, 0.2f, 0.0f,
        0.05, 0.0, 0.0, 0.5f, 0.2f, 0.0f,
        -0.05, 0.2, 0.0, 0.5f, 0.2f, 0.0f,
        0.05, 0.2, 0.0, 0.5f, 0.2f, 0.0f
    };

    std::vector<float> doorLine = {
        -0.05, 0.0, 0.0, 0.0f, 0.0f, 0.0f,
        -0.05, 0.2, 0.0, 0.0f, 0.0f, 0.0f,
        0.05, 0.2, 0.0, 0.0f, 0.0f, 0.0f,
        0.05, 0.0, 0.0, 0.0f, 0.0f, 0.0f
    };

    std::vector<float> windowHouse = {
        -0.1, 0.28, 0.0, 1.0f, 1.0f, 0.0f,
        -0.1, 0.38, 0.0, 1.0f, 1.0f, 0.0f,
        0.0, 0.28, 0.0, 1.0f, 1.0f, 0.0f,
        -0.1, 0.38, 0.0, 1.0f, 1.0f, 0.0f,
        0.0, 0.38, 0.0, 1.0f, 1.0f, 0.0f,
        0.0, 0.28, 0.0, 1.0f, 1.0f, 0.0f
    };

    std::vector<float> windowSquareLine = {
        -0.1, 0.28, 0.0, 0.0f, 0.0f, 0.0f,
        -0.1, 0.38, 0.0, 0.0f, 0.0f, 0.0f,
        0.0, 0.38, 0.0, 0.0f, 0.0f, 0.0f,
        0.0, 0.28, 0.0, 0.0f, 0.0f, 0.0f
    };

    std::vector<float> windowLineHorizontal = {
        -0.1, 0.33, 0.0, 0.0f, 0.0f, 0.0f,
        0.0, 0.33, 0.0, 0.0f, 0.0f, 0.0f
    };

    std::vector<float> windowLineVertical = {
        -0.05, 0.38, 0.0, 0.0f, 0.0f, 0.0f,
        -0.05, 0.28, 0.0, 0.0f, 0.0f, 0.0f
    };


    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(5.0);
        renderShape(shaderProgram, floor, GL_LINE_STRIP, 3);
        renderShape(shaderProgram, house, GL_LINE_LOOP, 4);
        renderShape(shaderProgram, roof, GL_TRIANGLES, 3);
        renderShape(shaderProgram, roofLines, GL_LINE_LOOP, 3);
        renderShape(shaderProgram, door, GL_TRIANGLES, 6);
        renderShape(shaderProgram, doorLine, GL_LINE_STRIP, 4);
        renderShape(shaderProgram, windowHouse, GL_TRIANGLES, 6);
        renderShape(shaderProgram, windowSquareLine, GL_LINE_LOOP, 4);
        renderShape(shaderProgram, windowLineHorizontal, GL_LINE_STRIP, 2);
        renderShape(shaderProgram, windowLineVertical, GL_LINE_STRIP, 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}