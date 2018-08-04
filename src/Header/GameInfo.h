#ifndef __GAME_INFO_H
#define __GAME_INFO_H


#include "Constants.h"


const int PLAY_TYPE_NORMAL	= 0;
const int PLAY_TYPE_KICKOFF	= 1;
const int PLAY_TYPE_PAT		= 2;

const int SHOW_NONE				= 0;
const int SHOW_FIRSTDOWN		= 1;
const int SHOW_TURNOVER			= 2;
const int SHOW_TOUCHDOWN		= 3;
const int SHOW_SACK				= 4;
const int SHOW_PATGOOD			= 5;
const int SHOW_PATNOGOOD		= 6;
const int SHOW_FIELDGOALGOOD	= 7;
const int SHOW_FIELDGOALNOGOOD	= 8;



struct CLOCK
	{
	int Minutes;
	int Seconds;
	};



class GAME_INFO
	{
	public:

		void SetupNewGame();

		int PlayDone();

		BOOL CheckForFirstDown();
		void FirstDown();
		void IncrementDown();
		void SetYardsToGo();

		void Touchdown();
		void PAT1Good();
		void PAT2Good();
		void PATNoGood();
		void FieldGoalGood();
		void FieldGoalNoGood();

		void ChangePossession();


	public:

		int Score[NUM_SIDES];
		int Direction[NUM_SIDES];

		int Possession;				// 0 = HOME		1 = AWAY

		int PlayType;

		CLOCK Clock;
		BOOL ClockCounting;

		int Quater;

		//GAME_STATS Stats;
	};



#endif