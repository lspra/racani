#include <stdlib.h>
#include <GL/glew.h>
#include <ctime>
#include <fstream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#define MAXP 20000

GLuint tex2d;

glm::vec3 ociste = { 0.4f, 0.0f, 1.8f }, glediste = {0.0f, 0.0f, 0.0f};
float w, h;
GLint utime, wind;
float windIntensity;

GLuint loadShader(GLenum type)
{
    std::ifstream reader;
    if(type == GL_FRAGMENT_SHADER)
        reader.open("fragmentShader.glsl");
    else
        reader.open("vertexShader.glsl");
    std::string source((std::istreambuf_iterator<char>(reader)),
                       std::istreambuf_iterator<char>());
    const char* shaderSource = source.c_str();
    
    GLuint shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &shaderSource, 0);
    glCompileShader(shaderId);
    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	    std::vector<GLchar> infoLog(maxLength);
	    glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);
        for(int i = 0; i < infoLog.size(); i++)
            printf("%c", infoLog[i]);
        glDeleteShader(shaderId); // Don't leak the shader.
        exit(1);
    }
    
    return shaderId;
}

void renderScene()
{
    static float vtime = 0.0f;
    vtime += 0.16f;
    glUniform1f(wind, windIntensity);
    glUniform1f(utime, vtime);
    auto r = [](){ return (float) rand() / (RAND_MAX); };
    static float r1, r2, r3, r4;
    if(sin(vtime) < 0.0f && sin(vtime) > -0.2f) {
        r1 = r(); r2 = r(); r3 = r(); r4 = r();
    }
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, r1);
    glVertex3f(w, 0.0f, r2);
    glVertex3f(0.0f, h, r3);
    glVertex3f(w, h, r4);
    glEnd();
}

void animate(int value)
{
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
    switch (theKey)
    {
    case 27:  exit(0);
        break;
    }
}

void motionCallback(int x, int y) {
    windIntensity = ((float)x - w/2) / 200.f;
    printf("%f\n", windIntensity);
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
    glutPassiveMotionFunc(motionCallback);
    glewInit();
    GLuint vertex = loadShader(GL_VERTEX_SHADER);
    GLuint fragment = loadShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Link our program
    glLinkProgram(program);
    glUseProgram(program);

    wind = glGetUniformLocation(program, "wind");    
    utime = glGetUniformLocation(program, "time");

    glutMainLoop();
}
