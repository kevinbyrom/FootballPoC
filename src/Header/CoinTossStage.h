#ifndef __COINTOSSSTAGE_H
#define __COINTOSSSTAGE_H



#include "Gamelib.h"



class COIN_TOSS_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif