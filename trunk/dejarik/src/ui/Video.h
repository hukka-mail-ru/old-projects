#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
#include <string>
#include <map>

#include <GLES/gl.h>

#include "../system/Macros.h"
#include "Window.h"

#define BOARD_TEXTURE_WIDTH 128

#define WINDOW_WIDTH  240
#define WINDOW_HEIGHT 320


struct RGBA_Color
{
    RGBA_Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}
    float r;
    float g;
    float b;
    float a;
};


enum BindXY
{
    XY_CENTER,
    XY_LEFT_BOTTOM,
    XY_RIGHT_BOTTOM,
    XY_LEFT_TOP,
    XY_RIGHT_TOP
};



class Video
{
private:
    
    struct Surface // for BMP loading
    {
        GLbyte* pixels;
        int w;
        int h;
    };

    struct Texture
    {
        unsigned id;
        GLfloat w;
        GLfloat h;
    };

    CLASSPTR(Texture);  
    
public:
    
    void startup(const std::vector<std::string>& pieceNames);
    void stop();
    
    // a new version
    void drawPolygon(const GLshort* vertexArray, unsigned vertNum, const RGBA_Color& color);
    
    void drawLineLoop(const GLshort* vertexArray, unsigned vertNum, const RGBA_Color& color,
            float width);

    void drawSprite(const std::string& texName, const RGBA_Color& color, 
            BindXY bindXY, GLshort x, GLshort y, float angle);    
        
private:
    
    Surface* loadBMP(const char* filename);
    void freeSurface(Surface* surface);

   
    void createImages(const std::vector<std::string>& names);
    void createImage(const std::string& name);

    void loadTexture(TexturePtr& texture, const std::string& path);
    void resizeWindow(unsigned width, unsigned height);
    


    std::map<std::string, TexturePtr> textures;
    
    int viewport[4]; 

};

#endif /*VIDEO_H_*/
