#ifndef BALL_H
#define BALL_H

#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>

class Block;

class Ball {
public:
    Ball(float radius, glm::vec2 initialPosition, glm::vec2 velocity);
    ~Ball();

    void move(float deltaTime);
    void moveFirst(glm::vec2 direction, float deltaTime);
    void moveCollision(Block& block);
    void draw();

    glm::vec2 getPosition() const;
    float getRadius() const;
    glm::vec2 velocity;
private:
    GLuint VAO, VBO;
    glm::vec2 position;
    float radius;
};

#endif