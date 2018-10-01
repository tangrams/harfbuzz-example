#include "freetypelib.h"
#include <cassert>

FreeTypeLib::FreeTypeLib() {
    FT_Init_FreeType(&lib);
}

FreeTypeLib::~FreeTypeLib() {
    FT_Done_FreeType(lib);
}

FT_Face* FreeTypeLib::loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) {
    FT_Face* face = new FT_Face;

    FT_Error error = FT_New_Face(lib, fontName.c_str(), 0, face);
    assert(error == 0 && "Cannot open font file");

    force_ucs2_charmap(*face);
    FT_Set_Char_Size(*face, 0, ptSize, deviceHDPI, deviceVDPI);

    return face;
}

void FreeTypeLib::freeFace(FT_Face* face) {
    FT_Done_Face(*face);
    delete face;
}

void FreeTypeLib::freeGlyph(Glyph* glyph) {
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

Glyph* FreeTypeLib::rasterize(FT_Face* face, uint32_t glyphIndex) const {
    Glyph* g = new Glyph;

    FT_Int32 flags =  FT_LOAD_DEFAULT;

    FT_Load_Glyph(*face,
        glyphIndex, // the glyph_index in the font file
        flags
    );

    FT_GlyphSlot slot = (*face)->glyph;
    FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

    FT_Bitmap ftBitmap = slot->bitmap;

    g->buffer = ftBitmap.buffer;
    g->width = ftBitmap.width;
    g->height = ftBitmap.rows;
    g->bearing_x = slot->bitmap_left;
    g->bearing_y = slot->bitmap_top;

    return g;
}
