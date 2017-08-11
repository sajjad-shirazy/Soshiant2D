#include "Engine.h"

using namespace Soshiant2D;

namespace Soshiant2D {

	Sprite::Sprite(Layer *parent) : Entity()
	{
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

		//this->collidable = true;
		//this->collisionMethod = COLLISION_RECT;
		
		this->drowBounds = false;
		this->lineSpace = 5;

		this->parent=parent;
	}
	
	Sprite::~Sprite()  {

		clearPhysics();

		if (imageLoaded)
			delete image;
		if(line)
			line->Release();
	}
	
	bool Sprite::loadImage(std::string filename, D3DCOLOR transcolor)
	{
		if (image != NULL) delete image;
	
		image = new Texture();
		//g_engine->Message(Path::getStartupPath()+"\\");
		if (image->Load(Path::getStartupPath()+"\\"+filename,transcolor))
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
		D3DXVECTOR2 trans((float)(getX()+parent->getPosition().getX()+editor_x_base), (float)(getY()+parent->getPosition().getY()+editor_y_base));
		D3DXMatrixTransformation2D(&mat,NULL,0,&scale,&center,(float)body->a,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);
	}

	void Sprite::draw()
	{
		int fx = (this->curframe % this->animcolumns) * this->width;
		int fy = (this->curframe / this->animcolumns) * this->height;
		RECT srcRect = {fx,fy, fx+this->width, fy+this->height};

		this->transform();

		g_engine->getSpriteHandler()->Draw(this->image->GetTexture(),&srcRect,NULL,NULL,color);

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

		g_engine->getDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_engine->getDevice()->SetRenderState( D3DRS_DESTBLEND, blendType );
		g_engine->getSpriteHandler()->Draw(this->image->GetTexture(),&srcRect,NULL,NULL,color);
		//g_engine->getDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

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

    Rect Sprite::getBounds()
    {
    	Rect rect;
        rect.left    = getX() + parent->getPosition().getX() + editor_x_base;
		rect.top     = getY() + parent->getPosition().getY() + editor_y_base;
		rect.right   = getX() + parent->getPosition().getX() + editor_x_base + getWidth()  * getXScale();
		rect.bottom  = getY() + parent->getPosition().getY() + editor_y_base + getHeight() * getYScale();
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
				this->body = cpBodyNew(mass, inertia);
				this->body->p = cpv(x, y);
				g_engine->getPhysicsHandler()->RegisterBody( this->body );
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
			g_engine->getPhysicsHandler()->RemoveShape((*i)->content);
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
				g_engine->getPhysicsHandler()->RemoveShape((*i)->content);
				this->shapesList.erase(i);
				break;
			}
			i++;
		}
	}

	int Sprite::addCircle(float radius , float xOffset , float yOffset)
	{
		if(this->isPhysical)
		{
			cpVect offset;
			offset.x = xOffset;
			offset.y = yOffset;

			Shape *shape=new Shape();
			shape->setType(ShapeTypes::CIRCLE);

			shape->content = cpCircleShapeNew ( this->body , radius , offset);

			shape->content->data = this;

			this->shapesList.push_back( shape );

			if(this->isStatic)
				g_engine->getPhysicsHandler()->RegisterStaticShape(shape->content);
			else
				g_engine->getPhysicsHandler()->RegisterShape(shape->content);

			

			return shape->content->id;
		}
		else
			g_engine->Message("Adding shape to a unPhysical Sprite ...");
		return -1;
	}
	
	int  Sprite::addLineSegment(float ax,float ay,float bx,float by,float radius)
	{
		if(this->isPhysical)
		{
			cpVect a,b;
			a.x = ax; a.y = ay;
			b.x = bx; b.y = by;
			

			Shape *shape=new Shape();
			shape->setType(ShapeTypes::LINESEGMENT);
			//Beep( 750, 50 );
			shape->content = cpSegmentShapeNew(this->body,a,b,radius);
			shape->content->data = this;
			//Beep( 500, 50 );


			if(this->isStatic)
				g_engine->getPhysicsHandler()->RegisterStaticShape(shape->content);
			else
				g_engine->getPhysicsHandler()->RegisterShape(shape->content);		


			this->shapesList.push_back( shape );
			return shape->content->id;
		}
		else
			g_engine->Message("Adding shape to a unPhysical Sprite ...");
		return -1;
	}

	int  Sprite::addConvexPolygon(int vertxCnt,cpVect *vertxs , float xOffset , float yOffset)
	{
		if(this->isPhysical)
		{
			cpVect offset;
			offset.x = xOffset;
			offset.y = yOffset;

			Shape *shape = new Shape();
			shape->setType(ShapeTypes::CONVEXPOLYGON);

			shape->content = cpPolyShapeNew(this->body,vertxCnt,vertxs,offset);
			shape->content->data = this;

			this->shapesList.push_back( shape );

			if(this->isStatic)
				g_engine->getPhysicsHandler()->RegisterStaticShape(shape->content);
			else
				g_engine->getPhysicsHandler()->RegisterShape(shape->content);

			return shape->content->id;
		}
		else
			g_engine->Message("Adding shape to a unPhysical Sprite ...");
		return -1;
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


}