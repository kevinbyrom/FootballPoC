#include <windows.h>
#include "Receiver.h"


void RECEIVER::OnBallHiked()
	{
	CanBeGrabbed = FALSE;
	CanBeTackled = FALSE;
	
	BeginActions();
	}




void RECEIVER::OnBallThrown(PLAYER * target, int destx, int desty)
	{
	// If ball is thrown to this receiver then run to destx/y //
	
	if (target == this)
		{
		//RunTo(destx, desty);
		}
	}

