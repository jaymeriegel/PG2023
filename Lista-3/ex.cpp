#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int numRows = 10;
const int numCols = 10;
std::vector<glm::vec3> randomColors;
const int numQuadrados = numRows * numCols;
glm::vec3 squarePosition(0.0f, 0.0f, 0.0f);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_W: // Move para cima
                squarePosition.y += 1.0f;
                break;
            case GLFW_KEY_S: // Move para baixo
                squarePosition.y -= 1.0f;
                break;
            case GLFW_KEY_A: // Move para a esquerda
                squarePosition.x -= 1.0f;
                break;
            case GLFW_KEY_D: // Move para a direita
                squarePosition.x += 1.0f;
                break;
        }
    }
}

glm::vec3 getRandomColor() {
    float r = static_cast<float>(rand()) / RAND_MAX;
    float g = static_cast<float>(rand()) / RAND_MAX;
    float b = static_cast<float>(rand()) / RAND_MAX;
    return glm::vec3(r, g, b);
}

void populateRandomColors() {
    const int numQuadrados = numRows * numCols;
    randomColors.reserve(numQuadrados);
    for (int i = 0; i < numQuadrados; ++i) {
        randomColors.push_back(getRandomColor());
    }
}


bool initGLFW(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar o GLFW" << std::endl;
        return false;
    }

    populateRandomColors();

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

void renderShape(GLuint shaderProgram, GLenum primitiveType, float red, float green, float blue, const glm::mat4& modelMatrix) {
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

    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
    GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

    GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, red, green, blue);

    glDrawArrays(primitiveType, 0, 6);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void renderColoredSquareGrid(GLuint shaderProgram) {
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
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

    

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(col * 1.0f, row * 1.0f, 0.0f));

            glm::vec3 randomColor = randomColors[row * numCols + col];
            glUniform3f(glGetUniformLocation(shaderProgram, "shapeColor"), randomColor.x, randomColor.y, randomColor.z);
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
            GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }

}

void renderSceneInQuadrants(GLuint shaderProgram) {
    // Renderize o primeiro quadrante (superior direito)
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::scale(model1, glm::vec3(5.0f, 5.0f, 1.0f)); // Escala 2x em x e y
    glViewport(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 0.0f, 0.0f, model1);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(-2.0f, 0.0f, 0.0f));
    glViewport(0, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 0.0f, 1.0f, 0.0f, model2);

    glm::mat4 model3 = glm::mat4(1.0f);
    model3 = glm::rotate(model3, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotação de 45 graus em torno do eixo z
    glViewport(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    renderShape(shaderProgram, GL_TRIANGLES, 0.0f, 0.0f, 1.0f, model3);
}

void ex1(GLuint shaderProgram) {
    renderSceneInQuadrants(shaderProgram);
}

void ex2(GLuint shaderProgram) {
    renderColoredSquareGrid(shaderProgram);
}

void ex3(GLuint shaderProgram) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, squarePosition);
    renderShape(shaderProgram, GL_TRIANGLES, 1.0f, 1.0f, 1.0f, model);
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
        uniform mat4 model;
        void main() {
            gl_Position = projection * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
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

    glfwSetKeyCallback(window, keyCallback);
    

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //ex1(shaderProgram);
        //ex2(shaderProgram);
        ex3(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}