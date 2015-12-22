#pragma once

#include "hb.h"
#include "hb-ft.h"
#include <cmath>
#include <vector>
#include <limits>

#include "fontlib.h"
#include "hbtext.h"
#include "glutils.h"

using namespace std;

namespace HBFeature {
    const hb_tag_t KernTag = HB_TAG('k', 'e', 'r', 'n'); // kerning operations
    const hb_tag_t LigaTag = HB_TAG('l', 'i', 'g', 'a'); // standard ligature substitution
    const hb_tag_t CligTag = HB_TAG('c', 'l', 'i', 'g'); // contextual ligature substitution

    static hb_feature_t LigatureOff = { LigaTag, 0, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t LigatureOn  = { LigaTag, 1, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t KerningOff  = { KernTag, 0, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t KerningOn   = { KernTag, 1, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t CligOff     = { CligTag, 0, 0, std::numeric_limits<unsigned int>::max() };
    static hb_feature_t CligOn      = { CligTag, 1, 0, std::numeric_limits<unsigned int>::max() };
}

template <typename FF>
class HBShaper {
public:
	HBShaper(const string& fontFile, FontLib<FF>* lib);
	~HBShaper();

	void init();
	vector<gl::Mesh*> drawText(HBText& text, float x, float y);
    void addFeature(hb_feature_t feature);

private:
	FontLib<FF> *lib;
	FF* face;

	hb_font_t* font;
	hb_buffer_t* buffer;
    vector<hb_feature_t> features;
};

template <typename FF>
HBShaper<FF>::HBShaper(const string& fontFile, FontLib<FF>* fontLib) {
    lib = fontLib;
    float size = 50;
    face = lib->loadFace(fontFile, size * 64, 72, 72);
}

template <typename FF>
void HBShaper<FF>::addFeature(hb_feature_t feature) {
    features.push_back(feature);
}

template <typename FF>
vector<gl::Mesh*> HBShaper<FF>::drawText(HBText& text, float x, float y) {
    vector<gl::Mesh*> meshes;

    hb_buffer_reset(buffer);

    hb_buffer_set_direction(buffer, text.direction);
    hb_buffer_set_script(buffer, text.script);
    hb_buffer_set_language(buffer, hb_language_from_string(text.language.c_str(), text.language.size()));
    size_t length = text.data.size();

    hb_buffer_add_utf8(buffer, text.c_data(), length, 0, length);

    // harfbuzz shaping
    hb_shape(font, buffer, features.empty() ? NULL : &features[0], features.size());

    unsigned int glyphCount;
    hb_glyph_info_t *glyphInfo = hb_buffer_get_glyph_infos(buffer, &glyphCount);
    hb_glyph_position_t *glyphPos = hb_buffer_get_glyph_positions(buffer, &glyphCount);

    for(int i = 0; i < glyphCount; ++i) {
        hb_position_t kernX = 0;
        hb_position_t kernY = 0;

        if(i > 0) {
            hb_font_get_glyph_kerning_for_direction(font,
                glyphInfo[i - 1].codepoint,
                glyphInfo[i].codepoint,
                text.direction,
                &kernX, &kernY
            );
        }
        Glyph* glyph = lib->rasterize(face, glyphInfo[i].codepoint);

        int twidth = pow(2, ceil(log(glyph->width)/log(2)));
        int theight = pow(2, ceil(log(glyph->height)/log(2)));

        auto tdata = new unsigned char[twidth * theight] ();

        for(int iy = 0; iy < glyph->height; ++iy) {
            memcpy(tdata + iy * twidth, glyph->buffer + iy * glyph->width, glyph->width);
        }

        #ifdef DEBUG
        for(int iy = 0; iy < glyph->height; ++iy) {
            for(int ix = 0; ix < glyph->width; ++ix) {
                int c = (int) glyph->buffer[iy * glyph->width + ix];
                cout << (c == 255 ? '#' : '`');
            }
            cout << endl;
        }
        cout << endl;
        #endif

        float s0 = 0.0;
        float t0 = 0.0;
        float s1 = (float) glyph->width / twidth;
        float t1 = (float) glyph->height / theight;
        float kx = (float) kernX / 64;
        float ky = (float) kernY / 64;
        float xa = (float) glyphPos[i].x_advance / 64 + kx;
        float ya = (float) glyphPos[i].y_advance / 64 + ky;
        float xo = (float) glyphPos[i].x_offset / 64;
        float yo = (float) glyphPos[i].y_offset / 64;
        float x0 = x + xo + glyph->bearing_x;
        float y0 = floor(y + yo + glyph->bearing_y);
        float x1 = x0 + glyph->width;
        float y1 = floor(y0 - glyph->height);

        gl::Vertex* vertices = new gl::Vertex[4];
        vertices[0] = { x0,y0, s0,t0 };
        vertices[1] = { x0,y1, s0,t1 };
        vertices[2] = { x1,y1, s1,t1 };
        vertices[3] = { x1,y0, s1,t0 };

        unsigned short* indices = new unsigned short[6];
        indices[0] = 0; indices[1] = 1;
        indices[2] = 2; indices[3] = 0;
        indices[4] = 2; indices[5] = 3;

        gl::Mesh* m = new gl::Mesh;

        m->indices = indices;
        m->textureData = tdata;

        // don't do this!! use atlas texture instead
        m->textureId = gl::getTextureId(twidth, theight);

        m->vertices = vertices;
        m->nbIndices = 6;
        m->nbVertices = 4;

        gl::uploadTextureData(m->textureId, twidth, theight, tdata);

        meshes.push_back(m);

        x += xa;
        y += ya;

        lib->freeGlyph(glyph);
    }

    return meshes;
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
