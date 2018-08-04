#ifndef __BEGINGAMESTAGE_H
#define __BEGINGAMESTAGE_H



#include "Gamelib.h"



class BEGIN_GAME_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif