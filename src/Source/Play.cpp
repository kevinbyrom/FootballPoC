#include <windows.h>
#include <stdio.h>
#include "Play.h"
#include "StrUtils.h"
#include "Football.h"



BOOL PLAY::Load(STRING strName)
	{

	/////////////////////////////////////////
	//									   //
	// Used to load a saved play from file //
	//									   //
	/////////////////////////////////////////
 

	GAME_FILE file;
	STRING filestring;
	STRING readstring;
	STRING_ARRAY params;
	int numactions;
	int a, p;
	BOOL retval = TRUE;



	// First attempt to open the file, on error return false 

	filestring = strGameDir;
	filestring += "Plays\\";
	filestring += strName;
	filestring += ".txt";

	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING PLAY FILE\r\n");
		return FALSE;
		}

	

	// Read the play name

	Name = file.ReadLine(TRUE);


	// Read the graphic filename

	GraphicFilename = file.ReadLine(TRUE);



	// For each player...

	for (p = 0; p < MAX_SIDE_PLAYERS; p++)
		{
		// Read the player type

		params = file.ReadParams();

		if (params.Count() < 9)
			{
			OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR PLAYER TYPE IN PLAY FILE\r\n");
			return FALSE;
			}

		PlayerFlags[p].Passer			= (BOOL)params[0];	
		PlayerFlags[p].Receiver			= (BOOL)params[1];
		PlayerFlags[p].IsDefReceiver	= (BOOL)params[2];
		PlayerFlags[p].Blocker			= (BOOL)params[3];
		PlayerFlags[p].Rusher			= (BOOL)params[4];
		PlayerFlags[p].Kicker			= (BOOL)params[5];
		PlayerFlags[p].Punter			= (BOOL)params[6];
		PlayerFlags[p].Placeholder		= (BOOL)params[7];
		PlayerFlags[p].IsDefBallHolder	= (BOOL)params[8];


		// Read number of prehike actions 

		PreHikeActions[p].Clear();

		readstring = file.ReadLine(TRUE);

		numactions = (int)readstring;


		// Read each prehike action

		for (a = 0; a < numactions; a++)
			{
			params = file.ReadParams(TRUE);

			if (params.Count() < 5)
				{
				OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR PRE HIKE ACTION IN PLAY FILE\r\n");
				return FALSE;
				}

			PreHikeActions[p].PushEnd((int)params[0], (int)params[1], (float)params[2], (float)params[3], (float)params[4]);
			}



		// Read number of posthike actions 

		PostHikeActions[p].Clear();

		readstring = file.ReadLine(TRUE);

		numactions = (int)readstring;


		// Read each prehike action

		for (a = 0; a < numactions; a++)
			{
			params = file.ReadParams(TRUE);

			if (params.Count() < 5)
				{
				OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR POSTHIKE ACTION IN PLAY FILE\r\n");
				return FALSE;
				}

			PostHikeActions[p].PushEnd((int)params[0], (int)params[1], (float)params[2], (float)params[3], (float)params[4]);
			}
		}



	// Close the file 

	file.Close();


	return retval;
	}