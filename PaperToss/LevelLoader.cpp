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
#include <stdio.h>
Face::Face(){}

Face::Face(point3D min, point3D max) {

	if (min.x == max.x) {
		p0.x = min.x; p0.y = min.y; p0.z = min.z;
		p1.x = min.x; p1.y = max.y; p1.z = min.z;
		p2.x = min.x; p2.y = max.y; p2.z = max.z;
		p3.x = min.x; p3.y = min.y; p3.z = max.z;

	}
	else if (min.y == max.y) {
		p0.x = min.x; p0.y = min.y; p0.z = min.z;
		p1.x = max.x; p1.y = min.y; p1.z = min.z;
		p2.x = max.x; p2.y = min.y; p2.z = max.z;
		p3.x = min.x; p3.y = min.y; p3.z = max.z;
	}
	else if (min.z == max.z) {
		p0.x = min.x; p0.y = min.y; p0.z = min.z;
		p1.x = min.x; p1.y = max.y; p1.z = min.z;
		p2.x = max.x; p2.y = max.y; p2.z = min.z;
		p3.x = max.x; p3.y = min.y; p3.z = min.z;
	}

	calcNormal();
}

void Face::calcNormal() { 
	vec3D t1, t2, norm;

	t1 = t1.createVector(p0, p1);
	t2 = t2.createVector(p2, p1);

	norm = t1.crossProduct(t2);

	if (norm.x == 0.0f) norm.x = 0.0f;
	if (norm.y == 0.0f) norm.y = 0.0f;
	if (norm.z == 0.0f) norm.z = 0.0f;

	normal = norm.normalize();

	printf("%f, %f, %f\n", normal.x, normal.y, normal.z);
};


Object::Object(){}

Object::Object(objectType o, point3D min, point3D max) {
	t = o;
	objectFaces = new vector<Face*>();

	switch (t) {
		case plane:
		{	
			Face *plane = new Face(min, max);
			objectFaces->push_back(plane);
		}

	}

}

Level::Level() {
	point3D lBoundMin;
	point3D lBoundMax;

	point3D rBoundMin;
	point3D rBoundMax;

	point3D bBoundMin;
	point3D bBoundMax;

	point3D cBoundMin;
	point3D cBoundMax;

	point3D fBoundMin;
	point3D fBoundMax;

	lvl = 1;

	levelObjects = new vector<Object*>();

	switch(lvl) {
		case 1:
			lBoundMin.x = -10; lBoundMin.y = 0; lBoundMin.z = 10;
			lBoundMax.x = -10; lBoundMax.y = 10; lBoundMax.z = -10;

			rBoundMin.x = 10; rBoundMin.y = 0; rBoundMin.z = -10;
			rBoundMax.x = 10; rBoundMax.y = 10; rBoundMax.z = 10;

			bBoundMin.x = -10; bBoundMin.y = 0; bBoundMin.z = -10;
			bBoundMax.x = 10; bBoundMax.y = 10; bBoundMax.z = -10;

			cBoundMin.x = 10; cBoundMin.y = 10; cBoundMin.z = -10;
			cBoundMax.x = -10; cBoundMax.y = 10; cBoundMax.z = 10;

			fBoundMin.x = -10; fBoundMin.y = 0; fBoundMin.z = -10;
			fBoundMax.x = 10; fBoundMax.y = 0; fBoundMax.z = 10;

		break;

		case 2:

		break;

		case 3:

		break;
	}
		printf("Left wall\n");
		Object* leftWall = new Object(plane, lBoundMin, lBoundMax);
		printf("Right wall\n");
		Object* rightWall = new Object(plane, rBoundMin, rBoundMax);
		printf("Back wall\n");
		Object* backWall = new Object(plane, bBoundMin, bBoundMax);
		printf("Floor\n");
		Object* floor = new Object(plane, fBoundMin, fBoundMax);
		printf("Ceiling\n");
		Object* ceiling = new Object(plane, cBoundMin, cBoundMax);

		levelObjects->push_back(leftWall);
		levelObjects->push_back(rightWall);
		levelObjects->push_back(backWall);
		levelObjects->push_back(floor);
		levelObjects->push_back(ceiling);
}

void Level::drawLevel() {
	const int objects = levelObjects->size();
	//printf("There are %i objects\n", objects);
	if (objects > 0) {
		for (int i = 0; i < objects; i++) {
			levelObjects->at(i)->drawObject();
		}
	}
}

vector<int> Level::testIntersection(float x, float y, float z) {
	const int objects = levelObjects->size();
	int intersectID;
	//printf("There are %i objects\n", objects);
	if (objects > 0) {
		for (int i = 0; i < objects; i++) {
			intersectID = levelObjects->at(i)->testIntersection(x, y, z);
			if (intersectID!= -1) {
				return vector<int>(i, intersectID);
			}
		}
	}
	return vector<int>(-1, -1);
}

void Object::drawObject() {
	const int faces = objectFaces->size();
	//printf("There are %i faces\n", faces);
	if (faces > 0) {
		for (int i = 0; i < faces; i++) {
			objectFaces->at(i)->drawFace();
		}
	}
}

int Object::testIntersection(float x, float y, float z) {
	const int faces = objectFaces->size();
	//printf("There are %i faces\n", faces);
	if (faces > 0) {
		for (int i = 0; i < faces; i++) {
			if (objectFaces->at(i)->testIntersection(x, y, z)) {
				return i;
			}
		}
	}
	return -1;
}

void Face::drawFace() {
	//printf("Drawing a face\n");
	glBegin(GL_QUADS);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	glEnd();
}

float alpha = 0.05;

bool Face::testIntersection(float x, float y, float z) {

	if (min.x == max.x) {
		if (x > (min.x - alpha) && x < (min.x + alpha)){
			if (y > min.y && y < max.y && z > min.z && z < max.z) {
				return true;
			}
		}
		
	}
	else if (min.y == max.y) {
		if (y > (min.y - alpha) && y < (min.y + alpha)){
			if (x > min.y && x < max.y && z > min.z && z < max.z) {
				return true;
			}
		}

	}
	else if (min.z == max.z) {
		if (z > (min.z - alpha) && z < (min.z + alpha)){
			if (x > min.y && x < max.y && y > min.z && y < max.z) {
				return true;
			}
		}
	
	}
	return false;
}


