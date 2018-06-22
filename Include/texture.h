#ifndef TEXTURE_H
#define TEXTURE_H
#include "gl_core_3_3.hpp"

class Texture
{
public:
    Texture(GLuint textureTarget, GLuint textureUnit, GLint internalFormat, GLenum Format, GLenum Type);

    void SetData(void* data, int xsize, int ysize = 0, int zsize = 0);
    void Bind();

    void SetMinFilter(GLint param);
    void SetMagFilter(GLint param);
    void SetWrapS(GLint param);
    void SetWrapT(GLint param);
    void SetWrapR(GLint param);

    int GetBoundTextureUnit();
    GLuint GetTextureID();

private:
    GLuint texture_id;
    GLuint texture_unit;
    GLenum texture_target;
    GLint internal_format;
    GLenum format;
    GLenum type;
};

#endif // TEXTURE_H
