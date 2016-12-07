/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/
#ifndef MATH_3D_H
#define MATH_3D_H

//defining each of the classes
class point3D;
class vec3D;
class vec4D;
class colour;

//class of points in 3D
class point3D{
public:
	float x, y, z;
	point3D();
	point3D(float xIn, float yIn, float zIn);	//takes in points for x, y and z

	float distance (point3D p2);	//distance forumla
	float fastDistance(point3D p2);	//fastpoint formula
	point3D movePoint(vec3D v);	//moving a point

};

//class of vectors in 3D
class vec3D{
public:
	float x, y, z, mag;
	vec3D();
	vec3D(float xIn, float yIn, float zIn);	//takes in x, y, z values 

	float length();	//finding the length 
	vec3D normalize();	//normalize function
	vec3D crossProduct(vec3D p2);	//finding the cross product function
	vec3D createVector(point3D p1, point3D p2);	//creating a vector between two points
	vec3D addVector(vec3D p2);	//adding a vector 
	vec3D vectorMultiply(float scalar);	//multiplying a vector
};

//class of vectors in 4D
class vec4D{
public:
	float w, x, y, z;
	vec4D();
	vec4D(float wIn, float xIn, float yIn, float zIn);	//taking in w, x, y and z values
};


//overloaded + - and * operators for vectors 
vec3D operator+ (vec3D v1, vec3D v2);
vec3D operator- (vec3D v1, vec3D v2);
vec3D operator* (vec3D v1, vec3D v2);

#endif