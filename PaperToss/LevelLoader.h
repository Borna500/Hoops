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

class Face;
class Object;
class Level;

class Face {
public:
	Face();
	Face(point3D min, point3D max);
	point3D min;
	point3D max;
	point3D p0;
	point3D p1; 
	point3D p2;
	point3D p3;
	vec3D normal;


	bool testIntersection(float x, float y, float z);
	void drawFace();
	void calcNormal();
	
};

class Object {
public:
	Object();
	Object(objectType o, point3D min, point3D max);
	objectType t;
	vector<Face*> *objectFaces;

	int testIntersection(float x, float y, float z);
	void drawObject();
	
};

class Level {
public:
	Level();
	int lvl; 	
	vector<Object*> *levelObjects;
	Object basket;
	vec3D basketPosition;

	vector<int> testIntersection(float x, float y, float z);
	void drawLevel();
	

};





#endif