
#include "Engine.h"

#pragma once

namespace Soshiant2D 
{
	enum ShapeTypes {
		CIRCLE,
		LINESEGMENT,
		CONVEXPOLYGON
	};

	class Shape {

	private:
		ShapeTypes type;
		bool visible;
		bool selected;
		bool editMode;

		int  selectedPoint;
		bool isInArea(int baceX, int baceY, float r,int x, int y);

	public:
		cpShape* content;

		Shape();

		ShapeTypes getType()                { return type; }
		void       setType(ShapeTypes type) { this->type = type; }
		void       setVisible(bool value)   { this->visible = value; }
		void       setSelected(bool value)  { this->selected = value; }
		void       setEditeMode(bool value) { this->editMode = value; }

		bool       selectPoint(int x, int y);
		void       updatePoint(int x, int y);
		void       drow();

	};
};