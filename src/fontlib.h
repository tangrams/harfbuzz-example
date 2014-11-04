#pragma once

#include <iostream>

/* 
 * Font interface template
 */ 
template <typename FF, typename FG>
class FontLib {

public:
	virtual FF* loadFace(const std::string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) = 0;
	virtual void freeFace(FF* face) = 0;
	virtual FG* rasterize(FF* face, uint32_t glyphIndex) const = 0;
	virtual void freeGlyph(FG* glyph) = 0;

};