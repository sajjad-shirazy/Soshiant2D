
#include "Engine.h"

#pragma once

namespace Soshiant2D {

	class Entity {
	private:
		int id;
		std::string name;
		bool visible;
		bool alive;
		int objectType;
		int lifetimeLength;
		Timer lifetimeTimer;

	public:
		Entity();
		virtual ~Entity() { };

		virtual void move() = 0;
		virtual void animate() = 0;
		virtual void draw() = 0;

		void setID(int value) { id = value; }
		int getID() { return id; }

		std::string getName() { return name; }
		void setName(std::string value) { name = value; }

		bool getVisible() { return visible; }
		void setVisible(bool value) { visible = value; }

		bool getAlive() { return alive; }
		void setAlive(bool value) { alive = value; }

		int getLifetime() { return lifetimeLength; }
		void setLifetime(int ms) { lifetimeLength = ms; lifetimeTimer.reset(); }
		bool lifetimeExpired() { return lifetimeTimer.stopwatch(lifetimeLength); }

		int getObjectType() { return objectType; }
		void setObjectType(int value) { objectType = value; }

	};


};
