#include "Engine.h"

namespace Soshiant2D {
	
	Layer::Layer(int id,std::string name,Vector3 position)
	{
		this->name=name;
		this->id=id;
		this->position=position;
	}

	Layer::~Layer()
	{
		//
	}

	void Layer::UpdateEntities()
	{
		std::list<Entity*>::iterator iter;
		Entity *entity;
	
		iter = p_entities.begin();
		while (iter != p_entities.end())
		{
			//point local sprite to object in the list
			entity = *iter;
			
			//is this entity alive?
			if ( entity->getAlive() ) {

				//move/animate entity
				entity->move();
				entity->animate();

				//tell game that this entity has been updated
				//game_entityUpdate( entity );

				//see if this entity will auto-expire
				if ( entity->getLifetime() > 0)
				{
					if ( entity->lifetimeExpired() ) 
					{
						entity->setAlive(false);
					}
				}
				++iter;
			}
			else
			{
				delete (*iter);
				iter = p_entities.erase( iter );
			}
		} 
		//BuryEntities();
	}

	void Layer::Draw2DEntities()
	{
		Entity *entity;
		std::list<Entity*>::iterator iter = p_entities.begin();
		while (iter != p_entities.end())
		{
			//temporary pointer
			entity = *iter;
			
			//is this a 2D entity?
			//if ( entity->getRenderType() == RENDER2D ) {

				//is this entity in use?
				if ( entity->getAlive() ) {

                    if ( entity->getVisible() ) 
						entity->draw();

                    //game_entityRender( entity );
				}
			//}becuse we hav ont any 3D Object

			++iter;
		} 
	}

	void Layer::DrowLines()
	{
		Sprite* sprite=NULL;
		std::list<Entity*>::iterator iter = p_entities.begin();
		while (iter != p_entities.end())
		{
			sprite=(Sprite*)(*iter);
			sprite->drowShapes();
			iter++;
		} 
	}

	void Layer::BuryEntities()
	{
		std::list<Entity*>::iterator iter = p_entities.begin();
		while (iter != p_entities.end())
		{
			if ( (*iter)->getAlive() == false )  
			{
				delete (*iter);
				iter = p_entities.erase( iter );
			}
			else iter++;
		}
	}
		
	Entity *Layer::findEntity(std::string name)
	{
		std::list<Entity*>::iterator i = p_entities.begin();
		while (i != p_entities.end())
		{
			if ( (*i)->getAlive() == true && (*i)->getName() == name ) 
				return *i;
			else
				++i;
		}
		
		return NULL;
	}
	
	Entity *Layer::findEntity(int objectType)
	{
		std::list<Entity*>::iterator i = p_entities.begin();
		while (i != p_entities.end())
		{
			if ( (*i)->getAlive() == true && (*i)->getObjectType() == objectType ) 
				return *i;
			else
				++i;
		}
		return NULL;
	}

	Entity *Layer::findEntity_byID(int ID)
	{
		std::string expt = "";
		std::list<Entity*>::iterator i = p_entities.begin();
		while (i != p_entities.end())
		{
			expt+= (*i)->getName()+" == " + ToString((*i)->getID())+"\n";
			if ( (*i)->getAlive() == true && (*i)->getID() == ID ) 
				return *i;
			else
				++i;
		}
		g_engine->Message(expt);
		return NULL;
	}

	int Layer::getEntityCount(int objectType)
	{
		int total = 0;
		std::list<Entity*>::iterator i = p_entities.begin();
		while (i != p_entities.end())
		{
			if ( (*i)->getAlive() == true && (*i)->getObjectType() == objectType ) 
				total++;
			else
				++i;
		}
		return total;
	}

	void Layer::addEntity(Entity *entity)
	{
		static int id = 0;
		
		entity->setID(id);
		p_entities.push_back(entity);
		id++;
	}

	void Layer::removeEntity(int ID)
	{
		std::list<Entity*>::iterator iter = this->p_entities.begin();
		while (iter != this->p_entities.end())
		{
			if ( (*iter)->getID() == ID )  
			{
				delete (*iter);
				iter = this->p_entities.erase( iter );
				break;
			}
			iter++;
		}
	}

	Sprite* Layer::SelectSprite(int x,int y)
	{
		Rect rect;	

		Sprite* sprite=NULL;
		Sprite* selectedSprite=NULL;

		std::list<Entity*>::iterator i = this->p_entities.begin();
		while (i != this->p_entities.end())
		{
			sprite=(Sprite*)(*i);
			/*std::string k="i am in select cheching loop / Entity Cunt : "+ToString(this->p_entities.size())+"/itertor : "+ToString((int)i._Ptr);
			MessageBox(g_engine->getWindowHandle(),k.c_str(),"",0);*/
			rect=sprite->getBounds();		
			
			if ( rect.isInside(x,y)) 
			{
				if(selectedSprite)
				{
					selectedSprite->setSelected(false);
				}
				sprite->setSelected(true);
				selectedSprite = sprite;					
			}
			else
				sprite->setSelected(false);


			i++;
		}
		return selectedSprite;
	}

	void Layer::UnSelectAll()
	{
		Sprite* sprite=NULL;

		std::list<Entity*>::iterator i = this->p_entities.begin();
		while (i != this->p_entities.end())
		{
			sprite=(Sprite*)(*i);
			sprite->setSelected(false);
			i++;
		}
	}
}
