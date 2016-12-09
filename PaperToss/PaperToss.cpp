/* Snowman sample 
 * by R. Teather
 * Edited by Noel Brett
 */
/* 
 Borna Kalantar kalantb 001207031
 Rebecca Tran tranr5 001425611
 Neil Robichaud robichne 001425566
 Sean McKay mckaysm 001423885
 */

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <string>
#include "math3D.h"
#include "LevelLoader.h"
#include <string>
#include <sstream>

using namespace std;

//The level
Level *L;
int currentLevel;

//Ball variables
point3D position;
vec3D velocity;
vec3D acceleration;
float ballRadius;
ballType ball;
float ballBounciness;

//Basket variables
float basketRadius;
point3D basketPos;

int firstwindow;
int secondwindow;
//Lighting and materials
float m_amb1[] = {0.135, 0.2225, 0.1575, 1.0}; //setting the material for the ambient, diffuse and specular values
float m_diff1[] = {0.54, 0.89, 0.63, 1.0};
float m_spec1[] = {0.316228, 0.316228, 0.316228, 1.0};
float shiny1 = 0.1;

//obsidian
// float m_amb1[] = {0.05375, 0.05, 0.06625, 1.0}; //setting the material for the ambient, diffuse and specular values
// float m_diff1[] = {0.18275, 0.17, 0.22525, 1.0};
// float m_spec1[] = {0.332741, 0.328634, 0.346435, 1.0};
// float shiny1 = 0.3;

float m_amb2[] = {0.0, 0.0, 0.0, 1.0}; //setting the material for the ambient, diffuse and specular values black plastic
float m_diff2[] = {0.01, 0.01, 0.01, 1.0};
float m_spec2[] = {0.50, 0.50, 0.50, 1.0};
float shiny2 = 0.25;

float m_amb3[] = {0.0, 0.0, 0.0, 1.0}; //setting the material for the ambient, diffuse and specular values white plastic
float m_diff3[] = {0.55, 0.55, 0.55, 1.0};
float m_spec3[] = {0.70, 0.70, 0.70, 1.0};
float shiny3 = 0.25;

float m_amb4[] = {0.25, 0.20725, 0.20725, 1.0}; //setting the material for the ambient, diffuse and specular values pearl for ball
float m_diff4[] = {1.00, 0.829, 0.829, 1.0};
float m_spec4[] = {0.296648, 0.296648, 0.296648, 1.0};
float shiny4 = 0.088;

float amb0[4] = {1,1,1,1};
float diff0[4] = {1,1,1,1};
float spec0[4] = {1,1,1,1};
//float m_amb[] = {0.33, 0.22, 0.03, 1.0};
//float m_diff[] = {0.78, 0.57, 0.11, 1.0};
//float m_spec[] = {0.99, 0.91, 0.81, 1.0};

//float m_amb[] = {0.33, 0.22, 0.03, 1.0}; 
//float m_diff[] = {0.78, 0.57, 0.11, 1.0}; 
//float m_spec[] = {0.99, 0.91, 0.81, 1.0}; 
//float shiny = 27.8;

float light1_pos[]= {-5, 5, 0,1}; //last num one is pofloat light
float light2_pos[]= {5, 5, 0,1}; //last num one is pofloat light

//Score counter
int intscorecounter = 0;
string stringscorecounter = "";
const char* counterstring;

//State variables for ball
bool selected = false;
bool throwing = false;
bool launched = false;

//Ball throwing variables
int startingMousepos[2];
int finalMousepos[2];
float startTime;
float endTime;
float averageXVelocity;
float averageZVelocity;

//Wind speed
float windspeed = 0;


//Eye Location
float eye[] = {0,1,7};
float lookat[] = {0,0,0};

float eyex = 0;
float eyey = 6;
float eyez = 7;

bool spherical = false; 


// BORNA UPDATE FROM HERE

float eyeradius = 7;
float eyetheta = 4.71;
float eyephi = 1.43;

float lookatradius = 0;
float lookattheta = 0;
float lookatphi = 0;

// x = eyeradius * sin(eyephi) * cos(eyetheta)
// y = eyeradius  * cos(eyephi)
// z = eyeradius * sin(eyetheta) * sin(eyephi)

//Texture variables
bool textureToggle = false;

GLubyte *img_data1;
GLubyte *img_data2;
GLubyte *img_data3;

GLuint textures[3]; //array for storing the three different textures
int width, height, ppmax;  //width, height, max variables for the file of the texures

GLUquadricObj *sphereOBJ = NULL;

void setStartingParameters() {

	ballRadius = L -> ballRadius;
	ball = L -> ball;
	ballBounciness = 1 - L -> ballBounciness;

	eyez = L->startingPosition.z + 2;
	eyey = L->startingPosition.y + 5;

	basketRadius = L->basketRadius;
	basketPos.x = L->basketPosition1.x;
	basketPos.y = L->basketPosition1.y;
	basketPos.z = L->basketPosition1.z;

	position.x = L->startingPosition.x; 
	position.y = L->startingPosition.y;
	position.z = L->startingPosition.z;

	velocity.x = 0.0f;
	velocity.y = 0.0f;
	velocity.z = 0.0f;

	acceleration.x = windspeed;
	acceleration.y = -100.0f;
	acceleration.z = 0.0f;
}

void switchRound(int i) {
	switch(i) {
		case 0:
			basketPos.x = L->basketPosition1.x;
			basketPos.y = L->basketPosition1.y;
			basketPos.z = L->basketPosition1.z;
		break;
		case 1:
			basketPos.x = L->basketPosition2.x;
			basketPos.y = L->basketPosition2.y;
			basketPos.z = L->basketPosition2.z;
		break;
		case 2:
			basketPos.x = L->basketPosition3.x;
			basketPos.y = L->basketPosition3.y;
			basketPos.z = L->basketPosition3.z;
		break;
		case 3:
			basketPos.x = L->basketPosition4.x;
			basketPos.y = L->basketPosition4.y;
			basketPos.z = L->basketPosition4.z;
		break;
		case 4:
			basketPos.x = L->basketPosition5.x;
			basketPos.y = L->basketPosition5.y;
			basketPos.z = L->basketPosition5.z;
		break;
	}
}

void switchLevel(int i) {
	L = new Level(i);
	setStartingParameters();
}

void drawBall(){

    
    glPushMatrix();
    glTranslatef(position.x,position.y,position.z);
    //printf("%f",position[1]);
    // glBindTexture(GL_TEXTURE_2D, textures[0]);
    float m_amb2[] = {0.0, 0.0, 0.0, 1.0}; //setting the material for the ambient, diffuse and specular values black plastic
    float m_diff2[] = {0.01, 0.01, 0.01, 1.0};
    float m_spec2[] = {0.50, 0.50, 0.50, 1.0};
    float shiny2 = 0.25;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb2); //putting material onto the terrain
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec2);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny2);
    sphereOBJ = gluNewQuadric();
    gluQuadricDrawStyle(sphereOBJ, GLU_FILL);
    gluQuadricTexture(sphereOBJ, GL_TRUE);
    gluQuadricNormals(sphereOBJ, GLU_SMOOTH);
    
    gluSphere(sphereOBJ, 1, 100, 100);
    glPopMatrix();
}

GLubyte* LoadPPM(char* file, int* width, int* height, int* ppmax)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    //first open file and check if it's an ASCII PPM (indicated by P3 at the start)
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    printf("%s is a PPM file\n", file);
    fscanf(fd, "%c",&c);
    
    //next, skip past the comments - any line starting with #
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);
    
    // now get the dimensions and max colour value from the image
    fscanf(fd, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    // calculate number of pixels and allocate storage for this
    nm = n*m;
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    s=255.0/k;
    
    // for every pixel, grab the read green and blue values, storing them in the image data array
    for(i=0;i<nm;i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }
    
    // finally, set the "return parameters" (width, height, max) and return the image array
    *width = n;
    *height = m;
    *ppmax = k;
    
    return img;
}

// textures function
void initTexture(void) {
    img_data1 = LoadPPM("hay.ppm", &width, &height, &ppmax);    //loading the texture by using the LoadPPM function, into img_data1
    img_data2 = LoadPPM("tree.ppm", &width, &height, &ppmax);
    img_data3 = LoadPPM("crates.ppm", &width, &height, &ppmax);
    
    //enabling textures
    glEnable(GL_TEXTURE_2D);
    
    glGenTextures(3, textures); //using three textures
    glBindTexture(GL_TEXTURE_2D, textures[0]);  //binding the first texture stored in the textures array
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data1); //getting the necessary information of the texture
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   //setting parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);  //binding the first texture stored in the textures array
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data2); //getting the necessary information of the texture
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   //setting parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, textures[2]);  //binding the first texture stored in the textures array
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data3); //getting the necessary information of the texture
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   //setting parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void updateeyeposition(){

	if (spherical){

		eye[0] = eyeradius * sin(eyephi) * cos(eyetheta);
		eye[1] = eyeradius  * cos(eyephi);
		eye[2] = eyeradius * sin(eyetheta) * sin(eyephi);
	}

	else{

		eye[0] = eyex;
		eye[1] = eyey;
		eye[2] = eyez;
	}

}
void sphericaltocartesion(){
	eyex = eyeradius * sin(eyephi) * cos(eyetheta);
	eyey = eyeradius  * cos(eyephi);
	eyez = eyeradius * sin(eyetheta) * sin(eyephi);

}
void cartesiontospherical(){
	eyeradius = sqrtf(eyex * eyex + eyey * eyey + eyez * eyez);
	eyephi = acos(eyey/eyeradius);
	eyetheta = atan(eyez/eyex);
	if(eyetheta > 0 ){
	eyetheta += 3.14;
	}
	if(eyez > 0){
		eyetheta += 3.14;
	}

}



// void updatelookatposition(){
    
//     lookat[0] = lookatradius * sin(lookatphi) * cos(lookattheta);
//     lookat[1] = lookatradius  * cos(lookatphi);
//     lookat[2] = lookatradius * sin(lookattheta) * sin(lookatphi);
// }

// BORNA UPDATE TO HERE
void textprinter(int x, int y, char* text)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 800, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);

    glColor3f(0,0,1);
    char* c;  //character iterator (ptr)
    for(c = text; *c != '\0'; c++) //stop when we hit null character
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c); //print one char
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
//    glEnable(GL_DEPTH_TEST);
    
}
void changetostring(int integer){
    
    stringstream streamint;
    
    streamint << integer;
    
    stringscorecounter = streamint.str();
    
    counterstring = stringscorecounter.c_str();;
    
}

void drawBasket() {
    glColor3f(1,0,0);
    glPushMatrix();
        glTranslatef(basketPos.x, basketPos.y, basketPos.z);
        glPushMatrix();
            glTranslatef(0, 0.8, -1);
            glScalef(2.5, 2.5, 0.1);
            glutSolidCube(1);
        glPopMatrix();
        glRotatef(90, 1, 0, 0);
        glutSolidTorus(basketRadius / 5, basketRadius, 100, 100);
    glPopMatrix();
}

bool pendingStop = false;
float groundstartTime;
float currentTime;
bool bounced = false;

void startTimer(){
    groundstartTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);
}

vector<int> intersectionIndex;
vec3D intersectedFaceNormal;
point3D intersectedFacePosition;
int oInd;
int fInd;



void resetBall() {
    position = L->startingPosition;
    velocity.x = 0.0f; velocity.y = 0.0f; velocity.z = 0.0f;
    acceleration.x = windspeed; acceleration.y = -100.0f; acceleration.z = 0.0f;
    launched = false;
    pendingStop = false;
    bounced = false;
}


void ballMotion(int value){

	//Ball resets after certain amount of time 
    if(bounced){

        if (pendingStop == false){
            pendingStop = true;
            startTimer();           //starts a 3 second timer
        }
        else{
            currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
           // printf("%f\n", currentTime-groundstartTime);
            if (currentTime - groundstartTime > 5000){
                resetBall();
            }
        }
    }
    if (launched == true) {
    	//Increment velocity based on acceleration
	    velocity.x = velocity.x + acceleration.x/60;
	    velocity.y = velocity.y + acceleration.y/60;
	    velocity.z = velocity.z + acceleration.z/60;

	    //Increment position based on velocity
	    position.x += velocity.x/60;
	    position.y += velocity.y/60;
	    position.z += velocity.z/60;

	    //Reset variables
	    intersectedFaceNormal.x = 0; intersectedFaceNormal.y = 0; intersectedFaceNormal.z = 0;
	    intersectedFacePosition.x = 0; intersectedFacePosition.y = 0; intersectedFacePosition.z = 0;

	    //Test if balls position intersects with any faces of objects in the level
	    intersectionIndex = L->testIntersection(position.x, position.y, position.z, ballRadius);
	    if (intersectionIndex.size() > 0) {
	    	if (intersectionIndex.at(0) != -1) {
	    		oInd = intersectionIndex.at(0);
	    		fInd = intersectionIndex.at(1);
	    		intersectedFaceNormal = L->levelObjects->at(oInd)->objectFaces->at(fInd)->normal;
	    		intersectedFacePosition = L->levelObjects->at(oInd)->objectFaces->at(fInd)->min;
	    	}
	    }

	    //Intersecting with left object face
	    if (intersectedFaceNormal.x == 1) {
	    	velocity.x = -1 * (velocity.x - ballBounciness*velocity.x);
            position.x = intersectedFacePosition.x + velocity.x/60 + ballRadius;
	    }

	    //Intersecting with right object face
	   	else if (intersectedFaceNormal.x == -1) {
	    	velocity.x = -1 * (velocity.x - ballBounciness *velocity.x);
            position.x = intersectedFacePosition.x + velocity.x/60 - ballRadius;
	    }

	    //Intersecting with bottom object face
	    else if (intersectedFaceNormal.y == 1) {
	    	velocity.y = -1 * (velocity.y - ballBounciness *velocity.y);
            position.y = intersectedFacePosition.y + velocity.y/60 + ballRadius;
            if (intersectedFacePosition.y == 0) {
            	bounced = true;
                printf("bounce");
            }
	    }

	    //Intersecting with top object face
	    else if (intersectedFaceNormal.y == -1) {
	    	velocity.y = -1 * (velocity.y - ballBounciness * velocity.y);
            position.y = intersectedFacePosition.y + velocity.y/60 - ballRadius;

	    }

	    //Intersecting with front object face
	    else if (intersectedFaceNormal.z == 1) {
            velocity.z = -1 * (velocity.z - ballBounciness * velocity.z);
            position.z = intersectedFacePosition.z + velocity.z/60 + ballRadius;
	    }

	    //Intersecting with back object face
	    else if (intersectedFaceNormal.z == -1) {
            velocity.z = -1 * (velocity.z - ballBounciness * velocity.z);
            position.z = intersectedFacePosition.z + velocity.z/60 - ballRadius;
	    }

        //Intersection with inside of basket (scoring)
        if ((position.x > (basketPos.x - basketRadius)) && (position.x < (basketPos.x + basketRadius))) {
            if ((position.z > (basketPos.z - basketRadius)) && (position.z < (basketPos.z + basketRadius))) {
                if (position.y > basketPos.y && position.y < (basketPos.y + 0.5f) && !bounced) {
                    resetBall();
                    intscorecounter++;
                    switchRound(intscorecounter % 5);
                    if (intscorecounter != 0 && intscorecounter % 5 == 0) {
                    	currentLevel++;
                    	switchLevel(currentLevel);
                    }
                }
            }
        }
        //Intersection with outside of basket (rim shot)
         if ((position.x > (basketPos.x - basketRadius)) && (position.x < (basketPos.x + basketRadius))) {
             if ((position.z > (basketPos.z - basketRadius)) && (position.z < (basketPos.z + basketRadius))) {
                if (position.y < basketPos.y  && bounced) {
                     velocity.x = -1*(velocity.x - ballBounciness * velocity.x);       //lose half magnitude and reverse direction
                     position.x = position.x + velocity.x/60;

                     velocity.z = -1*(velocity.z - ballBounciness *velocity.z);       //lose half magnitude and reverse direction
                     position.x = position.x + velocity.x/60;
                }
            }
        }

    }

    glutTimerFunc(17,ballMotion,0);

}
int snowmanCounter = 0;
bool movingRight = true;
float pos[] = {0,1,0};
float rot[] = {0,0,0};
float headRot[] = {0,0,0};
void DrawSnowman(float* pos, float* rot){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb3); //putting material onto the terrain
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff3);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec3);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny3*128);
    
    glPushMatrix();
    // printf("%i\n", counter);
    if(movingRight == true){
        if(snowmanCounter > -50){
            glTranslatef(0.1*snowmanCounter,0,0);
            snowmanCounter--;
        } else if (snowmanCounter == -50){
            glTranslatef(0.1*snowmanCounter,0,0);
            snowmanCounter++;
            movingRight = false;
        }
    }
    
    if(movingRight == false){
        if(snowmanCounter < 50){
            glTranslatef(0.1*snowmanCounter,0,0);
            snowmanCounter++;
            
        } else if(snowmanCounter == 50){
            glTranslatef(0.1*snowmanCounter,0,0);
            snowmanCounter--;
            movingRight = true;
        }
    }
    
    
    glPushMatrix();
    
    glTranslatef(pos[0], pos[1], pos[2]);
    // glRotatef(rot[1], 0, 1, 0);
    glRotatef(180,0,1,0);
    
    //draw body
    glColor3f(1,1,1);
    glutSolidSphere(1, 16, 16);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb2); //putting material onto the terrain
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec2);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny2*128);
    
    //draw buttons
    glPushMatrix();
    glTranslatef(0, 0.35, 0.9);
    glColor3f(0, 0, 0);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0.15, 0.95);
    glColor3f(0, 0, 0);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -0.05, 0.95);
    glColor3f(0, 0, 0);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb3); //putting material onto the
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff3);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec3);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny3*128);
    
    glPushMatrix();
    //translate relative to body, and draw head
    glTranslatef(0, 1.25, 0);
    glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
    glColor3f(1,1,1);
    glutSolidSphere(0.5, 16, 16);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb2); //putting material onto the
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec2);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny2*128);
    
    //translate and draw right eye
    glPushMatrix();
    glTranslatef(0.2, 0.15, 0.45);
    glColor3f(0,0,0);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    //translate and draw left eye
    glPushMatrix();
    glTranslatef(-0.2, 0.15, 0.45);
    glColor3f(0,0,0);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    //translate and draw nose
    glPushMatrix();
    glTranslatef(0, 0, 0.5);
    glColor3f(1,0.4,0);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    glPopMatrix();//body
    glPopMatrix();//snowman
    
    glPopMatrix();
}

float input1, input2, input3;

void keyboard(unsigned char key, int x, int y)
{

    /* key presses move the cube, if it isn't at the extents (hard-coded here) */
    switch (key)
    {
        case 'q':
        case 27:
            exit (0);
            break;
            
		case 'W':
        case 'w':
      		eyez +=0.1;
        	break;

        case 'a':
        case 'A':
          	eyex +=0.1;
        	break;

    	case 'S':
        case 's':
        	eyez -=0.1;
        	break;

        case 'D':
        case 'd':
          	eyex -=0.1;
        	break;

        case 'U':
        case 'u':
          	eyey +=0.1;
        	break;

        case 'I':
        case 'i':
        	if (eyey > 0) {
        		eyey -=0.1;
        	}
        	break;

        case 'z':
        	eyeradius += 0.1;
        	break;

        case 'Z':
        	eyeradius -= 0.1;
        	break;


        case 'x':
        	eyetheta += 0.05;
        	break;


        case 'X':
        	eyetheta -= 0.05;
        	break;

        case 'c':
        	if(eyephi < 1.5){
        	eyephi += 0.05;
        	}
      		break;

        case 'C':
        	if(eyephi > -1.5){
        	eyephi -= 0.05;
        	}        	
        	break;

        case 'm':
        case 'M':
        	if (spherical){
        	sphericaltocartesion();
        	spherical = false;
	        }

        	else{
        	cartesiontospherical();
        	spherical = true;
        	}
        	break;
          
            break;
        case '1':
            if (textureToggle == true){
                glEnable(GL_TEXTURE_2D);
                textureToggle = false;
            } else {
                glDisable(GL_TEXTURE_2D);
                textureToggle = true;
            }
            break;
            
        case '2':
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            // drawBall();
            break;
            
        case '3':
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            // drawBall();
            break;
            
        case '4':
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            // drawBall();
            break;
            
        case 'r':
            case 'R':
            resetBall();
            break;
    

    }
    glutPostRedisplay();
}

void special(int key, int x, int y)
{

    switch(key)
    { case GLUT_KEY_UP:
            eye[1] += 1;
            lookat[1] += 1;
        break;
        case GLUT_KEY_DOWN:
            eye[1] -= 1;
            lookat[1] -= 1;
        break;
        case GLUT_KEY_LEFT:
            eye[0] -= 1;
            lookat[0] -= 1;
        break;
        case GLUT_KEY_RIGHT:
            eye[0] += 1;
            lookat[0] += 1;
        break;
        
    }
    glutPostRedisplay();
}

void init(void)
{   glClearColor(0, 0, 0, 0);
    glColor3f(1,0,0);
    
    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10,10,-10,10,0,1000);
    gluPerspective(90, 1, 1, 1000);

    //Load the level
    L = new Level(1);
    currentLevel = 1;

}



void display(void)
{
    float origin[3] = {0,0,0};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    updateeyeposition();
    glLoadIdentity();
    changetostring(intscorecounter);
    glDisable(GL_LIGHTING);
    textprinter(750,780, (char*)counterstring);
    glEnable(GL_LIGHTING);
    gluLookAt(eye[0],eye[1],eye[2], lookat[0],lookat[1],lookat[2], 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light2_pos);
    
    glDisable(GL_TEXTURE_2D);

    L->drawLevel();
    
    drawBasket();
    if(textureToggle == true){
        glEnable(GL_TEXTURE_2D);
    }
    drawBall();
    glutSwapBuffers();
    glutPostRedisplay();
    
}



bool calcIntersection(vec3D r0, vec3D rd) {
    //Max and min of bounding box for ball
    vec3D min, max;
    min.x = position.x - 0.5f; min.y = position.y - 0.5f; min.z = position.z - 0.5f;
    max.x = min.x + 1.0f; max.y = min.y + 1.0f; max.z = min.z + 1.0f;
    
    //Calculate points on the front face of the ball
    point3D p0, p1, p2, p3;
    p0.x = min.x; p0.y = min.y; p0.z = max.z;
    p1.x = max.x; p1.y = min.y; p1.z = max.z;
    p2.x = max.x; p2.y = max.y; p2.z = max.z;
    p3.x = min.x; p3.y = max.y; p3.z = max.z;
    
    //Calculate the equation of the plane intersecting with the face
    float a, b, c, d;
    vec3D t1, t2, norm;
    
    t1 = t1.createVector(p1, p0);
    t2 = t2.createVector(p2, p0);
    
    norm = t1.crossProduct(t2);
    
    norm = norm.normalize();
    
    d = -((norm.x * p0.x) + (norm.y * p0.y) + (norm.z * p0.z));
    a = norm.x; b = norm.y; c = norm.z;
    
    //Test for intersection of the ray and the plane
    float t, dot0, dotD;
    vec3D is;
    
    dot0 = (a * r0.x) + (b * r0.y) + (c * r0.z);
    dotD = (a * rd.x) + (b * rd.y) + (c * r0.z);
    
    if (dot0 == 0) return false;
    
    t = -((dot0 + d) / dotD);
    
    is = r0 + rd.vectorMultiply(t);
    
    if ((is.x > p0.x) && (is.x < p1.x) && (is.y > p0.y) && (is.y < p2.y)) {
        return true;
    } else {
        return false;
    }
    
    
    
}

bool rayCast(float x, float y) {
    GLint viewport[4];          //declaring arrays and variables
    GLdouble modelview[16];
    GLdouble projection[16];
    float winX;
    float winY;
    GLdouble pos1X, pos1Y, pos1Z;
    GLdouble pos2X, pos2Y, pos2Z;
    GLdouble dirX, dirY, dirZ;
    vec3D r0, rd;
    
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    //Flip the y coordinate to have the proper opengl screen coords
    winX = x;
    winY = (float)viewport[3] - y;
    
    //Unproject the coordinates from screen to world coordinates on the near clipping plane
    gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &pos1X, &pos1Y, &pos1Z);
    
    //Unproject the coordinates from screen to world coordinates on the far clipping plane
    gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &pos2X, &pos2Y, &pos2Z);
    
    //Calculate a normalized ray between the far and near clipping planes
    dirX = pos2X - pos1X;
    dirY = pos2Y - pos1Y;
    dirZ = pos2Z - pos1Z;
    
    r0.x = pos1X; r0.y = pos1Y; r0.z = pos1Z;
    rd.x = dirX; rd.y = dirY; rd.z = dirZ;
    
    rd = rd.normalize();
    
    if (calcIntersection(r0, rd)) {
        return true;
    } else {
        return false;
    }
}


void mouse(int btn, int state, int x, int y){
    
    if (btn == GLUT_LEFT_BUTTON && launched == false){
        if (state == GLUT_DOWN){
            startingMousepos[0] = x, startingMousepos[1] = y;       //store the starting mouse position
            startTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);          //store the time when the user first clicks down
        }
        else if(state == GLUT_UP){
            
            finalMousepos[0] = x;
            finalMousepos[1] = y;
            endTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);          //store the time when the user first clicks down
            
            float timeElapsed = endTime - startTime;
            float dy = -(finalMousepos[1] - startingMousepos[1]);
            float dx = (finalMousepos[0] - startingMousepos[0]);
            
            averageXVelocity = dx/timeElapsed;
            averageZVelocity = dy/timeElapsed;

            if (ball == normal) {
            	velocity.x += averageXVelocity*15;
            	velocity.y = 2* (averageXVelocity+averageZVelocity)/2 + 20 ;
            	velocity.z -= averageZVelocity*15;
            }
            else if (ball == bouncy) {
            	 velocity.x += averageXVelocity*15;
	             velocity.y = 35;
	             velocity.z -= averageZVelocity*15;
            }
            else if (ball == rock) {
            	velocity.x += averageXVelocity*15;
            	velocity.y = 25;
            	velocity.z -= averageZVelocity*15;
            }

            
            printf("%f,%f\n", averageXVelocity,averageZVelocity);
            launched = true;
            bounced = false;
        }
    }
    
}
void callbackinit(){
    glutTimerFunc(0, ballMotion, 0);
}
/* main function - program entry point */
int main(int argc, char** argv)
{
    srand(time(NULL));
    
    glutInit(&argc, argv);      //starts up GLUT
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);

    firstwindow = glutCreateWindow("BallToss");  //creates the window
    
    
    glutDisplayFunc(display);   //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    
    glEnable(GL_DEPTH_TEST);
    //glFrontFace((GL_CCW));
    //glCullFace(GL_FRONT);
    //glEnable(GL_CULL_FACE);
    init();
    //initTexture();
    callbackinit();
    setStartingParameters();

    glutMainLoop();             //starts the event loop

    return(0);                  //return may not be necessary on all compilers
}