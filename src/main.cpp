#include <iostream>
#include <cassert>

#include "hbshaper.h"
#include "freetypelib.h"

using namespace std;

vector<gl::Mesh*> meshes;

int main(int argc, char** argv) {
    // the font rasterizing library
    FreeTypeLib lib;

    HBShaper<FT_Face> latinShaper("fonts/DejaVuSerif.ttf", &lib);
    HBShaper<FT_Face> arabicShaper("fonts/amiri-regular.ttf", &lib);

    latinShaper.init();
    arabicShaper.init();

    HBText hbt2 = {
        "ficellé",
        "fr",
        HB_SCRIPT_LATIN,
        HB_DIRECTION_LTR
    };

    HBText hbt1 = {
        "The quick brown fox jumps over the lazy dog",
        "en",
        HB_SCRIPT_LATIN,
        HB_DIRECTION_LTR
    };

    HBText hbt3 = {
        "هذه هي بعض النصوص العربي",
        "ar",
        HB_SCRIPT_ARABIC,
        HB_DIRECTION_RTL
    };

    gl::initGL(argc, argv);

    for(auto mesh: latinShaper.drawText(hbt2, 20, 200)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: latinShaper.drawText(hbt1, 20, 260)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: arabicShaper.drawText(hbt3, 20, 320)) {
        meshes.push_back(mesh);
    }

    gl::uploadMeshes(meshes);
    
    gl::loop(meshes);
    gl::finish();

    gl::deleteMeshes(meshes);

    return 0;
}