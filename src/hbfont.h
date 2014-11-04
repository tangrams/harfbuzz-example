#pragma once

#include <hb.h>
#include <hb-ft.h>

#include "freetypelib.h"
#include "hbtext.h"

class HBFont {
public:
	HBFont(const string& fontFile);
	~HBFont();

	void init();
	void drawText(HBText& text);

private:
	FreeTypeLib ftl;
	FT_Face face;

	hb_font_t* font;
	hb_buffer_t* buffer;
};