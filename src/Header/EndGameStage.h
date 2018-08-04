#ifndef __ENDGAMESTAGE_H
#define __ENDGAMESTAGE_H



#include "Gamelib.h"



class END_GAME_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif