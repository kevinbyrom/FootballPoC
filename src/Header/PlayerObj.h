#ifndef __PLAYEROBJ_H
#define __PLAYEROBJ_H


#include "Gamelib.h"
#include "PlayerInfo.h"
#include "Constants.h"


///////////////
// Constants //
///////////////


const float PLAYER_WIDTH			= 20.0f;
const float PLAYER_HEIGHT			= 32.0f;



////////////
// States //
////////////


DEFINE_STATE( STAND, 0)
DEFINE_STATE( STANCE , 1 )
DEFINE_STATE( RUN, 2 )
DEFINE_STATE( SLOWDOWN, 3 )
DEFINE_STATE( LEAP, 4 )
DEFINE_STATE( DIVE, 5 )
DEFINE_STATE( GRAPPLE, 6 )
DEFINE_STATE( BREAKGRAPPLE, 7 )
DEFINE_STATE( TUMBLE, 8 )
DEFINE_STATE( LAYDOWN, 9 )
DEFINE_STATE( SIT, 10 )
DEFINE_STATE( GETUP, 11 )
DEFINE_STATE( HANDOFF, 12 )
DEFINE_STATE( FAKEHANDOFF, 13 )
DEFINE_STATE( TOSS, 14 )
DEFINE_STATE( SNAP, 15 )
DEFINE_STATE( THROW, 16 )
DEFINE_STATE( KICK, 17 )
DEFINE_STATE( AIMKICK, 18 )
DEFINE_STATE( PUNT, 19 )
DEFINE_STATE( PLACEHOLD, 20 )
DEFINE_STATE( CELEBRATE, 21 )
DEFINE_STATE( COMPLAIN, 22 )




/////////////
// Actions //
/////////////


DEFINE_ACTION( STAND, 0 ) 
DEFINE_ACTION( STANCE, 0 ) 
DEFINE_ACTION( RUN, 2)
DEFINE_ACTION( RUN_TOSS, 3)
DEFINE_ACTION( RUN_TO, 4)
DEFINE_ACTION( RUNTO_ABSOLUTE, 5 )
DEFINE_ACTION( RUNTO_BALL_HOLDER, 6 )
DEFINE_ACTION( FACE, 7 )
DEFINE_ACTION( BLOCK, 8 )

DEFINE_ACTION( HANDOFF, 20 )
DEFINE_ACTION( FAKE_HANDOFF, 21 )
DEFINE_ACTION( TOSS, 22 )
DEFINE_ACTION( FAKE_TOSS, 23 )
DEFINE_ACTION( GET_HANDOFF, 24 )
DEFINE_ACTION( GET_FAKE_HANDOFF, 25 )

DEFINE_ACTION( SPIKE, 40 )
DEFINE_ACTION( KNEEL, 41 )

DEFINE_ACTION( SNAP, 60 )
DEFINE_ACTION( PLACEHOLD, 61 )
DEFINE_ACTION( KICK, 62 )
DEFINE_ACTION( PUNT, 63 )
DEFINE_ACTION( START_POWER, 64 )
DEFINE_ACTION( END_POWER, 65 )
DEFINE_ACTION( AIM_KICK, 66 )
DEFINE_ACTION( END_AIM,	67 )

DEFINE_ACTION( WAIT_FOR_SNAP_QB, 80 ) 
DEFINE_ACTION( WAIT_FOR_SNAP_P, 81 )
DEFINE_ACTION( WAIT_FOR_SNAP_PH, 82 )

DEFINE_ACTION( CATCH_PUNT, 100 )
DEFINE_ACTION( CATCH_BALL, 101 )
DEFINE_ACTION( BLOCK_FOR_BALL_HOLDER, 102 )
DEFINE_ACTION( TACKLE_BALL_HOLDER, 103 )
DEFINE_ACTION( RECOVER_FUMBLE, 104 )

DEFINE_ACTION( TARGET_PASSER, 120 )
DEFINE_ACTION( TARGET_RUSHER, 121 )
DEFINE_ACTION( TARGET_KICKER, 122 )
DEFINE_ACTION( TARGET_PUNTER, 123 )
DEFINE_ACTION( TARGET_PLACEHOLDER, 124 )

DEFINE_ACTION( FIND_RECEIVER, 200 )
DEFINE_ACTION( FIND_CLOSEST_RECEIVER, 201 )
DEFINE_ACTION( FIND_RECEIVER_FOR_MAN, 202 )
DEFINE_ACTION( LINEUP_TO_RECEIVER, 203 )
DEFINE_ACTION( WAIT_FOR_RECEIVER, 204 )
DEFINE_ACTION( WAIT_FOR_ANY_RECEIVER, 205 )
DEFINE_ACTION( SAFETY_WAIT_HIGH, 206 )
DEFINE_ACTION( SAFETY_WAIT_LOW, 207 )
DEFINE_ACTION( COVER_RECEIVER, 208 )
DEFINE_ACTION( SPY_TARGET, 209 )

DEFINE_ACTION( HIDE_BALL_HOLDER, 300 )
DEFINE_ACTION( REVEAL_BALL_HOLDER, 301 )

DEFINE_ACTION( ASSIGN_RECEIVER_NUM, 350 )

DEFINE_ACTION( USER_CONTROL, 320 )
DEFINE_ACTION( DELAY, 321 )


const int FRAME_STAND			= 0;
const int FRAME_RUN				= 1;
const int FRAME_LEAP			= 10;
const int FRAME_DIVE			= 7;
const int FRAME_TUMBLE			= 9;
const int FRAME_GRAPPLE			= 11;
const int FRAME_LAY				= 13;
const int FRAME_SIT				= 14;
const int FRAME_GETUP			= 16;
const int FRAME_STANCE			= 18;
const int FRAME_THROW			= 19;
const int FRAME_HANDOFF			= 22;

const int FRAME_AMT_STAND		= 1;
const int FRAME_AMT_RUN			= 4;
const int FRAME_AMT_LEAP		= 1;
const int FRAME_AMT_DIVE		= 1;
const int FRAME_AMT_GRAPPLE		= 2;
const int FRAME_AMT_LAY			= 1;
const int FRAME_AMT_SIT			= 2;
const int FRAME_AMT_GETUP		= 2;
const int FRAME_AMT_STANCE		= 1;
const int FRAME_AMT_THROW		= 3;
const int FRAME_AMT_HANDOFF		= 2;

const float STAND_DAMAGE	= -0.01f;
const float RUN_DAMAGE		= 0.001f;
const float GRAPPLE_DAMAGE	= 0.05f;



struct PLAYER_OBJ_FLAGS
	{
	BOOL Passer;
	BOOL Receiver;
	BOOL IsDefReceiver;
	BOOL Blocker;
	BOOL Rusher;
	BOOL Kicker;
	BOOL Punter;
	BOOL Placeholder;
	BOOL IsDefBallHolder;
	};




///////////////////
// Player Object //
///////////////////


class PLAYER_OBJ : public OBJECT
	{
	public:
		
		PLAYER_OBJ();


		virtual void SetSide(){}
		void PrepareForPlay();

	
	public:

		void SetPlayerInfo(PLAYER_INFO * pi);

		PLAYER_INFO * PlayerInfo;

		float GetAttrStrength()			{ return PlayerInfo->Attrib.Strength.Value(); }
		float GetAttrStamina()			{ return PlayerInfo->Attrib.Stamina.Value(); }
		float GetAttrAwareness()		{ return PlayerInfo->Attrib.Aware.Value(); }
		float GetAttrThrowPower()		{ return PlayerInfo->Attrib.ThrowPow.Value(); }
		float GetAttrThrowAccuracy()	{ return PlayerInfo->Attrib.ThrowAcc.Value(); }
		float GetAttrCatchAttr()		{ return PlayerInfo->Attrib.Catch.Value(); }
		float GetAttrAcceleration()		{ return PlayerInfo->Attrib.Accel.Value(); }
		float GetAttrSpeed()			{ return PlayerInfo->Attrib.Speed.Value(); }
		float GetAttrBreakTackle()		{ return PlayerInfo->Attrib.BreakTak.Value(); }
		float GetAttrTackle()			{ return PlayerInfo->Attrib.Tackle.Value(); }
		float GetAttrBlock()			{ return PlayerInfo->Attrib.Block.Value(); }
		float GetAttrBreakBlock()		{ return PlayerInfo->Attrib.BreakBlock.Value(); }
		float GetAttrKickPower()		{ return PlayerInfo->Attrib.KickPow.Value(); }
		float GetAttrKickAccuracy()		{ return PlayerInfo->Attrib.KickAcc.Value(); }

		int SideNum;
		int TeamNum;

		int DepthType;
		int DepthPos;
		int ReceiverNum;
		int RouteDir;

		int ControlType;

		int FaceDir;
		
		int Speed;
		
		int ThrowType;
		float ThrowAcc;
		float ThrowPow;
		float ThrowCtr;
		
		COORDINATE_F	KickDir;
		float			KickPow;


		BOOL Catching;
		BOOL HasBall;

		PLAYER_OBJ_FLAGS Flag;


	protected:

		void ProcessControl();
		//virtual void ProcessState(int reason);
		void DoControl_Action();


	protected:

		virtual DWORD OnMessage(int from, int msg, DWORD data1, DWORD data2);

		void OnBallHiked();
		void OnHandOffGiven();
		void OnFakeHandOffGiven();
		void OnBallThrown(int time);
		void OnBallReceived(PLAYER_OBJ * target);
		void OnBallFumbled();
		void OnBallPassedScrimmage(PLAYER_OBJ * target);
		void OnPlayDead();
		void OnTouchdown(PLAYER_OBJ * target);
		DWORD OnGrappled(PLAYER_OBJ * target);
		void OnGrappleBroken(PLAYER_OBJ * target);
		void OnTackled(PLAYER_OBJ * target);
		void OnKnockedDown(PLAYER_OBJ * target);
		void OnTakenDown();
		

	public:

		virtual void CheckCoordinates();

		void Stop();
		void SlowStop();

		BOOL IsStanding();
		BOOL IsFacing(float xdir, float ydir);
		void FaceMoveDirection();

		int GetDirection(float xdir, float ydir);


	public:

		void Face(float xdir, float ydir);
		void Stand();
		void Stance();
		void Run(float xdir, float ydir);
		void Leap(float xdir, float ydir);
		void Dive(float xdir, float ydir);
		void Grapple(float xdir, float ydir);
		void BreakGrapple(float xdir, float ydir);
		void Tumble(float xdir, float ydir);
		void LayDown();
		void Sit();
		void GetUp();
		void HandOff();
		void FakeHandOff();
		void Toss();
		void Snap();
		void Throw();
		void Punt();
		void Kick();
		void AimKick();
		void Placehold();
		void Celebrate();
		void Complain();


		void DoState_Stand(int reason);
		void DoState_Stance(int reason);
		void DoState_Run(int reason);
		void DoState_SlowDown(int reason);
		void DoState_Leap(int reason);
		void DoState_Dive(int reason);
		void DoState_Grapple(int reason);
		void DoState_BreakGrapple(int reason);
		void DoState_Tumble(int reason);
		void DoState_LayDown(int reason);
		void DoState_Sit(int reason);
		void DoState_GetUp(int reason);
		void DoState_HandOff(int reason);
		void DoState_FakeHandOff(int reason);
		void DoState_Toss(int reason);
		void DoState_Snap(int reason);
		void DoState_Throw(int reason);
		void DoState_Kick(int reason);
		void DoState_AimKick(int reason);
		void DoState_Punt(int reason);
		void DoState_Placehold(int reason);
		void DoState_Celebrate(int reason);
		void DoState_Complain(int reason);

		DECLARE_STATE_MAP()


		void GetBall();
		void HandOverBall(PLAYER_OBJ * target);
		void TossBallToPlayer(PLAYER_OBJ * player);
		void ThrowBallToReceiver();
		void PuntBall();
		void KickBall();

		BOOL TryToKnockDown(PLAYER_OBJ * target);
		BOOL TryToTakeDown(PLAYER_OBJ * target);
		void TryToBreakGrapple();
		void TryToCatch();
		void TryToCatchSnap();



		void Damage(float amt);



	public:

		void BeginActions();					
		void BeginPreHikeActions()		{ Actions.Clear(); Actions = PreHikeActions; BeginActions(); }
		void BeginPostHikeActions()		{ Actions.Clear(); Actions = PostHikeActions; BeginActions(); }

		//void DoAction(int reason);
		//void DoNextAction();

		void DoAction_Stand(int reason);
		void DoAction_Stance(int reason);
		void DoAction_Run(int reason);
		void DoAction_Face(int reason);
		void DoAction_RunToss(int reason);
		void DoAction_RunTo(int reason);
		void DoAction_RunToCatchBall(int reason);
		void DoAction_CatchBall(int reason);
		void DoAction_Block(int reason);
		void DoAction_HandOff(int reason);
		void DoAction_FakeHandOff(int reason);
		void DoAction_Toss(int reason);
		void DoAction_Snap(int reason);
		void DoAction_WaitForSnapQB(int reason);
		void DoAction_WaitForSnapPH(int reason);
		void DoAction_RunToBallHolder(int reason);
		void DoAction_Spike(int reason);
		void DoAction_Kneel(int reason);
		void DoAction_PlaceHold(int reason);
		void DoAction_AimKick(int reason);
		void DoAction_EndAim(int reason);
		void DoAction_Kick(int reason);
		void DoAction_Punt(int reason);
		void DoAction_FindReceiver(int reason);
		void DoAction_FindClosestReceiver(int reason);
		void DoAction_FindReceiverForMan(int reason);
		void DoAction_LineUpToReceiver(int reason);
		void DoAction_WaitForReceiver(int reason);
		void DoAction_WaitForAnyReceiver(int reason);
		void DoAction_SpyTarget(int reason);
		void DoAction_SafetyWaitHigh(int reason);
		void DoAction_SafetyWaitLow(int reason);
		void DoAction_CoverReceiver(int reason);
		void DoAction_CatchPunt(int reason);
		void DoAction_BlockForBallHolder(int reason);
		void DoAction_TackleBallHolder(int reason);
		void DoAction_RecoverFumble(int reason);
		void DoAction_TargetPasser(int reason);
		void DoAction_TargetRusher(int reason);
		void DoAction_TargetPlaceholder(int reason);
		void DoAction_AssignReceiverNum(int reason);
		void DoAction_UserControl(int reason);

		DECLARE_ACTION_MAP()


	public: 

		ACTIONS PreHikeActions;
		ACTIONS PostHikeActions;
		

	public:
		
		void SetTrackTarget(PLAYER_OBJ * target)	{ TrackTarget = target; TrackCount = 0; }
		PLAYER_OBJ	* TrackTarget;
		int TrackCount;

		BOOL AddGrappler(PLAYER_OBJ * target);
		void RemoveGrappler(PLAYER_OBJ * target);
		void RemoveAllGrapplers()					{ NumGrapplers = 0; }

		PLAYER_OBJ	* GrappleTarget;
		PLAYER_OBJ	* Grapplers[MAX_GRAPPLERS];
		int NumGrapplers;

		int PadNum;


	public:

		BOOL RollForCatch();
		BOOL RollForCatchSnap();
		BOOL RollForGrappleBreak(PLAYER_OBJ * target);
		BOOL RollForKnockDown(PLAYER_OBJ * target);
		BOOL RollForTakeDown(PLAYER_OBJ * target);

		int TimeToPosition(float xpos, float ypos);
		void GetDirectionToSpot(float xpos, float ypos, float * pxdir, float * pydir);
		float GetDistanceToSpot(float xpos, float ypos);
		int GetNumberOfDefendersNear();

		BOOL IsPlayerInRange(PLAYER_OBJ * target, float distx, float disty);
		

		PLAYER_OBJ * FindNearestPlayerToBlock();
		PLAYER_OBJ * FindUntargetedReceiver();
		PLAYER_OBJ * FindClosestReceiver();
		PLAYER_OBJ * FindClosestUntargetedReceiver();
		PLAYER_OBJ * FindReceiverForMan(int recnum, int routedir);
	};



#endif