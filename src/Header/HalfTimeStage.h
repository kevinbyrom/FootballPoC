#ifndef __HALFTIMESTAGE_H
#define __HALFTIMESTAGE_H



#include "Gamelib.h"



class HALF_TIME_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif