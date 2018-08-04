#include <windows.h>
#include "FirstDownStage.h"
#include "Football.h"



void FIRST_DOWN_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////
	}



void FIRST_DOWN_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////
	}



void FIRST_DOWN_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////

	Screen->GetBack()->Fill(16);
	Font.DrawText(Screen->GetBack(), "FIRST DOWN!!!!!!", 0, 0);

	if (Input->GetKey(DIK_K))
		MainGame.OpenCheckTimeStage();
	}







