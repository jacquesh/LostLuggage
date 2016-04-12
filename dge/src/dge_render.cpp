#include "dge_render.h"

#include <SDL.h>
#include <SDL_rwops.h>

#include <GL/gl3w.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "dge.h"
#include "dge_math.h"
#include "dge_debug.h"

GLuint textTexture;
GLuint textVAO;
GLuint textShader;
GLuint textLocBuffer;
GLuint textUVBuffer;
GLuint textIndexBuffer;

GLuint lineVAO;
GLuint lineShader;
GLuint lineLocBuffer;

GLuint spriteVAO;
GLuint spriteShader;
GLuint flatColorShader;
GLuint indexBuffer;
GLuint vertexLocBuffer;
GLuint texCoordBuffer;

// TODO: Building with emscripten
// TODO: Some interesting notes: https://hero.handmadedev.org/forum/code-discussion/916-opengl-renderer-questions
// TODO: More reading ("buffer orphaning"?): https://www.reddit.com/r/gamedev/comments/3y1va8/sdl2_c_tile_map_most_efficient_way_to_render_lots/

GLuint dge::loadShader(const char* shaderFilename, GLenum shaderType)
{
    SDL_RWops* shaderFile = SDL_RWFromFile(shaderFilename, "r");
    if(shaderFile == nullptr)
    {
        debug("Error while trying to open shader file: %s", SDL_GetError());
        return 0;
    }

    int64 shaderSize = SDL_RWseek(shaderFile, 0, RW_SEEK_END);
    SDL_RWseek(shaderFile, 0, RW_SEEK_SET);
    char* shaderText = (char*)malloc(shaderSize+1);

    // TODO: Handle the case where it doesn't read the entire shader at once, only a part of it
    //       IE we should probably loop here until it reads the entire file
    int64 bytesRead = SDL_RWread(shaderFile, shaderText, 1, shaderSize);
    if(bytesRead != shaderSize)
    {
        debug("Expected to read %ld bytes from %s, instead got %ld", shaderSize, shaderFilename, bytesRead);
        free(shaderText);
        SDL_RWclose(shaderFile);
        return 0;
    }

    shaderText[shaderSize] = '\0';
    SDL_RWclose(shaderFile);

    // TODO: Handle GLerrors (Do we actually need to even?)
    //       If we have a problem here we'll catch it when we try to link the shader anyways
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);

    free(shaderText);

    return shader;
}

GLuint dge::loadShaderProgram(const char* vertShaderFilename,
                             const char* fragShaderFilename)
{
    GLuint vertShader = dge::loadShader(vertShaderFilename, GL_VERTEX_SHADER);
    GLuint fragShader = dge::loadShader(fragShaderFilename, GL_FRAGMENT_SHADER);

    // TODO: Handle GLerrors
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE)
    {
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &logLength, message);
        debug("Error: %s", message);

        glDeleteProgram(program);
        return 0;
    }

    debug("Successfully loaded shader from %s and %s", vertShaderFilename, fragShaderFilename);
    return program;
}

GLuint dge::loadShaderProgram(const char* vertShaderFilename,
                             const char* geomShaderFilename,
                             const char* fragShaderFilename)
{
    GLuint vertShader = dge::loadShader(vertShaderFilename, GL_VERTEX_SHADER);
    GLuint geomShader = dge::loadShader(geomShaderFilename, GL_GEOMETRY_SHADER);
    GLuint fragShader = dge::loadShader(fragShaderFilename, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, geomShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glDeleteShader(vertShader);
    glDeleteShader(geomShader);
    glDeleteShader(fragShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE)
    {
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &logLength, message);
        debug("Error: %s", message);

        glDeleteProgram(program);
        return 0;
    }

    debug("Successfully loaded shader from %s, %s and %s",
            vertShaderFilename, geomShaderFilename, fragShaderFilename);
    return program;
}

bool32 dge::initRenderer(SDL_Window* window)
{
    SDL_GLContext glc = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glc);

    if(gl3wInit())
    {
        debug("Unable to initialize OpenGL/GL3W");
        return false;
    }
    debug("Initialized OpenGL %s with support for GLSL %s",
            glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void dge::loadTexture(const char* fileName, GLuint textureID)
{
    int imageWidth;
    int imageHeight;
    int componentCount;
    unsigned char* imageBytes = stbi_load(fileName, &imageWidth, &imageHeight, &componentCount, 0);
    if(componentCount != 4)
    {
        debug("Error: Unable to load all 4 RGBA channels from image %s, only found %d",
                fileName, componentCount);
        // TODO: We need proper asserts at some point, as in HandmadeHero or something
    }

    // NOTE: We need to flip the image vertically, since
    //       OpenGL expects the first byte to be the bottom-left while
    //       stb_image gives the first byte as the top-left
    // TODO: Its probably more efficient to just copy the entire image as a single
    //       block rather than line by line, because that's 1 big copy at the start
    //       and then just 2 copies per line rather than 3 (for half the lines)
    int rowByteCount = 4*imageWidth;
    unsigned char* tempImageRow = new unsigned char[rowByteCount];
    for(int row=0; row<imageHeight/2; ++row)
    {
        int partnerRow = imageHeight - row - 1;
        uint8* rowPtr = imageBytes + row*rowByteCount;
        uint8* partnerRowPtr = imageBytes + partnerRow*rowByteCount;

        memcpy(tempImageRow, rowPtr, rowByteCount);
        memcpy(rowPtr, partnerRowPtr, rowByteCount);
        memcpy(partnerRowPtr, tempImageRow, rowByteCount);
    }
    delete[] tempImageRow;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Note: See OpenGL Common Mistakes Wiki page: "Creating a complete texture"
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    // NOTE: OpenGL expects image data in the order bottom-left to top-right
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 imageWidth, imageHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, imageBytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    // TODO: We free the allocated memory here because we've sent it to the GPU (we think)
    //       In future we probably want to not do that and rather keep it around
    //       so that we can re-send things to the GPU at some later stage
    stbi_image_free(imageBytes);
    debug("Successfully loaded texture from %s", fileName);
}

// NOTE: This is purely a convenience function so that you need to understand
//       less of this file in order to use it.
void dge::allocateAndLoadTexture(const char* fileName, GLuint* textureID)
{
    glGenTextures(1, textureID);
    dge::loadTexture(fileName, *textureID);
}

void dge::loadTextRenderer()
{
    textShader = dge::loadShaderProgram("resources/shaders/sprite.vsh",
                                        "resources/shaders/sprite.fsh");
    GLint positionLoc = glGetAttribLocation(textShader, "position");

    glGenTextures(1, &textTexture);
    dge::loadTexture("resources/gfx/font.png", textTexture);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textUVBuffer);

    float vertexLocData[12] = {-0.5f, -0.5f, 0.0f,
                               0.5f, -0.5f, 0.0f,
                               0.5f,  0.5f, 0.0f,
                              -0.5f,  0.5f, 0.0f};
    GLuint indexData[6] = {0,1,3,
                           1,2,3};

    glGenBuffers(1, &textLocBuffer);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textLocBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), vertexLocData,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, false, 0, 0);

    glGenBuffers(1, &textIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), indexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void dge::loadDefaultShaders()
{
    // Load shaders
    // Line Shader
    glGenVertexArrays(1, &lineVAO);
    lineShader = dge::loadShaderProgram("resources/shaders/line.vsh",
                                        "resources/shaders/line.fsh");
    glGenBuffers(1, &lineLocBuffer);

    // Sprite Shader
    glGenVertexArrays(1, &spriteVAO);
    spriteShader = dge::loadShaderProgram("resources/shaders/sprite.vsh",
                                          "resources/shaders/sprite.fsh");
    GLint spritePositionLoc = glGetAttribLocation(spriteShader, "position");
    GLint spriteTexCoordLoc = glGetAttribLocation(spriteShader, "texCoord");

    // Flat Color Shader
    flatColorShader = dge::loadShaderProgram("resources/shaders/flatColor.vsh",
                                             "resources/shaders/flatColor.fsh");
    // TODO: We don't need to set properties here because both spriteShader and flatColorShader have
    //       position (the only shared attribute) at location 0 (hard-specified in the shader file)

    // Load VBO/VAO data
    float vertexLocData[12] = {-0.5f, -0.5f, 0.0f,
                               0.5f, -0.5f, 0.0f,
                               0.5f,  0.5f, 0.0f,
                              -0.5f,  0.5f, 0.0f};
    float texCoordData[8] = {0.0f, 0.0f,
                             1.0f, 0.0f,
                             1.0f, 1.0f,
                             0.0f, 1.0f};
    GLuint indexData[6] = {0,1,3,
                           1,2,3};

    glBindVertexArray(spriteVAO);
    glGenBuffers(1, &vertexLocBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexLocBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), vertexLocData,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(spritePositionLoc);
    glVertexAttribPointer(spritePositionLoc, 3, GL_FLOAT, false, 0, 0);

    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), texCoordData,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(spriteTexCoordLoc);
    glVertexAttribPointer(spriteTexCoordLoc, 2, GL_FLOAT, false, 0, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), indexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void dge::updateShaderCameraState(CameraState camera, GLuint shader)
{
    float screenDepth = 2.0f;
    float projectionMatrix[16] = {2.0f/camera.size.x,0,0,0,
                                  0,2.0f/camera.size.y,0,0,
                                  0,0,-2.0f/screenDepth,0,
                                  0,0,-1,1};
    float viewingMatrix[16] = {1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                -camera.position.x-camera.size.x/2.0f, -camera.position.y-camera.size.y/2.0f, 0, 1};

    GLint projectionMatrixLoc = glGetUniformLocation(shader, "projectionMatrix");
    GLint viewingMatrixLoc = glGetUniformLocation(shader, "viewingMatrix");
    
    glUniformMatrix4fv(projectionMatrixLoc, 1, false, projectionMatrix);
    glUniformMatrix4fv(viewingMatrixLoc, 1, false, viewingMatrix);
}

// TODO: These 2 functions both assume that the window is 640x480,
//       we really need it to use the actual window size (for now the
//       window is not resizable)
dge::Vector2 dge::screenToWorldPoint(CameraState camera, Vector2 point)
{
    int screenWidth = 640;
    int screenHeight = 480;
    Vector2 result = point;
    result.y = screenHeight - result.y;

    result.x *= camera.size.x/screenWidth;
    result.y *= camera.size.y/screenHeight;

    result += camera.position;
    return result;
}
dge::Vector2 dge::worldToScreenPoint(CameraState camera, Vector2 point)
{
    int screenWidth = 640;
    int screenHeight = 480;
    Vector2 result = point;
    result -= camera.position;

    result.x *= screenWidth/camera.size.x;
    result.y *= screenWidth/camera.size.y;

    result.y = screenHeight - result.y;
    return result;
}

void dge::drawLine2D(CameraState camera, Vector2 fromLoc, Vector2 toLoc, Vector4 color)
{
    glUseProgram(lineShader);

    dge::updateShaderCameraState(camera, lineShader);

    GLint colorTintLoc = glGetUniformLocation(lineShader, "colorTint");
    glUniform4f(colorTintLoc, color.x, color.y, color.z, color.w);

    glBindVertexArray(lineVAO);
    GLint positionLoc = glGetAttribLocation(lineShader, "position");

    float vertexLocData[6] = {fromLoc.x, fromLoc.y, 0.0f,
                               toLoc.x, toLoc.y, 0.0f};
    glBindBuffer(GL_ARRAY_BUFFER, lineLocBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2*3*sizeof(float), vertexLocData, GL_STREAM_DRAW);
    glEnableVertexAttribArray(positionLoc);
    //TODO: If we move glVertexAttribPointer above glBindBuffer, we get a segfault,
    //      why is that? Is glVertexAttribPointer the function that actually causes data to
    //      be sent to the GPU? If thats true then things make sense, if not then what call does that?
    //      I'm pretty sure I thought it was glBufferData...
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, false, 0, 0);

    glDrawArrays(GL_LINES, 0, 2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(positionLoc);
    glBindVertexArray(0);
    glUseProgram(0);
}

void dge::renderString(CameraState camera, const char* string, int length, Vector2 position, float size, Vector4 color)
{
    glUseProgram(textShader);
    dge::updateShaderCameraState(camera, textShader);
    glBindVertexArray(textVAO);

    GLint colorTintLoc = glGetUniformLocation(textShader, "colorTint");
    glUniform4f(colorTintLoc, color.x, color.y, color.z, color.w);

    GLint textureSamplerLoc = glGetUniformLocation(textShader, "spriteTex");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glUniform1i(textureSamplerLoc, 0);

    // TODO: Do some more buffer footwork to just do this all in a single draw call
    //       By instead setting appropriate UV coords and buffering all the characters together
    GLint uvLoc = glGetAttribLocation(textShader, "texCoord");
    for(int ch=0; ch<length; ++ch)
    {
        float modelMatrix[16] = {size, 0, 0, 0,
                                 0, size, 0, 0,
                                 0, 0, 1, 0,
                                 position.x, position.y, 0, 1};

        GLint modelMatrixLoc = glGetUniformLocation(textShader, "modelMatrix");
        glUniformMatrix4fv(modelMatrixLoc, 1, false, modelMatrix);


        int texIndex = string[ch] - ' ';
        float topV = 1.0f - texIndex*(8.0f/1024.0f);
        float botV = 1.0f - (texIndex+1)*(8.0f/1024.0f);
        float texCoordData[8] = {0.0f, botV,
                                 1.0f, botV,
                                 1.0f, topV,
                                 0.0f, topV};
        glBindBuffer(GL_ARRAY_BUFFER, textUVBuffer);
        glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), texCoordData, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(uvLoc);
        glVertexAttribPointer(uvLoc, 2, GL_FLOAT, false, 0, 0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        position.x += size;
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void dge::renderQuad(CameraState camera, Vector2 centre, Vector2 size, float rotation, Vector4 color)
{
    glUseProgram(flatColorShader);
    dge::updateShaderCameraState(camera, flatColorShader);

    float cosTheta = cos(rotation);
    float sinTheta = sin(rotation);
    float modelMatrix[16] = {cosTheta*size.x, sinTheta*size.y, 0, 0,
                             -sinTheta*size.x, cosTheta*size.y, 0, 0,
                             0, 0, 1, 0,
                             centre.x, centre.y, 0, 1};

    GLint colorLoc = glGetUniformLocation(flatColorShader, "colorTint");
    GLint modelMatrixLoc = glGetUniformLocation(flatColorShader, "modelMatrix");

    glUniform4f(colorLoc, color.x, color.y, color.z, color.w);
    glUniformMatrix4fv(modelMatrixLoc, 1, false, modelMatrix);

    glBindVertexArray(spriteVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

/*
 * Renders a sprite at the given position and size and rotation (in radians).
 * The specified colour is used to tint the sprite (use white if no tinting is desired)
*/
void dge::renderSprite(CameraState camera, GLuint textureID, Vector2 position, float rotation, Vector2 size, Vector4 color)
{
    glUseProgram(spriteShader);

    dge::updateShaderCameraState(camera, spriteShader);

    float cosTheta = cos(rotation);
    float sinTheta = sin(rotation);
    float modelMatrix[16] = {cosTheta*size.x, sinTheta*size.y, 0, 0,
                             -sinTheta*size.x, cosTheta*size.y, 0, 0,
                             0, 0, 1, 0,
                             position.x, position.y, 0, 1};

    GLint colorTintLoc = glGetUniformLocation(spriteShader, "colorTint");
    GLint textureSamplerLoc = glGetUniformLocation(spriteShader, "spriteTex");
    GLint modelMatrixLoc = glGetUniformLocation(spriteShader, "modelMatrix");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureSamplerLoc, 0);

    glUniform4f(colorTintLoc, color.x, color.y, color.z, color.w);

    glUniformMatrix4fv(modelMatrixLoc, 1, false, modelMatrix);

    glBindVertexArray(spriteVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

void dge::renderSprite(CameraState camera, GLuint textureID, Vector2 position, float rotation, Vector2 size)
{
    Vector4 color(1.0f, 1.0f, 1.0f, 1.0f);
    dge::renderSprite(camera, textureID, position, rotation, size, color);
}
