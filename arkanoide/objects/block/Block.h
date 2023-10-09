#ifndef Block_H
#define Block_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Ball.h"

class Ball;

class Block {
public:
    Block(float width, float height, float initialX, float initialY);
    ~Block();

    void draw(float r, float g, float b);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    glm::vec2 getPosition() const;
    bool checkCollision(Ball& ball);

private:
    GLuint VAO, VBO;
    glm::vec2 position;
    float width, height;
};

#endif