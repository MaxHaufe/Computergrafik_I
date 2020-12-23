#pragma once

#include <glm.hpp>
#include <glew.h>
#include <vector>
#include "Shape.h"

#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Shape {
public:
	Sphere(float radius = 1.0f, unsigned int stackCount = 16, unsigned int sectorCount = 16, bool funny = false, bool randomcolors = false);
};

#endif;