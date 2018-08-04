#ifndef __SELECTGAMEMODESTAGE_H
#define __SELECTGAMEMODESTAGE_H



#include "Gamelib.h"



class SELECT_GAME_MODE_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif