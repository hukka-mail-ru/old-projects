#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
#include <string>
#include <map>

#include <GLES/gl.h>
#include "BMP.h"

#include "../system/Macros.h"
#include "Window.h"

#define FIELD_TEXTURE_WIDTH 128
#define CENTRAL_TEXTURE_WIDTH 64

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
    
    struct Texture
    {
        Texture(): id(0), fragmentsInRow(1) {}
        
        unsigned id;
        unsigned fragmentsInRow; // e.g. '4' means that the texture consists of 16 fragments.  
        EDR_SurfacePtr surface;
    };

    typedef boost::shared_ptr<Texture> TexturePtr; 
    
public:
    
    void startup();
    
    void drawSolidPolygon(const GLshort* vertexArray, unsigned vertNum, const RGBA_Color& color);
    
    void drawLineLoop(const GLshort* vertexArray, unsigned vertNum, const RGBA_Color& color,
                      float width);

    void drawSprite(const std::string& textureName, const unsigned fragmentID, 
                    const RGBA_Color& color, BindXY bindXY, GLfloat x, GLfloat y, float angle);    
    
    void enableBlend();
    void disableBlend();
    
    void copyBufferIntoTexture(const std::string& texName, GLint x, GLint y);
    
    void clearScreen();
           
private:

   
    void createTextures();
    
    void createTexture(const char* dir, const char* name, unsigned fragmentsInRow);
    void createCompressedTexture(const char* dir, const char* name);
    void createEmptyTexture(const char* name, unsigned short width);
    
    int getTextureSize(int format, int width, int height);
    void checkError(const std::string where = "");

    std::map<std::string, TexturePtr> textures;
};

#endif /*VIDEO_H_*/
