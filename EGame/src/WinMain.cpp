
#include "EGraphics.h"
#include "EWindow.h"
#include "EMeshUtil.h"
#include "EUtil.h"
#include "EGame.h"
//#include "../res/resource.h"

using namespace E3D;

int WINAPI WinMain(
				   HINSTANCE hInstance,      
				   HINSTANCE hPrevInstance,  
				   LPSTR lpCmdLine,         
				   int nCmdShow              
				   )
{	
	InitLog("E3D.Log");

	Log("Init Graphics...");
	EGraphics::initGraphics(hInstance);
	Log("Graphics Load Successed!");

	EGameWindow::GWindow = new EGameWindow("E3D [3DTankWar]", hInstance);
	EGameWindow::GWindow->showWindow(true);
	HWND hwnd = EGameWindow::GWindow->getHWnd();
	EGame *game = new EGame();
	EGameWindow::GWindow->setCanvasListener(game);
	EGameWindow::GWindow->addInputListener(game);
	EGameWindow::GWindow->startLoop();
	Log("Shutdown Graphics...");
	EGraphics::shutdownGraphics();

	CloseLog();

	return 0;
}