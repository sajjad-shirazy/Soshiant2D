#include "Engine.h"

#pragma once


namespace Soshiant2D {	
	
	class Manager
	{
	private :
		lua_State* luaState;
		Sequence*  rootSeq;
		Sequence*  clientSeq;

		void LoadFromFile(std::string path);



	public :
		Sequence*  _activeSeq ;

		Manager(std::string path);
		void Update();
		void Render();
	};
};