#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be
// filled with an asteroid. It should be an integer between 0 and 100.

// Globals.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 180.0; // Angle of the spacecraft.
static float xVal = 0, zVal = -50; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.
static int frameCount = 0; // Number of frames
static float relativeSpeed = 0.2;

static float d = 1.0; // Diffuse and specular white light intensity.
static float m = 0.2; // Global ambient white light intensity.
static int localViewer = 1; // Local viewpoint?
static float p = 1.0; // Positional light?
static float t = 0.0; // Quadratic attenuation factor.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, const char *string)
{
    const char *c;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
float radians(float degrees)
{
    return degrees * (M_PI / 180);

}

// Asteroid class.
class Asteroid
{
public:
    Asteroid();

    Asteroid(float colorR, float colorG, float colorB);

    float getCenterX() { return centerX; }
    float getCenterY() { return centerY; }
    float getCenterZ() { return centerZ; }
    float getRadius() { return radius; }
    float getDistance() { return distance; }
    float getAngleOrbit() { return angleOrbit; }
    float getAngleRotation() { return angleRotation; }
    float getVelocity() { return velocity; }
    void draw(bool sun);

    void setCenterX(float centerX);

    void setCenterY(float centerY);

    void setCenterZ(float centerZ);

    void setRadius(float radius);

    void setDistance(float distance);

    void setAngleOrbit(float angleOrbit);

    void setAngleRotation(float angleRotation);

    void setVelocity(float velocity);

private:
    float centerX, centerY, centerZ, radius, distance, angleOrbit, angleRotation, velocity;
    float color[3]{};

    float * getColor();
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;
    radius = 0.0; // Indicates no asteroid exists in the position.
    distance = 0.0;
    angleOrbit = 0.0;
    angleRotation = 0.0;
    velocity = 0.0;
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float colorR,
                   float colorG, float colorB)
{
    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;
    radius = 0.0; // Indicates no asteroid exists in the position.
    distance = 0.0;
    angleOrbit = 0.0;
    angleRotation = 0.0;
    velocity = 0.0;
    color[0] = colorR;
    color[1] = colorG;
    color[2] = colorB;
}



// Function to draw asteroid.
void Asteroid::draw(bool sun)
{
    if(!sun){
        if (radius > 0.0) // If asteroid exists.
        {
            float matAmbAndDif[] = { (color[0]) / 256, (color[1]) / 256, (color[2]) / 256, 1.0 };
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);

            glPushMatrix();
            glTranslatef(centerX, centerY, centerZ);
            glutSolidSphere(radius, (int)radius * 6, (int)radius * 6);
            glPopMatrix();


        }
    }else{
        glDisable(GL_LIGHTING);

        glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        glColor3f(color[0]/256, color[1]/256, color[2]/256);
        glutSolidSphere(radius, (int)radius * 6, (int)radius * 6);
        glPopMatrix();

        glEnable(GL_LIGHTING);

    }

}



void Asteroid::setCenterX(float x) {
    Asteroid::centerX = x;
}

void Asteroid::setCenterY(float y) {
    Asteroid::centerY = y;
}

void Asteroid::setCenterZ(float z) {
    Asteroid::centerZ = z;
}

void Asteroid::setRadius(float r) {
    Asteroid::radius = r;
}

void Asteroid::setDistance(float d) {
    Asteroid::distance = d;
}

void Asteroid::setAngleOrbit(float angleO) {
    Asteroid::angleOrbit = angleO;
}

void Asteroid::setAngleRotation(float angleR) {
    Asteroid::angleRotation = angleR;
}

void Asteroid::setVelocity(float v) {
    Asteroid::velocity = v;
}



Asteroid arrayAsteroids[10]; // Global array of asteroids.

// Routine to count the number of frames drawn every second.
void frameCounter(int value)
{
    if (value != 0) // No output the first time frameCounter() is called (from main()).
        std::cout << "FPS = " << frameCount << std::endl;
    frameCount = 0;
    glutTimerFunc(1000, frameCounter, 1);
}

// Initialization routine.
void setup(void)
{

    spacecraft = glGenLists(1);
    glNewList(spacecraft, GL_COMPILE);
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, -20.0); // To make the spacecraft point down the $z$-axis initially.
    glColor3f(1.0, 1.0, 1.0);
    glutWireCone(3.0, 2.0, 10, 3);
    glPopMatrix();
    glEndList();

    Asteroid sun = Asteroid(255.0, 212.0, 27.0);
    sun.setVelocity(0) ;
    sun.setAngleOrbit(0);
    sun.setAngleRotation(0);
    sun.setDistance(0);
    sun.setRadius(4.5);
    sun.setCenterX(0);
    sun.setCenterY(0);
    sun.setCenterZ(0);

    arrayAsteroids[0] = sun;

    Asteroid mercury = Asteroid(148.0, 147.0, 141.0);
    mercury.setVelocity(3.3 * relativeSpeed) ;
    mercury.setAngleOrbit(0);
    mercury.setAngleRotation(0);
    mercury.setDistance(10);
    mercury.setRadius(2);
    mercury.setCenterX(mercury.getDistance()  * std::cos(radians(mercury.getAngleOrbit())));
    mercury.setCenterY(0);
    mercury.setCenterZ(mercury.getDistance()  * std::sin(radians(mercury.getAngleOrbit())));

    arrayAsteroids[1] = mercury;

    Asteroid venus = Asteroid(179.0, 151.0, 7.0);
    venus.setVelocity(2.9 * relativeSpeed) ;
    venus.setAngleOrbit(0);
    venus.setAngleRotation(0);
    venus.setDistance(15);
    venus.setRadius(2.2);
    venus.setCenterX(venus.getDistance() * std::cos(radians(venus.getAngleOrbit())));
    venus.setCenterY(0);
    venus.setCenterZ(venus.getDistance() * std::sin(radians(venus.getAngleOrbit())));

    arrayAsteroids[2] = venus;

    Asteroid earth = Asteroid(19.0, 125.0, 9.0);
    earth.setVelocity(2.6 * relativeSpeed) ;
    earth.setAngleOrbit(0);
    earth.setAngleRotation(0);
    earth.setDistance(20);
    earth.setRadius(2.3);
    earth.setCenterX(earth.getDistance() * std::cos(radians(earth.getAngleOrbit())));
    earth.setCenterY(0);
    earth.setCenterZ(earth.getDistance() * std::sin(radians(earth.getAngleOrbit())));

    arrayAsteroids[3] = earth;

    Asteroid mars = Asteroid(204.0, 13.0, 13.0);
    mars.setVelocity(2.2 * relativeSpeed) ;
    mars.setAngleOrbit(0);
    mars.setAngleRotation(0);
    mars.setDistance(25);
    mars.setRadius(2.3);
    mars.setCenterX(mars.getDistance() * std::cos(radians(mars.getAngleOrbit())));
    mars.setCenterY(0);
    mars.setCenterZ(mars.getDistance() * std::sin(radians(mars.getAngleOrbit())));

    arrayAsteroids[4] = mars;

    Asteroid jupiter = Asteroid(240.0, 98.0, 23.0);
    jupiter.setVelocity(1.9 * relativeSpeed) ;
    jupiter.setAngleOrbit(0);
    jupiter.setAngleRotation(0);
    jupiter.setDistance(30);
    jupiter.setRadius(2.6);
    jupiter.setCenterX(jupiter.getDistance() * std::cos(radians(jupiter.getAngleOrbit())));
    jupiter.setCenterY(0);
    jupiter.setCenterZ(jupiter.getDistance() * std::sin(radians(jupiter.getAngleOrbit())));

    arrayAsteroids[5] = jupiter;

    Asteroid saturn = Asteroid(196.0, 132.0, 14.0);
    saturn.setVelocity(1.6 * relativeSpeed) ;
    saturn.setAngleOrbit(0);
    saturn.setAngleRotation(0);
    saturn.setDistance(37);
    saturn.setRadius(2.65);
    saturn.setCenterX(saturn.getDistance() * std::cos(radians(saturn.getAngleOrbit())));
    saturn.setCenterY(0);
    saturn.setCenterZ(saturn.getDistance() * std::sin(radians(saturn.getAngleOrbit())));

    arrayAsteroids[6] = saturn;

    Asteroid uranus = Asteroid(90.0, 166.0, 186.0);
    uranus.setVelocity(1.5 * relativeSpeed) ;
    uranus.setAngleOrbit(0);
    uranus.setAngleRotation(0);
    uranus.setDistance(43);
    uranus.setRadius(2.2);
    uranus.setCenterX(uranus.getDistance() * std::cos(radians(uranus.getAngleOrbit())));
    uranus.setCenterY(0);
    uranus.setCenterZ(uranus.getDistance() * std::sin(radians(uranus.getAngleOrbit())));

    arrayAsteroids[7] = uranus;

    Asteroid neptune = Asteroid(13.0, 136.0, 168.0);
    neptune.setVelocity(1 * relativeSpeed) ;
    neptune.setAngleOrbit(0);
    neptune.setAngleRotation(0);
    neptune.setDistance(48);
    neptune.setRadius(2.3);
    neptune.setCenterX(neptune.getDistance() * std::cos(radians(neptune.getAngleOrbit())));
    neptune.setCenterY(0);
    neptune.setCenterZ(neptune.getDistance() * std::sin(radians(neptune.getAngleOrbit())));

    arrayAsteroids[8] = neptune;

    Asteroid earthMoon = Asteroid(170.0, 170.0, 170.0);
    earthMoon.setVelocity(100 * relativeSpeed) ;
    earthMoon.setAngleOrbit(0);
    earthMoon.setAngleRotation(0);
    earthMoon.setDistance(5);
    earthMoon.setRadius(1);
    earthMoon.setCenterX(earth.getCenterX() + (earthMoon.getDistance() * std::cos(radians(earthMoon.getAngleOrbit()))));
    earthMoon.setCenterY(0);
    earthMoon.setCenterZ(earth.getCenterZ() + (earthMoon.getDistance() * std::sin(radians(earthMoon.getAngleOrbit()))));

    arrayAsteroids[9] = earthMoon;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);

    // Material property vectors.
    float matAmbAndDif[] = { 0.0, 0.0, 1.0, 1.0 };
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };
    float matShine[] = { 50.0 };

    // Material properties of ball.
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    // Cull back faces.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glutTimerFunc(0, frameCounter, 0); // Initial call of frameCounter().
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
                             float x2, float y2, float z2, float r2)
{
    return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
    int i;

    // Check for collision with each asteroid.
        for (i = 0; i<9; i++)
            if (arrayAsteroids[i].getRadius() > 0) // If asteroid exists.
                if (checkSpheresIntersection(x - 5 * sin((M_PI / 180.0) * a), 0.0,
                                             z - 5 * cos((M_PI / 180.0) * a), 7.072,
                                             arrayAsteroids[i].getCenterX(), arrayAsteroids[i].getCenterY(),
                                             arrayAsteroids[i].getCenterZ(), arrayAsteroids[i].getRadius()))
                    return 1;
    return 0;
}

// Drawing routine.
void drawScene(void)
{
    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec0[] = { d, d, d, 1.0 };
    float lightPos0[] = { 0.0, 0.0, 1.0, p }; //Demo: last value p define whether it is direction or positional
    float globAmb[] = { m, m, m, 1.0 };

    // Light0 properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer); // Enable local viewpoint
    glEnable(GL_LIGHT0);


    frameCount++; // Increment number of frames every redraw.

    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Beg	in left viewport.
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, width, height);

    glLoadIdentity();
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, t);

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
    glPopMatrix();

    // Fixed camera.

    gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle),
              0.0,
              zVal - 10 * cos((M_PI / 180.0) * angle),
              xVal - 11 * sin((M_PI / 180.0) * angle),
              0.0,
              zVal - 11 * cos((M_PI / 180.0) * angle),
              0.0,
              1.0,
              0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // Draw all the asteroids in arrayAsteroids.
    for (i = 0; i<9; i++){
        arrayAsteroids[i].setAngleOrbit(arrayAsteroids[i].getAngleOrbit() + arrayAsteroids[i].getVelocity());
        arrayAsteroids[i].setCenterX(arrayAsteroids[i].getDistance() * std::cos(radians(arrayAsteroids[i].getAngleOrbit())));
        arrayAsteroids[i].setCenterY(0);
        arrayAsteroids[i].setCenterZ(arrayAsteroids[i].getDistance() * std::sin(radians(arrayAsteroids[i].getAngleOrbit())));
        arrayAsteroids[i].draw(i == 0);
    }
    // Moon
    arrayAsteroids[9].setAngleOrbit(arrayAsteroids[9].getAngleOrbit() + arrayAsteroids[9].getVelocity());
    arrayAsteroids[9].setCenterX(arrayAsteroids[3].getCenterX() + (arrayAsteroids[9].getDistance() * std::cos(radians(arrayAsteroids[9].getAngleOrbit()))));
    arrayAsteroids[9].setCenterY(0);
    arrayAsteroids[9].setCenterZ(arrayAsteroids[3].getCenterZ() + (arrayAsteroids[9].getDistance() * std::sin(radians(arrayAsteroids[9].getAngleOrbit()))));
    arrayAsteroids[9].draw(false);
    // End SpaceCraft viewport.

    //-------------------------------------------------------------------------------------------------------------------------------------------------//

    // Begin Solar System viewport.
    glEnable(GL_SCISSOR_TEST);
    glScissor(((3 * width) / 4), 0, width / 4, height / 4);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glViewport( ((3 * width) / 4), 0, width / 4, height / 4);

    glLoadIdentity();
    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
    glPopMatrix();

    // Draw a vertical line on the left of the viewport to separate the two viewports
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-5.0, -5.0, -5.0);
    glVertex3f(-5.0, 5.0, -5.0);
    glEnd();
    glLineWidth(1.0);

    // Locate the camera at the tip of the cone and pointing in the direction of the cone.
    gluLookAt(10.0, 50.0, -50.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // Draw all the asteroids in arrayAsteroids.
    for (i = 0; i<10; i++)
        arrayAsteroids[i].draw(i == 0);

    // Draw spacecraft.
    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glCallList(spacecraft);
    glPopMatrix();

    // End Solar System viewport.

    glutSwapBuffers();
    glutPostRedisplay();

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
    glMatrixMode(GL_MODELVIEW);

    // Pass the size of the OpenGL window.
    width = w;
    height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

    // Compute next position.
    if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
    if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
    if (key == GLUT_KEY_UP)
    {
        tempxVal = xVal - sin(angle * M_PI / 180.0);
        tempzVal = zVal - cos(angle * M_PI / 180.0);
    }
    if (key == GLUT_KEY_DOWN)
    {
        tempxVal = xVal + sin(angle * M_PI / 180.0);
        tempzVal = zVal + cos(angle * M_PI / 180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    if (tempAngle < 0.0) tempAngle += 360.0;

    // Move spacecraft to next position only if there will not be collision with an asteroid.
    if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle))
    {
        isCollision = 0;
        xVal = tempxVal;
        zVal = tempzVal;
        angle = tempAngle;
    }
    else isCollision = 1;

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press the left/right arrow keys to turn the craft." << std::endl
              << "Press the up/down arrow keys to move the craft." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("spaceTravel.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
