#include <windows.h>
#include "DepthChart.h"



int DEPTH_CHART::GetRosterPos(int type, int slot)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to get a roster position from the depth chart slot //
	// of a specific type									   //
	//														   //
	/////////////////////////////////////////////////////////////

	if (type < 0 || type >= NUM_DEPTH_TYPES ||
		slot < 0 || slot >= NUM_DEPTH_SLOTS)
		return INVALID_ROSTER_POS;

	return RosterPos[type][slot];
	}




BOOL DEPTH_CHART::Load(STRING strDirectory, STRING strTeam)
	{

	//////////////////////////////////////////
	//										//
	// Used to load a depth chart from file //
	//										//
	//////////////////////////////////////////


	GAME_FILE file;
	STRING readstring;
	STRING filestring;
	BOOL retval = TRUE;
	int i, ii;



	// First attempt to open the file, on error return false 

	filestring = strDirectory;
	filestring += "Depth\\";
	filestring += strTeam;
	filestring += ".txt";


	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING DEPTH CHART FILE\r\n");
		return FALSE;
		}



	// For each depth type

	for (i = 0; i < NUM_DEPTH_TYPES; i++)
		{

		// Read the number of slots filled

		NumSlotsFilled[i] = (int)file.ReadLine(TRUE);


		// For each filled slot
		
		for (ii = 0; ii < NumSlotsFilled[i]; ii++)
			{

			// Read type roster pos

			RosterPos[i][ii] = (int)file.ReadLine(TRUE);
			}
		}


	return retval;
	}