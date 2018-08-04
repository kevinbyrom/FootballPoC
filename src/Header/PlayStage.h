#ifndef __PLAYSTAGE_H
#define __PLAYSTAGE_H



#include "Gamelib.h"


class PLAYER_OBJ;


class PLAY_STAGE : public STAGE
	{
	public:

		enum { TINFO_WIDTH = 200,
			   TINFO_HEIGHT = 70 };

		void Opening();						
		void Closing(STAGE * stage);
		void Loop();

		virtual DWORD OnMessage(int from, int msg, DWORD data1, DWORD data2);


	private:

		void CreateSurfaces();
		void DestroySurfaces();

		void OnBallHitGround();
		void OnLooseBallOutOfBounds();
		void OnBallReceived(PLAYER_OBJ * target);
		void OnBallHolderPassedScrimmage(PLAYER_OBJ * target);
		void OnBallHolderOutOfBounds(PLAYER_OBJ * target);
		void OnBallHolderDown(PLAYER_OBJ * target);
		void OnTouchdown();

		void SetupPlayerObjs();
		void SetupOffensePlayerObjs();
		void SetupDefensePlayerObjs();
		void ConvertWorldToScreenCoordinates();
		void Draw();	
		void DrawTeamInfo(int team);
		void DrawGameInfo();
		void DrawMiniMap();
		void DrawView();

		void EndPlay();


	private:

		SURFACE TeamInfoSurf[2],
				GameInfoSurf,
				MiniMapSurf,
				ViewSurf;

		BOOL	ShowBallHolder;

		GAME_TIMER EndPlayTimer;
	};



#endif