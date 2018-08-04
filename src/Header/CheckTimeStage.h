#ifndef __CHECKTIMESTAGE_H
#define __CHECKTIMESTAGE_H



#include "Gamelib.h"



class CHECK_TIME_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif