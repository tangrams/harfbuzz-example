#pragma once
  
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cassert>

using namespace std;

class FreeTypeLib {
	public:
		FreeTypeLib();
		~FreeTypeLib();

		FT_Face loadFace(const string& fontName, int ptSize, int deviceHDPI, int deviceVDPI);
		void freeFace(FT_Face face);

	private:
		FT_Library lib;
		int force_ucs2_charmap(FT_Face ftf);
};