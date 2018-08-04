#ifndef __ROSTER_H
#define __ROSTER_H


#include "Gamelib.h"
#include "PlayerInfo.h"
#include "Constants.h"


class ROSTER
	{
	public:

		PLAYER_INFO * GetPlayerInfo(int pos);

		void RemoveAllFromPlay();

		BOOL Load(STRING directory, STRING filename);

		
	public:

		int NumPlayers;

		PLAYER_INFO PlayerInfo[MAX_ROSTER_PLAYERS];
	};



#endif