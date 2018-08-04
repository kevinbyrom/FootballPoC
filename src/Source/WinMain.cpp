#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "WinMain.h"
#include "Football.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	{
	if (!MainGame.Init(hInstance, WndProc, "FOOTBALL", SCREEN_WIDTH, SCREEN_HEIGHT, 8, FALSE, FPS))
		return 0;

	MainGame.Loop();

	return (0);
	}





LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	switch(msg)
		{
		case WM_CREATE:
			if (MainGame.IsFullScreen())
				ShowCursor(FALSE);
			return 1;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}


	return DefWindowProc(hWnd, msg, wParam, lParam);
	}
