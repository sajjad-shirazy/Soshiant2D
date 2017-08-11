#include "Engine.h"

#pragma once

namespace Soshiant2D
{
	namespace Lua
	{
		bool			loadScript(lua_State* from,std::string scriptfile);
		std::string		getLuaGlobalString(lua_State* from,std::string varName);
		double			getLuaGlobalNumber(lua_State* from,std::string varName);
		bool			getLuaGlobalBoolean(lua_State* from,std::string varName);
		
		bool			luaFunctionExist(lua_State* from,std::string varName);


		//Soshiant Lua Libs
		static int ApplyImplus( lua_State *L );
		static int ApplyForce( lua_State *L );
		

		static int PrintConsol( lua_State *L );
		//Sprite Position and Rotaion and Scale
		static int SetX( lua_State *L );
		static int GetX( lua_State *L );
		static int SetY( lua_State *L );
		static int GetY( lua_State *L );
		static int SetXScale( lua_State *L );
		static int GetXScale( lua_State *L );
		static int SetYScale( lua_State *L );
		static int GetYScale( lua_State *L );
		static int SetRotation( lua_State *L );
		static int GetRotation( lua_State *L );
		//Sprite Physicals
		static int GetVX( lua_State *L );
		static int GetVY( lua_State *L );
		//Sprite Animation
		static int Stop( lua_State *L );
		static int Play( lua_State *L );
		static int GotoAndStop( lua_State *L );
		static int GotoAndPlay( lua_State *L );
		//Sound Playing
		static int PlaySound( lua_State *L );


		//register soshiant lib
		void OpenSohiantLuaLib(lua_State *L);
	}
}