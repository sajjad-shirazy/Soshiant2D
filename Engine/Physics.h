#include "Engine.h"

#pragma once

namespace Soshiant2D {

	class Physics
	{
	private:
		cpSpace *space;
		Timer timer;
		int updatePerSecond;

	public:
		Physics(float gravity);
		void Release();
		void Update();
		void RegisterBody(cpBody *body);
		void RegisterShape(cpShape *shape);
		void RegisterStaticShape(cpShape *shape);

		void RemoveShape(cpShape *shape);


		cpSpace *getSpace(){return this->space;}

		void setUpdatePerSecond(int value){this->updatePerSecond = value;}
		int  getUpdatePerSecond(){return this->updatePerSecond;}
	};

};