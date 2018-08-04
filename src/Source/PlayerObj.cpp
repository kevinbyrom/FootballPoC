#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "PlayerObj.h"
#include "Football.h"
#include "MathUtils.h"










PLAYER_OBJ::PLAYER_OBJ()
	{
	
	/////////////////////////////
	//						   //
	// Base Player Constructor //
	//						   //
	/////////////////////////////


	SetSide(); 

	Friction	= .01;

	Width		= 44;
	Height		= 44;
	SprOffset.X	= -22;
	SprOffset.Y = -44;

	SetNumBounds(3);
	SetBounds(0, PLAYER_WIDTH / 3, 0.0f, 0.0f, 0.0f);
	SetBounds(1, PLAYER_WIDTH / 3, 0.0f, 0.0f, -(PLAYER_HEIGHT / 2));
	SetBounds(2, PLAYER_WIDTH / 3, 0.0f, 0.0f, -(PLAYER_HEIGHT));

	InputEnabled = FALSE;

	PadNum = 0;

	FaceDir = DIR_RIGHT;

	ThrowType	= THROW_TYPE_NORMAL;
	}




void PLAYER_OBJ::CheckCoordinates()
	{

	////////////////////////////////////////////////////////
	//													  //
	// Used to perform checks on the player's coordinates //
	//													  //
	////////////////////////////////////////////////////////


	// Increment the track count 

	TrackCount++;


	// Prevent the player Z value from going lower than zero

	if (Wld.Z < 0) Wld.Z = 0;



	// Prevent the player from going out of the endzone bounds

	if (Wld.X >= ENDZONE_SIDELINE_POS) 
		{
		Wld.X = ENDZONE_SIDELINE_POS - 1;
		Delta.X = 0;
		}
	
	if (Wld.X <= -ENDZONE_SIDELINE_POS) 
		{
		Wld.X = -ENDZONE_SIDELINE_POS + 1;
		Delta.X = 0;
		}

	


	if (!DownInfo.Done)
		{
		if (HasBall)
			{

			// If player passes scrimmage and has the ball, send a message to all players

			if (!DownInfo.PassedScrimmage)
				{
				if ((Side[SideNum].Direction == DIR_LEFT && Wld.X <= DownInfo.ScrimmagePos) ||
					(Side[SideNum].Direction == DIR_RIGHT && Wld.X >= DownInfo.ScrimmagePos))
					{
					SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_PASSED_SCRIMMAGE, 0, (DWORD)this);
					}
				}


			// Check for touchdown

			if ((GameInfo.Direction[TeamNum] == DIR_LEFT && (Wld.X - (Width / 2) <= -ENDZONE_POS)) ||
				(GameInfo.Direction[TeamNum] == DIR_RIGHT && (Wld.X + (Width / 2) >= ENDZONE_POS)))
					SendMessageToGroup(MESSAGE_DEST_ALL, MSG_TOUCHDOWN, 0, (DWORD)this);
			else if (Wld.Y <= -SIDELINE_POS || Wld.Y >= SIDELINE_POS)
				SendMessage(0, MSG_BALL_HOLDER_OUT_OF_BOUNDS, 0, (DWORD)this);
			}
		}
	}



void PLAYER_OBJ::PrepareForPlay()
	{
	
	////////////////////////////////////////////////////
	//												  //
	// Used to prepare the player object for the play //
	//												  //
	////////////////////////////////////////////////////


	// Clear the actions 

	Actions.Clear();


	// Reset the flags 

	HasBall			= FALSE;
	InputEnabled	= FALSE;
	TrackTarget		= NULL;
	GrappleTarget	= NULL;

	
	// Set grapplers to zero

	NumGrapplers = 0;


	// Stand

	Stop();
	Stand();
	}





void PLAYER_OBJ::SetPlayerInfo(PLAYER_INFO * pi)
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to attach a PlayerInfo struct to this object //
	//													 //
	///////////////////////////////////////////////////////


	PlayerInfo = pi;

	if (PlayerInfo == NULL)
		return;


	// Set the max speed 

	//DestVel.X = ((PlayerInfo->Attrib.Speed.Value() * 9) / 100) * (9.8 / FPS);
	//DestVel.Y = ((PlayerInfo->Attrib.Speed.Value() * 9) / 100) * (9.8 / FPS);


	// Set the max acceleration

	Acc.X = DestVel.X / (FPS * ((100 - PlayerInfo->Attrib.Accel.Value()) / 20));
	Acc.Y = DestVel.Y / (FPS * ((100 - PlayerInfo->Attrib.Accel.Value()) / 20));
	}




//////////////////
// CONTROL CODE //
//////////////////




void PLAYER_OBJ::ProcessControl()
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
			if (InputEnabled && Side[SideNum].Control == this)
				//ProcessState(INPUT_STATE);
			break;


		case CONTROL_ACTIONS:
			DoControl_Action();
			break;
		}
	}





void PLAYER_OBJ::DoControl_Action()
	{

	//////////////////////////////////////////////////
	//												//
	// Used to process control from an action array //
	//												//
	//////////////////////////////////////////////////


	// Do nothing if there are no actions

	if (Actions.GetNumActions() == 0)
		return;


	// Do the current action 

	if (!Actions.IsDelayed())
		{
		DoAction(0);


		// If this is a time based action, and the time is met 
		// then do the next action 

		if (Actions.GetTime() != -1)
			{
			// Decrement the action time 

			Actions.DecrementTime(1);

			if (Actions.GetTime() <= 0)
				DoNextAction();
			}
		}
	else
		Actions.DecrementDelayTime(1);
	}




/**********************************************************************************

STATE CODE

***********************************************************************************/








void PLAYER_OBJ::Stand()
	{

	///////////////////////////////////
	//								 //
	// Used to make the player stand //
	//								 //
	///////////////////////////////////


	// Set the state to Stand
	
	SetState(STATE_STAND);
	}





void PLAYER_OBJ::Stance()
	{

	//////////////////////////////////////////////
	//								            //
	// Used to make the player go into a stance //
	//								            //
	//////////////////////////////////////////////


	// Set the state to stance

	SetState(STATE_STANCE);
	}





void PLAYER_OBJ::Run(float xdir, float ydir)
	{

	/////////////////////////////////
	//							   //
	// Used to make the player run //
	//							   //
	/////////////////////////////////


	// Only allow running if the player CAN run 

	if (State != STATE_STANCE && 
		State != STATE_STAND && 
		State != STATE_RUN && 
		State != STATE_SLOWDOWN)
		return;


	// Set the state to run //

	SetState(STATE_RUN);



	// Set the new move direction //

	MoveDir.X = xdir;
	MoveDir.Y = ydir;


	// Set the face direction //

	FaceMoveDirection();
	}





void PLAYER_OBJ::Leap(float xdir, float ydir)
	{

	//////////////////////////////////
	//								//
	// Used to make the player leap //
	//								//
	//////////////////////////////////


	// Only allow leaping if the player CAN leap 

	if (State != STATE_STAND && State != STATE_RUN)
		return;


	// Set the state to leap

	SetState(STATE_LEAP);


	// Set the face direction 
	
	Face(xdir, ydir);	
	}





void PLAYER_OBJ::Dive(float xdir, float ydir)
	{

	//////////////////////////////////
	//								//
	// Used to make the player dive //
	//								//
	//////////////////////////////////


	// Only allow player to dive if he CAN dive

	if (State != STATE_STAND && State != STATE_RUN)
		return;


	// Set the state to dive

	SetState(STATE_DIVE);


	// Set the move direction 

	MoveDir.X	= xdir;
	MoveDir.Y	= ydir;


	// Set the face direction 
	
	FaceMoveDirection();
	}





void PLAYER_OBJ::Grapple(float xdir, float ydir)
	{

	/////////////////////////////////////
	//								   //
	// Used to make the player grapple //
	//								   //
	/////////////////////////////////////


	// Set the state to grapple

	SetState(STATE_GRAPPLE);


	// Face the grappler

	Face(xdir, ydir);
	}





void PLAYER_OBJ::BreakGrapple(float xdir, float ydir)
	{

	/////////////////////////////////////
	//								   //
	// Used to make the player grapple //
	//								   //
	/////////////////////////////////////


	// Set the state to break grapple

	SetState(STATE_BREAKGRAPPLE);


	// Face the grappler

	Face(xdir, ydir);
	}





void PLAYER_OBJ::Tumble(float xdir, float ydir)
	{

	////////////////////////////////////
	//							      //
	// Used to make the player tumble //
	//							      //
	////////////////////////////////////


	SetState(STATE_TUMBLE);

	Face(xdir, ydir);

	MoveDir.X	= xdir;
	MoveDir.Y	= ydir;
	}





void PLAYER_OBJ::LayDown()
	{

	//////////////////////////////////////
	//							        //
	// Used to make the player lay down //
	//							        //
	//////////////////////////////////////


	// Set the state to lay down

	SetState(STATE_LAYDOWN);
	}





void PLAYER_OBJ::Sit()
	{

	/////////////////////////////////
	//							   //
	// Used to make the player sit //
	//							   //
	/////////////////////////////////


	// Only allow the player to sit if he CAN sit

	if (State != STATE_LAYDOWN && State != STATE_DIVE)
		return;


	// Set the state to sit

	SetState(STATE_SIT);
	}





void PLAYER_OBJ::GetUp()
	{

	////////////////////////////////////
	//							      //
	// Used to make the player get up //
	//							      //
	////////////////////////////////////


	// Only allow the player to get up if he CAN get up

	if (State != STATE_SIT)
		return;


	// Set the state to get up

	SetState(STATE_GETUP);
	}





void PLAYER_OBJ::HandOff()
	{

	//////////////////////////////////////////
	//										//
	// Used to make the player do a handoff //
	//										//
	//////////////////////////////////////////


	// Set the state to handoff

	SetState(STATE_HANDOFF);
	}





void PLAYER_OBJ::FakeHandOff()
	{

	///////////////////////////////////////////////
	//											 //
	// Used to make the player do a fake handoff //
	//											 //
	///////////////////////////////////////////////


	// Set the state to fake handoff

	SetState(STATE_FAKEHANDOFF);
	}





void PLAYER_OBJ::Toss()
	{

	///////////////////////////////////////////
	//										 //
	// Used to make the player toss the ball //
	//										 //
	///////////////////////////////////////////


 	SetState(STATE_TOSS);
	}




void PLAYER_OBJ::Snap()
	{

	///////////////////////////////////////////
	//										 //
	// Used to make the player snap the ball //
	//										 //
	///////////////////////////////////////////


	// Set the state to snap

	SetState(STATE_SNAP);
	}




void PLAYER_OBJ::Throw()
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to make the player throw to another player //
	//												   //
	/////////////////////////////////////////////////////


	// Set the state to throw

	SetState(STATE_THROW);
	}





void PLAYER_OBJ::Punt()
	{

	///////////////////////////////////////////
	//										 //
	// Used to make the player punt the ball //
	//										 //
	///////////////////////////////////////////


	// Set the state to punt

	SetState(STATE_PUNT);
	}





void PLAYER_OBJ::Kick()
	{

	///////////////////////////////////////////
	//										 //
	// Used to make the player kick the ball //
	//										 //
	///////////////////////////////////////////


	// Set the state to kick

	SetState(STATE_KICK);
	}





void PLAYER_OBJ::AimKick()
	{

	//////////////////////////////////////////
	//										//
	// Used to make the player aim the kick //
	//										//
	//////////////////////////////////////////


	// Set the state to aim kick

	SetState(STATE_AIMKICK);
	}




void PLAYER_OBJ::GetBall()
	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to set the possession of the ball to this player //
	//														 //
	///////////////////////////////////////////////////////////


	Ball.Caught(this);
	HasBall = TRUE;
	}




void PLAYER_OBJ::HandOverBall(PLAYER_OBJ * target)
	{

	//////////////////////////////////////////////
	//											//
	// Used to hand over the ball to the target //
	//											//
	//////////////////////////////////////////////


	HasBall = FALSE;

	SendMessage(target->GetMsgTargetID(), MSG_HANDOFF_GIVEN);
	}




void PLAYER_OBJ::TossBallToPlayer(PLAYER_OBJ * player)
	{

	/////////////////////////////////////
	//								   //
	// Used to toss a ball to a player //
	//								   //
	/////////////////////////////////////


	float distpx,		// X Distance to player
		  distpy,		// Y Distance to player
		  distp;		// Total Distance to player

	float destx,		// X Destination
		  desty,		// Y Destination
		  distx,		// X Distance to destination
		  disty;		// Y Distance to destination

	float errorx,		// Error X (Based on accuracy)
		  errory;		// Error Y (Based on accuracy)

	float distance;		// Distance to destination
	int   lead;			// Pass lead
	int	  time;			// Time in frames
	
	int	  acc;			// Accuracy

	float xdir,			// Ball X direction
		  ydir,			// Ball Y direction
		  zdir;			// Ball Z direction



	// Calculate the distance between the players 

	distpx = Wld.X - player->Wld.X;
	distpy = Wld.Y - player->Wld.Y;
	distp = sqrt((distpx * distpx) + (distpy * distpy));


	// Calculate the time in frames for the ball to hit the destination 
	// Based on the throw type of the passer

	//lead = THROW_SPEED_NORMAL;
	//lead = FPS - (ThrowCtr * 2);

	//if (lead > THROW_SPEED_LOB)		lead = THROW_SPEED_LOB;
	//if (lead < THROW_SPEED_BULLET)	lead = THROW_SPEED_BULLET;
//	lead = THROW_SPEED_BULLET;
	lead = 0;

	time = lead + ((distp - 30) / 5);


	// Calculate the destination & distances to destination

	//acc = 100 - int(ThrowAcc);

	//if (acc == 0) acc = 1;
	
	acc = 1;

	errorx = 0;//((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);
	errory = 0;//((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);

	destx = player->Wld.X + (player->Delta.X * time) + errorx;
	desty = player->Wld.Y + (player->Delta.Y * time) + errory;

	distx = destx - Wld.X;
	disty = desty - Wld.Y;


	// Calculate the distance to the destination

	distance	= sqrt((distx * distx) + (disty * disty));


	// Calculate the ball vector 

	if (time == 0)
		time = 2;

	xdir = distx / time;
	ydir = disty / time;
	zdir = ((distance * GRAVITY) / (2 * (distance / time))) + (GRAVITY / 2);

	

	//if (ThrowPow == 0) ThrowPow = 1.0f;

	//xdir *= ThrowPow / 100;
	//ydir *= ThrowPow / 100;
	//zdir *= ThrowPow / 100;

	xdir *= .5;
	ydir *= .5;
	zdir *= .5;


	// Throw the ball

	Ball.Toss(xdir, ydir, zdir);


	// Set the has ball flag to false

	HasBall = FALSE;
	}




void PLAYER_OBJ::ThrowBallToReceiver()
	{
	float distpx,		// X Distance to player
		  distpy,		// Y Distance to player
		  distp;		// Total Distance to player

	float destx,		// X Destination
		  desty,		// Y Destination
		  distx,		// X Distance to destination
		  disty;		// Y Distance to destination

	float errorx,		// Error X (Based on accuracy)
		  errory;		// Error Y (Based on accuracy)

	float distance;		// Distance to destination
	int   lead;			// Pass lead
	int	  time;			// Time in frames
	
	int	  acc;			// Accuracy

	float xdir,			// Ball X direction
		  ydir,			// Ball Y direction
		  zdir;			// Ball Z direction

	PLAYER_OBJ * receiver = Side[SideNum].TargetRec;


	// Calculate the distance between the players 

	distpx = Wld.X - receiver->Wld.X;
	distpy = Wld.Y - receiver->Wld.Y;
	distp = sqrt((distpx * distpx) + (distpy * distpy));


	// Calculate the time in frames for the ball to hit the destination 
	// Based on the throw type of the passer

	switch(ThrowType)
		{
		case THROW_TYPE_LOB:
			lead = THROW_SPEED_LOB;
			break;

		case THROW_TYPE_NORMAL:
			lead = THROW_SPEED_NORMAL;
			break;

		case THROW_TYPE_BULLET:
			lead = THROW_SPEED_BULLET;
			break;
		}

	lead = FPS - (ThrowCtr * 2);

	if (lead > THROW_SPEED_LOB)		lead = THROW_SPEED_LOB;
	if (lead < THROW_SPEED_BULLET)	lead = THROW_SPEED_BULLET;
//	lead = THROW_SPEED_BULLET;

	time = lead + ((distp - 30) / 5);


	// Calculate the destination & distances to destination

	acc = 100 - int(ThrowAcc);

	if (acc == 0) acc = 1;
	
	errorx = ((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);
	errory = ((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);

	destx = receiver->Wld.X + (receiver->Delta.X * time) + errorx;
	desty = receiver->Wld.Y + (receiver->Delta.Y * time) + errory;

	distx = destx - Wld.X;
	disty = desty - Wld.Y;


	// Calculate the distance to the destination

	distance	= sqrt((distx * distx) + (disty * disty));


	// Calculate the ball vector 

	xdir = distx / time;
	ydir = disty / time;
	zdir = ((distance * GRAVITY) / (2 * (distance / time))) + (GRAVITY / 2);

	
	if (ThrowPow == 0) ThrowPow = 1.0f;

	xdir *= ThrowPow / 100;
	ydir *= ThrowPow / 100;
	zdir *= ThrowPow / 100;


	// Throw the ball

	Ball.Throw(xdir, ydir, zdir);


	// Set the ball thrown flag 

	DownInfo.BallPassed = TRUE;


	// Set the has ball flag to false

	HasBall = FALSE;


	// Inform the players the ball has been thrown 
	
	SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_THROWN, 0, time);
	}





void PLAYER_OBJ::PuntBall()
	{

	///////////////////////////
	//						 //
	// Used to punt the ball //
	//						 //
	///////////////////////////


	float spotx,
		  spoty;

	float distsx,		// X Distance to spot
		  distsy,		// Y Distance to spot
		  dists;		// Total Distance to spot

	float destx,		// X Destination
		  desty,		// Y Destination
		  distx,		// X Distance to destination
		  disty;		// Y Distance to destination

	float errorx,		// Error X (Based on accuracy)
		  errory;		// Error Y (Based on accuracy)

	float distance;		// Distance to destination
	int   lead;			// Pass lead
	int	  time;			// Time in frames
	
	int	  acc;			// Accuracy

	float xdir,			// Ball X direction
		  ydir,			// Ball Y direction
		  zdir;			// Ball Z direction



	// Calculate the landing spot

	if (Side[SideNum].Direction == DIR_LEFT)
		spotx = Wld.X - (YARD_DISTANCE * 40);
	else
		spotx = Wld.X + (YARD_DISTANCE * 40);


	spoty = 0.0f;


	// Calculate the distance between the spot 

	distsx = Wld.X - spotx;
	distsy = Wld.Y - spoty;
	dists = sqrt((distsx * distsx) + (distsy * distsy));


	// Calculate the time in frames for the ball to hit the destination 
	// Based on the throw type of the passer

	//lead = THROW_SPEED_NORMAL;
	//lead = FPS - (ThrowCtr * 2);

	//if (lead > THROW_SPEED_LOB)		lead = THROW_SPEED_LOB;
	//if (lead < THROW_SPEED_BULLET)	lead = THROW_SPEED_BULLET;
//	lead = THROW_SPEED_BULLET;
	lead = 60;

	time = lead + ((dists - 30) / 5);


	// Calculate the destination & distances to destination

	//acc = 100 - int(ThrowAcc);

	//if (acc == 0) acc = 1;
	
	acc = 1;

	errorx = 0;//((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);
	errory = 0;//((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);

	destx = spotx;//player->Wld.X + (player->Delta.X * time) + errorx;
	desty = spoty;//player->Wld.Y + (player->Delta.Y * time) + errory;

	distx = destx - Wld.X;
	disty = desty - Wld.Y;


	// Calculate the distance to the destination

	distance	= sqrt((distx * distx) + (disty * disty));


	// Calculate the ball vector 

	xdir = distx / time;
	ydir = disty / time;
	zdir = ((distance * GRAVITY) / (2 * (distance / time))) + (GRAVITY / 2);

	

	//if (ThrowPow == 0) ThrowPow = 1.0f;

	//xdir *= ThrowPow / 100;
	//ydir *= ThrowPow / 100;
	//zdir *= ThrowPow / 100;

	//xdir *= 1;
	//ydir *= 1;
	//zdir *= 1;


	// Punt the ball

	Ball.Punt(xdir, ydir, zdir);


	// Set the has ball flag to false

	HasBall = FALSE;
	}




void PLAYER_OBJ::KickBall()
	{

	///////////////////////////
	//						 //
	// Used to kick the ball //
	//						 //
	///////////////////////////


	float spotx,
		  spoty;

	float distsx,		// X Distance to spot
		  distsy,		// Y Distance to spot
		  dists;		// Total Distance to spot

	float destx,		// X Destination
		  desty,		// Y Destination
		  distx,		// X Distance to destination
		  disty;		// Y Distance to destination

	float errorx,		// Error X (Based on accuracy)
		  errory;		// Error Y (Based on accuracy)

	float distance;		// Distance to destination
	int	  time;			// Time in frames
	
	int	  acc;			// Accuracy

	float xdir,			// Ball X direction
		  ydir,			// Ball Y direction
		  zdir;			// Ball Z direction



	// Calculate the landing spot

	
	spotx = Wld.X + ((DownInfo.KickAim.Wld.X - Wld.X) * 10);
	spoty = Wld.Y + ((DownInfo.KickAim.Wld.Y - Wld.Y) * 10);

	float hyp = sqrt((spotx * spotx) + (spoty * spoty));
	float ratio = hyp == 0 ? 0 : (YARD_DISTANCE * 50) / hyp;
	
	spotx = spotx * ratio;
	spoty = spoty * ratio;


	// Calculate the distance between the spot 

	distsx = Wld.X - spotx;
	distsy = Wld.Y - spoty;
	dists = sqrt((distsx * distsx) + (distsy * distsy));

	time = ((dists - 30) / 5);


	// Calculate the destination & distances to destination

	//acc = 100 - int(ThrowAcc);

	//if (acc == 0) acc = 1;
	
	acc = 1;

	errorx = ((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);
	errory = ((rand() % 10) - 5) + ((rand() % int(acc * 2)) - acc);

	destx = spotx;// + errorx;player->Wld.X + (player->Delta.X * time) + errorx;
	desty = spoty;// + errory;player->Wld.Y + (player->Delta.Y * time) + errory;

	distx = destx - Wld.X;
	disty = desty - Wld.Y;


	// Calculate the distance to the destination

	distance	= sqrt((distx * distx) + (disty * disty));


	// Calculate the ball vector 

	xdir = distx / time;
	ydir = disty / time;
	zdir = ((distance * GRAVITY) / (2 * (distance / time))) + (GRAVITY / 2);


	// Kick the ball

	Ball.Kick(xdir, ydir, zdir);


	// Set the has ball flag to false

	HasBall = FALSE;
	}




void PLAYER_OBJ::Placehold()
	{

	///////////////////////////////////////
	//									 //
	// Used to make the player placehold //
	//									 //
	///////////////////////////////////////

	

	SetState(STATE_CELEBRATE);
	}




void PLAYER_OBJ::Celebrate()
	{

	///////////////////////////////////////
	//									 //
	// Used to make the player celebrate //
	//									 //
	///////////////////////////////////////

	

	SetState(STATE_CELEBRATE);
	}





void PLAYER_OBJ::Complain()
	{

	//////////////////////////////////////
	//									//
	// Used to make the player complain //
	//									//
	//////////////////////////////////////


	SetState(STATE_COMPLAIN);
	}





BOOL PLAYER_OBJ::TryToKnockDown(PLAYER_OBJ * target)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to make the player try to take down another player //
	//														   //
	/////////////////////////////////////////////////////////////


	Damage(GRAPPLE_DAMAGE * 2);


	// If roll is successful, send a knockdown msg to the target

	if (RollForKnockDown(target))
		{
		SendMessage(target->GetMsgTargetID(), MSG_KNOCKED_DOWN, 0, (DWORD)this);
		
		return TRUE;
		}


	return FALSE;
	}




BOOL PLAYER_OBJ::TryToTakeDown(PLAYER_OBJ * target)
	{

	//////////////////////////////////////////////
	//										    //
	// Used to make the player try to take down //
	//											//
	//////////////////////////////////////////////


	Damage(GRAPPLE_DAMAGE * 2);


	// If roll is successful, send a takedown msg to the target

	if (RollForTakeDown(target))
		{
		SendMessage(target->GetMsgTargetID(), MSG_TAKEN_DOWN, 0, (DWORD)this);
		
		return TRUE;
		}


	return FALSE;
	}





void PLAYER_OBJ::TryToBreakGrapple()
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to make the player try to break the grapple //
	//													//
	//////////////////////////////////////////////////////


	Damage(GRAPPLE_DAMAGE * 2);

	RollForGrappleBreak(Grapplers[0]);
	}





void PLAYER_OBJ::TryToCatch()
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to make the player try to catch the ball //
	//												 //
	///////////////////////////////////////////////////


	if (RollForCatch())
		{
		SendMessageToGroup(MESSAGE_DEST_ALL, MSG_BALL_CAUGHT, 0, (DWORD)this);
		
		Side[SideNum].ChangeUserControl(this);
		//InputEnabled	= TRUE;
		//ControlType		= CONTROL_USER;
		//Side[SideNum].Control = this;

		GetBall();
		}
	}




void PLAYER_OBJ::TryToCatchSnap()
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to make the player try to catch the snap //
	//												 //
	///////////////////////////////////////////////////


	if (RollForCatchSnap())
		{
		GetBall();
		}
	}








////////////
// EVENTS //
////////////





DWORD PLAYER_OBJ::OnMessage(int from, int msg, DWORD data1, DWORD data2)
	{

	///////////////////////////////////
	//								 //
	// Used to handle message events //
	//								 //
	///////////////////////////////////


	switch(msg)
		{
		case MSG_BALL_HIKED:
			OnBallHiked();
			return 1;

		case MSG_BALL_THROWN:
			OnBallThrown((int)data1);
			return 1;

		case MSG_BALL_CAUGHT:
			OnBallReceived((PLAYER_OBJ *)data1);
			return 1;

		case MSG_BALL_PASSED_SCRIMMAGE:
			OnBallPassedScrimmage((PLAYER_OBJ *)data1);
			return 1;

		case MSG_TOUCHDOWN:
			OnTouchdown((PLAYER_OBJ *)data1);
			return 1;

		case MSG_PLAY_DEAD:
			OnPlayDead();
			return 1;

		case MSG_GRAPPLED:
			return OnGrappled((PLAYER_OBJ *)data1);

		case MSG_GRAPPLE_BROKEN:
			OnGrappleBroken((PLAYER_OBJ *)data1);
			return 1;

		case MSG_TACKLED:
			OnTackled((PLAYER_OBJ *)data1);
			return 1;

		case MSG_KNOCKED_DOWN:
			OnKnockedDown((PLAYER_OBJ*)data1);
			return 1;

		case MSG_TAKEN_DOWN:
			OnTakenDown();
			return 1;

		case MSG_BALL_FUMBLED:
			OnBallFumbled();
			return 1;

		case MSG_HANDOFF_GIVEN:
			OnHandOffGiven();
			return 1;

		case MSG_FAKE_HANDOFF_GIVEN:
			OnFakeHandOffGiven();
			return 1;
		}
	
	return -1;
	}





void PLAYER_OBJ::OnBallHiked()
	{

	//////////////////////////////////////
	//									//
	// Used to handle ball hiked events //
	//									//
	//////////////////////////////////////


	BeginPostHikeActions();
	}





void PLAYER_OBJ::OnHandOffGiven()
	{

	/////////////////////////////////////////
	//									   //
	// Used to handle handoff given events //
	//									   //
	/////////////////////////////////////////

	

	// Acquire the ball

	GetBall();

	// or fumble
	}





void PLAYER_OBJ::OnFakeHandOffGiven()
	{

	//////////////////////////////////////////////
	//									        //
	// Used to handle fake handoff given events //
	//										    //
	//////////////////////////////////////////////


	// Pop the current action

	DoNextAction();
	}





void PLAYER_OBJ::OnBallThrown(int time)
	{

	///////////////////////////////////////
	//									 //
	// Used to handle ball thrown events //
	//									 //
	///////////////////////////////////////



	// If this player cant reach the ball before it lands, then exit

	if (TimeToPosition(Ball.CatchSpot.Wld.X, Ball.CatchSpot.Wld.Y) <= Ball.TimeToCatchSpot || SideNum == OFFENSE)
		{
		Dest.X = Ball.CatchSpot.Wld.X;
		Dest.Y = Ball.CatchSpot.Wld.Y;
		}
	else if (TimeToPosition(Ball.LandingSpot.Wld.X, Ball.LandingSpot.Wld.Y) <= Ball.TimeToLandingSpot + 20)
		{
		Dest.X = Ball.LandingSpot.Wld.X;
		Dest.Y = Ball.LandingSpot.Wld.Y;
		}
	else if (TimeToPosition(Ball.InRangeSpot.Wld.X, Ball.InRangeSpot.Wld.Y) <= Ball.TimeToInRangeSpot)
		{
		Dest.X = Ball.InRangeSpot.Wld.X;
		Dest.Y = Ball.InRangeSpot.Wld.Y;
		}
	else
		return;



	// Clear out the actions 

	Actions.Clear();


	// Push the delay action

	Actions.PushEnd(ACTION_DELAY, rand() % FPS);


	// Push the catch ball action

	Actions.PushEnd(ACTION_CATCH_BALL, -1);
	}





void PLAYER_OBJ::OnBallReceived(PLAYER_OBJ * target)
	{

	/////////////////////////////////////////
	//									   //
	// Used to handle ball received events //
	//									   //
	/////////////////////////////////////////


	int LastActionType;


	// Store the last action type

	LastActionType = Actions.GetType();


	// Clear out the actions 

	Actions.Clear();


	// If this player was blocking, being blocked, grappling or being grappled, repush that
	// action



	// Otherwise, push the last action with a delayed time (based on AWARENESS)

	//else if (target != this)
	//	Actions.PushEnd(LastActionType, 0);


	// Do nothing if this is the receiver //

	if (target == this)	
		return;

	

	// If the player who received the ball is on this player's team
	// then block for that player and set UserControl to false

	if (target->SideNum == SideNum)
		Actions.PushEnd(ACTION_BLOCK_FOR_BALL_HOLDER);

	// else if the player who received the ball is on the other team
	// then track that player

	else
		{
		TrackTarget = target;
		Actions.PushEnd(ACTION_TACKLE_BALL_HOLDER);
		}
	}





void PLAYER_OBJ::OnBallFumbled()
	{

	////////////////////////////////////////
	//									  //
	// Used to handle ball fumbled events //
	//									  //
	////////////////////////////////////////


	int LastActionType;


	// Store the last action type

	LastActionType = Actions.GetType();


	// Clear out the actions 

	Actions.Clear();


	// If this player was blocking, being blocked, grappling or being grappled, repush that
	// action

	//if (LastActionType == ACTION_GRAPPLE || LastActionType == ACTION_BREAKGRAPPLE || 
	//	LastActionType == ACTION_BLOCK || LastActionType == ACTION_BREAKBLOCK)
	//	Actions.PushEnd(LastActionType, -1);


	// Otherwise, push the last action with a delayed time (based on AWARENESS)

	//else if (target != this)
	//	Actions.PushEnd(LastActionType, 0);


	// Push the pickup ball action 

	Actions.PushEnd(ACTION_DELAY, FPS);
	Actions.PushEnd(ACTION_RECOVER_FUMBLE);
	}





void PLAYER_OBJ::OnBallPassedScrimmage(PLAYER_OBJ * target)
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to handle ball passed scrimmage events //
	//											   //
	/////////////////////////////////////////////////


	// Do nothing if this is the player

	if (target == this)
		return;


	Actions.Clear();
	

	// If the player is on this side, then block for him

	if (target->SideNum == SideNum)
		{
		Actions.Delay(rand() % (FPS * 2));
		Actions.PushEnd(ACTION_BLOCK_FOR_BALL_HOLDER);
		}


	// else if the player who has the ball is on the other team
	// then track that player

	else
		{
		TrackTarget = target;
		Actions.Delay(rand() % (FPS * 5));
		Actions.PushEnd(ACTION_TACKLE_BALL_HOLDER);
		}		
	}





void PLAYER_OBJ::OnPlayDead()
	{

	/////////////////////////////////////
	//								   //
	// Used to handle play dead events //
	//								   //
	/////////////////////////////////////


	// Just stop running

	Actions.Clear();
	Stand();
	}





void PLAYER_OBJ::OnTouchdown(PLAYER_OBJ * target)
	{

	/////////////////////////////////////
	//								   //
	// Used to handle touchdown events //
	//								   //
	/////////////////////////////////////


	// Celebrate if this player's team 
	
	Actions.Clear();

	if (target->SideNum == SideNum)
		Celebrate();
	else
		Complain();
	}





DWORD PLAYER_OBJ::OnGrappled(PLAYER_OBJ * target)
	{

	////////////////////////////////////
	//								  //
	// Used to handle grappled events //
	//								  //
	////////////////////////////////////


	float dirx, diry;


	if (State != STATE_STAND && State != STATE_RUN && State != STATE_THROW && State != STATE_BREAKGRAPPLE)
		return (DWORD)FALSE;


	if (!AddGrappler(target))
		return FALSE;


	// Get the direction of the grappler 

	GetDirectionToSpot(target->Wld.X, target->Wld.Y, &dirx, &diry);

	
	// Set the state to break grapple

	BreakGrapple(dirx, diry);


	return (DWORD)TRUE;
	}





void PLAYER_OBJ::OnGrappleBroken(PLAYER_OBJ * target)
	{

	//////////////////////////////////////////
	//								        //
	// Used to handle grapple broken events //
	//								        //
	//////////////////////////////////////////


	RemoveGrappler(target);

	if (NumGrapplers == 0)
		Stand();
	}





void PLAYER_OBJ::OnTackled(PLAYER_OBJ * target)
	{

	///////////////////////////////////
	//								 //
	// Used to handle tackled events //
	//								 //
	///////////////////////////////////


	LayDown();
	}





void PLAYER_OBJ::OnKnockedDown(PLAYER_OBJ * target)
	{

	////////////////////////////////////////
	//									  //
	// Used to handle knocked down events //
	//									  //
	////////////////////////////////////////


	RemoveAllGrapplers();


	if (HasBall)
		SendMessage(0, MSG_BALL_HOLDER_DOWN, 0, (DWORD)this);


	//TODO : get right params

	Tumble(1,1);
	}




void PLAYER_OBJ::OnTakenDown()
	{

	//////////////////////////////////////
	//									//
	// Used to handle taken down events //
	//									//
	//////////////////////////////////////


	RemoveAllGrapplers();


	if (HasBall)
		SendMessage(0, MSG_BALL_HOLDER_DOWN, 0, (DWORD)this);


	LayDown();
	}



void PLAYER_OBJ::Stop()
	{

	///////////////////////////
	//						 //
	// Used to stop movement //
	//						 //
	///////////////////////////


	Vel.X		= 0.0f;
	Vel.Y		= 0.0f;
	DestVel.X	= 0.0f;
	DestVel.Y	= 0.0f;
	Acc.X		= 0.0f;
	Acc.Y		= 0.0f;
	}




void PLAYER_OBJ::SlowStop()
	{

	////////////////////////////////////
	//								  //
	// Used to slowly stop the player //
	//								  //
	////////////////////////////////////

	//Acc.X		= 0.05f;
	//Acc.Y		= 0.05f;
	//DestVel.X	= 0.0f;
	//DestVel.Y	= 0.0f;

	//if (Vel.X > 0.0f || Acc.X > 0.0f)	Acc.X = -0.15f;
	//if (Vel.Y > 0.0f || Acc.Y > 0.0f)	Acc.Y = -0.15f;

	SetState(STATE_SLOWDOWN);
	}







int PLAYER_OBJ::GetDirection(float xdir, float ydir)
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

	if (xdir <= -0.4999f)
		left = TRUE;
	else if (xdir >= 0.4999f)
		right = TRUE;


	// Set the up / down flags

	if (ydir <= -0.4999f)
		up = TRUE;
	else if (ydir >= 0.4999f)
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





void PLAYER_OBJ::Face(float xdir, float ydir)
	{

	/////////////////////////////////////////////
	//										   //
	// Used to face the player towards a point //
	//										   //
	/////////////////////////////////////////////


	FaceDir = GetDirection(xdir, ydir);
	}




BOOL PLAYER_OBJ::IsStanding()
	{

	/////////////////////////////////////////////
	//										   //
	// Used to check if the player is standing //
	//										   //
	/////////////////////////////////////////////


	return State == STATE_STAND;
	}




BOOL PLAYER_OBJ::IsFacing(float xdir, float ydir)
	{

	///////////////////////////////////////////////////////////////////
	//																 //
	// Used to check if the player is facing the specified direction //
	//																 //
	///////////////////////////////////////////////////////////////////


	int dir;


	dir = GetDirection(xdir, ydir);

	return (FaceDir == dir);
	}




void PLAYER_OBJ::FaceMoveDirection()
	{

	////////////////////////////////////////////////////////
	//													  //
	// Used to face the player towards the move direction //
	//													  //
	////////////////////////////////////////////////////////


	int dir;


	dir = GetDirection(MoveDir.X, MoveDir.Y);

	if (dir != DIR_NONE)	FaceDir = dir;
	}





////////////////////
// ROLL FUNCTIONS //
////////////////////





BOOL PLAYER_OBJ::RollForCatch()
	{

	////////////////////////////////////////
	//									  //
	// Used to perform a roll for a catch //
	//									  //
	////////////////////////////////////////


	int modifier	= 0;
	int difficulty	= 10;

	
	// Accumulate the modifiers

	modifier += PlayerInfo->Attrib.Catch.Bonus();


	// Adjust the difficulty

	difficulty += Ball.Delta.Z > 0 ? int(Ball.Delta.Z) : 0;
	difficulty += !IsStanding() ? 1 : -4;
	difficulty += SideNum == DEFENSE ? 4 : -4;
	difficulty += GetNumberOfDefendersNear();


	return Roll(20, modifier, difficulty);
	}





BOOL PLAYER_OBJ::RollForCatchSnap()
	{

	/////////////////////////////////////////////
	//									       //
	// Used to perform a roll for a catch snap //
	//									       //
	/////////////////////////////////////////////


	int modifier	= 0;
	int difficulty	= 1;

	
	// Accumulate the modifiers

	//modifier += PlayerInfo->Attrib.Catch.Bonus();


	// Adjust the difficulty

	//difficulty += Ball.Delta.Z > 0 ? int(Ball.Delta.Z) : 0;
	//difficulty += !IsStanding() ? 1 : -4;
	//difficulty += SideNum == DEFENSE ? 4 : -4;
	//difficulty += GetNumberOfDefendersNear();


	return Roll(100, modifier, difficulty);
	}




BOOL PLAYER_OBJ::RollForGrappleBreak(PLAYER_OBJ * target)
	{

	////////////////////////////////////////////////
	//											  //
	// Used to perform a roll for a grapple break //
	//											  //
	////////////////////////////////////////////////


	BOOL retval;
	int modifier	= 0;
	int difficulty	= 15;

	
	// Accumulate the modifiers

	modifier += PlayerInfo->Attrib.Strength.Bonus();
	modifier += PlayerInfo->Attrib.Stamina.Bonus();


	// Adjust the difficulty

	difficulty += target->PlayerInfo->Attrib.Strength.Bonus();
	difficulty += target->PlayerInfo->Attrib.Stamina.Bonus();


	// Perform the roll 

	retval = Roll(20, modifier, difficulty);


	// On success, send the messages

	if (retval)
		{
		SendMessage(target->GetMsgTargetID(), MSG_TAKEN_DOWN, 0, (DWORD)this);
		SendMessage(GetMsgTargetID(), MSG_GRAPPLE_BROKEN, 0, (DWORD)target);
		}


	return retval;
	}





BOOL PLAYER_OBJ::RollForKnockDown(PLAYER_OBJ * target)
	{

	////////////////////////////////////////////
	//										  //
	// Used to perform a roll for a knockdown //
	//										  //
	////////////////////////////////////////////


	BOOL retval;
	int modifier	= 0;
	int difficulty	= 17;
	int max			= 20;



	// If running, add +2 bonus

	if (State == STATE_RUN)
		modifier += 2;



	// For ever 10 str above the other player, add +1

	if (GetAttrStrength() > target->GetAttrStrength())
		modifier += int((GetAttrStrength() - target->GetAttrStrength()) / 10);



	// For ever 10 sta above the other player, add +1

	if (GetAttrStamina() > target->GetAttrStamina())
		modifier += int((GetAttrStamina() - target->GetAttrStamina()) / 10);


	// If other is facing same direction, add +5

	if (target->FaceDir == FaceDir)
		modifier += 5;


	
	// If other is facing sideways, add +1
	// TODO : Complete this


	// If trying to tackle ballholder, and player is ball holder, add +2
	// TODO : Complete this


	// If other is running, subtract -1

	if (target->State == STATE_RUN)
		modifier -= 2;



	// For every 10 str other is above, subract -1

	if (GetAttrStrength() < target->GetAttrStrength())
		modifier -= int((target->GetAttrStrength() - GetAttrStrength()) / 10);



	// For every 10 sta other is above, subract -1

	if (GetAttrStamina() < target->GetAttrStamina())
		modifier -= int((target->GetAttrStamina() - GetAttrStamina()) / 10);



	// Perform the roll

	retval = Roll(max, modifier, difficulty);



	return retval;
	}




BOOL PLAYER_OBJ::RollForTakeDown(PLAYER_OBJ * target)
	{

	///////////////////////////////////////////
	//										 //
	// Used to perform a roll for a takedown //
	//										 //
	///////////////////////////////////////////


	BOOL retval;
	int modifier	= 0;
	int difficulty	= 10;
	int max			= 20;



	// If running, add +2 bonus

	if (State == STATE_RUN)
		modifier += 2;



	// For ever 10 str above the other player, add +1

	if (GetAttrStrength() > target->GetAttrStrength())
		modifier += int((GetAttrStrength() - target->GetAttrStrength()) / 10);



	// For ever 10 sta above the other player, add +1

	if (GetAttrStamina() > target->GetAttrStamina())
		modifier += int((GetAttrStamina() - target->GetAttrStamina()) / 10);


	// If other is facing same direction, add +5

	if (target->FaceDir == FaceDir)
		modifier += 5;


	
	// If other is facing sideways, add +1
	// TODO : Complete this


	// If trying to tackle ballholder, and player is ball holder, add +2
	// TODO : Complete this


	// If other is running, subtract -1

	if (target->State == STATE_RUN)
		modifier -= 2;



	// For every 10 str other is above, subract -1

	if (GetAttrStrength() < target->GetAttrStrength())
		modifier -= int((target->GetAttrStrength() - GetAttrStrength()) / 10);



	// For every 10 sta other is above, subract -1

	if (GetAttrStamina() < target->GetAttrStamina())
		modifier -= int((target->GetAttrStamina() - GetAttrStamina()) / 10);



	// Perform the roll

	retval = Roll(max, modifier, difficulty);



	return retval;
	}





int PLAYER_OBJ::TimeToPosition(float xpos, float ypos)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to calculate the time in frames that it would take  //
	// this player to reach the position specified by xpos/ypos //
	//															//
	//////////////////////////////////////////////////////////////


	float distx, disty;
	float distance;

	
	// Calculate the distance to the spot

	distx = Wld.X - xpos;
	disty = Wld.Y - ypos;
	distance = sqrt((distx * distx) + (disty * disty));


	// Calculate the amount of frame it would take to reach the spot

	return (Round(distance / DestVel.X));
	}





void PLAYER_OBJ::GetDirectionToSpot(float xpos, float ypos, float * pxdir, float * pydir)
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to get the direction to a specified spot //
	//												 //
	///////////////////////////////////////////////////


	float distx, disty, totaldist;


	// Validate the pointers 

	if (pxdir == NULL || pydir == NULL)
		return;


	// Get the distance to the spot

	distx = xpos - Wld.X;
	disty = ypos - Wld.Y;



	// Assign the pointer values

	
	totaldist = fabs(distx) + fabs(disty);


	if (totaldist == 0.0f)
		{
		*pxdir = 0.0f;
		*pydir = 0.0f;
		}
	else
		{
		*pxdir = distx / totaldist;
		*pydir = disty / totaldist;
		}
	}




float PLAYER_OBJ::GetDistanceToSpot(float xpos, float ypos)
	{

	//////////////////////////////////////
	//									//
	// Used to get a distance to a spot //
	//									//
	//////////////////////////////////////


	float distx, disty;


	// Get the x/y distances to the spot

	distx = xpos - Wld.X;
	disty = ypos - Wld.Y;


	// Return the distance

	return (sqrtf((distx * distx) + (disty * disty)));
	}





int PLAYER_OBJ::GetNumberOfDefendersNear()
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to return the number of defenders near this player //
	//														   //
	/////////////////////////////////////////////////////////////

	
	PLAYER_OBJ * target;
	int i;
	int numdefenders = 0;


	for (i = 0; i < MAX_SIDE_PLAYERS; i++)
		{
		target = &Side[!SideNum].Player[i];

		if (target->TrackTarget == this && GetDistanceToSpot(target->Wld.X, target->Wld.Y) <= Width * 2)
			numdefenders++;
		}

	return numdefenders;
	}





BOOL PLAYER_OBJ::IsPlayerInRange(PLAYER_OBJ * target, float distx, float disty)
	{

	//////////////////////////////////////////////////////////////////////////
	//																		//
	// Used to check if a player is within a specified range of this player //
	//																		//
	//////////////////////////////////////////////////////////////////////////
	
	
	BOOL inrangex, inrangey;


	if (GameInfo.Direction[TeamNum] == DIR_LEFT)
		{
		inrangex = (target->Wld.X >= (Wld.X + distx));
		inrangey = (target->Wld.Y <= Wld.Y + disty && target->Wld.Y >= Wld.Y - disty);
		}
	else
		{
		inrangex = (target->Wld.X <= (Wld.X - distx));
		inrangey = (target->Wld.Y <= Wld.Y + disty && target->Wld.Y >= Wld.Y - disty);
		}


	return (inrangex && inrangey);
	}





PLAYER_OBJ * PLAYER_OBJ::FindNearestPlayerToBlock()
	{

	////////////////////////////////////////////////
	//											  //
	// Used to find the nearest opponent to block //
	//											  //
	////////////////////////////////////////////////


	PLAYER_OBJ * nearest = NULL;
	float playerdist;
	float lastdist = 9999999.0f;
	int i;


	// Check each opponent 

	for (i = 0; i < MAX_SIDE_PLAYERS; i++)
		{

		// If the opponent is already grappling then continue

		if (Side[!SideNum].Player[i].State == STATE_BREAKGRAPPLE ||
			Side[!SideNum].Player[i].State == STATE_TUMBLE ||
			Side[!SideNum].Player[i].State == STATE_LAYDOWN)
			continue;

		
		// Get the distance to the opponent

		playerdist = GetDistanceToSpot(Side[!SideNum].Player[i].Wld.X, Side[!SideNum].Player[i].Wld.Y);

		
		// If this opponent is closer than the last, set the flag to this opponent

		if (playerdist < lastdist)
			{
			nearest = &Side[!SideNum].Player[i];
			lastdist = playerdist;
			}
		}


	// Return the pointer to the nearest opponent
	return nearest;
	}





PLAYER_OBJ * PLAYER_OBJ::FindUntargetedReceiver()
	{

	///////////////////////////////////////////////////////////////////////
	//																	 //
	// Used to find a receiver who is not yet targeted by another player //
	//																	 //
	///////////////////////////////////////////////////////////////////////


	PLAYER_OBJ * target = NULL;
	BOOL targeted;
	int i, ii;


	// Scroll through each player of the opposite team ...

	for (i = 0; i < MAX_SIDE_PLAYERS; i++)
		{

		// And if a receiver is found ...

		if (Side[!SideNum].Player[i].Flag.Receiver)
			{	
			target		= &Side[!SideNum].Player[i];
			targeted	= FALSE;


			// Check the TrackTarget of each player on this team to make sure the player
			// is not already targeted

			for (ii = 0; ii < MAX_SIDE_PLAYERS; ii++)
				if (this != &Side[SideNum].Player[ii] && Side[SideNum].Player[ii].TrackTarget == target)
					targeted = TRUE;


			// If not targeted by anyone else, return the pointer

			if (!targeted)
				return target;
			}
		}


	return NULL;
	}





PLAYER_OBJ * PLAYER_OBJ::FindClosestUntargetedReceiver()
	{

	/////////////////////////////////////////////////////////////////////////////////
	//																			   //
	// Used to find the closest receiver who is not yet targeted by another player //
	//																			   //
	/////////////////////////////////////////////////////////////////////////////////


	PLAYER_OBJ * target = NULL;
	PLAYER_OBJ * finaltarget = NULL;
	BOOL targeted;
	int i, ii;
	float dist, lastdist = 99999.0f;


	// Scroll through each player of the opposite team ...

	for (i = 0; i < MAX_SIDE_PLAYERS; i++)
		{

		// And if a receiver is found ...

		if (Side[!SideNum].Player[i].Flag.Receiver)
			{	
			target		= &Side[!SideNum].Player[i];
			targeted	= FALSE;


			// Check the TrackTarget of each player on this team to make sure the player
			// is not already targeted

			for (ii = 0; ii < MAX_SIDE_PLAYERS; ii++)
				if (this != &Side[SideNum].Player[ii] && Side[SideNum].Player[ii].TrackTarget == target)
					targeted = TRUE;


			// If not targeted by anyone else, return the pointer

			if (!targeted)
				{
				dist = GetDistanceToSpot(target->Wld.X, target->Wld.Y);

				if (dist < lastdist)
					{
					finaltarget = target;
					lastdist = dist;
					}
				}
			}
		}


	return finaltarget;
	}






PLAYER_OBJ * PLAYER_OBJ::FindReceiverForMan(int recnum, int routedir)
	{

	///////////////////////////////////////////////////////////////////
	//																 //
	// Used to find the receiver whose receiver number and route dir //
	// match a given value											 //
	//																 //
	///////////////////////////////////////////////////////////////////



	// Scroll through each player of the opposite team ...

	for (int i = 0; i < MAX_SIDE_PLAYERS; i++)
		{

		// And if a receiver is found and his recnum and routedir match
		// return the address of the player

		if (Side[!SideNum].Player[i].Flag.Receiver && Side[!SideNum].Player[i].ReceiverNum == recnum)
			{
			if (routedir == 0 ||  Side[!SideNum].Player[i].RouteDir == routedir)
				return &Side[!SideNum].Player[i];
			}
		}


	return NULL;
	}





BOOL PLAYER_OBJ::AddGrappler(PLAYER_OBJ * target)
	{

	//////////////////////////////////////////////////
	//												//
	// Used to add a grappler to the grappler array //
	//												//
	//////////////////////////////////////////////////


	// Check if amount of grappler slots is full

	if (NumGrapplers >= MAX_GRAPPLERS)
		return FALSE;


	// Check if grappler is already set

	for (int i = 0; i < NumGrapplers; i++)
		if (Grapplers[i] == target)
			return FALSE;


	Grapplers[NumGrapplers] = target;
	NumGrapplers++;

	return TRUE;
	}




void PLAYER_OBJ::RemoveGrappler(PLAYER_OBJ * target)
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to remove a grappler from the grappler array //
	//													 //
	///////////////////////////////////////////////////////


	for (int i = 0; i < NumGrapplers - 1; i++)
		if (Grapplers[i] == target)
			{
			memcpy(&Grapplers[i], &Grapplers[i + 1], sizeof(PLAYER_OBJ *) * (NumGrapplers - 1 - i));
			NumGrapplers--;
			return;
			}

	NumGrapplers--;
	}





void PLAYER_OBJ::Damage(float amt)
	{

	////////////////////////////////////////
	//									  //
	// Used to damage the players stamina //
	//									  //
	////////////////////////////////////////


	PlayerInfo->Attrib.Stamina.Val -= amt;

	if (PlayerInfo->Attrib.Stamina.Val < 0.0f)
		PlayerInfo->Attrib.Stamina.Val = 0.0f;

	if (PlayerInfo->Attrib.Stamina.Val > 100.0f)
		PlayerInfo->Attrib.Stamina.Val = 100.0f;
	}




