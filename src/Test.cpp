#include <windows.h>
#include "Test.h"
#include "Person.h"

PERSON Obj;
SPRITE Sprite;
TESTSTAGE TestStage;

int frames[] = { 0, 1, 1, 1, ANIM_LOOP, 0};


SURFACE Bmp;

void TESTGAME::Opening()
	{
	int x,y;

	

	Bmp.CreateFromBitmap(Screen, "HERO1.BMP");
	Screen->SetPalette(Bmp.Pal, 0, 256);

	Sprite.CreateFromBitmap(Screen, "HERO1.BMP", 3, 4, 16, 24, 1, 255);
	Obj.Sprite = &Sprite;

	DInputMgr.InstallKeyboard();

	VPad[0].AssignUp(DIK_UP, VPAD_NONE, VPAD_NONE);
	VPad[0].AssignDown(DIK_DOWN, VPAD_NONE, VPAD_NONE);
	VPad[0].AssignLeft(DIK_LEFT, VPAD_NONE, VPAD_NONE);
	VPad[0].AssignRight(DIK_RIGHT, VPAD_NONE, VPAD_NONE);
	
	SetStage(&TestStage);
	}



void TESTSTAGE::Opening()
	{
	Screen->GetBack()->SetClip(0, 0, 640, 480);
	}




void TESTSTAGE::Loop()
	{
	// Input 

	if (VPad[0].Left())
		Obj.WalkLeft();
	else if (VPad[0].Right())
		Obj.WalkRight();
	else if (VPad[0].Up())
		Obj.WalkUp();
	else if (VPad[0].Down())
		Obj.WalkDown();
	else
		Obj.Stop();

	
	// Graphics 

	Screen->GetBack()->Fill(2);

	Obj.Update();

	/*for (int ii = 0; ii < 250; ii++)
		{
		Obj.X = rand() % 250;
		}*/


	//for (int i = 0; i < 1000; i++)
	//	{
		//Obj.Draw(Screen->GetBack(), (Obj.Dir * 3) + Obj.Frame, rand() % 640, rand() % 480);
		Obj.Draw(Screen->GetBack(), (Obj.Dir * 3) + Obj.Frame, 0, 0);
	//	}
	}
