#pragma once

#include <iostream>

#include "fontlib.h"

using namespace std;

typedef struct {

} TrueTypeFace;

class TrueTypeLib : public FontLib<TrueTypeFace> {
public:
	TrueTypeLib();
	~TrueTypeLib();

	TrueTypeFace* loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) override;
	void freeFace(TrueTypeFace* face) override;
	Glyph* rasterize(TrueTypeFace* face, uint32_t glyphIndex) const override;
	void freeGlyph(Glyph* glyph) override;

private:

};