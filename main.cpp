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
#include "Orbit.h"
#include "Ring.h"
#include "Rectangle.h"

#define DIFFUSE_LIGHTNING 1
#define PLANETS 1
#define CARBONATOM 0

#define CORE 1
#define ELECTRONS 1
#define SHELLS 1
bool RANDOMELECTRONS = true;


using namespace glm;

GLuint loadShaders(const char* vertexFilePath,
	const char* fragmentFilePath,
	const char* geometryFilePath,
	const char* tesscontrolFilePath,
	const char* tessevaluationFilePath,
	const char* computeFilePath);
void redGiant();
GLint height = 1080, width = 1920;
enum VAO_IDs { VAOCube, NumVAOs };
enum Buffer_IDs { VertexBuffer, ColorBuffer, NormalBuffer, ArrayBufferCube, ColorBufferCube, NormalBufferCube, TextureBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vColor, vTexCoord, VertexNormal };
GLuint VAO[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint program;

GLfloat rotationAngle = 0.0;

GLfloat rotationIncrement = 0.009;		

GLfloat kposx = 0.0;
GLfloat kposy = 0.0;
GLint mposx, mposy;

GLfloat alpha = 0.2, beta = 0.9, dist = 10, DELTA = 0.5;


void timer(int value) {

	if (rotationAngle > M_PI * 2) {
		rotationAngle = 0;
	}
	rotationAngle -= rotationIncrement;

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, value);
}


Cube* cube = new Cube();
Cube* cubeugly = new Cube();
Sphere* sphere = new Sphere();

Sphere* moon = new Sphere(1, 100, 100);

Planet* sun = new Planet(1, 500, 500);
Planet* jupiter = new Planet(1, 200, 200);
Planet* earth = new Planet(1, 300, 300);
Planet* venus = new Planet(1, 100, 100);
Planet* mercury = new Planet(1, 100, 100);
Planet* mars = new Planet(1.0f, 100, 100);
Planet* saturn = new Planet(1.0f, 100, 100);
Planet* uranus = new Planet(1.0f, 100, 100);
Planet* neptune = new Planet(1.0f, 100, 100);

Circle* moonOrbit = new Circle(1, 100);
Circle* electronShell = new Circle(1,100);

Orbit* earthOrbit = new Orbit(1, 100);
Orbit* venusOrbit = new Orbit(1, 100);
Orbit* mercuryOrbit = new Orbit(1, 100);
Orbit* marsOrbit = new Orbit(1, 100);
Orbit* jupiterOrbit = new Orbit(1, 100);
Orbit* saturnOrbit = new Orbit(1, 100);
Orbit* uranusOrbit = new Orbit(1, 100);
Orbit* neptuneOrbit = new Orbit(1, 100);

Ring* saturnRing = new Ring(0.5f, 300);

RectangleBackground* background = new RectangleBackground(1.0f,1.0f);


std::vector<Shape*> celestials = { sun,moon,earth,venus,mercury, mars, jupiter, saturn, uranus, neptune};
std::vector<Shape*> orbits = { earthOrbit, moonOrbit, venusOrbit, mercuryOrbit, marsOrbit,jupiterOrbit, saturnOrbit, saturnRing, uranusOrbit, neptuneOrbit };

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

	for (int i = 0;i < celestials.size();i++) {		//disable reflections on all celestial bodies
		celestials[i]->material.specular = vec3(0, 0, 0);
	}

	FreeImage_Initialise(TRUE);

	sun->material.ambient = vec3(1, 1, 1);		//make sun bright

	moon->EnableTexture("moon.jpg");
	sun->EnableTexture("sun.jpg");
	//earth->EnableTexture("earth.jpg");	//2
	//earth->EnableTexture("earth3.jpg");		//1
	//earth->EnableTexture("earth4.jpg");			//0
	//earth->EnableTexture("earthClouds2.jpg");		//icy earth, v cool	
	//earth->EnableTexture("earthClouds.jpg");		//cloudy earth, also v cool	
	earth->EnableTexture("earthClouds3.jpg");		//cloudy earth, > earthClouds
//	earth->EnableTexture("earthNASA.jpg");//3
	//earth->EnableTexture("earthNight.jpg");
	venus->EnableTexture("venus.jpg");
	mars->EnableTexture("mars.jpg");
	mercury->EnableTexture("mercury.jpg");
	jupiter->EnableTexture("jupiter.jpg");
	saturn->EnableTexture("saturn.jpg");
	saturnRing->EnableTexture("saturnRing.jpg");
	uranus->EnableTexture("uranus.jpg");
	neptune->EnableTexture("neptune.jpg");
	
	background->EnableTexture("milkyWay.jpg");

	//generate Buffers

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glUseProgram(program);
	glGenBuffers(NumBuffers, Buffers);
	glGenVertexArrays(NumVAOs, VAO);
	
	glEnable(GL_TEXTURE_2D);
}

void drawBackground() {
	mat4 Projection, ModelT;
	background->material.ambient = vec3(1.0f);
	background->material.specular = vec3(0.0f);
	background->material.diffuse = vec3(0.0f);

	background->setViewPos(vec3(0, 0, 1));
	ModelT = translate(mat4(1.0f), vec3(0, 0, -100));

	Projection = ortho(-1, 1, -1, 1, 1, 2);

	background->Projection = Projection;

	glDisable(GL_DEPTH_TEST);

	background->draw();

	glEnable(GL_DEPTH_TEST);
}

float redGiantIncrement = 0;
bool redGiantEnabled = false;

void redGiant() {
	sun->setGrows(true);
}


void Planets(vec3 viewPos, mat4 Projection) {		
	vec3 orbitColor = vec3(88.0f / 255.0f);

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

	for (int i = 0; i < solarSystem.size();i++) {		//set all Projections and viewPos
		solarSystem[i]->setViewPos(viewPos);
		solarSystem[i]->Projection = Projection;
	}

	for (int i = 0;i < orbits.size();i++) {
		orbits[i]->setColor(orbitColor);
	}

	//earth

	float earthscaleparameter = 0.4f;

	//calculate earth and its rotation
	float orbitRadius = 2.9f;
	float xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);
	float earthOrbitVal = xzOrbitValue;		//for the translation of the moon orbit
	

	earth->setRevolutionSpeed(365.256f);
	earth->setRotationSpeed(5.0f);	//24h
	earth->setRotationAngleOffset(0.408407f);
	earth->setScaleParameter(earthscaleparameter);
	earth->setRadiusFromSun(orbitRadius);
	earth->draw();

	earthOrbit->setRadiusFromSun(orbitRadius);
	earthOrbit->draw(false, GL_LINES);


	//moon ---------------------------------------------------- the moon can not be handled as a planet due to the varying center of rotation

	//calculate moon and its rotation
	orbitRadius = earth->getRadiusFromSun();		//orbit around sun
	xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

	ModelR = rotate(mat4(1.0), -rotationAngle, vec3(0.0, 1.0, 0.0));
	ModelT = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));

	orbitRadius = 0.7f;		//orbit around earth
	xzOrbitValue = orbitRadius / sqrt(2);		//x = z; r = sqrt(x^2 + z^2); r = sqrt(2 * x^2); x = r/sqrt(2);

	ModelR2 = rotate(mat4(1.0), 1.0f / (2*27.0f) * 365.256f * rotationAngle, vec3(0.0, 1.0, 0.0));	//27 days for revolution, halved moons rotation speed because it looks better

	ModelT2 = translate(mat4(1.0), vec3(xzOrbitValue, 0, xzOrbitValue));

	ModelS = scale(mat4(1.0), vec3(0.27f * earthscaleparameter, 0.27f * earthscaleparameter, 0.27f * earthscaleparameter));		//earth is 0.27* the size of earth

	Model = ModelR * ModelT * ModelR2 * ModelT2 * ModelS;

	moon->Model = Model;

	moon->draw();

	//moon Orbit
	ModelS = scale(mat4(1.0f), vec3(orbitRadius, orbitRadius, 0));
	ModelR2 = rotate(mat4(1.0f), (float)M_PI / 2, vec3(1.0f, 0.0f, 0.0f));

	Model = ModelR * ModelT * ModelR2* ModelS;
	moonOrbit->Model = Model;

	moonOrbit->draw(false, GL_LINES);
	//--------------------------------------------------------------------------------------------------------
	//sun

	if (sun->getGrows()) {
		sun->setScaleParameter(sun->getScaleParameter()-0.3f + 0.31f);
		sun->material.ambient = vec3(1.0f, sun->material.ambient.y - 0.002f, sun->material.ambient.z - 0.002f);
	}
	else {
		sun->setScaleParameter(0.7f);
	}

	if (sun->getScaleParameter() > 9.0f) {
		exit(2);
	}
	sun->setRotationSpeed(0.25f);	//very slow, 25 days

	sun->draw();

	//mars
	orbitRadius = 4.0f;		//orbit around sun
	mars->setRevolutionSpeed(686.980f);
	mars->setRotationSpeed(5.0f);		//24h
	mars->setRotationAngleOffset(0.439648439f);		//25,19 deg =0.439648439 radians
	mars->setScaleParameter(earthscaleparameter * 6792.4f/ 12756.27f);		//6792.4f/ 12756.27f <=> diameter mars/diameter earth
	mars->setRadiusFromSun(orbitRadius);
	mars->draw();

	marsOrbit->setRadiusFromSun(orbitRadius);
	marsOrbit->draw(false, GL_LINES);

	//venus
	orbitRadius = 1.7f;		//orbit around sun
	venus->setRevolutionSpeed(224.701f);		//224 days for one revolution
	venus->setRotationSpeed(0.5f);		//243 days !!!! very long
	venus->setRotationAngleOffset(3.09551596);		//177,36 deg, also, venus rotates anticlockwise
	venus->setScaleParameter(earthscaleparameter * 12103.6f / 12756.27f);		// <=> diameter venus/diameter earth
	venus->setRadiusFromSun(orbitRadius);
	venus->draw();

	venusOrbit->setRadiusFromSun(orbitRadius);
	venusOrbit->draw(false, GL_LINES);

	//mercury
	orbitRadius = 1.0f;		//orbit around sun
	mercury->setRevolutionSpeed(87.969);		//87 d for revolution
	mercury->setRotationSpeed(1.5f);		//58 d -> slow
	mercury->setRotationAngleOffset(0.00059341195);		
	mercury->setScaleParameter(earthscaleparameter * 4879.4f / 12756.27f);		// <=> diameter mercury/diameter earth
	mercury->setRadiusFromSun(orbitRadius);
	mercury->draw();

	mercuryOrbit->setRadiusFromSun(orbitRadius);
	mercuryOrbit->draw(false, GL_LINES);

	//jupiter
	orbitRadius = 5.5f;		//orbit around sun
	jupiter->setRevolutionSpeed(3.0f*365.256f);		//11y, 315d, just made it slow-ish
	jupiter->setRotationSpeed(8.0f);		//9h 55 min -> faster than earth
	jupiter->setRotationAngleOffset(0.05462881f);
	jupiter->setScaleParameter(earthscaleparameter * 2);		// 142984.0f, would be too huge
	jupiter->setRadiusFromSun(orbitRadius);
	jupiter->draw();

	jupiterOrbit->setRadiusFromSun(orbitRadius);
	jupiterOrbit->draw(false, GL_LINES);

	//saturn
	orbitRadius = 7.5f;		//orbit around sun
	saturn->setRevolutionSpeed(4 * 365.256f);		//29.5y, just made it slow- ish
	saturn->setRotationSpeed(8.0f);		//10 h 33 min -> faster than earth
	saturn->setRotationAngleOffset(0.46652651f);
	saturn->setScaleParameter(earthscaleparameter * 2);		// <would be too huge
	saturn->setRadiusFromSun(orbitRadius);
	saturn->draw();

	saturnOrbit->setRadiusFromSun(orbitRadius);
	saturnOrbit->draw(false, GL_LINES);

	saturnRing->setRevolutionSpeed(4 * 365.256f);		
	saturnRing->setRotationAngleOffset(0.46652651f);
	saturnRing->setScaleParameter(earthscaleparameter * 5);		
	saturnRing->setRadiusFromSun(orbitRadius);
	saturnRing->draw();

	//uranus
	orbitRadius = 9.0f;		//orbit around sun
	uranus->setRevolutionSpeed(6 * 365.256f);		//84y, just made it slow- ish
	uranus->setRotationSpeed(6.0f);		//	17 h 14 min 24 s -> faster than earth
	uranus->setRotationAngleOffset(1.70640841f);
	uranus->setScaleParameter(earthscaleparameter * 1.2f);		// would be too huge
	uranus->setRadiusFromSun(orbitRadius);
	uranus->draw();

	uranusOrbit->setRadiusFromSun(orbitRadius);
	uranusOrbit->draw(false, GL_LINES);

	//neptune
	orbitRadius = 10.0f;		//orbit around sun
	neptune->setRevolutionSpeed(7 * 365.256f);		//164,79y , just made it slow- ish
	neptune->setRotationSpeed(6.0f);		//	15 h 57 min 59 s -> faster than earth
	neptune->setRotationAngleOffset(0.494277244f);
	neptune->setScaleParameter(earthscaleparameter * 1.3f);		// would be too huge
	neptune->setRadiusFromSun(orbitRadius);
	neptune->draw();

	neptuneOrbit->setRadiusFromSun(orbitRadius);
	neptuneOrbit->draw(false, GL_LINES);
}

void CarbonAtom() {		//!!!GLOABAL!!! Circle* electronShell = new Circle(); AND Sphere* sphere = new Sphere(1, 30, 30); are required; Sphere Parameters do not matter, as long as Radius equals 1
	
	/*
	Make sure that CORE = 1, SHELLS = 1, ELECTRONS = 1
	RANDOMELECTRONS = 1 makes the electrons go wild
	RANDOMELECTRONS = false synchronizes them
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

	Projection = ortho((float)width / (float)height * -5.0f, (float)width/(float)height*5.0f, -5.0f, 5.0f, 5.0f, 15.0f);

	sphere->Projection = Projection;
	sphere->pointLightEnabled = false;
	sphere->lightLocation = vec3(2.0f, 0, 5.0f);
	electronShell->Projection = Projection;
	electronShell->material.ambient = vec3(1.0f);

	//draw the core, i.e. neutrons and protons
	if (CORE) {	
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

	ModelS = mat4(1.0);
	ModelS = scale(ModelS, vec3(4.0f));

	ModelR = rotate(mat4(1.0f), 3.0f / 7.0f * (float)M_PI,vec3(0.0f, 1.0f, 0.0f));
	
	electronShell->Model =  ModelS;

	if (SHELLS) {
		for (int i = 0; i < 6;i++) {
			vec3 EulerAngles(0, electronShellAngle, i * electronShellAngleY);

			quat MyQuaternion;
			MyQuaternion = quat(EulerAngles);
			ModelR = toMat4(MyQuaternion);

			Model = ModelR * ModelS;

			electronShell->setColor(vec3(0.0f));
			electronShell->Model = Model;

			electronShell->draw(false,GL_LINES);
		}
	} 

	//draw electrons
	Model = mat4(1.0);
	ModelR = mat4(1.0);
	ModelR2 = mat4(1.0f);
	ModelT = mat4(1.0);
	ModelS = mat4(1.0f);

	sphere->setColor(vec3(51.0f / 255.0f, 51.0f / 255.0f, 255.0f / 255.0f));		//darker blue for electrons

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

GLuint state = 1;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	if (state == 3)
	{
		glClearColor(0.5, 0., 0.5, 0);
		CarbonAtom();
	}
	else if (state == 2) {
		rotationAngle;
		float aspect = float(width) / float(height) * 2;		//*2 because we half the screen
		mat4 Projection;

		drawBackground();
		//top
		glViewport(0, 0.5 * height, width, 0.5 * height);
		Projection = ortho(-5.0f, 5.0f, -5.0f,5.0f, -20.0f, 20.0f);
		Planets(vec3(0, 10, -0.1f), ortho(aspect * -5.0f, aspect * 5.0f, -5.0f, 5.0f, -20.0f, 20.0f));
		//bottom
		glViewport(0, 0, width, 0.5 * height);
		Projection  = perspective(45.0f, aspect, 0.0001f, 40.0f);
		Planets(vec3(0, 3.5f, 10.0f), Projection);
	}
	else if (state == 1) {
		GLfloat viewpoint[3];
		viewpoint[0] = dist * sin(beta) * sin(alpha);
		viewpoint[1] = dist * cos(beta);
		viewpoint[2] = dist * sin(beta) * cos(alpha);

		vec3 viewPos = vec3(viewpoint[0], viewpoint[1], viewpoint[2]);

		mat4 Projection = perspective(45.0f, float(width) / float(height), 0.0001f, 40.0f);
		drawBackground();
		Planets(viewPos, Projection);
	}
	glFlush();
}

bool texturesOn = true;
void switchTextures() {
	if (texturesOn) {
		for (int i = 0;i < celestials.size();i++) {		
			celestials[i]->DisableTexture();
			float random = (float)(rand() % 10 + 1) / 10;
			float random2 = (float)(rand() % 10 + 1) / 10;
			float random3 = (float)(rand() % 10 + 1) / 10;
			celestials[i]->setColor(vec3(random, random2, random3));
			if ((float)(rand() % 10 + 1) < 5) {
				celestials[i]->material.specular = vec3(1.0f);
			}
		}
	}
	else {
		for (int i = 0;i < celestials.size();i++) {		//disable reflections on all celestial bodies
			celestials[i]->material.specular = vec3(0.0f);
		}
		moon->EnableTexture("moon.jpg");
		sun->EnableTexture("sun.jpg");
		earth->EnableTexture("earthClouds3.jpg");		
		venus->EnableTexture("venus.jpg");
		mars->EnableTexture("mars.jpg");
		mercury->EnableTexture("mercury.jpg");
		jupiter->EnableTexture("jupiter.jpg");
		saturn->EnableTexture("saturn.jpg");
		saturnRing->EnableTexture("saturnRing.jpg");
		uranus->EnableTexture("uranus.jpg");
		neptune->EnableTexture("neptune.jpg");
	}
	texturesOn = !texturesOn;
}


bool spotLightOn = true;
void switchSpotlight() {
	if (spotLightOn) {
		for (int i = 0;i < celestials.size();i++) {
			celestials[i]->pointLightEnabled = false;
			celestials[i]->lightLocation = vec3(10.0f,0.0f,0.0f);
		}
	}
	else {
		for (int i = 0;i < celestials.size();i++) {
			celestials[i]->pointLightEnabled = true;
		}
	}
	spotLightOn = !spotLightOn;
}



void keyboard(unsigned char theKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = height - mouseY;
	switch (theKey) {
	case 'v': dist -= DELTA; display(); break;
	case 'z': dist += DELTA; display(); break;
	case 't': switchTextures(); display(); break;
	case 'l': switchSpotlight(); display(); break;
	case 'g': redGiant(); display(); break;
	case '1': state = 1; display(); break;
	case '2': state = 2; display(); break;
	case '3': state = 3; display(); break;
	case 'r': RANDOMELECTRONS = !RANDOMELECTRONS; display(); break;
	case 'e': exit(-1);
	}
}

void mouse(int theButton, int State, int mouseX, int mouseY) {
	GLint x = mouseX; GLint y = height - mouseY;
}

void motion(int mouseX, int mouseY) {
	if (mouseX < (width / 2)) { alpha -= (mouseX - (width / 2)) / 10000.0; }
	else { alpha -= (mouseX - (width / 2)) / 10000.0; }
	if (mouseY < (height / 2)) { beta -= (mouseY - (height / 2)) / 10000.0; }
	else { beta -= (mouseY - (height / 2)) / 10000.0; }
	display();
}

void special(int specKey, int mouseX, int mouseY) {
	GLint x = mouseX;
	GLint y = height - mouseY;
	switch (specKey) {
	case GLUT_KEY_LEFT: alpha -= DELTA; display(); break;
	case GLUT_KEY_RIGHT: alpha += DELTA; display(); break;
	case GLUT_KEY_UP: beta -= DELTA; display(); break;
	case GLUT_KEY_DOWN: display(); beta += DELTA; break;
	}
}

void reshape(int w, int h) {
	width = w;
	height = h;
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	//glutInitWindowSize(2560, 1440);
	glutInitWindowSize(1920, 1080);
	//glutInitWindowSize(500, 500);
	glutInitContextVersion(4, 5);  // (4,2) (3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//GLUT_COMPATIBILITY_PROFILE
	glutCreateWindow("CG-Beleg");
	glewExperimental = GL_TRUE;
	if (glewInit()) printf("Error");
	init();
	glutFullScreen();
	timer(0);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
}
