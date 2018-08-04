#include <windows.h>
#include "BeginGameStage.h"
#include "Football.h"



void BEGIN_GAME_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////


	OutputDebugString("OPENED :: BEGIN GAME STAGE\r\n");
	}



void BEGIN_GAME_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void BEGIN_GAME_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
			
	
	// Open the Coin Toss stage 

	MainGame.OpenCoinTossStage();
	}







