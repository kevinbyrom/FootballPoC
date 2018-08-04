#ifndef __PLAY_H
#define __PLAY_H



#include "Constants.h"
#include "Formation.h"
#include "PlayerObj.h"

 

class PLAY
	{
	public:

		BOOL Load(STRING strName);


	public:

		STRING Name;
		STRING GraphicFilename;

		PLAYER_OBJ_FLAGS PlayerFlags[MAX_SIDE_PLAYERS];

		ACTIONS PreHikeActions[MAX_SIDE_PLAYERS];
		ACTIONS PostHikeActions[MAX_SIDE_PLAYERS];
	};



#endif