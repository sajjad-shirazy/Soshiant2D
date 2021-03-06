#include "Engine.h"


using namespace Soshiant2D::Lua;

namespace Soshiant2D
{
	Engine::Engine(std::string path)
	{
		this->fps = 64;
		this->updateTimer = 14;

		this->luaState = lua_open();
		LoadFromFile(path);
		
		//window handle must be set later on for DirectX!
		this->setWindowHandle(0);		
	}
	Engine::Engine(int width,int height,int depth,bool fullscreen,std::string manager_path)
	{
		this->p_fullscreen = fullscreen;
		this->p_screenwidth = width;
		this->p_screenheight = height;
		this->p_colordepth = depth;
		this->manager_path = manager_path;

		//window handle must be set later on for DirectX!
		this->setWindowHandle(0);	
	}

	void Engine::LoadFromFile(std::string path)
	{
		if(Lua::loadScript(this->luaState , path ))
		{
			this->p_fullscreen = Lua::getLuaGlobalBoolean(this->luaState,"fullscreen");
			this->p_screenwidth = Lua::getLuaGlobalNumber(this->luaState,"screenWidth");
			this->p_screenheight = Lua::getLuaGlobalNumber(this->luaState,"screenHeight");
			this->p_colordepth = Lua::getLuaGlobalNumber(this->luaState,"colorDepth");
			this->manager_path = Lua::getLuaGlobalString( this->luaState , "managerPath" );
		}
		else
		{
			//Error in loading
		}
	}

	Engine::~Engine()
	{
		if (this->p_device)         this->p_device->Release();
		if (this->p_d3d)            this->p_d3d->Release();	
	}

	int Engine::Init(HWND hwnd)
	{
		p_windowHandle = hwnd;
	   //initialize Direct3D

		this->p_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (this->p_d3d == NULL) {
			MessageBox(0,"error in p_d3d","",0);
			return 0;
		}
	
		//get system desktop color depth
		D3DDISPLAYMODE dm;
		this->p_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
	
		//set configuration options for Direct3D
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = (!p_fullscreen);
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;

		if(p_colordepth==32)
			d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
		else
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = p_screenwidth;
		d3dpp.BackBufferHeight = p_screenheight;
		d3dpp.hDeviceWindow = p_windowHandle;
	
		//create Direct3D device
		this->p_d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL     ,
			this->p_windowHandle,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&this->p_device);
	
		if (this->p_device == NULL) 
		{
			MessageBox(this->p_windowHandle,"error in p_device","",0);
			return 0;
		}
	
		//clear the backbuffer to black
		this->p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	
		//create pointer to the back buffer
		this->p_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->p_backbuffer);
	
	    //use ambient lighting and z-buffering
		//this->p_device->SetRenderState(D3DRS_ZENABLE, TRUE);
		this->p_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        //this->p_device->SetRenderState(D3DRS_LIGHTING, true);
		//this->SetAmbient(this->p_ambientColor);
	
		//initialize 2D renderer
		HRESULT result = D3DXCreateSprite(this->p_device, &this->p_sprite_handler);
		if (result != D3D_OK) return 0;

		D3DXCreateLine(this->p_device, &this->p_line_handler);

        //initialize directinput	
		p_input = new Input(this->getWindowHandle());

        //initialize math library
		//math = new Math();

		//call game initialization extern function
		//if (!game_init(this->getWindowHandle()))
		//return 0;
	
		//set a default material
		//SetDefaultMaterial();

		//initializing Font
		D3DXCreateFont( this->p_device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &this->p_font_handler );

		
		this->consol_text = "@ Soshiant 2D Engine 1.0.0";
		_manager = new Manager(Path::getStartupPath() + this->manager_path);				

		//Sequence are Loaded So call All Sprites Onload()
		this->_manager->_activeSeq->OnLoad();

		//,"D:\\Sajjad\\Flash\\web site 2\\22738.swf"
		//inting Editor Items		
	
		return 1;
	}

	void Engine::ResizeDevice(HWND windowHandle,int width, int height, int colordepth, bool fullscreen)
	{
		D3DFORMAT rd;
		if(colordepth==32)
			rd=D3DFMT_D32;
		else
			rd=D3DFMT_D16;
		//get system desktop color depth
		D3DPRESENT_PARAMETERS d3dpp=					// d3dpp Tells Windows How We Want Things To Be
		{
			width,										// Back Buffer Width		
			height,										// Back Buffer Height
			D3DFMT_R5G6B5,								// Back Buffer Format (Color Depth)
			1,											// Back Buffer Count (Double Buffer)
			D3DMULTISAMPLE_NONE,						// No Multi Sample Type
			0,											// No Multi Sample Quality
			D3DSWAPEFFECT_DISCARD,						// Swap Effect (Fast)
			windowHandle,										// The Window Handle
			!fullscreen,								// Windowed or Fullscreen
			TRUE,										// Enable Auto Depth Stencil  
			rd,									// 16Bit Z-Buffer (Depth Buffer)
			0,											// No Flags
			D3DPRESENT_RATE_DEFAULT,					// Default Refresh Rate
			D3DPRESENT_INTERVAL_DEFAULT					// Default Presentation Interval (vertical sync)
		};

		if(this->p_device->Reset(&d3dpp)==D3DERR_INVALIDCALL)
			MessageBox(0,"Error in Reseting ... ","",0);
	}

	int Engine::ResetDirect3D()
	{
		CleanUp();
		////////////////////////////////////////////////
		//initialize Direct3D
		this->p_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (this->p_d3d == NULL) {
			MessageBox(0,"error in p_d3d","",0);
			return 0;
		}
		D3DDISPLAYMODE dm;
		this->p_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = (!g_engine->getFullscreen());
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		if(g_engine->getColorDepth()==32)
			d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
		else
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = g_engine->getScreenWidth();
		d3dpp.BackBufferHeight = g_engine->getScreenHeight();
		d3dpp.hDeviceWindow = p_windowHandle;
		this->p_d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			this->p_windowHandle,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&this->p_device);	
		if (this->p_device == NULL) 
		{
			MessageBox(this->p_windowHandle,"error in p_device","",0);
			return 0;
		}
		this->p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		this->p_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->p_backbuffer);
		this->p_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		HRESULT result = D3DXCreateSprite(this->p_device, &this->p_sprite_handler);
		if (result != D3D_OK) return 0;
		D3DXCreateLine(this->p_device, &this->p_line_handler);
		//////////////////////////////////////////////////////////////////////////

	}
	void Engine::CleanUp()
	{
		if (this->p_backbuffer)
			this->p_backbuffer->Release();
		if (this->p_device) 
			this->p_device->Release();
		if (this->p_d3d)
			this->p_d3d->Release();
	}

	void Engine::Update()
	{
		//calculate core framerate
		p_frameCount_core++;
		if (p_coreTimer.stopwatch(999)) {
			p_frameRate_core = p_frameCount_core;
			p_frameCount_core = 0;
		}

		this->_manager->Update();

		//update with 60fps timing
		if (!timedUpdate.stopwatch(this->updateTimer)) {
			if (!this->getMaximizeProcessor()) 
			{
				Sleep(1);
			}
		}
		else {
			//calculate real framerate
			p_frameCount_real++; 
			if (p_realTimer.stopwatch(999)) {
				p_frameRate_real = p_frameCount_real;
				p_frameCount_real = 0;
			}
	
			//update input devices
			p_input->Update();
			this->UpdateKeyboard();
			this->UpdateMouse();

			//update audio system
			//audio->Update();

			//begin rendering
			this->RenderStart();
			
			this->_manager->Render();

			D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,255,0);
			RECT rct;
			rct.left=2;
			rct.right=780;
			rct.top=10;
			rct.bottom=rct.top+20;
			p_font_handler->DrawText(NULL, this->consol_text.c_str(), -1, &rct, 0, fontColor );

			//done rendering
			this->RenderStop();
		}
	}

	void Engine::ClearScene(D3DCOLOR color)
	{
		this->p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
	}

	int Engine::RenderStart()
	{
		HRESULT hResult = this->p_device->TestCooperativeLevel();
		if (hResult == D3DERR_DEVICELOST)
		{
			//g_engine->Message("LOsted");
			this->_manager->_activeSeq->setPaused(true);
		}
		else if (hResult == D3DERR_DEVICENOTRESET)
		{
			ResetDirect3D();
			this->_manager->_activeSeq->setPaused(true);
		}
		if (!this->p_device) return 0;
		if (this->p_device->BeginScene() != D3D_OK) return 0;
		return 1;
	}
	
	int Engine::RenderStop()
	{
	   if (!this->p_device) return 0;
	   if (this->p_device->EndScene() != D3D_OK) return 0;
	   if (p_device->Present(NULL, NULL, NULL, NULL) != D3D_OK) return 0;
	   return 1;
	}

	void Engine::UpdateMouse()
	{
		static int oldPosX = 0;
		static int oldPosY = 0;
		int deltax = p_input->GetDeltaX();
		int deltay = p_input->GetDeltaY();
	
		//check mouse buttons
		for (int n=0; n<4; n++) 
		{
			if (p_input->GetMouseButton(n));
				//game_mouseButton(n);
		}
	
		//check mouse position
		if (p_input->GetPosX() != oldPosX || p_input->GetPosY() != oldPosY) {
			//game_mouseMove(p_input->GetPosX(), p_input->GetPosY() );
			if(this->_manager->_activeSeq!=NULL)
				this->_manager->_activeSeq->MouseMove(p_input->GetPosX(), p_input->GetPosY());
			oldPosX = p_input->GetPosX();
			oldPosY = p_input->GetPosY();
		}
	
		//check mouse motion
		if (deltax != 0 || deltay ) {
			//game_mouseMotion(deltax,deltay);
		}
	
		//check mouse wheel
		int wheel = p_input->GetDeltaWheel();
		if (wheel != 0);
			//game_mouseWheel(wheel);	
	}
	
	void Engine::UpdateKeyboard()
	{
		static char old_keys[256];

		for (int n=0; n<255; n++) {
			//check for key press
			if (p_input->GetKeyState(n) & 0x80) {
				//
				//
				if(this->_manager->_activeSeq!=NULL)
					this->_manager->_activeSeq->KeyDown(n);
				///
				old_keys[n] = p_input->GetKeyState(n);
			} 
			//check for release
			else if (old_keys[n] & 0x80) {
				
				//
				///
				///
				if(this->_manager->_activeSeq!=NULL)
					this->_manager->_activeSeq->KeyUp(n);
				///
				///
				old_keys[n] = p_input->GetKeyState(n);
			}
		}
	}
}
while(true)
{
	//do some things
	Sleep(1);
}