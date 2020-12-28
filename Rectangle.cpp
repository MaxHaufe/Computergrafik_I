#include "Rectangle.h"

RectangleBackground::RectangleBackground(float height, float width) {
	this->vertices.push_back(-1.0f);
	this->vertices.push_back(1.0f);
	this->vertices.push_back(0.0f);

	this->vertices.push_back(1.0f);
	this->vertices.push_back(1.0f);
	this->vertices.push_back(0.0f);

	this->vertices.push_back(-1.0f);
	this->vertices.push_back(-1.0f);
	this->vertices.push_back(0.0f);

	this->vertices.push_back(-1.0f);
	this->vertices.push_back(-1.0f);
	this->vertices.push_back(0.0f);

	this->vertices.push_back(1.0f);
	this->vertices.push_back(1.0f);
	this->vertices.push_back(0.0f);

	this->vertices.push_back(1.0f);
	this->vertices.push_back(-1.0f);
	this->vertices.push_back(0.0f);

	this->normals = this->vertices;
	this->colors = this->vertices;

	this->texCoords.push_back(0.0f);
	this->texCoords.push_back(1.0f);

	this->texCoords.push_back(1.0f);
	this->texCoords.push_back(1.0f);

	this->texCoords.push_back(0.0f);
	this->texCoords.push_back(0.0f);

	this->texCoords.push_back(0.0f);
	this->texCoords.push_back(0.0f);

	this->texCoords.push_back(1.0f);
	this->texCoords.push_back(1.0f);

	this->texCoords.push_back(1.0f);
	this->texCoords.push_back(0.0f);
}