#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include "gl_core_3_3.hpp"


class VertexBuffer
{
public:
    VertexBuffer();

    void Update(GLfloat *bytes);

    void Draw();

private:
    GLuint vao;
    GLuint vbo;
};

#endif // VERTEXBUFFER_H
