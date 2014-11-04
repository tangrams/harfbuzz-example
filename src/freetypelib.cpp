#include "freetypelib.h"

FreeTypeLib::FreeTypeLib() {
	assert(!FT_Init_FreeType(&lib));
}

FreeTypeLib::~FreeTypeLib() {
	FT_Done_FreeType(lib);
}

FT_Face FreeTypeLib::loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) {
	FT_Face face = nullptr;

    assert(!FT_New_Face(lib, fontName.c_str(), 0, &face));
    assert(!force_ucs2_charmap(face));
    assert(!FT_Set_Char_Size(face, 0, ptSize, deviceHDPI, deviceVDPI));

    return face;
}

void FreeTypeLib::freeFace(FT_Face face) {
	FT_Done_Face(face);
}

int FreeTypeLib::force_ucs2_charmap(FT_Face ftf) {
    for(int i = 0; i < ftf->num_charmaps; i++) {
        if ((  (ftf->charmaps[i]->platform_id == 0)
            && (ftf->charmaps[i]->encoding_id == 3))
           || ((ftf->charmaps[i]->platform_id == 3)
            && (ftf->charmaps[i]->encoding_id == 1))) {
                return FT_Set_Charmap(ftf, ftf->charmaps[i]);
        }
    }
    return -1;
}
