#include "Engine.h"

#pragma once

namespace Soshiant2D {
	class ParticleSystem : public Sprite
	{
	private:
		std::vector<Sprite*>::iterator itt;
		std::vector<Sprite*> particles;
		int count;

		float startColor_R;
		float startColor_G;
		float startColor_B;

		float endColor_R;
		float endColor_G;
		float endColor_B;

		float startAlpha;
		float endAlpha;


		int startSpin;
		int endSpin;
		int minParticleLifeTime;
		int maxParticleLifeTime;
		float minStartSpeed;
		float maxStartSpeed;
		Vector3 acceleration;
		Vector3 gravity;
		double minScale;
		double maxScale;
		bool repeat ;

	public :
		ParticleSystem(Layer *parent);
		virtual ~ParticleSystem();
		void animate();
		void draw();
		void init(int count);
		void reborn(Sprite* particle);
		void recount(int count);
		void addParticle(int count);
		void removeParticle(int count);
		void resetImage();

		void setStartColor(float R,float G,float B) { 
			this->startColor_R = R ;
			this->startColor_G = G ;
			this->startColor_B = B ;
		}
		void setEndColor(float R,float G,float B) { 
			this->endColor_R = R ;
			this->endColor_G = G ;
			this->endColor_B = B ;
		}

		void setStartSpin(int startSpin) { this->startSpin = startSpin;}
		void setEndSpin(int endSpin) { this->endSpin = endSpin;}

		void setMinParticleLifeTime(int minParticleLifeTime){this->minParticleLifeTime = minParticleLifeTime;}
		void setMaxParticleLifeTime(int maxParticleLifeTime){this->maxParticleLifeTime = maxParticleLifeTime;}

		void setMinStartSpeed(float minStartSpeed){this->minStartSpeed = minStartSpeed;}
		void setMaxStartSpeed(float maxStartSpeed){this->maxStartSpeed = maxStartSpeed;}

		void setMaxScale(double maxScale) { this->maxScale = maxScale;}
		void setMinScale(double minScale) { this->minScale = minScale;}

		void setAcceleration(Vector3 acceleration){this->acceleration = acceleration;}

		void setGravity(Vector3 gravity){this->gravity = gravity;}

		void setRepeat(bool repeat){ this->repeat = repeat;}

		void setStartAlpha(float startAlpha){this->startAlpha = startAlpha ; }
		void setEndAlpha(float endAlpha){this->endAlpha = endAlpha ; }
	};
}