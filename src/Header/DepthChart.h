#ifndef __DEPTHCHART_H
#define __DEPTHCHART_H


#include "Gamelib.h"
#include "Constants.h"


class DEPTH_CHART
	{
	public:
		
		int GetRosterPos(int type, int slot);

		BOOL Load(STRING strDirectory, STRING strTeam);
	

	public:

		int NumSlotsFilled[NUM_DEPTH_TYPES];
		int RosterPos[NUM_DEPTH_TYPES][NUM_DEPTH_SLOTS];
	};



#endif