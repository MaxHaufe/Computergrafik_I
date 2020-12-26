#include "Orbit.h"
#include <glm.hpp>
#include <glew.h>
#include <gtx/transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

void Orbit::displayOrbit(bool enable) {
    

}
float Orbit::getRadiusFromSun() const
{
    return radiusFromSun;
}

void Orbit::setRadiusFromSun(float radiusFromSun)
{
    this->radiusFromSun = radiusFromSun;

    mat4 ModelS = mat4(1.0f);
    mat4 ModelR = mat4(1.0f);

    ModelS = scale(mat4(1.0f), vec3(this->radiusFromSun, this->radiusFromSun, 0));
    ModelR = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));		//rotate to fit x-z axis

    this->Model = ModelR * ModelS;
}

