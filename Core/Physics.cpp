#include "Engine.h"

namespace Soshiant2D {

	Physics::Physics(float gravity)
	{
		cpInitChipmunk(); 

		//timer=Timer();
		this->space = cpSpaceNew();
		this->space->gravity = cpv( 0 , gravity );
		this->updatePerSecond = 100;
		cpResetShapeIdCounter();
		//space->iterations=10;

	}
	Physics::~Physics()
	{
		this->Release();
	}
	
	void Physics::Release()
	{
		//cpSpaceFreeChildren(this->space);
		cpSpaceFree(this->space);
		//delete this->space;
	}

	void Physics::Update()
	{
		if(timer.stopwatch(1000/updatePerSecond))
		{
			cpSpaceStep(space, (float)1/updatePerSecond);
		}
	}

	void Physics::RegisterBody(cpBody *body)
	{
		cpSpaceAddBody(this->space, body);
		cpSpaceRehashStatic(this->space);
	}

	void Physics::RegisterShape(cpShape *shape)
	{
		shape->e = 0.5; shape->u = 0.8;
		//shape->collision_type = 1;
		cpSpaceAddShape(this->space, shape);
	}

	void Physics::RegisterStaticShape(cpShape *shape)
	{
		shape->e = 0.5; shape->u = 0.1;// shape->collision_type = 0;
		cpSpaceAddStaticShape( this->space , shape );
	}

	void Physics::RemoveShape(cpShape *shape)
	{
		Sprite* sprite = (Sprite*) shape->data ;

		if(sprite->getIsStatic())
			cpSpaceRemoveStaticShape(this->space,shape);
		else
			cpSpaceRemoveShape(this->space,shape);

		if(shape)
		{
			cpShapeFree(shape);
			//delete shape;
		}
	}


}