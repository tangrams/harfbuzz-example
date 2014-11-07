#pragma once

#include <iostream>

typedef struct {
	unsigned char* buffer;
	unsigned int width;
	unsigned int height;
	float bearing_x;
	float bearing_y;
} Glyph;

/* 
 * Font interface template
 */ 
template <typename FF>
class FontLib {

public:
	virtual FF* loadFace(const std::string& fontName, int ptSize, int deviceHDPI, int deviceVDPI) = 0;
	virtual void freeFace(FF* face) = 0;
	virtual Glyph* rasterize(FF* face, uint32_t glyphIndex) const = 0;
	virtual void freeGlyph(Glyph* glyph) = 0;

};