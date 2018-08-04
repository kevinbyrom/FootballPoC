#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "PlayerObj.h"
#include "Football.h"
#include "MathUtils.h"




/////////////////
// ACTION CODE //
/////////////////




		
void PLAYER_OBJ::BeginActions()
	{ 

	//////////////////////////////////
	//								//
	// Used to begin action control //
	//								//
	//////////////////////////////////


	ControlType	= CONTROL_ACTIONS;

	DoAction(ENTER_ACTION);
	}




BEGIN_ACTION_MAP(PLAYER_OBJ, OBJECT)
	ACTION( STAND, DoAction_Stand )
	ACTION( STANCE, DoAction_Stance )
	ACTION( RUN, DoAction_Run )
	ACTION( RUN_TOSS, DoAction_RunToss )
	ACTION( RUN_TO, DoAction_RunTo )
	ACTION( FACE, DoAction_Face )
	ACTION( BLOCK, DoAction_Block )
	ACTION( HANDOFF, DoAction_HandOff )
	ACTION( FAKE_HANDOFF, DoAction_FakeHandOff )
	ACTION( TOSS, DoAction_Toss )
	ACTION( SNAP, DoAction_Snap )
	ACTION( PUNT, DoAction_Punt )
	ACTION( KICK, DoAction_Kick )
	ACTION( AIM_KICK, DoAction_AimKick )
	ACTION( END_AIM, DoAction_EndAim )
	ACTION( WAIT_FOR_SNAP_QB, DoAction_WaitForSnapQB)
	ACTION( WAIT_FOR_SNAP_PH, DoAction_WaitForSnapPH)
	ACTION( PLACEHOLD, DoAction_PlaceHold)
	ACTION( RUNTO_BALL_HOLDER, DoAction_RunToBallHolder)
	ACTION( FIND_RECEIVER, DoAction_FindReceiver)
	ACTION( FIND_CLOSEST_RECEIVER, DoAction_FindClosestReceiver)
	ACTION( FIND_RECEIVER_FOR_MAN, DoAction_FindReceiverForMan)
	ACTION( WAIT_FOR_RECEIVER, DoAction_WaitForReceiver)
	ACTION( WAIT_FOR_ANY_RECEIVER, DoAction_WaitForAnyReceiver)
	ACTION( SPY_TARGET,	DoAction_SpyTarget)
	ACTION( SAFETY_WAIT_HIGH, DoAction_SafetyWaitHigh)
	ACTION( SAFETY_WAIT_LOW, DoAction_SafetyWaitLow)
	ACTION( COVER_RECEIVER, DoAction_CoverReceiver)
	ACTION( BLOCK_FOR_BALL_HOLDER, DoAction_BlockForBallHolder)
	ACTION( TACKLE_BALL_HOLDER, DoAction_TackleBallHolder)
	ACTION( CATCH_BALL, DoAction_CatchBall)
	ACTION( RECOVER_FUMBLE, DoAction_RecoverFumble)
	ACTION( TARGET_PASSER, DoAction_TargetPasser)
	ACTION( TARGET_RUSHER, DoAction_TargetRusher)
	ACTION( TARGET_PLACEHOLDER, DoAction_TargetPlaceholder)
	ACTION( ASSIGN_RECEIVER_NUM, DoAction_AssignReceiverNum)
	ACTION( USER_CONTROL, DoAction_UserControl)
END_ACTION_MAP()




void PLAYER_OBJ::DoAction_Stand(int reason)
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to do the action : Stand by simply standing //
	//													//
	//////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		Stand();
		}
	}




void PLAYER_OBJ::DoAction_Stance(int reason)
	{

	//////////////////////////////////////////////////////////////////
	//																//
	// Used to do the action : Stance by placing the player in the  //
	// three point stance											//
	//																//
	//////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		Stance();
		}
	}




void PLAYER_OBJ::DoAction_Run(int reason)
	{

	///////////////////////////////////////////////////////////////////////	
	//													                 //
	// Used to do the action : Run by running in the specified direction //
	//													                 //
	///////////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		Run(GameInfo.Direction[TeamNum] == DIR_LEFT ? -Actions.GetParam(0) : Actions.GetParam(0), Actions.GetParam(1));
		}
	}




void PLAYER_OBJ::DoAction_Face(int reason)
	{


	////////////////////////////////////////////////////////////////////
	//													              //
	// Used to do the action : Face by facing the specified direction //
	//																  //
	////////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		Face(GameInfo.Direction[TeamNum] == DIR_LEFT ? -Actions.GetParam(0) : Actions.GetParam(0), Actions.GetParam(1));
		}
	}




void PLAYER_OBJ::DoAction_RunToss(int reason)
	{

	///////////////////////////////////////////////////////////////////////////
	//																		 //
	// Used to do the action : RunToss by running to the direction specified //
	// and catching the toss												 //
	//																		 //
	///////////////////////////////////////////////////////////////////////////


	float xdir = Actions.GetParam(0);
	float ydir = Actions.GetParam(1);



	ON_ACTION_UPDATE(reason)
		{
		Run(GameInfo.Direction[TeamNum] == DIR_LEFT ? -xdir : xdir, ydir);


		if (Collision(&Ball))
			{

			// Clear the actions

			DoNextAction();


			// Add ACTION_DELAY and ACTION_CATCHBALL again


			// Try to catch the ball

			TryToCatch();
			}
		}
	}




void PLAYER_OBJ::DoAction_RunTo(int reason)
	{

	////////////////////////////////////////////////////////////////////
	//																  //
	// Used to do the action : RunTo by running to the spot specified //
	// by the scrimmage pos plus the offsets						  //
	//																  //
	////////////////////////////////////////////////////////////////////


	float xoffset, yoffset;
	float xpos, ypos;
	float xdir, ydir;


	ON_ACTION_UPDATE(reason)
		{
		xoffset = (float)Actions.GetParam(0);
		yoffset = (float)Actions.GetParam(1);


		// Calculate the x & y world positions to run to

		if (GameInfo.Direction[TeamNum] == DIR_LEFT)
			{
			xpos = DownInfo.ScrimmagePos - xoffset;
			ypos = yoffset;
			}
		else
			{
			xpos = DownInfo.ScrimmagePos + xoffset;
			ypos = yoffset;
			}


		// If the current coordinates of the player is within range of the dest then pop the action

		if (fabs(Wld.X - xpos) <= (Width / 4) && fabs(Wld.Y - ypos) <= (Width / 4))
			{
			DoNextAction();
			}

		// Otherwise, continue to run to the spot

		else
			{
			GetDirectionToSpot(xpos, ypos, &xdir, &ydir);
			Run(xdir, ydir);
			}
		}
	}




void PLAYER_OBJ::DoAction_CatchBall(int reason)
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to do the action : CatchBall by running to //
	// and catching the ball						   //
	//												   //
	/////////////////////////////////////////////////////


	float dist,
		  dirx,
		  diry;


	ON_ACTION_UPDATE(reason)
		{
		// Return if grappling

		if (State == STATE_GRAPPLE || State == STATE_BREAKGRAPPLE)
			return;


		// Get the distance to the landing spot

		dist = GetDistanceToSpot(Dest.X, Dest.Y);


		// Get the direction to the landing spot

		GetDirectionToSpot(Dest.X, Dest.Y, &dirx, &diry);


		// If standing on the spot, then face the ball

		if (dist < Width / 4)
			{
			Stand();
			}
		else
			Run(dirx, diry);


		if (Collision(&Ball))
			{

			// Clear the actions

			Actions.Clear();


			// Add ACTION_DELAY and ACTION_CATCHBALL again


			// Try to catch the ball

			TryToCatch();
			}
		}
	}




void PLAYER_OBJ::DoAction_Block(int reason)
	{

	//////////////////////////////////////////////////////////////
	//														    //
	// Used to do the action : Block by waiting for an opponent //
	// to get near the player and blocking him					//
	//														    //
	//////////////////////////////////////////////////////////////


	float dirx, diry;


	ON_ACTION_UPDATE(reason)
		{
		// If grappling, just return

		if (State == STATE_GRAPPLE || State == STATE_BREAKGRAPPLE)
			return;


		// If an opponent hasn't been found to block or the target is grappled find a new target

		if (TrackTarget == NULL || TrackTarget->State == STATE_BREAKGRAPPLE || TrackCount >= FPS * 2)
			{
			SetTrackTarget(FindNearestPlayerToBlock());

			if (TrackTarget == NULL)
				return;
			}


		// If standing or running...

		if (State == STATE_STAND || State == STATE_RUN || State == STATE_STANCE)
			{

			// If standing on the target, attempt to grapple him

			if (Collision(TrackTarget))
				{
				if (SendMessage(TrackTarget->GetMsgTargetID(), MSG_GRAPPLED, 0, (DWORD)this) == TRUE)
					{
					GetDirectionToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y, &dirx, &diry);
					
					GrappleTarget = TrackTarget;

					Grapple(dirx, diry);
					}
				}

			// Otherwise, run to the target

			else
				{
				float xdir, ydir, dist;

				dist = GetDistanceToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y);

				if (dist <= Width)
					{
					GetDirectionToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y, &xdir, &ydir);
					Run(xdir, ydir);
					}
				else
					{
					if (GameInfo.Direction[TeamNum] == DIR_LEFT)
						Face(-1.0f, 0.0f);
					else
						Face(1.0f, 0.0f);

					Stand();
					}
				}
			}
		}
	}





void PLAYER_OBJ::DoAction_HandOff(int reason)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to perform the action : Handoff by performing the	  //
	// handoff state and handing the ball to the nearest receiver //
	//															  //
	////////////////////////////////////////////////////////////////



	PLAYER_OBJ * target = Side[SideNum].Rusher;


	ON_ACTION_UPDATE(reason)
		{
		// Start the handoff

		HandOff();


		// If handoff state reaches a certain time

		if (StateDone && Collision(target))
			{

			// Give the target the ball

			HandOverBall(target);

			DoNextAction();

			Stand();
			}
		}
	}




void PLAYER_OBJ::DoAction_FakeHandOff(int reason)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to perform the action : FakeHandoff by performing the //
	// handoff state											  //
	//															  //
	////////////////////////////////////////////////////////////////


	PLAYER_OBJ * target = Side[SideNum].Rusher;


	ON_ACTION_UPDATE(reason)
		{

		// Start the fake handoff

		FakeHandOff();


		// If fake handoff state reaches a certain time

		if (StateDone && Collision(target))
			{

			// Pop the current action

			DoNextAction();

			Stand();
			}
		}
	}





void PLAYER_OBJ::DoAction_Toss(int reason)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to perform the action : Toss by performing the //
	// toss state										   //
	//													   //
	/////////////////////////////////////////////////////////


	PLAYER_OBJ * target = Side[SideNum].Rusher;


	ON_ACTION_UPDATE(reason)
		{
		// Start the handoff

		Toss();


		// If handoff state reaches a certain time

		if (StateDone)
			{

			// Toss the ball to the target

			TossBallToPlayer(target);

			DoNextAction();

			Stand();
			}
		}
	}




void PLAYER_OBJ::DoAction_Punt(int reason)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to perform the action : Punt by performing the //
	// punt state										   //
	//													   //
	/////////////////////////////////////////////////////////


	// Start the punt

	ON_ACTION_ENTER(reason)
		{
		
		Punt();

		}

	ON_ACTION_UPDATE(reason)
		{

		// If handoff state reaches a certain time

		if (StateDone)
			{

			// Punt the ball

			PuntBall();

			DoNextAction();

			Stand();
			}
		}
	}




void PLAYER_OBJ::DoAction_Snap(int reason)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to perform the action : Snap by performing the //
	// snap state										   //
	//													   //
	/////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		Snap();
		DoNextAction();
		}
	}




void PLAYER_OBJ::DoAction_PlaceHold(int reason)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to perform the action : Placehold by performing the //
	// placehold state										    //
	//															//
	//////////////////////////////////////////////////////////////


	ON_ACTION_ENTER(reason)
		{
		Placehold();
		}

	ON_ACTION_UPDATE(reason)
		{
		if (StateDone)
			{
			DoNextAction();
			}
		}
	}





void PLAYER_OBJ::DoAction_WaitForSnapQB(int reason)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to perform the action : WaitForSnap by performing the //
	// wait for snap state										  //
	//															  //
	////////////////////////////////////////////////////////////////



	ON_ACTION_UPDATE(reason)
		{

		// Return if grappling

		if (State == STATE_GRAPPLE || State == STATE_BREAKGRAPPLE)
			return;


		// Stand and wait

		Stand();


		if (Collision(&Ball))
			{
			// Try to catch the snap

			TryToCatchSnap();
			DoNextAction();
			}
		}
	}




void PLAYER_OBJ::DoAction_WaitForSnapPH(int reason)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to perform the action : WaitForSnap by performing the //
	// wait for snap state										  //
	//															  //
	////////////////////////////////////////////////////////////////



	ON_ACTION_UPDATE(reason)
		{

		// Return if grappling

		if (State == STATE_GRAPPLE || State == STATE_BREAKGRAPPLE)
			return;


		// Stand and wait

		Stand();


 		if (Collision(&Ball))
			{
			// Try to catch the snap

			TryToCatchSnap();
			DoNextAction();
			}
		}
	}





void PLAYER_OBJ::DoAction_RunToBallHolder(int reason)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to perform the action : RunToBallHolder by running to //
	// the ball holder											  //
	//															  //
	////////////////////////////////////////////////////////////////


	float xdir, ydir;


	ON_ACTION_UPDATE(reason)
		{
		if (Ball.Holder)
			{
			GetDirectionToSpot(Ball.Holder->Wld.X, Ball.Holder->Wld.Y, &xdir, &ydir);
			Run(xdir, ydir);

			if (GetDistanceToSpot(Ball.Holder->Wld.X, Ball.Holder->Wld.Y) <= Actions.GetParam(0))
				DoNextAction();
			}
		else
			Stand();
		}
	}





void PLAYER_OBJ::DoAction_Spike(int reason)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to perform the action : Spike by throwing the ball //
	// at the player's feet to stop the play				   //
	//														   //
	/////////////////////////////////////////////////////////////
	}





void PLAYER_OBJ::DoAction_Kneel(int reason)
	{

	////////////////////////////////////////////////////////////
	//														  //
	// Used to perform the action : Kneel by kneeling down to //
	// stop the play										  //
	//														  //
	////////////////////////////////////////////////////////////
	}





//void PLAYER_OBJ::DoAction_PlaceHold(int reason)
//	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to perform the action : PlaceHold by holding the //
	// ball in place										 //
	//														 //
	///////////////////////////////////////////////////////////
//	}





void PLAYER_OBJ::DoAction_AimKick(int reason)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to perform the action : AimKick by sending a		//
	// StartAIM Message to the play and waiting for a keypress	//
	//														    //
	//////////////////////////////////////////////////////////////



	ON_ACTION_UPDATE(reason)
		{
		AimKick();

		if (StateDone)
			{
			DoNextAction();
			}
		}
	}





void PLAYER_OBJ::DoAction_EndAim(int reason)
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to perform the action : EndAim by ending the //
	// aim sequence for kicking the ball				 //
	//													 //
	///////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		SendMessage(0, MSG_END_AIM, (DWORD)1.0f, 0);
		DoNextAction();
		Stand();
		}
	}





void PLAYER_OBJ::DoAction_Kick(int reason)
	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to perform the action : Kick by kicking the ball //
	// towards the goal posts								 //
	//														 //
	///////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{

		// Start the kick

		Kick();


		// If handoff state reaches a certain time

		if (StateDone)
			{

			// Punt the ball

			KickBall();

			DoNextAction();

			Stand();
			}
		}
	}





void PLAYER_OBJ::DoAction_FindReceiver(int reason)
	{

	//////////////////////////////////////////////////////////
	//												        //
	// Used to perform the action : FindReceiver by finding //
	// the closest untargeted receiver                      //
	//												        //
	//////////////////////////////////////////////////////////
	}





void PLAYER_OBJ::DoAction_FindClosestReceiver(int reason)
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Used to perform the action : FindClosestReceiver by finding //
	// the closest untargeted receiver							   //
	//															   //
	/////////////////////////////////////////////////////////////////



	ON_ACTION_UPDATE(reason)
		{
		TrackTarget = FindClosestUntargetedReceiver();
		DoNextAction();
		}
	}





void PLAYER_OBJ::DoAction_FindReceiverForMan(int reason)
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Used to perform the action : FindReceiverForMan by finding  //
	// the receiver whose receiver num and route dir match a given // 
	// value 													   //
	//															   //
	/////////////////////////////////////////////////////////////////



	ON_ACTION_UPDATE(reason)
		{
		TrackTarget = FindReceiverForMan(Actions.GetParam(0), Actions.GetParam(1));
		DoNextAction();
		}
	}





void PLAYER_OBJ::DoAction_LineUpToReceiver(int reason)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to perform the action : LineUpToReceiver by lining //
	// up to the targeted receiver							   //
	//														   //
	/////////////////////////////////////////////////////////////
	}





void PLAYER_OBJ::DoAction_WaitForReceiver(int reason)
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Used to perform the action : WaitForReceiver by waiting for //
	// the receiver to get within dist range					   //
	//															   //
	/////////////////////////////////////////////////////////////////


	float distx = (float)Actions.GetParam(0);
	float disty = (float)Actions.GetParam(1);


	ON_ACTION_UPDATE(reason)
		{

		// If a receiver is not locked on, just return

		if (TrackTarget == NULL)
			return;


		// If both requirements are met, pop the action

		if (IsPlayerInRange(TrackTarget, distx, disty))
			DoNextAction();
		}
	}





void PLAYER_OBJ::DoAction_WaitForAnyReceiver(int reason)
	{

	////////////////////////////////////////////////////////////////////
	//																  //
	// Used to perform the action : WaitForAnyReceiver by waiting for //
	// any receiver to get within dist range						  //
	//																  //
	////////////////////////////////////////////////////////////////////


	float distx = (float)Actions.GetParam(0);
	float disty = (float)Actions.GetParam(1);
	}





void PLAYER_OBJ::DoAction_SpyTarget(int reason)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to perform the action : SpyTarget by floating with //
	// the target on the Y axis								   //
	//														   //
	/////////////////////////////////////////////////////////////


	float xdir, ydir;


	ON_ACTION_UPDATE(reason)
		{

		// If a target is not locked on, just return

		if (TrackTarget == NULL)
			return;


		// Get the direction to the target

		if (GameInfo.Direction[TeamNum] == DIR_LEFT)
			GetDirectionToSpot(Wld.X, TrackTarget->Wld.Y, &xdir, &ydir);
		else
			GetDirectionToSpot(Wld.X, TrackTarget->Wld.Y, &xdir, &ydir);


		if (TrackTarget->Wld.Y < Wld.Y - 30.0f || TrackTarget->Wld.Y > Wld.Y + 30.0f)
			{
			Run(xdir, ydir);
			}
		else
			{
			Face(-1.0f, ydir);
			Stand();
			}
		}
	}



void PLAYER_OBJ::DoAction_SafetyWaitHigh(int reason)
	{

	///////////////////////////////////////////////////////////////////////////
	//																		 //
	// Used to perform the action : SafetyWaitHigh by waiting for any		 //
	// receiver to get near the safety who is on the upper part of the field //
	//																		 //
	///////////////////////////////////////////////////////////////////////////

	
	float distx = (float)Actions.GetParam(0);
	float disty = (float)Actions.GetParam(1);
	int mincover = (int)Actions.GetParam(2);
	PLAYER_OBJ * target;
	int i;


	ON_ACTION_UPDATE(reason)
		{
		Stand();


		for (i = 0; i < MAX_SIDE_PLAYERS; i++)
			{

			target = &Side[!SideNum].Player[i];


			// Find a receiver in upper field

			if (target->Wld.Y < 0)
				{


				// Check if the receiver is in range
				
				if (IsPlayerInRange(target, distx, disty))
					{


					// Check if the number of defenders on receiver is less than mincover

					if (target->GetNumberOfDefendersNear() < mincover)
						{


						// Set the TrackTarget to the receiver

						TrackTarget = target;


						// Pop this action & return
						
						DoNextAction();

						return;
						}
					}
				}
			}

		// Delay the action

		Actions.Delay(FPS / 4);
		}
	}





void PLAYER_OBJ::DoAction_SafetyWaitLow(int reason)
	{

	///////////////////////////////////////////////////////////////////////////
	//																		 //
	// Used to perform the action : SafetyWaitLow by waiting for any		 //
	// receiver to get near the safety who is on the lower part of the field //
	//																		 //
	///////////////////////////////////////////////////////////////////////////


	float distx = (float)Actions.GetParam(0);
	float disty = (float)Actions.GetParam(1);
	int mincover = (int)Actions.GetParam(2);
	PLAYER_OBJ * target;
	int i;


	ON_ACTION_UPDATE(reason)
		{
		Stand();


		for (i = 0; i < MAX_SIDE_PLAYERS; i++)
			{

			target = &Side[!SideNum].Player[i];


			// Find a receiver in lower field

			if (target->Wld.Y >= 0)
				{


				// Check if the receiver is in range
				
				if (IsPlayerInRange(target, distx, disty))
					{


					// Check if the number of defenders on receiver is less than mincover

					if (target->GetNumberOfDefendersNear() < mincover)
						{


						// Set the TrackTarget to the receiver

						TrackTarget = target;


						// Pop this action & return
						
						DoNextAction();

						return;
						}
					}
				}
			}


		// Delay the action

		Actions.Delay(FPS / 4);
		}
	}







void PLAYER_OBJ::DoAction_CoverReceiver(int reason)
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Used to perform the action : CoverReceiver by sticking with //
	// the targeted receiver and staying withing dist range		   //
	//															   //
	/////////////////////////////////////////////////////////////////


	float distx = (float)Actions.GetParam(0);
	float disty = (float)Actions.GetParam(1);
	float xdir, ydir;
	float dx, dy;


	ON_ACTION_UPDATE(reason)
		{
		// If a receiver is not locked on, just return

		if (TrackTarget == NULL)
			return;


		// Get the direction to the receiver

		if (GameInfo.Direction[TeamNum] == DIR_LEFT)
			{
			if (Wld.X > TrackTarget->Wld.X + distx)
				{
				dx = TrackTarget->Wld.X + distx + ((Wld.X - TrackTarget->Wld.X) / 2);
				dy = TrackTarget->Wld.Y + disty;
				}
			else
				{
				dx = TrackTarget->Wld.X + distx;
				dy = TrackTarget->Wld.Y + disty;
				}
			}
		else
			{
			if (Wld.X < TrackTarget->Wld.X - distx)
				{
				dx = TrackTarget->Wld.X - ((TrackTarget->Wld.X - Wld.X) / 2);
				dy = TrackTarget->Wld.Y + disty;
				}
			else
				{
				dx = TrackTarget->Wld.X - distx;
				dy = TrackTarget->Wld.Y + disty;
				}
			//GetDirectionToSpot(TrackTarget->Wld.X - distx, TrackTarget->Wld.Y + disty, &xdir, &ydir);
			}


		GetDirectionToSpot(dx, dy, &xdir, &ydir);

		Run(xdir, ydir);
		}
	}





void PLAYER_OBJ::DoAction_CatchPunt(int reason)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to perform the action : CatchPunt by moving to the //
	// ball catch spot to catch the punt					   //
	//														   //
	/////////////////////////////////////////////////////////////
	}





void PLAYER_OBJ::DoAction_BlockForBallHolder(int reason)
	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to do the action : BlockForBallHolder by finding //
	// the nearest unblocked opponent and grappling with him //
	//														 //
	///////////////////////////////////////////////////////////


	float dirx, diry;


	ON_ACTION_UPDATE(reason)
		{

		// If grappling, just return

		if (State == STATE_GRAPPLE || State == STATE_BREAKGRAPPLE)
			return;


		// If an opponent hasn't been found to block or the target is grappled find a new target

		if (TrackTarget == NULL || 
			TrackTarget->State == STATE_BREAKGRAPPLE || 
			TrackTarget->State == STATE_TUMBLE ||
			TrackTarget->State == STATE_LAYDOWN ||
			TrackCount >= FPS * 3)
			{
			SetTrackTarget(FindNearestPlayerToBlock());

			if (TrackTarget == NULL)
				return;
			}


		// If standing or running...

		if (State == STATE_STAND || State == STATE_RUN)
			{

			// If standing on the target, attempt to grapple him

			if (Collision(TrackTarget))
				{
				if (SendMessage(TrackTarget->GetMsgTargetID(), MSG_GRAPPLED, 0, (DWORD)this) == TRUE)
					{
					GetDirectionToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y, &dirx, &diry);
					
					GrappleTarget = TrackTarget;

					if (TryToKnockDown(GrappleTarget))
						Stand();
					else
						Grapple(dirx, diry);
					}
				}

			// Otherwise, run to the target
 
			else
				{
				float xdir, ydir;

				GetDirectionToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y, &xdir, &ydir);
				Run(xdir, ydir);
				}
			}
		}
	}





void PLAYER_OBJ::DoAction_TackleBallHolder(int reason)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to do the action : TackleBallHolder by chasing //
	// down and tackling the person holding the ball	   //
	//													   //
	/////////////////////////////////////////////////////////


	float dist,
		  dirx,
		  diry;


	ON_ACTION_UPDATE(reason)
		{

		// If grappling, just return

		if (State != STATE_STAND && State != STATE_RUN && State != STATE_STANCE)
			return;


		// Make sure TrackTarget is set

		if (TrackTarget == NULL)
			{
			if (Ball.Holder && Ball.Holder->SideNum != SideNum)
				TrackTarget = Ball.Holder;
			else
				return;
			}


		// Get the distance to the ball holder

		dist = GetDistanceToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y);


		// Get the direction to the ball holder

		GetDirectionToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y, &dirx, &diry);


		// Add in an influence factor

		//dirx = (dirx + TrackTarget->MoveDir.X) / 2;
		
		//if (TrackTarget->MoveDir.Y)
		//	diry = (diry + TrackTarget->MoveDir.Y) / 2;
			

		if (Collision(TrackTarget))
			{
			if (State == STATE_DIVE)
				{
				SendMessage(TrackTarget->GetMsgTargetID(), MSG_TACKLED, 0, (DWORD)this);
				Actions.Clear();
				Actions.PushEnd(ACTION_DELAY, FPS / 2);
				Actions.PushEnd(ACTION_TACKLE_BALL_HOLDER);
				}
			else
				{
				if (SendMessage(TrackTarget->GetMsgTargetID(), MSG_GRAPPLED, 0, (DWORD)this) == TRUE)
					{
					GetDirectionToSpot(TrackTarget->Wld.X, TrackTarget->Wld.Y, &dirx, &diry);
					
					GrappleTarget = TrackTarget;

					Grapple(dirx, diry);
					}
				}
			}
		else if (dist < (Width * 2) && TrackTarget->PlayerInfo->Position != DEPTH_QB)
			Dive(dirx, diry);
		else
			Run(dirx, diry);


		Actions.Delay(FPS / 4);
		}
	}





void PLAYER_OBJ::DoAction_RecoverFumble(int reason)
	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to do the action : RECOVERFUMBLE by chasing down //
	// the ball and picking it up							 //
	//														 //
	///////////////////////////////////////////////////////////


	float dist,
		  dirx,
		  diry;


	ON_ACTION_UPDATE(reason)
		{

		// Get the distance to the ball

		dist = GetDistanceToSpot(Ball.Wld.X, Ball.Wld.Y);


		// Get the direction to the ball 

		GetDirectionToSpot(Ball.Wld.X, Ball.Wld.Y, &dirx, &diry);
		

		if (dist < Ball.Width / 2)
			{
			SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_CAUGHT, 0, (DWORD)this);
			}
		else if (dist < Ball.Width * 2)
			Dive(dirx, diry);
		else
			Run(dirx, diry);
		}
	}




void PLAYER_OBJ::DoAction_TargetPasser(int reason)
	{
	////////////////////////////////////////////////////////////////////////
	//																	  //
	// Used to do the action : TargetPasser by setting the target pointer //
	// to the passer of the team										  //
	//																	  //
	////////////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		SetTrackTarget(Side[OFFENSE].Passer);
		DoNextAction();
		}
	}




void PLAYER_OBJ::DoAction_TargetRusher(int reason)
	{

	////////////////////////////////////////////////////////////////////////
	//																	  //
	// Used to do the action : TargetRusher by setting the target pointer //
	// to the rusher of the team										  //
	//																	  //
	////////////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		SetTrackTarget(Side[OFFENSE].Rusher);
		DoNextAction();
		}
	}




void PLAYER_OBJ::DoAction_TargetPlaceholder(int reason)
	{

	/////////////////////////////////////////////////////////////////////////////
	//																	       //
	// Used to do the action : TargetPlaceholder by setting the target pointer //
	// to the placeholder of the team										   //
	//																	       //
	/////////////////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		SetTrackTarget(Side[OFFENSE].Placeholder);

		DoNextAction();
		}
	}




void PLAYER_OBJ::DoAction_AssignReceiverNum(int reason)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to do the action : AssignReceiverNum by setting the //
	// ReceiverNum and RouteDir properties based on the given   //
	// parameters												//
	//															//
	//////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		ReceiverNum = (int)Actions.GetParam(0);
		RouteDir = (int)Actions.GetParam(1);
	
		DoNextAction();
		}
	}





void PLAYER_OBJ::DoAction_UserControl(int reason)
	{

	/////////////////////////////////////////////////////////////////////////
	//																	   //
	// Used to do the action : UserControl by allowing the user to control //
	// the current player												   //
	//																	   //
	/////////////////////////////////////////////////////////////////////////


	ON_ACTION_UPDATE(reason)
		{
		Side[SideNum].ChangeUserControl(this);
	
		DoNextAction();
		}
	}