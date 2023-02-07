#version 330 core

out vec4 FragColor;

in vec3 vertexRGB;
in vec2 vertexUV;

uniform sampler2D tex0;
uniform vec2 uvScale;

void main()
{
    FragColor = texture(tex0, vertexUV * uvScale) * vec4(vertexRGB, 1.0);
}