#include "Engine.h"

#pragma once

namespace Soshiant2D {
	class Sequence;
	class TextureLibraryItem
	{
	public :
		std::string name;
		std::string path;
		Texture* content;
	};
	class SoundLibraryItem
	{
	public :
		std::string name;
		std::string path;
		//
	};

	class Library
	{
	private :
		std::list<TextureLibraryItem*> p_libTextures ;
		std::list<SoundLibraryItem*>   p_libSounds   ;

		lua_State* luaState;

		Sequence* p_parent;



		TextureLibraryItem* findTexture(std::string name);
		void LoadFromFile(std::string path);
		void Release();


	public :
		Library(std::string fileName,Sequence* p_parent);
		~Library();
		Texture* getTexture(std::string name);

	};
}