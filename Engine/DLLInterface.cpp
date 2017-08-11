#include "DllInterface.h"
#include "Engine.h"

using namespace Soshiant2D;


Texture *part2;

Soshiant2D::Engine *g_engine;

//editor Types
int selected_sprite_ID = -1;
int base_selection_x = 0;
int base_selection_y = 0;

Sprite* selected_sprite;
Shape*  active_shape;

double editor_x_base = 0;
double editor_y_base = 0;
double edirot_scale_base = 1;

void test();
Sprite *circle;
cpBody *ballBody;
bool gameover;


void InitEngine( HWND hWnd , int width,int height,int depth,bool fullscreen )
{
	CloseEngine();

	g_engine = new Soshiant2D::Engine();

	g_engine->setWindowHandle(hWnd);
	g_engine->setScreenWidth(width);
	g_engine->setScreenHeight(height);
	g_engine->setColorDepth(depth);
	g_engine->setFullscreen(fullscreen);

	if (!g_engine->Init(g_engine->getScreenWidth(), g_engine->getScreenHeight(), g_engine->getColorDepth(), g_engine->getFullscreen())) 	{
		MessageBox(hWnd, "Error initializing the engine", "Error", MB_OK);
	}
	//Sprite *asteroid;
	//Texture *image = new Texture();
	//image->Load("c:\\asteroid.tga");
	//for (int n=0; n < 10; n++) {
	//	//create a new asteroid sprite
	//	asteroid = new Sprite(g_engine->getAvtiveLayer());
	//	//asteroid->setObjectType(OBJECT_SPRITE);
	//	asteroid->setImage(image);
	//	asteroid->setTotalFrames(64);
	//	asteroid->setColumns(8);
	//	asteroid->setSize(60,60);
	//	asteroid->setPosition( rand() % 700, rand() % 500 );
	//	asteroid->setFrameTimer( rand() % 50);
	//	asteroid->setCurrentFrame( rand() % 64 );
	//	if (rand()%2==0) asteroid->setAnimationDirection(-1);
	//	//add sprite to the entity manager
	//	g_engine->addEntity(asteroid);
	//}	

	//g_engine->addLayer("bace0",Soshiant2D::Vector3(0,0,0));
	//g_engine->setActiveLayer(0);
	//
	//Sprite *grnd = new Sprite(g_engine->getAvtiveLayer());
	//grnd->loadImage("c:\\gr.png");
	//grnd->setPosition( 300, 500);
	////grnd->setIsStatic(true);
	////grnd->setPhysics(INFINITY,INFINITY);	
	////grnd->findShape(grnd->addLineSegment(0,0,grnd->getWidth(),0,0.0f))->setVisible(true);
	//g_engine->addEntity(grnd);

	//circle = new Sprite(g_engine->getAvtiveLayer());
	//circle->loadImage("c:\\ball.png");
	//circle->setPosition( 200, 100);
	//g_engine->addEntity(circle);
	//
	//
	//circle->setPhysics(1000.0,1,false);
	//g_engine->getPhysicsHandler()->RegisterShape(circle->findShape(circle->addCircle(32.0,0,0))->content);

	//grnd->setPhysics(0,0,true);
	//g_engine->getPhysicsHandler()->RegisterStaticShape(grnd->findShape(grnd->addLineSegment(-300,0,300,20,0.0f))->content);
	part2 = new Texture();
	part2->Load("part.png");

}

void ResizeEngine(int width,int height)
{
	g_engine->ResizeDevice(g_engine->getWindowHandle(),width, height, 32, false);
}
void Render()
{
	g_engine->Update();
}

void CloseEngine()
{	
	
	if(g_engine!=NULL)
		delete g_engine;
}

int GetRealFPS()
{
	return (int)g_engine->getFrameRate_real();
}
int GetCoreFPS()
{
	return (int)g_engine->getFrameRate_core();
}

void SetBackColor(int R,int G, int B)
{
	g_engine->SetBackColor(D3DCOLOR_XRGB(R,G,B));
}

int AddSprite(char* name,double x,double y,double scale,double rotation,int alpha,int row,int col,int totalFrame,int width,int height,int anim_timer,int isRev,double xmov,double ymov,int mov_timer,char* imagePath,bool isPhysical,bool isStatic,float mass,float inertia)
{
	if(g_engine->getAvtiveLayer()!=NULL)
	{
		Sprite *sprite=new Sprite(g_engine->getAvtiveLayer());
		sprite->setName(name);
		if(!sprite->loadImage(imagePath))
			g_engine->Message("Engine : Error in Loading Texture");
		sprite->setTotalFrames(totalFrame);
		sprite->setColumns(col);
		if(totalFrame>1)
			sprite->setSize(width,height);

	    //MessageBox(0,name,0,0);
		sprite->setRotation(rotation);
		sprite->setScale(scale);
		sprite->setAlpha(alpha);
		sprite->setVelocity(xmov,ymov);
		sprite->setPosition( x, y );
		sprite->setFrameTimer( anim_timer);
		sprite->setAnimationDirection(isRev);
		sprite->setMoveTimer(mov_timer);

		if(isPhysical)
		{
			if(isStatic)
				sprite->setPhysics(0,0,true);
			else
			{
				sprite->setPhysics(mass,inertia,false);
			}
		}



		//add sprite to the entity manager
		g_engine->addEntity(sprite);
		return sprite->getID();
	}
	return -1;

}

int SelectSprite(int x,int y)
{
	selected_sprite_ID = -1;
	selected_sprite = g_engine->getAvtiveLayer()->SelectSprite(x,y);	
	if(selected_sprite)
	{
		selected_sprite_ID = selected_sprite->getID();
		base_selection_x = x - (int)selected_sprite->getX();
		base_selection_y = y - (int)selected_sprite->getY();
	}
	return selected_sprite_ID;
}

bool IsSelectedSprite()
{
	
	if(selected_sprite_ID == -1)
	{
		MessageBox(0,"test is false !","",0);
		return false;
	}
	else
	{
		MessageBox(0,"test is true !","",0);
		return true;
	}
}

void UpdateSprite(int ID,int x,int y)
{
	selected_sprite->setX(x-base_selection_x+selected_sprite->getWidth()/2);
	selected_sprite->setY(y-base_selection_y+selected_sprite->getHeight()/2);
}

void ResetSprite(int ID,char* name,double x,double y,double scale,double rotation,int alpha,int width,int height,int row,int col,int totalFrame,int isRev,int anim_timer,double xmov,double ymov,int mov_timer , bool isPhysical , bool isStatic , float mass , float inertia)
{
	Sprite* sprite=(Sprite*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	
	sprite->setName(name);
	sprite->setScale(scale);
	sprite->setRotation(rotation);
	sprite->setAlpha(alpha);
	sprite->setColumns(col);
	sprite->setTotalFrames(totalFrame);
	if(width > 0)
		sprite->setSize(width,height);
	sprite->setVelocity(xmov,ymov);
	sprite->setPosition( x, y );
	sprite->setFrameTimer( anim_timer);
	sprite->setAnimationDirection(isRev);
	sprite->setMoveTimer(mov_timer);

	sprite->setIsStatic(isStatic);
	sprite->setIsPhysical(isPhysical);
}

void ResetSpriteImage(int ID,char* imagePath)
{
	Sprite* sprite=(Sprite*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	if(!sprite->loadImage(imagePath))
			g_engine->Message("Engine : Error in Loading Texture");
}

void SetPause(bool choice)
{
	g_engine->setPaused(choice);
}

int  GetSpriteWidth(int ID)
{
	Sprite* sprite=(Sprite*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	return sprite->getWidth();
}
int  GetSpriteHeight(int ID)
{
	Sprite* sprite=(Sprite*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	return sprite->getHeight();
}

void DeleteSprite(int ID)
{
	g_engine->getAvtiveLayer()->removeEntity(ID);
}

void SetSelectedSprite(int ID)
{
	if(selected_sprite)
		selected_sprite->setSelected(false);

	selected_sprite = (Sprite*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	if(selected_sprite)
	{
		selected_sprite->setSelected(true);
		selected_sprite_ID = selected_sprite->getID();
	}
}

int  AddLayer(char* name,double z)
{
	Vector3 position(0.0,0.0,z);
	return g_engine->addLayer(name,position);
}

void SetActivLayer(int ID)
{
	//if id==1- we have not any activ layer
	if(g_engine->getAvtiveLayer()!= NULL)
		g_engine->getAvtiveLayer()->UnSelectAll();
	g_engine->setActiveLayer(ID);
	

	//test();
	/*std::stringstream k;
	k<<"Active Layer Changed to Layer With this ID: ";
	k<<ID;
	MessageBox(0,k.str().c_str(),0,0);*/
}

void SetFocusLayer(int ID)
{
	g_engine->setFocusLayer(ID);
	/*std::stringstream k;
	k<<"Active Layer Changed to Layer With this ID: ";
	k<<ID;
	MessageBox(0,k.str().c_str(),0,0);*/
}

void SetPivotLayer(int ID)
{
	g_engine->setPivotLayer(ID);
	/*std::stringstream k;
	k<<"Active Layer Changed to Layer With this ID: ";
	k<<ID;
	MessageBox(0,k.str().c_str(),0,0);*/
}

void ResetLayersPosition()
{
	Layer *layer;
	Vector3 position0 ;
	std::list<Layer*>::iterator i = g_engine->getLayersList().begin();
	while (i != g_engine->getLayersList().end())
	{
		layer = *i;
		position0 = layer->getPosition();
		position0.setX(0.0);
		position0.setY(0.0);
		layer->setPosition(position0);
		++i;
	}
}

void ResetLayers(char* name,double z)
{
	g_engine->getAvtiveLayer()->setName(name);
	g_engine->getAvtiveLayer()->setPosition(0,0,z);
}

void EditorMouseDown(int x,int y)
{
	base_selection_x = x;
    base_selection_y = y;

	//Sprite *asteroid;
	//Texture *image = new Texture();
	//image->Load("c:\\asteroid.tga");
	////create a new asteroid sprite
	//asteroid = new Sprite(g_engine->getAvtiveLayer());
	////asteroid->setObjectType(OBJECT_SPRITE);
	//asteroid->setImage(image);
	//asteroid->setTotalFrames(64);
	//asteroid->setColumns(8);
	//asteroid->setSize(60,60);
	//asteroid->setPosition( x, y );
	//asteroid->setFrameTimer( rand() % 50);
	//asteroid->setCurrentFrame( rand() % 64 );
	//if (rand()%2==0) asteroid->setAnimationDirection(-1);
	////add sprite to the entity manager
	//g_engine->addEntity(asteroid);

	//asteroid->setBody(cpBodyNew(100, 0.9));
	//cpShape *ballShape = cpCircleShapeNew(asteroid->getBody(), 20.0, cpvzero);

	//ballShape->e = 0.5; // Elasticity

	//ballShape->u = 0.8; // Friction

	//ballShape->collision_type = 1; // Collisions are grouped by types

	//// Add the shape to out space

	//if(space!=NULL)
	//	cpSpaceAddShape(space, ballShape);
}

void UpdateEditorPositionBase(int x,int y)
{
	editor_x_base -= base_selection_x - x;
    editor_y_base -= base_selection_y - y;
	base_selection_x = x;
    base_selection_y = y;
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//Shape Editing

void setShapeVisible(int id , bool value)
{
	selected_sprite->setShapeVisible( id , value );
}

int AddLineShape(float ax , float ay , float bx , float by ,float r)
{
	if( selected_sprite != NULL )
	{
		return selected_sprite->addLineSegment( ax , ay , bx , by , r);
	}

	MessageBox(0,"Engine :: no selected_sprite",0,0);
	return -1;
}

void setActiveShape(int id)
{
	selected_sprite->UnselectAllShapes();
	active_shape = selected_sprite->findShape(id);
	active_shape->setSelected(true);
}

void ResetLineShape(float ax , float ay , float bx , float by ,float r)
{
	cpSegmentShape *seg = (cpSegmentShape *) active_shape->content ;
	seg->a.x=ax;
	seg->a.y=ay;
	seg->b.x=bx;
	seg->b.y=by;

	seg->r = r;
}

int  AddCircleShape(float x , float y , float r )
{
	if( selected_sprite != NULL )
	{
		if(selected_sprite->getIsStatic())
			return selected_sprite->addCircle(abs(r),x,y);
		else
			return selected_sprite->addCircle(abs(r),0,0);
	}

	MessageBox(0,"Engine :: no selected_sprite",0,0);
	return -1;
}
void ResetCircleShape(float x , float y , float r )
{
	cpCircleShape *cir = (cpCircleShape *) active_shape->content ;
	if(selected_sprite->getIsStatic())
	{
		cir->c.x = x;
		cir->c.y = y;
	}
	else
	{
		cir->c.x = 0;
		cir->c.y = 0;
	}
	cir->r = abs(r);
}

void RemoveShape(int id)
{
	selected_sprite->RemoveShape(id);
}

void SwitchEditeMode(bool value)
{
	if(value)
	{
		selected_sprite->UnselectAllShapes();
		active_shape->setEditeMode(true);
		active_shape->setSelected(true);
	}
	else
		active_shape->setEditeMode(false);

}

bool SelectPoint(int x,int y)
{
	if(active_shape->selectPoint( x - editor_x_base , y - editor_y_base ))
		return true;
	else
		return false;
}

void UpdatePoint(int x,int y)
{
	active_shape->updatePoint(x- editor_x_base,y- editor_y_base);
}

void  GetLineProperties(int id , float* carry)
{
	if( selected_sprite != NULL )
	{	
		cpSegmentShape *seg = (cpSegmentShape *) selected_sprite->findShape(id)->content ;
		carry[0] = seg->a.x;
		carry[1] = seg->a.y;
		carry[2] = seg->b.x;
		carry[3] = seg->b.y;
		carry[4] = seg->r;

		return;
	}

	MessageBox(0,"Engine :: no selected_sprite",0,0);
}

void GetCircleProperties(int id , float* carry)
{
	cpCircleShape *cir = (cpCircleShape *) selected_sprite->findShape(id)->content;
	carry[0]=cir->c.x ;
	carry[1]=cir->c.y ;
	carry[2]=cir->r ;
}


void test()
{
	//Sprite *asteroid;
	//Texture *image = new Texture();
	//image->Load("c:\\gr.png");
	////create a new asteroid sprite
	//asteroid = new Sprite(g_engine->getAvtiveLayer());
	////asteroid->setObjectType(OBJECT_SPRITE);
	//asteroid->setImage(image);
	//asteroid->setPosition( 0, 500);
	////add sprite to the entity manager
	//g_engine->addEntity(asteroid);

	//asteroid->setBody(cpBodyNew(INFINITY, INFINITY));

	//asteroid->getBody()->p.x=0;
	//asteroid->getBody()->p.y=500;

	//cpResetShapeIdCounter();
	//
	//space = cpSpaceNew();
	//cpSpaceResizeActiveHash(space, 30.0, 999);
	//cpSpaceResizeStaticHash(space, 200.0, 99);
	//space->gravity = cpv(0, 600);

	//cpVect a = cpv(0, 0);
	//cpVect b = cpv(0,  500);

	//cpShape* shape = cpSegmentShapeNew(asteroid->getBody(), a, b, 0.0f);
	//shape->e = 1.0; shape->u = 1.0;
	//shape->collision_type=0;
	//cpSpaceAddStaticShape(space, shape);
}

int  AddParticleSystem(char* name,double x,double y,int count,int mnpl,int mxpl,int isRep,int scr,int scg,int scb,int sa,int ecr,int ecg,int ecb,int ea,float mnScale,float mxScale,float acc_x,float acc_y,float grv_x,float grv_y,float mnVel,float mxVel,char* imagePath)
{
	ParticleSystem* ps = new ParticleSystem(g_engine->getAvtiveLayer());
	ps->setName(name);
	ps->setPosition(x,y);
	ps->setMinParticleLifeTime(mnpl);
	ps->setMaxParticleLifeTime(mxpl);
	ps->setRepeat(isRep);
	ps->setStartColor(scr,scg,scb);
	ps->setEndColor(ecr,ecg,ecb);
	ps->setStartAlpha(sa);
	ps->setEndAlpha(ea);
	ps->setMinScale(mnScale);
	ps->setMaxScale(mxScale);
	ps->setAcceleration(Vector3(acc_x,acc_y,0.0));
	ps->setGravity(Vector3(grv_x,grv_y,0.0));
	ps->setMinStartSpeed(mnVel);
	ps->setMaxStartSpeed(mxVel);
	ps->loadImage(imagePath);
	ps->init(count);
	g_engine->addEntity(ps);
	return ps->getID();
}
void ResetParticleSystem(int ID,char* name,double x,double y,int count,int mnpl,int mxpl,int isRep,int scr,int scg,int scb,int sa,int ecr,int ecg,int ecb,int ea,float mnScale,float mxScale,float acc_x,float acc_y,float grv_x,float grv_y,float mnVel,float mxVel,char* imagePath)
{
	ParticleSystem* ps=(ParticleSystem*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	
	if(ps!=0)
	{
		ps->setName(name);
		ps->setPosition(x,y);
		ps->setMinParticleLifeTime(mnpl);
		ps->setMaxParticleLifeTime(mxpl);
		ps->setRepeat(isRep);
		ps->setStartColor(scr,scg,scb);
		ps->setEndColor(ecr,ecg,ecb);
		ps->setStartAlpha(sa);
		ps->setEndAlpha(ea);
		ps->setMinScale(mnScale);
		ps->setMaxScale(mxScale);
		ps->setAcceleration(Vector3(acc_x,acc_y,0.0));
		ps->setGravity(Vector3(grv_x,grv_y,0.0));
		ps->setMinStartSpeed(mnVel);
		ps->setMaxStartSpeed(mxVel);
		ps->recount(count);
	}
}
void ResetParticleSystemImage(int ID,char* imagePath)
{
	ParticleSystem* ps=(ParticleSystem*)g_engine->getAvtiveLayer()->findEntity_byID(ID);
	
	if(ps!=0)
	{
		ps->loadImage(imagePath);
		ps->resetImage();
	}
}





















// Defines the entry point for the DLL application.
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}