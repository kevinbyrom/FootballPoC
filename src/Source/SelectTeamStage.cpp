#include <windows.h>
#include "SelectTeamStage.h"
#include "Football.h"



void SELECT_TEAM_STAGE::Opening()
	{

	//////////////////////////
	//						//
	// Opening of the stage //
	//						//
	//////////////////////////


	OutputDebugString("OPENED :: SELECT TEAM STAGE\r\n");


	// Load the teams from the teams file

	LoadTeams();

	SelectMode = SELECT_HOME;


//DEBUG ONLY
		STRING teamdir;
	
		teamdir = strGameDir;
		teamdir += "EXHIBITION\\";

		Team[HOME].Load(teamdir, Teams[TeamNum[HOME]].Filename, TRUE);
		Team[AWAY].Load(teamdir, Teams[TeamNum[AWAY]].Filename, FALSE);

		GameInfo.SetupNewGame();
		MainGame.OpenSelectInputStage();

	}





void SELECT_TEAM_STAGE::Closing(STAGE * Stage)
	{

	//////////////////////////
	//						//
	// Closing of the stage //
	//						//
	//////////////////////////


	}





void SELECT_TEAM_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////

	
	// Setup the new game 

	//GameInfo.SetupNewGame();


	// Open the Select Input stage 

	//MainGame.OpenSelectInputStage();


	// Check for team changes

	if (VPad[0].LeftClicked() || VPad[1].LeftClicked())
		TeamNum[SelectMode]--;
	else if (VPad[0].RightClicked() || VPad[1].RightClicked())
		TeamNum[SelectMode]++;


	if (VPad[0].UpClicked() || VPad[1].UpClicked())
		SelectMode = SELECT_HOME;
	else if (VPad[0].DownClicked() || VPad[1].DownClicked())
		SelectMode = SELECT_AWAY;


	// Verify the team nums are in range

	for (int i = 0; i < MAX_TEAMS; i++)
		{
		if (TeamNum[i] < 0)			TeamNum[i] = NumTeams - 1;
		if (TeamNum[i] >= NumTeams)	TeamNum[i] = 0;
		}

	
		

	// If enter is pressed, then move on to the next stage

	if (VPad[0].Start() || VPad[1].Start())
		{
		STRING teamdir;
	
		teamdir = strGameDir;
		teamdir += "EXHIBITION\\";

		Team[HOME].Load(teamdir, Teams[TeamNum[HOME]].Filename, TRUE);
		Team[AWAY].Load(teamdir, Teams[TeamNum[AWAY]].Filename, FALSE);

		GameInfo.SetupNewGame();
		MainGame.OpenSelectInputStage();
		}


	Draw();
	}




void SELECT_TEAM_STAGE::Draw()
	{

	/////////////////////////////
	//						   //
	// Used to draw the screen //
	//						   //
	/////////////////////////////


	Screen->GetBack()->Fill(0);


	if (TeamNum[HOME] < NumTeams)
		Font.DrawText(Screen->GetBack(), Teams[TeamNum[HOME]].Name, 0, 0);
	else
		Font.DrawText(Screen->GetBack(), "(NONE)", 0, 0);

	if (TeamNum[AWAY] < NumTeams)
		Font.DrawText(Screen->GetBack(), Teams[TeamNum[AWAY]].Name, 0, 20);
	else
		Font.DrawText(Screen->GetBack(), "(NONE)", 0, 20);

	}



void SELECT_TEAM_STAGE::LoadTeams()
	{

	////////////////////////////////////////////////
	//											  //
	// Used to load the teams from the teams file //
	//											  //
	////////////////////////////////////////////////


	GAME_FILE file;
	STRING readstring;
	STRING_ARRAY params;


	NumTeams		= 0;
	TeamNum[HOME]	= 0;
	TeamNum[AWAY]	= 0;//1;


	// Attempt to open the team file 

	if (!file.OpenForReading("C:\\GAME SOURCE\\NEW FOOTBALL\\EXHIBITION\\TEAMS\\TEAMS.TXT"))
		{
		OutputDebugString("ERROR :: OPENING TEAMS FILE\r\n");
		return;
		}



	// For each team, read the team name and team file name

	NumTeams = (int)file.ReadLine(TRUE);


	for (int i = 0; i < NumTeams; i++)
		{
		params = file.ReadParams();

		if (params.Count() < 2)
			{
			OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR TEAM IN TEAMS FILE\r\n");
			return;
			}


		Teams[i].Name		= params[0];
		Teams[i].Filename	= params[1];
		}
	}


