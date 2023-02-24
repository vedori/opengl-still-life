#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

out vec3 vertexRGB;
out vec2 vertexUV;
out vec3 vertexNormal;
out vec3 vertexFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main ()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexRGB = color;
    vertexUV = uv;
    vertexNormal = mat3(transpose(inverse(model))) * normal;
    vertexFragPos = vec3(model * vec4(position, 1.0f));
}