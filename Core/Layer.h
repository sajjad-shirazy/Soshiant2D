#include "Engine.h"

#pragma once


namespace Soshiant2D {
	class Sequence;

	class Layer
	{
	private:
		Vector3 position;
		std::string name;
		int id;

		std::list<Entity*> p_entities;
		lua_State* luaState;


		

	public :
		Sequence*  _parent;
		Sprite*    _activeSprite;

		Layer(std::string name,Vector3 position);
		Layer(std::string path,Sequence* parent);
		~Layer();
		
		void Release();
		Vector3 getPosition() { return position; }
		void setPosition(Vector3 position) { this->position = position; }
		void setPosition(double x, double y,double z) { position.Set(x,y,z); }

		void setID(int value) { id = value; }
		int getID() { return id; }

		std::string getName() { return name; }
		void setName(std::string value) { name = value; }

		void UpdateEntities();
		void Draw2DEntities();
		void DrowLines();
		void BuryEntities();

		std::list<Entity*> getEntityList() { return p_entities; }
		long getEntityCount() { return (long)p_entities.size(); }
		void addEntity(Entity *entity);
		void removeEntity(int id);
		Entity *findEntity(std::string name);
		Entity *findEntity(int objectType);
		Entity *findEntity_byID(int ID);
		int getEntityCount(int objectType);

		Sprite *SelectSprite(int x,int y);
		void UnSelectAll();

		void LoadFromFile(std::string path);
	};
};