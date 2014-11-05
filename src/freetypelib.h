#pragma once
  
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cassert>
#include <cmath>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "fontlib.h"

using namespace std;

class FreeTypeLib : public FontLib<FT_Face> {
public:
	FreeTypeLib();
	~FreeTypeLib();

	FT_Face* loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) override;
	void freeFace(FT_Face* face) override;
	Glyph* rasterize(FT_Face* face, uint32_t glyphIndex) const override;
	void freeGlyph(Glyph* glyph) override;

private:
	FT_Library lib;
	int force_ucs2_charmap(FT_Face ftf);
};