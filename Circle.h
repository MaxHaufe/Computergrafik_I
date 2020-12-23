#pragma once

#include <glm.hpp>
#include <glew.h>
#include <vector>
#include "Shape.h"

#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
private:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
public:
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);
	Circle(float radius = 1.0f, unsigned int segments = 100);
	void draw(glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f));
	void pushMatrices();
};

//class Circle : public Shape {
//public:
//	Circle(float radius = 1.0f, unsigned int segments = 50);
//};

#endif;