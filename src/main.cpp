#include <iostream>
#include <cassert>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "hbshaper.h"
#include "freetypelib.h"

using namespace std;

int main(int argc, char** argv) {
    //glutInit(&argc, argv);
    //glutCreateWindow("");

    // the font rasterizing library
    FreeTypeLib lib;

    HBShaper<FT_Face, FreeTypeGlyph> hbs("fonts/DejaVuSerif.ttf", &lib);

    hbs.init();
    
    HBText hbt = {
        "The quick brown fox jumps over the lazy dog",
        "en",
        HB_SCRIPT_LATIN,
        HB_DIRECTION_LTR
    };

    hbs.drawText(hbt);

    return 0;
}