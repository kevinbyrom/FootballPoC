#include <windows.h>
#include "SelectGameModeStage.h"
#include "Football.h"



void SELECT_GAME_MODE_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////


	OutputDebugString("OPENED :: SELECT GAME MODE STAGE\r\n");
	}



void SELECT_GAME_MODE_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void SELECT_GAME_MODE_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////

	
	// Open the Select Team stage 

	MainGame.OpenSelectTeamStage();
	}







