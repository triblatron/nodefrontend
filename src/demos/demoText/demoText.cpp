//
// Created by tony on 24/08/24.
//
#include "config/config.h"

#include <GL/glut.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <cstdint>


class Image
{
public:
    Image(size_t width, size_t height, size_t numComponents)
    :
    _width(width),
    _height(height),
    _numComponents(numComponents)
    {
        _buffer = new GLubyte[height*width*numComponents];
    }

    ~Image()
    {
        delete [] _buffer;
    }

    size_t width() const
    {
        return _width;
    }

    size_t height() const
    {
        return _height;
    }

    void set(size_t row, size_t col, std::uint8_t red, std::uint8_t green, std::uint8_t blue)
    {
        if (row < _height && col < _width)
        {
            _buffer[row*_width*_numComponents+col*_numComponents+0] = red;
            _buffer[row*_width*_numComponents+col*_numComponents+1] = green;
            _buffer[row*_width*_numComponents+col*_numComponents+2] = blue;
            _buffer[row*_width*_numComponents+col*_numComponents+3] = 255;
        }
    }

    GLubyte* data()
    {
        return _buffer;
    }
private:
    size_t _width{0};
    size_t _height{0};
    size_t _numComponents{0};
    GLubyte* _buffer{nullptr};
};

static Image* texImage = new Image(512,512,4);
static GLuint texName;
static FT_Library  library;
static FT_Face     face;      /* handle to face object */

void copyGlyphToImage(FT_GlyphSlot glyph, Image* image)
{
    for (auto row = 0; row<glyph->bitmap.rows; ++row)
    {
        for (auto col=0; col<glyph->bitmap.width; ++col)
        {
            auto intensity = glyph->bitmap.buffer[row*glyph->bitmap.width+col];

            image->set(glyph->bitmap.rows - 1 - row, col, intensity, intensity, intensity);
        }
    }
}

void createImage(Image* image)
{
    for (size_t row=0; row<image->height(); ++row)
    {
        for (size_t col=0; col<image->width(); ++col)
        {
            // Should be yellow
            image->set(row,col,255,255,0);
        }
    }
    FT_GlyphSlot slot = face->glyph;
    copyGlyphToImage(slot, image);
}


void init()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    createImage(texImage);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImage->width(), texImage->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage->data());
}

void onReshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void onDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texName);

    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);
    glTexCoord2d(0.0, 0.0);
    glVertex2d(-0.5, -0.5);
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0.5,-0.5);
    glTexCoord2d(1.0,1.0);
    glVertex2d(0.5,0.5);
    glTexCoord2d(0.0,1.0);
    glVertex2d(-0.5,0.5);
    glEnd();
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
    int error = FT_Init_FreeType( &library );
    if ( error )
    {
        std::cerr << "Error initialising FreeType library, bailing\n";
        return -1;
    }
    error = FT_New_Face( library,
                         "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                         0,
                         &face );
    if ( error == FT_Err_Unknown_File_Format )
    {
        std::cerr << "Unsupported font format, bailing\n";

        return -2;
    }
    else if ( error )
    {
        std::cerr << "Failed to open font file, bailing\n";

        return -3;
    }
    std::cout << "Scalable:" << (face->face_flags & FT_FACE_FLAG_SCALABLE) << std::endl;
    error = FT_Set_Char_Size(
            face,    /* handle to face object         */
            0,       /* char_width in 1/64 of points  */
            16*64,   /* char_height in 1/64 of points */
            300,     /* horizontal device resolution  */
            300 );   /* vertical device resolution    */
    auto glyph_index = FT_Get_Char_Index( face, 65 );
    if (glyph_index==0)
    {
        std::cerr << "Failed to get glyph index, bailing\n";

        return -4;
    }
    error = FT_Load_Glyph(
            face,          /* handle to face object */
            glyph_index,   /* glyph index           */
            0x0 );  /* load flags, see below */
    if (error)
    {
        std::cerr << "Error loading glyph\n";

        return -5;
    }
    if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    {
        std::cout << "Rendering glyph\n";

        error = FT_Render_Glyph( face->glyph,   /* glyph slot  */
                                 FT_RENDER_MODE_NORMAL ); /* render mode */
        if (error)
        {
            std::cerr << "Failed to render glyph, bailing\n";

            return -6;
        }
    }
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(512,512);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutCreateWindow("demoText");
    init();
    glutReshapeFunc(onReshape);
    glutDisplayFunc(onDisplay);
    glutMainLoop();
}
