#include <windows.h>
#include "Side.h"



SIDE::SIDE()
	{
	Control		= NULL;
	TargetRec	= NULL;

	for (int i = 0; i < MAX_SIDE_PLAYERS; i++)
		Player[i].Init();
	}




void SIDE::Clear()
	{

	/////////////////////////////////
	//							   //
	// Used to clear the side info //
	//							   //
	/////////////////////////////////


	Control		= NULL;
	TargetRec	= NULL;
	Passer		= NULL;
	Rusher		= NULL;
	Kicker		= NULL;
	Punter		= NULL;
	Placeholder	= NULL;
	}




void SIDE::Update()
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to call the update function for all of the players //
	//														   //
	/////////////////////////////////////////////////////////////


	for (int i = 0; i < MAX_SIDE_PLAYERS; i++)
		Player[i].Update();
	}




void SIDE::ChangeUserControl(PLAYER_OBJ * player)
	{

	//////////////////////////////////////////////////////////
	//														//
	// Used to change the current player that is controlled //
	// by user input										//
	//														//
	//////////////////////////////////////////////////////////


	// Set the previous player's control type to NONE 

	if (Control != NULL)
		{
		Control->ControlType = CONTROL_NONE;
		Control->InputEnabled = FALSE;
		}


	// Set the pointer to the new player 

	Control = player;


	// Set the player's control type to USER 

	if (Control != NULL)
		{
		Control->ControlType = CONTROL_USER;
		Control->InputEnabled = TRUE;
		}
	}



void SIDE::ChangeTargetReceiver()
	{

	////////////////////////////////////////
	//									  //
	// Used to change the target receiver //
	//									  //
	////////////////////////////////////////


	int i;
	int first = -1;
	int next = -1;
	BOOL playerfound = FALSE;


	for (i = 0; i < MAX_SIDE_PLAYERS; i++)
		{
		if (first == -1 && Player[i].Flag.Receiver)
			first = i;

		if (next == -1 && playerfound && Player[i].Flag.Receiver)
			next = i;

		if (TargetRec == &Player[i])
			playerfound = TRUE;
		}

	if (next != -1)
		TargetRec = &Player[next];
	else if (first != -1)
		TargetRec = &Player[first];
	else
		TargetRec = NULL;
	}