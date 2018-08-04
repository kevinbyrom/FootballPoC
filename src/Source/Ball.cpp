#include <windows.h>
#include "Ball.h"
#include "Football.h"
#include "MathUtils.h"


int SpiralAnim[]		= { 0,1,2,3,4,5,6,7,ANIM_LOOP,0 };



BALL::BALL()
	{
	
	/////////////////////////////
	//						   //
	// Base Player Constructor //
	//						   //
	/////////////////////////////


	Width		= 22;
	Height		= 22;
	SprOffset.X = -11;
	SprOffset.Y = -22;

	SetNumBounds(1);
	SetBounds(0, 5.0f, 0.0f, 0.0f, -2.0f);

	Acc.X = 1;
	Acc.Y = 1;
	Acc.Z = 1;

	FaceDir = DIR_RIGHT;

	Loose		= FALSE;
	Thrown		= FALSE;
	HitGround	= FALSE;

	Holder		= NULL;
	}






void BALL::Update()
	{

	//////////////////////////////
	//							//
	// The main update function //
	//							//
	//////////////////////////////


	UpdateDeltas();
	UpdateAnimation();
	UpdateMove();

	TimeToInRangeSpot--;
	TimeToCatchSpot--;
	TimeToLandingSpot--;

	
	// Check if the ball hits the ground 

	if (Wld.Z <= 0 && Loose)
		{

		// If ball was kicked and has not bounced and passed within the goal post then ..
			// Set the kickgood flag


		// Bounce the ball 

		if (Delta.Z < 0)
			{
			Bounce();

			// If it was thrown, send a BALLHITGROUND message to the stage 

			if ((Thrown || Kicked) && !HitGround)	
				{
				HitGround = TRUE;
				SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_HIT_GROUND);
				}
			}


		Wld.Z = 0;
		}


	// Set the frame //

	switch(FaceDir)
		{
		case DIR_UP:
			Frame = Anim.Frame; 
			break;

		case DIR_DOWN:
			Frame = 8 + Anim.Frame; 
			break;

		case DIR_LEFT:
			Frame = 16 + Anim.Frame; 
			break;

		case DIR_RIGHT:
			Frame = 24 + Anim.Frame; 
			break;

		case DIR_UPLEFT:
			Frame = 32 + Anim.Frame; 
			break;

		case DIR_UPRIGHT:
			Frame = 40 + Anim.Frame; 
			break;

		case DIR_DOWNLEFT:
			Frame = 48 + Anim.Frame; 
			break;

		case DIR_DOWNRIGHT:
			Frame = 56 + Anim.Frame; 
			break;
		} 
	}




void BALL::Throw(float xdir, float ydir, float zdir)
	{

	////////////////////////////
	//						  //
	// Used to throw the ball //
	//						  //
	////////////////////////////


	CalculateSpots(xdir, ydir, zdir);

	Origin.X	= Wld.X;
	Origin.Y	= Wld.Y;
	Origin.Z	= Wld.Z;

	Vel.X		= 1;
	Vel.Y		= 1;
	Vel.Z		= 1;

	DestVel.X	= 1;
	DestVel.Y	= 1;
	DestVel.Z	= 1;

	MoveDir.X	= xdir;
	MoveDir.Y	= ydir;
	MoveDir.Z	= zdir;

	Delta.Z = (MoveDir.Z * Vel.Z) + GRAVITY;

	Visible = TRUE;

	SetAnimation(SpiralAnim);
	SetAnimationSpeed(3);
	SetAnimationPos(0);
	Animate(TRUE);

	FaceMoveDirection();

	Loose		= TRUE;
	Thrown		= TRUE;
	Tossed		= FALSE;
	Punted		= FALSE;
	HitGround	= FALSE;
	
	Holder		= NULL;

	UnlockObject();
	}





void BALL::Toss(float xdir, float ydir, float zdir)
	{

	///////////////////////////
	//						 //
	// Used to toss the ball //
	//						 //
	///////////////////////////


	CalculateSpots(xdir, ydir, zdir);

	Origin.X	= Wld.X;
	Origin.Y	= Wld.Y;
	Origin.Z	= Wld.Z;

	Vel.X		= 1;
	Vel.Y		= 1;
	Vel.Z		= 1;

	DestVel.X	= 1;
	DestVel.Y	= 1;
	DestVel.Z	= 1;

	MoveDir.X	= xdir;
	MoveDir.Y	= ydir;
	MoveDir.Z	= zdir;

	Delta.Z = (MoveDir.Z * Vel.Z) + GRAVITY;


	Visible = TRUE;

	SetAnimation(SpiralAnim);
	SetAnimationSpeed(3);
	SetAnimationPos(0);
	Animate(TRUE);

	FaceMoveDirection();

	Loose		= TRUE;
	Thrown		= FALSE;
	Tossed		= TRUE;
	Punted		= FALSE;
	Kicked		= FALSE;
	HitGround	= FALSE;
	
	Holder		= NULL;

	UnlockObject();
	}




void BALL::Punt(float xdir, float ydir, float zdir)
	{

	///////////////////////////
	//						 //
	// Used to punt the ball //
	//						 //
	///////////////////////////


	CalculateSpots(xdir, ydir, zdir);

	Origin.X	= Wld.X;
	Origin.Y	= Wld.Y;
	Origin.Z	= Wld.Z;

	Vel.X		= 1;
	Vel.Y		= 1;
	Vel.Z		= 1;

	DestVel.X	= 1;
	DestVel.Y	= 1;
	DestVel.Z	= 1;

	MoveDir.X	= xdir;
	MoveDir.Y	= ydir;
	MoveDir.Z	= zdir;

	Delta.Z = (MoveDir.Z * Vel.Z) + GRAVITY;

	Visible = TRUE;

	SetAnimation(SpiralAnim);
	SetAnimationSpeed(3);
	SetAnimationPos(0);
	Animate(TRUE);

	FaceMoveDirection();

	Loose		= TRUE;
	Thrown		= FALSE;
	Tossed		= FALSE;
	Punted		= TRUE;
	Kicked		= FALSE;
	HitGround	= FALSE;
	
	Holder		= NULL;

	UnlockObject();
	}





void BALL::Kick(float xdir, float ydir, float zdir)
	{

	///////////////////////////
	//						 //
	// Used to kick the ball //
	//						 //
	///////////////////////////


	CalculateSpots(xdir, ydir, zdir);

	Origin.X	= Wld.X;
	Origin.Y	= Wld.Y;
	Origin.Z	= Wld.Z;

	Vel.X		= 1;
	Vel.Y		= 1;
	Vel.Z		= 1;

	DestVel.X	= 1;
	DestVel.Y	= 1;
	DestVel.Z	= 1;

	MoveDir.X	= xdir;
	MoveDir.Y	= ydir;
	MoveDir.Z	= zdir;

	Delta.Z = (MoveDir.Z * Vel.Z) + GRAVITY;

	Visible = TRUE;

	SetAnimation(SpiralAnim);
	SetAnimationSpeed(3);
	SetAnimationPos(0);
	Animate(TRUE);

	FaceMoveDirection();

	Loose		= TRUE;
	Thrown		= FALSE;
	Tossed		= FALSE;
	Punted		= FALSE;
	Kicked		= TRUE;
	HitGround	= FALSE;
	
	Holder		= NULL;

	UnlockObject();
	}





void BALL::Bounce()
	{
	Vel.X *= .5;
	Vel.Y *= .5;

	DestVel.X = Vel.X;
	DestVel.Y = Vel.Y;
	DestVel.Z = Vel.Z;

	Delta.Z = -(Delta.Z * .5);

	if (Vel.X <= 0.0f)
		Stop();
	}




void BALL::Caught(PLAYER_OBJ * player)
	{

	//////////////////////////////////////////
	//										//
	// Used to handle the ball begin caught //
	//										//
	//////////////////////////////////////////


	Stop();
	
	Animate(FALSE);

	Visible = FALSE;
	Loose	= FALSE;
	
	Holder = player;

	LockObject(player, TRUE, TRUE, TRUE, 0, 0, player->Height / 2 - 5);
	}





void BALL::Stop()
	{

	///////////////////////////
	//						 //
	// Used to stop movement //
	//						 //
	///////////////////////////


	Vel.X = 0.0f;
	Vel.Y = 0.0f;
	Acc.X = 0.0f;
	Acc.Y = 0.0f;

	Animate(FALSE);
	}




void BALL::SlowStop()
	{

	//////////////////////////////////
	//								//
	// Used to slowly stop the ball //
	//								//
	//////////////////////////////////


	if (Vel.X > 0.0f || Acc.X > 0.0f)	Acc.X = -0.1f;
	if (Vel.Y > 0.0f || Acc.Y > 0.0f)	Acc.Y = -0.1f;
	}





void BALL::UpdateDeltas()
	{

	///////////////////////////////////////////////
	//											 //
	// Used to calculate the new delta values by //
	// the move direction						 //
	//											 //
	///////////////////////////////////////////////


	Delta.X = MoveDir.X * Vel.X;
	Delta.Y = MoveDir.Y * Vel.Y;
	Delta.Z -= GRAVITY;
	}




int BALL::GetDirection(float xdir, float ydir)
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Used to get a direction constant based on the xdir and ydir //
	//															   //				
	/////////////////////////////////////////////////////////////////


	int dir		= 0;
	BOOL left	= FALSE;
	BOOL right	= FALSE;
	BOOL up		= FALSE;
	BOOL down	= FALSE;


	// Set the left / right flags

	if (xdir <= -0.5f)
		left = TRUE;
	else if (xdir >= 0.5f)
		right = TRUE;


	// Set the up / down flags

	if (ydir <= -0.5f)
		up = TRUE;
	else if (ydir >= 0.5f)
		down = TRUE;


	// Set the dir variable based on the set flags

	if		(up && left)		dir = DIR_UPLEFT;
	else if (up && right)		dir = DIR_UPRIGHT;
	else if (up)				dir = DIR_UP;
	else if (down && left)		dir = DIR_DOWNLEFT;
	else if (down && right)		dir = DIR_DOWNRIGHT;
	else if (down)				dir = DIR_DOWN;
	else if (left)				dir = DIR_LEFT;
	else if (right)				dir = DIR_RIGHT;


	return dir;
	}




void BALL::FaceMoveDirection()
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to face the ball towards the move direction //
	//													//
	//////////////////////////////////////////////////////


	FaceDir = GetDirection(MoveDir.X, MoveDir.Y);
	}




void BALL::CalculateSpots(float xdir, float ydir, float zdir)
	{
	int StepsToZOrigin;
	int HalfwayStep1, HalfwayStep2;
	float MaxHeight;
	float Steps1, Steps2, Steps;
	float A, B, C;


	// Calculate the amount of steps until the z position is at origin
	
	StepsToZOrigin = int(((zdir / GRAVITY) * 2) + 1);
	

	// Calculate the halfway steps

	HalfwayStep1 = (int)(StepsToZOrigin / 2);
	HalfwayStep2 = HalfwayStep1 + 1;


	// Calculate the maximum height of the ball

	MaxHeight = ((HalfwayStep1 * zdir) + (((float)HalfwayStep1 / 2) * (HalfwayStep1 - 1) * -GRAVITY));


	// Calculate the in range spot

	A = 0.5f * -GRAVITY;
	B = -GRAVITY - (0.5f * -GRAVITY);
	C = MaxHeight - 40;

	
	if (QuadraticFormula(A, B, C, &Steps1, &Steps2))
		{
		Steps = Steps1 >= 0 ? Round(Steps1) + HalfwayStep2 : Round(Steps2) + HalfwayStep2;

		InRangeSpot.Wld.X = Wld.X + (xdir * Steps);
		InRangeSpot.Wld.Y = Wld.Y + (ydir * Steps);
		InRangeSpot.Wld.Z = 0;

		TimeToInRangeSpot = Steps;
		}



	// Calculate the catch spot

	CatchSpot.Wld.X = Wld.X + (xdir * StepsToZOrigin);
	CatchSpot.Wld.Y = Wld.Y + (ydir * StepsToZOrigin);
	CatchSpot.Wld.Z = 0;

	TimeToCatchSpot = StepsToZOrigin;


	// Calculate the landing spot 

	A = 0.5f * -GRAVITY;
	B = -GRAVITY - (0.5f * -GRAVITY);
	C = MaxHeight + Wld.Z;


	if (QuadraticFormula(A, B, C, &Steps1, &Steps2))
		{
		Steps = Steps1 >= 0 ? Round(Steps1) + HalfwayStep2 : Round(Steps2) + HalfwayStep2;

		LandingSpot.Wld.X = Wld.X + (xdir * Steps);
		LandingSpot.Wld.Y = Wld.Y + (ydir * Steps);
		LandingSpot.Wld.Z = 0;

		TimeToLandingSpot = Steps;
		}
	}