#include <windows.h>
#include "TouchdownStage.h"
#include "Football.h"



void TOUCHDOWN_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////
	}



void TOUCHDOWN_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void TOUCHDOWN_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
		
	Screen->GetBack()->Fill(16);
	Font.DrawText(Screen->GetBack(), "TOUCHDOWN!!!!!!", 0, 0);

	if (Input->GetKey(DIK_K))
		MainGame.OpenCheckTimeStage();
	}
 






