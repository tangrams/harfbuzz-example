#include "hbfont.h"

HBFont::HBFont(const string& fontFile) {;
    face = ftl.loadFace(fontFile, 50 * 64, 72, 72);
}

void HBFont::drawText(HBText& text) {
	hb_buffer_reset(buffer);
    
    hb_buffer_set_direction(buffer, text.direction);
    hb_buffer_set_script(buffer, text.script);
    hb_buffer_set_language(buffer, hb_language_from_string(text.language.c_str(), text.language.size())); 
    size_t length = text.data.size();
    
    hb_buffer_add_utf8(buffer, text.c_data(), length, 0, length);

    hb_shape(font, buffer, NULL, 0);  

    unsigned int glyphCount;
    hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(buffer, &glyphCount);
    hb_glyph_position_t *glyphPos = hb_buffer_get_glyph_positions(buffer, &glyphCount);

    FT_Int32 flags =  FT_LOAD_RENDER | FT_LOAD_TARGET_LCD;

    for(int i = 0; i < glyphCount; ++i) {
        assert(!FT_Load_Glyph(face,
            glyphInfo[i].codepoint, // the glyph_index in the font file 
            flags
        )); // rasterize
        FT_GlyphSlot slot = face->glyph;
        FT_Bitmap ftBitmap = slot->bitmap;
        int top = slot->bitmap_top;     
        int left = slot->bitmap_left;  

        unsigned char* buffer = ftBitmap.buffer; 

        cout << top << endl;
        cout << left << endl;
    }
}

void HBFont::init() {
    font = hb_ft_font_create(face, NULL);
    buffer = hb_buffer_create();
    
    assert(hb_buffer_allocation_successful(buffer));
}

HBFont::~HBFont() {
    ftl.freeFace(face);

    hb_buffer_destroy(buffer);
    hb_font_destroy(font); 
}