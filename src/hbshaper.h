#pragma once

#include <hb.h>
#include <hb-ft.h>
#include <cmath>

#include "fontlib.h"
#include "hbtext.h"
#include "glutils.h"

using namespace std;

template <typename FF>
class HBShaper {
public:
	HBShaper(const string& fontFile, FontLib<FF>* lib);
	~HBShaper();

	void init();
	void drawText(HBText& text);

private:
	FontLib<FF> *lib;
	FF* face;

	hb_font_t* font;
	hb_buffer_t* buffer;
};

template <typename FF>
HBShaper<FF>::HBShaper(const string& fontFile, FontLib<FF>* fontLib) {
	lib = fontLib;
    float size = 50;
    face = lib->loadFace(fontFile, size * 64, 72, 72);
}

template <typename FF>
void HBShaper<FF>::drawText(HBText& text) {
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

    float x = 20;
    float y = 20;

    for(int i = 0; i < glyphCount; ++i) {
        Glyph* glyph = lib->rasterize(face, glyphInfo[i].codepoint);
        
        int twidth = pow(2, ceil(log(glyph->width)/log(2)));
        int theight = pow(2, ceil(log(glyph->height)/log(2)));

        auto tdata = new unsigned char[twidth * theight] ();

        for(int y = 0; y < glyph->height; ++y) {
            for(int x = 0; x < glyph->width; ++x) {
                tdata[y * twidth + x] = glyph->buffer[y * glyph->width + x];
            }
        }
        
        unsigned int textureId = uploadTextureData(twidth, theight, tdata);

        delete[] tdata;

        float x0 = x + glyphPos[i].x_offset + glyph->offset_x;
        float x1 = x0 + glyph->width;
        float y0 = floor(y + glyphPos[i].y_offset + glyph->offset_y);
        float y1 = floor(y0 - glyph->height);

        cout << x0 << " " << y0 << endl;
        cout << x1 << " " << y1 << endl;

        x += glyphPos[i].x_advance;
        y += glyphPos[i].y_advance;

        lib->freeGlyph(glyph);

        glDeleteTextures(1, &textureId);
    }
}

template <typename FF>
void HBShaper<FF>::init() {
    // TODO : use hb_font_create to make this generic
    font = hb_ft_font_create(*face, NULL); 
    buffer = hb_buffer_create();
    
    assert(hb_buffer_allocation_successful(buffer));
}

template <typename FF>
HBShaper<FF>::~HBShaper() {
    lib->freeFace(face);

    hb_buffer_destroy(buffer);
    hb_font_destroy(font); 
}