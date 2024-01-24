#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
//#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#define MAX_INTENSITY 20

typedef glm::vec3 Ociste;
typedef glm::vec3 vert3d;

typedef glm::vec4 rgba;
class Particle
{
private:
public:
    vert3d pos, speed;
    int lifespan, age;
    rgba color = {1, 1, 0, 0.45};
    Particle();
    Particle(int intensity);
    ~Particle();
    void draw(Ociste o);
    void process(vert3d wind_dir);
};

