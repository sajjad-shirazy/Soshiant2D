//Helper Functions an Classes
#include "Engine.h"

#pragma once


#define PI 3.141592 

//integer to String ...
std::string ToString(double input);
D3DXVECTOR2 *getSquare(float x,float y,float width,float height);
D3DXVECTOR2 *getTriangle(float x,float y,float l);

namespace Color
{
	int getR(D3DCOLOR color);
	int getG(D3DCOLOR color);
	int getB(D3DCOLOR color);
	int getA(D3DCOLOR color);

}
namespace Path
{
	
	std::string getStartupPath();
	std::string getParentDirectory(std::string path);
	std::string getFullPath(std::string path);
	std::string getName(std::string path);
}
namespace Math
{
	float random();
}