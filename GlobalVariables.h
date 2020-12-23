#pragma once

#include <glm.hpp>
#include <glew.h>

extern GLint height, width;
enum VAO_IDs { VAOCube, VAOSphere, NumVAOs };
enum Buffer_IDs { VertexBuffer, ColorBuffer, NormalBuffer, ArrayBufferCube, ColorBufferCube, NormalBufferCube, ArrayBufferSphere, ColorBufferSphere, NormalBufferSphere, NumBuffers };
enum Attrib_IDs { vPosition, vColor, vTexcoord, VertexNormal };
extern GLuint VAO[NumVAOs];
extern GLuint Buffers[NumBuffers];
extern GLuint program;