#include <windows.h>
#include "CoinTossStage.h"
#include "Football.h"



void COIN_TOSS_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////


	OutputDebugString("OPENED :: COIN TOSS STAGE\r\n");
	}



void COIN_TOSS_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void COIN_TOSS_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////

	
	// Open the Select Play stage

	MainGame.OpenSelectPlayStage();
	}







