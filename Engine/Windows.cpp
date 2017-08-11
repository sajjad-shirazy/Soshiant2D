#include "Engine.h"

namespace Soshiant2D {

	LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		switch( msg )
		{
			 case WM_QUIT:
			 case WM_CLOSE:
			 case WM_DESTROY:
			 gameover = true;
			 break;
	    }
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}


	Windows::Windows( std::string name , int width , int height , int colorDepth , bool isFullscrean )
	{
		this->title = name;
		this->width = width;
		this->height = height;
		this->colorDepth = colorDepth;
		this->isFullscrean = isFullscrean ;
	}

	Windows::~Windows()
	{
		//
	}	

	HWND Windows::Init(HINSTANCE hInstance,int nCmdShow)
	{
		this->hInstance = hInstance;
		this->nCmdShow = nCmdShow;
		srand((unsigned int)time(NULL));
		DWORD dwStyle, dwExStyle;
		RECT windowRect;


		WNDCLASSEX wcex;
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WinProc;
		wcex.cbClsExtra		= NULL;
		wcex.cbWndExtra		= DLGWINDOWEXTRA;
		wcex.hInstance		= this->hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= this->title.c_str();
		wcex.hIconSm		= NULL;

		//set up the window with the class info
		RegisterClassEx(&wcex);

		if (this->isFullscrean) 
		{
		   DEVMODE dm;
		   memset(&dm, 0, sizeof(dm));
		   dm.dmSize = sizeof(dm);
		   dm.dmPelsWidth = this->width;
		   dm.dmPelsHeight = this->height;
		   dm.dmBitsPerPel = this->colorDepth;
		   dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		   if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			  MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			  this->isFullscrean = false ;
			  g_engine->setFullscreen(false);
		   }

			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_APPWINDOW;
			ShowCursor(FALSE);
		}
		else {
			dwStyle = WS_OVERLAPPEDWINDOW;
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}

		//adjust window to true requested size
		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

		//create the program window
		//***modifications
		this->hWnd = CreateWindowEx( 0,
		   this->title.c_str(),								 //window class
		   this->title.c_str(),								 //title bar
		   dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	  
		   0, 0,											 //x,y coordinate
		   this->width,									   //width of the window
		   this->height,									 //height of the window
		   0,											 //parent window
		   0,											 //menu
		   this->hInstance,									  //application instance
		   0);											//window parameters

		//was there an error creating the window?
		if (!this->hWnd) 	{
			MessageBox(this->hWnd, "Engine::Error creating program window!", "Error", MB_OK);
			return 0; 
		}
		return this->hWnd;
	}

	void Windows::ProcessMessages()
	{
		while (PeekMessage(&this->msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Windows::Show()
	{
		if(this->hWnd)
		{
			//display the window
			ShowWindow(this->hWnd, this->nCmdShow);
			UpdateWindow(this->hWnd);
		}
	}

	void Windows::setTitle(std::string value) 
	{
		SetWindowText(this->hWnd,value.c_str());
	}
	
}