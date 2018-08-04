#include <windows.h>
#include "PlayerInfo.h"
#include "StrUtils.h"




ATTRIBUTE::ATTRIBUTE()
	{
	Val = 100.0f;
	}



BOOL ATTRIBUTE::Load(GAME_FILE * pFile)
	{

	//////////////////////////////////////////
	//										//
	// Used to load the attribute from file //
	//										//
	//////////////////////////////////////////


	STRING_ARRAY params;


	// Read the params

	params = pFile->ReadParams();

	if (params.Count() < 4)
		{
		OutputDebugString("ERROR :: NOT ENOUGH PARAMS FOR ATTRIBUTE IN PLAYER FILE\r\n");
		return FALSE;
		}


	// Set the attribute values

	Val			= (float)params[0];	
	Exp			= (int)params[0];	
	NextLvl		= (int)params[0];	
	TotalExp	= (int)params[0];	

	return TRUE;
	}




int ATTRIBUTE::Bonus()
	{

	////////////////////////////////////////////////
	//											  //
	// Used to get a bonus/penalty modifier based //
	// on the attribute value					  //
	//											  //
	////////////////////////////////////////////////


	int value = (int)Value();

	if (value < 10) return -5;
	if (value < 20) return -4;
	if (value < 30) return -3;
	if (value < 40) return -2;
	if (value < 50) return -1;
	if (value < 60) return 0;
	if (value < 70)	return 1;
	if (value < 80)	return 2;
	if (value < 90)	return 3;

	return 4;
	}




PLAYER_INFO::PLAYER_INFO()
	{
	FirstName	= "";
	LastName	= "";
	Number		= 0;
	}




BOOL PLAYER_INFO::Load(STRING strDirectory, STRING strName)
	{

	////////////////////////////////////////////
	//										  //
	// Used to load the player info from file //
	//										  //
	////////////////////////////////////////////


	GAME_FILE file;
	STRING readstring;
	STRING filestring;
	BOOL retval = TRUE;


	// First attempt to open the file, on error return false 

	filestring = strDirectory;
	filestring += "Players\\";
	filestring += strName;
	filestring += ".txt";


	if (!file.OpenForReading(filestring))
		{
		OutputDebugString("ERROR :: OPENING PLAYERINFO FILE\r\n");
		return FALSE;
		}



	// Read the first & last name 

	FirstName = file.ReadLine(TRUE);
	LastName = file.ReadLine(TRUE);

	

	// Read the age, position, number & skin color 

	Age			= (int)file.ReadLine(TRUE);
	Position	= (int)file.ReadLine(TRUE);
	Number		= (int)file.ReadLine(TRUE);
	SkinColor	= (int)file.ReadLine(TRUE);



	// Load the attributes 

	Attrib.Aware.Load(&file);
	Attrib.Strength.Load(&file);
	Attrib.ThrowPow.Load(&file);
	Attrib.ThrowAcc.Load(&file);
	Attrib.Catch.Load(&file);
	Attrib.Accel.Load(&file);
	Attrib.Speed.Load(&file);
	Attrib.BreakTak.Load(&file);
	Attrib.Tackle.Load(&file);
	Attrib.Block.Load(&file);
	Attrib.BreakBlock.Load(&file);
	Attrib.KickPow.Load(&file);
	Attrib.KickAcc.Load(&file);


	// Close the file 

	file.Close();


	return TRUE;
	}