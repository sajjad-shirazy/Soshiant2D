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

//..

#include "Helpers.h"

#include "Input.h"
#include "Timer.h"
#include "Vector3.h"
#include "Rect.h"

#include "Physics.h"

#include "Entity.h"
#include "Layer.h"
#include "Shape.h"
#include "Texture.h"
#include "Sprite.h"
#include "ParticleSystem.h"

#include "Windows.h"


extern bool gameover;



namespace Soshiant2D
{
	class Engine
	{
		HWND               p_windowHandle;
		LPDIRECT3D9        p_d3d;
		LPDIRECT3DDEVICE9  p_device;
		LPDIRECT3DSURFACE9 p_backbuffer;
		LPDIRECT3DSURFACE9 p_offscreanSurf;
		LPD3DXSPRITE       p_sprite_handler;
		LPD3DXLINE         p_line_handler;
		LPD3DXFONT         p_font_handler;
		//Physics
		Physics*           p_physics;

		D3DCOLOR backColor;
		
		bool p_fullscreen;
		int p_screenwidth;
		int p_screenheight;
		int p_colordepth;

		bool p_pauseMode;
		bool p_maximizeProcessor;
		Timer p_coreTimer;
		long p_frameCount_core;
		long p_frameRate_core;
		Timer p_realTimer;
		long p_frameCount_real;
		long p_frameRate_real;
		Timer timedUpdate;
		
		std::list<Layer*> p_layers;

		//Onley For Editing
		Layer* p_activeLayer;

		//for Rendering
		Layer* p_focusLayer;
		Layer* p_pivotLayer;

		Input *p_input;
		void UpdateKeyboard();
		void UpdateMouse();


		void UpdateLayers();
		void DrawLayers();
		void DrowLines();

		void ParalaxScroling();



	public:
		
		std::string consol_text;

		Engine();
		virtual ~Engine();
		int Init(int width, int height, int colordepth, bool fullscreen);
		void ResizeDevice(HWND windowHandle,int width, int height, int colordepth, bool fullscreen);
		void Update();
		void ClearScene(D3DCOLOR color);		
		int RenderStart();
		int RenderStop();
		int Render2D_Start();
		int Render2D_Stop();
		void Message(std::string text){ MessageBox(this->p_windowHandle,text.c_str(),"Engine Message ...",0);}

		LPDIRECT3DDEVICE9  getDevice()        { return this->p_device; }
		LPDIRECT3DSURFACE9 getBackBuffer()    { return this->p_backbuffer; }
		LPDIRECT3DSURFACE9 getOffscreanSurf() { return this->p_offscreanSurf; }
		LPD3DXSPRITE       getSpriteHandler() { return this->p_sprite_handler; }
		LPD3DXLINE         getLineHandler()   { return this->p_line_handler; }
		Physics*		   getPhysicsHandler(){ return this->p_physics; }

		//accessor/mutator functions expose the private variables
		bool isPaused() { return this->p_pauseMode; }
		void setPaused(bool value) { this->p_pauseMode = value; }

		void SetBackColor(D3DCOLOR color){ this->backColor=color; }

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

		std::list<Layer*> getLayersList() { return p_layers; }
		long getLayersCount() { return (long)p_layers.size(); }
		void addEntity(Entity *entity);
		Layer *findLayer(std::string name);
		Layer *findLayer_byID(int ID);

		Sprite* findSprite(std::string name);
		Sprite* findSprite(int sid,int lid);

		int addLayer(std::string name ,Vector3 position);

		void setActiveLayer(int id);
		Layer* getAvtiveLayer(){ return p_activeLayer; }

		void setFocusLayer(int id);
		Layer* getFocusLayer(){ return p_focusLayer; }

		void setPivotLayer(int id);
		Layer* getPivotLayer(){ return p_pivotLayer; }

		Layer* getZeroLayer(){std::list<Layer*>::iterator k = p_layers.begin();return (*k);}
	};
}
extern Soshiant2D::Engine *g_engine;
extern double editor_x_base;
extern double editor_y_base;
extern double edirot_scale_base;

