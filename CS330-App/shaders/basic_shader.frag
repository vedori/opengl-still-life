#version 330 core

out vec4 FragColor;

in vec3 vertexRGB;

void main()
{
    FragColor = vec4(vertexRGB, 1.0);
}