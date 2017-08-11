#include "Engine.h"

namespace Soshiant2D {
	
	Layer::Layer(std::string name,Vector3 position)
	{
		this->name=name;
		this->position=position;
		//this->luaState = lua_open();
		//luaL_openlibs(this->luaState);
	}

	Layer::~Layer()
	{
		lua_close(this->luaState);
		this->Release();
		
	}

	void Layer::Release()
	{
		std::list<Entity*>::iterator iter = p_entities.begin();
		while (iter != p_entities.end())
		{
			delete (*iter);
			iter++;
		} 
		p_entities.clear();
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
			}
			++iter;
		} 
		BuryEntities();
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
		std::list<Entity*>::iterator i = p_entities.begin();
		while (i != p_entities.end())
		{
			if ( (*i)->getAlive() == true && (*i)->getID() == ID ) 
				return *i;
			else
				++i;
		}
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
		entity->_parent = this ;
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
				//delete (*iter);
				//g_engine->Message("Layaer Is Null ! : "+ ToString(ID));
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

	void Layer::LoadFromFile(std::string path)
	{
		this->Release();
		std::string spt_ext = ".spt.lua" ;
		std::string prt_ext = ".prt.lua" ;
		if(Lua::loadScript(this->luaState,path))
		{
			this->name = Lua::getLuaGlobalString( this->luaState ,"name");
			this->position.setZ(Lua::getLuaGlobalNumber( this->luaState , "depth" ));
			int spritesCount = (int)Lua::getLuaGlobalNumber( this->luaState , "spritesCount" );

			for(int i=0;i<spritesCount;i++)
			{
				std::string spriteType = Lua::getLuaGlobalString( this->luaState ,"sprite_"+ToString(i)+"_type");
				if(spriteType == "Sprite")
				{
					this->_activeSprite = new Sprite(Path::getParentDirectory(path)+"\\"+ this->name+"_sprite_"+ToString(i)+spt_ext,this);
					this->addEntity(this->_activeSprite);
				}
				else if(spriteType == "Particle")
				{
					this->_activeSprite = new ParticleSystem(Path::getParentDirectory(path)+"\\"+ this->name+"_particle_"+ToString(i)+prt_ext,this);
					this->addEntity(this->_activeSprite);
				}
			}
		}
		else
		{
			//Error in loading
		}
	}

	Layer::Layer(std::string path,Sequence* parent)
	{	
		this->_parent = parent;
		this->luaState = lua_open();
		//luaL_openlibs(this->luaState);
		this->LoadFromFile(path);
	}
}
