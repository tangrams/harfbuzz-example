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
    HBShaper<FT_Face> russian("fonts/DejaVuSerif.ttf", &lib);

    latinShaper.init();
    arabicShaper.init();
    russian.init();

    HBText hbt1 = {
        "ficellé fffffi.",
        "fr",
        HB_SCRIPT_LATIN,
        HB_DIRECTION_LTR
    };

    HBText hbt2 = {
        "الى ما شاسعة وقوعها، ليتسنّى. عل يتم",
        "ar",
        HB_SCRIPT_ARABIC,
        HB_DIRECTION_RTL
    };

    HBText hbt3 = {
        "Дуо вёжи дёжжэнтиюнт ут",
        "ru",
        HB_SCRIPT_CYRILLIC,
        HB_DIRECTION_LTR
    };

    gl::initGL(argc, argv);

    // ask for some meshes, this is not optimal since every glyph has its
    // own texture, should make an atlas which contains every single glyph
    // inside
    for(auto mesh: latinShaper.drawText(hbt1, 20, 300)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: arabicShaper.drawText(hbt2, 20, 200)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: russian.drawText(hbt3, 20, 100)) {
        meshes.push_back(mesh);
    }

    gl::uploadMeshes(meshes);
    
    gl::loop(meshes);
    gl::finish();

    gl::deleteMeshes(meshes);

    return 0;
}