#version 330 core

in vec3 myCol;
out vec4 fragCol;


void main()
{
    fragCol = vec4(myCol, 1.0f);
}