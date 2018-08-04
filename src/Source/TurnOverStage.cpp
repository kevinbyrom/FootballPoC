#include <windows.h>
#include "TurnOverStage.h"
#include "Football.h"



void TURNOVER_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////
	}



void TURNOVER_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void TURNOVER_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
			
	Screen->GetBack()->Fill(16);
	Font.DrawText(Screen->GetBack(), "TURNOVER!!!!!!", 0, 0);

	if (Input->GetKey(DIK_K))
		MainGame.OpenCheckTimeStage();
	}







