#include <windows.h>
#include "SelectPlayStage.h"
#include "Football.h"

 


void SELECT_PLAY_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////


	OutputDebugString("OPENED :: SELECT PLAY STAGE\r\n");


	PlaySel[OFFENSE].Init(OFFENSE);
	PlaySel[DEFENSE].Init(DEFENSE);

	// DEBUG ONLY
	PlaySel[OFFENSE].SlotSelect(3);
	PlaySel[OFFENSE].SlotSelect(0);
	PlaySel[DEFENSE].SlotSelect(0);
	PlaySel[DEFENSE].SlotSelect(0);
	MainGame.OpenPlayStage();
	}





void SELECT_PLAY_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////


	PlaySel[OFFENSE].DeInit();
	PlaySel[DEFENSE].DeInit();
	}





void SELECT_PLAY_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
				

	VPAD * pVPad[NUM_SIDES];
	int i;


	// Draw the screen

	Draw();



	// Assign the vpad pointers

	pVPad[OFFENSE] = &VPad[Team[GameInfo.Possession].ControlledBy];
	pVPad[DEFENSE] = &VPad[Team[!GameInfo.Possession].ControlledBy];


	// Get input states

	for (i = 0; i < NUM_SIDES; i++)
		{

		if (pVPad[i]->UpClicked())
			PlaySel[i].SlotPageUp();

		else if (pVPad[i]->DownClicked())
			PlaySel[i].SlotPageDown();

		else if (pVPad[i]->Left() && pVPad[i]->ButtonClicked(0))
			PlaySel[i].SlotSelect(SELECT_SLOT1);

		else if (pVPad[i]->Right() && pVPad[i]->ButtonClicked(0))
			PlaySel[i].SlotSelect(SELECT_SLOT3);

		else if (pVPad[i]->ButtonClicked(0))
			PlaySel[i].SlotSelect(SELECT_SLOT2);

		else if (pVPad[i]->ButtonClicked(1))
			PlaySel[i].SlotPrevious();
		}



	// If both teams have selected, open the Play stage 

	if (PlaySel[OFFENSE].IsDoneSelecting() && PlaySel[DEFENSE].IsDoneSelecting())
		MainGame.OpenPlayStage();
	}














void SELECT_PLAY_STAGE::Draw()
	{

	////////////////////////////
	//						  //
	// Used to draw the stage //
	//						  //
	////////////////////////////


	Screen->GetBack()->Fill(0);


	DrawSelectSlots(OFFENSE, 0, 0);
	DrawSelectSlots(DEFENSE, 0, 200);
	}





void SELECT_PLAY_STAGE::DrawSelectSlots(int side, int x, int y)
	{


	/////////////////////////////////
	//							   //
	// Used to draw the play slots //
	//							   //
	/////////////////////////////////


	for (int i = 0; i < 3; i++)
		{
		Font.DrawText(Screen->GetBack(), PlaySel[side].GetSlotName(i), x + (i * 120), y);
		Screen->GetBack()->BlitFast(PlaySel[side].GetSlotSurface(i), x + (i * 120), y + 20);
		}
	}










void PLAY_SELECT_ITEM::Init(int side)
	{

	/////////////////////////////////////////////
	//										   //
	// Used to initialize the play select item //
	//										   //
	/////////////////////////////////////////////


	SetTeamAndPlaybookPointers(side);
	
	SetKickoffAndSpecialTeamsFormations();

	InitSelectInfo();
	
	FillSelectSlots();

	// If the play type is KICKOFF, default select the Kickoff formation
	// If the play type is PAT or 4th Down, default select the SPECIAL TEAMS formation
	
	if (SelectInfo.Type == SELECT_FORMATION)
		LoadFormationSurfs();
	else if (SelectInfo.Type == SELECT_PLAY)
		LoadPlaySurfs();
	}






void PLAY_SELECT_ITEM::DeInit()
	{

	///////////////////////////////////////////////
	//											 //
	// Used to deinitialize the play select item //
	//											 //
	///////////////////////////////////////////////


	UnloadFormationSurfs();
	UnloadPlaySurfs();
	}






void PLAY_SELECT_ITEM::SetTeamAndPlaybookPointers(int side)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to set the team and playbook pointers for later use //
	//															//
	//////////////////////////////////////////////////////////////


	if (side == OFFENSE)
		pTeam = &Team[GameInfo.Possession];
	else
		pTeam = &Team[!GameInfo.Possession];

	pPlaybook = &pTeam->Playbook[side];
	}






void PLAY_SELECT_ITEM::SetKickoffAndSpecialTeamsFormations()
	{

	////////////////////////////////////////////////////////////////////////
	//																	  //
	// Used to set the kickoff and special teams formations for later use //
	//																	  //
	////////////////////////////////////////////////////////////////////////


	for (int f = 0; f < pPlaybook->NumFormations; f++)
		{
		if (pPlaybook->Formations[f].IsKickoff)
			KickoffForm = f;

		if (pPlaybook->Formations[f].IsSpecialTeams)
			SpecialTeamsForm = f;
		}
	}





void PLAY_SELECT_ITEM::InitSelectInfo()
	{

	/////////////////////////////////////
	//								   //
	// Used to init the selection info //
	//								   //
	/////////////////////////////////////


	switch(GameInfo.PlayType)
		{
		case PLAY_TYPE_NORMAL:
			SelectInfo.Type			= SELECT_FORMATION;
			SelectInfo.Pos			= 0;
			SelectInfo.Formation	= 0;
			SelectInfo.Play			= 0;
			break;

		case PLAY_TYPE_KICKOFF:
			SelectInfo.Type			= SELECT_PLAY;
			SelectInfo.Pos			= 0;
			SelectInfo.Formation	= KickoffForm;
			SelectInfo.Play			= 0;
			break;

		case PLAY_TYPE_PAT:
			SelectInfo.Type			= SELECT_PLAY;
			SelectInfo.Pos			= 0;
			SelectInfo.Formation	= SpecialTeamsForm;
			SelectInfo.Play			= 0;
			break;
		}


	pTeam->SelectedFormation = 0;
	pTeam->SelectedPlay		 = 0;
	}






void PLAY_SELECT_ITEM::FillSelectSlots()
	{

	////////////////////////////////////////////////////////////////////////
	//																	  //
	// Used to fill the select slots with either formation or play values //
	//																	  //
	////////////////////////////////////////////////////////////////////////


	int i;
	int CurrFormation;


	
	switch(SelectInfo.Type)
		{
		
		// If selecting a formation, fill in three slots from the formations

		case SELECT_FORMATION:
		
			NumSelectSlots = (int)(pPlaybook->NumFormations / NUM_DISPLAY_SLOTS) * NUM_DISPLAY_SLOTS;

			if (NumSelectSlots < pPlaybook->NumFormations)
				NumSelectSlots += NUM_DISPLAY_SLOTS;

			for (i = 0; i < NumSelectSlots; i++)
				{
				SelectSlots[i].Name = (i < pPlaybook->NumFormations) ? pPlaybook->Formations[i].Name : pPlaybook->Formations[pPlaybook->NumFormations - 1].Name;
				SelectSlots[i].Pos = (i < pPlaybook->NumFormations) ? i : pPlaybook->NumFormations - 1;
				}

			break;


		// If selecting a play, fill in three slots from the plays

		case SELECT_PLAY:
		
			CurrFormation = SelectInfo.Formation;


			NumSelectSlots = (int)(pPlaybook->NumPlays[CurrFormation] / NUM_DISPLAY_SLOTS) * NUM_DISPLAY_SLOTS;

			if (NumSelectSlots < pPlaybook->NumPlays[CurrFormation])
				NumSelectSlots += NUM_DISPLAY_SLOTS;

			for (i = 0; i < NumSelectSlots; i++)
				{
				SelectSlots[i].Name = (i < pPlaybook->NumPlays[CurrFormation]) ? pPlaybook->Plays[CurrFormation][i].Name : pPlaybook->Plays[CurrFormation][pPlaybook->NumPlays[CurrFormation] - 1].Name;
				SelectSlots[i].Pos = (i < pPlaybook->NumPlays[CurrFormation]) ? i : pPlaybook->NumPlays[CurrFormation] - 1;
				}

			break;

		default:
			NumSelectSlots = 0;
			break;
		}
	}





void PLAY_SELECT_ITEM::LoadFormationSurfs()
	{


	//////////////////////////////////////////
	//										//
	// Used to load the formations surfaces //
	//										//
	//////////////////////////////////////////


	STRING filestring;



	// Set the number of formation surfaces
	
	NumFormationSurfs = pPlaybook->NumFormations;



	// For each formation in the sides playbook

	for (int i = 0; i < NumFormationSurfs; i++)
		{

		// Get the formation graphic string and build a 
		// filename string for it
		
		filestring = strGameDir;
		filestring += "Formation Graphics\\";
		filestring += pPlaybook->Formations[i].GraphicFilename;
		filestring += ".bmp";


		// Load the surface with the filename

		FormationSurf[i].Destroy();
		FormationSurf[i].CreateFromBitmap(Screen, filestring);
		}
	}






void PLAY_SELECT_ITEM::LoadPlaySurfs()
	{

	//////////////////////////////////////////////////////
	//												    //
	// Used to load the plays surfaces for a given side //
	//													//
	//////////////////////////////////////////////////////


	STRING filestring;



	// Set the number of play surfaces

	NumPlaySurfs = pPlaybook->NumPlays[SelectInfo.Formation];



	// For each play in the sides playbook

	for (int i = 0; i < NumPlaySurfs; i++)
		{

		// Get the play graphic string and build a 
		// filename string for it
		
		filestring = strGameDir;
		filestring += "Play Graphics\\";
		filestring += pPlaybook->Plays[SelectInfo.Formation][i].GraphicFilename;
		filestring += ".bmp";


		// Load the surface with the filename

		PlaySurf[i].Destroy();
		PlaySurf[i].CreateFromBitmap(Screen, filestring);
		}
	}





void PLAY_SELECT_ITEM::UnloadFormationSurfs()
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to unload all formation surfaces from memory //
	//													 //
	///////////////////////////////////////////////////////


	for (int i = 0; i < NUM_SIDES; i++)
		for (int ii = 0; ii < MAX_FORMATIONS; ii++)
			FormationSurf[ii].Destroy();
	}





void PLAY_SELECT_ITEM::UnloadPlaySurfs()
	{

	//////////////////////////////////////////////////
	//												//
	// Used to unload all play surfaces from memory //
	//												//
	//////////////////////////////////////////////////


	for (int i = 0; i < NUM_SIDES; i++)
		for (int ii = 0; ii < MAX_PLAYS; ii++)
			PlaySurf[ii].Destroy();
	}






void PLAY_SELECT_ITEM::SlotPageUp()
	{

	///////////////////////////////
	//						     //
	// Used to page up the slots //
	//						     //
	///////////////////////////////


	SelectInfo.Pos -= NUM_DISPLAY_SLOTS;

	if (SelectInfo.Pos < 0)						SelectInfo.Pos = NumSelectSlots - NUM_DISPLAY_SLOTS;
	}





void PLAY_SELECT_ITEM::SlotPageDown()
	{

	/////////////////////////////////
	//						       //
	// Used to page down the slots //
	//						       //
	/////////////////////////////////


	SelectInfo.Pos += NUM_DISPLAY_SLOTS;

	if (SelectInfo.Pos >= NumSelectSlots)		SelectInfo.Pos = 0;
	}






void PLAY_SELECT_ITEM::SlotSelect(int index)
	{

	/////////////////////////////////////
	//								   //
	// Used to select a specified slot //
	//								   //
	/////////////////////////////////////


		// If selecting a formation, store the selected formation and move 
		// onto the SELECT PLAY mode

		switch(SelectInfo.Type)
			{
			case SELECT_FORMATION:

				SelectInfo.Formation = SelectSlots[SelectInfo.Pos + index].Pos;
				SelectInfo.Pos = 0;
				SelectInfo.Type++;

				FillSelectSlots();

				UnloadFormationSurfs();
				LoadPlaySurfs();

				break;


		// If selecting a play, set the teams selected formation and play
		// and set the select mode to DONE

			case SELECT_PLAY:

				SelectInfo.Play = SelectSlots[SelectInfo.Pos + index].Pos;
				SelectInfo.Pos = 0;
				SelectInfo.Type++;				

				UnloadPlaySurfs();

				FinishSelecting();
				break;
			}

	}





void PLAY_SELECT_ITEM::SlotPrevious()
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to move to the prevoius selection type //
	//											   //
	/////////////////////////////////////////////////


	// If the current play type is not Kickoff and the player is in SELECT PLAY mode
	// then go back to SELECT FORMATION.  In kickoffs, the plays must be picked from
	// the kickoff formations

	if (GameInfo.PlayType != PLAY_TYPE_KICKOFF && SelectInfo.Type != SELECT_FORMATION)
		{
		SelectInfo.Type--;

		FillSelectSlots();

		UnloadPlaySurfs();
		LoadFormationSurfs();
		}
	}





void PLAY_SELECT_ITEM::FinishSelecting()
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to finish the selecting by setting the team //
	// selected formation and play						//
	//													//
	//////////////////////////////////////////////////////


	pTeam->SelectedFormation = SelectInfo.Formation;
	pTeam->SelectedPlay		= SelectInfo.Play;
	}





STRING PLAY_SELECT_ITEM::GetSlotName(int index)
	{


	/////////////////////////////////////////////////
	//											   //
	// Used to get a string to the slot name based //
	// on the given slot position				   //
	//											   //
	/////////////////////////////////////////////////


	return SelectSlots[index + SelectInfo.Pos].Name;
	}





SURFACE * PLAY_SELECT_ITEM::GetSlotSurface(int index)
	{

	//////////////////////////////////////////////
	//											//
	// Used to get a pointer to a surface based //
	// on the given slot position				//
	//											//
	//////////////////////////////////////////////

	
	SURFACE * pSurf = NULL;


	switch(SelectInfo.Type)
		{
		case SELECT_FORMATION:
			pSurf = &FormationSurf[SelectSlots[index + SelectInfo.Pos].Pos];
			break;

		case SELECT_PLAY:
			pSurf = &PlaySurf[SelectSlots[index + SelectInfo.Pos].Pos];
			break;
		}

	return pSurf;
	}