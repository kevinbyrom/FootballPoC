#ifndef __SELECTPLAYSTAGE_H
#define __SELECTPLAYSTAGE_H



#include "Gamelib.h"
#include "Constants.h"
#include "Team.h"



const int SELECT_FORMATION	= 0;
const int SELECT_PLAY		= 1;
const int SELECT_DONE		= 2;

const int SELECT_SLOT1		= 0;
const int SELECT_SLOT2		= 1;
const int SELECT_SLOT3		= 2;

const int NUM_DISPLAY_SLOTS	= 3;




struct SELECT_SLOT
	{
	STRING Name;
	int Pos;
	int SurfPos;
	};



struct SELECT_INFO
	{
	int Type;
	int Pos;
	int Formation;
	int Play;
	};




class PLAY_SELECT_ITEM
	{
	public:

		void Init(int side);
		void DeInit();

		void SlotPageUp();
		void SlotPageDown();
		void SlotSelect(int index);
		void SlotPrevious();
		void FinishSelecting();
		BOOL IsDoneSelecting()	{ return SelectInfo.Type == SELECT_DONE; }

		STRING GetSlotName(int index);
		SURFACE * GetSlotSurface(int index);


	private:
	

		void SetTeamAndPlaybookPointers(int side);
		void SetKickoffAndSpecialTeamsFormations();
		void InitSelectInfo();
		void FillSelectSlots();

		void LoadFormationSurfs();
		void LoadPlaySurfs();

		void UnloadFormationSurfs();
		void UnloadPlaySurfs();


	private:

		TEAM * pTeam;
		PLAYBOOK * pPlaybook;

		int				NumSelectSlots;
		SELECT_SLOT		SelectSlots[MAX_PLAYS];
		SELECT_INFO		SelectInfo;

		int NumFormationSurfs;
		SURFACE FormationSurf[MAX_FORMATIONS];

		int NumPlaySurfs;
		SURFACE PlaySurf[MAX_PLAYS];

		int KickoffForm;
		int SpecialTeamsForm;

	};



class SELECT_PLAY_STAGE : public STAGE
	{
	public:

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();


	private:

		void Draw();
		void DrawSelectSlots(int side, int x, int y);
		

	private:
		
		PLAY_SELECT_ITEM PlaySel[NUM_SIDES];
	};




#endif