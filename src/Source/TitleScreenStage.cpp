#include <windows.h>
#include "TitleScreenStage.h"
#include "Football.h"



void TITLE_SCREEN_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////


	OutputDebugString("OPENED :: TITLE SCREEN STAGE\r\n");
	}



void TITLE_SCREEN_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void TITLE_SCREEN_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
	
				
	// Open the Select Game Mode stage 

	GameInfo.SetupNewGame();
	DownInfo.ScrimmagePos = 0;
	DownInfo.FirstDownPos = YARD_DISTANCE * 10;
	DownInfo.Down = 1;
	DownInfo.YardsToGo = 10;
	
	MainGame.OpenSelectGameModeStage();
	}







