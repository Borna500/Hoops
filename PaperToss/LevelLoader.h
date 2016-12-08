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

class Level;
class Object;
class Face;

class Level {
	Level(int i);
	vector<Object*> *levelobjects;

};

class Object {
	Object();
	vector<Face*> *faces;
};

class Face {
	Face();
	vector<vec3D*> *points;
};

#endif