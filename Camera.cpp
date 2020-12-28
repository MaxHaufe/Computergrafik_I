#include "Camera.h"

glm::vec3 Camera::getViewPos() const
{
    return viewPos;
}

void Camera::setViewPos(glm::vec3 viewPos)
{
    this->viewPos = viewPos;
}

glm::vec3 Camera::getViewDir() const
{
    return viewDir;
}

void Camera::setViewDir(glm::vec3 viewDir)
{
    this->viewDir = viewDir;
}

glm::vec3 Camera::getViewUp() const
{
    return viewUp;
}

void Camera::setViewUp(glm::vec3 viewUp)
{
    this->viewUp = viewUp;
}