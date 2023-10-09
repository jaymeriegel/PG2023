#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Paddle.h"
#include "Block.h"
#include "Ball.h"
#include <glm/gtc/type_ptr.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
float blockWidth = 0.15f;
float blockHeight = 0.1f;
int numRows = 5;
int numCols = 8;
std::vector<glm::vec3> randomColors;
std::vector<Block> disabledBlocks;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Função para inicializar o GLFW e criar a janela
bool initGLFW(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar o GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Arkanoide", nullptr, nullptr);
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

glm::vec3 getRandomColor() {
    float r = static_cast<float>(rand()) / RAND_MAX;
    float g = static_cast<float>(rand()) / RAND_MAX;
    float b = static_cast<float>(rand()) / RAND_MAX;
    return glm::vec3(r, g, b);
}

void populateRandomColors() {
    randomColors.reserve(numRows);
    for (int i = 0; i < numRows; ++i) {
        randomColors.push_back(getRandomColor());
    }
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

std::vector<Block> drawBlocks() {
    std::vector<Block> activeBlocks;
    for (int i = 0; i < numRows; ++i) {
        glm::vec3 randomColor = randomColors[i];
        for (int j = 0; j < numCols; ++j) {
            float x = -0.65f + j * (blockWidth + 0.02f);  // Espaçamento entre os blocos: 0.02f
            float y = 0.8f - i * (blockHeight + 0.02f);  // Espaçamento entre os blocos: 0.02f
            Block block(blockWidth, blockHeight, x, y);
            block.draw(randomColor.x, randomColor.y, randomColor.z);
            activeBlocks.push_back(block);
        }
    }
    return activeBlocks;
}

void drawContour() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * vec4(aPos.x, aPos.y, 0.0, 1.0);
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

    float vertices[] = {
        -0.8f, -1.0f,
        -0.8f, 0.9f,
         0.7f, 0.9f,
         0.7f, -1.0f
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

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

    GLint shapeColorLocation = glGetUniformLocation(shaderProgram, "shapeColor");
    glUniform3f(shapeColorLocation, 0.0f, 0.0f, 1.0f);

    glLineWidth(3.0f);

    glDrawArrays(GL_LINE_STRIP, 0, 4);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

bool checkCollisionPaddle(Ball& ball, Paddle& paddle) {
    // Obtenha informações do paddle
    float paddleX = paddle.getPosition().x;
    float paddleY = paddle.getPosition().y;
    float paddleWidth = paddle.getWidth();
    float paddleHeight = paddle.getHeight();

    // Obtenha informações da bola
    glm::vec2 ballPosition = ball.getPosition();
    float ballRadius = ball.getRadius();

    // Verifique a colisão
    if (ballPosition.x + ballRadius > paddleX - paddleWidth / 2.0f &&
        ballPosition.x - ballRadius < paddleX + paddleWidth / 2.0f &&
        ballPosition.y + ballRadius > paddleY - paddleHeight / 2.0f &&
        ballPosition.y - ballRadius < paddleY + paddleHeight / 2.0f) {
        // A colisão ocorreu, inverta a componente y da velocidade da bola para fazê-la quicar para cima
        glm::vec2 ballVelocity = ball.velocity;
        ballVelocity.y = -ballVelocity.y;
        ball.velocity = ballVelocity;
        return true; // Houve colisão
    }

    return false; // Não houve colisão
}

void verifyCollisionBlocks(std::vector<Block>& blocks, Ball& ball) {
    // Loop através de todos os blocos
    for (auto& block : blocks) {
        // Verifique se o bloco não está desativado
        if (block.checkCollision(ball)) { 
            ball.moveCollision(block);
            break;
        }
    }
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    populateRandomColors();
    
    Paddle paddle(0.2f, 0.02f, 0.0f);
    Ball ball(0.02f, glm::vec2(0.0f, -0.85f), glm::vec2(0.5f, 0.5f));

    bool gameStarted = false; // Variável para controlar se o jogo começou
    int first = 0;

    while (!glfwWindowShouldClose(window)) {
        // Verifique eventos
        glfwPollEvents();
    
        float deltaTime = 0.016f;

        // Verifique se a tecla Backspace foi pressionada para iniciar o jogo
        if (!gameStarted && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            first = 1;
            gameStarted = true; // Inicie o jogo
        }

        if (gameStarted) {
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                paddle.moveLeft(deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                paddle.moveRight(deltaTime);
            }

            // Limpe a tela
            glClear(GL_COLOR_BUFFER_BIT);
            drawContour();
            paddle.draw();
            std::vector<Block> activeBlocks = drawBlocks();


            if (first == 1) {
                ball.moveFirst(glm::vec2(1.0f, 1.0f), deltaTime);
                first = 2;
            } else {
                ball.move(deltaTime);
            }

            if (ball.getPosition().y < -1.0f) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            verifyCollisionBlocks(activeBlocks, ball); 
            checkCollisionPaddle(ball, paddle);    
            
            ball.draw();
        } else {
            drawContour();
            paddle.draw();
            drawBlocks();
            ball.draw();
        }

    glfwSwapBuffers(window);
}
    // Encerre o GLFW
    glfwTerminate();

    return 0;
}