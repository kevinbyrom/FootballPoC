#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "PlayerObj.h"
#include "Football.h"
#include "MathUtils.h"




////////////////
// Animations //
////////////////


int RunAnim[]		= { 0,1,0,2,ANIM_LOOP,0 };
int GrappleAnim[]	= { 0,1,ANIM_LOOP,0 };
int ThrowAnim[]		= { 0,1,ANIM_LOOP,0 };
int CelebrateAnim[]	= { 0,1,ANIM_LOOP,0 };
int ComplainAnim[]	= { 0,1,ANIM_LOOP,0 };
int LeapAnim[]		= { 1,ANIM_LOOP,0 };
int SitAnim[]		= { 0,1,ANIM_LOOP,0 };
int TumbleAnim[]	= { 0, 2, ANIM_LOOP, 0 };




BEGIN_STATE_MAP(PLAYER_OBJ, OBJECT)
	STATE( STAND, DoState_Stand )
	STATE( STANCE, DoState_Stance )
	STATE( RUN, DoState_Run )
	STATE( SLOWDOWN, DoState_SlowDown )
	STATE( LEAP, DoState_Leap )
	STATE( DIVE, DoState_Dive )
	STATE( GRAPPLE, DoState_Grapple )
	STATE( BREAKGRAPPLE, DoState_BreakGrapple )
	STATE( FAKEHANDOFF, DoState_FakeHandOff )
	STATE( TUMBLE, DoState_Tumble )
	STATE( LAYDOWN, DoState_LayDown )
	STATE( SIT, DoState_Sit )
	STATE( GETUP, DoState_GetUp )
	STATE( HANDOFF, DoState_HandOff )
	STATE( FAKEHANDOFF, DoState_FakeHandOff )
	STATE( TOSS, DoState_Toss)
	STATE( SNAP, DoState_Snap)
	STATE( THROW, DoState_Throw)
	STATE( KICK, DoState_Kick)
	STATE( AIMKICK, DoState_AimKick)
	STATE( PUNT, DoState_Punt)
	STATE( PLACEHOLD, DoState_Placehold)
	STATE( CELEBRATE, DoState_Celebrate)
	STATE( COMPLAIN, DoState_Complain)
END_STATE_MAP()





void PLAYER_OBJ::DoState_Stand(int reason)
	{

	///////////////////////////////////////
	//									 //
	// Used to perform the state : STAND //
	//									 //
	///////////////////////////////////////


	ON_STATE_ENTER(reason)
		{
		Stop();
		Animate(FALSE);
		}


	ON_STATE_UPDATE(reason)
		{
		// Increase stamina a little

		Damage(STAND_DAMAGE);

			
		// Set the frame //

		Frame = FRAME_STAND + ((FaceDir - 1) * NUM_SPRTILES_X);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		float xdir = 0.0f;
		float ydir = 0.0f;


		// Check for running direction 

		if (VPad[PadNum].Left())
			xdir		= -1.0f;
		else if (VPad[PadNum].Right())
			xdir		= 1.0f;

		if (VPad[PadNum].Up())
			ydir		= -1.0f;
		else if (VPad[PadNum].Down())
			ydir		= 1.0f;

		if (xdir != 0 && ydir != 0)
			{
			xdir *= .71;
			ydir *= .71;
			}


		// Check for running 

		if (xdir != 0.0f || ydir != 0.0f)
			Run(xdir, ydir);


		// Check for passes && target receiver changes

		if (SideNum == OFFENSE && Flag.Passer && HasBall && !DownInfo.BallPassed && !DownInfo.PassedScrimmage)
			{
			if (VPad[PadNum].Button(0))
				{
				Stand();
				Throw();
				}
			else if (VPad[PadNum].ButtonClicked(1))
				{
				Side[SideNum].ChangeTargetReceiver();
				}
			}
		}
	}





void PLAYER_OBJ::DoState_Stance(int reason)
	{

	////////////////////////////////////////
	//									  //
	// Used to perform the state : STANCE //
	//									  //
	////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		// Set the frame //

		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_STANCE + FRAME_AMT_STANCE;
		//else
			Frame = FRAME_STANCE + ((FaceDir - 1) * NUM_SPRTILES_X);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Run(int reason)
	{

	/////////////////////////////////////
	//								   //
	// Used to perform the state : RUN //
	//								   //
	/////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SetAnimation(RunAnim);
		SetAnimationSpeed(10);
		SetAnimationPos(0);
		Animate(TRUE);


		// Set the acceleration and velocity //

		Acc.X = 0.01 + (0.04f * (PlayerInfo->Attrib.Accel.Value() / 100.0f));
		Acc.Y = 0.01 + (0.04f * (PlayerInfo->Attrib.Accel.Value() / 100.0f));

		DestVel.X = 1.0f + (1.0f * (PlayerInfo->Attrib.Speed.Value() / 100.0f));
		DestVel.Y = 1.0f + (1.0f * (PlayerInfo->Attrib.Speed.Value() / 100.0f));
		}


	ON_STATE_UPDATE(reason)
		{
		// Damage the stamina a little

		Damage(RUN_DAMAGE);

		
		// Set the animation speed 

		if (TotalVel >= 1.5f)
			SetAnimationSpeed(10);
		else
			SetAnimationSpeed(20);


		// Set the frame //

		Frame = FRAME_RUN + ((FaceDir - 1) * NUM_SPRTILES_X) + Anim.Frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		BOOL  dirkeypressed	= FALSE;
		float xdir			= 0.0f;
		float ydir			= 0.0f;


		// Check for running direction 

		if (VPad[PadNum].Left())
			xdir		= -1.0f;
		else if (VPad[PadNum].Right())
			xdir		= 1.0f;

		if (VPad[PadNum].Up())
			ydir		= -1.0f;
		else if (VPad[PadNum].Down())
			ydir		= 1.0f;

		if (xdir != 0 && ydir != 0)
			{
			xdir *= .71;
			ydir *= .71;
			}

		dirkeypressed = (xdir != 0.0f || ydir != 0.0f);


		// Check for running 

		if (dirkeypressed)
			Run(xdir, ydir);


		// Check for standing 

		else 
			SlowStop();


		// Check for button 0 presses

		if (VPad[PadNum].Button(0))
			{
			if (Flag.Passer && HasBall && !DownInfo.BallPassed && !DownInfo.PassedScrimmage)
				{
				Stand();
				Throw();
				}
			else if (xdir != 0.0f || ydir != 0.0f)
				{
				Dive(xdir, ydir);
				}
			}

		// Check for button 1 clicks 

		if (VPad[PadNum].ButtonClicked(1))
			{
			if (Flag.Passer && HasBall && !DownInfo.BallPassed)
				{
				Side[OFFENSE].ChangeTargetReceiver();
				}
			}
		}
	}





void PLAYER_OBJ::DoState_SlowDown(int reason)
	{

	//////////////////////////////////////////
	//									    //
	// Used to perform the state : SLOWDOWN //
	//									    //
	//////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SetAnimation(RunAnim);
		SetAnimationSpeed(15);
		SetAnimationPos(0);
		Animate(TRUE);


		// Set the acceleration //

		Acc.X = 0.05f;
		Acc.Y = 0.05f;

		DestVel.X = 0;
		DestVel.Y = 0;
		}


	ON_STATE_UPDATE(reason)
		{
		Frame = FRAME_RUN + ((FaceDir - 1) * NUM_SPRTILES_X) + Anim.Frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		BOOL  dirkeypressed	= FALSE;
		float xdir			= 0.0f;
		float ydir			= 0.0f;


		// Check for running direction 

		if (VPad[PadNum].Left())
			xdir		= -1.0f;
		else if (VPad[PadNum].Right())
			xdir		= 1.0f;

		if (VPad[PadNum].Up())
			ydir		= -1.0f;
		else if (VPad[PadNum].Down())
			ydir		= 1.0f;

		if (xdir != 0 && ydir != 0)
			{
			xdir *= .71;
			ydir *= .71;
			}

		dirkeypressed = (xdir != 0.0f || ydir != 0.0f);


		// Check for running 

		if (dirkeypressed)
			Run(xdir, ydir);


		// Check for standing 

		else if (TotalVel == 0.0f)
			{
			Stand();
			}


		// Check for button 0 presses

		if (VPad[PadNum].Button(0))
			{
			if (Flag.Passer && HasBall && !DownInfo.BallPassed && !DownInfo.PassedScrimmage)
				{
				Stand();
				Throw();
				}
			else if (xdir != 0.0f || ydir != 0.0f)
				{
				Dive(xdir, ydir);
				}
			}

		// Check for button 1 clicks 

		if (VPad[PadNum].ButtonClicked(1))
			{
			if (Flag.Passer && HasBall && !DownInfo.BallPassed)
				{
				Side[OFFENSE].ChangeTargetReceiver();
				}
			}
		}
	}





void PLAYER_OBJ::DoState_Leap(int reason)
	{

	//////////////////////////////////////
	//									//
	// Used to perform the state : LEAP //
	//									//
	//////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		// Set the leap animation 

		SetAnimation(LeapAnim);
		SetAnimationSpeed(3);
		SetAnimationPos(0);
		Animate(TRUE);

		// Set the Z delta 

		Delta.Z = 1.5;
		}


	ON_STATE_UPDATE(reason)
		{
		// If the player has landed, set the state to stand

		if (Wld.Z == 0 && Vel.Z <= 0)
			Stand();


		// Set the frame 

		Frame = FRAME_LEAP + ((FaceDir - 1) * NUM_SPRTILES_X);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Dive(int reason)
	{

	//////////////////////////////////////
	//									//
	// Used to perform the state : DIVE //
	//									//
	//////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SetAnimation(LeapAnim);
		SetAnimationSpeed(3);
		SetAnimationPos(0);
		Animate(TRUE);
		
		Acc.X		= 1;
		Acc.Y		= 1;

		DestVel.X	+= DIVE_SPEED;
		DestVel.Y	+= DIVE_SPEED;

		Vel.X		= DestVel.X;
		Vel.Y		= DestVel.Y;

		Wld.Z		= Height / 4;
		Delta.Z		= -GRAVITY;
		}


	ON_STATE_UPDATE(reason)
		{
		// If the player has landed...

		if (Wld.Z == 0.0f && Vel.Z <= 0.0f)
			{

			// Slow down

			Acc.X		= 0.05f;
			Acc.Y		= 0.05f;
			DestVel.X	= 0.0f;
			DestVel.Y	= 0.0f;


			// If the player is stopped, set the state to sit

			if (TotalVel == 0.0f)
				{
				Stop();
				Sit();
				}
			}


		// Set the frame 

		Frame = FRAME_DIVE + ((FaceDir - 1) * NUM_SPRTILES_X);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Grapple(int reason)
	{

	/////////////////////////////////////////
	//									   //
	// Used to perform the state : GRAPPLE //
	//									   //
	/////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SetAnimation(GrappleAnim);
		SetAnimationSpeed(10);
		SetAnimationPos(0);
		Animate(TRUE);

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		// Every second, check for take downs

		if (StateCount >= FPS)
			{
			StateCount = 0;

			if (TryToTakeDown(GrappleTarget))
				Stand();
			}


		// Set the frame 

		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_GRAPPLE + FRAME_AMT_GRAPPLE + Anim.Frame;
		//else
			Frame = FRAME_GRAPPLE + ((FaceDir - 1) * NUM_SPRTILES_X) + Anim.Frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_BreakGrapple(int reason)
	{

	//////////////////////////////////////////////
	//									        //
	// Used to perform the state : BREAKGRAPPLE //
	//									        //
	//////////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SetAnimation(GrappleAnim);
		SetAnimationSpeed(10);
		SetAnimationPos(0);
		Animate(TRUE);

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		// Damage the stamina a little

		Damage(GRAPPLE_DAMAGE);


		// Every second, check for a grapple break

		if (StateCount >= FPS)
			{
			StateCount = 0;
			TryToBreakGrapple();
			}

		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_GRAPPLE + FRAME_AMT_GRAPPLE + Anim.Frame;
		//else
			Frame = FRAME_GRAPPLE + ((FaceDir - 1) * NUM_SPRTILES_X) + Anim.Frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		if (VPad[PadNum].ButtonClicked(0))
			TryToBreakGrapple();
		}
	}





void PLAYER_OBJ::DoState_Tumble(int reason)
	{

	////////////////////////////////////////
	//									  //
	// Used to perform the state : TUMBLE //
	//									  //
	////////////////////////////////////////


	ON_STATE_ENTER(reason)
		{
		SetAnimation(TumbleAnim);
		SetAnimationSpeed(10);
		SetAnimationPos(0);
		Animate(TRUE);


		// Set the acceleration and velocity //

		Acc.X		= .05;
		Acc.Y		= .05;

		DestVel.X	= 0;
		DestVel.Y	= 0;

		Vel.X		= 2;
		Vel.Y		= 2;
		}


	ON_STATE_UPDATE(reason)
		{

		if (Vel.X <= 0.0f || Vel.Y <= 0.0f)
		{
			Stop();
			LayDown();
		}


		// Set the frame //

		Frame = FRAME_TUMBLE + ((FaceDir - 1) * NUM_SPRTILES_X) + Anim.Frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_LayDown(int reason)
	{

	/////////////////////////////////////////
	//									   //
	// Used to perform the state : LAYDOWN //
	//									   //
	/////////////////////////////////////////


	ON_STATE_ENTER(reason)
		{
		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		// If laying for a certain time, sit up

		if (StateCount >= FPS * 2)
			Sit();


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_LAY + FRAME_AMT_LAY;
		//else
			Frame = FRAME_LAY + ((FaceDir - 1) * NUM_SPRTILES_X);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Sit(int reason)
	{

	/////////////////////////////////////
	//								   //
	// Used to perform the state : SIT //
	//								   //
	/////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SetAnimation(SitAnim);
		SetAnimationSpeed(FPS / 4);
		SetAnimationPos(0);
		Animate(TRUE);

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		// If the state count is greater than 2 seconds then getup

		if (StateCount >= FPS * 2)
			GetUp();

		
		// Set the frame 

		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_SIT + FRAME_AMT_SIT + Anim.Frame;
		//else
			Frame = FRAME_SIT + ((FaceDir - 1) * NUM_SPRTILES_X) + Anim.Frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_GetUp(int reason)
	{

	///////////////////////////////////////
	//									 //
	// Used to perform the state : GETUP //
	//									 //
	///////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		int frame;


		// If the state count is less than 1/6 a second, set the frame to the first frame

		if (StateCount < (FPS / 6))
			frame = 0;

		// Else if the state is less than 1/3 a second, set the frame to the second frame

		else if (StateCount < (FPS / 3))
			frame = 1;

		// Else set the state to STAND

		else
			Stand();

		
		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_GETUP + FRAME_AMT_GETUP + (frame * 2);
		//else
			Frame = FRAME_GETUP + ((FaceDir - 1) * NUM_SPRTILES_X) + (frame * 2);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_HandOff(int reason)
	{

	/////////////////////////////////////////
	//									   //
	// Used to perform the state : HANDOFF //
	//									   //
	/////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		StateTime = -1;
		}


	ON_STATE_UPDATE(reason)
		{
		int frame;


		if (StateCount < FPS / 4)
			frame = 0;
		else
			{
			frame = 1;
			StateDone = TRUE;
			}


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_HANDOFF + FRAME_AMT_HANDOFF + (frame * 2);
		//else
			Frame = FRAME_HANDOFF + ((FaceDir - 1) * NUM_SPRTILES_X) + frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_FakeHandOff(int reason)
	{

	/////////////////////////////////////////////
	//									       //
	// Used to perform the state : FAKEHANDOFF //
	//									       //
	/////////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		StateTime = -1;
		}


	ON_STATE_UPDATE(reason)
		{
		int frame;


		if (StateCount < FPS / 4)
			frame = 0;
		else
			{
			frame = 1;
			StateDone = TRUE;
			}


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_HANDOFF + FRAME_AMT_HANDOFF + (frame * 2);
		//else
			Frame = FRAME_HANDOFF + ((FaceDir - 1) * NUM_SPRTILES_X) + frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Toss(int reason)
	{

	//////////////////////////////////////
	//									//
	// Used to perform the state : TOSS //
	//									//
	//////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		StateTime = -1;
		}


	ON_STATE_UPDATE(reason)
		{
		int frame;


		if (StateCount < FPS / 4)
			frame = 0;
		else
			{
			frame = 1;
			
			StateDone = TRUE;
			}


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_HANDOFF + FRAME_AMT_HANDOFF + (frame * 2);
		//else
			Frame = FRAME_HANDOFF + ((FaceDir - 1) * NUM_SPRTILES_X) + frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Snap(int reason)
	{

	//////////////////////////////////////
	//									//
	// Used to perform the state : SNAP //
	//									//
	//////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		// Face the receiver 

		/*if (Side[SideNum].TargetRec->Wld.X > Wld.X)			
			Face(1.0f, 0.0f);
		else if (Side[SideNum].TargetRec->Wld.X < Wld.X)	
			Face(-1.0f, 0.0f);
		else															
			Face(1.0f, 0.0f);


		// Set the throw attributes 

		ThrowAcc = 90.0f;
		ThrowPow = 50.0f;
		ThrowCtr = 0.0f;*/


		// Stop movement

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		int frame = 2;


		// If the button is let go, finish throw

		/*if (StateCount < (FPS * .5))
			{
			if (VPad[PadNum].Button(0))
				ThrowCtr += 1.0f ;
			else
				StateCount = (FPS * .5);
			}


		if (StateCount < (FPS * .5))
			frame = 0;
		else if (StateCount < (FPS * .75))
			frame = 1;
		else if (StateCount < FPS)
			{
			frame = 2;
			
			if (HasBall)
				ThrowBallToReceiver();
			}
		else
			Stand();*/

		if (HasBall)
			{
			TossBallToPlayer(TrackTarget);
			Stand();
			DoNextAction();
			}

		/*if (StateCount >= FPS * .5)
			{
			Stand();
			DoNextAction();
			}


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_THROW + FRAME_AMT_THROW + (frame * 2);
		//else
			Frame = FRAME_THROW + ((FaceDir - 1) * NUM_SPRTILES_X) + (frame * 2);*/
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Throw(int reason)
	{

	///////////////////////////////////////
	//									 //
	// Used to perform the state : THROW //
	//									 //
	///////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		// Face the receiver 

		if (Side[SideNum].TargetRec->Wld.X > Wld.X)			
			Face(1.0f, 0.0f);
		else if (Side[SideNum].TargetRec->Wld.X < Wld.X)	
			Face(-1.0f, 0.0f);
		else															
			Face(1.0f, 0.0f);


		// Set the throw attributes 

		ThrowAcc = PlayerInfo->Attrib.ThrowAcc.Value();
		ThrowPow = PlayerInfo->Attrib.ThrowPow.Value();
		ThrowCtr = 0.0f;


		// Stop movement

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		int frame = 2;


		// If the button is let go, finish throw

		if (StateCount < (FPS * .5))
			{
			if (VPad[PadNum].Button(0))
				ThrowCtr += 1.0f ;
			else
				StateCount = (FPS * .5);
			}


		if (StateCount < (FPS * .5))
			frame = 0;
		else if (StateCount < (FPS * .75))
			frame = 1;
		else if (StateCount < FPS)
			{
			frame = 2;
			
			if (HasBall)
				ThrowBallToReceiver();
			}
		else
			Stand();



		//ThrowPow = 100;
		//ThrowAcc += 1;
		//ThrowCtr += 1;


		//if (ThrowPow > 100)	
		///	ThrowPow = 100;


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_THROW + FRAME_AMT_THROW + (frame * 2);
		//else
			Frame = FRAME_THROW + ((FaceDir - 1) * NUM_SPRTILES_X) + (frame * 2);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Kick(int reason)
	{

	//////////////////////////////////////
	//									//
	// Used to perform the state : KICK //
	//									//
	//////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		StateTime = -1;
		}


	ON_STATE_UPDATE(reason)
		{
		int frame;


		if (StateCount < FPS / 4)
			frame = 0;
		else
			{
			frame = 1;
			
			StateDone = TRUE;
			}


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_HANDOFF + FRAME_AMT_HANDOFF + (frame * 2);
		//else
			Frame = FRAME_HANDOFF + ((FaceDir - 1) * NUM_SPRTILES_X) + frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_AimKick(int reason)
	{

	/////////////////////////////////////////
	//									   //
	// Used to perform the state : AIMKICK //
	//									   //
	/////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		SendMessage(0, MSG_START_AIM, 0, (DWORD)1.5f, 0);
		StateTime = -1;
		InputEnabled = TRUE;
		}


	ON_STATE_UPDATE(reason)
		{
		}


	ON_STATE_EXIT(reason)
		{
		InputEnabled = FALSE;
		}


	ON_STATE_INPUT(reason)
		{
		if (VPad[PadNum].ButtonClicked(0))
			StateDone = TRUE;
		}
	}





void PLAYER_OBJ::DoState_Punt(int reason)	
	{

	//////////////////////////////////////
	//									//
	// Used to perform the state : PUNT //
	//									//
	//////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		Animate(FALSE);
		Stop();
		StateTime = -1;
		}


	ON_STATE_UPDATE(reason)
		{
		int frame;


		if (StateCount < FPS / 4)
			frame = 0;
		else
			{
			frame = 1;
			
			StateDone = TRUE;
			}


		//if (FaceDir == DIR_UP || FaceDir == DIR_UPLEFT || 
		//	FaceDir == DIR_LEFT || FaceDir == DIR_DOWNLEFT || FaceDir == DIR_DOWN)
		//	Frame = FRAME_HANDOFF + FRAME_AMT_HANDOFF + (frame * 2);
		//else
			Frame = FRAME_HANDOFF + ((FaceDir - 1) * NUM_SPRTILES_X) + frame;
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}






void PLAYER_OBJ::DoState_Placehold(int reason)
	{

	///////////////////////////////////////////
	//									     //
	// Used to perform the state : PLACEHOLD //
	//									     //
	///////////////////////////////////////////



	ON_STATE_ENTER(reason)
		{
		//SetAnimation(CelebrateAnim);
		//SetAnimationSpeed(3);
		//SetAnimationPos(0);
		//Animate(TRUE);

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		int frame = 2;


		// If the button is let go, finish throw

		if (StateCount < (FPS * .5))
			{
			if (VPad[PadNum].Button(0))
				ThrowCtr += 1.0f ;
			else
				StateCount = (FPS * .5);
			}


		if (StateCount < (FPS * .5))
			frame = 0;
		else if (StateCount < (FPS * .75))
			frame = 1;
		else if (StateCount < FPS)
			{
			frame = 2;
			
			StateDone = TRUE;
			}
		else
			Stand();

		Frame = FRAME_THROW + ((FaceDir - 1) * NUM_SPRTILES_X) + (frame * 2);
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Celebrate(int reason)
	{

	///////////////////////////////////////////
	//									     //
	// Used to perform the state : CELEBRATE //
	//									     //
	///////////////////////////////////////////


	ON_STATE_ENTER(reason)
		{
		SetAnimation(CelebrateAnim);
		SetAnimationSpeed(3);
		SetAnimationPos(0);
		Animate(TRUE);

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}





void PLAYER_OBJ::DoState_Complain(int reason)
	{

	//////////////////////////////////////////
	//									    //
	// Used to perform the state : COMPLAIN //
	//									    //
	//////////////////////////////////////////


	ON_STATE_ENTER(reason)
		{
		SetAnimation(CelebrateAnim);
		SetAnimationSpeed(3);
		SetAnimationPos(0);
		Animate(TRUE);

		Stop();
		}


	ON_STATE_UPDATE(reason)
		{
		}


	ON_STATE_EXIT(reason)
		{
		}


	ON_STATE_INPUT(reason)
		{
		}
	}


