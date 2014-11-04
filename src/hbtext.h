#pragma once 

typedef struct {
	string data;
	string language;
	hb_script_t script;
	hb_direction_t direction;
	const char* c_data() { return data.c_str(); };
} HBText;