#pragma once

#include <glm.hpp>
#include <glew.h>
#include <vector>
#include "Shape.h"

#ifndef CUBE_H
#define CUBE_H

class Cube : public Shape{
public:
	Cube(bool randomcolors = false);
};

#endif;