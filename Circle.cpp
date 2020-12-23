#include "Circle.h"
#include <gtx/transform.hpp>
#include "GlobalVariables.h"
#define _USE_MATH_DEFINES
#include <math.h>

//Circle::Circle(float radius, unsigned int segments) {
//	float x1, y1, x2, y2, theta;
//	x1 = radius;
//	y1 = 0;
//	theta = (M_PI * 2) / segments;
//
//	for (int i = 0; i < segments*4; i+=4) {
//
//		GLfloat x2 = cos(theta) * x1 - sin(theta) * y1;
//		GLfloat y2 = sin(theta) * x1 + cos(theta) * y1;
//
//		this->vertices.push_back(x1);
//		this->vertices.push_back(y1);
//		this->vertices.push_back(x2);
//		this->vertices.push_back(y2);
//
//		x1 = x2;
//		y1 = y2;
//	}
//
//}
//
//void Circle::pushMatrices() {
//
//	this->View = glm::lookAt(this->getViewPos(), this->getViewDir(), this->getViewUp());
//
//	//uniform eye direc
//	glm::vec3 EyeDirection = this->getViewPos() - this->getViewDir();
//	GLuint locColor = glGetUniformLocation(program, "EyeDirection");
//
//	glUniform3fv(locColor, 1, &EyeDirection[0]);
//
//	glm::mat3 NormalMatrix;
//	glm::mat4 ModelView;
//	glm::mat4 ModelViewProjection = glm::mat4(1.0f);
//
//	ModelViewProjection = this->Projection * this->View * this->Model;
//	ModelView = this->View * this->Model;
//	for (int i = 0;i < 3;i++) for (int j = 0;j < 3;j++) NormalMatrix[i][j] = ModelView[i][j];
//
//	GLuint Location = glGetUniformLocation(program, "ModelViewProjection");
//	glUniformMatrix4fv(Location, 1, GL_FALSE, &ModelViewProjection[0][0]);
//
//	GLuint locMV = glGetUniformLocation(program, "MVMatrix");
//	GLuint locNM = glGetUniformLocation(program, "NormalMatrix");
//
//	glUniformMatrix4fv(locMV, 1, GL_FALSE, &ModelView[0][0]);
//	glUniformMatrix3fv(locNM, 1, GL_FALSE, &NormalMatrix[0][0]);
//}
//
//void Circle::draw(glm::vec3 Color) {
//
//	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);		//setting ambient to full because 2D shape is not affected by light
//	GLuint locAmb = glGetUniformLocation(program, "material.ambient");
//	glUniform3fv(locAmb, 1, &ambient[0]);
//
//	Circle::pushMatrices();
//
//	this->colors.clear();
//	for (int i = 0;i < vertices.size()*1.5;i += 3) {
//		this->colors.push_back(Color.x);
//		this->colors.push_back(Color.y);
//		this->colors.push_back(Color.z);
//	}
//
//	glBindVertexArray(VAO[VAOCube]);
//	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexBuffer]);
//	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), &this->vertices[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	glEnableVertexAttribArray(vPosition);
//
//	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->colors.size(), &this->colors[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	glEnableVertexAttribArray(vColor);
//
//	glDrawArrays(GL_LINES, 0, this->vertices.size()/2);
//
//}

Circle::Circle(float radius, unsigned int segments) {
	float x1, y1, x2, y2, z, theta;
	x1 = radius;
	y1 = 0.0f;
	z = -10.0f;
	theta = (M_PI * 2) / segments;

	for (int i = 0; i < segments * 4; i += 4) {

		float x2 = cos(theta) * x1 - sin(theta) * y1;
		float y2 = sin(theta) * x1 + cos(theta) * y1;

		this->vertices.push_back(x1);
		this->vertices.push_back(y1);
		this->vertices.push_back(z);
		this->vertices.push_back(x2);
		this->vertices.push_back(y2);
		this->vertices.push_back(z);

		x1 = x2;
		y1 = y2;
	}
	this->normals = this->vertices;
	this->colors = this->vertices;

	//also make them "immune" to our light sources
	this->material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	this->material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	this->material.shininess = 3000.0f;
}