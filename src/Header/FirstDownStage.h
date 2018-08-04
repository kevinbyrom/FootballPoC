#ifndef __FIRSTDOWNSTAGE_H
#define __FIRSTDOWNSTAGE_H



#include "Gamelib.h"



class FIRST_DOWN_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif