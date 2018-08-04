#ifndef __RECEIVER_H
#define __RECEIVER_H


#include "Player.h"
#include "Constants.h"


class RECEIVER : public PLAYER
	{
	public:

		virtual void SetSide()		{ OFFENSE; }

		virtual void OnBallHiked();
		virtual void OnBallThrown(PLAYER * target, int destx, int desty);
		virtual void OnBallFumbled(int destx, int desty){}
		virtual void OnPlayDead(){}
		virtual void OnTouchdown(){}
	};



#endif