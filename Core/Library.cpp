#include "Engine.h"

namespace Soshiant2D{

	Library::Library(std::string fileName,Sequence* p_parent)
	{
		this->p_parent = p_parent;
		this->luaState = lua_open();
		//luaL_openlibs(this->luaState);

		this->LoadFromFile(fileName);
	}

	Library::~Library()
	{
		lua_close(this->luaState);
		Release();
	}

	void Library::LoadFromFile(std::string path)
	{
		if(Lua::loadScript(this->luaState,path))
		{
			//loading Textures
			int texturesCount = (int)Lua::getLuaGlobalNumber( this->luaState , "texturesCount" );
			for( int i=0; i<texturesCount; i++ )
			{
				TextureLibraryItem* item = new TextureLibraryItem();
				item->name = Lua::getLuaGlobalString( this->luaState , "texture" + ToString(i) + "_name" );
				item->path = Lua::getLuaGlobalString( this->luaState , "texture" + ToString(i) + "_path" );
				item->content = new Texture();
				if(!item->content->Load(Path::getStartupPath()+"\\"+item->path))
				{
					//Error in Loading texture
				}
				p_libTextures.push_back(item);
			}
			//loading Sounds
			int soundsCount = (int)Lua::getLuaGlobalNumber( this->luaState , "soundsCount" );
			for( int i=0; i<soundsCount; i++ )
			{
				std::string name = Lua::getLuaGlobalString( this->luaState , "sound" + ToString(i) + "_name" );

				std::string path = Lua::getLuaGlobalString( this->luaState , "sound" + ToString(i) + "_path" );
				if(!this->p_parent->audio->Load(Path::getStartupPath()+"\\"+path,name))
				{
					//Error in Loading Sound
				}
			}
		}
		else
		{
			//Error in loading
		}
	}

	void Library::Release()
	{
		std::list<TextureLibraryItem*>::iterator itt = p_libTextures.begin();
		while(itt != p_libTextures.end())
		{
			delete (*itt)->content;
			delete (*itt);
		}
	}

	Texture* Library::getTexture(std::string name)
	{
		return this->findTexture(name)->content;
	}

	TextureLibraryItem* Library::findTexture(std::string name)
	{
		std::list<TextureLibraryItem*>::iterator itt = p_libTextures.begin();
		while(itt != p_libTextures.end())
		{
			
			if((*itt)->name == name)
				return (*itt);
			itt++;
		}
		return NULL;		
	}
}