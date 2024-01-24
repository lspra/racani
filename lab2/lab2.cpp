#include <stdlib.h>
#include<ctime>
#include "particle.hpp"
#define MAXP 20000

GLuint tex2d;

Ociste	ociste = { 0.4f, 0.0f, 1.8f }, glediste = {0.0f, 0.0f, 0.0f};
int w, h;
glm::vec3 wind_dir;
Particle p[MAXP];
int intensity = MAX_INTENSITY;
int time_ = 0;

void renderScene()
{
    for(int i = 0; i <MAXP; i++) {
        if(p[i].lifespan > p[i].age)
            p[i].draw(ociste);
        else
            p[i] = Particle(intensity);
    }
}

void animate(int value)
{
    time_ += 20;
    if(time_ > 3000) {
        time_ = 0;
        intensity--;
    }
    for(int i = 0; i <MAXP; i++) {
        p[i].process(wind_dir);
        //p[i].process(wind_dir);
    }
    glutPostRedisplay();
    glutTimerFunc(20, animate, 0);
}
void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderScene();
    glutSwapBuffers();
}
void reshape(int width, int height)
{
    w = width; h = height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, 1, 5);
    //glFrustum(-1, 1, -1, 1, 1, 5);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluPerspective(60, 1, 2, 10);
    gluLookAt(ociste.x, ociste.y, ociste.z, glediste.x, glediste.y, glediste.z, 0.0, 1.0, 0.0);	// ociste x,y,z; glediste x,y,z; up vektor x,y,z
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    float len = (ociste-glediste).length();
    if(theKey == 'a' && ociste.x < 0)
        theKey = 'd';
    else if(theKey == 'd' && ociste.x > 0)
        theKey = 'a';
    float deltax = 0.0f, deltaz = 0.0f;
    switch (theKey)
    {
    case 'w':
        deltax = 0.2f * (glediste.x - ociste.x) / len;
        deltaz = 0.2f * (glediste.z - ociste.z) / len;
        break;
    case 'a':
        deltaz = 0.2f * (ociste.z - glediste.z) / (ociste.x - glediste.x);
        deltax = -0.2f;
        break;
    case 's':
        deltax = -0.2f * (glediste.x - ociste.x) / len;
        deltaz = -0.2f * (glediste.z - ociste.z) / len;
        break;
    case 'd':
        deltaz = -0.2f * (ociste.z - glediste.z) / (ociste.x - glediste.x);
        deltax = 0.2f;
        break;
    case 'r': 
        ociste = { 0.4f, 0.0f, 1.8f }, glediste = {0.0f, 0.0f, 0.0f};
        break;
    case '+':
        intensity++;
        if(intensity > 20) intensity = 20;
        break;
    case '-':
        intensity--;
        if(intensity < 0) intensity = 0;
        break;
    case 27:  exit(0);
        break;
    }
    ociste.x += deltax;
    ociste.z += deltaz;
    glediste.x += deltax;
    glediste.z += deltaz;
    printf("%lf, %lf, %lf\n", ociste.x, ociste.y, ociste.z);
    reshape(w, h);
    glutPostRedisplay();
}

void mouseCallback(int x, int y, int a, int b) {
    printf("%d, %d, %d, %d\n", x, y, a, b);
}
//27
void motionCallback(int x, int y) {
    wind_dir.x = ((float)x - w/2) / 2000000.f;
    wind_dir.y = -((float)y - h/2) / 9000000.f;
}

int main(int argc, char ** argv)
{
    float bmin[3], bmax[3];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(" Primjer animacije ");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(15, animate, 0);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);

    glutPassiveMotionFunc(motionCallback);
    //tex2d = SOIL_load_OGL_texture
    //(
    //    "iskrica.tga",
    //    SOIL_LOAD_AUTO,
    //    SOIL_CREATE_NEW_ID,
    //    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    //);
    //glBlendEquation(GL_FUNC_ADD);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
}
