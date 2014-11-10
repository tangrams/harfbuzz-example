#include "truetypelib.h"


TrueTypeLib::TrueTypeLib() {
	
}

TrueTypeLib::~TrueTypeLib() {

}

TrueTypeFace* TrueTypeLib::loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) { 
	return NULL; 
}

void TrueTypeLib::freeFace(TrueTypeFace* face) {

}

Glyph* TrueTypeLib::rasterize(TrueTypeFace* face, uint32_t glyphIndex) const { 
	return NULL;
}

void TrueTypeLib::freeGlyph(Glyph* glyph) {

}