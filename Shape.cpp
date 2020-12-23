//#include "GlobalVariables.h"
#include "Shape.h"
#include <gtx/transform.hpp>
#include "GlobalVariables.h"


void Shape::pushMatrices() {

	this->View = glm::lookAt(this->getViewPos(),this->getViewDir(),this->getViewUp());

	//uniform eye direc
	vec3 EyeDirection = viewPos - target;
	GLuint locColor = glGetUniformLocation(program, "EyeDirection");

	glUniform3fv(locColor, 1, &EyeDirection[0]);

	glm::mat3 NormalMatrix;
	glm::mat4 ModelView;
	glm::mat4 ModelViewProjection = glm::mat4(1.0f);
	
	ModelViewProjection = this->Projection * this->View * this->Model;
	ModelView = this->View * this->Model;
	for (int i = 0;i < 3;i++) for (int j = 0;j < 3;j++) NormalMatrix[i][j] = ModelView[i][j];

	GLuint Location = glGetUniformLocation(program, "ModelViewProjection");
	glUniformMatrix4fv(Location, 1, GL_FALSE, &ModelViewProjection[0][0]);

	GLuint locMV = glGetUniformLocation(program, "MVMatrix");
	GLuint locNM = glGetUniformLocation(program, "NormalMatrix");

	glUniformMatrix4fv(locMV, 1, GL_FALSE, &ModelView[0][0]);
	glUniformMatrix3fv(locNM, 1, GL_FALSE, &NormalMatrix[0][0]);
}

void Shape::pushMaterial() {
	GLuint locAmb = glGetUniformLocation(program, "material.ambient");
	glUniform3fv(locAmb, 1, &this->material.ambient[0]);
	GLuint locDif = glGetUniformLocation(program, "material.diffuse");
	glUniform3fv(locDif, 1, &this->material.diffuse[0]);
	GLuint locSpec = glGetUniformLocation(program, "material.shininess");
	glUniform1f(locSpec, this->material.shininess);
}

void Shape::draw(bool filled, GLuint drawingMode) {

	Shape::pushMaterial();
	Shape::pushMatrices();

	glBindVertexArray(VAO[VAOCube]);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->normals.size(), &this->normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(VertexNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(VertexNormal);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->colors.size(), &this->colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(vColor);

	if (filled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glDrawArrays(drawingMode, 0, this->vertices.size() / 3);
}

void Shape::setColor(glm::vec3 color) {
	this->colors.clear();
	for (int i = 0;i < vertices.size();i += 3) {
		this->colors.push_back(color.x);
		this->colors.push_back(color.y);
		this->colors.push_back(color.z);
	}
}