#version 330 core

in vec3 myCol;
in vec2 myTextCoord;
out vec4 fragCol;

uniform sampler2D myTexture;


void main()
{
    fragCol = texture(myTexture,myTextCoord);
}