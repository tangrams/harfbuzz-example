#pragma once

#include <hb.h>
#include <hb-ft.h>

#include "fontlib.h"
#include "hbtext.h"

using namespace std;

template <typename FF, typename FG>
class HBFont {
public:
	HBFont(const string& fontFile, FontLib<FF,FG>* lib);
	~HBFont();

	void init();
	void drawText(HBText& text);

private:
	FontLib<FF,FG> *lib;
	FF* face;

	hb_font_t* font;
	hb_buffer_t* buffer;
};

template <typename FF, typename FG>
HBFont<FF,FG>::HBFont(const string& fontFile, FontLib<FF,FG>* fontLib) {
	lib = fontLib;
    face = lib->loadFace(fontFile, 50 * 64, 72, 72);
}

template <typename FF, typename FG>
void HBFont<FF,FG>::drawText(HBText& text) {
	hb_buffer_reset(buffer);

    hb_buffer_set_direction(buffer, text.direction);
    hb_buffer_set_script(buffer, text.script);
    hb_buffer_set_language(buffer, hb_language_from_string(text.language.c_str(), text.language.size())); 
    size_t length = text.data.size();
    
    hb_buffer_add_utf8(buffer, text.c_data(), length, 0, length);

    // harfbuzz shaping
    hb_shape(font, buffer, NULL, 0);  

    unsigned int glyphCount;
    hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(buffer, &glyphCount);
    hb_glyph_position_t *glyphPos = hb_buffer_get_glyph_positions(buffer, &glyphCount);

    for(int i = 0; i < glyphCount; ++i) {
        FG* glyph = lib->rasterize(face, glyphInfo[i].codepoint);

        // TODO rendering stuff

        lib->freeGlyph(glyph);
    }
}

template <typename FF, typename FG>
void HBFont<FF,FG>::init() {
	// TODO : use hb_font_create to make this generic
    font = hb_ft_font_create(*face, NULL); 
    buffer = hb_buffer_create();
    
    assert(hb_buffer_allocation_successful(buffer));
}

template <typename FF, typename FG>
HBFont<FF,FG>::~HBFont() {
    lib->freeFace(face);

    hb_buffer_destroy(buffer);
    hb_font_destroy(font); 
}