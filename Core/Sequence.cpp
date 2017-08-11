#include "Engine.h"

namespace Soshiant2D {
	
	Sequence::Sequence(std::string path)
	{
		this->p_pauseMode = false;
		this->visible = true ;

		//create audio system
		audio = new Audio();
		audio->Init();

		this->luaState = lua_open();
		//luaL_openlibs(this->luaState);
		//Lua::OpenSohiantLuaLib(this->luaState);
		this->LoadFromFile(path);	
		this->center.setX(g_engine->getScreenWidth()/2);
		this->center.setY(4*g_engine->getScreenHeight()/6);	
	}
	Sequence::~Sequence()
	{
		lua_close(this->luaState);
		this->Release();			
		delete audio;
		
	}
	void Sequence::Release()
	{
		delete this->_library;
		delete this->p_physics;

		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			delete (*i);
			++i;
		} 
	}		

	void Sequence::UpdateLayers()
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			(*i)->UpdateEntities();
			++i;
		} 
	}

	void Sequence::Update()
	{
		if (!this->p_pauseMode)
		{			
			//updating Physics
			this->p_physics->Update();

			//parelax scroling
			ParalaxScroling();

		    //update entities
			this->UpdateLayers();
		}
	}

	void Sequence::Render()
	{
		///////////////////////////////////////
		//Runing OnEnterFrame Lua Functions
		this->itt = this->p_onEnterFrameEventFunctions.begin();
		while (this->itt != p_onEnterFrameEventFunctions.end())
		{
			this->p_activeLayer = (*this->itt)->_parent;
			this->p_activeLayer->_activeSprite = (*this->itt);
			(*this->itt)->OnEnterFrame();
			++this->itt;
		}
		/////////////////////////////////////////
		g_engine->ClearScene(this->backColor);
		/////////////////////////////
		//Drow Entities
		Render2D_Start();

		DrawLayers();

		Render2D_Stop();
		/////////////////////////////
		//Drow Lines
		Render2D_Start();

		g_engine->getLineHandler()->Begin();
		DrowLines();
		g_engine->getLineHandler()->End();

		Render2D_Stop();
	}

	int Sequence::Render2D_Start()
	{
		if (g_engine->getSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
			return 0;
		else
			return 1;
	}
	
	int Sequence::Render2D_Stop()
	{
	   g_engine->getSpriteHandler()->End();
	   return 1;
	}
	
	void Sequence::DrawLayers()
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			(*i)->Draw2DEntities();
			++i;
		} 
	}

	void Sequence::DrowLines()
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			(*i)->DrowLines();
			++i;
		}
	}
		
	Layer *Sequence::findLayer(std::string name)
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

	Layer *Sequence::findLayer(int ID)
	{
		std::list<Layer*>::iterator i = p_layers.begin();
		while (i != p_layers.end())
		{
			//g_engine->Message("Layer Activing .... "+ToString((*i)->getID()));
			if ((*i)->getID() == ID ) 
				return *i;
			else
				++i;
		}
		return NULL;
	}

	int Sequence::addLayer(Layer *layer)
	{
		static int id = 0;
		layer->_parent = this ;
		layer->setID(id);
		p_layers.push_back(layer);
		return id++;
	}

	void Sequence::setActiveLayer(int id)
	{
		if(id>-1)
			this->p_activeLayer = findLayer(id);
		else
			this->p_activeLayer = NULL;
		/*if(this->p_activeLayer )
			g_engine->Message("layer founded"+ToString(this->p_activeLayer->getID()));*/
		
	}

	void Sequence::setFocusLayer(int id)
	{
		if(id>-1)
			this->p_focusLayer = findLayer(id);
		else
			this->p_focusLayer = NULL;
	}

	void Sequence::setPivotLayer(int id)
	{
		if(id>-1)
			this->p_pivotLayer = findLayer(id);
		else
			this->p_pivotLayer = NULL;
	}

	void Sequence::ParalaxScroling()
	{
		if(p_pivotLayer!=NULL && p_focusLayer!=NULL && p_cameraLookAt!= NULL)
		{
			Layer *layer;

			p_focusLayer->setPosition(center.getX()-p_cameraLookAt->getX(),center.getY()-p_cameraLookAt->getY(),p_focusLayer->getPosition().getZ());

			Vector3 position0 = p_pivotLayer->getPosition();
			Vector3 position1 = p_focusLayer->getPosition();

			//testing ...

			

			/*position1.setX(position1.getX()-0.1f);
			p_focusLayer->setPosition(position1);*/

			//position1.setY(position1.getY()+ 0.05f);
			//p_focusLayer->setPosition(position1);

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

	void Sequence::AddKeyDownEventFunction(Sprite* sprite)
	{
		this->p_keyDownEventFunctions.push_back(sprite);
	}

	void Sequence::AddKeyUpEventFunction(Sprite* sprite)
	{
		this->p_keyUpEventFunctions.push_back(sprite);
	}

	void Sequence::AddOnLoadEventFunction(Sprite* sprite)
	{
		this->p_onLoadEventFunctions.push_back(sprite);
	}

	void Sequence::AddOnEnterFrameEventFunction(Sprite* sprite)
	{
		this->p_onEnterFrameEventFunctions.push_back(sprite);
	}

	void Sequence::AddMouseMoveEventFunction(Sprite* sprite)
	{
		this->p_mouseMoveEventFunctions.push_back(sprite);
	}

	void Sequence::KeyDown(int e)
	{
		this->itt = p_keyDownEventFunctions.begin();
		while (this->itt != p_keyDownEventFunctions.end())
		{
			this->p_activeLayer = (*this->itt)->_parent;
			this->p_activeLayer->_activeSprite = (*this->itt);
			(*this->itt)->OnKeyDown(e);
			++this->itt;
		}
	}

	void Sequence::KeyUp(int e)
	{
		this->itt = p_keyUpEventFunctions.begin();
		while (this->itt != p_keyUpEventFunctions.end())
		{
			this->p_activeLayer = (*this->itt)->_parent;
			this->p_activeLayer->_activeSprite = (*this->itt);
			(*this->itt)->OnKeyUp(e);
			++this->itt;
		}
	}

	void Sequence::OnLoad()
	{
		this->itt = p_onLoadEventFunctions.begin();
		while (this->itt != p_onLoadEventFunctions.end())
		{
			this->p_activeLayer = (*this->itt)->_parent;
			this->p_activeLayer->_activeSprite = (*this->itt);
			(*this->itt)->OnLoad();
			++this->itt;
		}
	}

	void Sequence::MouseMove(int x,int y)
	{
		this->itt = p_mouseMoveEventFunctions.begin();
		while (this->itt != p_mouseMoveEventFunctions.end())
		{
			this->p_activeLayer = (*this->itt)->_parent;
			this->p_activeLayer->_activeSprite = (*this->itt);
			(*this->itt)->OnMouseMove(x,y);
			++this->itt;
		}
	}

	void Sequence::LoadFromFile(std::string path)
	{
		std::string lyr_ext = ".lyr.lua";
		if(Lua::loadScript(this->luaState , path + "seq.seq.lua"))
		{
			this->name = Lua::getLuaGlobalString( this->luaState , "name" );

			int r = (int)Lua::getLuaGlobalNumber( this->luaState , "backColor_r" );
			int g = (int)Lua::getLuaGlobalNumber( this->luaState , "backColor_g" );
			int b = (int)Lua::getLuaGlobalNumber( this->luaState , "backColor_b" );
			this->backColor = D3DCOLOR_XRGB(r ,g ,b );

			this->isPhysical = Lua::getLuaGlobalBoolean(this->luaState , "isPhysical") ;
			if(this->isPhysical)
				this->p_physics = new Physics( 5000 );
			std::string libPath = Lua::getLuaGlobalString( this->luaState , "library" );
			this->_library = new Library( path + libPath ,this);
			

			//loading Layers
			int layersCount = (int)Lua::getLuaGlobalNumber( this->luaState , "layersCount" );
			for(int i=0 ; i<layersCount ; i++)
			{	
				this->p_activeLayer = new Layer(path +"layer_"+ToString(i)+lyr_ext,this);
				this->addLayer(this->p_activeLayer);				

			}

			this->p_pivotLayer = this->findLayer(Lua::getLuaGlobalString( this->luaState , "pivotLayer" ));
			this->p_focusLayer = this->findLayer(Lua::getLuaGlobalString( this->luaState , "focusLayer" ));
			if(this->p_focusLayer!=NULL)
				this->p_cameraLookAt  = (Sprite*)this->p_focusLayer->findEntity(Lua::getLuaGlobalString( this->luaState , "cameraLookAt" ));

		}
		else
		{
			//Error in loading
			g_engine->Message("Error in loading Script File of Sequence .\n"+path+ "\\seq.seq.lua");
		}
	}

}