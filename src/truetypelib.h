#pragma once

#include <iostream>

#include "fontlib.h"

using namespace std;

typedef struct {

} TrueTypeGlyph;

typedef struct {

} TrueTypeFace;

class TrueTypeLib : public FontLib<TrueTypeFace, TrueTypeGlyph> {
	public:
		TrueTypeLib() {};
		~TrueTypeLib() {};

		TrueTypeFace* loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) override { return NULL; };
		void freeFace(TrueTypeFace* face) override {};
		TrueTypeGlyph* rasterize(TrueTypeFace* face, uint32_t glyphIndex) const override { return NULL; };
		void freeGlyph(TrueTypeGlyph* glyph) override {};

	private:

};