#pragma once

#include <glm.hpp>
#include <glew.h>
#include <vector>
#include <string>
#include <FreeImage.h>

#include "Camera.h"

#ifndef SHAPE_H
#define SHAPE_H

class Shape : public Camera{
private:
	glm::mat4 View = glm::mat4(1.0f);
	float textureEnabled = 0.0f;		//this is a boolean, 0 disables texture in the fragment shader, this is disabled by default
	char* path = "NULL";
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	glm::vec3 singleColor = glm::vec3(0.5f, 0.5f, 0.5f);
public:
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);

	struct Material {
		glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
		float shininess = 52.0f;		
	};

	Material material;

	void setColor(glm::vec3 color);
	void draw(bool filled = true, GLuint drawingMode = GL_TRIANGLES);
	void pushMatrices();		//and EyeDirection
	void pushMaterial();

	struct Image {
		FREE_IMAGE_FORMAT bitmapFormat = FIF_UNKNOWN;
		FIBITMAP* bitmapData;
		int bitmapWidth;
		int bitmapHeight;
		BYTE* bitmapBits;
		GLuint tex;
	};

	Image image;

	void EnableTexture(char* path);
	void DisableTexture();
};

#endif;