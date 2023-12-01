
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Display list for leg
GLuint legList;

// Rotation angles for x, y, z axes
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;

// Flag to toggle between wireframe and solid mode
bool isWireframeMode = false;


void init(void) {
    // Create display list for leg
    legList = glGenLists(1);
    glNewList(legList, GL_COMPILE);

// Define the number of sides for the cylinder
    int sides = 20;

// Define the radius and height of the cylinder
    float radius = 0.2f;
    float height = 0.7f;

// Draw the bottom circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        float angle = i * 2.0f * M_PI / sides;
        glVertex3f(radius*cosf(angle), 0.0f, radius*sinf(angle));
    }
    glEnd();

// Draw the top circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; ++i) {
        float angle = i * 2.0f * M_PI / sides;
        glVertex3f(radius*cosf(angle), height, radius*sinf(angle));
    }
    glEnd();

// Draw the sides of the cylinder
    for (int i = 0; i < sides; ++i) {
        float angle1 = i * 2.0f * M_PI / sides;
        float angle2 = (i+1) * 2.0f * M_PI / sides;

        glBegin(GL_POLYGON);
        glVertex3f(radius*cosf(angle1), 0.0f, radius*sinf(angle1));
        glVertex3f(radius*cosf(angle2), 0.0f, radius*sinf(angle2));
        glVertex3f(radius*cosf(angle2), height, radius*sinf(angle2));
        glVertex3f(radius*cosf(angle1), height, radius*sinf(angle1));
        glEnd();
    }

    glEndList();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

}
void display(void) {
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply rotation transformations based on user input
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

    glPushMatrix();
    glScaled(2.5, 0.4, 2.5);
    glColor3f(1.0f, 0.5f, 0.2f);
    glCallList(legList); // Execute display list.
    glPopMatrix();



//     Draw three legs using display list and appropriate rotations
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glColor3f(0.9f, 0.9f, 1.0f);

        switch (i) {
            case 0:
                glRotatef(20, 1.0f, 0.0f, 0.0f);

                glScaled(0.4, 1.2, 0.4);
                glTranslated(0.0, -0.65, -0.2);

                glCallList(legList);

                break;
            case 1:
                glRotatef(25, 0.0f, 0.0f, 1.0f);
                glRotatef(-10, 1.0f, 0.0f, 0.0f);

                glScaled(0.4, 1.2, 0.4);
                glTranslated(0.4, -0.7, 0.2);
                glCallList(legList);

                break;
            case 2:
                glRotatef(-25, 0.0f, 0.0f, 1.0f);
                glRotatef(-10, 1.0f, 0.0f, 0.0f);

                glScaled(0.4, 1.2, 0.4);
                glTranslated(-0.4, -0.7, 0.2);
                glCallList(legList);
                break;
        }
        glPopMatrix();
    }


// Swap buffers to display rendered image
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
        case 'X':
            angleX = 5.0f;
            angleY = 0.0f;
            angleZ = 0.0f;
            break;
        case 'y':
        case 'Y':
            angleY = 5.0f;
            angleX = 0.0f;
            angleZ = 0.0f;
            break;
        case 'z':
        case 'Z':
            angleZ = 5.0f;
            angleX = 0.0f;
            angleY = 0.0f;
            break;
        case ' ':
            isWireframeMode = !isWireframeMode;
            if (isWireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            angleY = 0.0f;
            angleX = 0.0f;
            angleZ = 0.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
// Initialize GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 700);
    glutCreateWindow("3-Legged Stool");

// Initialize scene and set up callback functions
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

// Start main loop
    glutMainLoop();
    return 0;
}

