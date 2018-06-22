#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <SDL.h>

#include "gl_core_3_3.hpp"

class Shader
{
public:
    Shader();

    void LoadVertexShaderFromFile(const char* file);
    void LoadGeometryShaderFromFile(const char* file);
    void LoadPixelShaderFromFile(const char* file);
    void Link();

    void Bind();
    void Unbind();

    GLuint GetProgram();

private:
    GLuint vertexShader;
    GLuint geometryShader;
    GLuint pixelShader;

    GLuint programID;

    bool hasVertexShader;
    bool hasGeometryShader;
    bool hasPixelShader;

    std::string ReadFile(const char* file);

    void printInfo(GLuint object, bool program);
};

#endif // SHADER_H
