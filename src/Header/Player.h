#ifndef __PLAYER_H
#define __PLAYER_H


#include "Gamelib.h"
#include "Actions.h"



#define CONTROL_USER			0
#define CONTROL_ACTIONS			1
#define CONTROL_TRACKPLAYER		2
#define CONTROL_STATE			3



//////////////////
// State Macros //
//////////////////

#define ENTER	0
#define UPDATE	1
#define EXIT	2

#define OnEnter()	if (reason == ENTER)
#define OnUpdate()	if (reason == UPDATE)
#define OnExit()	if (reason == EXIT)




struct ATTRIBUTES
	{
	int Accel;
	int Speed;
	int Awareness;
	int Catch;
	int Carry;
	int BreakTackle;
	int Strength;
	};




///////////////////
// Player Object //
///////////////////


class PLAYER : public OBJECT
	{
	public:
		
		PLAYER();

		enum {	STATE_STAND			= 0,
				STATE_RUN,
				STATE_RUNTO,
				STATE_LEAP,
				STATE_DIVE,

				STATE_GRAPPLE,
				STATE_BREAKGRAPPLE,
				STATE_TUMBLE,
	
				STATE_CELEBRATE,
				STATE_COMPLAIN
			};


		virtual void SetSide(){}

		void Stop();
	
		virtual void Update();

		virtual void OnBallHiked(){}
		virtual void OnBallThrown(int destx, int desty){}
		virtual void OnBallReceived(PLAYER * target);
		virtual void OnBallFumbled(int destx, int desty){}
		virtual void OnPlayDead(){}
		virtual void OnTouchdown(){}

	
	public:

		int Side;
		
		int ControlType;

		int FaceDir;
		VECTOR MoveV;

		int State;
		int StateDone;
		int StateCount;
		int StateTime;

		int Speed;
		
		BOOL Catching;
		BOOL CanGrab;
		BOOL CanBeGrabbed;
		BOOL CanTackle;
		BOOL CanBeTackled;


	protected:

		void ProcessControl();
		void DoUserControl();
		void DoActionControl();
		void DoStateControl();

		void UpdateDeltas();


	protected:

		void SetState(int state, int param = -1);

		void ProcessState(int reason, int param = -1);

		virtual void DoStand(int reason);
		virtual void DoRun(int reason, int dir);
		virtual void DoRunTo(int reason);
		virtual void DoLeap(int reason);
		virtual void DoDive(int reason);
		virtual void DoGrapple(int reason);
		virtual void DoBreakGrapple(int reason);
		virtual void DoTumble(int reason);
		virtual void DoCelebrate(int reason);
		virtual void DoComplain(int reason);


	public:

		void SetActions(ACTIONS * actions);	
		void BeginActions();					
		void DoAction();
		void DoNextAction();


	public: 

		int ActionPos;
		int ActionCount;
		ACTIONS * Actions;

		ACTIONS PreHikeActions;
		ACTIONS PostHikeActions;
		

	public:
		
		PLAYER * Target;
	};





#endif