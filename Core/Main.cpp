#include "Engine.h"
using namespace Soshiant2D;

Engine *g_engine;
double editor_x_base = 0;
double editor_y_base = 0;
double edirot_scale_base = 0;

bool gameover = false;
std::string consol_text = "Salam ...";

int InitGame();
Window* main;



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//Seting Engine Engine From Lua File
	g_engine=new Engine(Path::getFullPath("engine.lua"));

	//Initing new Window
	main = new Window("Allah",
		g_engine->getScreenWidth(),
		g_engine->getScreenHeight(),
		g_engine->getColorDepth(),
		g_engine->getFullscreen());

	main->Init(hInstance,nCmdShow);
	main->Show();
	main->setCursorHide(true);
	
	//Initing Engine On new Window
	g_engine->Init(main->getHandle());

	//Initing Game
	InitGame();
	g_engine->_manager->_activeSeq->setPaused(false);

	int i=0;
	while(!gameover)
	{
		main->ProcessMessages();

		g_engine->Update();
		if(i++>100)
		{
			consol_text = "Core FPS = "+ToString(g_engine->getFrameRate_core())+"  | Real FPS = "+ToString(g_engine->getFrameRate_real());
			main->setTitle("Core FPS = "+ToString(g_engine->getFrameRate_core())+"  | Real FPS = "+ToString(g_engine->getFrameRate_real()));
			i=0;
		}
	}

	delete g_engine;
	delete main;
	return 0;
}

int InitGame()
{
	 
	
	return 1;
}
