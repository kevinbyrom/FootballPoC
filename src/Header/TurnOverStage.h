#ifndef __TURNOVERSTAGE_H
#define __TURNOVERSTAGE_H



#include "Gamelib.h"



class TURNOVER_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();
	};



#endif