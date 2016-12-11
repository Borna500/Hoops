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
float ma[] = {0.1745, 0.01175, 0.01175, 1.0};
float md[] = {0.61424, 0.04136, 0.04136, 1.0};
float ms[] = {0.727811, 0.626959, 0.626959, 1.0};

float ma2[] = {0.0215, 0.1745, 0.0215, 1.0};
float md2[] = {0.07568, 0.61424, 0.07568, 1.0};
float ms2[] = {0.633, 0.727811, 0.633, 1.0};

float ma3[] = {0.05375, 0.05, 0.06625, 1.0};
float md3[] = {0.18275, 0.17, 0.22525, 1.0};
float ms3[] = {0.332741, 0.328634, 0.346435, 1.0};

float ma4[] = {0.25, 0.20725, 0.20725, 1.0};
float md4[] = {1, 0.829, 0.829, 1.0};
float ms4[] = {0.296648, 0.296648, 0.296648, 1.0};

float sh = 1.0;
float sh2 = 3.2;
float sh3 = 1.0;
float sh4 = 0.7;

Face::Face(){}

Face::Face(point3D min, point3D max, int normalflip, int material) {

	printf("ok so %f %f %f %f %f %f\n", min.x, min.y, min.z, max.x, max.y, max.z);
	materialIndex = material;

	if (min.x == max.x) {
		p0.x = min.x; p0.y = min.y; p0.z = min.z;
		p1.x = min.x; p1.y = max.y; p1.z = min.z;
		p2.x = min.x; p2.y = max.y; p2.z = max.z;
		p3.x = min.x; p3.y = min.y; p3.z = max.z;

	}
	else if (min.y == max.y) {
		p0.x = min.x; p0.y = min.y; p0.z = min.z;
		p1.x = min.x; p1.y = min.y; p1.z = max.z;
		p2.x = max.x; p2.y = min.y; p2.z = max.z;
		p3.x = max.x; p3.y = min.y; p3.z = min.z;
	}
	else if (min.z == max.z) {
		p0.x = min.x; p0.y = min.y; p0.z = min.z;
		p1.x = max.x; p1.y = min.y; p1.z = min.z;
		p2.x = max.x; p2.y = max.y; p2.z = min.z;
		p3.x = min.x; p3.y = max.y; p3.z = min.z;
	}

	this->min = min;
	this->max = max;

	calcNormal(normalflip);
}

void Face::calcNormal(int normalflip) { 
	vec3D t1, t2, norm;

	t1 = t1.createVector(p1, p0);
	t2 = t2.createVector(p2, p0);

	norm = t1.crossProduct(t2);

	norm.x = norm.x * normalflip;
	norm.y = norm.y * normalflip;
	norm.z = norm.z * normalflip;

	if (norm.x == 0.0f) norm.x = 0.0f;
	if (norm.y == 0.0f) norm.y = 0.0f;
	if (norm.z == 0.0f) norm.z = 0.0f;

	normal = norm.normalize();

	printf("%f, %f, %f\n", normal.x, normal.y, normal.z);
};


Object::Object(){}

Object::Object(objectType o, point3D min, point3D max, int normalflip, int material) {
	t = o;
	objectFaces = new vector<Face*>();
    materialIndex = material;

    point3D frontMin;
    point3D frontMax;

    point3D backMin;
    point3D backMax;

    point3D leftMin;
    point3D leftMax;

    point3D rightMin;
    point3D rightMax;

    point3D bottomMin;
    point3D bottomMax;

    point3D topMin;
    point3D topMax;
    
	switch (t) {
		case plane:
		{	
			Face *plane = new Face(min, max, normalflip, material);
			objectFaces->push_back(plane);
		}
		break;
		case box:
		{
			backMin.x = min.x; backMin.y = min.y; backMin.z = min.z;
			backMax.x = max.x; backMax.y = max.y; backMax.z = min.z;

			frontMin.x = min.x; frontMin.y = min.y; frontMin.z = max.z;
			frontMax.x = max.x; frontMax.y = max.y; frontMax.z = max.z;

			leftMin.x = min.x; leftMin.y = min.y; leftMin.z = min.z;
			leftMax.x = min.x; leftMax.y = max.y; leftMax.z = max.z;

			rightMin.x = max.x; rightMin.y = min.y; rightMin.z = min.z;
			rightMax.x = max.x; rightMax.y = max.y; rightMax.z = max.z;

			bottomMin.x = min.x; bottomMin.y = min.y; bottomMin.z = min.z;
			bottomMax.x = max.x; bottomMax.y = min.y; bottomMax.z = max.z;

			topMin.x = min.x; topMin.y = max.y; topMin.z = min.z;
			topMax.x = max.x; topMax.y = max.y; topMax.z = max.z;

			Face *back = new Face(backMin, backMax, -1, material);
			Face *front = new Face(frontMin, frontMax, 1, material);
			Face *left = new Face(leftMin, leftMax, -1, material);
			Face *right = new Face(rightMin, rightMax, 1, material);
			Face *bottom = new Face(bottomMin, bottomMax, -1, material);
			Face *top = new Face(topMin, topMax, 1, material);

			objectFaces->push_back(back);
			objectFaces->push_back(front);
			objectFaces->push_back(left);
			objectFaces->push_back(right);
			objectFaces->push_back(bottom);
			objectFaces->push_back(top);
		}
		break;


	}

}

Level::Level() {}

Level::Level(int l) {
	point3D lBoundMin;
	point3D lBoundMax;

	point3D rBoundMin;
	point3D rBoundMax;

	point3D bBoundMin;
	point3D bBoundMax;

	point3D dBoundMin;
	point3D dBoundMax;

	point3D cBoundMin;
	point3D cBoundMax;

	point3D fBoundMin;
	point3D fBoundMax;

	point3D obs1BoundMin;
	point3D obs1BoundMax;


	point3D obs2BoundMin;
	point3D obs2BoundMax;

	point3D obs3BoundMin;
	point3D obs3BoundMax;

	point3D obs4BoundMin;
	point3D obs4BoundMax;

	point3D obs5BoundMin;
	point3D obs5BoundMax;

	Object* obstacle1;
	Object* obstacle2;
	Object* obstacle3;
	Object* obstacle4;
	Object* obstacle5;

	int wallmat;


	lvl = l;

	levelObjects = new vector<Object*>();

	switch(lvl) {
		case 3:

			ballRadius = 0.5; 
			ballBounciness = 0.5;
			ball = normal;

			startingPosition.x = 0; startingPosition.y = 1; startingPosition.z = 4;

			lBoundMin.x = -10; lBoundMin.y = 0; lBoundMin.z = -10;
			lBoundMax.x = -10; lBoundMax.y = 10; lBoundMax.z = 10;

			rBoundMin.x = 10; rBoundMin.y = 0; rBoundMin.z = -10;
			rBoundMax.x = 10; rBoundMax.y = 10; rBoundMax.z = 10;

			bBoundMin.x = -10; bBoundMin.y = 0; bBoundMin.z = -10;
			bBoundMax.x = 10; bBoundMax.y = 10; bBoundMax.z = -10;

			dBoundMin.x = -10; dBoundMin.y = 0; dBoundMin.z = 10;
			dBoundMax.x = 10; dBoundMax.y = 10; dBoundMax.z = 10;

			cBoundMin.x = -10; cBoundMin.y = 10; cBoundMin.z = -10;
			cBoundMax.x = 10; cBoundMax.y = 10; cBoundMax.z = 10;

			fBoundMin.x = -10; fBoundMin.y = 0; fBoundMin.z = -10;
			fBoundMax.x = 10; fBoundMax.y = 0; fBoundMax.z = 10;

			basketPosition1.x = 0; basketPosition1.y = 2; basketPosition1.z = -4;
			basketPosition2.x = 3; basketPosition2.y = 1; basketPosition2.z = -6;
			basketPosition3.x = -3; basketPosition3.y = 2; basketPosition3.z = -8;
			basketPosition4.x = 5; basketPosition4.y = 2; basketPosition4.z = -9;
			basketPosition5.x = 3; basketPosition5.y = 3; basketPosition5.z = -4;

			basketRadius = 1;

			wallmat = 2;

		break;

		case 1:
			ballRadius = 1; 
			ballBounciness = 0.9;
			ball = bouncy;

			startingPosition.x = 0; startingPosition.y = 1; startingPosition.z = 9;

			lBoundMin.x = -20; lBoundMin.y = 0; lBoundMin.z = -20;
			lBoundMax.x = -20; lBoundMax.y = 20; lBoundMax.z = 20;

			rBoundMin.x = 20; rBoundMin.y = 0; rBoundMin.z = -20;
			rBoundMax.x = 20; rBoundMax.y = 20; rBoundMax.z = 20;

			bBoundMin.x = -20; bBoundMin.y = 0; bBoundMin.z = -20;
			bBoundMax.x = 20; bBoundMax.y = 20; bBoundMax.z = -20;

			dBoundMin.x = -20; dBoundMin.y = 0; dBoundMin.z = 20;
			dBoundMax.x = 20; dBoundMax.y = 20; dBoundMax.z = 20;

			cBoundMin.x = -20; cBoundMin.y = 20; cBoundMin.z = -20;
			cBoundMax.x = 20; cBoundMax.y = 20; cBoundMax.z = 20;

			fBoundMin.x = -20; fBoundMin.y = 0; fBoundMin.z = -20;
			fBoundMax.x = 20; fBoundMax.y = 0; fBoundMax.z = 20;

			obs1BoundMin.x = -5; obs1BoundMin.y = 0; obs1BoundMin.z = -3;
			obs1BoundMax.x = 5; obs1BoundMax.y = 2; obs1BoundMax.z = 3;

			obs2BoundMin.x = -5; obs2BoundMin.y = 6; obs2BoundMin.z = -2;
			obs2BoundMax.x = 5; obs2BoundMax.y = 9; obs2BoundMax.z = 1;

			obs3BoundMin.x = -20; obs3BoundMin.y = 4; obs3BoundMin.z = -3;
			obs3BoundMax.x = -15; obs3BoundMax.y = 7; obs3BoundMax.z = 3;

			obs4BoundMin.x = 15; obs4BoundMin.y = 4; obs4BoundMin.z = -3;
			obs4BoundMax.x = 20; obs4BoundMax.y = 7; obs4BoundMax.z = 3;

			obs5BoundMin.x = -6; obs5BoundMin.y = 0; obs5BoundMin.z = 12;
			obs5BoundMax.x = 6; obs5BoundMax.y = 7; obs5BoundMax.z = 15;

			obstacle1 = new Object(box, obs1BoundMin, obs1BoundMax, 1, 1);
			obstacle2 = new Object(box, obs2BoundMin, obs2BoundMax, 1, 1);
			obstacle3 = new Object(box, obs3BoundMin, obs3BoundMax, 1, 1);
			obstacle4 = new Object(box, obs4BoundMin, obs4BoundMax, 1, 1);
			obstacle5 = new Object(box, obs5BoundMin, obs5BoundMax, 1, 1);

			levelObjects->push_back(obstacle1);
			levelObjects->push_back(obstacle2);
			levelObjects->push_back(obstacle3);
			levelObjects->push_back(obstacle4);
			levelObjects->push_back(obstacle5);



			basketPosition1.x = 1; basketPosition1.y = 2; basketPosition1.z = -4;
			basketPosition2.x = -3; basketPosition2.y = 2; basketPosition2.z = -7;
			basketPosition3.x = 10; basketPosition3.y = 4; basketPosition3.z = 15;
			basketPosition4.x = 3; basketPosition4.y = 5; basketPosition4.z = -15;
			basketPosition5.x = 0; basketPosition5.y = 5; basketPosition5.z = -8;

			basketRadius = 1;

			wallmat = 3;

		break;

		case 2:
			ballRadius = 1; 
			ballBounciness = 0.2;
			ball = rock;

			startingPosition.x = 0; startingPosition.y = 1; startingPosition.z = 7;

			lBoundMin.x = -15; lBoundMin.y = 0; lBoundMin.z = -15;
			lBoundMax.x = -15; lBoundMax.y = 10; lBoundMax.z = 10;

			rBoundMin.x = 15; rBoundMin.y = 0; rBoundMin.z = -15;
			rBoundMax.x = 15; rBoundMax.y = 10; rBoundMax.z = 10;

			bBoundMin.x = -15; bBoundMin.y = 0; bBoundMin.z = -15;
			bBoundMax.x = 15; bBoundMax.y = 10; bBoundMax.z = -15;

			dBoundMin.x = -15; dBoundMin.y = 0; dBoundMin.z = 10;
			dBoundMax.x = 15; dBoundMax.y = 10; dBoundMax.z = 10;

			cBoundMin.x = -15; cBoundMin.y = 10; cBoundMin.z = -15;
			cBoundMax.x = 15; cBoundMax.y = 10; cBoundMax.z = 10;

			fBoundMin.x = -15; fBoundMin.y = 0; fBoundMin.z = -15;
			fBoundMax.x = 15; fBoundMax.y = 0; fBoundMax.z = 10;

			obs1BoundMin.x = -9; obs1BoundMin.y = 0; obs1BoundMin.z = -10;
			obs1BoundMax.x = -4; obs1BoundMax.y = 5; obs1BoundMax.z = -5;

			obs2BoundMin.x = 5; obs2BoundMin.y = 5; obs2BoundMin.z = -7;
			obs2BoundMax.x = 10; obs2BoundMax.y = 10; obs2BoundMax.z = -2;

			obs3BoundMin.x = -1; obs3BoundMin.y = 0; obs3BoundMin.z = -13;
			obs3BoundMax.x = 5; obs3BoundMax.y = 10; obs3BoundMax.z = -8;

			obstacle1 = new Object(box, obs1BoundMin, obs1BoundMax, 1, 1);
			obstacle2 = new Object(box, obs2BoundMin, obs2BoundMax, 1, 1);
			obstacle3 = new Object(box, obs3BoundMin, obs3BoundMax, 1, 1);

			levelObjects->push_back(obstacle1);
			levelObjects->push_back(obstacle2);
			levelObjects->push_back(obstacle3);

			basketPosition1.x = 0; basketPosition1.y = 2; basketPosition1.z = -5;
			basketPosition2.x = -5; basketPosition2.y = 2; basketPosition2.z = -3;
			basketPosition3.x = -2; basketPosition3.y = 1; basketPosition3.z = -9;
			basketPosition4.x = 4; basketPosition4.y = 3; basketPosition4.z = -3;
			basketPosition5.x = 7; basketPosition5.y = 2; basketPosition5.z = -5;
			basketRadius = 1;

			wallmat = 4;

		break;
	}
		printf("Left wall\n");
		Object* leftWall = new Object(plane, lBoundMin, lBoundMax, 1, wallmat);
		printf("Right wall\n");
		Object* rightWall = new Object(plane, rBoundMin, rBoundMax, -1, wallmat);
		printf("Back wall\n");
		Object* backWall = new Object(plane, bBoundMin, bBoundMax, 1, wallmat);

		Object* frontWall = new Object(plane, dBoundMin, dBoundMax, -1, wallmat);
		printf("Floor\n");
		Object* floor = new Object(plane, fBoundMin, fBoundMax, 1, wallmat);
		printf("Ceiling\n");
		Object* ceiling = new Object(plane, cBoundMin, cBoundMax, -1, wallmat);

		levelObjects->push_back(leftWall);
		levelObjects->push_back(rightWall);
		levelObjects->push_back(backWall);
		levelObjects->push_back(frontWall);
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

vector<int> Level::testIntersection(float x, float y, float z, float ballRad) {
	const int objects = levelObjects->size();
	int intersectID;
	vector<int> IDs;
	//printf("There are %i objects\n", objects);
	if (objects > 0) {
		for (int i = 0; i < objects; i++) {
			intersectID = levelObjects->at(i)->testIntersection(x, y, z, ballRad);
			if (intersectID != -1) {
				IDs.push_back(i);
				IDs.push_back(intersectID);
				return IDs;
			}
		}
	}
	IDs.push_back(-1);
	IDs.push_back(-1);
	return IDs;
}

void Object::drawObject() {
	const int faces = objectFaces->size();
	//printf("There are %i faces\n", faces);
	if (faces > 0) {
		for (int i = 0; i < faces; i++) {
			objectFaces->at(i)->drawFace(0);
		}
	}
}

int Object::testIntersection(float x, float y, float z, float ballRad) {
	const int faces = objectFaces->size();
	//printf("There are %i faces\n", faces);
	if (faces > 0) {
		for (int i = 0; i < faces; i++) {
			if (objectFaces->at(i)->testIntersection(x, y, z, ballRad)) {
				return i;
			}
		}
	}
	return -1;
}


void Face::drawFace(int i) { // i is the index of the material for this face
	//printf("Drawing a face\n");
	switch(materialIndex) {
		case 1:
    		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ma); //putting material onto the terrain
   			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  ms);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, md);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  sh*128);
    	break;
		case 2:
		    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ma2); //putting material onto the terrain
   			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  ms2);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, md2);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  sh2*128);
    	break;
    	case 3:
		    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ma3); //putting material onto the terrain
   			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  ms3);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, md3);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  sh3*128);
    	break;
    	case 4:
		    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ma4); //putting material onto the terrain
   			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  ms4);
    		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, md4);
    		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  sh4*128);
    	break;

	}

	glBegin(GL_QUADS);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	glEnd();
}

//Ball radius
float alpha = 1.0f;

bool Face::testIntersection(float x, float y, float z, float ballRad) {

	//Hitting the left side of the object
	if (normal.x == 1) {
		if (x - ballRad < min.x && x + ballRad > min.x){
			if (y >= min.y && y <= max.y && z >= min.z && z <= max.z) {
				//printf("OOPsx+\n");
				return true;
			}
		}
		
	}
	//Hitting the right side of the object
	else if (normal.x == -1) {
		if (x + ballRad > max.x && x - ballRad < max.x){
			if (y >= min.y && y <= max.y && z >= min.z && z <= max.z) {
				//printf("OOPsx-\n");
				return true;
			}
		}

	}
	//Hitting the bottom of the face
	else if (normal.y == 1) {
		if (y - ballRad < min.y && y + ballRad > max.y){
			if (x >= min.x && x <= max.x && z >= min.z && z <= max.z) {
				//printf("OOPsy+\n");
				return true;
			}
		}
	}
	//Hitting the top of the face
	else if (normal.y == -1) {
		if (y + ballRad > max.y && y - ballRad < max.y){
			if (x >= min.x && x <= max.x && z >= min.z && z <= max.z) {
				return true;
			}
		}
	}

	//Hitting the front of an object
	else if (normal.z == 1) {
		if (z - ballRad < min.z && z + ballRad > min.z){
			if (x >= min.x && x <= max.x && y >= min.y && y <= max.y) {
				printf("Intersected with %f %f\n", min.z, z);
				return true;
			}
		}
	
	}

	//Hitting the back of an object
	else if (normal.z == -1) {
		if (z + ballRad > max.z && z - ballRad < max.z){
			if (x >= min.x && x <= max.x && y >= min.y && y <= max.y) {
				return true;
			}
		}
	
	}
	return false;
}

//void Level::loadMaterials(){
//    float matList[4][4] = {
//        {0.135, 0.2225, 0.1575, 1.0}, //setting the material for the ambient, diffuse and specular values
//        {0.54, 0.89, 0.63, 1.0},
//        {0.316228, 0.316228, 0.316228, 1.0},
//        {0.1}
//    };
//    materials.push_back(matList);
//    float matList1[4][4] = {
//        {0.0, 0.0, 0.0, 1.0}, //setting the material for the ambient, diffuse and specular values black plastic
//        {0.01, 0.01, 0.01, 1.0},
//        {0.50, 0.50, 0.50, 1.0},
//        {0.25}
//    };
//    materials.push_back(matList1);
//
//}
