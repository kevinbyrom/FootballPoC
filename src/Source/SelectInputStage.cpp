#include <windows.h>
#include "SelectInputStage.h"
#include "Football.h"



void SELECT_INPUT_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////
	}



void SELECT_INPUT_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void SELECT_INPUT_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
				

	// Open the Begin Game stage
	
	
	Team[HOME].ControlledBy = 0;
	Team[AWAY].ControlledBy = 1;


	MainGame.OpenBeginGameStage();
	}







