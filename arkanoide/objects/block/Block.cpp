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

template <typename T>
constexpr const T& custom_clamp(const T& value, const T& minValue, const T& maxValue) {
    return (value < minValue) ? minValue : ((value > maxValue) ? maxValue : value);
}

bool Block::checkCollision(Ball& ball) {
    // Calculate the closest point on the rectangle to the ball
    float closestX = custom_clamp(ball.getPosition().x, position.x, position.x + width);
    float closestY = custom_clamp(ball.getPosition().y, position.y, position.y + height);

    // Calculate the distance between the ball's center and the closest point on the rectangle
    float distanceX = ball.getPosition().x - closestX;
    float distanceY = ball.getPosition().y - closestY;

    // Calculate the squared distance (avoiding square root for performance)
    float squaredDistance = (distanceX * distanceX) + (distanceY * distanceY);

    // If the squared distance is less than the squared radius of the ball, there is a collision
    return squaredDistance <= (ball.getRadius() * ball.getRadius());
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