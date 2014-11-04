#pragma once
  
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cassert>

#include "fontlib.h"

using namespace std;

typedef struct {
	unsigned char* buffer;
	unsigned int width;
	unsigned int rows;
	float offset_x;
	float offset_y;
} FreeTypeGlyph;

class FreeTypeLib : public FontLib<FT_Face, FreeTypeGlyph> {
	public:
		FreeTypeLib();
		~FreeTypeLib();

		FT_Face* loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) override;
		void freeFace(FT_Face* face) override;
		FreeTypeGlyph* rasterize(FT_Face* face, uint32_t glyphIndex) const override;
		void freeGlyph(FreeTypeGlyph* glyph) override;

	private:
		FT_Library lib;
		int force_ucs2_charmap(FT_Face ftf);
};