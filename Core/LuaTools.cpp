#include "Engine.h"

namespace Soshiant2D
{
	namespace Lua
	{
		bool loadScript(lua_State* from,std::string scriptfile)
		{
			bool value = true;
			try {
				luaL_dofile(from, scriptfile.c_str());
			}
			catch(...) {
				value = false;
			}
			return value;
		}
		std::string getLuaGlobalString(lua_State* from,std::string varName)
		{
			std::string output = "";

			lua_getglobal( from , varName.c_str() );
			output = lua_tostring( from , -1 );
			lua_pop( from , 1 );

			return output;
		}

		double getLuaGlobalNumber(lua_State* from,std::string varName)
		{
			double output = 0.0;

			lua_getglobal( from , varName.c_str() );
			output = lua_tonumber( from , -1 );
			lua_pop( from , 1 );

			return output;
		}

		bool getLuaGlobalBoolean(lua_State* from,std::string varName)
		{
			bool output = false;

			lua_getglobal( from , varName.c_str() );
			if(lua_toboolean( from , -1 ))
				output = true;
			lua_pop( from , 1 );

			return output;
		}

		bool luaFunctionExist(lua_State* from,std::string varName)
		{
			bool output = false;

			lua_getglobal( from , varName.c_str() );
			output = (bool)lua_isfunction( from , -1 );
			lua_pop( from , 1 );

			return output;
		}

		///////////////////////////////////////////////
		//Soshiant Lib

		void OpenSohiantLuaLib(lua_State *L)
		{
			lua_register(L, "applyImplus", ApplyImplus);
			lua_register(L, "applyForce", ApplyForce);
			lua_register(L, "printConsol", PrintConsol);

			lua_register(L, "getX", GetX);
			lua_register(L, "setX", SetX);
			lua_register(L, "getY", GetY);
			lua_register(L, "setY", SetY);			
			lua_register(L, "setXScale", SetXScale);
			lua_register(L, "getXScale", GetXScale);
			lua_register(L, "setYScale", SetXScale);
			lua_register(L, "getYScale", GetYScale);
			lua_register(L, "setRotation", SetRotation);
			lua_register(L, "getRotation", GetRotation);

			lua_register(L, "getVX", GetVX);
			lua_register(L, "getVY", GetVY);	

			lua_register(L, "play", Play);
			lua_register(L, "stop", Stop);
			lua_register(L, "gotoAndStop", GotoAndStop);
			lua_register(L, "gotoAndPlay", GotoAndPlay);

			lua_register(L, "playSound", PlaySound);
		}

		static int ApplyImplus(lua_State *L)
		{
			if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}

			double i = lua_tonumber(L, 1);
			double j = lua_tonumber(L, 2);

			if(g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getIsPhysical())
				cpBodyApplyImpulse(g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getBody(), cpv(i,j),cpv(0,0));			

			return 0;
		}
		static int ApplyForce(lua_State *L)
		{
			if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyForce'");
				lua_error(L);
			}

			double i = lua_tonumber(L, 1);
			double j = lua_tonumber(L, 2);

			if(g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getIsPhysical())
				cpBodyApplyForce(g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getBody(), cpv(i,j),cpv(0,0));			

			return 0;
		}
		static int PrintConsol( lua_State *L )
		{
			if (!lua_isstring(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}

			g_engine->consol_text = lua_tostring(L, 1);

			return 0;
		}

		static int SetX( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}
			double x = lua_tonumber(L, 1);
			//g_engine->Message(ToString(x));
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->setX(x);
			return 0;
		}
		static int GetX( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getX());
			return 1;
		}
		static int SetY( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}
			double y = lua_tonumber(L, 1);
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->setY(y);
			return 0;
		}
		static int GetY( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getY());
			return 1;
		}
		static int SetYScale( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}
			double yscale = lua_tonumber(L, 1);
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->setYScale(yscale);
			return 0;
		}
		static int GetYScale( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getYScale());
			return 1;
		}
		static int SetXScale( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}
			double xscale = lua_tonumber(L, 1);
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->setXScale(xscale);
			return 0;
		}
		static int GetXScale( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getXScale());
			return 1;
		}
		static int SetRotation( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}
			double rot = lua_tonumber(L, 1);
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->setRotation(rot);
			return 0;
		}
		static int GetRotation( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getRotation());
			return 1;
		}

		static int GetVX( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getV().getX());
			return 1;
		}
		static int GetVY( lua_State *L )
		{
			lua_pushnumber(L, g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->getV().getY());
			return 1;
		}
		

		static int Stop( lua_State *L )
		{
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->stop();
			return 0;
		}
		static int Play( lua_State *L )
		{
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->play();
			return 0;
		}
		
		static int GotoAndStop( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'GotoAndStop'");
				lua_error(L);
			}
			double frame = lua_tonumber(L, 1);
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->gotoAndStop((int)frame);
			return 0;
		}
		static int GotoAndPlay( lua_State *L )
		{
			if (!lua_isnumber(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'GotoAndPlay'");
				lua_error(L);
			}
			double frame = lua_tonumber(L, 1);
			g_engine->_manager->_activeSeq->p_activeLayer->_activeSprite->gotoAndPlay((int)frame);
			return 0;
		}

		static int PlaySound( lua_State *L )
		{
			if (!lua_isstring(L, 1)) {
				lua_pushstring(L, "Incorrect argument to 'ApplyImplus'");
				lua_error(L);
			}

			g_engine->_manager->_activeSeq->audio->Play(lua_tostring(L, 1),1000);

			return 0;
		}


	}
}