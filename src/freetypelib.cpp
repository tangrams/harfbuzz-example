#include "freetypelib.h"

FreeTypeLib::FreeTypeLib() {
	assert(!FT_Init_FreeType(&lib));
}

FreeTypeLib::~FreeTypeLib() {
	FT_Done_FreeType(lib);
}

FT_Face* FreeTypeLib::loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) {
	FT_Face* face = new FT_Face;

    assert(!FT_New_Face(lib, fontName.c_str(), 0, &(*face)));
    assert(!force_ucs2_charmap(*face));
    assert(!FT_Set_Char_Size(*face, 0, ptSize, deviceHDPI, deviceVDPI));

    return face;
}

void FreeTypeLib::freeFace(FT_Face* face) {
	FT_Done_Face(*face);
    delete face;
}

void FreeTypeLib::freeGlyph(FreeTypeGlyph* glyph) {
    delete glyph;
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

FreeTypeGlyph* FreeTypeLib::rasterize(FT_Face* face, uint32_t glyphIndex) const {
    FreeTypeGlyph* g = new FreeTypeGlyph;
    
    FT_Int32 flags =  FT_LOAD_RENDER;

    assert(!FT_Load_Glyph(*face,
        glyphIndex, // the glyph_index in the font file 
        flags
    )); // rasterize

    FT_GlyphSlot slot = (*face)->glyph;
    FT_Bitmap ftBitmap = slot->bitmap;
    int top = slot->bitmap_top;     
    int left = slot->bitmap_left;  

    unsigned char* buffer = ftBitmap.buffer; 

    cout << top << endl;
    cout << left << endl;

    return g;
}