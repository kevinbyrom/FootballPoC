#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "PlayStage.h"
#include "Football.h"
#include "MathUtils.h"




PLAYER_INFO	BlankPlayer;





void PLAY_STAGE::Opening()
	{

	/////////////////////////////////////////
	//									   //
	// Used to handle opening of the stage //
	//									   //
	/////////////////////////////////////////



	MsgMgr.RemoveTargets();
	EnableMessaging(&MsgMgr);


	// Disable the end play timer

	EndPlayTimer.Enable(FALSE);


	// Create the surfaces 

	CreateSurfaces();


	// Reset the down info defaults 

	DownInfo.Done				= FALSE;
	DownInfo.BallHiked			= FALSE;
	DownInfo.BallPassed			= FALSE;
	DownInfo.BallSpotPos		= DownInfo.ScrimmagePos;
	DownInfo.Turnover			= FALSE;
	DownInfo.PassedScrimmage	= FALSE;


	// Set the side directions

	Side[OFFENSE].Direction = GameInfo.Direction[GameInfo.Possession];
	Side[DEFENSE].Direction = GameInfo.Direction[!GameInfo.Possession];


	Ball.EnableMessaging(&MsgMgr);
	Ball.Wld.X = DownInfo.ScrimmagePos;
	Ball.Wld.Y = 0;
	Ball.Wld.Z = 0;


	// Clear the display list

	ObjectDList.RemoveAll();

	Ball.AddToDisplayList(&ObjectDList);


	// Setup the players

	SetupOffensePlayerObjs();
	SetupDefensePlayerObjs();


	// Lock the camera to the ball

	Camera.LockObject(&Ball, TRUE, TRUE, FALSE, -320.0f, -240.0f, 0.0f);
	Camera.Wld.Y = -240.0f;

	Ball.Sprite = &BallSpr;


	// Set ShowBallHolder to true by default

	ShowBallHolder = TRUE;


	// Hide the AIM object and set its position to the scrimmage pos

	DownInfo.KickAim.Origin.X	= DownInfo.ScrimmagePos;
	DownInfo.KickAim.Origin.Y	= 0;
	DownInfo.KickAim.Origin.Z	= 0;
	DownInfo.KickAim.Wld		= DownInfo.KickAim.Origin;
	DownInfo.KickAim.Delta		= 1.0f;
	DownInfo.KickAim.Active		= FALSE;
	}





void PLAY_STAGE::Closing(STAGE * Stage)
	{

	/////////////////////////////////////////
	//									   //
	// Used to handle closing of the stage //
	//									   //
	/////////////////////////////////////////


	TEAM * OffTeam;
	TEAM * DefTeam;


	// Destroy the surfaces 

	DestroySurfaces();


	// Clear the message manager

	MsgMgr.RemoveTargets();


	// Remove all players from the player objects 

	OffTeam = &Team[GameInfo.Possession];
	DefTeam = &Team[!GameInfo.Possession];

	OffTeam->RemovePlayersFromPlay();
	DefTeam->RemovePlayersFromPlay();
	}





void PLAY_STAGE::CreateSurfaces()
	{

	////////////////////////////////////////
	//									  //
	// Used to create the screen surfaces //
	//									  //
	////////////////////////////////////////


	TeamInfoSurf[0].Create(Screen, TINFO_WIDTH, TINFO_HEIGHT);
	TeamInfoSurf[1].Create(Screen, TINFO_WIDTH, TINFO_HEIGHT);
	GameInfoSurf.Create(Screen, SCREEN_WIDTH - (TINFO_WIDTH * 2), TINFO_HEIGHT);
	MiniMapSurf.Create(Screen, SCREEN_WIDTH, TINFO_HEIGHT);
	ViewSurf.Create(Screen, SCREEN_WIDTH, SCREEN_HEIGHT - TINFO_HEIGHT);
	}





void PLAY_STAGE::DestroySurfaces()
	{

	////////////////////////////////////////
	//									  //
	// Used to destroy the screen surface //
	//									  //
	////////////////////////////////////////


	TeamInfoSurf[0].Destroy();
	TeamInfoSurf[1].Destroy();
	GameInfoSurf.Destroy();
	MiniMapSurf.Destroy();
	ViewSurf.Destroy();
	}





void PLAY_STAGE::SetupOffensePlayerObjs()
	{

	////////////////////////////////////////////////
	//											  //
	// Used to setup the offensive player objects //
	//											  //
	////////////////////////////////////////////////


	TEAM * OffTeam;
	PLAY * OffPlay;
	FORMATION * OffFormation;


	Side[OFFENSE].Clear();


	// Set the team & play pointers 

	OffTeam = &Team[GameInfo.Possession];
	OffPlay = OffTeam->GetSelectedPlay(OFFENSE);
	OffFormation = OffTeam->GetSelectedFormation(OFFENSE);



	// Setup each player 

	for (int p = 0; p < MAX_SIDE_PLAYERS; p++)
		{

		// Prepare for the play

		Side[OFFENSE].Player[p].PrepareForPlay();


		// Enable messaging for the player 

		Side[OFFENSE].Player[p].EnableMessaging(&MsgMgr, OFFENSE);


		// Set the player side & team

		Side[OFFENSE].Player[p].SideNum = OFFENSE;
		Side[OFFENSE].Player[p].TeamNum	= GameInfo.Possession;


		// Assign the actions 

		Side[OFFENSE].Player[p].PreHikeActions	= OffPlay->PreHikeActions[p]; 
		Side[OFFENSE].Player[p].PostHikeActions	= OffPlay->PostHikeActions[p];


		Side[OFFENSE].Player[p].BeginPreHikeActions();
		

		// Assign the position 

		if (GameInfo.Direction[GameInfo.Possession] == DIR_LEFT)
			{
			Side[OFFENSE].Player[p].Wld.X	= DownInfo.ScrimmagePos + OffFormation->OffsetWX[p] + (Side[OFFENSE].Player[p].Bound[0].Radius + 5);
			Side[OFFENSE].Player[p].Wld.Y	= OffFormation->OffsetWY[p];
			}
		else
			{
			Side[OFFENSE].Player[p].Wld.X	= DownInfo.ScrimmagePos - OffFormation->OffsetWX[p] - (Side[OFFENSE].Player[p].Bound[0].Radius + 5);
			Side[OFFENSE].Player[p].Wld.Y	= OffFormation->OffsetWY[p];
			}


		// Assign the player flags 

		Side[OFFENSE].Player[p].Flag = OffPlay->PlayerFlags[p];

		if (Side[OFFENSE].Player[p].Flag.Passer)		Side[OFFENSE].Passer		= &Side[OFFENSE].Player[p];
		if (Side[OFFENSE].Player[p].Flag.Rusher)		Side[OFFENSE].Rusher		= &Side[OFFENSE].Player[p];
		if (Side[OFFENSE].Player[p].Flag.Kicker)		Side[OFFENSE].Kicker		= &Side[OFFENSE].Player[p];
		if (Side[OFFENSE].Player[p].Flag.Punter)		Side[OFFENSE].Punter		= &Side[OFFENSE].Player[p];
		if (Side[OFFENSE].Player[p].Flag.Placeholder)	Side[OFFENSE].Placeholder	= &Side[OFFENSE].Player[p];
		if (Side[OFFENSE].Player[p].Flag.IsDefReceiver)	Side[OFFENSE].TargetRec		= &Side[OFFENSE].Player[p];
		if (Side[OFFENSE].Player[p].Flag.IsDefBallHolder)	Side[OFFENSE].Player[p].GetBall();


		// Set the sprites and make the object visible 

		Side[OFFENSE].Player[p].Sprite	= &OffTeam->PlayerSpr;
		Side[OFFENSE].Player[p].Visible = TRUE;


		// Assign the PlayerInfo pointer 

		Side[OFFENSE].Player[p].SetPlayerInfo(OffTeam->GetAvailablePlayerFromType(OffFormation->DepthType[p], OffFormation->DepthPos[p]));
		Side[OFFENSE].Player[p].DepthPos = OffFormation->DepthPos[p];

		if (Side[OFFENSE].Player[p].PlayerInfo == NULL)
			Side[OFFENSE].Player[p].SetPlayerInfo(&BlankPlayer);

		
		// Add to the display list 

		Side[OFFENSE].Player[p].AddToDisplayList(&ObjectDList);


		// Face the player 

		Side[OFFENSE].Player[p].FaceDir = GameInfo.Direction[GameInfo.Possession];


		// Assign the controller

		Side[OFFENSE].Player[p].PadNum = 0;
		}
	}





void PLAY_STAGE::SetupDefensePlayerObjs()
	{

	////////////////////////////////////////////////
	//											  //
	// Used to setup the defensive player objects //
	//											  //
	////////////////////////////////////////////////


	TEAM * DefTeam;
	PLAY * DefPlay;
	FORMATION * DefFormation;


	Side[DEFENSE].Clear();


	// Set the team & play pointers 

	DefTeam = &Team[!GameInfo.Possession];
	DefPlay = DefTeam->GetSelectedPlay(DEFENSE);
	DefFormation = DefTeam->GetSelectedFormation(DEFENSE);


	// Setup each player 

	for (int p = 0; p < MAX_SIDE_PLAYERS; p++)
		{

		// Prepare for the play

		Side[DEFENSE].Player[p].PrepareForPlay();


		// Enable messaging for the player 

		Side[DEFENSE].Player[p].EnableMessaging(&MsgMgr, DEFENSE);


		// Set the player side & team 

		Side[DEFENSE].Player[p].SideNum = DEFENSE;
		Side[DEFENSE].Player[p].TeamNum	= !GameInfo.Possession;


		// Assign the actions 

		Side[DEFENSE].Player[p].PreHikeActions	= DefPlay->PreHikeActions[p]; 
		Side[DEFENSE].Player[p].PostHikeActions	= DefPlay->PostHikeActions[p];

		Side[DEFENSE].Player[p].BeginPreHikeActions();
		

		// Assign the position 

		if (GameInfo.Direction[GameInfo.Possession] == DIR_LEFT)
			{
			Side[DEFENSE].Player[p].Wld.X	= DownInfo.ScrimmagePos - DefFormation->OffsetWX[p] - NEUTRAL_ZONE_DISTANCE;
			Side[DEFENSE].Player[p].Wld.Y	= DefFormation->OffsetWY[p];
			}
		else
			{
			Side[DEFENSE].Player[p].Wld.X	= DownInfo.ScrimmagePos + DefFormation->OffsetWX[p] + NEUTRAL_ZONE_DISTANCE;
			Side[DEFENSE].Player[p].Wld.Y	= DefFormation->OffsetWY[p];
			}
		

		// Assign the player flags 

		Side[DEFENSE].Player[p].Flag = DefPlay->PlayerFlags[p];


		// Set the sprites and make the object visible 

		Side[DEFENSE].Player[p].Sprite	= &DefTeam->PlayerSpr;
		Side[DEFENSE].Player[p].Visible = TRUE;


		// Assign the PlayerInfo pointer 

		Side[DEFENSE].Player[p].PlayerInfo = DefTeam->GetAvailablePlayerFromType(DefFormation->DepthType[p], DefFormation->DepthPos[p]);
		Side[DEFENSE].Player[p].DepthPos = DefFormation->DepthPos[p];

		if (Side[DEFENSE].Player[p].PlayerInfo == NULL)
			Side[DEFENSE].Player[p].SetPlayerInfo(&BlankPlayer);


		// Add to the display list 

		Side[DEFENSE].Player[p].AddToDisplayList(&ObjectDList);


		// Face the player 

		Side[DEFENSE].Player[p].FaceDir = GameInfo.Direction[!GameInfo.Possession];


		// Assign the controller

		Side[OFFENSE].Player[p].PadNum = 0;
		}
	}




void PLAY_STAGE::ConvertWorldToScreenCoordinates()
	{

	////////////////////////////////////////////////////
	//												  //
	// Used to convert all OBJECTs' world coordinates //
	// to screen coordinates						  //
	//												  //
	////////////////////////////////////////////////////


	Camera.Scr.X = int(Camera.Wld.X);
	Camera.Scr.Y = int(Camera.Wld.Y - Camera.Wld.Z);


	//if (Camera.Scr.X < -1260)	Camera.Scr.X = -1260;  // Negative HalfWidth + 60
	//if (Camera.Scr.X > 620)		Camera.Scr.X = 620;	   // Positive HalfWidth + 60 - 640
	//if (Camera.Scr.Y < -300)	Camera.Scr.Y = -300;
	if (Camera.Scr.Y > -200)	Camera.Scr.Y = -200;

	
	if (DownInfo.KickAim.Active)
		{
		DownInfo.KickAim.Scr.X = int(DownInfo.KickAim.Wld.X);
		DownInfo.KickAim.Scr.Y = int(DownInfo.KickAim.Wld.Y - DownInfo.KickAim.Wld.Z);
		}


	for (int i = 0; i < MAX_SIDE_PLAYERS; i++)
		{
		Side[OFFENSE].Player[i].Scr.X = int(Side[OFFENSE].Player[i].Wld.X - Camera.Scr.X);
		Side[OFFENSE].Player[i].Scr.Y = int(Side[OFFENSE].Player[i].Wld.Y - Side[OFFENSE].Player[i].Wld.Z - Camera.Scr.Y);

		Side[DEFENSE].Player[i].Scr.X = int(Side[DEFENSE].Player[i].Wld.X - Camera.Scr.X);
		Side[DEFENSE].Player[i].Scr.Y = int(Side[DEFENSE].Player[i].Wld.Y - Side[DEFENSE].Player[i].Wld.Z - Camera.Scr.Y);
		}

	Ball.Scr.X = Ball.Wld.X - Camera.Scr.X;
	Ball.Scr.Y = Ball.Wld.Y - Ball.Wld.Z - Camera.Scr.Y;
	}





void PLAY_STAGE::Loop()
	{
	
	/////////////////////////////
	//						   //
	// Main loop for the stage //
	//						   //
	/////////////////////////////
	

	// Update the message manager

	MsgMgr.Update();


	// If the 

	if (Input->GetKey(DIK_SPACE) && !DownInfo.BallHiked)
		{
		SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_HIKED);

		DownInfo.BallHiked = TRUE;
		}



	// Update the players, ball & camera 

	Side[OFFENSE].Update();
	Side[DEFENSE].Update();
	Ball.Update();
	Camera.Update();


	// Update the AIM

	if (DownInfo.KickAim.Active)
		{
		DownInfo.KickAim.Wld.Y += DownInfo.KickAim.Delta;

		if (DownInfo.KickAim.Delta < 0)
			{
			if (DownInfo.KickAim.Wld.Y < DownInfo.KickAim.Origin.Y - DOWN_MAX_AIM_RANGE)
				{
				DownInfo.KickAim.Wld.Y = DownInfo.KickAim.Origin.Y - DOWN_MAX_AIM_RANGE;
				DownInfo.KickAim.Delta = -DownInfo.KickAim.Delta;
				}
			}
		else
			{
			if (DownInfo.KickAim.Wld.Y > DownInfo.KickAim.Origin.Y + DOWN_MAX_AIM_RANGE)
				{
				DownInfo.KickAim.Wld.Y = DownInfo.KickAim.Origin.Y + DOWN_MAX_AIM_RANGE;
				DownInfo.KickAim.Delta = -DownInfo.KickAim.Delta;
				}
			}
		}


	// Draw the screen

	ConvertWorldToScreenCoordinates();
	Draw();					


	// Check if the play is done

	if (DownInfo.Done)
		{
		if (!EndPlayTimer.IsEnabled())
			EndPlayTimer.CountDownSecs(3);
		else if (EndPlayTimer.IsDone())
			EndPlay();
		}
	}





void PLAY_STAGE::EndPlay()
	{

	//////////////////////////////////
	//								//
	// Used to end the current play //
	//								//
	//////////////////////////////////


	switch(GameInfo.PlayDone())
		{
		case SHOW_TOUCHDOWN:
			MainGame.OpenTouchdownStage();
			break;

		case SHOW_FIRSTDOWN:
			MainGame.OpenFirstDownStage();
			break;

		case SHOW_TURNOVER:
			MainGame.OpenTurnOverStage();
			break;

		case SHOW_SACK:
			//MainGame.OpenSackStage();
			MainGame.OpenCheckTimeStage();
			break;

		case SHOW_PATGOOD:
			//MainGame.OpenPATGoodStage();
			break;

		case SHOW_PATNOGOOD:
			//MainGame.OpenPATNoGoodStage();
			break;

		case SHOW_FIELDGOALGOOD:
			//MainGame.OpenFieldGoalGoodStage();
			break;

		case SHOW_FIELDGOALNOGOOD:
			//MainGame.OpenFieldGoalNoGoodStage();
			break;

		case SHOW_NONE:
		default:
			MainGame.OpenCheckTimeStage();
			break;
		}
	}






DWORD PLAY_STAGE::OnMessage(int from, int msg, DWORD data1, DWORD data2)
	{

	/////////////////////////////
	//						   //
	// Used to handle messages //
	//						   //
	/////////////////////////////

	switch(msg)
		{
		case MSG_BALL_HIT_GROUND:
			OnBallHitGround();
			break;

		case MSG_LOOSE_BALL_OUT_OF_BOUNDS:
			OnLooseBallOutOfBounds();
			break;

		case MSG_BALL_CAUGHT:
			OnBallReceived((PLAYER_OBJ *)data1);
			break;

		case MSG_BALL_PASSED_SCRIMMAGE:
			OnBallHolderPassedScrimmage((PLAYER_OBJ *)data1);
			break;

		case MSG_BALL_HOLDER_OUT_OF_BOUNDS:
			OnBallHolderOutOfBounds((PLAYER_OBJ *)data1);
			break;

		case MSG_BALL_HOLDER_DOWN:
			OnBallHolderDown((PLAYER_OBJ *)data1);
			break;

		case MSG_START_AIM:
			DownInfo.KickAim.Active = TRUE;
			DownInfo.KickAim.Delta	= (float)data1;
			break;

		case MSG_END_AIM:
			DownInfo.KickAim.Active = FALSE;
			break;

		case MSG_TOUCHDOWN:
			OnTouchdown();
			break;
		}

	return -1;
	}




void PLAY_STAGE::OnBallHitGround()
	{

	///////////////////////////////////////////
	//										 //
	// Used to handle ball hit ground events //
	//										 //
	///////////////////////////////////////////


	BOOL Fumbled = FALSE;


	// Check if the ball was thrown behind the QB, if so, it is a fumble 

	if (Side[OFFENSE].Direction == DIR_LEFT ? Ball.Wld.X > Ball.Origin.X : Ball.Wld.X < Ball.Origin.X)
		Fumbled = TRUE;


	// If fumbled, send the fumbled message

	if (Fumbled)
		{
		SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_FUMBLED);
		}


	// Otherwise, end the down with an incomplete pass

	else
		{
		//SendMessageToGroup(MESSAGE_DEST_ALL, MSG_INCOMPLETE_PASS);

		DownInfo.Done		= TRUE;
		DownInfo.DoneReason = DOWN_DONE_INCOMPLETEPASS;
		}
	}





void PLAY_STAGE::OnLooseBallOutOfBounds()
	{

	////////////////////////////////////////////////////
	//												  //
	// Used to handle loose ball out of bounds events //
	//												  //
	////////////////////////////////////////////////////
	}





void PLAY_STAGE::OnBallReceived(PLAYER_OBJ * target)
	{

	//////////////////////////////////////////
	//										//
	// Used to handle ball reception events //
	//										//
	//////////////////////////////////////////


	if (!DownInfo.Done && target->TeamNum != GameInfo.Possession)
		DownInfo.Turnover = TRUE;
	}





void PLAY_STAGE::OnBallHolderPassedScrimmage(PLAYER_OBJ * target)
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to handle ball passed scrimmage events //
	//											   //
	/////////////////////////////////////////////////


	DownInfo.PassedScrimmage = TRUE;
	}





void PLAY_STAGE::OnBallHolderOutOfBounds(PLAYER_OBJ * target)
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to handle ball holder out of bounds events //
	//												   //
	/////////////////////////////////////////////////////


	DownInfo.Done			= TRUE;
	DownInfo.DoneReason		= DOWN_DONE_OUTOFBOUNDS;
	DownInfo.BallSpotPos	= target->Wld.X;
	}





void PLAY_STAGE::OnBallHolderDown(PLAYER_OBJ * target)
	{

	////////////////////////////////////////////
	//								          //
	// Used to handle ball holder down events //
	//								          //
	////////////////////////////////////////////


	// Set the done flag to TRUE

	DownInfo.Done		= TRUE;


	// Calculate whether it is a sack or a regular tackle 

	if (target->PlayerInfo->Position == DEPTH_QB && !DownInfo.PassedScrimmage)
		DownInfo.DoneReason	= DOWN_DONE_SACKED;
	else
		DownInfo.DoneReason	= DOWN_DONE_TACKLED;
	

	// Spot the ball

	DownInfo.BallSpotPos = target->Wld.X;
	}





void PLAY_STAGE::OnTouchdown()
	{

	/////////////////////////////////////
	//								   //
	// Used to handle touchdown events //
	//								   //
	/////////////////////////////////////


	DownInfo.Done		= TRUE;
	DownInfo.DoneReason	= DOWN_DONE_TOUCHDOWN;
	}






void PLAY_STAGE::Draw()
	{

	/////////////////////////////
	//						   //
	// Used to draw the screen //
	//						   //
	/////////////////////////////


	DrawTeamInfo(0);
	DrawTeamInfo(1);
	DrawGameInfo();
	DrawMiniMap();
	DrawView();
	
	Screen->GetBack()->BlitFast(&TeamInfoSurf[0], 0, 0);
	Screen->GetBack()->BlitFast(&GameInfoSurf, TINFO_WIDTH, 0);
	Screen->GetBack()->BlitFast(&TeamInfoSurf[1], SCREEN_WIDTH - TINFO_WIDTH, 0);
	Screen->GetBack()->BlitFast(&ViewSurf, 0, TINFO_HEIGHT);
	}





void PLAY_STAGE::DrawTeamInfo(int team)
	{

	////////////////////////////////
	//							  //
	// Used to draw the team info //
	//							  //
	////////////////////////////////


	char TeamNameAndScore[30];


	// Clear the surface 

	TeamInfoSurf[team].Fill(16);


	// Draw a border 

	/*TeamInfoSurf[team].Rect(0, 0, TINFO_WIDTH - 1, TINFO_HEIGHT - 1, 15, 2);


	// Draw the team name and score

	sprintf(TeamNameAndScore, "%s - %d", Team[team].Name, GameInfo.Score[team]);
	Font.DrawTextCentered(&TeamInfoSurf[team], TeamNameAndScore, TINFO_WIDTH / 2, 15);


	// Draw the controlled player name & stamina

	if (Side[team].Control)
		{
		int stawidth;
		float stapercent;
		

		// Calculate the width of the stamina bar

		stapercent = Side[team].Control->PlayerInfo->Attrib.Stamina.Value() / 100.0f;
		stawidth = (TINFO_WIDTH - 15) - (TINFO_WIDTH / 2);

		Font.DrawText(&TeamInfoSurf[team], Side[0].Control->PlayerInfo->LastName, 15, 40);
		TeamInfoSurf[team].Rect(TINFO_WIDTH / 2, 45, (TINFO_WIDTH / 2) + stawidth, 50, 15);
		TeamInfoSurf[team].FillRect((TINFO_WIDTH / 2) + 1, 46, (TINFO_WIDTH / 2) + 1 + ((stawidth - 2) * stapercent), 49, 2);
		}*/
	}





void PLAY_STAGE::DrawGameInfo()
	{
	GameInfoSurf.Fill(16);
	}





void PLAY_STAGE::DrawMiniMap()
	{
	}





void PLAY_STAGE::DrawView()
	{

	////////////////////////////////////////////
	//										  //
	// Used to draw the field onto the screen //
	//										  //
	////////////////////////////////////////////


	int i;


	// Draw the field 

	FieldMap.Draw(&ViewSurf, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - TINFO_HEIGHT, Camera.Scr.X + 1200, Camera.Scr.Y + 400);


	// Draw the shadows 

	for (i = 0; i < MAX_SIDE_PLAYERS; i++)
		{
		IconSpr.Draw(&ViewSurf, 0, int(Side[OFFENSE].Player[i].Wld.X - Camera.Scr.X - 6), int(Side[OFFENSE].Player[i].Wld.Y - Camera.Scr.Y - 7));
		}

	IconSpr.Draw(&ViewSurf, 1, int(Ball.Wld.X - Camera.Scr.X - 6), int(Ball.Wld.Y - Camera.Scr.Y - 7));


	// Draw the control & receiver icons

	if (Side[OFFENSE].Control)
		IconSpr.Draw(&ViewSurf, 2, int(Side[OFFENSE].Control->Wld.X - Camera.Scr.X - 6), int(Side[OFFENSE].Control->Wld.Y - Camera.Scr.Y - 4));

	if (!DownInfo.PassedScrimmage && DownInfo.BallHiked && Side[OFFENSE].TargetRec && (Side[OFFENSE].TargetRec->Scr.X > -Side[OFFENSE].TargetRec->Width && Side[OFFENSE].TargetRec->Scr.X < 640 &&
		Side[OFFENSE].TargetRec->Scr.Y > -Side[OFFENSE].TargetRec->Height && Side[OFFENSE].TargetRec->Scr.Y < 480))
		IconSpr.Draw(&ViewSurf, 2, int(Side[OFFENSE].TargetRec->Wld.X - Camera.Scr.X - 6), int(Side[OFFENSE].TargetRec->Wld.Y - Camera.Scr.Y - 4));

	if (Ball.IsLoose())
		{
		IconSpr.Draw(&ViewSurf, 2, int(Ball.InRangeSpot.Wld.X - Camera.Scr.X - 6), int(Ball.InRangeSpot.Wld.Y - Camera.Scr.Y - 4));
		IconSpr.Draw(&ViewSurf, 2, int(Ball.CatchSpot.Wld.X - Camera.Scr.X - 6), int(Ball.CatchSpot.Wld.Y - Camera.Scr.Y - 4));
		IconSpr.Draw(&ViewSurf, 2, int(Ball.LandingSpot.Wld.X - Camera.Scr.X - 6), int(Ball.LandingSpot.Wld.Y - Camera.Scr.Y - 4));
		}


	// Draw the players & ball 

	ObjectDList.DrawAll(&ViewSurf);


	// Draw the offscreen control & receiver icons

	if (!DownInfo.PassedScrimmage && Side[OFFENSE].TargetRec && (Side[OFFENSE].TargetRec->Scr.X <= -Side[OFFENSE].TargetRec->Width || Side[OFFENSE].TargetRec->Scr.X >= 640 ||
		Side[OFFENSE].TargetRec->Scr.Y <= -Side[OFFENSE].TargetRec->Height || Side[OFFENSE].TargetRec->Scr.Y >= 480))
		IconSpr.Draw(&ViewSurf, 4, 640 - 11, int(Side[OFFENSE].TargetRec->Wld.Y - Camera.Scr.Y - 14));


	// Draw the player infos


	// Draw the KICK AIM

	if (DownInfo.KickAim.Active)
		{
		IconSpr.Draw(&ViewSurf, 4, int(DownInfo.KickAim.Wld.X - Camera.Scr.X - 6 - 30), int(DownInfo.KickAim.Wld.Y - Camera.Scr.Y - 14));
		}
	}



