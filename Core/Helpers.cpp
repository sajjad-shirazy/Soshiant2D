#include "Helpers.h"

//Helper Functions an Classes
std::string ToString(double input)
{
	std::stringstream output;
	output<<input;
	return output.str();
}

D3DXVECTOR2 *getSquare(float x,float y,float width,float height)
{
	D3DXVECTOR2 lines[] = {D3DXVECTOR2(x-width/2,y-height/2 ), D3DXVECTOR2(x+width/2,y-height/2), D3DXVECTOR2(x+width/2,y+height/2), D3DXVECTOR2(x-width/2,y+height/2),D3DXVECTOR2(x-width/2,y-height/2 )};
	return lines;
}

D3DXVECTOR2 *getTriangle(float x,float y,float l)
{
	D3DXVECTOR2 lines[] = {D3DXVECTOR2(x,y-1/2 ), D3DXVECTOR2(x+l/2,y+l/2), D3DXVECTOR2(x-l/2,y+l/2),D3DXVECTOR2(x,y-1/2 )};
	return lines;
}

namespace Path{
	//returns Parent Directory path without
	std::string getParentDirectory(std::string path)
	{
		std::string output ;
		int i = path.length() - 1;
		while(path[i] !='\\')
			i--;
		return path.substr(0,i) ;		 
	}
	
	std::string getStartupPath()
	{
		 char buffer[1024];
		 GetCurrentDirectory(1024,buffer);
		 //std::string output = buffer;
		 return buffer;
	}

	std::string getFullPath(std::string path)
	{
		return getStartupPath()+"\\"+path;
	}
	std::string getName(std::string path)
	{
		int i = path.size()-1;
		while(path[i]!='\\')
			i--;
		return path.substr(i+1,path.size()-1);
	}
}
namespace Color
{
	int getR(D3DCOLOR color)
	{
		DWORD output = color & 0x00ff0000 ;
		output /=65536 ;
		return (int)output;
	}
	int getG(D3DCOLOR color)
	{
		DWORD output = color & 0x0000ff00 ;
		output /=256 ;
		return (int)output;
	}
	int getB(D3DCOLOR color)
	{
		DWORD output = color & 0x000000ff ;
		return (int)output;
	}
	int getA(D3DCOLOR color)
	{
		DWORD output = color & 0xff000000 ;
		output /=16777216 ;
		return (int)output;
	}
}

namespace Math
{
	float random()
	{
		float output = (float)(rand()% 1000);
		return output/1000;
	}
}
