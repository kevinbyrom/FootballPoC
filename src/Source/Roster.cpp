#include <windows.h>
#include <stdio.h>
#include "Roster.h"
#include "StrUtils.h"
#include "Gamelib.h"



PLAYER_INFO * ROSTER::GetPlayerInfo(int pos)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to get a PlayerInfo pointer from a specified position //
	//															  //
	////////////////////////////////////////////////////////////////


	if (pos < 0 || pos >= MAX_ROSTER_PLAYERS || pos == INVALID_ROSTER_POS)
		return NULL;

	return &PlayerInfo[pos];
	}




void ROSTER::RemoveAllFromPlay()
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to set all PlayerInfo's InPlay flag to false //
	//													 //
	///////////////////////////////////////////////////////


	for (int pos = 0; pos < MAX_ROSTER_PLAYERS; pos++)
		PlayerInfo[pos].InPlay = FALSE;
	}




BOOL ROSTER::Load(STRING strDirectory, STRING strTeam)
	{

	/////////////////////////////////////	
	//								   //
	// Used to load a roster from file //
	//								   //
	/////////////////////////////////////	


	GAME_FILE file;
	STRING readstring;
	STRING filestring;
	BOOL retval = TRUE;
	int i;


	
	// Attempt to open the file 

	OutputDebugString("ROSTER LOADING\r\n");



	// First attempt to open the file, on error return false 

	filestring = strDirectory;
	filestring += "ROSTERS\\";
	filestring += strTeam;
	filestring += ".txt";


	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING ROSTER FILE\r\n");
		return FALSE;
		}



	// Read the number of players

	NumPlayers = (int)file.ReadLine(TRUE);



	// Load each player

	for (i = 0; i < NumPlayers; i++)
	{
		readstring = file.ReadLine(TRUE);

		PlayerInfo[i].Load(strDirectory, readstring);
	}



	// Close the file

	file.Close();


	if (retval)
		OutputDebugString("ROSTER LOADED\r\n");
	else
		OutputDebugString("ERROR :: ROSTER\r\n");


	return retval;
	}