#ifndef __TEAM_H
#define __TEAM_H


#include "Playbook.h"
#include "Roster.h"
#include "DepthChart.h"


#define STADIUM_GRASS	0
#define STADIUM_TURF	1
#define STADIUM_DOME	2



struct STADIUM
	{
	char Name[20];
	int Type;
	};



class TEAM
	{
	public:

		BOOL Load(STRING strDirectory, STRING strTeam, BOOL hometeam = FALSE);

		PLAYER_INFO * GetAvailablePlayerFromType(int type, int pos);
		void RemovePlayersFromPlay();

		FORMATION * GetSelectedFormation(int type);
		PLAY * GetSelectedPlay(int type);


	public:

		STRING Name;
		STRING City;
		STRING State;

		SPRITE		PlayerSpr;
		SPRITE		FieldSpr;

		PLAYBOOK	Playbook[NUM_SIDES];

		ROSTER		Roster;
		DEPTH_CHART DepthChart;

		int ControlledBy;
		int SelectedFormation;
		int SelectedPlay;
	};



#endif