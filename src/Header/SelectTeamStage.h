#ifndef __SELECTTEAMSTAGE_H
#define __SELECTTEAMSTAGE_H



#include "Gamelib.h"
#include "Constants.h"




struct SELECT_TEAMS
	{
	STRING Name;
	STRING Filename;
	};




class SELECT_TEAM_STAGE : public STAGE
	{
	public:

		enum {
				SELECT_HOME = 0,
				SELECT_AWAY = 1};


		void Opening();						
		void Closing(STAGE * stage);
		void Loop();


	protected:

		void LoadTeams();


	private:
		
		void Draw();


	protected:

		int SelectMode;

		int TeamNum[MAX_TEAMS];

		int NumTeams;
		SELECT_TEAMS Teams[MAX_TEAMS];
	};



#endif