#include <iostream>
#include <SDL.h>
#include <libnoise/noise.h>
#include "gl_core_3_3.hpp"
#include "vertexbuffer.h"
#include "shader.h"
#include "MarchingCubesLut.h"
#include "texture.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define RES_X 800
#define RES_Y 600

#include "framework.h"
#include "camera.h"

void printPlatformInformation()
{
    SDL_Log("Platform: %s", SDL_GetPlatform());
    SDL_Log("Number of physical cores: %d", SDL_GetCPUCount());
    SDL_Log("Physical Memory: %d", SDL_GetSystemRAM());
    SDL_Log("Video Driver: %s", SDL_GetCurrentVideoDriver());
}

double GetTicksInSeconds()
{
    return (float)SDL_GetTicks()/1000.0;
}

int main(int argc, char** argv)
{
    Framework theFramework;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    printPlatformInformation();

    SDL_Window* window = SDL_CreateWindow("Marching Cubes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_X, RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GLContext openglContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(0);

    gl::exts::LoadTest loadtest = gl::sys::LoadFunctions();

    if(!loadtest) {
        SDL_Log("SOME SHIT HAPPENED?");
    }
    //gl::PolygonMode(gl::FRONT_AND_BACK, gl::LINE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    theFramework.Initialize();

    SDL_Event e;

    gl::Enable(gl::DEPTH_TEST);
    gl::Viewport(0, 0, 1024, 768);

    VertexBuffer testBuffer;
    noise::module::Perlin testModule;
    noise::module::Clamp clampModule;
    testModule.SetSeed(12313213056456);

    clampModule.SetSourceModule(0, testModule);
    clampModule.SetBounds(0.0, 1.0);
    //Generate test chunk
    float* cData = new float[32*32*32];
    std::vector<float> bData;


    bool toggle=true;
    for(int x = 0; x < 16; x++)
    {
        for(int y = 0; y < 16; y++)
        {
            for(int z = 0; z < 16; z++)
            {
                cData[y+z*16+x*16*16] = clampModule.GetValue(x/16.0, y/16.0, z/16.0);
            }
        }
    }

    for(int x = 0; x < 16; x++)
    {
        for(int y = 0; y < 16; y++)
        {
            for(int z = 0; z < 16; z++)
            {
                bData.push_back(x);
                bData.push_back(y);
                bData.push_back(z);
            }
        }
    }

    testBuffer.Update(&bData[0]);

    Texture chunkData(gl::TEXTURE_3D, 0, gl::R32F, gl::RED, gl::FLOAT);
    chunkData.Bind();
    chunkData.SetMinFilter(gl::LINEAR);
    chunkData.SetMagFilter(gl::LINEAR);
    chunkData.SetWrapS(gl::CLAMP_TO_EDGE);
    chunkData.SetWrapT(gl::CLAMP_TO_EDGE);
    chunkData.SetWrapR(gl::CLAMP_TO_EDGE);

    chunkData.SetData(cData, 16, 16, 16);


    Texture lut(gl::TEXTURE_RECTANGLE, 1, gl::R32I, gl::RED_INTEGER, gl::INT);
    lut.SetMinFilter(gl::NEAREST);
    lut.Bind();

    lut.SetData((void*)triTable, 16, 256);

    int* lutTest = new int[256*16];
    gl::GetTexImage(gl::TEXTURE_2D, 0, gl::RED_INTEGER, gl::INT, lutTest);

    Shader testShader;
    testShader.LoadVertexShaderFromFile("Shaders/terrainChunk.vert");
    testShader.LoadGeometryShaderFromFile("Shaders/terrainChunk.geom");
    testShader.LoadPixelShaderFromFile("Shaders/terrainChunk.frag");
    testShader.Link();

    testShader.Bind();
    gl::Uniform1i(gl::GetUniformLocation(testShader.GetProgram(), "chunkData"), chunkData.GetBoundTextureUnit());
    gl::Uniform1i(gl::GetUniformLocation(testShader.GetProgram(), "trisLUT"), lut.GetBoundTextureUnit());
    gl::Uniform1f(gl::GetUniformLocation(testShader.GetProgram(), "isoLevel"), 0.5f);

    GLfloat cubeCorners[24] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f
    };


    gl::Uniform3fv(gl::GetUniformLocation(testShader.GetProgram(), "cubeCorners"), 8, cubeCorners);

    Camera theCamera;

    testShader.Unbind();

    bool running = true;
    bool hasMouseFocus = true;
    double elapsed_time = 0.0;
    double current_time = GetTicksInSeconds();
    int fps=0;

    bool wKey = false;
    bool sKey = false;
    bool dKey = false;
    bool aKey = false;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) {
                running = false;
            }

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    running = false;

                if(e.key.keysym.sym == SDLK_w)
                {
                    wKey = true;
                }

                if(e.key.keysym.sym == SDLK_s)
                {
                    sKey = true;
                }

                if(e.key.keysym.sym == SDLK_a)
                {
                    aKey = true;
                }

                if(e.key.keysym.sym == SDLK_d)
                {
                    dKey = true;
                }


                if(e.key.keysym.sym == SDLK_LALT)
                {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                }
            }

            if(e.type == SDL_KEYUP)
            {
                if(e.key.keysym.sym == SDLK_w)
                {
                    wKey = false;
                }

                if(e.key.keysym.sym == SDLK_s)
                {
                    sKey = false;
                }

                if(e.key.keysym.sym == SDLK_a)
                {
                    aKey = false;
                }

                if(e.key.keysym.sym == SDLK_d)
                {
                    dKey = false;
                }
            }

            if(e.type == SDL_MOUSEMOTION)
            {
                if(hasMouseFocus)
                {
                    //SDL_Log("%d, %d", e.motion.xrel, e.motion.yrel);
                    theCamera.MouseOrient(e.motion.xrel, e.motion.yrel);
                }

            }

            if(e.type == SDL_WINDOWEVENT)
            {
                if(e.window.event == SDL_WINDOWEVENT_ENTER)
                {
                    hasMouseFocus = true;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }

                if(e.window.event == SDL_WINDOWEVENT_LEAVE)
                {
                    hasMouseFocus = false;
                }

                if(e.window.event == SDL_WINDOWEVENT_MINIMIZED)
                {
                    hasMouseFocus = false;
                }
            }
        }

        double new_time = GetTicksInSeconds();
        double frame_time = new_time - current_time;
        current_time = GetTicksInSeconds();
        elapsed_time += frame_time;
        fps++;
        if(elapsed_time > 1.0)
        {
            SDL_Log("FPS %d", fps);
            fps = 0;
            elapsed_time = 0;
        }

        theFramework.Update(frame_time);
        theFramework.Draw();

        if(aKey)
        {
            theCamera.Move(CameraDirection::LEFT);
        }

        if(sKey)
        {
            theCamera.Move(CameraDirection::BACK);
        }

        if(wKey)
        {
            theCamera.Move(CameraDirection::FORWARD);
        }

        if(dKey)
        {
            theCamera.Move(CameraDirection::RIGHT);
        }

        gl::ClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        testShader.Bind();

        glm::mat4 view = theCamera.GetMatrix();
        glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
        glm::mat4 MVPmatrix = projection * view;
        gl::UniformMatrix4fv(gl::GetUniformLocation(testShader.GetProgram(), "modelViewProjectionMatrix"), 1, false, &MVPmatrix[0][0]);



        testBuffer.Draw();
        testShader.Unbind();
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}

