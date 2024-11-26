#include <GL/glut.h>
#include <iostream>
#include <glm/glm.hpp>

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

void initializeGL() {

    // Print OpenGL version and renderer information
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << glm::acos(3) << std::endl;
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // Double buffer and RGB color mode
    glutInitWindowSize(800, 600);  // Window size
    glutCreateWindow("GLUT & GLEW Test");  // Create the window

    initializeGL();  // Initialize GLEW and print OpenGL info

    // Register the display function
    glutDisplayFunc(display);

    // Start the main loop
    glutMainLoop();

    return 0;
}
