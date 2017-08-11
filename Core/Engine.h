// Engine
// Main header file

#pragma once

#pragma once

#define WIN32_EXTRA_LEAN
#include <windows.h>

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>


#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <dinput.h>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
//macro to read the keyboard asynchronously
#define KEY_DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000)?1:0)



//Physics Engine
#include "chipmunk.h"

//Script Engine
#include "lua.hpp"
#include "LuaTools.h"

//Fmod Audio Engine
#include "fmod.hpp"

#include "Helpers.h"

#include "Input.h"
#include "Timer.h"
#include "Vector3.h"
#include "Rect.h"
#include "Physics.h"
#include "Window.h"
#include "Texture.h"
#include "Library.h"
#include "Entity.h"
#include "Audio.h"

#include "Shape.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "Layer.h"
#include "Sequence.h"
#include "Manager.h"


extern bool gameover;



namespace Soshiant2D
{
	class Engine
	{
		HWND               p_windowHandle;
		LPDIRECT3D9        p_d3d;
		LPDIRECT3DDEVICE9  p_device;
		LPDIRECT3DSURFACE9 p_backbuffer;
		LPD3DXSPRITE       p_sprite_handler;
		LPD3DXLINE         p_line_handler;
		LPD3DXFONT         p_font_handler;


		
		bool p_fullscreen;
		int p_screenwidth;
		int p_screenheight;
		int p_colordepth;
		std::string manager_path;

		
		bool p_maximizeProcessor;
		Timer p_coreTimer;
		long p_frameCount_core;
		long p_frameRate_core;
		Timer p_realTimer;
		long p_frameCount_real;
		long p_frameRate_real;
		Timer timedUpdate;	
		int	updateTimer;	
			

		Input *p_input;

		void UpdateKeyboard();
		void UpdateMouse();



	public:	
		std::string        consol_text;
		Manager*           _manager;		
		lua_State*		   luaState;
		int                fps;



		Engine(std::string path);
		Engine(int width,int height,int depth,bool fullscreen,std::string manager_path);
		virtual ~Engine();
		int Init(HWND hwnd);
		void LoadFromFile(std::string path);
		void ResizeDevice(HWND windowHandle,int width, int height, int colordepth, bool fullscreen);
		int ResetDirect3D();
		void CleanUp();
		void Update();
		void ClearScene(D3DCOLOR color);		
		int RenderStart();
		int RenderStop();
		void Message(std::string text){ MessageBox(this->p_windowHandle,text.c_str(),"Engine Message ...",0);}

		LPDIRECT3DDEVICE9  getDevice()        { return this->p_device; }
		LPDIRECT3DSURFACE9 getBackBuffer()    { return this->p_backbuffer; }
		LPD3DXSPRITE       getSpriteHandler() { return this->p_sprite_handler; }
		LPD3DXLINE         getLineHandler()   { return this->p_line_handler; }
		LPD3DXFONT         getFontHandler()   { return this->p_font_handler; }

		void setWindowHandle(HWND hwnd) { this->p_windowHandle = hwnd; }
		HWND getWindowHandle() { return this->p_windowHandle; }

		long getFrameRate_core() { return this->p_frameRate_core; };
		long getFrameRate_real() { return this->p_frameRate_real; };

		int getScreenWidth() { return this->p_screenwidth; }
		void setScreenWidth(int value) { this->p_screenwidth = value; }

		int getScreenHeight() { return this->p_screenheight; }
		void setScreenHeight(int value) { this->p_screenheight = value; }

		int getColorDepth() { return this->p_colordepth; }
		void setColorDepth(int value) { this->p_colordepth = value; }

		bool getFullscreen() { return this->p_fullscreen; }
		void setFullscreen(bool value) { this->p_fullscreen = value; }

		bool getMaximizeProcessor() { return this->p_maximizeProcessor; }
		void setMaximizeProcessor(bool value) { this->p_maximizeProcessor = value; }
	};
}
extern Soshiant2D::Engine *g_engine;
extern double editor_x_base;
extern double editor_y_base;
extern double edirot_scale_base;