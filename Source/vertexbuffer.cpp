#include "vertexbuffer.h"

VertexBuffer::VertexBuffer()
{
    gl::GenVertexArrays(1, &vao);
    gl::BindVertexArray(vao);

    gl::GenBuffers(1, &vbo);
    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
    gl::BufferData(gl::ARRAY_BUFFER, (31*31*31) * (sizeof(GLfloat) * 3), 0, gl::DYNAMIC_DRAW);

    gl::VertexAttribPointer(0, 3, gl::FLOAT, false, 0, 0);

    gl::EnableVertexAttribArray(0);
    gl::BindVertexArray(0);
}

void VertexBuffer::Update(GLfloat *bytes)
{
    gl::BindVertexArray(vao);

    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
    gl::BufferSubData(gl::ARRAY_BUFFER, 0, (31*31*31) * (sizeof(GLfloat) * 3), bytes);
    gl::BindVertexArray(0);
}

void VertexBuffer::Draw()
{
    gl::BindVertexArray(vao);
    gl::DrawArrays(gl::POINTS, 0, 16 * 16 * 16);
    gl::BindVertexArray(0);
}
