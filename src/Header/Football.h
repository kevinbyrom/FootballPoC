#ifndef __FOOTBALL_H
#define __FOOTBALL_H


#include "Gamelib.h"
#include "Constants.h"
#include "GameInfo.h"
#include "DownInfo.h"
#include "Team.h"
#include "Playbook.h"
#include "Side.h"
#include "Ball.h"


////////////////
// GAME CLASS //
////////////////


class FOOTBALL : public GAME
	{
	public:

		virtual void Opening();
		virtual void Closing();

		void SetupExhibitionGame();

	public:
		
		void OpenBeginGameStage();
		void OpenCoinTossStage();
		void OpenCheckTimeStage();
		void OpenEndGameStage();
		void OpenFirstDownStage();
		void OpenHalfTimeStage();
		void OpenPlayStage();
		void OpenSelectGameModeStage();
		void OpenSelectInputStage();
		void OpenSelectPlayStage();
		void OpenSelectTeamStage();
		void OpenTitleScreenStage();
		void OpenTouchdownStage();
		void OpenTurnOverStage();

	private:

		void LoadBallSprites();
		void LoadPlayerSprites();
		void LoadFieldSprites();
		void LoadIconSprites();
		void LoadPlaybooks();
	};



///////////////
// CONSTANTS //
///////////////

const int FPS				= 60;
const int SCREEN_WIDTH		= 640;
const int SCREEN_HEIGHT		= 480;



/////////////
// EXTERNS //
/////////////

extern FOOTBALL		MainGame;

extern SPRITE		FieldSpr;
extern SPRITE		BallSpr;
extern SPRITE		IconSpr;
extern SPRITE		PlayerSpr[NUM_SIDES];
extern SPRITE		Font;

extern GAME_INFO	GameInfo;
extern DOWN_INFO	DownInfo;
extern TEAM			Team[NUM_SIDES];
extern PLAYBOOK		Playbook[NUM_SIDES];

extern SIDE			Side[NUM_SIDES];
extern PLAY			CurrentPlay[NUM_SIDES];
extern OBJECT		Camera;
extern BALL			Ball;

extern TILE_MAP		FieldMap;

extern DISPLAY_LIST	ObjectDList;
extern DISPLAY_LIST ShadowDList;

extern VPAD			VPad[NUM_SIDES];

extern MESSAGE_MGR	MsgMgr;

extern STRING		strGameDir;


#endif