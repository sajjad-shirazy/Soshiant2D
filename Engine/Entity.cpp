
#include "Engine.h"

namespace Soshiant2D {

	Entity::Entity()
	{
		this->id = -1;
		this->name = "";
		this->visible = true;
		this->alive = true;
		this->objectType = 0;
		this->lifetimeLength = 0;
		this->lifetimeTimer.reset();
	}
	
};
