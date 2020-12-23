#pragma once

#include <glm.hpp>
#include <glew.h>
#include <vector>

#ifndef SHAPE_H
#define SHAPE_H

class Shape {
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> normals;
	glm::vec3 singleColor = glm::vec3(0.5f, 0.5f, 0.5f);
public:
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);

	struct Material {
		glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		float shininess = 56.0f;
	};

	Material material;

	Shape() { };
	void setColor(glm::vec3 color);
	void draw(bool filled = true, GLuint drawingMode = GL_TRIANGLES);
	void pushMatrices();
	void pushMaterial();
};

#endif;