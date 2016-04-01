#ifndef DGE_RENDER_H
#define DGE_RENDER_H

#include <stdio.h>

#include <SDL.h>
#include <GL/gl3w.h>

#include "dge_math.h"

struct WindowData
{
    int x;
    int y;
    int width;
    int height;

    uint32 flags;
    const char* title;
};

struct CameraState
{
    Vector2 position;
    Vector2 size;
};

#define glPrintError(alwaysPrint) __glPrintError(__FILE__, __LINE__, alwaysPrint)
static inline void __glPrintError(const char* file, int line, bool alwaysPrint)
{
    GLenum error = glGetError();
    const char* errorStr;
    if(alwaysPrint || (error != GL_NO_ERROR))
    {
        switch(error)
        {
        case GL_NO_ERROR:
            errorStr = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            errorStr = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            errorStr = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            errorStr = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            errorStr = "GL_OUT_OF_MEMORY";
            break;
        default:
            errorStr = "UNRECOGNIZED";
            break;
        }

        fprintf(stderr, "(%s:%d) OpenGL error: %s\n", file, line, errorStr);
    }
}

GLuint dge_loadShaderProgram(const char* vertShaderFilename, const char* fragShaderFilename);

bool32 dge_initRenderer(SDL_Window* window);
void dge_loadTexture(const char* fileName, GLuint textureID);
void dge_allocateAndLoadTexture(const char* fileName, GLuint* textureID);

void dge_loadDefaultShaders();
void dge_loadTextRenderer();
void dge_updateShaderCameraState(CameraState camera, GLuint shader);

void dge_drawLine2D(CameraState camera, Vector2 fromLoc, Vector2 toLoc, Vector4 color);
void dge_renderString(CameraState camera, const char* string, int length, Vector2 position, float size, Vector4 color);
void dge_renderSprite(CameraState camera, GLuint textureID, Vector2 position, float rotation, Vector2 size, Vector4 color);
void dge_renderSprite(CameraState camera, GLuint textureID, Vector2 position, float rotation, Vector2 size);

#endif
