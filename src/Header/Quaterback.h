#ifndef __QUATERBACK_H
#define __QUATERBACK_H


#include "Player.h"
#include "Constants.h"


class QUATERBACK : public PLAYER
	{
	public:

		virtual void SetSide()		{ Side = OFFENSE; }

		virtual void OnBallHiked();
		virtual void OnBallThrown(int destx, int desty){}
		virtual void OnBallReceived(PLAYER * target){}
		virtual void OnBallFumbled(int destx, int desty){}
		virtual void OnPlayDead(){}
		virtual void OnTouchdown(){}
	};



#endif