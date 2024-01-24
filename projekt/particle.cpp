#include "particle.hpp"
float sigm3(float x) {
    return 1/(1 + exp(-x))/(1 + exp(-x))/(1 + exp(-x));
}

Particle::Particle()
{
    float r = ((float) rand() / (RAND_MAX)) * 0.4;
    float fi = 2.*3.14* ((float) rand() / (RAND_MAX));
    pos = {r * cosf(fi), 0, r * sinf(fi)};
    speed = {0, 0.008, 0};
    lifespan = 450 / sigm3(r/0.5) * ((float) rand() / (RAND_MAX) / 2 + 0.5);
    age = 0;
    if(r/0.475 > 0.7) {
        speed.x = -0.001 * pos.x ; //+ 0.01;
        speed.z = -0.001 * pos.z;
        color = {1, 0, 0, 0.45};
    }
    else if(r/0.4 > 0.33) {
        speed.x = -0.0005 * pos.x; // + 0.01;
        speed.z = -0.0005 * pos.z;
        color = {1, 0.5, 0, 0.45};
    }
}

Particle::Particle(int intensity)
{
    float r = ((float) rand() / (RAND_MAX)) * 0.4 * ((float) intensity / MAX_INTENSITY);
    float fi = 2.*3.14* ((float) rand() / (RAND_MAX));
    pos = {r * cosf(fi), 0, r * sinf(fi)};
    speed = {0, 0.008, 0};
    lifespan = 450 / sigm3(r/0.5) * ((float) rand() / (RAND_MAX) / 2 + 0.75f);
    age = 0;
    if(r/0.475 > 0.7 * ((float) intensity / MAX_INTENSITY)) {
        speed.x = -0.001 * pos.x ; //+ 0.01;
        speed.z = -0.001 * pos.z;
        color = {1, 0, 0, 0.45};
    }
    else if(r/0.4 > 0.33 * ((float) intensity / MAX_INTENSITY)) {
        speed.x = -0.0008 * pos.x; // + 0.01;
        speed.z = -0.0008 * pos.z;
        color = {1, 0.5, 0, 0.45};
    }
    lifespan *= ((float) intensity / MAX_INTENSITY);
}

Particle::~Particle()
{
}

void Particle::process(vert3d wind_dir) {
    speed += wind_dir;
    auto r = [](){ return 0.01 * (2*(float) rand() / (RAND_MAX) - 1); };
    pos += speed + vert3d{r(), 0.f, r()};
    age += 15;
    if(age > 0.45 * lifespan) {
        speed.x += - 0.0001 * pos.x; // + 0.01;
        speed.z += - 0.0001 * pos.z;
        speed.y *= 1.001;
    }
    speed *= 0.97f;
    if(age > 0.78 * lifespan) {
        color = {0.1, 0.11, 0.1, 0.5};
        //speed = vert3d{0, 0.01, 0} + vert3d{3*r(), 0.f, 3*r()} + wind_dir;
    }
    else if(age * 3 > 2 * lifespan)
        color = {1, 0, 0, 1};
    else if(age * 3 > lifespan && color.g == 1)
        color = {1, 0.5, 0, 1};
}

void Particle::draw(Ociste o) {
    glPointSize(1.f);
    vert3d normala = {o.x - pos.x, o.y - pos.y, o.z - pos.z};
    glPushMatrix();
    GLfloat angle = acosf(normala.z / sqrt(normala.x * normala.x  + normala.y * normala.y  + normala.z * normala.z)) * 180.f / 2.f / 3.14159f;
    glRotatef(angle, -normala.y, normala.x, 0);
    glColor4f(color.r, color.g, color.b, color.a);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    //glTexCoord2d(1, 1);
    float size = 0.007f;
    glVertex3f(pos.x-size, pos.y-size, pos.z+0.0f);
    //glTexCoord2d(1, 0);
    glVertex3f(pos.x+size, pos.y-size, pos.z+0.0f);
    //glTexCoord2d(0, 0);
    glVertex3f(pos.x+size, pos.y+size, pos.z+0.0f);
    //glTexCoord2d(0, 1);
    glVertex3f(pos.x-size, pos.y+size, pos.z+0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
}