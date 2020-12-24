/*
  Dreiecke
*/

#define _USE_MATH_DEFINES



#include <math.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <glew.h>
#include <freeglut.h>
#include <FreeImage.h>


#include <gtc/quaternion.hpp>		//euler vectors
#include <gtx/quaternion.hpp>

#include <vector>

#include "Camera.h"
#include "Shape.h"
#include "Cube.h"
#include "Sphere.h"
#include "Circle.h"

#define DIFFUSE_LIGHTNING 1
#define PLANETS 1
#define CARBONATOM 0

#define CORE 1
#define ELECTRONS 1
#define SHELLS 1
#define RANDOMELECTRONS 1

//messing around
#define PAULA 1


using namespace glm;

GLuint loadShaders(const char* vertexFilePath,
	const char* fragmentFilePath,
	const char* geometryFilePath,
	const char* tesscontrolFilePath,
	const char* tessevaluationFilePath,
	const char* computeFilePath);
GLint height = 100, width = 100;
enum VAO_IDs { VAOCube, NumVAOs };
enum Buffer_IDs { VertexBuffer, ColorBuffer, NormalBuffer, ArrayBufferCube, ColorBufferCube, NormalBufferCube, TextureBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vColor, vTexCoord, VertexNormal };
GLuint VAO[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint program;

GLfloat rotationAngle = 0.0;

GLfloat rotationIncrement = 0.009*2;		//*2, slowed down to 30fps from 60, nvm


void timer(int value) {

	if (rotationAngle > M_PI * 2) {
		rotationAngle = 0;
	}
	rotationAngle -= rotationIncrement;

	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, value);
}





void init(void) {

	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	program = loadShaders("beleg.vs", "beleg.fs", "", "", "", "");


	//ugly image stuff, TODO
	FreeImage_Initialise(TRUE);

	FREE_IMAGE_FORMAT bitmapFormat = FIF_UNKNOWN;
	FIBITMAP* bitmapData;
	if (PAULA) {
		bitmapFormat = FreeImage_GetFileType("paula.jpeg");
		bitmapData = FreeImage_Load(bitmapFormat, "paula.jpeg");
	}
	else {
		bitmapFormat = FreeImage_GetFileType("earth.jpg");
		bitmapData = FreeImage_Load(bitmapFormat, "earth.jpg");
	}
	int bitmapWidth = FreeImage_GetWidth(bitmapData);
	int bitmapHeight = FreeImage_GetHeight(bitmapData);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	BYTE* bitmapBits = FreeImage_GetBits(bitmapData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmapBits);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_WRAP_BORDER, GL_REPEAT);

	//generate Buffers
	glUseProgram(program);
	glGenBuffers(NumBuffers, Buffers);
	glGenVertexArrays(NumVAOs, VAO);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glFlush();
}

Cube* cube = new Cube();
Cube* cubeugly = new Cube();
Sphere* sphere = new Sphere(1, 1000, 1000);
Sphere* sphereColorful = new Sphere(1, 300, 300);
Sphere* sphereFunny = new Sphere(1, 300, 300);

Circle* electronShell = new Circle();
Circle* earthOrbit = new Circle();
Circle* moonOrbit = new Circle();

void Planets() {

	vec3 viewPos(0.0f, 3.5f, 10.0f);

	sphere->setViewPos(viewPos);
	sphereColorful->setViewPos(viewPos);
	earthOrbit->setViewPos(viewPos);
	moonOrbit->setViewPos(viewPos);

	mat4 Projection = mat4(1.0);
	mat4 Model = mat4(1.0);
	mat4 ModelR = mat4(1.0);
	mat4 ModelT = mat4(1.0);
	mat4 ModelS = mat4(1.0f);
	mat4 ModelT2 = mat4(1.0f);
	mat4 ModelT3 = mat4(1.0f);
	mat4 ModelT4 = mat4(1.0f);

	glViewport(0, 0, width, height);

	//Projection = ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 20.0f);									//GEHT AUCH
	Projection = frustum(-5.0f, 5.0f, -5.0f, 5.0f, 5.0f, 25.0f);												//FUNKTIONIERT !!!!!!!!!!!
	float aspect = (float)width / (float)height;
	//Projection = ortho(-aspect, aspect,(float) -1, (float)1, (float)-1, (float)1); // geht glaub
	//Projection = glm::perspective(aspect, 1.0f, 3.5f, -0.5f);	//EINFACH MAL NCIHT MACHEN

	sphere->Projection = Projection;
	sphereColorful->Projection = Projection;
	earthOrbit->Projection = Projection;
	moonOrbit->Projection = Projection;

	//earth

	Model = mat4(1.0);
	ModelR = mat4(1.0);
	ModelT = mat4(1.0);
	ModelS = mat4(1.0f);
	ModelT2 = mat4(1.0f);
	ModelT3 = mat4(1.0f);


	//calculate earth and its rotation
	float orbitRadius = 4;
	float xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);
	float earthOrbitVal = xzOrbitValue;		//for the translation of the moon orbit

	ModelR = rotate(mat4(1.0), -rotationAngle, vec3(0.0, 1.0, 0.0));
	ModelT = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));
	ModelS = scale(mat4(1.0), vec3(0.5f, 0.5f, 0.5f));
	Model = ModelR * ModelT * ModelS;

	sphere->Model = Model;


	//calculate earth orbit
	ModelT = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
	ModelS = scale(mat4(1.0f), vec3(orbitRadius, orbitRadius, 0));
	ModelR = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));		//rotate to fit x-z axis
	Model = ModelR * ModelS;
	earthOrbit->Model = Model;

	//draw it
	//earthOrbit->draw(false, GL_LINES);
	//sphere->draw();
	//moon

	//calculate moon and its rotation
	orbitRadius = 1.5f;
	xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

	float xEarth, zEarth;			//translate center of rotation to earth's current point
	xEarth = earthOrbitVal * cosf(rotationAngle) - earthOrbitVal * sinf(rotationAngle);
	zEarth = earthOrbitVal * sinf(rotationAngle) + earthOrbitVal * cosf(rotationAngle);

	ModelR = rotate(mat4(1.0), 5 * rotationAngle, vec3(0.0, 1.0, 0.0f));
	ModelS = scale(mat4(1.0), vec3(0.2f, 0.2f, 0.2f));
	ModelT = translate(mat4(1.0), vec3(xEarth, 0.0, zEarth));
	ModelT2 = translate(mat4(1.0), vec3(1, 0.0, 1));

	Model = ModelT * ModelR * ModelT2 * ModelS;		

	sphereColorful->Model = Model;

	//calculate moon orbit
	ModelT = translate(mat4(1.0f), vec3(xEarth, 0.0, zEarth));		//fit rotation centre to earths current pos
	ModelS = scale(mat4(1.0f), vec3(orbitRadius, orbitRadius, 0));
	ModelR = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));
	Model = ModelT * ModelR *ModelS;
	moonOrbit->Model = Model;

	//draw them, make sure to draw orbits before spheres
	moonOrbit->setColor(vec3(153/255, 102/255, 255/255));
	//moonOrbit->draw(false, GL_LINES);
	sphereColorful->setColor(vec3(255/255, 102/255, 0/255));
	//sphereColorful->draw();

	//sun
	ModelS = scale(mat4(1.0), vec3(4.0f, 4.0f, 4.0f));
	ModelR = rotate(mat4(1.0), 2* rotationAngle, vec3(1.0, 1.0, 0.0f));
	Model = ModelR * ModelS;
	sphere->Model = Model;
	sphere->draw();
}

void CarbonAtom() {		//!!!GLOABAL!!! Circle* electronShell = new Circle(); AND Sphere* sphere = new Sphere(1, 30, 30); are required; Sphere Parameters do not matter, as long as Radius equals 1
	
	/*
	Make sure that CORE = 1, SHELLS = 1, ELECTRONS = 1
	RANDOMELECTRONS = 1 makes the electrons go wild
	RANDOMELECTRONS = 0 synchronizes them
	*/
	
	vec3 viewPos(0.0f, 0.0f, 10.0f);
	vec3 target(0.0f, 0.0f, 0.0f);

	mat4 Projection = mat4(1.0);
	mat4 Model = mat4(1.0);
	mat4 ModelR = mat4(1.0);
	mat4 ModelR2 = mat4(1.0f);
	mat4 ModelT = mat4(1.0);
	mat4 ModelS = mat4(1.0f);

	glViewport(0, 0, width, height);

	Projection = ortho(-5.0f, 5.0f, -5.0f, 5.0f, 5.0f, 15.0f);

	//draw the core, i.e. neutrons and protons
	if (CORE) {	
		sphere->Projection = Projection;
		ModelS = scale(ModelS, vec3(0.3f, 0.3f, 0.3f));

		vec3 red = vec3(1.0f, 0.0f, 0.0f);
		vec3 blue = vec3(51.0f / 255.0f, 153.0f / 255.0f, 255.0f / 255.0f);

		ModelT = mat4(1.0);		//1
		ModelT = translate(ModelT, vec3(-0.2, 0.5, -0.5));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(red);
		sphere->draw();

		ModelT = mat4(1.0);		//2
		ModelT = translate(ModelT, vec3(0.2, 0.5, -0.4));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(blue);
		sphere->draw();

		ModelT = mat4(1.0);		//3
		ModelT = translate(ModelT, vec3(0.4, 0.2, -0.6));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(red);
		sphere->draw();


		ModelT = mat4(1.0);		//4
		ModelT = translate(ModelT, vec3(0.4, 0.0, -0.4));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(blue);
		sphere->draw();


		ModelT = mat4(1.0);		//5
		ModelT = translate(ModelT, vec3(0.2, 0.2, -0.2));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(red);
		sphere->draw();

		ModelT = mat4(1.0);		//6
		ModelT = translate(ModelT, vec3(-0.1, 0.25, 0.2));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(blue);
		sphere->draw();

		ModelT = mat4(1.0);		//7
		ModelT = translate(ModelT, vec3(-0.45, 0.15, -0.3));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(blue);
		sphere->draw();

		ModelT = mat4(1.0);		//8
		ModelT = translate(ModelT, vec3(-0.2, -0.1, 0.0));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(red);
		sphere->draw();

		ModelT = mat4(1.0);		//9
		ModelT = translate(ModelT, vec3(0.2, -0.1, 0.3));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(blue);
		sphere->draw();

		ModelT = mat4(1.0);		//10
		ModelT = translate(ModelT, vec3(0.2, -0.35, 0.0));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(red);
		sphere->draw();

		ModelT = mat4(1.0);		//11
		ModelT = translate(ModelT, vec3(-0.1, -0.4, -0.2));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(blue);
		sphere->draw();

		ModelT = mat4(1.0);		//12
		ModelT = translate(ModelT, vec3(-0.45, -0.3, -0.3));
		Model = ModelT * ModelS;
		sphere->Model = Model;
		sphere->setColor(red);
		sphere->draw();
	}	 

	//draw Shells
	float electronShellAngle = 3 * M_PI / 7;		//rotate circle on xz to make it elliptic
	float electronShellAngleY = M_PI / 6;			//rotating increment to make 6 shells
	electronShell->Projection = Projection;

	ModelS = mat4(1.0);
	ModelS = scale(ModelS, vec3(4.0f, 4.0f, 4.0f));

	
	if (SHELLS) {
		for (int i = 0; i < 6;i++) {
			vec3 EulerAngles(0, electronShellAngle, i * electronShellAngleY);

			quat MyQuaternion;
			MyQuaternion = quat(EulerAngles);
			ModelR = toMat4(MyQuaternion);

			Model = ModelR * ModelS;
			electronShell->Model = Model;

			electronShell->draw();
		}
	} 

	//draw electrons
	Model = mat4(1.0);
	ModelR = mat4(1.0);
	ModelR2 = mat4(1.0f);
	ModelT = mat4(1.0);
	ModelS = mat4(1.0f);

	sphere->setColor(vec3(51.0f / 255.0f, 51.0f / 255.0f, 255.0f / 255.0f));		//darker blue for electrons

	sphere->Projection = Projection;

	ModelS = scale(ModelS, vec3(0.3f, 0.3f, 0.3f));		//match the size of protons and neutrons


	if (ELECTRONS) {
		for (int i = 0; i < 6;i++) {
			ModelT = mat4(1.0);
			ModelR = mat4(1.0);

			ModelT = translate(ModelT, vec3(4, 0, 0));								//hardcoded, adjust to fit ModelS of shells
			if (RANDOMELECTRONS) {															//added offset to rotation angle in order to de-synchronize them
				ModelR = rotate(ModelR, 4 * rotationAngle + i, vec3(0.0, 0.0, 1.0f));		//translate and rotate, to rotate z-axis
			}
			else																			
			{
				ModelR = rotate(ModelR, 4 * rotationAngle, vec3(0.0, 0.0, 1.0f));
			}

			vec3 EulerAngles(0, electronShellAngle, i * electronShellAngleY);		//rotate to fit the electronShells
			quat MyQuaternion;
			MyQuaternion = quat(EulerAngles);
			ModelR2 = toMat4(MyQuaternion);

			Model = ModelR2 * ModelR * ModelT * ModelS;

			sphere->Model = Model;

			sphere->draw();
		}

	}

}

void display(void) {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0., 0.5, 0);
	//glClearColor(0.0, 0.0, 0.0,0);



	//vec3 viewPos(0.0f, 0.0f, 10.0f);		

	if (DIFFUSE_LIGHTNING) {
		//vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
		vec3 LightColor = vec3(0.5f, 0.5f, 0.5f);
		//vec3 LightColor = vec3(0.0f, 0.0f, 0.0f);
		vec3 LightPosition = vec3(0.0f, 0.0f, -10.0f);		//for point light: Multiplied with MV matrix -> light is -z value FROM VIEWPOINT!!!!!
		//vec3 LightPosition = viewPos;

		GLuint locColor = glGetUniformLocation(program, "LightColor");
		GLuint locLight = glGetUniformLocation(program, "LightPosition");

		glUniform3fv(locColor, 1, &LightColor[0]);
		glUniform3fv(locLight, 1, &LightPosition[0]);

		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(32 * sizeof(float)));
		glEnableVertexAttribArray(vTexCoord);
	}
	else {
		vec3 LightColor = vec3(0.0f, 0.0f, 0.0f);
		vec3 LightPosition = vec3(-5, 0, 5);

		GLuint locColor = glGetUniformLocation(program, "LightColor");
		GLuint locLight = glGetUniformLocation(program, "LightPosition");

		glUniform3fv(locColor, 1, &LightColor[0]);
		glUniform3fv(locLight, 1, &LightPosition[0]);

		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(32 * sizeof(float)));
		glEnableVertexAttribArray(vTexCoord);
	}
	


	mat4 Projection = mat4(1.0);
	mat4 Model = mat4(1.0);
	mat4 ModelR = mat4(1.0);
	mat4 ModelR2 = mat4(1.0f);
	mat4 ModelT = mat4(1.0);
	mat4 ModelS = mat4(1.0f);
	mat4 ModelT2 = mat4(1.0f);
	mat4 ModelT3 = mat4(1.0f);
	mat4 ModelT4 = mat4(1.0f);

	glViewport(0, 0, width, height);

	Projection = ortho(-5.0f, 5.0f, -5.0f, 5.0f, 5.0f, 20.0f);										//GEHT AUCH	//atommodell
	//Projection = frustum(-2.f, 2.f, -2.f, 2.f, 2.f, -2.f);												//FUNKTIONIERT !!!!!!!!!!!
	float aspect = (float)width / (float)height;
	//Projection = ortho(3*-aspect,3* aspect,(float) -3, (float)3, (float)3, (float)-3); // geht glaub
	//Projection = glm::perspective(aspect, 1.0f, 3.5f, -0.5f);	//EINFACH MAL NCIHT MACHEN
	
	if (CARBONATOM) {
		CarbonAtom();
	}

	//Planets lol
	if (PLANETS) {
		Planets();
	}

	glFlush();	
}

void reshape(int w, int h) {
	width = w;
	height = h;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1080, 1080);
	glutInitContextVersion(4, 5);  // (4,2) (3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//GLUT_COMPATIBILITY_PROFILE
	glutCreateWindow("CG-Beleg");
	glewExperimental = GL_TRUE;
	if (glewInit()) printf("Error");
	init();
	glutTimerFunc(1, timer, 0);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
