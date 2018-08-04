#ifndef __SIDE_H
#define __SIDE_H


#include "PlayerObj.h"
#include "Formation.h"
#include "Play.h"
#include "Constants.h"


class SIDE
	{
	public:

		SIDE();

		int TeamIndex;
		int Direction;

		PLAYER_OBJ Player[MAX_SIDE_PLAYERS];
		
		void Clear();
		void Update();
		void ChangeUserControl(PLAYER_OBJ * player);
		void ChangeTargetReceiver();

	public:

		PLAYER_OBJ * Control;			// Current controlled player
		PLAYER_OBJ * TargetRec;			// Target receiver
		PLAYER_OBJ * Passer;			// Target Passer 
		PLAYER_OBJ * Rusher;			// Target Rusher
		PLAYER_OBJ * Kicker;			// Target Kicker
		PLAYER_OBJ * Punter;			// Target Punter
		PLAYER_OBJ * Placeholder;		// Target Placeholder


		FORMATION	Formation;
		PLAY		Play;
	};



#endif