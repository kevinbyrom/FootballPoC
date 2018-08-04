#ifndef __TOUCHDOWNSTAGE_H
#define __TOUCHDOWNSTAGE_H



#include "Gamelib.h"



class TOUCHDOWN_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif