#version 450 core
out vec4 FragColor;
in vec4 pos;
uniform float time;
uniform float wind;

float circle(vec2 p, vec2 center) {
    return (abs(p.x) - center.x)*(abs(p.x) - center.x) + (p.y - center.y)*(p.y - center.y);
}

float sech(vec2 p) {
    return 2 * exp(abs(p.x * 3)) / (exp(2*abs(p.x * 3))+1);
}

float shape_circ(vec2 p) {
    float c = circle(p, vec2(0.6, 0.6));
    float sin1 = 0.014 * sin(127 * p.x + 4*time);
    float sin2 = 0.007 * sin(211 * p.x + 100 + 3*time);
    float sin3 = 0.003 * sin(331 * p.x + 15 + 7*time);
    float sin4 = 0.004 * sin(383 * p.x + 70 + 5*time);
    return c + sin1 - sin2 + sin3 -sin4 - 0.36f;
}
float shape_bell(vec2 p) {
    // float c = circle(p, vec2(0.6, 0.6));
    float c = sech(p);
    float sin1 = 0.02 * sin(127 * p.x + 4*time);
    float sin2 = 0.013 * sin(211 * p.x + 100 + 3*time);
    float sin3 = 0.009 * sin(331 * p.x + 15 + 7*time);
    float sin4 = 0.005 * sin(383 * p.x + 70 + 5*time);
    float sin5 = 0.021 * sin(113 * p.y + 4*time);
    return c + sin1 - sin2 + sin3 -sin4 + sin5 - 0.55 -p.y*1.5;
}
float spark_shape(vec2 p) {
    vec2 trans_p;   
    trans_p.x = p.x + 0.01 * abs(sin(127 * p.x + time)) + 0.008 * abs(sin(211 * p.x + 100 + 2*time));
    trans_p.y = p.y + 0.01 * abs(sin(131 * p.x + time)) + 0.008 * abs(sin(199 * p.x + 100 + 2*time));
    trans_p.x *= 2;
    float cx = 0.1f + 0.5f * pos.z;
    float cy = 0.1f * pos.z + 0.45 - cx;
    if(p.x < 0.0f)
        cx += 0.09f;
    return circle(trans_p, vec2(cx, cy)) - 0.0007f;
}

float smoke(vec2 p, float a, float b) {
    if(b > 0)
        b = 0.1 + 0.5 * b;
    float sin1 = 0.1 * sin(127 * p.y + 4*time);
    float sin2 = 0.07 * sin(211 * p.x + 100 + 3*time);
    float sin3 = 0.04 * sin(331 * p.y + 15 + 7*time);
    float sin4 = 0.01 * sin(383 * p.x + 70 + 5*time);
    return p.x*p.x/a - p.y*p.y/b -1 + sin1 + sin2 + sin3 + sin4;

}

void addSpark(vec2 p) {
    if(abs(p.x) > 0.0f && p.y > 0.0f) {
        if(spark_shape(p) < -0.0006f)
            FragColor = vec4(1.0f, 1, 0.0f, 1.0f) * sin(time);
        else if(spark_shape(p) < -0.0004f)
            FragColor = vec4(1.0f, 0.6, 0.0f, 1.0f)* sin(time);
        else if(spark_shape(p) < -0.0002f)
            FragColor = vec4(1.0f, 0.3, 0.0f, 1.0f)* sin(time);
        else if(spark_shape(p) < 0.0f)
            FragColor = vec4(1.0f, 0, 0.0f, 1.0f) * sin(time);
    }
}

float quadrat(vec2 p) {
    float q = p.y + p.x*p.x - 0.8 - 0.2 * sin(0.35*time +120);
    float sin3 = 0.01 * sin(73 * p.x + 23 + 3*time);
    float sin4 = 0.006 * sin(181 * p.x + 69 + 4*time);
    float sin5 = 0.01 * sin(79 * p.y + 4*time);
    float sin6 = 0.006 * sin(179 * p.y + 4*time);
    return q + sin3 + sin5 + sin4 - sin6;
}

void main()
{
    vec2 pos = pos.xy;
    if(wind != 0)
        pos = vec2(pos.x - 0.12 * pos.y * wind, pos.y);
    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float s = smoke(pos.xy + vec2(0.0, -0.15), 0.02, 0.2 * abs(sin(0.2*time)));
    if(pos.y > 0.0f && s < 0 && quadrat(pos.xy) < 0) {
        FragColor = vec4(0.1 + 0.1/s, 0.11+ 0.1/s, 0.1+ 0.1/s, 0.5);
    if(pos.y > 0.0f && smoke(pos.xy + vec2(0.0, -0.15), 0.02, -0.2 * abs(sin(0.2*time + 115))) < 0)
        FragColor = vec4(0.12 + 0.1/s, 0.121+ 0.1/s, 0.12+ 0.1/s, 0.5);
    }
    if(pos.y > 0.0f && abs(pos.x) < 0.6f && pos.y < 0.6f) {
        float shape_c = shape_circ(pos.xy);
        float shape_b = shape_bell(pos.xy);
        if(shape_c > 0.25f)
            FragColor = vec4(1.0f, 1- length(pos.xy)*2 - shape_c / 1.5, 0.3f, 1.0f);
        else if(shape_c > 0.2f)
            FragColor = vec4(1.0f, 1- length(pos.xy)*3 - shape_c / 1.75, 0.15f, 1.0f);
        else if(shape_c > 0.15f)
            FragColor = vec4(1.0f, 1- length(pos.xy)*3 - shape_c / 2, 0.0f, 1.0f);
        else if(shape_c > 0.1f)
            FragColor = vec4(1.0f, 1- length(pos.xy)*3.5 - shape_c / 2.5, 0.0f, 1.0f);
        else if(shape_b > 0.125f || shape_c > 0.07f)
            FragColor = vec4(1.0f, 1- length(pos.xy)*5 - shape_c / 3, 0.0f, 1.0f);
        else if(sin(time) > 0.0)
            addSpark(pos.xy - vec2(sin(2*time) * 0.03f, -cos(time) * 0.05));
    }
} 