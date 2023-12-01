#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <ostream>
#include <vector>
#include <cmath>

using namespace std;
int user_choice = 0;
// DDA algorithm for drawing lines
void dda(float x1, float y1, float x2, float y2, std::vector<float> &points) {
    // Calculate the change in x and y
    float dx = x2 - x1;
    float dy = y2 - y1;
    // Calculate the number of steps
    int steps = std::max(abs(dx), abs(dy));
    // Calculate the increments for x and y
    float x_inc = dx / steps;
    float y_inc = dy / steps;
    // Draw the line
    for (int i = 0; i < steps; i++) {
        points.push_back(x1);
        points.push_back(y1);
        x1 += x_inc;
        y1 += y_inc;
    }
}

// Bresenham algorithm for drawing lines
void bresenham(float x1, float y1, float x2, float y2, std::vector<float> &points) {
    // Calculate the change in x and y
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    // Determine the direction of the x and y axis
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    // Calculate the error term
    int err = dx - dy;
    // Draw the line
    while (x1 != x2 || y1 != y2) {
        points.push_back(x1);
        points.push_back(y1);
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1.0f, 0.5f, 0.2f);
    glPointSize(10.0);

    // Define the string "Ahmed" as a vector of lines
    std::vector<std::pair<float, float>> lines = {
            //A
            {100.0f,  150.0f},
            {125.0f, 250.0f},
            {125.0f, 250.0f},
            {150.0f, 150.0f},
            {135.0f, 200.0f},
            {115.0f,  200.0f},
            //H
            {175.0f, 250.0f},
            {175.0f, 150.0f},
            {225.0f, 250.0f},
            {225.0f, 150.0f},
            {175.0f, 200.0f},
            {225.0f, 200.0f},
            //M
            {250.0f,  150.0f},
            {250.0f, 250.0f},
            {250.0f, 250.0f},
            {275.0f, 150.0f},
            {275.0f, 150.0f},
            {300.0f,  250.0f},
            {300.0f,  250.0f},
            {300.0f,  150.0f},
            //E
            {325.0f,  150.0f},
            {325.0f, 250.0f},
            {325.0f, 250.0f},
            {375.0f, 250.0f},
            {325.0f, 200.0f},
            {375.0f,  200.0f},
            {325.0f,  150.0f},
            {375.0f,  150.0f},
            //D
            {400.0f,  150.0f},
            {400.0f, 250.0f},
            {400.0f, 250.0f},
            {450.0f, 225.0f},
            {450.0f, 225.0f},
            {450.0f, 175.0f},
            {450.0f,  175.0f},
            {400.0f,  150.0f},


    };
    // Initialize the points vector
    std::vector<float> points;
    // Iterate over the lines and add their points to the points vector
    for (int i = 0; i < lines.size() - 1; i = i + 2) {
        // Use DDA algorithm for the first 4 lines
        if (user_choice == 1) {
            dda(lines[i].first, lines[i].second, lines[i + 1].first, lines[i + 1].second, points);
        }
        // Use Bresenham algorithm for the last 6 lines
        else{
            bresenham(lines[i].first, lines[i].second, lines[i + 1].first, lines[i + 1].second, points);
        }
    }


    for (int j = 0; j < points.size() - 1; j = j + 2) {
        glBegin(GL_POINTS);
        glVertex3f(points[j], points[j + 1], 0.0f);
        glEnd();
    }

    // Create a vertex array object (VAO)
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Create a vertex buffer object (VBO)
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
// Copy the points into the VBO
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
// Specify the vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

// Swap buffers
    glutSwapBuffers();
    glBindVertexArray(0);
    glFlush();

}

void setup(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

// Reshape function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 800, 0.0, 700, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
// Initialize GLUT and GLEW
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    std::cout << "Which line type do you want\n1) DDA\n2) Bresenham\n>> ";
    std::cin >> user_choice;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("name.cpp");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glewExperimental = GL_TRUE;
    glewInit();
    setup();
    glutMainLoop();
}

