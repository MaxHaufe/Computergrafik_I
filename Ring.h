#pragma once
#include "Shape.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Ring :
    public Shape
{
private:
    float scaleParameter = 1.0f;	//for ModelS
    float radiusFromSun = 0.0f;
    float revolutionSpeed = 365.256f; //nr in days to do a full circle around the sun
    float rotationAngleOffset = 0.0f;		//tilt of the axis
public:
    Ring(float holeRadius = 0.5f, unsigned int segments = 100);     //holeRadius is [0,1], , with the hole having the holeRadius
    void draw();

    float getScaleParameter() const;
    void setScaleParameter(float scaleParameter);

    float getRadiusFromSun() const;
    void setRadiusFromSun(float radiusFromSun);

    float getRevolutionSpeed() const;
    void setRevolutionSpeed(float revolutionSpeed);

    float getRotationAngleOffset() const;
    void setRotationAngleOffset(float rotationAngleOffset);

};

