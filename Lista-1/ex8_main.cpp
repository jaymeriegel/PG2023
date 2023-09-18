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

void renderShape(GLuint shaderProgram, std::vector<float> vertices, GLenum primitiveType, std::vector<GLuint> indices) {

    
    GLuint VAO, VBO, EBO; 
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint colorAttrib = glGetAttribLocation(shaderProgram, "inColor");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    glDrawElements(primitiveType, indices.size(), GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
    
    // Vetores de vértices para o triângulo e pontos
    std::vector<float> triangleVertices = {
        // Posições (x, y, z) e cores (r, g, b)
        0.0, 0.5, 0.0, 0.0, 0.0, 1.0,  // P1 (Vermelho)
        -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, // P2 (Verde)
        0.5, -0.5, 0.0, 0.0, 0.0, 1.0  // P3 (Azul)
    };

    std::vector<float> pointVertices = {
        // Posições (x, y, z) e cores (r, g, b)
        0.0, 0.5, 0.0, 1.0, 0.0, 0.0,  // P1 (Vermelho)
        -0.5, -0.5, 0.0, 0.0, 1.0, 0.0, // P2 (Verde)
        0.5, -0.5, 0.0, 0.0, 0.0, 1.0  // P3 (Azul)
    };

    std::vector<GLuint> triangleIndices = {0, 1, 2};
    std::vector<GLuint> pointIndices = {0, 1, 2}; 

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glPointSize(10.0);
        renderShape(shaderProgram, pointVertices, GL_POINTS, pointIndices);
        renderShape(shaderProgram, triangleVertices, GL_LINE_LOOP, triangleIndices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}