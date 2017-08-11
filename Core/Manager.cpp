#include "Engine.h"

namespace Soshiant2D {	

	Manager::Manager(std::string path)
	{
		this->luaState = lua_open();
		LoadFromFile(path);
		this->_activeSeq = this->rootSeq ;
	}

	void Manager::LoadFromFile(std::string path)
	{
		if(Lua::loadScript(this->luaState , path + "\\manager.man.lua"))
		{
			
			std::string dir = Lua::getLuaGlobalString( this->luaState , "rootSeq" );
			this->_activeSeq = new Sequence( path  +"\\"+ dir + "\\");
			this->rootSeq = this->_activeSeq;
			
		}
		else
		{
			//Error in loading
			g_engine->Message("Error in loading Script File of Manager .\n"+path+ "\\manager.man.lua");
		}
	}

	void Manager::Update()
	{
		rootSeq->Update();
		/*if(clientSeq != NULL)
			clientSeq->Update();*/
	}

	void Manager::Render()
	{
		if(rootSeq->isVisible())
			rootSeq->Render();
		/*if(clientSeq != NULL)
			if(clientSeq->isVisible())
				clientSeq->Render();*/
	}

}