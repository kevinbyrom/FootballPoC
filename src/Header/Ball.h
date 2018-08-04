#ifndef __BALL_H
#define __BALL_H


#include "Gamelib.h"


class PLAYER_OBJ;


/////////////////
// Ball Object //
/////////////////


class BALL : public OBJECT
	{
	public:
		
		enum {	STATE_THROWN = 0,
				STATE_FUMBLED };

		BALL();

		virtual void Update();

	
	public:

		int FaceDir;

		OBJECT InRangeSpot;
		OBJECT LandingSpot;
		OBJECT CatchSpot;
		
		int TimeToInRangeSpot;
		int TimeToLandingSpot;
		int TimeToCatchSpot;


	protected:

		void Stop();
		void SlowStop();
		void UpdateDeltas();

		int GetDirection(float xdir, float ydir);
		void FaceMoveDirection();


	public:

		BOOL IsLoose()		{ return Loose; }
		BOOL WasThrown()	{ return Thrown; }

		void ThrowTo(float x, float y, float pow, float acc, float lead);
		void Toss(float xdir, float ydir, float zdir);
		void Throw(float xdir, float ydir, float zdir);
		void Punt(float xdir, float ydir, float zdir);
		void Kick(float xdir, float ydir, float zdir);

		void Bounce();
		void Caught(PLAYER_OBJ * player);


		COORDINATE_F	Origin;
		PLAYER_OBJ *	Holder;


	protected:

		void CalculateSpots(float xdir, float ydir, float zdir);

		
		int State;
		int Loose;
		int Punted;
		int Kicked;
		int Thrown;
		int Tossed;
		int HitGround;
	};





#endif