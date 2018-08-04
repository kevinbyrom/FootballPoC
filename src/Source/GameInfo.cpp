#include <windows.h>
#include "GameInfo.h"
#include "Football.h"



void GAME_INFO::SetupNewGame()
	{

	//////////////////////////////
	//							//
	// Used to setup a new game //
	//							//
	//////////////////////////////

	
	// Default the scores to zero

	Score[HOME]		= 0;
	Score[AWAY]		= 0;


	// Set the default directions 

	Direction[HOME]	= DIR_RIGHT;
	Direction[AWAY] = DIR_LEFT;


	// Set the default possession

	Possession		= HOME;


	// Set the quarter and clock time

	Quater			= 1;	
	Clock.Minutes	= 5;
	Clock.Seconds	= 0;
	ClockCounting	= FALSE;
	}




int GAME_INFO::PlayDone()
	{
	BOOL ShowTurnover			= FALSE;
	BOOL ShowFirstdown			= FALSE;
	BOOL ShowTouchdown			= FALSE;
	BOOL ShowSack				= FALSE;
	BOOL ShowPATGood			= FALSE;
	BOOL ShowPATNoGood			= FALSE;
	BOOL ShowFieldGoalGood		= FALSE;
	BOOL ShowFieldGoalNoGood	= FALSE;


	DownInfo.ScrimmagePos = DownInfo.BallSpotPos;


	if (CheckForFirstDown())
		{
		FirstDown();
		ShowFirstdown = TRUE;
		}
	else
		{
		IncrementDown();
		SetYardsToGo();
		}


	// Change possession on turnovers 

	if (DownInfo.Turnover)	
		{
		ChangePossession();
		ShowTurnover = TRUE;
		}

	
	switch(DownInfo.DoneReason)
		{
		case DOWN_DONE_OUTOFBOUNDS:
			break;


		case DOWN_DONE_INCOMPLETEPASS:
			break;


		case DOWN_DONE_TACKLED:
			break;


		case DOWN_DONE_SACKED:
			ShowSack = TRUE;
			break;


		case DOWN_DONE_TOUCHDOWN:
			Touchdown();
			ShowTouchdown = TRUE;
			break;


		case DOWN_DONE_PATGOOD:
			PAT1Good();
			ShowPATGood = TRUE;
			break;


		case DOWN_DONE_PATNOGOOD:
			PATNoGood();
			ShowPATNoGood = TRUE;
			break;


		case DOWN_DONE_FIELDGOALGOOD:
			FieldGoalGood();
			ShowFieldGoalGood = TRUE;
			break;


		case DOWN_DONE_FIELDGOALNOGOOD:
			FieldGoalNoGood();
			ShowFieldGoalGood = TRUE;
			break;
		}


	if (DownInfo.Down > 4)
		{
		ChangePossession();
		ShowTurnover = TRUE;
		}


	if (ShowTouchdown)			return SHOW_TOUCHDOWN;
	if (ShowFirstdown)			return SHOW_FIRSTDOWN;
	if (ShowTurnover)			return SHOW_TURNOVER;
	if (ShowSack)				return SHOW_SACK;
	if (ShowPATGood)			return SHOW_PATGOOD;
	if (ShowPATNoGood)			return SHOW_PATNOGOOD;
	if (ShowFieldGoalGood)		return SHOW_FIELDGOALGOOD;
	if (ShowFieldGoalNoGood)	return SHOW_FIELDGOALNOGOOD;
	
	return SHOW_NONE;
	}





BOOL GAME_INFO::CheckForFirstDown()
	{
	
	///////////////////////////////////////////
	//										 //
	// Used to check if a first down was met //
	//										 //
	///////////////////////////////////////////


	if (Direction[Possession] == DIR_LEFT)
		{
		if (DownInfo.BallSpotPos <= DownInfo.FirstDownPos)
			return TRUE;
		}
	else
		{
		if (DownInfo.BallSpotPos >= DownInfo.FirstDownPos)
			return TRUE;
		}


	return FALSE;
	}





void GAME_INFO::FirstDown()
	{

	/////////////////////////////////////
	//								   //
	// Used to change down to 1 and 10 //
	//								   //
	/////////////////////////////////////


	DownInfo.Down			= 1;
	DownInfo.YardsToGo		= 10;
	DownInfo.FirstDownPos	= Direction[Possession] == DIR_LEFT ? DownInfo.ScrimmagePos - (YARD_DISTANCE * 10) : DownInfo.ScrimmagePos + (YARD_DISTANCE * 10);
	}




void GAME_INFO::IncrementDown()
	{

	////////////////////////////////
	//							  //
	// Used to increment the down //
	//							  //
	////////////////////////////////

	DownInfo.Down++;
	}





void GAME_INFO::SetYardsToGo()
	{

	/////////////////////////////////
	//							   //
	// Used to set the yards to go //
	//							   //
	/////////////////////////////////


	DownInfo.YardsToGo = int(abs(DownInfo.FirstDownPos - DownInfo.ScrimmagePos) / YARD_DISTANCE);
	}




void GAME_INFO::ChangePossession()
	{

	///////////////////////////////
	//							 //
	// Used to change possession //
	//							 //
	///////////////////////////////


	Possession = !Possession;

	DownInfo.ScrimmagePos	= DownInfo.BallSpotPos;
	DownInfo.BallSpotPos	= DownInfo.ScrimmagePos;
	
	FirstDown();
	}








void GAME_INFO::Touchdown()
	{

	////////////////////////////////
	//							  //
	// Used to handle a touchdown //
	//							  //
	////////////////////////////////


	// Increment the score for the offensive team

	Score[Possession] += 6;


	// Set the new scrimmage position for the extra point 

	DownInfo.ScrimmagePos	= Direction[Possession] == DIR_LEFT ? -PAT_POS : PAT_POS;
	DownInfo.FirstDownPos	= DownInfo.ScrimmagePos;
	DownInfo.Down			= -1;
	DownInfo.YardsToGo		= -1;


	// Set the next play type

	PlayType = PLAY_TYPE_PAT;
	}





void GAME_INFO::PAT1Good()
	{

	//////////////////////////////
	//							//
	// Used to handle PAT1 good //
	//							//
	//////////////////////////////


	// Increment the score by 1

	Score[Possession] += 1;


	// Set the new scrimmage position for the kickoff

	DownInfo.ScrimmagePos	= Direction[Possession] == DIR_LEFT ? -KICKOFF_POS : KICKOFF_POS;
	DownInfo.FirstDownPos	= DownInfo.ScrimmagePos;
	DownInfo.Down			= -1;
	DownInfo.YardsToGo		= -1;


	// Set the next play type

	PlayType = PLAY_TYPE_KICKOFF;
	}




void GAME_INFO::PAT2Good()
	{

	//////////////////////////////
	//							//
	// Used to handle PAT2 good //
	//							//
	//////////////////////////////


	// Increment the score by 2

	Score[Possession] += 2;


	// Set the new scrimmage position for the kickoff

	DownInfo.ScrimmagePos	= Direction[Possession] == DIR_LEFT ? -KICKOFF_POS : KICKOFF_POS;
	DownInfo.FirstDownPos	= DownInfo.ScrimmagePos;
	DownInfo.Down			= -1;
	DownInfo.YardsToGo		= -1;


	// Set the next play type

	PlayType = PLAY_TYPE_KICKOFF;
	}




void GAME_INFO::PATNoGood()
	{

	////////////////////////////////
	//							  //
	// Used to handle PAT no good //
	//						 	  //
	////////////////////////////////


	// Set the new scrimmage position for the kickoff

	DownInfo.ScrimmagePos	= Direction[Possession] == DIR_LEFT ? -KICKOFF_POS : KICKOFF_POS;
	DownInfo.FirstDownPos	= DownInfo.ScrimmagePos;
	DownInfo.Down			= -1;
	DownInfo.YardsToGo		= -1;


	// Set the next play type

	PlayType = PLAY_TYPE_KICKOFF;
	}




void GAME_INFO::FieldGoalGood()
	{

	////////////////////////////////
	//							  //
	// Used to handle field goals //
	//							  //
	////////////////////////////////


	// Increment the score by 3

	Score[Possession] += 3;


	// Set the new scrimmage position for the kickoff

	DownInfo.ScrimmagePos	= Direction[Possession] == DIR_LEFT ? -KICKOFF_POS : KICKOFF_POS;
	DownInfo.FirstDownPos	= DownInfo.ScrimmagePos;
	DownInfo.Down			= -1;
	DownInfo.YardsToGo		= -1;

	
	// Set the next play type

	PlayType = PLAY_TYPE_KICKOFF;
	}




void GAME_INFO::FieldGoalNoGood()
	{

	//////////////////////////////////////
	//									//
	// Used to handle field goal misses //
	//									//
	//////////////////////////////////////


	// Change the possession

	ChangePossession();


	// Set the next play type

	PlayType = PLAY_TYPE_NORMAL;
	}




