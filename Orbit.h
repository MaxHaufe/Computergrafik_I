#pragma once
#include "Circle.h"

#ifndef ORBIT_H
#define ORBIT_H

class Orbit : public Circle {
private:
	float radiusFromSun = 0.0f;
	bool enable = true;
public:
	using Circle::Circle;
	void displayOrbit(bool enable);
    float getRadiusFromSun() const;
    void setRadiusFromSun(float radiusFromSun);
};

#endif;