#pragma once
#include <glm.hpp>
#include <glew.h>
class Camera
{
private:
	glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
public:
    glm::vec3 getViewPos() const;
    void setViewPos(glm::vec3 viewPos);

    glm::vec3 getViewDir() const;
    void setViewDir(glm::vec3 viewDir);

    glm::vec3 getViewUp() const;
    void setViewUp(glm::vec3 viewUp);
};

