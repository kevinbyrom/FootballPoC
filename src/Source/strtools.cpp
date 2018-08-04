#include "StrTools.h"



void RTrim(char * string, int slen)
	{

	///////////////////////////////////////////////
	//											 //
	// Used to trim a string from the right side //
	//											 //
	///////////////////////////////////////////////


	int endpos = slen + 1;

	for (int i = slen; i >= 0, endpos == slen + 1; i--)
		if (string[i] != ' ')
			endpos = i + 1;

	string[endpos] = '\0';
	}