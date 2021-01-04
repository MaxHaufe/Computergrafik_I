#pragma once
#include "Circle.h"

#ifndef ORBIT_H
#define ORBIT_H

class Orbit : public Circle {
private:
	float radiusFromSun = 0.0f;
public:
	using Circle::Circle;
    float getRadiusFromSun() const;
    void setRadiusFromSun(float radiusFromSun);
};

#endif;