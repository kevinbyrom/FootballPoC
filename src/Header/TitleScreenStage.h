#ifndef __TITLESCREENSTAGE_H
#define __TITLESCREENSTAGE_H



#include "Gamelib.h"



class TITLE_SCREEN_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif