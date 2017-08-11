#include "Engine.h"

#pragma once 

namespace Soshiant2D 
{
	class Windows
	{
		std::string title;
		int width;
		int height;
		int colorDepth;
		bool isFullscrean;

		HINSTANCE hInstance;		
		HWND      hWnd;
		MSG       msg;
		int       nCmdShow;

	public:
		Windows( std::string name , int width , int height , int colorDepth , bool isFullscrean );
		~Windows();

		HWND getHandle(){ return hWnd; }

		std::string getTitle(){return this->title;}
		void setTitle(std::string value) ;

		HWND Init(HINSTANCE hInstance,int nCmdShow);
		void ProcessMessages();
		void Show();
	};
}
