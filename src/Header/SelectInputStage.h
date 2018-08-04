#ifndef __SELECTINPUTSTAGE_H
#define __SELECTINPUTSTAGE_H



#include "Gamelib.h"



class SELECT_INPUT_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif