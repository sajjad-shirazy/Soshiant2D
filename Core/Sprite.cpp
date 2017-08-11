#include "Engine.h"


namespace Soshiant2D {

	Sprite::Sprite(Layer* parent) : Entity()
	{
		this->_parent = parent;
		this->body = cpBodyNew(INFINITY,INFINITY);
		this->isPhysical = false;
		this->isStatic   = true;
		this->image = NULL;
		this->imageLoaded = false;	
		this->setPosition(0.0f,0.0f);	
		this->setVelocity(0.0f,0.0f);
		this->state = 0;
		this->direction = 0;
		this->width = 1;
		this->height = 1;
		this->curframe = 0;
		this->totalframes = 1;
		this->animdir = 1;
		this->animcolumns = 1;
		this->framestart = 0;
		this->frametimer = 0;
		this->animcolumns = 1;
		this->animstartx = 0;
		this->animstarty = 0;
		this->faceAngle = 0;
		this->moveAngle = 0;
		//this->rotation = 0;
		this->xScale = 1.0f;
		this->yScale = 1.0f;
		this->alpha=255;
		this->color = D3DCOLOR_RGBA(255,255,255,(int)this->alpha);
		this->movetimer = 0;
		this->movestart = 0;
		
		this->drowBounds = false;
		this->lineSpace = 5;

		//this->luaState = lua_open();
		//luaL_openlibs(this->luaState);
		//Lua::OpenSohiantLuaLib(this->luaState);
	}
	
	Sprite::~Sprite()  {

		clearPhysics();
		lua_close(this->luaState);

		if (imageLoaded)
			delete image;
		if(line)
			line->Release();
	}
	
	bool Sprite::loadImage(std::string filename, D3DCOLOR transcolor)
	{
		if (image != NULL) delete image;
	
		image = new Texture();
		if (image->Load(filename,transcolor))
		{
			this->setSize(image->getWidth(),image->getHeight());
			imageLoaded = true;
			return true;
		}
		else
			return false;
	}
	
	void Sprite::setImage(Texture *image)
	{
		this->image = image;
		this->setWidth(image->getWidth());
		this->setHeight(image->getHeight());
		this->imageLoaded = false;
	}
	
	void Sprite::transform()
	{
		D3DXMATRIX mat;
		D3DXVECTOR2 scale((float)xScale,(float)yScale);
		D3DXVECTOR2 center((float)(width*xScale)/2, (float)(height*yScale)/2);
		D3DXVECTOR2 trans((float)((getX()- (this->width*this->xScale)/2)+_parent->getPosition().getX()+editor_x_base), (float)((getY()- (this->height*this->yScale)/2)+_parent->getPosition().getY()+editor_y_base));
		D3DXMatrixTransformation2D(&mat,NULL,0,&scale,&center,(float)body->a,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);
	}

	void Sprite::draw()
	{
		int fx = (this->curframe % this->animcolumns) * this->width;
		int fy = (this->curframe / this->animcolumns) * this->height;
		RECT srcRect = {fx,fy, fx+this->width, fy+this->height};

		this->transform();
		HRESULT res =  g_engine->getSpriteHandler()->Draw(this->image->GetTexture(),&srcRect,NULL,NULL,this->color);

		if(this->drowBounds)
		{
			LPD3DXLINE line;
			D3DXCreateLine(g_engine->getDevice(), &line);
			
			Rect bonds = this->getBounds();
			D3DXVECTOR2 lines[] = {D3DXVECTOR2((float)bonds.left-lineSpace, (float)bonds.top-lineSpace), D3DXVECTOR2((float)bonds.right+lineSpace,(float) bonds.top-lineSpace), D3DXVECTOR2((float)bonds.right+lineSpace, (float)bonds.bottom+lineSpace), D3DXVECTOR2((float)bonds.left-lineSpace,(float) bonds.bottom+lineSpace),D3DXVECTOR2((float)bonds.left-lineSpace, (float)bonds.top-lineSpace)};			
			g_engine->getLineHandler()->Begin();
			g_engine->getLineHandler()->Draw(lines, 5, D3DCOLOR_XRGB(255,0,0));
			g_engine->getLineHandler()->End();

		}
	}
	void Sprite::draw(DWORD blendType)
	{
		int fx = (this->curframe % this->animcolumns) * this->width;
		int fy = (this->curframe / this->animcolumns) * this->height;
		RECT srcRect = {fx,fy, fx+this->width, fy+this->height};

		this->transform();
		
		g_engine->getSpriteHandler()->Flush();
		g_engine->getDevice()->SetRenderState( D3DRS_DESTBLEND, blendType );
		g_engine->getSpriteHandler()->Draw(this->image->GetTexture(),&srcRect,NULL,NULL,color);
		g_engine->getSpriteHandler()->Flush();
		g_engine->getDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		

		if(this->drowBounds)
		{
			LPD3DXLINE line;
			D3DXCreateLine(g_engine->getDevice(), &line);
			
			Rect bonds = this->getBounds();
			D3DXVECTOR2 lines[] = {D3DXVECTOR2((float)bonds.left-lineSpace, (float)bonds.top-lineSpace), D3DXVECTOR2((float)bonds.right+lineSpace,(float) bonds.top-lineSpace), D3DXVECTOR2((float)bonds.right+lineSpace, (float)bonds.bottom+lineSpace), D3DXVECTOR2((float)bonds.left-lineSpace,(float) bonds.bottom+lineSpace),D3DXVECTOR2((float)bonds.left-lineSpace, (float)bonds.top-lineSpace)};			
			g_engine->getLineHandler()->Begin();
			g_engine->getLineHandler()->Draw(lines, 5, D3DCOLOR_XRGB(255,0,0));
			g_engine->getLineHandler()->End();

		}
	}
	
	void Sprite::move()
	{
		if(!this->isPhysical)
		{
			if (movetimer > 0) {
				if (timeGetTime() > (DWORD)(movestart + movetimer)) {
					//reset move timer
					movestart = timeGetTime();

					//move sprite by velocity amount
					this->setX(this->getX() + this->velocity.getX());
    				this->setY(this->getY() + this->velocity.getY());
				}
			}
			else {
				//no movement timer--update at cpu clock speed
				this->setX(body->p.x + this->velocity.getX());
				this->setY(body->p.y + this->velocity.getY());
			}
		}
	}
	

	void Sprite::animate() 
	{
		if(!this->paused)
		{
			//update frame based on animdir
			if (frametimer > 0) {
				if (timeGetTime() > (DWORD)(framestart + frametimer))  {
					//reset animation timer
					framestart = timeGetTime();
					curframe += animdir;

					//keep frame within bounds	
					if (curframe < 0) curframe = totalframes-1;
					if (curframe > totalframes-1) curframe = 0;
				}
			}
			else {
				//no animation timer--update at cpu clock speed
				curframe += animdir;
				if (curframe < 0) curframe = totalframes-1;
				if (curframe > totalframes-1) curframe = 0;
			}
		}
	}

    Rect Sprite::getBounds()
    {
    	Rect rect;
        rect.left    = (getX()- (this->width*this->xScale)/2) + _parent->getPosition().getX() + editor_x_base;
		rect.top     = (getY()- (this->height*this->yScale)/2) + _parent->getPosition().getY() + editor_y_base;
		rect.right   = (getX()- (this->width*this->xScale)/2) + _parent->getPosition().getX() + editor_x_base + getWidth()  * getXScale();
		rect.bottom  = (getY()- (this->height*this->yScale)/2) + _parent->getPosition().getY() + editor_y_base + getHeight() * getYScale();
        return rect;
    }

	void Sprite::setPhysics(float mass, float inertia,bool isStatic)
	{
		float x = (float)this->getX();
		float y = (float)this->getY();
		if(!this->isPhysical)
		{
			this->isPhysical = true ;			
			if(!isStatic)
			{
				this->isStatic = false ;
				if(inertia==-1)inertia = INFINITY;
				this->body = cpBodyNew(mass, inertia);
				this->body->p = cpv(x, y);
				this->_parent->_parent->getPhysicsHandler()->RegisterBody( this->body );
			}
		}
		else
			MessageBox(0,"Engine :: you can not reset physics for a sprite !",0,0);
	}

	void Sprite::clearPhysics()
	{
		cpBodyFree(this->body);

		std::list<Shape*>::iterator i = this->shapesList.begin();
		while(i!=this->shapesList.end())
		{
			this->_parent->_parent->getPhysicsHandler()->RemoveShape((*i)->content);
			delete (*i);
			i++;
		}
		this->isPhysical = false;
		this->shapesList.clear();
	}

	void Sprite::RemoveShape(int id)
	{
		std::list<Shape*>::iterator i = this->shapesList.begin();

		while(i!=this->shapesList.end())
		{
			if((*i)->content->id == id)
			{
				this->_parent->_parent->getPhysicsHandler()->RemoveShape((*i)->content);
				this->shapesList.erase(i);
				break;
			}
			i++;
		}
	}

	Shape* Sprite::addCircle(float radius , float xOffset , float yOffset)
	{
		if(this->isPhysical)
		{
			cpVect offset;
			offset.x = xOffset;
			offset.y = yOffset;

			Shape *shape=new Shape();
			shape->setType(CIRCLE);

			shape->content = cpCircleShapeNew ( this->body , radius , offset);

			shape->content->data = this;

			this->shapesList.push_back( shape );

			if(this->isStatic)
				this->_parent->_parent->getPhysicsHandler()->RegisterStaticShape(shape->content);
			else
				this->_parent->_parent->getPhysicsHandler()->RegisterShape(shape->content);

			

			return shape;
		}
		else
			g_engine->Message("Adding shape to a unPhysical Sprite ...");
		return NULL;
	}
	
	Shape*  Sprite::addLineSegment(float ax,float ay,float bx,float by,float radius)
	{
		if(this->isPhysical)
		{
			cpVect a,b;
			a.x = ax; a.y = ay;
			b.x = bx; b.y = by;
			

			Shape *shape=new Shape();
			shape->setType(LINESEGMENT);
			//Beep( 750, 50 );
			shape->content = cpSegmentShapeNew(this->body,a,b,radius);
			shape->content->data = this;
			//Beep( 500, 50 );


			if(this->isStatic)
				this->_parent->_parent->getPhysicsHandler()->RegisterStaticShape(shape->content);
			else
				this->_parent->_parent->getPhysicsHandler()->RegisterShape(shape->content);		


			this->shapesList.push_back( shape );
			return shape;
		}
		else
			g_engine->Message("Adding shape to a unPhysical Sprite ...");
		return NULL;
	}

	Shape*  Sprite::addConvexPolygon(int vertxCnt,cpVect *vertxs , float xOffset , float yOffset)
	{
		if(this->isPhysical)
		{
			cpVect offset;
			offset.x = xOffset;
			offset.y = yOffset;

			Shape *shape = new Shape();
			shape->setType(CONVEXPOLYGON);

			shape->content = cpPolyShapeNew(this->body,vertxCnt,vertxs,offset);
			shape->content->data = this;

			this->shapesList.push_back( shape );

			if(this->isStatic)
				this->_parent->_parent->getPhysicsHandler()->RegisterStaticShape(shape->content);
			else
				this->_parent->_parent->getPhysicsHandler()->RegisterShape(shape->content);

			return shape;
		}
		else
			g_engine->Message("Adding shape to a unPhysical Sprite ...");
		return NULL;
	}

	void Sprite::setShapeVisible( int id , bool value )
	{
		findShape(id)->setVisible(value);
	}

	void Sprite::drowShapes()
	{
		/*if(shapesList.size()>0)
		{
			MessageBox(0,::ToString(shapesList.size()).c_str(),0,0);
		}*/
		std::list<Shape*>::iterator i = this->shapesList.begin();

		while(i!=this->shapesList.end())
		{
			(*i)->drow();
			i++;
		}
	}

	Shape* Sprite::findShape(int id)
	{
		std::list<Shape*>::iterator i = this->shapesList.begin();

		while(i!=this->shapesList.end())
		{
			if((*i)->content->id == id)
			{
				return (*i);
			}
			i++;
		}
		return NULL;
	}

	void Sprite::UnselectAllShapes()
	{
		std::list<Shape*>::iterator i = this->shapesList.begin();

		while(i!=this->shapesList.end())
		{
			(*i)->setSelected(false);
			(*i)->setEditeMode(false);
			i++;
		}
	}

	//Input Functions
	void Sprite::OnKeyDown(int e)
	{
		//g_engine->Message(ToString(e));
		//0 means regular lua state
		if( lua_status(this->luaState) == 0 )
		{
			lua_getglobal(this->luaState,"OnKeyDown");
			lua_pushnumber(this->luaState, e);
			lua_call(this->luaState,1,0);
		}
		else
		{
			g_engine->Message("Engine::Sprite ["+this->getName()+"] has losts his Script Thread!");
		}
	}
	void Sprite::OnKeyUp(int e)
	{
		//0 means regular lua state
		if( lua_status(this->luaState) == 0 )
		{
			lua_getglobal(this->luaState,"OnKeyUp");
			lua_pushnumber(this->luaState, e);
			lua_call(this->luaState,1,0);
		}
		else
		{
			g_engine->Message("Engine::Sprite ["+this->getName()+"] has losts his Script Thread!");
		}
	}

	void Sprite::OnLoad()
	{
		//0 means regular lua state
		if( lua_status(this->luaState) == 0 )
		{
			lua_getglobal(this->luaState,"OnLoad");
			lua_call(this->luaState,0,0);
		}
		else
		{
			g_engine->Message("Engine::Sprite ["+this->getName()+"] has losts his Script Thread!");
		}
	}
	void Sprite::OnEnterFrame()
	{
		//0 means regular lua state
		if( lua_status(this->luaState) == 0 )
		{
			lua_getglobal(this->luaState,"OnEnterFrame");
			lua_call(this->luaState,0,0);
		}
		else
		{
			g_engine->Message("Engine::Sprite ["+this->getName()+"] has losts his Script Thread!");
		}
	}

	void Sprite::OnMouseMove(int x, int y)
	{
		//0 means regular lua state
		if( lua_status(this->luaState) == 0 )
		{
			lua_getglobal(this->luaState,"OnMouseMove");
			lua_pushnumber(this->luaState, x);
			lua_pushnumber(this->luaState, y);
			lua_call(this->luaState,2,0);
		}
		else
		{
			g_engine->Message("Engine::Sprite ["+this->getName()+"] has losts his Script Thread!");
		}
	}

	void Sprite::LoadFromFile(std::string path)
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

	Sprite::Sprite(std::string path,Layer* parent) : Entity()
	{
		
		this->_parent = parent;
		this->body = cpBodyNew(INFINITY,INFINITY);
		this->isPhysical = false;
		this->isStatic   = true;
		this->image = NULL;
		this->imageLoaded = false;	
		this->setPosition(0.0f,0.0f);	
		this->setVelocity(0.0f,0.0f);
		this->state = 0;
		this->direction = 0;
		this->width = 1;
		this->height = 1;
		this->curframe = 0;
		this->totalframes = 1;
		this->animdir = 1;
		this->animcolumns = 1;
		this->framestart = 0;
		this->frametimer = 0;
		this->animcolumns = 1;
		this->animstartx = 0;
		this->animstarty = 0;
		this->faceAngle = 0;
		this->moveAngle = 0;
		//this->rotation = 0;
		this->xScale = 1.0f;
		this->yScale = 1.0f;
		this->alpha=255;
		this->color = D3DCOLOR_RGBA(255,255,255,(int)this->alpha);
		this->movetimer = 0;
		this->movestart = 0;
		
		this->drowBounds = false;
		this->lineSpace = 5;

		this->luaState = lua_open();
		luaL_openlibs(this->luaState);
		Lua::OpenSohiantLuaLib(this->luaState);

		this->LoadFromFile(path);
	}

	Vector3 Sprite::getV()
	{
		Vector3 out(0.0,0.0,0.0);
		out.setX(this->body->v.x);
		out.setY(this->body->v.y);
		return out;
	}


}