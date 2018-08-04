#include <windows.h>
#include "Formation.h"
#include "Gamelib.h"
#include "Football.h"



BOOL FORMATION::Load(STRING strName)
	{

	//////////////////////////////////////////////
	//										    //
	// Used to load a saved formation from file //
	//										    //
	//////////////////////////////////////////////
 


	GAME_FILE file;
	STRING readstring;
	STRING filestring;
	STRING_ARRAY params;
	BOOL retval = TRUE;



	// First attempt to open the file, on error return false 

	filestring = strGameDir;
	filestring += "Formations\\";
	filestring += strName;
	filestring += ".txt";


	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING FORMATION FILE\r\n");
		return FALSE;
		}



	// Read the formation name and graphic file

	Name			= file.ReadLine(TRUE);
	GraphicFilename = file.ReadLine(TRUE);



	// Read the SpecialTeams and Kickoff flag

	IsSpecialTeams	= (int)file.ReadLine(TRUE);
	IsKickoff		= (int)file.ReadLine(TRUE);



	// Read each player type and offset

	for (int i = 0; i < MAX_SIDE_PLAYERS; i++)
		{
		params = file.ReadParams();

		if (params.Count() < 2)
			{
			OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR OFFSETS IN FORMATION FILE\r\n");
			return FALSE;
			}

		DepthType[i]	= (int)params[0];
		DepthPos[i]		= (int)params[1];


		params = file.ReadParams();

		if (params.Count() < 2)
			{
			OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR OFFSETS IN FORMATION FILE\r\n");
			return FALSE;
			}

		OffsetWX[i]		= (float)params[0];	
		OffsetWY[i]		= (float)params[1];	
		}



	// Close the file

	file.Close();



	return retval;
	}