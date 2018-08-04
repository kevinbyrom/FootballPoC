#ifndef __TEST_H
#define __TEST_H

#include "Header\Gamelib.h"

class TESTGAME : public GAME
	{
	public:

		virtual void Opening();
	};


class TESTSTAGE : public STAGE
	{
	public:

		virtual void Opening();
		virtual void Loop();
	};


#endif