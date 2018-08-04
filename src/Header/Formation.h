#ifndef __FORMATION_H
#define __FORMATION_H


#include "Gamelib.h"
#include "Constants.h"


class FORMATION
	{
	public:

		BOOL Load(STRING strName);


	public:

		STRING Name;
		STRING GraphicFilename;

		int DepthType[MAX_SIDE_PLAYERS];
		int DepthPos[MAX_SIDE_PLAYERS];

		float OffsetWX[MAX_SIDE_PLAYERS];
		float OffsetWY[MAX_SIDE_PLAYERS];

		BOOL IsSpecialTeams;
		BOOL IsKickoff;
	};



#endif