#pragma once
#include <glm.hpp>
#include <glew.h>
#include <math.h>
class Camera
{
private:
	glm::vec3 viewPos = glm::vec3(5.0f, 3.5f, 10.0f);
	glm::vec3 viewDir = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
  
public:

    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightLocation =  glm::vec3(0.0f, 0.0f, -length(viewPos));    //origin
    bool pointLightEnabled = true;

    glm::vec3 getViewPos() const;
    void setViewPos(glm::vec3 viewPos);

    glm::vec3 getViewDir() const;
    void setViewDir(glm::vec3 viewDir);

    glm::vec3 getViewUp() const;
    void setViewUp(glm::vec3 viewUp);
};

