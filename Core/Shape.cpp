#include "Engine.h"

#define CIRCLEDETAIL 14

namespace Soshiant2D {

	Shape::Shape()
	{
		this->selectedPoint = -1;
		this->selected = false ;
		this->visible = false ;
		this->editMode = false;
	}

	void Shape::drow()
	{
		if( this->visible)
		{
			D3DCOLOR color;
			if(this->selected)
				color=D3DCOLOR_XRGB(255,0,0);
			else
				color=D3DCOLOR_XRGB(0,255,0);

			double parentX = 0;
			double parentY = 0;

			if(this->content->data != NULL)
			{
				Sprite* parent= (Sprite*) this->content->data ;
				parentX = parent->getBody()->p.x ;
				parentY = parent->getBody()->p.y ;
			}


			if( this->type == LINESEGMENT)
			{
				cpSegmentShape *seg = (cpSegmentShape *)this->content;
				cpVect A = seg->a;
				A.x += editor_x_base + parentX ;
				A.y += editor_y_base + parentY ;
				cpVect B = seg->b;
				B.x +=editor_x_base + parentX  ;
				B.y +=editor_y_base + parentY  ;

				D3DXVECTOR2 lines[] = {D3DXVECTOR2( (float)A.x, (float)A.y ), D3DXVECTOR2((float)B.x,(float)B.y)};
				g_engine->getLineHandler()->Draw(lines , 2 , color);

				if(this->editMode)
				{
					//g_engine->getLineHandler()->Draw(getTriangle(editor_x_base+A.x ,editor_y_base + A.y ,15) , 4 , D3DCOLOR_XRGB(0,0,255));
					//g_engine->getLineHandler()->Draw(getTriangle(editor_x_base+B.x ,editor_y_base + B.y ,15) , 4 , D3DCOLOR_XRGB(0,0,255));
					g_engine->getLineHandler()->Draw(getSquare((float)A.x, (float)A.y ,12,12) , 5 , D3DCOLOR_XRGB(0,0,255));
					g_engine->getLineHandler()->Draw(getSquare((float)B.x, (float)B.y ,12,12) , 5 , D3DCOLOR_XRGB(0,0,255));
				}
				g_engine->getLineHandler()->Draw(getSquare((float)A.x,(float)A.y ,9,9) , 5 , D3DCOLOR_XRGB(0,0,255));
				g_engine->getLineHandler()->Draw(getSquare((float)B.x,(float)B.y ,9,9) , 5 , D3DCOLOR_XRGB(0,0,255));

				//drawing thicness
				if(seg->r > 0)
				{
					float rot  = (float)atan((B.y-A.y)/(B.x-A.x)) ;
					lines[0].x = (float)(A.x + cos(PI/2-rot)*seg->r );
					lines[0].y = (float)(A.y - sin(PI/2-rot)*seg->r );
					lines[1].x = (float)(B.x + cos(PI/2-rot)*seg->r );
					lines[1].y = (float)(B.y - sin(PI/2-rot)*seg->r );
					g_engine->getLineHandler()->Draw(lines , 2 , D3DCOLOR_XRGB(0,240,250));

					lines[0].x = (float)(A.x + cos(3*PI/2-rot)*seg->r );
					lines[0].y = (float)(A.y - sin(3*PI/2-rot)*seg->r );
					lines[1].x = (float)(B.x + cos(3*PI/2-rot)*seg->r );
					lines[1].y = (float)(B.y - sin(3*PI/2-rot)*seg->r );
					g_engine->getLineHandler()->Draw(lines , 2 , D3DCOLOR_XRGB(0,240,250));
				}
			}
			else if( this->type == CIRCLE )
			{
				cpCircleShape *cir = (cpCircleShape *) this->content;

				cpVect P = cir->c ;
				P.x += editor_x_base + parentX ;
				P.y += editor_y_base + parentY ;
				float  R = (float)cir->r ;

				D3DXVECTOR2 lines[15];
				int i=0;
				for(float angle=0; angle<=2*3.14; angle+=(float)(2*3.14/14))
				{
					lines[i++] = D3DXVECTOR2((float)(cos(angle)*R+P.x),(float)(sin(angle)*R+P.y));
					//MessageBox(0,::ToString(lines[i-1].x).c_str(),0,0);
				}
				g_engine->getLineHandler()->Draw(lines , 15 , color);

				if(this->editMode)
				{
					//g_engine->getLineHandler()->Draw(getTriangle(editor_x_base+P.x ,editor_y_base + P.y ,15) , 4 , D3DCOLOR_XRGB(0,0,255));
					g_engine->getLineHandler()->SetWidth(5);
					g_engine->getLineHandler()->Draw(getSquare((float)P.x, (float)P.y,12,12) , 5 , D3DCOLOR_XRGB(0,0,255));

				}
				g_engine->getLineHandler()->Draw(getSquare((float)P.x ,(float)P.y,9,9) , 5 , D3DCOLOR_XRGB(0,0,255));


			}
		}
	}

	bool  Shape::selectPoint(int x, int y)
	{
		if( this->type == LINESEGMENT )
		{
			cpSegmentShape *seg = (cpSegmentShape *)this->content;
			cpVect A = seg->a;
			cpVect B = seg->b;

			if(this->isInArea((int)A.x,(int)A.y,6,x,y))
			{
				this->selectedPoint = 0;
				return true;
			}
			if(this->isInArea((int)B.x,(int)B.y,6,x,y))
			{
				this->selectedPoint = 1;
				return true;
			}
		}
		selectedPoint=-1;
		return false;
	}

	void Shape::updatePoint(int x, int y)
	{
		if(selectedPoint!= -1)
		{
			if(this->type == LINESEGMENT)
			{
				cpSegmentShape *seg = (cpSegmentShape *)this->content;
				if(this->selectedPoint == 0)
				{
					seg->a.x = x;
					seg->a.y = y;
				}
				else
				{
					seg->b.x = x;
					seg->b.y = y;
				}
			}
		}
	}

	bool Shape::isInArea(int baceX, int baceY, float r,int x, int y)
	{
		D3DXVECTOR2 res((float)(x-baceX),(float)(y-baceY));

		if( D3DXVec2Length(&res) < r )
			return true;
		else
			return false;

	}
	
};