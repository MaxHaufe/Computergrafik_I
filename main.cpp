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
#include "Planet.h"

#define DIFFUSE_LIGHTNING 1
#define PLANETS 1
#define CARBONATOM 0

#define CORE 1
#define ELECTRONS 1
#define SHELLS 1
#define RANDOMELECTRONS 1

//messing around
#define PAULA 0


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

GLfloat rotationIncrement = 0.009;		//*2, slowed down to 30fps from 60, nvm


void timer(int value) {

	if (rotationAngle > M_PI * 2) {
		rotationAngle = 0;
	}
	rotationAngle -= rotationIncrement;

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, value);
}
Planet* mars = new Planet(1.0f,300,300);

Cube* cube = new Cube();
Cube* cubeugly = new Cube();
Sphere* sphere = new Sphere(1, 100, 100);

Sphere* sun = new Sphere(1, 500, 500);
Sphere* earth = new Sphere(1, 300, 300);
Sphere* moon = new Sphere(1, 300, 300);
Sphere* venus = new Sphere(1, 200, 200);
Sphere* mercury = new Sphere(1, 200, 200);

Circle* electronShell = new Circle();
Circle* earthOrbit = new Circle(1, 500);
Circle* moonOrbit = new Circle(1, 500);
Circle* venusOrbit = new Circle(1, 500);
Circle* mercuryOrbit = new Circle(1, 500);

std::vector<Shape*> celestials = { sun,moon,earth,venus,mercury, mars};
std::vector<Shape*> orbits = { earthOrbit, moonOrbit, venusOrbit, mercuryOrbit };

std::vector<Shape*> solarSystem;

void init(void) {

	printf("\n%s", (char*)glGetString(GL_RENDERER));
	printf("\n%s", (char*)glGetString(GL_VERSION));
	printf("\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	program = loadShaders("beleg.vs", "beleg.fs", "", "", "", "");

	//fill solarSystem vector
	for (int i = 0;i < celestials.size();i++) {
		solarSystem.push_back(celestials[i]);
	}

	for (int i = 0;i < orbits.size();i++) {
		solarSystem.push_back(orbits[i]);
	}

	//ugly image stuff, TODO
	FreeImage_Initialise(TRUE);

	moon->EnableTexture("moon.jpg");
	sun->EnableTexture("sun.jpg");
	//sun->EnableTexture("paula.jpeg");
	//earth->EnableTexture("earth.jpg");	//2
	//earth->EnableTexture("earth3.jpg");		//1
	earth->EnableTexture("earth4.jpg");			//0
//	earth->EnableTexture("earthNASA.jpg");//3
	//earth->EnableTexture("earthNight.jpg");
	venus->EnableTexture("venus.jpg");
	mars->EnableTexture("mars.jpg");
	//generate Buffers
	glUseProgram(program);
	glGenBuffers(NumBuffers, Buffers);
	glGenVertexArrays(NumVAOs, VAO);
	
	glEnable(GL_TEXTURE_2D);
}



void Planets() {

	vec3 viewPos(0.0f, 3.5f, 10.0f);
	vec3 orbitColor = vec3(166.0f / 255.0f, 166.0f / 255.0f, 166.0f / 255.0f);

	sun->material.ambient = vec3(1, 1, 1);		//make sun bright

	mat4 Projection = mat4(1.0);
	mat4 Model = mat4(1.0);
	mat4 ModelR = mat4(1.0);
	mat4 ModelR2 = mat4(1.0);
	mat4 ModelR3 = mat4(1.0);
	mat4 ModelR4 = mat4(1.0);
	mat4 ModelT = mat4(1.0);
	mat4 ModelS = mat4(1.0f);
	mat4 ModelT2 = mat4(1.0f);
	mat4 ModelT3 = mat4(1.0f);
	mat4 ModelT4 = mat4(1.0f);

	glViewport(0, 0, width, height);

	//Projection = ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 20.0f);									//GEHT AUCHs
	//Projection = frustum(-5.0f, 5.0f, -5.0f, 5.0f, 5.0f, 25.0f);												//FUNKTIONIERT !!!!!!!!!!!
	float aspect = (float)width / (float)height;
	Projection = perspective(45.0f, float(width) / float(height), 0.1f, 20.0f);
	//Projection = ortho(-aspect, aspect,(float) -1, (float)1, (float)-1, (float)1); // geht glaub
	//Projection = glm::perspective(aspect, 1.0f, 3.5f, -0.5f);	//EINFACH MAL NCIHT MACHEN

	for (int i = 0; i < solarSystem.size();i++) {		//set all Projections and viewPos
		solarSystem[i]->setViewPos(viewPos);
		solarSystem[i]->Projection = Projection;
	}

	for (int i = 0;i < celestials.size();i++) {		//disable reflections on all celestial bodies
		celestials[i]->material.specular = vec3(0, 0, 0);
	}

	for (int i = 0;i < orbits.size();i++) {
		orbits[i]->setColor(orbitColor);
	}

	mars->setViewPos(viewPos);
	mars->Projection = Projection;

	//earth


	//calculate earth and its rotation
	float orbitRadius = 4;
	float xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);
	float earthOrbitVal = xzOrbitValue;		//for the translation of the moon orbit
	
	//rotate it around the sun
	ModelR = rotate(mat4(1.0), -rotationAngle, vec3(0.0, 1.0, 0.0));
	ModelR4 = rotate(mat4(1.0), rotationAngle, vec3(0.0, 1.0, 0.0));		//if this wasn't applied, and if earth wouldn't spin around itself,
																			//one spot, for example, south america would always point towards the sun
	ModelT = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));
	
	ModelS = scale(mat4(1.0), vec3(0.3f, 0.3f, 0.3f));
	
	//tilt it and make it rotate around its own axis
	ModelR2 = rotate(mat4(1.0), -0.408407f, vec3(0.0, 0.0, 1.0));			//0,408407  = 23.4°
	ModelR3 = rotate(mat4(1.0), -5*rotationAngle, vec3(0.0, 1.0, 0.0));		//spin around own axis

	Model = ModelR * ModelT * ModelR4* ModelR2 * ModelR3 * ModelS;
	//Model = ModelR * ModelT * ModelR4 * ModelR2 * ModelR3;
	
	earth->Model = Model;
	earth->draw();


	//calculate earth orbit
	ModelT = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
	ModelS = scale(mat4(1.0f), vec3(orbitRadius, orbitRadius, 0));
	ModelR = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));		//rotate to fit x-z axis

	Model = ModelR * ModelS;
	earthOrbit->Model = Model;



	//draw it
	earthOrbit->setColor(orbitColor);
	earthOrbit->draw(false, GL_LINES);


	//moon ----------------------------------------------------

	//calculate moon and its rotation
	orbitRadius = 4.0f;		//orbit around sun
	xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

	ModelR = rotate(mat4(1.0), -rotationAngle, vec3(0.0, 1.0, 0.0));
	//ModelR4 = rotate(mat4(1.0), rotationAngle, vec3(0.0, 1.0, 0.0));		//unspin ModelR
	ModelT = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));

	orbitRadius = 0.7f;		//orbit around earth
	xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

	ModelR2 = rotate(mat4(1.0), 4*rotationAngle, vec3(0.0, 1.0, 0.0));

	ModelT2 = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));

	ModelS = scale(mat4(1.0), vec3(0.27f * 0.3f, 0.27f * 0.3f, 0.27f * 0.3f));		//earth is 0.27* the size of earth
	//ModelR3 = rotate(mat4(1.0f), 0.0872665f, vec3(0.0f, 0.0f, 1.0f));		//tilt of 5 degrees; 0,0872665 = 5deg

	//Model = ModelR * ModelT *ModelR4* ModelR3 * ModelR2 * ModelT2 * ModelS;
	Model = ModelR * ModelT * ModelR2 * ModelT2 * ModelS;

	moon->Model = Model;

	moon->draw();

	//--------------------------------------------------------

	//float xEarth, zEarth;			//translate center of rotation to earth's current point
	//xEarth = earthOrbitVal * cosf(rotationAngle) - earthOrbitVal * sinf(rotationAngle);
	//zEarth = earthOrbitVal * sinf(rotationAngle) + earthOrbitVal * cosf(rotationAngle);

	//ModelR = rotate(mat4(1.0), 5 * rotationAngle, vec3(0.0, 1.0, 0.0f));
	//ModelS = scale(mat4(1.0), vec3(0.2f, 0.2f, 0.2f));
	//ModelT = translate(mat4(1.0), vec3(xEarth, 0.0, zEarth));
	//ModelT2 = translate(mat4(1.0), vec3(1, 0.0, 1));

	//Model = ModelT * ModelR * ModelT2 * ModelS;		

	//moon->Model = Model;

	//moon->draw();

	//calculate moon orbit
	//ModelT = translate(mat4(1.0f), vec3(xEarth, 0.0, zEarth));		//fit rotation centre to earths current pos
	//ModelR3 = rotate(mat4(1.0f), 0.0872665f, vec3(0.0f, 0.0f, 1.0f));		//tilt of 5 degrees; 0,0872665 = 5deg
	ModelS = scale(mat4(1.0f), vec3(orbitRadius, orbitRadius, 0));
	ModelR2 = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));

	Model = ModelR * ModelT * ModelR2* ModelS;
	moonOrbit->Model = Model;

	//draw them, make sure to draw orbits before spheres
	moonOrbit->setColor(orbitColor);
	moonOrbit->draw(false, GL_LINES);

	//sun
	//ModelS = scale(mat4(1.0), vec3(4.0f, 4.0f, 4.0f));
	ModelS = scale(mat4(1.0), vec3(0.7f, 0.7f, 0.7f));
	ModelR = rotate(mat4(1.0), 0.4f * -rotationAngle, vec3(0.0, 1.0, 0.0f));
	Model = ModelR * ModelS;
	sun->Model = Model;
	sun->draw();

	//test
	mars->setRevolutionSpeed(686.980f);
	mars->setRotationSpeed(365);
	mars->setRotationAngleOffset(0.408407f);
	mars->setScaleParameter(0.4f);
	mars->setRadiusFromSun(7.0f);
	mars->draw();

	//venus
	ModelS = scale(mat4(1.0), vec3(0.2f, 0.2f, 0.2f));
	orbitRadius = 2.5f;		//orbit around sun
	xzOrbitValue = orbitRadius / sqrt(2);
	ModelS = scale(mat4(1.0), vec3(0.2f, 0.2f, 0.2f));
	ModelR = rotate(mat4(1.0), 5* rotationAngle, vec3(0.0, 1.0, 0.0));			//venus rotates clockwise, when looked at from "north" pole 
	ModelR2 = rotate(mat4(1.0), 365.0f / 224.0f * -rotationAngle, vec3(0.0, 1.0, 0.0));		//earth rotates at 5* angle with 365d/orbit around the sun; venus' orbit is 224d
	ModelT = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));

	Model = ModelR2 * ModelT * ModelR * ModelS;

	venus->Model = Model;
	venus->draw();

	//venus Orbit
	ModelS = scale(mat4(1.0f), vec3(orbitRadius, orbitRadius, 0));
	ModelR = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));

	Model = ModelR * ModelS;
	venusOrbit->Model = Model;
	venusOrbit->draw(false, GL_LINES);

	//mercury


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
	//glClearColor(0.5, 0., 0.5, 0);
	glClearColor(0.0, 0.0, 0.0,1);



	//vec3 viewPos(0.0f, 0.0f, 10.0f);		

	if (DIFFUSE_LIGHTNING) {
		vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
		//vec3 LightColor = vec3(0.5f, 0.5f, 0.5f);
		//vec3 LightColor = vec3(0.0f, 0.0f, 0.0f);
		vec3 LightPosition = vec3(0.0f, 0.0f, -10.0f);		//for point light: Multiplied with MV matrix -> light is -z value FROM VIEWPOINT!!!!!
		//vec3 LightPosition = viewPos;

		GLuint locColor = glGetUniformLocation(program, "LightColor");
		GLuint locLight = glGetUniformLocation(program, "LightPosition");

		glUniform3fv(locColor, 1, &LightColor[0]);
		glUniform3fv(locLight, 1, &LightPosition[0]);
	}
	else {
		vec3 LightColor = vec3(0.0f, 0.0f, 0.0f);
		vec3 LightPosition = vec3(0, 0, 0);

		GLuint locColor = glGetUniformLocation(program, "LightColor");
		GLuint locLight = glGetUniformLocation(program, "LightPosition");

		glUniform3fv(locColor, 1, &LightColor[0]);
		glUniform3fv(locLight, 1, &LightPosition[0]);
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
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
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
	glutFullScreen();
	//glutIdleFunc(idle);
	timer(0);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
