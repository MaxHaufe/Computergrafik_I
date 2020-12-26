#pragma once
#include "Sphere.h"

#ifndef PLANET_H
#define PLANET_H

class Planet : public Sphere {
private:
    float scaleParameter = 1.0f;	//for ModelS
    float radiusFromSun = 0.0f;
    float revolutionSpeed = 365.256f; //nr in days to do a full circle around the sun
    float rotationSpeed = 365.256f;		//nr of days to do a full rotation around own axis
    float rotationAngleOffset = 0.0f;		//tilt of the axis
    bool antiClockwiseRotation = true;		//earth rotates anticlockwise, venus doesn't
public:

    using Sphere::Sphere;       //inherit sphere constructor

    void draw();

    float getScaleParameter() const;
    void setScaleParameter(float scaleParameter);

    float getRadiusFromSun() const;
    void setRadiusFromSun(float radiusFromSun);

    float getRevolutionSpeed() const;
    void setRevolutionSpeed(float revolutionSpeed);

    float getRotationSpeed() const;
    void setRotationSpeed(float rotationSpeed);

    float getRotationAngleOffset() const;
    void setRotationAngleOffset(float rotationAngleOffset);

    bool getAntiClockwiseRotation() const;
    void setAntiClockwiseRotation(bool antiClockwiseRotation);

};

#endif;