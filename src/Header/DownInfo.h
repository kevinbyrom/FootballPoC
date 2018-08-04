#ifndef __DOWN_INFO_H
#define __DOWN_INFO_H


const int DOWN_DONE_OUTOFBOUNDS		= 0;
const int DOWN_DONE_INCOMPLETEPASS	= 1;
const int DOWN_DONE_TACKLED			= 2;
const int DOWN_DONE_SACKED			= 3;
const int DOWN_DONE_TOUCHDOWN		= 4;
const int DOWN_DONE_PATGOOD			= 5;
const int DOWN_DONE_PATNOGOOD		= 6;
const int DOWN_DONE_FIELDGOALGOOD	= 7;
const int DOWN_DONE_FIELDGOALNOGOOD	= 8;

const int DOWN_MAX_AIM_RANGE		= 40;



struct KICK_AIM
	{
	BOOL			Active;
	COORDINATE		Scr;
	COORDINATE_F	Wld;		
	COORDINATE_F	Origin;
	float			Delta;
	};



class DOWN_INFO
	{
	public:

		BOOL Done;
		int DoneReason;

		int Down;
		int YardsToGo;

		int OffenseDir;
		int DefenseDir;

		float ScrimmagePos;
		float FirstDownPos;
		float BallSpotPos;

		BOOL BallHiked;
		BOOL BallPassed;
		BOOL PassedScrimmage;

		BOOL Turnover;

		KICK_AIM KickAim;

		int CurrPlay[NUM_SIDES];
	};



#endif