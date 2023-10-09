#ifndef PADDLE_H
#define PADDLE_H

#include <glm/glm.hpp>
#include <glad/glad.h>

class Paddle {
public:
    Paddle(float width, float height, float initialX);
    ~Paddle();

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    void draw();

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    glm::vec2 getPosition() const;

private:
    GLuint VAO, VBO;
    glm::vec2 position;
    float width, height;
    float speed;
};

#endif