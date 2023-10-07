#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

bool initGLFW(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar o GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jayme-Lista2", nullptr, nullptr);
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

void renderShape(GLuint shaderProgram, GLenum primitiveType, float red, float green, float blue, const glm::mat4& projectionMatrix) {
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

    GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projectionMatrix));

    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    float scaleValue = 10.0f;

    GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
    glUniform1f(scaleLocation, scaleValue);


    glDrawArrays(primitiveType, 0, 6);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void renderSceneInQuadrants(GLuint shaderProgram, const glm::mat4& customProjection) {
    // Renderize o primeiro quadrante (superior direito)
    glViewport(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f, customProjection);

    // Renderize o segundo quadrante (superior esquerdo)
    glViewport(0, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 0.0f, 1.0f, 0.0f, customProjection);

    // Renderize o terceiro quadrante (inferior esquerdo)
    glViewport(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 0.0f, 0.0f, 1.0f, customProjection);

    // Renderize o quarto quadrante (inferior direito)
    glViewport(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 1.0f, 0.0f, customProjection);
}

void ex1(GLuint shaderProgram) {
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f, projection);
}

void ex2(GLuint shaderProgram) {
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f, projection);
}

void ex4(GLuint shaderProgram) {
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f, projection);
}

void ex5(GLuint shaderProgram) {
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

    renderSceneInQuadrants(shaderProgram, projection);
}

int main() {
    GLFWwindow* window = nullptr;

    if (!initGLFW(window)) {
        return -1;
    }

    if (!initGLAD()) {
        return -1;
    }

    glViewport(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 projection;
        uniform float scale; // Adicione um uniform para a escala
        void main() {
            // Aplique a escala diretamente às coordenadas aPos
            vec2 scaledPosition = aPos * scale;
            gl_Position = projection * vec4(scaledPosition.x, scaledPosition.y, 0.0, 1.0);
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

        //ex1(shaderProgram);
        //ex2(shaderProgram);
        //ex4(shaderProgram);
        ex5(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}