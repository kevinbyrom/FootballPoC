#include <windows.h>
#include <stdio.h>
#include "Player.h"
#include "Football.h"



int RunAnim[] = { 2,4,6,8,10,12,14,ANIM_LOOP,0};

 


PLAYER::PLAYER()
	{
	
	/////////////////////////////
	//						   //
	// Base Player Constructor //
	//						   //
	/////////////////////////////

	SetSide(); 

	FaceDir = DIR_RIGHT;
	MoveDir	= DIR_RIGHT;
	}






void PLAYER::Update()
	{

	//////////////////////////////
	//							//
	// The main update function //
	//							//
	//////////////////////////////


	ProcessControl();
	ProcessState(UPDATE);
	UpdateDeltas();
	UpdateAnimation();
	UpdateMove();


	// Set the frame //

	switch(FaceDir)
		{
		case DIR_UP:
			Frame = Anim.Frame; 
			break;

		case DIR_DOWN:
			Frame = 16 + Anim.Frame; 
			break;

		case DIR_LEFT:
			Frame = 32 + Anim.Frame; 
			break;

		case DIR_RIGHT:
			Frame = 48 + Anim.Frame; 
			break;

		case DIR_UPLEFT:
			Frame = 64 + Anim.Frame; 
			break;

		case DIR_UPRIGHT:
			Frame = 80 + Anim.Frame; 
			break;

		case DIR_DOWNLEFT:
			Frame = 96 + Anim.Frame; 
			break;

		case DIR_DOWNRIGHT:
			Frame = 112 + Anim.Frame; 
			break;
		} 
	}






//////////////////
// CONTROL CODE //
//////////////////



void PLAYER::ProcessControl()
	{

	////////////////////////////////////////////////
	//											  //
	// Used to perform logic based on the current //
	// control type								  //
	//											  //
	////////////////////////////////////////////////


	switch(ControlType)
		{
		case CONTROL_USER:
			DoUserControl();
			break;


		case CONTROL_ACTIONS:
			DoActionControl();
			break;
		}
	}




void PLAYER::DoUserControl()
	{

	///////////////////////////////////////////
	//										 //
	// User to process control from the user //
	//										 //
	///////////////////////////////////////////
	

	int dir			= 0;
	BOOL MoveLeft	= FALSE;
	BOOL MoveRight	= FALSE;
	BOOL MoveUp		= FALSE;
	BOOL MoveDown	= FALSE;
	

	if (Input->GetKey(DIK_LEFT))
		MoveLeft = TRUE;
	else if (Input->GetKey(DIK_RIGHT))
		MoveRight = TRUE;


	if (Input->GetKey(DIK_UP))
		MoveUp = TRUE;
	else if (Input->GetKey(DIK_DOWN))
		MoveDown = TRUE;


	if (MoveLeft)	dir |= DIR_LEFT;
	if (MoveRight)	dir |= DIR_RIGHT;
	if (MoveUp)		dir |= DIR_UP;
	if (MoveDown)	dir |= DIR_DOWN;
	

	if (dir)
		SetState(STATE_RUN, dir);
	else
		SetState(STATE_STAND);
	}




void PLAYER::DoActionControl()
	{

	//////////////////////////////////////////////////
	//												//
	// Used to process control from an action array //
	//												//
	//////////////////////////////////////////////////


	// Do nothing if action position is not set 

	if (ActionPos == -1 || Actions == NULL)
		return;


	// If this is a time based action, and the time is met 
	// then do the next action 

	if (Actions->Action[ActionPos].Time != -1)
		{
		if (ActionCount >= Actions->Action[ActionPos].Time)
			DoNextAction();
		}

	// If the state is done then do the next action

	else if (StateDone)
		DoNextAction();


	// Increment the action counter 

	ActionCount++;
	}





////////////////
// STATE CODE //
////////////////


void PLAYER::SetState(int state, int param)
	{

	//////////////////////////////////////
	//									//
	// Used to set the new player state //
	//									//
	//////////////////////////////////////

	
	State		= state;
	StateTime	= -1;
	StateCount	= 0;
	StateDone	= FALSE;

	ProcessState(ENTER, param);
	}




void PLAYER::ProcessState(int reason, int param)
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to process the logic for the current state //
	//												   //
	/////////////////////////////////////////////////////


	switch(State)
		{
		case STATE_STAND:
			DoStand(reason);
			break;

		case STATE_RUN:
			DoRun(reason, param);
			break;

		case STATE_RUNTO:
			DoRunTo(reason);
			break;

		case STATE_LEAP:
			DoLeap(reason);
			break;

		case STATE_DIVE:
			DoDive(reason);
			break;

		case STATE_GRAPPLE:
			DoGrapple(reason);
			break;

		case STATE_BREAKGRAPPLE:
			DoBreakGrapple(reason);
			break;

		case STATE_TUMBLE:
			DoTumble(reason);
			break;

		case STATE_CELEBRATE:
			DoCelebrate(reason);
			break;

		case STATE_COMPLAIN:
			DoComplain(reason);
			break;
		}


	// Increment the state counter //

	StateCount++;

	
	// If this state is timed, then if the time is met set the done flag //

	if (StateTime != -1 && StateCount >= StateTime)
		StateDone = TRUE;
	}




void PLAYER::DoStand(int reason)
	{
	OnEnter()
		{
		SetAnimation(NULL);
		Stop();
		}
	}




void PLAYER::DoRun(int reason, int dir)
	{
	OnEnter()
		{
		// If moving for the first time, start the run animation //

		if (Anim.Frames != RunAnim)
			{
			SetAnimation(RunAnim);
			SetAnimationSpeed(3);
			SetAnimationPos(0);
			}
		
		Animate(TRUE);
			


		// Set the acceleration //

		AccX = MaxAccX;
		AccY = MaxAccY;



		// Set the new move direction //

		MoveDir = dir;
		FaceDir = dir;
		}
	}





void PLAYER::DoRunTo(int reason)
	{
	int dir = 0;


	OnEnter()
		{
		// If moving for the first time, start the run animation //

		if (Anim.Frames != RunAnim)
			{
			SetAnimation(RunAnim);
			SetAnimationSpeed(3);
			SetAnimationPos(0);
			}
		
		Animate(TRUE);


		// Set the acceleration //

		AccX = MaxAccX;
		AccY = MaxAccY;
		}


	// Calculate where to move to //

	if (WX > DestWX) dir |= DIR_LEFT;
	if (WX < DestWX) dir |= DIR_RIGHT;
	if (WY > DestWY) dir |= DIR_UP;
	if (WY < DestWY) dir |= DIR_DOWN;


	// Set the new move direction //

	MoveDir = dir;
	FaceDir = dir;


	// Set the done flag if the destination is met //

	if (WX == DestWX && WY == DestWY)
		StateDone = TRUE;
	}




void PLAYER::DoLeap(int reason)
	{
	}




void PLAYER::DoDive(int reason)
	{
	}




void PLAYER::DoGrapple(int reason)
	{
	}




void PLAYER::DoBreakGrapple(int reason)
	{
	}




void PLAYER::DoTumble(int reason)
	{
	}




void PLAYER::DoCelebrate(int reason)
	{
	}




void PLAYER::DoComplain(int reason)
	{
	}




/////////////////
// ACTION CODE //
/////////////////



void PLAYER::SetActions(ACTIONS * actions)	
	{
	
	////////////////////////////////////////////
	//										  //
	// Used to set the current action pointer //
	//										  //
	////////////////////////////////////////////


	Actions		= actions; 
	ActionPos	= -1;
	}


		
void PLAYER::BeginActions()
	{ 

	//////////////////////////////////
	//								//
	// Used to begin action control //
	//								//
	//////////////////////////////////


	ControlType	= CONTROL_ACTIONS;
	ActionPos	= 0;
	ActionCount	= 0;

	DoAction();
	}



void PLAYER::DoAction()
	{

	////////////////////////////////////////
	//									  //
	// Used to perform the current action //
	//									  //
	////////////////////////////////////////


	ActionCount = 0;


	switch(Actions->Action[ActionPos].Type)
		{
		case ACTION_STAND:
			SetState(STATE_STAND);
			break;

		case ACTION_RUN:
			SetState(STATE_RUN, (int)Actions->Action[ActionPos].Param[0]);
				char Text[256];
				sprintf(Text, "RUN - %.02f - %d - %.02f %.02f\r\n", Actions->Action[ActionPos].Param[0], ActionPos, WX, WY);
				OutputDebugString(Text);
			break;

		case ACTION_RUNTODEST:
			DestWX = Actions->Action[ActionPos].Param[0];
			DestWY = Actions->Action[ActionPos].Param[1];
			SetState(STATE_RUNTO);
			break;

		case ACTION_FACE:		
			FaceDir = (int)Actions->Action[ActionPos].Param[0];
			break;

		case ACTION_SETDEST:	
			DestWX = Actions->Action[ActionPos].Param[0];
			DestWY = Actions->Action[ActionPos].Param[1];
			break;

		case ACTION_STATE:	
			SetState((int)Actions->Action[ActionPos].Param[0]);
			break;
		}
	}



void PLAYER::DoNextAction()
	{

	/////////////////////////////////////
	//								   //
	// Used to perform the next action //
	//								   //
	/////////////////////////////////////


	// Increment the action position 

	ActionPos++;


	// Check if the actions are over 

	if (ActionPos == Actions->NumActions)
		ActionPos = Actions->LoopToAction;


	// If at a valid action then perform it  

	if (ActionPos != -1)
		DoAction();
	}







void PLAYER::Stop()
	{
	AccX	= 0;
	AccY	= 0;
	VelX	= 0;
	VelY	 = 0;
	DeltaWX = 0;
	DeltaWY = 0;

	MoveDir = DIR_NONE;

	Animate(FALSE);
	}



/*void PLAYER::Run(int dir)
	{

	// If moving for the first time, start the run animation //

	if (MoveDir == DIR_NONE)
		{
		SetAnimation(RunAnim);
		SetAnimationSpeed(3);
		SetAnimationPos(0);
		Animate(TRUE);
		}



	// Set the acceleration //

	AccX = MaxAccX;
	AccY = MaxAccY;



	// Decrease speed if making a drastic turn //

	if (dir != MoveDir)
		{
		VelX /= 2;
		VelY /= 2;
		}


	// Set the new move direction //

	MoveDir = dir;
	FaceDir = dir;


	// Set the new deltas //

	switch(MoveDir)
		{
		case DIR_UP:
			DeltaWX = 0;
			DeltaWY = -VelY;
			break;


		case DIR_DOWN:
			DeltaWX = 0;
			DeltaWY = VelY;
			break;


		case DIR_LEFT:
			DeltaWX = -VelX;
			DeltaWY = 0;
			break;


		case DIR_RIGHT:
			DeltaWX = VelX;
			DeltaWY = 0;
			break;


		case DIR_UPLEFT:
			DeltaWX = -VelX;
			DeltaWY = -VelY;
			break;


		case DIR_UPRIGHT:
			DeltaWX = VelX;
			DeltaWY = -VelY;
			break;


		case DIR_DOWNLEFT:
			DeltaWX = -VelX;
			DeltaWY = VelY;
			break;


		case DIR_DOWNRIGHT:
			DeltaWX = VelX;
			DeltaWY = VelY;
			break;
		}
	}*/





void PLAYER::OnBallReceived(PLAYER * target)
	{

/*	// If this player received the ball then 
	// set UserControl to true				 

	if (target == this)
		UserControl = TRUE;
	
	
	// Otherwise ... 

	else
		{

		// If the player who received the ball is on this player's team
		// then block for that player and set UserControl to false

		if (target->Side == Side)
			{
			UserControl = FALSE;

			State = STATE_BLOCK;
			}

		// else if the player who received the ball is on the other team
		// then track that player

		else
			//TrackPlayer(target);
			{
			State = STATE_TRACKPLAYER;
			Target = target;
			}
		}*/
	}




void PLAYER::UpdateDeltas()
	{

	///////////////////////////////////////////////
	//											 //
	// Used to calculate the new delta values by //
	// the move direction						 //
	//											 //
	///////////////////////////////////////////////

	DeltaWX = 0;
	DeltaWY = 0;

	if (MoveDir & DIR_LEFT)		DeltaWX = -VelX;
	if (MoveDir & DIR_RIGHT)	DeltaWX = VelX;
	if (MoveDir & DIR_UP)		DeltaWY = -VelY;
	if (MoveDir & DIR_DOWN)		DeltaWY = VelY;
	}