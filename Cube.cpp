#include "Cube.h"
//#include "GlobalVariables.h"

Cube::Cube(bool randomcolors) {
	this->vertices = {
		// front
		-1, -1, 1,
		1, -1, 1,
		1, 1, 1,
		1, 1, 1,
		-1, 1, 1,
		-1, -1, 1,
		// back
		-1, -1, -1,
		1, -1, -1,
		1, 1, -1,
		1, 1, -1,
		-1, 1, -1,
		-1, -1, -1,
		//left
		-1, -1, -1,
		-1, -1, 1,
		-1, 1, 1,
		-1, 1, 1,
		-1, 1, -1,
		-1, -1, -1,
		//right
		1, -1, -1,
		1, -1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, -1,
		1, -1, -1,
		//top
		-1, 1, 1,
		1, 1, 1,
		1, 1, -1,
		1, 1, -1,
		-1, 1, -1,
		-1, 1, 1,
		//bottom
		-1, -1, 1,
		1, -1, 1,
		1, -1, -1,
		1, -1, -1,
		-1, -1, -1,
		-1, -1, 1,
	};
	this->normals = {
		//front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		//back
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		//left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		//right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		//top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		//botton
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
	};
	if (randomcolors)
	{
		for (int i = 0;i < vertices.size();i++) {
			int random = rand() % 10 + 1;
			GLfloat val = (GLfloat)random / 10;
			this->colors.push_back(val);		//random colors
		}
	}
	else {
		for (int i = 0;i < vertices.size();i += 3) {
			this->colors.push_back(singleColor.x);
			this->colors.push_back(singleColor.y);
			this->colors.push_back(singleColor.z);
		}
	}
}