#include <iostream>
#include <cassert>
#include <ctime>

#include "hbshaper.h"
#include "freetypelib.h"

using namespace std;

vector<gl::Mesh*> meshes;

int main(int argc, char** argv) {
    srand(time(NULL));

    // the font rasterizing library
    FreeTypeLib lib;
    clock_t begin, end; 

    HBShaper<FT_Face> latinShaper("fonts/DejaVuSerif.ttf", &lib);
    HBShaper<FT_Face> arabicShaper("fonts/amiri-regular.ttf", &lib);
    HBShaper<FT_Face> russianShaper("fonts/DejaVuSerif.ttf", &lib);
    HBShaper<FT_Face> hanShaper("fonts/fireflysung.ttf", &lib);
    HBShaper<FT_Face> hindiShaper("fonts/Sanskrit2003.ttf", &lib);

    latinShaper.init();
    arabicShaper.init();
    russianShaper.init();
    hanShaper.init();
    hindiShaper.init();

    HBText hbt1 = {
        "ficellé fffffi. VAV.",
        "fr",
        HB_SCRIPT_LATIN,
        HB_DIRECTION_LTR
    };

    HBText hbt2 = {
        "تسجّل يتكلّم",
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

    HBText hbt4 = {
        "緳 踥踕",
        "ch",
        HB_SCRIPT_HAN,
        HB_DIRECTION_TTB
    };

    HBText hbt5 = {
        "हालाँकि प्रचलित रूप पूजा",
        "hi",
        HB_SCRIPT_DEVANAGARI,
        HB_DIRECTION_LTR
    };

    latinShaper.addFeature(HBFeature::KerningOn);

    gl::initGL(argc, argv);

    begin = clock();
    
    // ask for some meshes, this is not optimal since every glyph has its
    // own texture, should use an atlas than contains glyph inside
    for(auto mesh: latinShaper.drawText(hbt1, 20, 320)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: arabicShaper.drawText(hbt2, 20, 220)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: russianShaper.drawText(hbt3, 20, 120)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: hanShaper.drawText(hbt4, 700, 380)) {
        meshes.push_back(mesh);
    }

    for(auto mesh: hindiShaper.drawText(hbt5, 20, 20)) {
        meshes.push_back(mesh);
    }

    end = clock();

    std::cout << ((float) (end - begin) / CLOCKS_PER_SEC) * 1000 << " ms." << std::endl;

    gl::uploadMeshes(meshes);
    
    gl::loop(meshes);
    gl::finish();

    gl::deleteMeshes(meshes);

    return 0;
}