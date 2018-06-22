#include <assert.h>
#include <GLES2/gl2.h>
#include "texture.h"

Texture::Texture(GLuint textureTarget, GLuint textureUnit, GLint internalFormat, GLenum Format, GLenum Type)
{
    assert(textureUnit < 32);
    texture_unit = textureUnit;
    texture_target = textureTarget;
    internal_format = internalFormat;
    format = Format;
    type = Type;
    gl::ActiveTexture(gl::TEXTURE0+textureUnit);
    gl::GenTextures(1, &texture_id);
    gl::BindTexture(textureTarget, texture_id);
}

void Texture::SetData(void *data, int xsize, int ysize, int zsize)
{
    if(texture_target == gl::TEXTURE_1D)
    {
        gl::TexImage1D(texture_target, 0, internal_format, xsize, 0, format, type, data);
    }

    if(texture_target == gl::TEXTURE_2D)
    {
        gl::TexImage2D(texture_target, 0, internal_format, xsize, ysize, 0, format, type, data);
    }

    if(texture_target == gl::TEXTURE_3D)
    {
        gl::TexImage3D(texture_target, 0, internal_format, xsize, ysize, zsize, 0, format, type, data);
    }

    if(texture_target == gl::TEXTURE_RECTANGLE)
    {
        gl::TexImage2D(texture_target, 0, internal_format, xsize, ysize, 0, format, type, data);
    }

}

void Texture::Bind()
{
    gl::BindTexture(texture_target, texture_id);
}

void Texture::SetMinFilter(GLint param)
{
    gl::TexParameteri(texture_target, gl::TEXTURE_MIN_FILTER, param);
}

void Texture::SetMagFilter(GLint param)
{
    gl::TexParameteri(texture_target, gl::TEXTURE_MAG_FILTER, param);
}

void Texture::SetWrapS(GLint param)
{
    gl::TexParameteri(texture_target, gl::TEXTURE_WRAP_S, param);
}

void Texture::SetWrapR(GLint param)
{
    gl::TexParameteri(texture_target, gl::TEXTURE_WRAP_R, param);
}

void Texture::SetWrapT(GLint param)
{
    gl::TexParameteri(texture_target, gl::TEXTURE_WRAP_T, param);
}

int Texture::GetBoundTextureUnit()
{
    return texture_unit;
}

GLuint Texture::GetTextureID()
{
    return texture_id;
}
