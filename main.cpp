#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

using namespace std;
float lengthX = 2.0f, lengthY = 1.0f, lengthZ = 3.0f;

float cameraDistance = 5.0f;
float cameraAzimuth = 45.0f;
float cameraElevation = 30.0f;
float panX = 0.0f, panY = 0.0f;
bool rotating = false;
bool panning = false;

float objectPosX = 0.0f, objectPosY = 0.0f, objectPosZ = 0.0f;
bool isDragging = false;
int lastX = 0, lastY = 0;


void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void updateCamera(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(objectPosX, objectPosY, objectPosZ);

    float eyeX = cameraDistance * cos(cameraElevation * M_PI / 180.0) * sin(cameraAzimuth * M_PI / 180.0);
    float eyeY = cameraDistance * sin(cameraElevation * M_PI / 180.0);
    float eyeZ = cameraDistance * cos(cameraElevation * M_PI / 180.0) * cos(cameraAzimuth * M_PI / 180.0);

    gluLookAt(eyeX + panX, eyeY + panY, eyeZ,
              panX, panY, 0.0,
              0.0, 1.0, 0.0);
}


void drawWirePrism(float lx, float ly, float lz){
    float vertices[8][3] = {
            {-lx / 2, -ly / 2, -lz / 2}, {lx / 2, -ly / 2, -lz / 2},
            {lx / 2, ly / 2, -lz / 2}, {-lx / 2, ly / 2, -lz / 2},
            {-lx / 2, -ly / 2, lz / 2}, {lx / 2, -ly / 2, lz / 2},
            {lx / 2, ly / 2, lz / 2}, {-lx / 2, ly / 2, lz / 2}
        };

    int edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

    glBegin(GL_LINES);
    for (int i = 0; i < 12; ++i){
        glVertex3fv(vertices[edges[i][0]]);
        glVertex3fv(vertices[edges[i][1]]);
    }
    glEnd();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();

    glColor3f(1.0, 1.0, 1.0);
    drawWirePrism(lengthX, lengthY, lengthZ);
    //glutSolidSphere(3.0, 20, 20);

    glutSwapBuffers();
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouseMotion(int x, int y){
    if (rotating){
        cameraAzimuth -= (x - lastX) * 0.5f;
        cameraElevation += (y - lastY) * 0.5f;

        if (cameraElevation > 89.0f) cameraElevation = 89.0f;
        if (cameraElevation < -80.0f) cameraElevation = -89.0f;
    }
    else if (panning){
        panX -= (x - lastX) * 0.01f;
        panY += (y - lastY) * 0.01f;
    }
    else if (isDragging){
        float dx = (x - lastX) * 0.0125f;
        float dy = -(y - lastY) * 0.0125f;

        objectPosX += dx;
        objectPosY += dy;
    }

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if (button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN){
            rotating = true;
        }
        else{
            rotating = false;
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON){
        if (state == GLUT_DOWN){
            panning = true;
        }
        else{
            panning = false;
        }
    }
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN){
            isDragging = true;
            lastX = x;
            lastY = y;
        }
        else{
            isDragging = false;
        }
        lastX = x;
        lastY = glutGet(GLUT_WINDOW_HEIGHT) - y;
        return void();
    }

    lastX = x;
    lastY = y;
}

void mouseWheel(int wheel, int direction, int x, int y){
    if (direction > 0){
        cameraDistance -= 0.5f;
    }

    else{
        cameraDistance += 0.5f;
    }

    if (cameraDistance < 1.0f) cameraDistance = 1.0f;
    if (cameraDistance > 50.0f) cameraDistance = 50.0f;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    if (key == 27){
        exit(0);
    }

    if (key == 'n'){
        panX = 0.0f;
        panY = 0.0f;

        cameraDistance = 5.0f;

        glutPostRedisplay();
    }

    if (key == 'z'){
        cameraAzimuth = 0.0f;
        cameraElevation = 0.0f;

        panX = 0.0f;
        panY = 0.0f;

        glutPostRedisplay();
    }

    if (key == 'x'){
        cameraAzimuth = 90.0f;
        cameraElevation = 0.0f;

        panX = 0.0f;
        panY = 0.0f;

        glutPostRedisplay();
    }

    if (key == 'y'){
        cameraAzimuth = 0.0f;
        cameraElevation = 90.0f;

        panX = 0.0f;
        panY = 0.0f;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mouse Interaction");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
