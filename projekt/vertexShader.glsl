#version 450 core

in vec3 position;
out vec4 pos;

void main()
{
   gl_Position = vec4(position.xy * 2 - 1, 1.0, 1.0);
   pos = gl_Position;
   pos.z = position.z;
}