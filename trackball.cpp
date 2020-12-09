// Program implements virtual trackball, translation, and scaling.
// trackball.cpp

#include <iostream>
#include <fstream>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include <sys/types.h>
#include <GLUT/glut.h>

#include "objLoader.hpp"

#define KEY_LEFT 100
#define KEY_UP 101
#define KEY_RIGHT 102
#define KEY_DOWN 103

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

int winWidth = 1024;
int winHeight = 1024;
bool firstTime = true;

WavefrontObj *obj_data;

// Trackball parameters initialization
float angle = 0.0, axis[3], trans[3];

bool trackingMouse = false;
bool redrawContinue = false;
bool trackballMove = false;

float lastPos[3] = { 0.0, 0.0, 0.0 };
int curx, cury;
int startX, startY;

// Translation & Rotation
float x_trans = 0.0f; // translate object in x direction
float y_trans = 0.0f; // translate object in y direction
float zoom = 1.0f; // zoom for scaling

void Init(int w, int h)
{
    glViewport(0, 0, w, h);
    glShadeModel(GL_SMOOTH);                                // Set Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                    // Background Color
    glClearDepth(1.0f);                                        // Depth buffer setup
    glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                    // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);        // Use perspective correct interpolation if available
    
    glMatrixMode(GL_PROJECTION);                            // Select The Projection Matrix
    glLoadIdentity();                                        // Reset The Projection Matrix
    double aspect = (double)h / w;
    glFrustum(-5, 5, -5 * aspect, 5 * aspect, 10, 500);          // Define perspective projection frustum
    //gluPerspective(30, w/h, 10, 74);
    glTranslated(0.0, 0.0, -24);                          // Viewing transformation
    
    glMatrixMode(GL_MODELVIEW);                                // Select The Modelview Matrix
    glLoadIdentity();                                        // Reset The Modelview Matrix
    
    if (firstTime)
    {
        glEnable(GL_LIGHTING);
        float frontColor[] = { 0.2f, 0.7f, 0.7f, 1.0f };
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, frontColor);
        glMaterialf(GL_FRONT, GL_SHININESS, 100);
        
        float lightDirection[] = { 2.0f, 0.0f, 1.0f, 0 };
        float ambientIntensity[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        float lightIntensity[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        float lightSpec[] = { 1.0f, 1.0f, 1.0f, 1 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientIntensity);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
        glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
        glEnable(GL_LIGHT0);
        firstTime = false;
    }
}


void Draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear The Screen And The Depth Buffer
    
    if (trackballMove){
        glRotatef(angle, axis[0], axis[1], axis[2]);
    }
        
    if (obj_data != NULL)
        obj_data->Draw();
    else
        glutSolidTeapot(1.0);    //draw a teapot when no argument is provided
    
    glFlush();
    glutSwapBuffers();
}

// handles translation along x, y, and z directions in eye space using keyboard callback
void onKeyPress(unsigned char key, int x, int y){
    // a key translates left
    if (key == 'a'){
        glTranslatef(-1.0f, 0.0f, 0.0f);
    }
    // w key translates up
    else if (key == 'w'){
        glTranslatef(0.0f, 1.0f, 0.0f);
    }
    // d key translates right
    else if (key == 'd'){
        glTranslatef(1.0f, 0.0f, 0.0f);
    }
    // s key translates down
    else if (key == 's'){
        glTranslatef(0.0f, -1.0f, 0.0f);
    }
    // z key translates out along z axix (similar to zooming)
    else if (key == 'z'){
        glTranslatef(0.0f, 0.0f, -1.0f);
    }
    // x key translates in along z axis (similar to zooming)
    else if (key == 'x'){
        glTranslatef(0.0f, 0.0f, 1.0f);
    }
    
    glutPostRedisplay();
}

// handles zoom in and zoom out (scaling) using up and down kets
void Specialkey(int key, int x, int y){
    // press key up for zoom in
    if(key == KEY_UP){
        glScalef(1.10f * zoom, 1.10f * zoom, 1.10f * zoom);
        glutPostRedisplay();
    }
    // press key down for zoom out
    if(key == KEY_DOWN){
        glScalef(0.90f * zoom, 0.90f * zoom, 0.90f * zoom);
        glutPostRedisplay();
    }
}
    
// handles virtual trackball motion rotation
void mouseMotion(int x, int y)
{
    float curPos[3],
    dx, dy, dz;
    
    float d, a;
     curPos[0] = (2.0*x - winWidth) / winWidth;
     curPos[1] = (winHeight - 2.0F*y) / winHeight;
     d = sqrt(curPos[0]*curPos[0] + curPos[1]*curPos[1]);
     curPos[2] = cos((M_PI/2.0) * ((d < 1.0) ? d : 1.0));
     a = 1.0 / sqrt(curPos[0]*curPos[0] + curPos[1]*curPos[1] + curPos[2]*curPos[2]);
     curPos[0] *= a;
     curPos[1] *= a;
     curPos[2] *= a;
    
    if(trackingMouse){
        dx = curPos[0] - lastPos[0];
        dy = curPos[1] - lastPos[1];
        dz = curPos[2] - lastPos[2];
        if (dx || dy || dz){
         angle = 90.0 * sqrt(dx*dx + dy*dy + dz*dz);
         axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
         axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
         axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];
         lastPos[0] = curPos[0];
         lastPos[1] = curPos[1];
         lastPos[2] = curPos[2];
         }
    }
    
    glutPostRedisplay();
}

// handles virtual trackball motion rotation
void mouseButton(int button, int state, int x, int y){
    if(button==GLUT_RIGHT_BUTTON) exit(0);
    if(button==GLUT_LEFT_BUTTON) switch(state){
     case GLUT_DOWN:
        y=winHeight-y;
        trackingMouse = true;
        redrawContinue = false;
        startX = x;
        startY = y;
        curx = x;
        cury = y;
        float d, a;
        lastPos[0] = (2.0*x - winWidth) / winWidth;
        lastPos[1] = (winHeight - 2.0F*y) / winHeight;
        d = sqrt(lastPos[0]*lastPos[0] + lastPos[1]*lastPos[1]);
        lastPos[2] = cos((M_PI/2.0) * ((d < 1.0) ? d : 1.0));
        a = 1.0 / sqrt(lastPos[0]*lastPos[0] + lastPos[1]*lastPos[1] + lastPos[2]*lastPos[2]);
        lastPos[0] *= a;
        lastPos[1] *= a;
        lastPos[2] *= a;
        trackballMove=true;
        break;
     case GLUT_UP:
        trackingMouse = false;
        if (startX != x || startY != y)
            redrawContinue = true;
        else {
            angle = 0.0;
            redrawContinue = false;
            trackballMove = false;
        }
        break;
    }
}

int main(int argc, char *argv[])
{
    // glut initialization functions:
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ImageViewer");
    
    Init(winWidth, winHeight);
    
    // display, onMouseButton, mouse_motion, onKeyPress, and resize are functions defined above
    glutDisplayFunc(Draw);
    glutKeyboardFunc(onKeyPress);
    glutSpecialFunc(Specialkey);  //use this function if you would like to use arrow buttons on your keyboard
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(Init);
    
    if (argc >= 2)
        obj_data = new WavefrontObj(argv[1]);
    else
        obj_data = NULL;
    
    // start glutMainLoop -- infinite loop
    glutMainLoop();
    
    return 0;
}
