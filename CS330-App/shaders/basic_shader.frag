#version 330 core

out vec4 FragColor;

in vec3 vertexRGB;
in vec2 vertexUV;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, vertexUV) * vec4(vertexRGB, 1.0);
}