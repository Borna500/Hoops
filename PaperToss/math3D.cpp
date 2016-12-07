/* 3GC3 Assignment 3
Name: Sean McKay, Rebecca Tran
Student Numbers: 1423885, 1425611
MacID: mckaysm, tranr5
*/
#include <math.h>
#include "math3D.h"


point3D::point3D() {

}

point3D::point3D(float xIn, float yIn, float zIn){
	x = xIn;	//setting the x, y and z values
	y = yIn;
	z = zIn;
}

float point3D::distance(point3D p2){
	float dist = sqrt(pow(p2.x-x,2) + pow(p2.y-y, 2) + pow(p2.z-z, 2));	//finding the distance
	return dist;
}

float point3D::fastDistance(point3D p2){
	float fDist = pow(p2.x-x,2) + pow(p2.y-y, 2) + pow(p2.z-z, 2);	//finding the fastest distance
	return fDist;
}

point3D point3D::movePoint(vec3D v){	//moving a point accordingly
	point3D n(x,y,z);
	n.x = x + v.x;
	n.y = y + v.y;
	n.z = z + v.z;
	return n;

}

vec3D::vec3D(){}

vec3D::vec3D(float xIn, float yIn, float zIn){	//setting the values for x, y and z
	x = xIn;
	y = yIn;
	z = zIn;
	length();
}

float vec3D::length(){
	float L = sqrt((pow(x,2) + pow(y,2) + pow(z,2)));	//finding the length of the vector
	mag = L;
	return L;
}

vec3D vec3D::normalize(){	//normalizing each vector by dividing by the length 
	vec3D normVec(x,y,z);
	normVec.x = x/length();
	normVec.y = y/length();
	normVec.z = z/length();
	if (normVec.length() == 1){
		return normVec;
	}
	return normVec;
}

vec3D vec3D::vectorMultiply(float scalar) {	//multiplying a vector by a scalar value
	vec3D multVec;
	multVec.x = x*scalar;
	multVec.y = y*scalar;
	multVec.z = z*scalar;
	multVec.length();
	return multVec;
}

vec3D vec3D::crossProduct(vec3D v2){	//finding the cross product of two vectors
	vec3D cross = vec3D(x,y,z);
	cross.x = (y * v2.z) - (z * v2.y);
	cross.y = (z * v2.x) - (x * v2.z);
	cross.z = (x * v2.y) - (y * v2.x);	
	return cross;															
}

vec3D vec3D::createVector(point3D p1, point3D p2){	//creating a vector between two points
	x = p2.x - p1.x;
	y = p2.y - p1.y;
	z = p2.z - p1.z;
	length();
	vec3D newVec(x,y,z);
	return newVec;
}

vec3D vec3D::addVector(vec3D v2) {	//adding two vectors together
	vec3D sum = vec3D(x, y, z); 
	sum.x += v2.x;
	sum.y += v2.y;
	sum.z += v2.z;
	return sum;
}

vec4D::vec4D() {}

vec4D::vec4D(float wIn, float xIn, float yIn, float zIn) {
	w = wIn;	//setting the w, x, y and z values
	x = xIn;
	y = yIn;
	z = zIn;
}

//overloading the plus operator for vectors
vec3D operator+ (vec3D v1, vec3D v2) {
	vec3D sum;
	sum.x = v1.x + v2.x;
	sum.y = v1.y + v2.y;
	sum.z = v1.z + v2.z;
	return sum;
}

//overloading the minus operator for vectors
vec3D operator- (vec3D v1, vec3D v2) {
	vec3D diff;
	diff.x = v1.x + v2.x;
	diff.y = v1.y + v2.y;
	diff.z = v1.z + v2.z;
	return diff;
}

//overloading the multiplcation operator for vectors
vec3D operator* (vec3D v1, vec3D v2) {
	vec3D prod;
	prod.x = v1.x * v2.x;
	prod.y = v1.y * v2.y;
	prod.z = v1.z * v2.z;
	return prod;
}

// int main(int argc, char** argv)
// {
// 	point2D a = point2D(0, 1);
// 	point2D b = point2D(1, 2);
// 	float d = a.distance(b);
// 	printf("Distance between points: %f", d);
// 	return 0;

// }

