#include "Engine.h"

#pragma once



namespace Soshiant2D {

	class Sprite : public Entity {
	private:
		LPD3DXLINE  line;
		//Vector3 position;
		Vector3 velocity;
		bool imageLoaded;
		int state;
		int direction;

	protected:
		Texture *image;
		Layer *parent;

		cpBody* body;
		std::list<Shape*> shapesList;
		bool isPhysical;
		bool isStatic;


		int width,height;
		int animcolumns;
		int framestart,frametimer;
		int movestart, movetimer;
		int curframe,totalframes,animdir;
		double faceAngle, moveAngle;
		int animstartx, animstarty;
		double /*rotation,*/ xScale,yScale;
		D3DXMATRIX matRotate;
		D3DXMATRIX matScale;
		void transform();
		D3DCOLOR color;

		bool drowBounds;
		float lineSpace;
		float r,g,b,alpha;
		bool paused;

		
	
	public:
		Sprite(Layer *parent);
		virtual ~Sprite();
		bool loadImage(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(255,0,255));
		void setImage(Texture *);
		void move();
		void animate();
		void draw();
		void draw(DWORD blendType);

		//screen position
		//Vector3 getPosition() { return position; }
		//void setPosition(Vector3 position) { this->position = position; }
		void setPosition(double x, double y) {body->p.x = x ;body->p.y = y ;}
		double getX() { return body->p.x - (this->width*this->xScale)/2; }
		double getY() { return body->p.y - (this->height*this->yScale)/2; }
		void setX(double x) { body->p.x = x ;}
		void setY(double y) { body->p.y = y ;}

		//movement velocity
		Vector3 getVelocity() { return velocity; }
		void setVelocity(Vector3 v) { this->velocity = v; }
		void setVelocity(double x, double y) { velocity.setX(x); velocity.setY(y); }
	
		//image size
		void setSize(int width, int height) { this->width = width; this->height = height; }
		int getWidth() { return this->width ; }
		void setWidth(int value) { this->width = value; }
		int getHeight() { return this->height ; }
		void setHeight(int value) { this->height = value; }

		int getState() { return state; }
		void setState(int value) { state = value; }

		int getDirection() { return direction; }
		void setDirection(int value) { direction = value; }

		int getColumns() { return animcolumns; }
		void setColumns(int value) { animcolumns = value; }

		int getFrameTimer() { return frametimer; }
		void setFrameTimer(int value) { frametimer = value; }

		int getCurrentFrame() { return curframe; }
		void setCurrentFrame(int value) { curframe = value; }

		int getTotalFrames() { return totalframes; }
		void setTotalFrames(int value) { totalframes = value; }

		int getAnimationDirection() { return animdir; }
		void setAnimationDirection(int value) { animdir = value; }

		double getRotation() { return body->a ; }
		void setRotation(double value) { body->a = value ; }
		double getXScale() { return xScale; }
		double getYScale() { return yScale; }

		void setScale(double value) { this->xScale=value; this->yScale = value; }
		void setScale(double xScale , double yScale) { this->xScale = xScale; this->yScale = yScale ;}
		void setXScale(double value) { this->xScale=value; }
		void setYScale(double value) { this->yScale = value; }

        //color methods
        D3DCOLOR getColor() { return color; }
		void setColor(D3DCOLOR col) {color = col ;}
		void setColor(float r,float g,float b,float alpha){ this->r =r;this->g=g;this->b=b;this->alpha = alpha;color = D3DCOLOR_RGBA((int)r,(int)g,(int)b,(int)alpha) ;}
		float getR(){return r;}
		float getG(){return g;}
		float getB(){return b;}
		float getAlpha() { return alpha; }
		void setAlpha(float value) { 
			alpha = value;			
			color = color & 0x00ffffff ;
			color += (int)alpha*16777216 ;
		}
		///////////////////////////////////

		int getMoveTimer() { return movetimer; }
		void setMoveTimer(int value) { movetimer = value; }

		void setLineSpace(float lineSpace){ this->lineSpace=lineSpace; }
		float getLineSpace(){ return this->lineSpace; }

		void setSelected(bool choise){ this->drowBounds=choise; }
		bool isDelected(){return this->drowBounds;}

        Rect getBounds();

		Layer *getParent(){ return this->parent ; }
		void setParent(Layer* value){ this->parent = value ; } 

		//Physics Methods
		void setPhysics(float mass, float inertia,bool isStatic);	
		void setIsPhysical(bool value) { this->isPhysical = value ;}
		bool getIsPhysical(){ return this->isPhysical; }
		cpBody* getBody() { return this->body;}
		void    setBody(cpBody* value) { this->body = value;}
		void    clearPhysics();

		void setIsStatic(bool value){ this->isStatic = value ;}
		bool getIsStatic(){ return this->isStatic;}
		void UnselectAllShapes();

		//return Shape ID
		int  addCircle(float radius,float xOffset,float yOffset);
		int  addLineSegment(float ax,float ay,float bx,float by,float radius);
		int  addConvexPolygon(int vertxCnt,cpVect *vertxs , float xOffset , float yOffset);

		Shape* findShape(int id);
		void setShapeVisible(int id , bool value);
		void drowShapes();

		//get shape properties ...
		////////////////void getCircleShapeInfos(int id,float dataCarrier[]);//redius,xpffset,yoffset
		////////////////void getLineSegmentShapeInfos(int id,float dataCarrier[]);//ax,ay,bx,by,tikness
		////////////////void getConvexPolygonShapeInfos(int id,float dataCarrierInfo[],float dataCarrierX[],float dataCarrierY[]);




		void RemoveShape(int id);

		void setMass(float mass)       { this->body->m = mass;    }
		void setInertia(float inertia) { this->body->i = inertia; }
	}; //class
};