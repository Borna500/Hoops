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

int firstwindow;
int secondwindow;
float amb0[4] = {1,1,1,1};
float diff0[4] = {1,1,1,1};
float spec0[4] = {1,1,1,1};
float m_amb[] = {0.33, 0.22, 0.03, 1.0}; 
float m_diff[] = {0.78, 0.57, 0.11, 1.0}; 
float m_spec[] = {0.99, 0.91, 0.81, 1.0}; 
float shiny = 27.8;
float yAngle = 0;
float xAngle = 0;

float boxDepth = 5;
bool selected = false;
bool throwing = false;
bool launched = false;
int startingMousepos[2];
int finalMousepos[2];
float startTime;
float endTime;
float averageXVelocity;
float averageZVelocity;

float windspeed = 0;
float position[] = {0,1,-4};
float startingPos[] = {0,1,-4};
float basketPos[] = {0, 0, 4};
float basketRadius = 1;
float velocity[]  = {0,0,0};
float acceleration[] = {windspeed,-100,0};

/*** 
 EYE LOCATION
 ***/
float eye[] = {0,1,-7};
float lookat[] = {0,0,0};
float light1_pos[]= {0,4,-4,1}; //last num one is pofloat light
float light2_pos[]= {0,0,0,1}; //last num one is pofloat light


float angle1 = 0.0f;
/* display function - GLUT display callback function
 *      clears the screen, sets the camera position, draws the ground plane and movable box
 */
int mapSize = 300;
float heightMap[1000][1000];
int maxX = mapSize;
int maxZ = mapSize;

//self explanatory various states of the mesh
enum mode{solid, wire, both};
mode currentMode;

enum shading{smooth, flat};
shading currentShading;

bool triangles;
bool lighting;

enum mapType {fault, circles};
mapType currentMap;

bool vertexShading;


float minHeight = 0;
float maxHeight = 0;


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



void updateeyeposition(){
    
    eye[0] = eyeradius * sin(eyephi) * cos(eyetheta);
    eye[1] = eyeradius  * cos(eyephi);
    eye[2] = eyeradius * sin(eyetheta) * sin(eyephi);
    
}

void updatelookatposition(){
    
    lookat[0] = lookatradius * sin(lookatphi) * cos(lookattheta);
    lookat[1] = lookatradius  * cos(lookatphi);
    lookat[2] = lookatradius * sin(lookattheta) * sin(lookatphi);
}

// BORNA UPDATE TO HERE

void drawFloor(){
    //floor
    glPushMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(1, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f(5, 0, 5);
        glVertex3f(5, 0, -5);
        glVertex3f(-5, 0, -5);
        glVertex3f(-5, 0, 5);
        glEnd();
    glPopMatrix();
    
    //ceiling
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glBegin(GL_QUADS);
    glVertex3f(5, 0, 5);
    glVertex3f(5, 0, -5);
    glVertex3f(-5, 0, -5);
    glVertex3f(-5, 0, 5);
    glEnd();
    glPopMatrix();
    
    //right wall
    glPushMatrix();
    glTranslatef(-5, 5, 0);
    glRotatef(90, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(5, 0, 5);
    glVertex3f(5, 0, -5);
    glVertex3f(-5, 0, -5);
    glVertex3f(-5, 0, 5);
    glEnd();
    glPopMatrix();
    
    //left wall
    glPushMatrix();
    glTranslatef(5, 5, 0);
    glRotatef(90, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(5, 0, 5);
    glVertex3f(5, 0, -5);
    glVertex3f(-5, 0, -5);
    glVertex3f(-5, 0, 5);
    glEnd();
    glPopMatrix();
    
    //back wall
    glPushMatrix();
    glTranslatef(0, 5, 5);
    glRotatef(90, 1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(5, 0, 5);
    glVertex3f(5, 0, -5);
    glVertex3f(-5, 0, -5);
    glVertex3f(-5, 0, 5);
    glEnd();
    glPopMatrix();
}

void drawBall(){
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(position[0],position[1],position[2]);
    glutSolidSphere(1, 100, 100);
    //glutSolidCube(1);
    glPopMatrix();
}

void drawBasket() {
    glColor3f(1,0,0);
    glPushMatrix();
        glTranslatef(basketPos[0], basketPos[1], basketPos[2]);
        glRotatef(90, 1, 0, 0);
        glutSolidTorus(basketRadius / 2, basketRadius, 100, 100);
    glPopMatrix();
}

bool grounded = true;
bool bounced = false;
void ballMotion(int value){

    if(abs(velocity[1]) < 0.5 && position[1] < 1){
        velocity[0] = 0, velocity[2] = 0, velocity[1] = 0;
        launched = false;
    }
    if (launched == true){
    velocity[0] += acceleration[0]/60;
    velocity[1] += acceleration[1]/60;
    velocity[2] += acceleration[2]/60;
    position[0] += velocity[0]/60;
    position[1] += velocity[1]/60;
    position[2] += velocity[2]/60;
//    if ((position[1]) < 1){
//        position[1] = 1;
//        velocity[0] = 0;
//        velocity[1] = 0;
        
//        velocity[2] = 0;
//        averageXVelocity = 0;
//        averageZVelocity = 0;
//        acceleration[0] = 0;
//        acceleration[2] = 0;
//        launched = false;
//        position[0] = startingPos[0], position[1] = startingPos[1], position[2] = startingPos[2];
//        
//    }
    
        if (position[2] > 5 ){
            velocity[2] = -1*(velocity[2] - 0.5*velocity[2]);
            position[2] = 5.0 + velocity[2]/60;
            printf("out of z pos wall");
        }
        else if (position[2] < -5 ){
            velocity[2] = -1*(velocity[2] - 0.5*velocity[2]);
            position[2] = -5.0 + velocity[2]/60;
            printf("out of z neg wall");
        }
        if (position[0] < -5){
            velocity[0] = -1*(velocity[0] - 0.5*velocity[0]);
            position[0] = -5.0 + velocity[0]/60;
            printf("out of x neg wall");
        }
        else if (position[0] > 5){
            velocity[0] = -1*(velocity[0] - 0.5*velocity[0]);
            position[0] = 5.0 + velocity[0]/60;
            printf("out of x pos wall");
        }
        if (position[1] < 1){
            //printf("%f",position[1]);
            velocity[1] = -1*(velocity[1] - 0.5*velocity[1]);       //lose half magnitude and reverse direction
            position[1] = 1.0 + velocity[1]/60;
            bounced = true;
        }
        //Intersection with inside of basket (scoring)
        if ((position[0] > (basketPos[0] - basketRadius)) && (position[0] < (basketPos[0] + basketRadius))) {
            if ((position[2] > (basketPos[2] - basketRadius)) && (position[2] < (basketPos[2] + basketRadius))) {
                if (position[1] > 1.0f && position[1] < 2.0f && !bounced) {
                    printf("YOU DID IT\n");
                }
            }
        }
        //Intersection with outside of basket (rim shot)
         if ((position[0] > (basketPos[0] - basketRadius)) && (position[0] < (basketPos[0] + basketRadius))) {
             if ((position[2] > (basketPos[2] - basketRadius)) && (position[2] < (basketPos[2] + basketRadius))) {
                if (position[1] < 1.05f && bounced) {
                    printf("BOUNCED\n");
                     velocity[0] = -1*(velocity[0] - 0.5*velocity[0]);       //lose half magnitude and reverse direction
                     position[0] = position[0] + velocity[0]/60;

                     velocity[2] = -1*(velocity[2] - 0.5*velocity[2]);       //lose half magnitude and reverse direction
                     position[2] = position[2] + velocity[2]/60;
                }
            }
        }

    
    
    }
    glutTimerFunc(17,ballMotion,0);

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
            
        case 'u':
        case 'U':
            printf("Type the x,y,z values of your look at \n");
            scanf("%f", &input1);
            scanf("%f", &input2);
            scanf("%f", &input3);
            lookat[0] = input1;
            lookat[1] = input2;
            lookat[2] = input2;
            break;
            
        case 'v':
        case 'V':
            printf("Type the x,y,z values of your camera \n");
            scanf("%f", &input1);
            scanf("%f", &input2);
            scanf("%f", &input3);
            eye[0] = input1;
            eye[1] = input2;
            eye[2] = input2;
            break;
        case 'a':
            lookatradius += 0.1;
            break;
            
        case 'A':
            lookatradius -= 0.1;
            break;
            
            
        case 's':
            lookattheta += 0.05;
            break;
            
            
        case 'S':
            lookattheta -= 0.05;
            break;
            
        case 'd':
            lookatphi += 0.05;
            break;
            
        case 'D':
            lookatphi -= 0.05;
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
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec0);

    
    glMatrixMode(GL_PROJECTION); 
    glFrustum(-10,10,-10,10,0,1000);
    gluPerspective(90, 1, 1, 1000);

}




void display(void)
{
    float origin[3] = {0,0,0};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /************************************************************************
     
                                    CAMERA SET UP
     
     ************************************************************************/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    updateeyeposition();
    updatelookatposition();
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2], lookat[0],lookat[1],lookat[2], 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light2_pos);
    
    drawFloor();
    drawBall();
    drawBasket();
    glutSwapBuffers();
    glutPostRedisplay();
    
}



bool calcIntersection(vec3D r0, vec3D rd) {
    //Max and min of bounding box for ball
    vec3D min, max;
    min.x = position[0] - 0.5f; min.y = position[1] - 0.5f; min.z = position[2] - 0.5f;
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



//void mouse(int btn, int state, int x, int y){
//    if (btn == GLUT_LEFT_BUTTON && launched == false && selected == true){
//        if (state == GLUT_DOWN){
//            throwing = true;
//            startingMousepos[0] = x, startingMousepos[1] = y;       //store the starting mouse position
//            startTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);          //store the time when the user first clicks down
//        }
//        else if(state == GLUT_UP && throwing == true){
//            
//            finalMousepos[0] = x;
//            finalMousepos[1] = y;
//            endTime=(GLfloat)glutGet(GLUT_ELAPSED_TIME);          //store the time when the user first clicks down
//            
//            float timeElapsed = endTime - startTime;
//            float dy = -(finalMousepos[1] - startingMousepos[1]);
//            float dx = -(finalMousepos[0] - startingMousepos[0]);
//            
//            averageXVelocity = dx/timeElapsed;
//            averageZVelocity = dy/timeElapsed;
//                        velocity[0] += averageXVelocity*5;
//                        velocity[1] = averageZVelocity*5;
//                        velocity[2] += averageZVelocity*5;
//            
//            printf("%f,%f\n", averageXVelocity,averageZVelocity);
//            launched = true;
//            selected = false;
//            throwing = false;
//        }
//    }
//    //Ray casting for selecting the ball
//    else if (btn == GLUT_LEFT_BUTTON && launched == false && selected == false) {
//        if (state == GLUT_DOWN) {
//            if (rayCast(float(x), float(y)) == true) {
//                printf("Selected the object\n");
//                selected = true;
//            }
//        }
//    }
//    
//}

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
            float dx = -(finalMousepos[0] - startingMousepos[0]);
            
            averageXVelocity = dx/timeElapsed;
            averageZVelocity = dy/timeElapsed;
            velocity[0] += averageXVelocity*5;
            velocity[1] = averageZVelocity*5;
            velocity[2] += averageZVelocity*5;
            
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

    firstwindow = glutCreateWindow("Terrain");  //creates the window
    
    
    glutDisplayFunc(display);   //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    
    glEnable(GL_DEPTH_TEST);
    //glFrontFace((GL_CCW));
    //glCullFace(GL_FRONT);
    //glEnable(GL_CULL_FACE);
    init();
    callbackinit();

    glutMainLoop();             //starts the event loop

    return(0);                  //return may not be necessary on all compilers
}