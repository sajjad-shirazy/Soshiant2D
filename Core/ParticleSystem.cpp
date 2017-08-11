#include "Engine.h"


namespace Soshiant2D{
	ParticleSystem::ParticleSystem(std::string path,Layer* parent):Sprite(parent)
	{
		this->count = 100 ;
		this->setStartColor(255,162,0);
		this->setEndColor(255,6,0);
		this->startSpin  = 0;
		this->endSpin    = 0;
		this->minParticleLifeTime = 1;
		this->maxParticleLifeTime = 500;
		this->minStartSpeed = 0.01f;
		this->maxStartSpeed = 2;
		this->acceleration  = Vector3(0.96f,0.96f,0.0f);
		this->gravity = Vector3(0.0,-0.04,0.0);
		this->minScale = 0.5;
		this->maxScale = 1.0;
		this->repeat = true ;
		this->startAlpha = 255 ;
		this->endAlpha = 0 ;
		this->paused = false;
		//loading from lua ...

		this->luaState = lua_open();
		luaL_openlibs(this->luaState);
		Lua::OpenSohiantLuaLib(this->luaState);
		this->LoadFromFile(path);
		this->init(this->count);
	}
	ParticleSystem::ParticleSystem(Layer* parent):Sprite(parent)
	{
		//g_engine->consol_text = "particle initedt:";
		this->count = 100 ;
		this->setStartColor(255,162,0);
		this->setEndColor(255,6,0);
		this->startSpin  = 0;
		this->endSpin    = 0;
		this->minParticleLifeTime = 1;
		this->maxParticleLifeTime = 500;
		this->minStartSpeed = 0.01f;
		this->maxStartSpeed = 2;
		this->acceleration  = Vector3(0.96f,0.96f,0.0f);
		this->gravity = Vector3(0.0,-0.04,0.0);
		this->minScale = 0.5;
		this->maxScale = 1.0;
		this->repeat = true ;
		this->startAlpha = 255 ;
		this->endAlpha = 0 ;
		this->paused = false;
	}

	ParticleSystem::~ParticleSystem()
	{
		itt = this->particles.begin();
		while(itt!=this->particles.end())
		{
			delete (*itt);
			itt++;
		}
		this->particles.clear();
	}

	void ParticleSystem::draw()
	{
		if(this->getVisible())
		{
			animate2();
			g_engine->getSpriteHandler()->Flush();
			g_engine->getDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			itt = this->particles.begin();
			while(itt!=this->particles.end())
			{
				(*itt)->draw();
				itt++;
			}	
			g_engine->getSpriteHandler()->Flush();
			g_engine->getDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
	}

	void ParticleSystem::animate2()
	{
		if(!this->paused)
		{

			static float r,g,b,a;
			itt = this->particles.begin();
			while(itt!=this->particles.end())
			{
				(*itt)->move();

				int totalFrames = (*itt)->getTotalFrames();
				(*itt)->setLifetime((*itt)->getLifetime()-1);

				if((*itt)->getLifetime()>0)
				{
					//Alpha & color Animating
					r = (*itt)->getR() + (endColor_R - startColor_R)/totalFrames;
					g = (*itt)->getG() + (endColor_G - startColor_G)/totalFrames;
					b = (*itt)->getB() + (endColor_B - startColor_B)/totalFrames;
					a = (*itt)->getAlpha() + (endAlpha - startAlpha)/totalFrames;
					(*itt)->setColor(r,g,b,a);

					//physics Animating ...
					(*itt)->setVelocity((*itt)->getVelocity().getX()*this->acceleration.getX()+this->gravity.getX(),(*itt)->getVelocity().getY()*this->acceleration.getY()+this->gravity.getY());
					itt++;
				}
				else
				{
					if(this->repeat)
					{
						
						reborn((*itt));
					}
					else
					{
						itt = this->particles.erase( itt );
					}
						
				}
			}
		}
	}
	void ParticleSystem::init(int count)
	{
		this->count = count;
		addParticle(count);
		
	}
	void ParticleSystem::reborn(Sprite* particle)
	{
		particle->setPosition(this->getX(),this->getY());
		particle->setColor(startColor_R,startColor_G,startColor_B,startAlpha);

		//init local physics
		float angle = (float)(Math::random()*2*PI);
		double xv = cos(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
		double yv = sin(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
		
		particle->setVelocity(xv,yv);
		//init life time
		particle->setLifetime((int)(this->minParticleLifeTime + Math::random()*(this->maxParticleLifeTime - this->minParticleLifeTime)));		
		particle->setTotalFrames(particle->getLifetime());
		//
		particle->setScale(this->minScale + Math::random()*(this->maxScale - this->minScale));

	}
	void ParticleSystem::recount(int count)
	{
		this->count = particles.size();
		//g_engine->Message(ToString(this->count-count ));
		if(count>this->count)
		{
			addParticle(count - this->count);
		}
		else if(count<this->count)
		{
			
			removeParticle(this->count-count );
		}
		this->count = count;
	}
	void ParticleSystem::addParticle(int count)
	{
		if(this->image !=NULL)
		{
			for(int i=0;i<count;i++)
			{
				Sprite* particle = new Sprite(this->_parent);
				particle->setImage(this->image);
				particle->setPosition(this->getX(),this->getY());
				particle->setColor(startColor_R,startColor_G,startColor_B,startAlpha);
				float angle = (float)(Math::random()*2*PI);
				double xv = cos(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
				double yv = sin(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
				particle->setVelocity(xv,yv);
				particle->setLifetime((int)(this->minParticleLifeTime + Math::random()*(this->maxParticleLifeTime - this->minParticleLifeTime)));
				particle->setTotalFrames(particle->getLifetime());
				particle->setScale(this->minScale + Math::random()*(this->maxScale - this->minScale));

				this->particles.push_back(particle);
			}
		}
	}
	void ParticleSystem::removeParticle(int count)
	{		
		for(int i=0;i<count;i++)
		{	
			this->particles.erase( this->particles.begin() );
		}
	}
	void ParticleSystem::resetImage()
	{
		itt = this->particles.begin();
		while(itt!=this->particles.end())
		{
			(*itt)->setImage(this->image);
			itt++;
		}
	}

	void ParticleSystem::LoadFromFile(std::string path)
	{
		if(Lua::loadScript(this->luaState,path))
		{
			this->name = Lua::getLuaGlobalString(this->luaState,"name");
			std::string imageName = Lua::getLuaGlobalString(this->luaState,"image");
			this->setImage(this->_parent->_parent->_library->getTexture(imageName));
			this->setX(Lua::getLuaGlobalNumber(this->luaState,"x"));
			this->setY(Lua::getLuaGlobalNumber(this->luaState,"y"));
			this->setScale(Lua::getLuaGlobalNumber(this->luaState,"scale"));
			this->width      = Lua::getLuaGlobalNumber(this->luaState,"width");
			this->height     = Lua::getLuaGlobalNumber(this->luaState,"height");
			this->setAlpha(Lua::getLuaGlobalNumber(this->luaState,"alpha"));

			this->animcolumns = Lua::getLuaGlobalNumber(this->luaState,"cols");
			this->totalframes= Lua::getLuaGlobalNumber(this->luaState,"totalFrames");
			this->frametimer = Lua::getLuaGlobalNumber(this->luaState,"anim_timer");
			this->animdir = Lua::getLuaGlobalNumber(this->luaState,"isRev");

			bool  isPhysical = Lua::getLuaGlobalBoolean(this->luaState,"isPhysical");
			bool  isStatic   = Lua::getLuaGlobalBoolean(this->luaState,"isStatic");
			float mass       = Lua::getLuaGlobalNumber(this->luaState,"mass");
			float inertia    = Lua::getLuaGlobalNumber(this->luaState,"inertia");

			this->count = Lua::getLuaGlobalNumber(this->luaState,"count");
			this->startColor_R = Lua::getLuaGlobalNumber(this->luaState,"startColor_R");
			this->startColor_G = Lua::getLuaGlobalNumber(this->luaState,"startColor_G");
			this->startColor_B = Lua::getLuaGlobalNumber(this->luaState,"startColor_B");
			this->endColor_R = Lua::getLuaGlobalNumber(this->luaState,"endColor_R");
			this->endColor_G = Lua::getLuaGlobalNumber(this->luaState,"endColor_G");
			this->endColor_B = Lua::getLuaGlobalNumber(this->luaState,"endColor_B");
			this->startAlpha = Lua::getLuaGlobalNumber(this->luaState,"startAlpha");
			this->endAlpha = Lua::getLuaGlobalNumber(this->luaState,"endAlpha");
			this->minParticleLifeTime = Lua::getLuaGlobalNumber(this->luaState,"minParticleLifeTime");
			this->maxParticleLifeTime = Lua::getLuaGlobalNumber(this->luaState,"maxParticleLifeTime");
			this->minStartSpeed = Lua::getLuaGlobalNumber(this->luaState,"minStartSpeed");
			this->maxStartSpeed = Lua::getLuaGlobalNumber(this->luaState,"maxStartSpeed");
			this->acceleration.setX( Lua::getLuaGlobalNumber(this->luaState,"acceleration_x") );
			this->acceleration.setY( Lua::getLuaGlobalNumber(this->luaState,"acceleration_y") );
			this->gravity.setX( Lua::getLuaGlobalNumber(this->luaState,"gravity_x") );
			this->gravity.setY( Lua::getLuaGlobalNumber(this->luaState,"gravity_y") );
			this->minScale = Lua::getLuaGlobalNumber(this->luaState,"minScale");
			this->maxScale = Lua::getLuaGlobalNumber(this->luaState,"maxScale");
			this->repeat = Lua::getLuaGlobalBoolean(this->luaState,"rpt");

			if(isPhysical)
				this->setPhysics(mass,inertia,isStatic);

			//Registering Lua functions
			if(Lua::luaFunctionExist(this->luaState,"OnKeyDown"))
				this->_parent->_parent->AddKeyDownEventFunction(this);
			if(Lua::luaFunctionExist(this->luaState,"OnKeyUp"))
				this->_parent->_parent->AddKeyUpEventFunction(this);
			if(Lua::luaFunctionExist(this->luaState,"OnEnterFrame"))
				this->_parent->_parent->AddOnEnterFrameEventFunction(this);	
			if(Lua::luaFunctionExist(this->luaState,"OnLoad"))
				this->_parent->_parent->AddOnLoadEventFunction(this);		
			if(Lua::luaFunctionExist(this->luaState,"OnMouseMove"))
				this->_parent->_parent->AddMouseMoveEventFunction(this);

			if(this->isPhysical)
			{
				int lineShapesCount = Lua::getLuaGlobalNumber(this->luaState,"lineShapesCount");
				for(int j=0;j<lineShapesCount;j++)
				{
					float ax = Lua::getLuaGlobalNumber(this->luaState,"lineShape_" + ToString(j) + "_aX");
					float ay = Lua::getLuaGlobalNumber(this->luaState,"lineShape_" + ToString(j) + "_aY");
					float bx = Lua::getLuaGlobalNumber(this->luaState,"lineShape_" + ToString(j) + "_bX");
					float by = Lua::getLuaGlobalNumber(this->luaState,"lineShape_" + ToString(j) + "_bY");
					float thickness = Lua::getLuaGlobalNumber(this->luaState,"lineShape_" + ToString(j) + "_thickness");
					this->addLineSegment(ax,ay,bx,by,thickness);
				}
				int circleShapesCount = Lua::getLuaGlobalNumber(this->luaState,"circleShapesCount");
				for(int j=0;j<circleShapesCount;j++)
				{
					float x = Lua::getLuaGlobalNumber(this->luaState,"circleShape_" + ToString(j) + "_x");
					float y = Lua::getLuaGlobalNumber(this->luaState,"circleShape_" + ToString(j) + "_y");
					float r = Lua::getLuaGlobalNumber(this->luaState,"circleShape_" + ToString(j) + "_r");
					this->addCircle(r,x,y);
				}
				int polygonShapesCount = Lua::getLuaGlobalNumber(this->luaState,"polygonShapesCount");
				for(int j=0;j<polygonShapesCount;j++)
				{
					//
				}
			}
		}
		else
		{
			//Error in loading
		}
	}
}