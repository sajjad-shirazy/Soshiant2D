#include "Engine.h"


namespace Soshiant2D{
	ParticleSystem::ParticleSystem(Layer *parent):Sprite(parent)
	{
		this->count = 100 ;
		this->setStartColor(255,162,0);
		this->setEndColor(255,6,0);
		this->startSpin  = 0;
		this->endSpin    = 0;
		this->minParticleLifeTime = 1;
		this->maxParticleLifeTime = 500;
		this->minStartSpeed = 0.01;
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
			itt = this->particles.begin();
			while(itt!=this->particles.end())
			{
				(*itt)->draw(D3DBLEND_ONE);
				itt++;
			}
		}
		g_engine->consol_text = ToString(particles.size());

	}

	void ParticleSystem::animate()
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
		float angle = Math::random()*2*PI;
		double xv = cos(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
		double yv = sin(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
		
		particle->setVelocity(xv,yv);
		//init life time
		particle->setLifetime(this->minParticleLifeTime + Math::random()*(this->maxParticleLifeTime - this->minParticleLifeTime));		
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
				Sprite* particle = new Sprite(this->parent);
				particle->setImage(this->image);
				particle->setPosition(this->getX(),this->getY());
				particle->setColor(startColor_R,startColor_G,startColor_B,startAlpha);
				float angle = Math::random()*2*PI;
				double xv = cos(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
				double yv = sin(angle)*(this->minStartSpeed + Math::random()*(this->maxStartSpeed-this->minStartSpeed));
				particle->setVelocity(xv,yv);
				particle->setLifetime(this->minParticleLifeTime + Math::random()*(this->maxParticleLifeTime - this->minParticleLifeTime));
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
}