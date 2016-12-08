#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif

#include "LevelLoader.h"
#include "math3D.h"
#include <vector>

Level::Level(int i) {
	switch(i) {
		case 1:
			// Object leftWall = new Object();
			// Object rightWall = new Object();
			// Object backWall = new Object();
			// Object wall4 = 
		break;

		case 2:

		break;

		case 3:

		break;
	}
}

Object::Object() {}

Face::Face() {}

