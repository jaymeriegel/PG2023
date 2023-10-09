#include "Ball.h"
#include "Block.h" 
#include <commons/Shader.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Ball::Ball(float radius, glm::vec2 initialPosition, glm::vec2 velocity) {
    this->radius = radius;
    this->position = initialPosition;
    this->velocity = velocity;

    int numSegments = 30;
    float angleIncrement = 2 * glm::pi<float>() / static_cast<float>(numSegments);

    std::vector<float> vertices;
    vertices.reserve(2 * (numSegments + 1));
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = static_cast<float>(i) * angleIncrement;
        float x = radius * glm::cos(angle);
        float y = radius * glm::sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Ball::~Ball() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Ball::moveFirst(glm::vec2 direction, float deltaTime) {
    position += velocity * direction * deltaTime;
}

void Ball::move(float deltaTime) {
    // Calcula a nova posição da bola com base na velocidade e no tempo delta
    glm::vec2 newPosition = position + velocity * deltaTime;

    // Verifica se a nova posição ultrapassa o limite da parede esquerda
    if (newPosition.x - radius < -0.8f) {
        // A bola atingiu a parede esquerda, inverte o componente x da velocidade
        velocity.x = -velocity.x;
        newPosition.x = -0.8f + radius; // Define a posição para evitar que a bola fique presa na parede
    }

    // Verifica se a nova posição ultrapassa o limite da parede direita
    if (newPosition.x + radius > 0.7f) {
        // A bola atingiu a parede direita, inverte o componente x da velocidade
        velocity.x = -velocity.x;
        newPosition.x = 0.7f - radius; // Define a posição para evitar que a bola fique presa na parede
    }

    // Verifica se a nova posição ultrapassa o limite do teto
    if (newPosition.y + radius > 0.9f) {
        // A bola atingiu o teto, inverte o componente y da velocidade
        velocity.y = -velocity.y;
        newPosition.y = 0.9f - radius; // Define a posição para evitar que a bola fique presa no teto
    }

    // Atualize a posição da bola
    position = newPosition;
}

void Ball::moveCollision(Block& block) {
    // Calculate the collision direction vector from the ball's center to the block's center
    glm::vec2 ballCenter = position + glm::vec2(radius, radius);
    glm::vec2 blockCenter = block.getPosition() + glm::vec2(block.getWidth() / 2.0f, block.getHeight() / 2.0f);
    glm::vec2 collisionDirection = glm::normalize(ballCenter - blockCenter);

    // Calculate the dimensions and edges of the block
    float blockX = block.getX();
    float blockY = block.getY();
    float blockWidth = block.getWidth();
    float blockHeight = block.getHeight();
    float blockLeftEdge = blockX;
    float blockRightEdge = blockX + blockWidth;
    float blockTopEdge = blockY;
    float blockBottomEdge = blockY + blockHeight;

    // Calculate the collision point on the block
    float collisionPointX = position.x + radius * collisionDirection.x;
    float collisionPointY = position.y + radius * collisionDirection.y;

    // Check which side of the block was hit
    bool hitLeft = collisionPointX < blockLeftEdge;
    bool hitRight = collisionPointX > blockRightEdge;
    bool hitTop = collisionPointY < blockTopEdge;
    bool hitBottom = collisionPointY > blockBottomEdge;

    // Adjust the ball's velocity based on the side of the block hit
    if (hitLeft || hitRight) {
        velocity.x = -velocity.x; // Invert the x-direction velocity
    }

    if (hitTop || hitBottom) {
        velocity.y = -velocity.y; // Invert the y-direction velocity
    }

    // Move the ball out of the block to avoid multiple collisions
    if (hitLeft) {
        position.x = blockLeftEdge - radius;
    } else if (hitRight) {
        position.x = blockRightEdge + radius;
    }

    if (hitTop) {
        position.y = blockTopEdge - radius;
    } else if (hitBottom) {
        position.y = blockBottomEdge + radius;
    }
}



void Ball::draw() {
    Shader shader("/home/jayme/College/processamento-grafico/PG2023/arkanoide/objects/ball/ball.vs", "/home/jayme/College/processamento-grafico/PG2023/arkanoide/objects/ball/ball.fs");
    
    shader.Use();

    glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    shader.setMat4("projection", glm::value_ptr(projection));
    shader.setMat4("model", glm::value_ptr(model));
    shader.setVec3("color", 0.0f, 0.0f, 1.0f);

    int numSegments = 30;
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);
    glBindVertexArray(0);
}

glm::vec2 Ball::getPosition() const {
    return position;
}

float Ball::getRadius() const {
    return radius;
}