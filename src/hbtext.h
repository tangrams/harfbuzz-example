#pragma once 

#include <iostream>

typedef struct {
	std::string data;
	std::string language;
	hb_script_t script;
	hb_direction_t direction;
	const char* c_data() { return data.c_str(); };
} HBText;