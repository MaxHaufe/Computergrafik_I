#include "Ring.h"
#include <glm.hpp>
#include <glew.h>
#include <gtx/transform.hpp>
#include "GlobalVariables.h"
using namespace glm;

Ring::Ring(float holeRadius, unsigned int segments) {
	float x1, y1, x2, y2, x3, x4, y3, y4, z, theta;
	x1 = 1.0f;	//outer circle
	y1 = 0.0f;
	x3 = holeRadius;
	y3 = 0.0f;
	z = 0.0f;
	theta = (M_PI * 2) / segments;

	for (int i = 0; i < segments; i++) {

		float x2 = cos(theta) * x1 - sin(theta) * y1;
		float y2 = sin(theta) * x1 + cos(theta) * y1;

		float x4 = cos(theta) * x3 - sin(theta) * y3;
		float y4 = sin(theta) * x3 + cos(theta) * y3;

		this->vertices.push_back(x1);
		this->vertices.push_back(y1);
		this->vertices.push_back(z);

		this->vertices.push_back(x2);
		this->vertices.push_back(y2);
		this->vertices.push_back(z);

		this->vertices.push_back(x3);
		this->vertices.push_back(y3);
		this->vertices.push_back(z);

		this->vertices.push_back(x3);
		this->vertices.push_back(y3);
		this->vertices.push_back(z);

		this->vertices.push_back(x2);
		this->vertices.push_back(y2);
		this->vertices.push_back(z);

		this->vertices.push_back(x4);
		this->vertices.push_back(y4);
		this->vertices.push_back(z);

		//texCoords

		float tx = (float)i / segments;
		float tx2 = (float)(i+1) / segments;

		this->texCoords.push_back(1.0f);
		this->texCoords.push_back(1.0f);

		this->texCoords.push_back(1.0f);
		this->texCoords.push_back(0.0f);

		this->texCoords.push_back(0.0f);
		this->texCoords.push_back(1.0f);

		this->texCoords.push_back(0.0f);
		this->texCoords.push_back(1.0f);

		this->texCoords.push_back(1.0f);
		this->texCoords.push_back(0.0f);

		this->texCoords.push_back(0.0f);
		this->texCoords.push_back(0.0f);

		x1 = x2;
		y1 = y2;

		x3 = x4;
		y3 = y4;
	}
	this->normals = this->vertices;
	this->colors = this->vertices;

	//also make them "immune" to our light sources
	this->material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	this->material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Ring::draw() {
	float xzOrbitValue = this->radiusFromSun / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

//i will be measuring the rotationangles relative to earth

	mat4 ModelS = mat4(1.0f);

	mat4 ModelTransRevolution = mat4(1.0);

	mat4 ModelRotRevolution = mat4(1.0);
	mat4 ModelRotRevolutionFix = mat4(1.0); //necessary for rotation offset, disable rotation and this, then see what happens :^)
	mat4 ModelRotTilt = mat4(1.0);
	mat4 ModelRotRotationOffset = mat4(1.0);

	//resize 
	ModelS = scale(mat4(1.0f), vec3(this->scaleParameter, this->scaleParameter, this->scaleParameter));

	//tilt to fit xy axis
	ModelRotTilt = rotate(mat4(1.0), (float)M_PI/2, vec3(1.0, 0.0, 0.0));

	//create offset tilt
	ModelRotRotationOffset = rotate(mat4(1.0), -this->rotationAngleOffset, vec3(0.0, 0.0, 1.0));

	//make it spin around origin
	ModelTransRevolution = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));
	//ModelTransRevolution = translate(mat4(1.0), vec3(0, 0, -this->radiusFromSun));
	ModelRotRevolution = rotate(mat4(1.0), 1.0f / this->revolutionSpeed * 365.256f * -rotationAngle, vec3(0.0, 1.0, 0.0));
	ModelRotRevolutionFix = rotate(mat4(1.0), 1.0f / this->revolutionSpeed * 365.256f * rotationAngle, vec3(0.0, 1.0, 0.0));

	//merge models
	this->Model = ModelRotRevolution * ModelTransRevolution * ModelRotRevolutionFix * ModelRotRotationOffset * ModelRotTilt * ModelS;

	Shape::draw();
}
float Ring::getScaleParameter() const
{
    return scaleParameter;
}

void Ring::setScaleParameter(float scaleParameter)
{
    this->scaleParameter = scaleParameter;
}

float Ring::getRadiusFromSun() const
{
    return radiusFromSun;
}

void Ring::setRadiusFromSun(float radiusFromSun)
{
    this->radiusFromSun = radiusFromSun;
}

float Ring::getRevolutionSpeed() const
{
    return revolutionSpeed;
}

void Ring::setRevolutionSpeed(float revolutionSpeed)
{
    this->revolutionSpeed = revolutionSpeed;
}

float Ring::getRotationAngleOffset() const
{
    return rotationAngleOffset;
}

void Ring::setRotationAngleOffset(float rotationAngleOffset)
{
    this->rotationAngleOffset = rotationAngleOffset;
}

