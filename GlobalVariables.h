#pragma once

#include <glm.hpp>
#include <glew.h>
#include <FreeImage.h>

extern GLint height, width;
enum VAO_IDs { VAOCube, VAOSphere, NumVAOs };
enum Buffer_IDs { VertexBuffer, ColorBuffer, NormalBuffer, ArrayBufferCube, ColorBufferCube, NormalBufferCube, ArrayBufferSphere, ColorBufferSphere, NormalBufferSphere, TextureBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vColor, vTexCoord, VertexNormal };
extern GLuint VAO[NumVAOs];
extern GLuint Buffers[NumBuffers];
extern GLuint program;