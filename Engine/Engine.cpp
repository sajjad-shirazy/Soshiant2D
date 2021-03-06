#include "Engine.h"

namespace Soshiant2D
{
	Engine::Engine()
	{
		p_windowHandle = 0;
		
		//set default values
		this->setScreenWidth(800);
		this->setScreenHeight(600);
		this->setColorDepth(16);
		this->setFullscreen(false);
		
		//window handle must be set later on for DirectX!
		this->setWindowHandle(0);

		this->SetBackColor(D3DCOLOR_XRGB(0,0,0));
	}

	Engine::~Engine()
	{
		if (this->p_device)         this->p_device->Release();
		if (this->p_d3d)            this->p_d3d->Release();
		if (this->p_physics)        this->p_physics->Release();
		if (this->p_offscreanSurf)  this->p_offscreanSurf->Release();
	}

	int Engine::Init(int width, int height, int colordepth, bool fullscreen)
	{
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
		d3dpp.Windowed = (!fullscreen);
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;

		if(colordepth==32)
			d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
		else
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.hDeviceWindow = p_windowHandle;
	
		//create Direct3D device
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
	
		//clear the backbuffer to black
		this->ClearScene(D3DCOLOR_XRGB(0,0,0));
	
		//create pointer to the back buffer
		this->p_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->p_backbuffer);
		this->p_device->CreateOffscreenPlainSurface(100,100,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&this->p_offscreanSurf,NULL);
	
	    //use ambient lighting and z-buffering
		//this->p_device->SetRenderState(D3DRS_ZENABLE, TRUE);
		this->p_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		this->p_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		this->p_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);

        //this->p_device->SetRenderState(D3DRS_LIGHTING, true);
		//this->SetAmbient(this->p_ambientColor);
	
		//initialize 2D renderer
		HRESULT result = D3DXCreateSprite(this->p_device, &this->p_sprite_handler);
		if (result != D3D_OK) return 0;

		D3DXCreateLine(this->p_device, &this->p_line_handler);

        //initialize directinput	
		p_input = new Input(this->getWindowHandle());

		//create audio system
		//audio = new Audio();
		//if (!audio->Init())	return 0;

        //initialize math library
		//math = new Math();

		//call game initialization extern function
		//if (!game_init(this->getWindowHandle()))
		//return 0;
	
		//set a default material
		//SetDefaultMaterial();

		//initializing Font
		D3DXCreateFont( this->p_device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &this->p_font_handler );
		this->consol_text = "Soshiant 2d Engine" ;


		//Initing Physics
		p_physics = new Physics( 500 );

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

	void Engine::Update()
	{
		//calculate core framerate
		p_frameCount_core++;
		if (p_coreTimer.stopwatch(999)) {
			p_frameRate_core = p_frameCount_core;
			p_frameCount_core = 0;
		}
	
		//fast update with no timing
		//game_update();

		if (!p_pauseMode)
		{
			
			//updating Physics
			p_physics->Update();

			//ParalaxScroling();
		    //update entities
			UpdateLayers();
		}


		//perform global collision testing at 20 Hz
		//if (!p_pauseMode && collisionTimer.stopwatch(50)) 
		//{
		//	TestForCollisions();
		//}

		//update with 60fps timing
		if (!timedUpdate.stopwatch(14)) {
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
			//if (!p_pauseMode)
				this->ClearScene(this->backColor);
 
            //call game 3d rendering
           // game_render3d();

			//render 3D entities 
			//if (!p_pauseMode) Draw3DEntities();

            //begin 2d rendering
			Render2D_Start();

			//render 2D entities 
			//if (!p_pauseMode)          //Editor() :  forselecting shapes this is changed
			DrawLayers();
			

            //let game do 2d rendering
			//game_render2d();

            //done with 2d rendering
			Render2D_Stop();	

			Render2D_Start();
			this->p_line_handler->Begin();
			DrowLines();
			this->p_line_handler->End();
			Render2D_Stop();

			//text rendering
			D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,255,0);
			RECT rct;
			rct.left=2;
			rct.right=780;
			rct.top=10;
			rct.bottom=rct.top+20;
			p_font_handler->DrawText(NULL, consol_text.c_str(), -1, &rct, 0, fontColor );

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

	int Engine::Render2D_Start()
	{
		if (p_sprite_handler->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
			return 0;
		else
			return 1;
	}
	
	int Engine::Render2D_Stop()
	{
	   p_sprite_handler->End();
	   return 1;
	}

	void Engine::UpdateLayers()
	{
		Layer *layer;
	
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			layer = *i;
			layer->UpdateEntities();

			++i;
		} 
	}
    /*  Modified from original.
        Now, when invisible, game_entityRender is still called.
    */
	void Engine::DrawLayers()
	{
		Layer *layer;
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			//temporary pointer
			layer = *i;
			layer->Draw2DEntities();
			++i;
		} 
	}

	void Engine::DrowLines()
	{
		Layer *layer;
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			//temporary pointer
			layer = *i;
			layer->DrowLines();
			++i;
		}
	}
		
	Layer *Engine::findLayer(std::string name)
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			if ((*i)->getName() == name ) 
				return *i;
			else
				++i;
		}
		
		return NULL;
	}

	Layer *Engine::findLayer_byID(int ID)
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			if ((*i)->getID() == ID ) 
				return *i;
			else
				++i;
		}
		return NULL;
	}

	Sprite* Engine::findSprite(std::string name)
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		Sprite *output;
		while (i != p_layers.end())
		{
			output = (Sprite*)(*i)->findEntity(name);
			if (output !=NULL)
				return output;
			else
				++i;
		}
		return NULL;
	}
	Sprite* Engine::findSprite(int sid,int lid)
	{
		return (Sprite*)this->findLayer_byID(lid)->findEntity_byID(sid);
	}

	void Engine::addEntity(Entity *entity)
	{
		if(p_activeLayer!=NULL)
			p_activeLayer->addEntity(entity);
		else
			MessageBox(0,"Engine : There are no Active Layer to Adding Sprite !",0,0);
	}

	int Engine::addLayer(std::string name ,Vector3 position)
	{
		static int id = 0;
		p_layers.push_back(new Layer(id,name,position));

		return id++;

	}

	void Engine::setActiveLayer(int id)
	{
		if(id>-1)
			this->p_activeLayer = findLayer_byID(id);
		else
			this->p_activeLayer = NULL;
	}

	void Engine::setFocusLayer(int id)
	{
		if(id>-1)
			this->p_focusLayer = findLayer_byID(id);
		else
			this->p_focusLayer = NULL;
	}

	void Engine::setPivotLayer(int id)
	{
		if(id>-1)
			this->p_pivotLayer = findLayer_byID(id);
		else
			this->p_pivotLayer = NULL;
	}

	void Engine::ParalaxScroling()
	{
		if(p_pivotLayer && p_focusLayer)
		{
			Layer *layer;

			Vector3 position0 = p_pivotLayer->getPosition();
			Vector3 position1 = p_focusLayer->getPosition();

			//testing ...

			/*position1.setX(position1.getX()-0.1f);
			p_focusLayer->setPosition(position1);*/

			/*position1.setY(position1.getY()+ 0.05f);
			p_focusLayer->setPosition(position1);*/

			//end of testing ...

			double angleX = atan((position1.getX()- position0.getX())/(position1.getZ()- position0.getZ()));
			double angleY = atan((position1.getY()- position0.getY())/(position1.getZ()- position0.getZ()));

			std::list<Layer*>::iterator i = p_layers.begin();
			while (i != p_layers.end())
			{
				layer = *i;
				position1 = layer->getPosition();
				position1.setX(tan(angleX)*(position1.getZ()- position0.getZ()));
				position1.setY(tan(angleY)*(position1.getZ()- position0.getZ()));
				layer->setPosition(position1);
				++i;
			}
		}
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
			if (p_input->GetMouseButton(n))
				;//game_mouseButton(n);
		}
	
		//check mouse position
		if (p_input->GetPosX() != oldPosX || p_input->GetPosY() != oldPosY) {
			;//game_mouseMove(p_input->GetPosX(), p_input->GetPosY() );
			oldPosX = p_input->GetPosX();
			oldPosY = p_input->GetPosY();
		}
	
		//check mouse motion
		if (deltax != 0 || deltay ) {
			;//game_mouseMotion(deltax,deltay);
		}
	
		//check mouse wheel
		int wheel = p_input->GetDeltaWheel();
		if (wheel != 0) 
			;//game_mouseWheel(wheel);
	
	}
	
	void Engine::UpdateKeyboard()
	{
		static char old_keys[256];
		static Sprite* sprite;
	
		for (int n=0; n<255; n++) {
			//check for key press
			if (p_input->GetKeyState(n) & 0x80) {
				;//game_keyPress(n);
				//this->Message(ToString(n)+"Hello !");
				if(n==200)
				{					
					if(sprite!=NULL)
					{
						cpBodyApplyImpulse(sprite->getBody(), cpv(0,-1000),cpv(0,0));
					}
					else
						sprite = this->findSprite("player");

				}
				if(n==203)
				{
					if(sprite!=NULL)
					{
						cpBodyApplyImpulse(sprite->getBody(), cpv(-1000,0),cpv(0,0));
					}
					else
						sprite = this->findSprite("player");
				}
				if(n==205)
				{
					if(sprite!=NULL)
					{
						cpBodyApplyImpulse(sprite->getBody(), cpv(1000,0),cpv(0,0));
					}
					else
						sprite = this->findSprite("player");
				}
				old_keys[n] = p_input->GetKeyState(n);
			} 
			//check for release
			else if (old_keys[n] & 0x80) {
				;//game_keyRelease(n);
				old_keys[n] = p_input->GetKeyState(n);
			}
		}
	}
}