#ifndef DGE_TEXTURE_H
#define DGE_TEXTURE_H

struct TextureData
{
    unsigned char* pixels;
    int width;
    int height;
    int channelCount;
}

TextureData dge_loadTexture(const char* fileName, GLuint textureID=0);
void dge_freeTexture(TextureData texture);

#endif
