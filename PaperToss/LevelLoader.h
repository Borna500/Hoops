#ifndef __LEVEL_LOADER_H__	
#define __LEVEL_LOADER_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include "math3D.h"
#include <vector>

using namespace std;

enum objectType{plane, box};
enum ballType{normal, rock, bouncy};
//std::vector<float[4][4]> materials;
class Face;
class Object;
class Level;

class Face {
public:
	Face();
	Face(point3D min, point3D max, int normalflip);
	point3D min;
	point3D max;
	point3D p0;
	point3D p1; 
	point3D p2;
	point3D p3;
	vec3D normal;


	bool testIntersection(float x, float y, float z, float ballRad);
	void drawFace(int i);
	void calcNormal(int normalflip);
	
};

class Object {
public:
	Object();
	Object(objectType o, point3D min, point3D max, int normalflip, int materialIndex);
	objectType t;
	vector<Face*> *objectFaces;
    int materialIndex;
	int testIntersection(float x, float y, float z, float ballRad);
	void drawObject();
	
};

class Level {
public:
	Level();
	Level(int l);
	int lvl;
	int score;
	//Ball type, size, and bouncines
	ballType ball;
	float ballRadius;
	//1 = full bounce, 0 = no bounce
	float ballBounciness;

	//Starting position of ball
	point3D startingPosition;

	//Object list
	vector<Object*> *levelObjects;

	//Basket object, position, and radius
	Object* basket;
	vec3D basketPosition1;
	vec3D basketPosition2;
	vec3D basketPosition3;
	vec3D basketPosition4;
	vec3D basketPosition5;

	float basketRadius;

	vector<int> testIntersection(float x, float y, float z, float ballRad);
	void drawLevel();
	void updateBasket();
	

};






#endif