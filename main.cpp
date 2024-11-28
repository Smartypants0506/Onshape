#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cfloat>

using namespace std;

float cameraDistance = 5.0f;
float cameraAzimuth = 45.0f;
float cameraElevation = 30.0f;
float panX = 0.0f, panY = 0.0f;
bool rotating = false;
bool panning = false;
int lastX = 0, lastY = 0;

class RectangularPrism {
public:
    float lx = 2.0f, ly = 1.0f, lz = 3.0f;
    float objectPosX = 0.0f, objectPosY = 0.0f, objectPosZ = 0.0f;
    bool isDragging = false;
    bool solid = false;

    RectangularPrism(float lengthX, float lengthY, float lengthZ, bool solid) {
        this -> lx = lengthX;
        this -> ly = lengthY;
        this -> lz = lengthZ;
        this -> solid = solid;
    }

    RectangularPrism(float lengthX, float lengthY, float lengthZ) {
        this -> lx = lengthX;
        this -> ly = lengthY;
        this -> lz = lengthZ;
        this -> solid = false;
    }

    RectangularPrism() {
        this -> lx = 1.0f, this -> ly = 1.0f, this -> lz = 1.0f;
        this -> solid = false;
    }

    vector<pair<float, float>> getScreenCoordinates() {
        float vertices[8][3] = {
            {-lx / 2, -ly / 2, -lz / 2}, {lx / 2, -ly / 2, -lz / 2},
            {lx / 2, ly / 2, -lz / 2}, {-lx / 2, ly / 2, -lz / 2},
            {-lx / 2, -ly / 2, lz / 2}, {lx / 2, -ly / 2, lz / 2},
            {lx / 2, ly / 2, lz / 2}, {-lx / 2, ly / 2, lz / 2}
        };

        vector<pair<float, float>> screenCoordinates;
        GLdouble modelView[16], projection[16];
        GLint viewport[4];

        glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        for (int i = 0; i < 8; ++i) {
            GLdouble winX, winY, winZ;
            if (gluProject(vertices[i][0] + objectPosX, vertices[i][1] + objectPosY, vertices[i][2] + objectPosZ,
                           modelView, projection, viewport,
                           &winX, &winY, &winZ)) {
                screenCoordinates.emplace_back(winX, winY);
                           }
        }

        return screenCoordinates;
    }

    void drawPrism() {
        float vertices[8][3] = {
            {-lx / 2, -ly / 2, -lz / 2}, {lx / 2, -ly / 2, -lz / 2},
            {lx / 2, ly / 2, -lz / 2}, {-lx / 2, ly / 2, -lz / 2},
            {-lx / 2, -ly / 2, lz / 2}, {lx / 2, -ly / 2, lz / 2},
            {lx / 2, ly / 2, lz / 2}, {-lx / 2, ly / 2, lz / 2}
        };

        int edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

        if (solid) {
            // Draw solid prism using quads
            glBegin(GL_QUADS);

            // Front face
            glVertex3fv(vertices[0]);
            glVertex3fv(vertices[1]);
            glVertex3fv(vertices[5]);
            glVertex3fv(vertices[4]);

            // Back face
            glVertex3fv(vertices[3]);
            glVertex3fv(vertices[2]);
            glVertex3fv(vertices[6]);
            glVertex3fv(vertices[7]);

            // Left face
            glVertex3fv(vertices[0]);
            glVertex3fv(vertices[3]);
            glVertex3fv(vertices[7]);
            glVertex3fv(vertices[4]);

            // Right face
            glVertex3fv(vertices[1]);
            glVertex3fv(vertices[2]);
            glVertex3fv(vertices[6]);
            glVertex3fv(vertices[5]);

            // Top face
            glVertex3fv(vertices[4]);
            glVertex3fv(vertices[5]);
            glVertex3fv(vertices[6]);
            glVertex3fv(vertices[7]);

            // Bottom face
            glVertex3fv(vertices[0]);
            glVertex3fv(vertices[1]);
            glVertex3fv(vertices[2]);
            glVertex3fv(vertices[3]);
        }

        else {
            glBegin(GL_LINES);
            for (int i = 0; i < 12; ++i) {
                glVertex3fv(vertices[edges[i][0]]);
                glVertex3fv(vertices[edges[i][1]]);
            }
            glEnd();
        }
    }
};

RectangularPrism* prism = new RectangularPrism(1.0f, 2.0f, 3.0f, false);

bool isPointInsideObject(int mouseX, int mouseY) {
    auto screenCoords = prism -> getScreenCoordinates();

    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;

    for (const auto& coord : screenCoords) {
        minX = std::min(minX, coord.first);
        maxX = std::max(maxX, coord.first);
        minY = std::min(minY, coord.second);
        maxY = std::max(maxY, coord.second);
    }

    return mouseX >= minX && mouseX <= maxX && mouseY >= minY && mouseY <= maxY;
}

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void updateCamera(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(prism -> objectPosX, prism -> objectPosY, prism -> objectPosX);

    float eyeX = cameraDistance * cos(cameraElevation * M_PI / 180.0) * sin(cameraAzimuth * M_PI / 180.0);
    float eyeY = cameraDistance * sin(cameraElevation * M_PI / 180.0);
    float eyeZ = cameraDistance * cos(cameraElevation * M_PI / 180.0) * cos(cameraAzimuth * M_PI / 180.0);

    gluLookAt(eyeX + panX, eyeY + panY, eyeZ,
              panX, panY, 0.0,
              0.0, 1.0, 0.0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();
    glColor3f(1.0, 1.0, 1.0);
    prism -> drawPrism();

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
    if (prism -> isDragging) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int invertedY = windowHeight - y;

        float dx = (x - lastX) * 0.0125f;
        float dy = -(invertedY - lastY) * 0.0125f;

        prism -> objectPosX += dx;
        prism -> objectPosY -= dy;

        lastX = x;
        lastY = invertedY;

        glutPostRedisplay();

        return void();
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
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int invertedY = windowHeight - y;

        if (isPointInsideObject(x, invertedY)) {
            prism -> isDragging = true;
            lastX = x;
            lastY = invertedY;
        }
    } else if (state == GLUT_UP) {
        prism -> isDragging = false;
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
    glutCreateWindow("Onshaper");

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
