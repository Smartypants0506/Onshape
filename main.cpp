#include <GL/glut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <windows.h>
#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)


void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    // Simple drawing of a colored triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);  // Green
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();
}



int main(int argc, char** argv){
    std::cout << SCREEN_WIDTH << " x " << SCREEN_HEIGHT << std::endl;
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // Double buffer and RGB color mode
    glutInitWindowSize(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);  // Window size
    glutCreateWindow("Onshaper");  // Create the window

    // Register the display function
    glutDisplayFunc(display);

    // Start the main loop
    glutMainLoop();

    return 0;
}
