#include "Planet.h"
#include <glm.hpp>
#include <glew.h>
#include <gtx/transform.hpp>
#include "GlobalVariables.h"
using namespace glm;

void Planet::draw() {


   float xzOrbitValue = this->radiusFromSun / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

   //i will be measuring the rotationangles relative to earth

   mat4 ModelS = mat4(1.0f);

   mat4 ModelTransRevolution = mat4(1.0);
  
   mat4 ModelRotRevolution = mat4(1.0);
   mat4 ModelRotRotation = mat4(1.0);
   mat4 ModelRotRotationOffset = mat4(1.0);

   //resize planet
   ModelS = scale(mat4(1.0f), vec3(this->scaleParameter, this->scaleParameter, this->scaleParameter));

   //make it rotate around its own axis
   if (this->antiClockwiseRotation) {
        ModelRotRotation = rotate(mat4(1.0), 1 / this->rotationSpeed * 365.256f * -rotationAngle, vec3(0.0, 1.0, 0.0));
   }
   else {
       ModelRotRotation = rotate(mat4(1.0), 1 / this->rotationSpeed * 365.256f * rotationAngle, vec3(0.0, 1.0, 0.0));
   }

   //create offset tilt
   ModelRotRotationOffset = rotate(mat4(1.0), -this->rotationAngleOffset, vec3(0.0, 0.0, 1.0));

   //make it spin around origin
   ModelTransRevolution = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));
   ModelRotRevolution = rotate(mat4(1.0), 1 / this->revolutionSpeed * 365.256f * -rotationAngle, vec3(0.0, 1.0, 0.0));
   
   //merge models
   this->Model = ModelRotRevolution * ModelTransRevolution *  ModelRotRotationOffset * ModelRotRotation * ModelS;
   
   Shape::draw();
}

float Planet::getScaleParameter() const
{
    return scaleParameter;
}

void Planet::setScaleParameter(float scaleParameter)
{
    this->scaleParameter = scaleParameter;
}

float Planet::getRadiusFromSun() const
{
    return radiusFromSun;
}

void Planet::setRadiusFromSun(float radiusFromSun)
{
    this->radiusFromSun = radiusFromSun;
}

float Planet::getRevolutionSpeed() const
{
    return revolutionSpeed;
}

void Planet::setRevolutionSpeed(float revolutionSpeed)
{
    this->revolutionSpeed = revolutionSpeed;
}

float Planet::getRotationSpeed() const
{
    return rotationSpeed;
}

void Planet::setRotationSpeed(float rotationSpeed)
{
    this->rotationSpeed = rotationSpeed;
}

float Planet::getRotationAngleOffset() const
{
    return rotationAngleOffset;
}

void Planet::setRotationAngleOffset(float rotationAngleOffset)
{
    this->rotationAngleOffset = rotationAngleOffset;
}

bool Planet::getAntiClockwiseRotation() const
{
    return antiClockwiseRotation;
}

void Planet::setAntiClockwiseRotation(bool antiClockwiseRotation)
{
    this->antiClockwiseRotation = antiClockwiseRotation;
}

