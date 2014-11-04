#pragma once

#include <hb.h>
#include <hb-ft.h>

#include "freetypelib.h"

typedef struct {
	string data;
	string language;
	hb_script_t script;
	hb_direction_t direction;
	const char* c_data() { return data.c_str(); };
} HBText;

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