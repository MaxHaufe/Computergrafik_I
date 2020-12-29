//#include "GlobalVariables.h"
#include "Shape.h"
#include <gtx/transform.hpp>
#include <FreeImage.h>
#include "GlobalVariables.h"

BYTE* bitmapBits;

void Shape::pushMatrices() {

	this->View = glm::lookAt(this->getViewPos(),this->getViewDir(),this->getViewUp());

	//uniform eye direc
	glm::vec3 EyeDirection = this->getViewPos() - this->getViewDir();
	GLuint locEye = glGetUniformLocation(program, "EyeDirection");

	glUniform3fv(locEye, 1, &EyeDirection[0]);

	//uniform light

	if (this->pointLightEnabled) {
		glm::vec3 pos = this->getViewPos();
		GLuint pointLightEnable = glGetUniformLocation(program, "pointLightEnable");
		glUniform1f(pointLightEnable, 1.0f);
		glm::vec3 lightLocation = glm::vec3(0.0f, 0.0f, -length(this->getViewPos()));			//make light source be in the origin (sun)
		this->lightLocation = lightLocation;
	}
	else {
		GLuint pointLightEnable = glGetUniformLocation(program, "pointLightEnable");
		glUniform1f(pointLightEnable, 0.0f);
	}

	GLuint locColor = glGetUniformLocation(program, "LightColor");
	GLuint locLight = glGetUniformLocation(program, "LightPosition");

	glUniform3fv(locColor, 1, &this->lightColor[0]);
	glUniform3fv(locLight, 1, &this->lightLocation[0]);

	//MVP and stuff

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
	GLuint locSpecVec = glGetUniformLocation(program, "material.specular");
	glUniform3fv(locSpecVec, 1, &this->material.specular[0]);
	GLuint locSpec = glGetUniformLocation(program, "material.shininess");
	glUniform1f(locSpec, this->material.shininess);
}

void Shape::draw(bool filled, GLuint drawingMode) {

	if (this->textureEnabled) {
		GLuint texEnableLoc = glGetUniformLocation(program, "textureEnabled");
		glUniform1f(texEnableLoc, this->textureEnabled);

		glEnable(GL_TEXTURE_2D);
		//bind texture
		glBindTexture(GL_TEXTURE_2D, this->image.tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->image.bitmapWidth, this->image.bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, this->image.bitmapBits);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);
	}
	else {
		GLuint texEnableLoc = glGetUniformLocation(program, "textureEnabled");
		glUniform1f(texEnableLoc, this->textureEnabled);
	}

	//push Matrices and Material
	Shape::pushMaterial();
	Shape::pushMatrices();

	//bind Arrays
	glBindVertexArray(VAO[VAOCube]);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[TextureBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->texCoords.size(), &this->texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(vTexCoord);

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

	//unbind everything - troubleshooting
	//helped fixing the issue of the depth buffer acting weird
	glDisableVertexArrayAttrib(VAOCube, vPosition);
	glDisableVertexArrayAttrib(VAOCube, VertexNormal);
	glDisableVertexArrayAttrib(VAOCube, vColor);
	glDisableVertexArrayAttrib(VAOCube, vTexCoord);
}

void Shape::setColor(glm::vec3 color) {
	this->colors.clear();
	for (int i = 0;i < vertices.size();i += 3) {
		this->colors.push_back(color.x);
		this->colors.push_back(color.y);
		this->colors.push_back(color.z);
	}
}

void Shape::EnableTexture(char* path) {		//set uniform to 1 for shader, load file ONCE, bind texture in draw call, inefficient??
	this->textureEnabled = 1.0f;
	this->path = path;

	this->image.bitmapFormat = FreeImage_GetFileType(this->path);
	this->image.bitmapData = FreeImage_Load(this->image.bitmapFormat, path);

	this->image.bitmapWidth = FreeImage_GetWidth(this->image.bitmapData);
	this->image.bitmapHeight = FreeImage_GetHeight(this->image.bitmapData);

	glGenTextures(1, &this->image.tex);

	this->image.bitmapBits = FreeImage_GetBits(this->image.bitmapData);
}

void Shape::DisableTexture() {
	this->textureEnabled = 0.0f;
	this->path = "NULL";
}