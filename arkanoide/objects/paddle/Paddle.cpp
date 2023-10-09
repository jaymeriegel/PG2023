#include "Paddle.h"
#include <commons/Shader.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

Paddle::Paddle(float width, float height, float initialX) {
    this->width = width;
    this->height = height;
    this->position = glm::vec2(initialX, -0.9f);
    this->speed = 2.0f;

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

Paddle::~Paddle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Paddle::moveLeft(float deltaTime) {
    if (position.x > -0.7f) {
        position.x -= speed * deltaTime;
    }
}

void Paddle::moveRight(float deltaTime) {
    if (position.x + width < 0.8) {
        position.x += speed * deltaTime;
    }
}

void Paddle::draw() {
    Shader shader("/home/jayme/College/processamento-grafico/PG2023/arkanoide/objects/paddle/paddle.vs", "/home/jayme/College/processamento-grafico/PG2023/arkanoide/objects/paddle/paddle.fs");
    
    shader.Use();

    glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    shader.setMat4("projection", glm::value_ptr(projection));
    shader.setMat4("model", glm::value_ptr(model));
    shader.setVec3("color", 1.0f, 0.0f, 0.0f); // Cor vermelha

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

float Paddle::getX() const {
    return position.x;
}

float Paddle::getY() const {
    return position.y;
}

float Paddle::getWidth() const {
    return width;
}

float Paddle::getHeight() const {
    return height;
}

glm::vec2 Paddle::getPosition() const {
    return position;
}