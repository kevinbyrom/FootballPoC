#include <windows.h>
#include <stdio.h>
#include "Team.h"
#include "Gamelib.h"
#include "StrUtils.h"
#include "Football.h"




PLAYER_INFO * TEAM::GetAvailablePlayerFromType(int type, int pos)
	{

	////////////////////////////////////////////////////////////////////////////
	//																		  //
	// Used to get the next available player (one not already in the play) by //
	// his formation type													  //
	//																		  //
	////////////////////////////////////////////////////////////////////////////


	int s, t;
	PLAYER_INFO * playerinfo;

	for (t = type; t < NUM_DEPTH_TYPES; t++)
		for (s = pos; s < NUM_DEPTH_SLOTS; s++)
			{
			playerinfo = Roster.GetPlayerInfo(DepthChart.GetRosterPos(type, s));

			if (playerinfo != NULL && !playerinfo->InPlay && !playerinfo->Injured)
				{
				playerinfo->InPlay = TRUE;
				return playerinfo;
				}
			}

	if (type != 0)
		for (t = 0; t < type; t++)
			for (s = 0; s < NUM_DEPTH_SLOTS; s++)
				{
				playerinfo = Roster.GetPlayerInfo(DepthChart.GetRosterPos(type, s));

				if (playerinfo != NULL && !playerinfo->InPlay && !playerinfo->Injured)
					{
					playerinfo->InPlay = TRUE;
					return playerinfo;
					}
				}

	return NULL;
	}





void TEAM::RemovePlayersFromPlay()
	{

	//////////////////////////////////////////////
	//											//
	// Used to remove all players from the play //
	//											//
	//////////////////////////////////////////////


	Roster.RemoveAllFromPlay();
	}





FORMATION * TEAM::GetSelectedFormation(int type)
	{

	/////////////////////////////////////////
	//									   //
	// Used to get the currently selected  //
	// formation for a given playbook type //
	//									   //
	/////////////////////////////////////////


	return &Playbook[type].Formations[SelectedFormation]; 
	}





PLAY * TEAM::GetSelectedPlay(int type)	
	{ 

	////////////////////////////////////////
	//									  //
	// Used to get the currently selected // 
	// play for a given playbook type	  //
	//									  //
	////////////////////////////////////////


	return &Playbook[type].Plays[SelectedFormation][SelectedPlay]; 
	}





BOOL TEAM::Load(STRING strDirectory, STRING strTeam, BOOL hometeam)
	{

	///////////////////////////////////	
	//								 //
	// Used to load a team from file //
	//								 //
	///////////////////////////////////	


	GAME_FILE file;
	STRING readstring;
	STRING filestring;
	BOOL retval = TRUE;


	
	// Attempt to open the file 

	OutputDebugString("TEAM LOADING\r\n");



	// First attempt to open the file, on error return false 

	filestring = strDirectory;
	filestring += "TEAMS\\";
	filestring += strTeam;
	filestring += ".txt";


	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING TEAM FILE\r\n");
		return FALSE;
		}



	// Read the team name 

	Name = file.ReadLine(TRUE);



	// Read the city

	City = file.ReadLine(TRUE);



	// Read the state

	State = file.ReadLine(TRUE);




	// Load the player gfx file 

	filestring = strGameDir;

	if (hometeam)
		filestring += "PlayerSprites\\Home\\";
	else
		filestring += "PlayerSprites\\Away\\";

	filestring += strTeam;
	filestring += ".bmp";
		
	PlayerSpr.CreateFromBitmap(Screen, filestring, NUM_SPRTILES_X, NUM_SPRTILES_Y, 44, 44, 1, 255);

	if (hometeam)
		Screen->SetPalette(PlayerSpr.Pal, 0, 256);



	// Load the field gfx file (home team only)

	filestring = strGameDir;
	filestring += "FieldSprites\\";
	filestring += strTeam;
	filestring += ".bmp";

	if (hometeam)
		FieldSpr.CreateFromBitmap(Screen, filestring, 6, 8, 20, 20, 1, 255);



	// Load the offensive playbook

	filestring = strDirectory;
	filestring += "Playbooks\\Offensive\\";

	
	if (!Playbook[OFFENSE].Load(filestring, strTeam))
		retval = FALSE;



	// Load the defensive playbook

	filestring = strDirectory;
	filestring += "Playbooks\\Defensive\\";

	
	if (!Playbook[DEFENSE].Load(filestring, strTeam))
		retval = FALSE;



	// Load the roster

	if (!Roster.Load(strDirectory, strTeam))
		retval = FALSE;



	// Load the depth chart

	if (!DepthChart.Load(strDirectory, strTeam))
		retval = FALSE;



	// Close the file

	file.Close();


	if (retval)
		OutputDebugString("TEAM LOADED\r\n");
	else
		OutputDebugString("ERROR FINISHING TEAM\r\n");


	return retval;
	}