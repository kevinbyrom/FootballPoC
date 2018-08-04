#include <windows.h>
#include <stdio.h>
#include "Playbook.h"
#include "StrUtils.h"





BOOL PLAYBOOK::Load(STRING strDirectory, STRING strTeam)
	{

	/////////////////////////////////////////////
	//										   //
	// Used to load a saved playbook from file //
	//										   //
	/////////////////////////////////////////////



	GAME_FILE file;
	STRING readstring;
	STRING filestring;
	BOOL retval = TRUE;
	int i, ii;



	// First attempt to open the file, on error return false 

	filestring = strDirectory;
	filestring += strTeam;
	filestring += ".txt";


	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING PLAYBOOK FILE\r\n");
		return FALSE;
		}



	// Read the number of formations

	NumFormations = (int)file.ReadLine(TRUE);


	// For each formation

	for (i = 0; i < NumFormations; i++)
		{


		// Read the formation name and load the formation

		readstring = file.ReadLine(TRUE);

		Formations[i].Load(readstring);



		// Read the number of plays

		NumPlays[i] = (int)file.ReadLine(TRUE);



		// Read each play and load the play

		for (ii = 0; ii < NumPlays[i]; ii++)
			{
			readstring = file.ReadLine(TRUE);
			readstring = readstring.Trim();

			Plays[i][ii].Load(readstring);
			}
		}




	// Close the file

	file.Close();


	if (retval)
		OutputDebugString("PLAYBOOK LOADED\r\n");
	else
		OutputDebugString("ERROR :: PLAYBOOK\r\n");



	return TRUE;
	}