#ifndef __PLAYBOOK_H
#define __PLAYBOOK_H



#include "Play.h"



class PLAYBOOK
	{
	public:

		BOOL Load(STRING strDirectory, STRING strFile);


	public:

		int NumFormations;
		FORMATION Formations[MAX_FORMATIONS];

		int NumPlays[MAX_FORMATIONS];
		PLAY Plays[MAX_FORMATIONS][MAX_PLAYS];
	};



#endif