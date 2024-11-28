#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

// Camera angles for azimuth (horizontal) and elevation (vertical)
float cameraAzimuth = 45.0f; // Horizontal rotation angle
float cameraElevation = 30.0f; // Vertical rotation angle
float cameraDistance = 5.0f; // Distance from the origin

// Mouse state
int isRightMouseButtonDown = 0; // Is the right mouse button pressed?
int lastMouseX, lastMouseY;     // Previous mouse positions

void init() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
}

void drawWirePrism(float lx, float ly, float lz) {
    // Vertices of the rectangular prism
    float vertices[8][3] = {
        {-lx / 2, -ly / 2, -lz / 2}, // 0: Bottom-back-left
        { lx / 2, -ly / 2, -lz / 2}, // 1: Bottom-back-right
        { lx / 2,  ly / 2, -lz / 2}, // 2: Top-back-right
        {-lx / 2,  ly / 2, -lz / 2}, // 3: Top-back-left
        {-lx / 2, -ly / 2,  lz / 2}, // 4: Bottom-front-left
        { lx / 2, -ly / 2,  lz / 2}, // 5: Bottom-front-right
        { lx / 2,  ly / 2,  lz / 2}, // 6: Top-front-right
        {-lx / 2,  ly / 2,  lz / 2}  // 7: Top-front-left
    };

    // Edges of the rectangular prism (pairs of vertex indices)
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Back face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Front face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting edges
    };

    // Draw edges
    glBegin(GL_LINES);
    for (int i = 0; i < 12; ++i) {
        glVertex3fv(vertices[edges[i][0]]);
        glVertex3fv(vertices[edges[i][1]]);
    }
    glEnd();
}

// Update camera view based on azimuth and elevation
void updateCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Convert polar coordinates to Cartesian for camera position
    float eyeX = cameraDistance * cos(cameraElevation * M_PI / 180.0) * sin(cameraAzimuth * M_PI / 180.0);
    float eyeY = cameraDistance * sin(cameraElevation * M_PI / 180.0);
    float eyeZ = cameraDistance * cos(cameraElevation * M_PI / 180.0) * cos(cameraAzimuth * M_PI / 180.0);

    gluLookAt(eyeX, eyeY, eyeZ,  // Camera position
              0.0, 0.0, 0.0,    // Look-at point (origin)
              0.0, 1.0, 0.0);   // Up direction
}

// Render the wireframe cube


// Handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Handle mouse button events
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRightMouseButtonDown = 1; // Right button pressed
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isRightMouseButtonDown = 0; // Right button released
        }
    }
}

// Handle mouse motion while dragging
void motion(int x, int y) {
    if (isRightMouseButtonDown) {
        // Calculate change in mouse position
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        // Update camera angles
        cameraAzimuth -= deltaX * 0.5f;    // Adjust horizontal rotation (azimuth)
        cameraElevation += deltaY * 0.5f; // Adjust vertical rotation (elevation)

        // Clamp elevation to avoid flipping the view
        if (cameraElevation > 89.0f) cameraElevation = 89.0f;
        if (cameraElevation < -89.0f) cameraElevation = -89.0f;

        // Update last mouse position
        lastMouseX = x;
        lastMouseY = y;

        // Redraw the scene
        glutPostRedisplay();
    }
}

float wireFrameInput(){
    float cameraX, cameraY, cameraZ;
    cout << "Height: " << endl;
    cin >> cameraY;
    cout << "Width: " << endl;
    cin >> cameraX;
    cout << "Depth: " << endl;
    cin >> cameraZ;
    return cameraX, cameraY, cameraZ;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1': // Zoom in
        cameraDistance -= 0.5f;
        if (cameraDistance < 1.0f) cameraDistance = 1.0f;
        glutPostRedisplay();
        break;
    case '2': // Zoom out
        cameraDistance += 0.5f;
        if (cameraDistance > 50.0f) cameraDistance = 50.0f;
        glutPostRedisplay();
        break;
    case 27: // Escape key to exit
        exit(0);
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera(); // Update the camera view

    glColor3f(1.0, 1.0, 1.0); // Set cube color to white

    drawWirePrism(2,3,4);      // Draw wireframe cube

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Onshaper");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);    // Register mouse motion callback

    glutMainLoop();
    return 0;
}
