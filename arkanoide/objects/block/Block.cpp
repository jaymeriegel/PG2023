#include "Block.h"
#include <commons/Shader.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Ball.h"

Block::Block(float width, float height, float initialX, float initialY) {
    this->width = width;
    this->height = height;
    this->position = glm::vec2(initialX, initialY);

    // Crie os vértices do Paddle
    float vertices[] = {
        // Posições
        -width / 2.0f, -height / 2.0f,
         width / 2.0f, -height / 2.0f,
         width / 2.0f,  height / 2.0f,
         width / 2.0f,  height / 2.0f,
        -width / 2.0f,  height / 2.0f,
        -width / 2.0f, -height / 2.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Block::~Block() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Block::draw(float r, float g, float b) {
    Shader shader("/home/jayme/College/processamento-grafico/PG2023/arkanoide/objects/block/block.vs", "/home/jayme/College/processamento-grafico/PG2023/arkanoide/objects/block/block.fs");
    
    shader.Use();

    glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    shader.setMat4("projection", glm::value_ptr(projection));
    shader.setMat4("model", glm::value_ptr(model));
    shader.setVec3("color", r, g, b);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// Verifica se há colisão entre a bola e o bloco
bool Block::checkCollision(Ball& ball) {
    // Obtenha informações sobre a bola
    glm::vec2 ballPosition = ball.getPosition();
    float ballRadius = ball.getRadius();

    // Obtenha informações sobre o bloco
    float blockX = position.x;
    float blockY = position.y;
    float blockWidth = width;
    float blockHeight = height;

    // Calcule as coordenadas do centro do bloco
    float blockCenterX = blockX + blockWidth / 2.0f;
    float blockCenterY = blockY + blockHeight / 2.0f;

    // Calcule a distância entre o centro do bloco e o centro da bola
    float deltaX = ballPosition.x - std::max(blockX, std::min(ballPosition.x, blockX + blockWidth));
    float deltaY = ballPosition.y - std::max(blockY, std::min(ballPosition.y, blockY + blockHeight));

    // Verifique se houve uma colisão
    if ((deltaX * deltaX + deltaY * deltaY) < (ballRadius * ballRadius)) {
        return true; // Houve colisão
    }

    return false; // Não houve colisão
}


float Block::getX() const {
    return position.x;
}

float Block::getY() const {
    return position.y;
}

float Block::getWidth() const {
    return width;
}

float Block::getHeight() const {
    return height;
}

glm::vec2 Block::getPosition() const {
    return position;
}