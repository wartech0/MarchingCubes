#include "shader.h"

Shader::Shader()
{
    programID = gl::CreateProgram();
}

void Shader::LoadVertexShaderFromFile(const char *file)
{
    std::string source = ReadFile(file);
    vertexShader = gl::CreateShader(gl::VERTEX_SHADER);
    const char* vSource = source.c_str();
    gl::ShaderSource(vertexShader, 1, &vSource, 0);

    gl::CompileShader(vertexShader);

    int shader_ok = 0;
    gl::GetShaderiv(vertexShader, gl::COMPILE_STATUS, &shader_ok);

    if(!shader_ok) {
        printInfo(vertexShader, false);
    }

    hasVertexShader = true;
}

void Shader::LoadGeometryShaderFromFile(const char *file)
{
    std::string source = ReadFile(file);
    geometryShader = gl::CreateShader(gl::GEOMETRY_SHADER);
    const char* gSource = source.c_str();
    gl::ShaderSource(geometryShader, 1, &gSource, 0);

    gl::CompileShader(geometryShader);

    int shader_ok = 0;
    gl::GetShaderiv(geometryShader, gl::COMPILE_STATUS, &shader_ok);

    if(!shader_ok) {
        printInfo(geometryShader, false);
    }

    hasGeometryShader = true;
}

void Shader::LoadPixelShaderFromFile(const char *file)
{
    std::string source = ReadFile(file);
    pixelShader = gl::CreateShader(gl::FRAGMENT_SHADER);
    const char* pSource = source.c_str();
    gl::ShaderSource(pixelShader, 1, &pSource, 0);

    gl::CompileShader(pixelShader);

    int shader_ok = 0;
    gl::GetShaderiv(pixelShader, gl::COMPILE_STATUS, &shader_ok);

    if(!shader_ok) {
        printInfo(pixelShader, false);
    }

    hasPixelShader = true;
}

void Shader::Link()
{
    if(hasVertexShader) {
        gl::AttachShader(programID, vertexShader);
        //gl::DeleteShader(vertexShader);
    }

    if(hasGeometryShader) {
        gl::AttachShader(programID, geometryShader);
        //gl::DeleteShader(geometryShader);
    }

    if(hasPixelShader) {
        gl::AttachShader(programID, pixelShader);
        //gl::DeleteShader(pixelShader);
    }

    gl::LinkProgram(programID);

    int linkStatus = 0;
    gl::GetProgramiv(programID, gl::LINK_STATUS, &linkStatus);
    if(!linkStatus) {
        printInfo(programID, true);
    }
}

void Shader::Bind()
{
    gl::UseProgram(programID);
}

void Shader::Unbind()
{
    gl::UseProgram(0);
}

std::string Shader::ReadFile(const char *file)
{
    std::ifstream t(file);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return str;
}

GLuint Shader::GetProgram()
{
    return programID;
}

void Shader::printInfo(GLuint object, bool program)
{
    int InfoLogLength = 0;

    if(!program) {
        gl::GetShaderiv(object, gl::INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ShaderErrorMessage(InfoLogLength);
        gl::GetShaderInfoLog(object, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        SDL_Log("%s\n", &ShaderErrorMessage[0]);
    } else {
        gl::GetProgramiv(object, gl::INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ShaderErrorMessage(InfoLogLength);
        gl::GetProgramInfoLog(object, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        SDL_Log("%s\n", &ShaderErrorMessage[0]);
    }

}
