#ifndef __PLAYERINFO_H
#define __PLAYERINFO_H


#include "Gamelib.h"



class ATTRIBUTE
	{
	public:

		ATTRIBUTE();

		BOOL Load(GAME_FILE * pFile);

		float Value() { return Val + Mod; }
		int Bonus();

	public:

		float Mod;
		float Val;
		int Exp;
		int NextLvl;
		int TotalExp;
	};



class PLAYER_ATTRIBUTES
	{
	public:

		ATTRIBUTE	Stamina;
		ATTRIBUTE	Aware;
		ATTRIBUTE	Strength;
		ATTRIBUTE	ThrowPow;
		ATTRIBUTE	ThrowAcc;
		ATTRIBUTE	Catch;
		ATTRIBUTE	Accel;
		ATTRIBUTE	Speed;
		ATTRIBUTE	BreakTak;
		ATTRIBUTE	Tackle;
		ATTRIBUTE	Block;
		ATTRIBUTE	BreakBlock;
		ATTRIBUTE	KickPow;
		ATTRIBUTE	KickAcc;
	};




class PLAYER_STATS
	{
	public:

		char Filler;
	};



class PLAYER_INFO
	{
	public:

		PLAYER_INFO();

		BOOL Load(STRING strDirectory, STRING strName);


	public:

		STRING FirstName;
		STRING LastName;
		int Age;
		int Position;
		int Number;
		int SkinColor;

		PLAYER_ATTRIBUTES Attrib;

		PLAYER_STATS CareerStats;
		PLAYER_STATS YearStats;
		PLAYER_STATS GameStats;

		BOOL	InPlay;

		BOOL	Injured;
		int		InjWeeks;
		int		InjType;

		STRING FacePic;
	};



#endif