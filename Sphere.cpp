#include "Sphere.h"
#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(float radius, unsigned int stackCount, unsigned int sectorCount, bool funny, bool randomcolors) {

	float x, y, z, xy, x2, y2, z2, xy2, x3, z3, x4, z4;                              // vertex position

	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep = M_PI / stackCount;
	float sectorAngle, stackAngle, sectorAngle2, stackAngle2;

	for (unsigned int i = 0; i <= stackCount; ++i)
	{
		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		stackAngle2 = M_PI / 2 - (i + 1) * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		xy2 = radius * cosf(stackAngle2);             
		y = radius * sinf(stackAngle);              // r * sin(u)
		y2 = radius * sinf(stackAngle2);              

		for (unsigned int j = 0; j < sectorCount; ++j)			//!!! changed to < instead of <= !!!
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi
			sectorAngle2 = (j + 1) * sectorStep;

			// vertex position (x, y, z)
			//k1
			z = xy * cosf(sectorAngle);             // r * cos(u) * cos(v) 
			x = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)	
			//k2
			z2 = xy2 * cosf(sectorAngle);             
			x2 = xy2 * sinf(sectorAngle);
			//k1+1
			z3 = xy * cosf(sectorAngle2);			
			x3 = xy * sinf(sectorAngle2);
			//k2+1
			z4 = xy2 * cosf(sectorAngle2);			
			x4 = xy2 * sinf(sectorAngle2);

			this->vertices.push_back(x);
			this->vertices.push_back(y);
			this->vertices.push_back(z);
			
			this->vertices.push_back(x2);
			this->vertices.push_back(y2);
			this->vertices.push_back(z2);
			
			this->vertices.push_back(x3);
			this->vertices.push_back(y);
			this->vertices.push_back(z3);
			
			this->vertices.push_back(x3);
			this->vertices.push_back(y);
			this->vertices.push_back(z3);
			
			this->vertices.push_back(x2);
			this->vertices.push_back(y2);
			this->vertices.push_back(z2);
			
			this->vertices.push_back(x4);
			this->vertices.push_back(y2);
			this->vertices.push_back(z4);

			if (funny) {
				this->vertices.push_back(x);
				this->vertices.push_back(y);
				this->vertices.push_back(z);
			}

			// vertex tex coord (s, t) range between [0, 1]
			float s, t, s2, t2;;

			s = (float)j / sectorCount;
			t = (float)i / stackCount;

			s2 = (float)(j + 1) / sectorCount;
			t2 = (float)(i + 1) / stackCount;
			for (int k = 0;k < 6;k++) {
				this->texCoords.push_back(s);
				this->texCoords.push_back(stackCount - t);		//prevents texture from being upside down
			}
		}
	}
	this->normals = this->vertices;

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