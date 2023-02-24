#include "mesh.h"

#include <utility>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<uint32_t> &elements, Texture texture) : _texture{std::move(texture)} {
    // generates a normal vector given three vertices
    for (auto i = 0; i < elements.size(); i += 3) {
        auto v1Index = elements[i];
        auto v2Index = elements[i + 1];
        auto v3Index = elements[i + 2];

        Shapes::UpdateNormals(vertices[v1Index], vertices[v2Index], vertices[v3Index]);
    }

    // bind the VAO and EBO for OpenGL to send mesh data to GPU
    // the VAO holds all subsequent VBOs and EBOs
    glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    // create VBO for OpenGL to send vertex data to the VAO
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(),
                 GL_STATIC_DRAW);

    // create EBO for OpenGL to send index data to GPU to avoid overhead resulting in overlapping lines
    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(uint32_t)), elements.data(),
                 GL_STATIC_DRAW);

    // enables OpenGL to know how to connect the shader attributes to the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Color)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Uv)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // caches element count
    _elementCount = elements.size();
}

void Mesh::Draw() const {
    // binds the texture to the shader
    _texture.Bind();

    // draws the object
    glBindVertexArray(_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, static_cast<int>(_elementCount), GL_UNSIGNED_INT, nullptr);
}

const Shader& Mesh::GetShader() {
    return _texture.GetShader();
}