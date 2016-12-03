/* Snowman sample 
 * by R. Teather
 * Edited by Noel Brett
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

bool launched = false;
int startingMousepos[2];
int finalMousepos[2];
float startTime;
float endTime;
float averageXVelocity;
float averageZVelocity;

float windspeed = 0;
float startingPos[] = {0,1,-5};
float position[] = {0,1,-5};
float velocity[]  = {0,0,0};
float acceleration[] = {windspeed,-30,0};

/*** 
 EYE LOCATION
 ***/
float eye[] = {0,1,-7};
float lookat[] = {0,0,0};
float light1_pos[]= {0,5,0,1}; //last num one is pofloat light
float light2_pos[]= {300,5,300,1}; //last num one is pofloat light


float angle1 = 0.0f;
/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
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


//face normal map
float normalMap[1000][1000][3];
//vertex normal map
float vertexnormalMap[1000][1000][3];
void calcNormals(){
    //face normals calculated by taking the cross product of two perpendicular vectors on the face
    for (int x=0; x< mapSize; x++ ){
        for(int z = 0; z < mapSize; z++){
            float point1[] = {static_cast<float>(x),heightMap[x][z],static_cast<float>(z)};
            float point2[] = {static_cast<float>(x),heightMap[x][z+1],static_cast<float>(z+1)};
            float point3[] = {static_cast<float>(x+1),heightMap[x+1][z],static_cast<float>(z)};
            
            float v[3];
            v[0] = point2[0] - point1[0];
            v[1] = point2[1] - point1[1];
            v[2] = point2[2] - point1[2];
            float u[3];
            u[0] = point3[0] - point1[0];
            u[1] = point3[1] - point1[1];
            u[2] = point3[2] - point1[2];
            
            
            
            float s1 = u[1]*v[2] - u[2]*v[2];
            float s2 = u[2]*v[0] - v[2]*u[0];
            float s3 = u[0]*v[1] - v[0]*u[1];
            
            float length = sqrtf(s1*s1 + s2*s2 + s3*s3);
            normalMap[x][z][0] = s1/length;
            normalMap[x][z][1] = s2/length;
            normalMap[x][z][2] = s3/length;
            
        }
        
    }
    //vertex normal is calculated by normalizing and averaging all the face normals surrounding a given vertex
    for (int x=0; x< mapSize; x++ ){
        for(int z = 0; z < mapSize; z++){
            if (0 < x && x < mapSize && z > 0 && z < mapSize){
                vertexnormalMap[x][z][0] = (normalMap[x][z+1][0] + normalMap[x][z-1][0] + normalMap[x+1][z][0] + normalMap[x-1][z][0])/4;
                vertexnormalMap[x][z][1] = (normalMap[x][z+1][1] + normalMap[x][z-1][1] + normalMap[x+1][z][1] + normalMap[x-1][z][1])/4;
                vertexnormalMap[x][z][2] = (normalMap[x][z+1][2] + normalMap[x][z-1][2] + normalMap[x+1][z][2] + normalMap[x-1][z][2])/4;
            }
        }}
    
    
}

float minHeight = 0;
float maxHeight = 0;


//when middle of ball hits y of 0.5 check whether x and z are in basket bounds

//draws the mesh as quads

//same as quads, but just with triangles
void drawTriangles(){
    for(int x = 0; x< maxX; x++){
        for (int z = 0; z < maxZ; z++){
            if (!vertexShading){
            glBegin(GL_TRIANGLES);
            
            glColor3f((heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight));
            glNormal3fv(normalMap[x][z+1]);
            glVertex3f(x,heightMap[x][z+1],z+1);
            
            glColor3f((heightMap[x][z] - minHeight)/(maxHeight - minHeight),(heightMap[x][z] - minHeight)/(maxHeight - minHeight),(heightMap[x][z] - minHeight)/(maxHeight - minHeight));
            glNormal3fv(normalMap[x][z]);
            glVertex3f(x,heightMap[x][z], z);
            
            glColor3f((heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight));
            glNormal3fv(normalMap[x+1][z]);
            glVertex3f(x+1, heightMap[x+1][z], z);
            glEnd();
            
            glBegin(GL_TRIANGLES);
            
            glColor3f((heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight));
            glNormal3fv(normalMap[x+1][z]);
            glVertex3f(x+1,heightMap[x+1][z],z);
            
            glColor3f((heightMap[x+1][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z+1] - minHeight)/(maxHeight - minHeight));
            glNormal3fv(normalMap[x+1][z+1]);
            glVertex3f(x+1,heightMap[x+1][z+1], z+1);
            
            glColor3f((heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight));
            glNormal3fv(normalMap[x][z+1]);
            glVertex3f(x, heightMap[x][z+1], z+1);
            glEnd();
            }
            else{
                glBegin(GL_TRIANGLES);
                
                glColor3f((heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight));
                glNormal3fv(vertexnormalMap[x][z+1]);
                glVertex3f(x,heightMap[x][z+1],z+1);
                
                glColor3f((heightMap[x][z] - minHeight)/(maxHeight - minHeight),(heightMap[x][z] - minHeight)/(maxHeight - minHeight),(heightMap[x][z] - minHeight)/(maxHeight - minHeight));
                glNormal3fv(vertexnormalMap[x][z]);
                glVertex3f(x,heightMap[x][z], z);
                
                glColor3f((heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight));
                glNormal3fv(vertexnormalMap[x+1][z]);
                glVertex3f(x+1, heightMap[x+1][z], z);
                glEnd();
                
                glBegin(GL_TRIANGLES);
                
                glColor3f((heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z] - minHeight)/(maxHeight - minHeight));
                glNormal3fv(vertexnormalMap[x+1][z]);
                glVertex3f(x+1,heightMap[x+1][z],z);
                
                glColor3f((heightMap[x+1][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x+1][z+1] - minHeight)/(maxHeight - minHeight));
                glNormal3fv(vertexnormalMap[x+1][z+1]);
                glVertex3f(x+1,heightMap[x+1][z+1], z+1);
                
                glColor3f((heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight),(heightMap[x][z+1] - minHeight)/(maxHeight - minHeight));
                glNormal3fv(vertexnormalMap[x][z+1]);
                glVertex3f(x, heightMap[x][z+1], z+1);
                glEnd();
            }
            
        }
    }
}
//function to deal with all the toggle options and call the appropriate drawing func
void drawHeightMap(){
    glPushMatrix();
    //move and rotate the map according to the user's commands, alternative to moving the camera around
    glTranslatef(-mapSize,0,-mapSize);
    glRotatef(xAngle, 1, 0, 0);
    glRotatef(yAngle, 0, 1, 0);
    switch (currentMode) {
        case solid:
             glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            break;
        case wire:
             glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            break;
        case both:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            if (triangles)drawTriangles();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }
    switch ( currentShading) {
        case smooth:
            glShadeModel(GL_SMOOTH);
            break;
        case flat:
            glShadeModel(GL_FLAT);
            break;
    }
    if (lighting){
        glEnable(GL_LIGHTING);
    }
    else{
        glDisable(GL_LIGHTING);
    }
    
    if(triangles)drawTriangles();
}


void drawFloor(){
    glPushMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(1, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f(5, 0, 5);
        glVertex3f(5, 0, -5);
        glVertex3f(-5, 0, -5);
        glVertex3f(-5, 0, 5);
        glEnd();
    
        glPushMatrix();
            glColor3f(1, 1, 1);
                glPushMatrix();
                glTranslatef(position[0],position[1],position[2]);
    
                    //printf("%f",position[1]);
                glutSolidSphere(1, 100, 100);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

}
bool grounded = true;
void ballMotion(int value){
    if (launched == true){
    velocity[0] += acceleration[0]/60;
    velocity[1] += acceleration[1]/60;
    velocity[2] += acceleration[2]/60;
    position[0] += velocity[0]/60;
    position[1] += velocity[1]/60;
    position[2] += velocity[2]/60;
    if ((position[1]) < 1){
        position[1] = 1;
        velocity[0] = 0;
        velocity[1] = 0;
        velocity[2] = 0;
        averageXVelocity = 0;
        averageZVelocity = 0;
        acceleration[0] = 0;
        acceleration[2] = 0;
        launched = false;
        position[0] = startingPos[0], position[1] = startingPos[1], position[2] = startingPos[2];
        
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
{
    printf("commands:\n 'n' = toggle shading\n 't' = triangles/quads\n 'u' = solid/wire/both\n 'l' = lighting\n 'y' = toggle size 50/300\n 'f' = toggle fault/circles algorithm\n 'v' = toggle vertex/face normals\n 'r' = draw new map\n 'wasd' = move lights\n extra features: fault algorithm, 2D representation, manual terrain manipulation by clicking on map");
	glClearColor(0, 0, 0, 0);
	glColor3f(1,0,0);
    
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    /************************************************************************
     
                            PERSPECTIVE SET UP!
     
     modelview moves the eye and objects, projection is for camera type
     frustum / ortho left, right, bottom, top, nearVal, farVal
     
     ************************************************************************/
    glMatrixMode(GL_PROJECTION); 
    glFrustum(-10,10,-10,10,0,1000);
    gluPerspective(90, 1, 1, 1000);

    currentMode = solid;
    currentShading = smooth;
    lighting = true;
    triangles = true;
    currentMap = circles;
    vertexShading = true;

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
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2], lookat[0],lookat[1],lookat[2], 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT0, GL_POSITION, light2_pos);
    drawFloor();
    glutSwapBuffers();
    glutPostRedisplay();
    //glutSetWindow(firstwindow);
    
    
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
            float dx = -(finalMousepos[0] - startingMousepos[0]);
            
            averageXVelocity = dx/timeElapsed;
            averageZVelocity = dy/timeElapsed;
            velocity[0] += averageXVelocity*3;
            velocity[1] = 10;
            velocity[2] += averageZVelocity*3;
            
            printf("%f,%f\n", averageXVelocity,averageZVelocity);
            launched = true;
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
    
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	firstwindow = glutCreateWindow("Terrain");	//creates the window
    
    
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
	glutSpecialFunc(special);
    
	glEnable(GL_DEPTH_TEST);
    //glFrontFace((GL_CCW));
    //glCullFace(GL_FRONT);
    //glEnable(GL_CULL_FACE);
    init();
    callbackinit();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
