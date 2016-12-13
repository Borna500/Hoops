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

//Global Variables
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
int text = 4;

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

float m_amb2[] = {0.0, 0.0, 0.0, 1.0}; //setting the material for the ambient, diffuse and specular values black plastic
float m_diff2[] = {0.01, 0.01, 0.01, 1.0};
float m_spec2[] = {0.50, 0.50, 0.50, 1.0};
float shiny2 = 0.25;

float m_amb3[] = {0.3, 0.3, 0.3, 1.0}; //setting the material for the ambient, diffuse and specular values white plastic
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
bool textureToggle = true;

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
		case 1:
			basketPos.x = L->basketPosition1.x;
			basketPos.y = L->basketPosition1.y;
			basketPos.z = L->basketPosition1.z;
		break;
		case 2:
			basketPos.x = L->basketPosition2.x;
			basketPos.y = L->basketPosition2.y;
			basketPos.z = L->basketPosition2.z;
		break;
		case 3:
			basketPos.x = L->basketPosition3.x;
			basketPos.y = L->basketPosition3.y;
			basketPos.z = L->basketPosition3.z;
		break;
		case 4:
			basketPos.x = L->basketPosition4.x;
			basketPos.y = L->basketPosition4.y;
			basketPos.z = L->basketPosition4.z;
		break;
		case 5:
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

//drawing the ball that is thrown function
void drawBall(){
    glPushMatrix();
    //everytime the ball travels one circumference it will rotate one revolution
        glTranslatef(position.x,position.y,position.z);  //movement of the ball by the position 
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb4); //putting material onto the ball
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff4);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec4);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny4);

        sphereOBJ = gluNewQuadric();                //putting texture onto the bal
        gluQuadricDrawStyle(sphereOBJ, GLU_FILL);
        gluQuadricTexture(sphereOBJ, GL_TRUE);
        gluQuadricNormals(sphereOBJ, GLU_SMOOTH);
        gluSphere(sphereOBJ, 1, 100, 100);  //drawing the sphere/ball
    glPopMatrix();
}

//LoadPPM function that does all the textures 
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

//switching between the textures given
void ballTexture(){
    switch(text){
        case 2:
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            break;
            
        case 3:
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            break;
            
        case 4:
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            break;
    }
}

//global variables for the snowman
int snowmanCounter = 0; //counter
bool movingRight = true;    //snowman is moving right/left
float pos[] = {0,1,0};
float rot[] = {0,0,0};
float headRot[] = {0,0,0};
point3D snowmanPos = point3D{0, 1, 0};

void DrawSnowman(float* pos, float* rot){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb3); //putting material onto the terrain
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff3);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec3);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny3*128);
    
    glPushMatrix();

    // determine snowman's direction
    if (movingRight){
        if(snowmanPos.x > 10){
            movingRight = false;
        }
    }
    else{
        if(snowmanPos.x < -10){
            movingRight = true;
        }
        
    }
    //move snowman in direction
    if (movingRight){
        snowmanPos.x+=0.1;
    }
    else{
        snowmanPos.x-=0.1;
    }
    glTranslatef(snowmanPos.x, snowmanPos.y, snowmanPos.z);
    
           glPushMatrix();
        
            glTranslatef(pos[0], pos[1], pos[2]);
            // glRotatef(rot[1], 0, 1, 0);
            glRotatef(180,0,1,0);
            
            //draw body
            glColor3f(1,1,1);
            glutSolidSphere(1, 16, 16);
            
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb2); //putting material onto the snowman buttons
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
            
            
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb3); //putting material onto the head
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff3);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec3);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny3*128);
            
            glPushMatrix();
            //translate relative to body, and draw head
                glTranslatef(0, 1.25, 0);
                glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
                glColor3f(1,1,1);
                glutSolidSphere(0.5, 16, 16);
                
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb2); //putting material onto the eyes and nose
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


void updateeyeposition(){
	if (spherical){
		eye[0] = eyeradius * sin(eyephi) * cos(eyetheta);
		eye[1] = eyeradius  * cos(eyephi);
		eye[2] = eyeradius * sin(eyetheta) * sin(eyephi);
	} else {
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

//aAAA
// void updatelookatposition(){
    
//     lookat[0] = lookatradius * sin(lookatphi) * cos(lookattheta);
//     lookat[1] = lookatradius  * cos(lookatphi);
//     lookat[2] = lookatradius * sin(lookattheta) * sin(lookatphi);
// }

//putting text onto the window
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
//changing from string to char
void changetostring(int integer){
    
    stringstream streamint;
    
    streamint << integer;
    
    stringscorecounter = streamint.str();
    
    counterstring = stringscorecounter.c_str();;
    
}

//drawing the basket for the ball to go into
void drawBasket() {
    glColor3f(1,0,0);
    glPushMatrix();
        glTranslatef(basketPos.x, basketPos.y, basketPos.z); //moving the position of the basket 
        glPushMatrix();
            glTranslatef(0, 0.8, -1);    //defining the position and size of the backboard
            glScalef(2.5, 2.5, 0.1);
            glutSolidCube(1);
        glPopMatrix();
        glRotatef(90, 1, 0, 0); //making the hoop horizontal
        glutSolidTorus(basketRadius / 5, basketRadius, 100, 100);   //drawing the torus for the hoop
    glPopMatrix();
}

//global variables for the timer
bool pendingStop = false;
float groundstartTime;
float currentTime;
bool bounced = false;

//timer function for when the ball resets after the shot is missed
void startTimer(){
    groundstartTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);
}

//global variables
vector<int> intersectionIndex;
vec3D intersectedFaceNormal;
point3D intersectedFacePosition;
int oInd;
int fInd;

//reseting the ball back into the original position
void resetBall() {
    //putting the current position back to the start position values
    position = L->startingPosition;

    velocity.x = 0.0f; //reseting velocity values to 0
    velocity.y = 0.0f; 
    velocity.z = 0.0f;

    acceleration.x = windspeed; //reseting the acceleration values back to the original 
    acceleration.y = -100.0f; 
    acceleration.z = 0.0f;

   //changing the bool values back to ball not being launched and not bouncing
    launched = false;
    pendingStop = false;
    bounced = false;
}

int updateinterval = 120;

//the motion of the ball function
void ballMotion(int value){
	//Ball resets after certain amount of time 
    if(bounced){
        if (pendingStop == false){
            pendingStop = true;
            startTimer();           //starts a 3 second timer
        } else {
            currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
           // printf("%f\n", currentTime-groundstartTime);
            if (currentTime - groundstartTime > 5000){
                resetBall();
            }
        }
    }
    if (launched == true) { //if the ball is launched
    	//Increment velocity based on acceleration
	    velocity.x = velocity.x + acceleration.x/updateinterval;
	    velocity.y = velocity.y + acceleration.y/updateinterval;
	    velocity.z = velocity.z + acceleration.z/updateinterval;

	    //Increment position based on velocity
	    position.x += velocity.x/updateinterval;
	    position.y += velocity.y/updateinterval;
	    position.z += velocity.z/updateinterval;

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
                //printf("%i, %i\n", oInd, fInd);
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
            position.x = intersectedFacePosition.x + velocity.x/updateinterval - ballRadius;
	    }

	    //Intersecting with bottom object face
	    else if (intersectedFaceNormal.y == 1) {
	    	velocity.y = -1 * (velocity.y - ballBounciness *velocity.y);
            position.y = intersectedFacePosition.y + velocity.y/updateinterval + ballRadius;
            if (intersectedFacePosition.y == 0) {
            	bounced = true;
            }
	    }

	    //Intersecting with top object face
	    else if (intersectedFaceNormal.y == -1) {
	    	velocity.y = -1 * (velocity.y - ballBounciness * velocity.y);
            position.y = intersectedFacePosition.y + velocity.y/updateinterval - ballRadius;

	    }

	    //Intersecting with front object face
	    else if (intersectedFaceNormal.z == 1) {
            velocity.z = -1 * (velocity.z - ballBounciness * velocity.z);
            position.z = intersectedFacePosition.z + velocity.z/updateinterval + ballRadius;
	    }

	    //Intersecting with back object face
	    else if (intersectedFaceNormal.z == -1) {
            velocity.z = -1 * (velocity.z - ballBounciness * velocity.z);
            position.z = intersectedFacePosition.z + velocity.z/updateinterval - ballRadius;
	    }

        //Intersection with inside of basket (scoring)
        if ((position.x > (basketPos.x - basketRadius)) && (position.x < (basketPos.x + basketRadius))) {
            if ((position.z > (basketPos.z - basketRadius)) && (position.z < (basketPos.z + basketRadius))) {
                if (position.y > basketPos.y && position.y < (basketPos.y + 0.5f) && !bounced) {
                    resetBall();
                    intscorecounter++;
                    switchRound(intscorecounter % 5 + 1);
                    if (intscorecounter != 0 && intscorecounter % 5 == 0) {
                    	currentLevel++;
                    	switchLevel(currentLevel);
                        text--;
                    }
                }
            }
        }
        //Intersection with outside of basket (rim shot)
         if ((position.x > (basketPos.x - basketRadius)) && (position.x < (basketPos.x + basketRadius))) {
             if ((position.z > (basketPos.z - basketRadius)) && (position.z < (basketPos.z + basketRadius))) {
                if (position.y < basketPos.y  && bounced) {
                     velocity.x = -1*(velocity.x - ballBounciness * velocity.x);       //lose half magnitude and reverse direction
                     position.x = position.x + velocity.x/updateinterval;

                     velocity.z = -1*(velocity.z - ballBounciness *velocity.z);       //lose half magnitude and reverse direction
                     position.x = position.x + velocity.x/updateinterval;
                }
            }
        }
        //Snowman intersection
    }
    glutTimerFunc(5,ballMotion,0);
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
      		eyez -=0.1; //changing the eye position in z+ direction
        	break;

        case 'a':
        case 'A':
          	eyex -=0.1; //changing the eye position in x+ direction
        	break;

    	case 'S':
        case 's':
        	eyez +=0.1; //changing the eye position in z- direction
        	break;

        case 'D':
        case 'd':
          	eyex +=0.1; //changing the eye position in x- direction
        	break;

        case 'U':
        case 'u':
          	eyey +=0.1; //changing the eye position in y+ direction
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
        	sphericaltocartesion();    //toggle for switching between spherical and cartesian
        	spherical = false; //switching it to cartesian from spherical
	        }

        	else{
        	cartesiontospherical();    //switching to spherical from cartesian
        	spherical = true;  //now on spherical
        	}
        case 'r':
            resetBall();    //reseting the ball back to original position
            break;
          
        case '1':
            if (textureToggle == true){ //texture is on
                glEnable(GL_TEXTURE_2D);    //enable texture
                // glBindTexture(GL_TEXTURE_2D, textures[0]);
                
                textureToggle = false;  //change it to false to be turned off
            } else {
                glDisable(GL_TEXTURE_2D);   //disable texture
                textureToggle = true;   //change toggle to be true to be turned back on
            }
            break;
            
        case '2':
            text = 2;   //setting value to be 2, to be switched to first texture
            // glBindTexture(GL_TEXTURE_2D, textures[0]);
            break;
            
        case '3':
            text = 3;   //setting value to be 3, to be switched to second texture
            // glBindTexture(GL_TEXTURE_2D, textures[1]);
            
            break;
            
        case '4':
            text = 4;   //setting value to be 4, to be switched to third texture
            // glBindTexture(GL_TEXTURE_2D, textures[2]);
            
            break;
    

    }
    glutPostRedisplay();    //refresh
}

void special(int key, int x, int y)
{

    switch(key)
    { case GLUT_KEY_UP:
            eye[1] += 1;    //increment the y eye position
            lookat[1] += 1; //increment the y lookat position
        break;
        case GLUT_KEY_DOWN:
            eye[1] -= 1;    //decrement the y eye position
            lookat[1] -= 1; //decrement the y lookat position 
        break;
        case GLUT_KEY_LEFT:
            eye[0] -= 1;    //decrement the x eye position
            lookat[0] -= 1; //decrement the x lookat position
        break;
        case GLUT_KEY_RIGHT:
            eye[0] += 1;    //increment the x eye position
            lookat[0] += 1; //increment the x lookat position
        break;
        
    }
    glutPostRedisplay();
}

void init(void)
{   glClearColor(0, 0, 0, 0);
    glColor3f(1,0,0);
    
    glEnable(GL_LIGHTING);  //enable lighting 
    //glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);    //enable light1
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); //setting the amb, diff, spec values for the light
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
    printf("m switches between spherical and cartesian camera, initially cartesian");
    printf("Cartesian camera controls W A S D to move the camera forward, left, back, and right\n");
    printf("U and I move the camera up and down\n");
    printf("z and Z increase and decrease the distance from the centre of the room\n");
    printf("x and X increase and decrease the azimuthal angle respectively\n");
    printf("c and C increase and decrease the polar angle respectively\n");
    printf("1 toggles textures on and off\n");
    printf("r resets the ball's positon\n");

    
}

//display function that is shown
void display(void)
{
    float origin[3] = {0,0,0};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    updateeyeposition();
    glLoadIdentity();
    changetostring(intscorecounter);    //changing the score counter depending on the user's score

    glDisable(GL_LIGHTING); //disable lighting
    textprinter(750,780, (char*)counterstring); //displaying text
    glEnable(GL_LIGHTING);  //enable lighting

    gluLookAt(eye[0],eye[1],eye[2], lookat[0],lookat[1],lookat[2], 0, 1, 0);    //placement of lookat
    glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);  //placement of lights
    glLightfv(GL_LIGHT1, GL_POSITION, light2_pos);
    
    glDisable(GL_TEXTURE_2D);   //disable texture

    L->drawLevel(); //drawing the level
    glDisable(GL_TEXTURE_2D);   //disable texture on the basket
    drawBasket();   //draw basket
    if (currentLevel==3) {
        DrawSnowman(pos, rot);  //draw snowman
    }
    
    if(textureToggle == true){  //turning on the texture with the toggle
        glEnable(GL_TEXTURE_2D);    //enable texture for the ball
    }
    ballTexture();  //switching between material
    drawBall(); //drawing ball

    glutSwapBuffers();
    glutPostRedisplay();
    
}

int calcInter(int x, int y, float ballx, float bally, float ballz) {
    GLdouble R0[3], R1[3], Rd[3];
    GLdouble modelMat[16];
    GLdouble projMat[16];
    GLint viewMat[4];
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
    glGetDoublev(GL_PROJECTION_MATRIX, projMat);
    glGetIntegerv(GL_VIEWPORT, viewMat);
    
    gluUnProject(x, y, 0.0, modelMat, projMat, viewMat, &R0[0], &R0[1], &R0[2]);
    gluUnProject(x, y, 1.0, modelMat, projMat, viewMat, &R1[0], &R1[1], &R1[2]);
    
    Rd[0] = R1[0] - R0[0];
    Rd[1] = R1[1] - R0[1];
    Rd[2] = R1[2] - R0[2];
    
    GLdouble m = sqrt(Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2]);
    Rd[0] /= m;
    Rd[1] /= m;
    Rd[2] /= m;
    
    double defOffset = 2.0; //the distance from the center of the ball to its outer bounding box.


        
        double t = (((double)ballz) - R0[2])/Rd[2];
        
        
        double pt[3];
        pt[0] = R0[0] + t * Rd[0];
        pt[1] = R0[1] + t * Rd[1];
        pt[2] = ballz;
  //  printf("ray: %f,%f,%f",pt[0],pt[1],pt[2]);
    //printf("ball: %f,%f,%f",position.x,position.y,position.z);
    
        if ( pt[0] > ballx - (defOffset)  //if point is within bounding box
            && pt[0] < ballx + (defOffset)
            && pt[1] > bally - (defOffset)
            && pt[1] < bally + (defOffset)
            && pt[2] > ballz - (defOffset)
            && pt[2] < ballz + (defOffset)) {
         //   printf("true\n");
            return true;
        }
        else{
         //   printf("false\n");
            return false;
        }
        
        
    
}

//replace mouse function
bool resetting = false; //only true after user has left clicked a ball to reset
void mouse(int btn, int state, int x, int y){
    if (pendingStop){
        float ballx = position.x; float bally  = position.y; float ballz = position.z;

        if (calcInter(x, 800-y,ballx,bally,ballz)){
            pendingStop = false;
            resetting = true;
            resetBall();
        }
        
    }
    else if (btn == GLUT_LEFT_BUTTON && launched == false){
        if (state == GLUT_DOWN){
            startingMousepos[0] = x, startingMousepos[1] = y;       //store the starting mouse position
            startTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);          //store the time when the user first clicks down
            resetting = false;
        }
        else if(state == GLUT_UP && !resetting){
            
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

            
       //     printf("%f,%f\n", averageXVelocity,averageZVelocity);
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
    initTexture();
    callbackinit();
    setStartingParameters();

    glutMainLoop();             //starts the event loop

    return(0);                  //return may not be necessary on all compilers
}