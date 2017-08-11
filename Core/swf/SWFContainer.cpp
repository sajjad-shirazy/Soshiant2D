#include "Engine.h"

SwfContainer::SwfContainer()
{
	//
}
bool SwfContainer::Init(IDirect3DDevice9 * m_pDevice,HWND parentHandle, int Width,int Height,char * Path)
{	
	HRESULT hr = S_OK;

	_width = Width;
	_height = Height;

	//create the system memory texture to hold the pixel buffer
	hr = m_pDevice->CreateTexture(Width,					// width
									Height,					// height
									1,						// levels
									D3DUSAGE_DYNAMIC,		// usage
									D3DFMT_X8R8G8B8,        // format
									D3DPOOL_SYSTEMMEM, 
									&memtex,
									NULL);

	//create Window and Attach Flash control
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = "FlashViewer";
	RegisterClassEx(&wc);
	this->windowless = CreateWindowEx(0, "FlashViewer", "FlashViewer", WS_POPUP
		, 0, 0, this->_width, this->_height, parentHandle, NULL, NULL, 0);
	if(!this->windowless)
		return false;
	hr = CoCreateInstance(__uuidof(ShockwaveFlash),0,CLSCTX_ALL, __uuidof(IShockwaveFlash), (void **)&this->iflash);

	//set up event notifications
	hr = this->iflash->QueryInterface(IID_IConnectionPointContainer, (void**)&this->m_lpConCont);
	if (FAILED(hr))
		return false;
	hr = this->m_lpConCont->FindConnectionPoint(DIID__IShockwaveFlashEvents, &this->m_lpConPoint);
	if (FAILED(hr))
		return false;
	hr = this->m_lpConPoint->Advise(0, &this->m_dwConPointID);
	if (FAILED(hr))
		return false;

	//We went through all this trouble, make the little prick give us the Alpha Channel
	hr = this->iflash->put_WMode(L"transparent");
	//hr = AtlAxAttachControl(this->iflash,this->windowless,0);
	hr = AtlAxAttachControl(this->iflash,this->windowless,0);

	//get the view object
	hr = this->iflash->QueryInterface(__uuidof(IViewObject),(void **)&this->viewobject);

	//create stream to Marshal view object into render thread
	this->pStream = NULL;
	hr = CoMarshalInterThreadInterfaceInStream(__uuidof(IViewObject), this->viewobject, &this->pStream);

	//sanity check
	this->RTviewobject = NULL;
	hr = this->iflash->put_Loop(true);
	//convert Path to WideChar	
	MultiByteToWideChar(CP_ACP,0,Path,-1,(LPWSTR)real_name,256);
	hr = this->iflash->put_Movie((BSTR)this->real_name);
	hr = this->iflash->Play();
	return 0;
}

//DWORD WINAPI LoadSWF (LPVOID data) throw()
//int SwfContainer::Load()
//{
//
//	HRESULT hr = S_OK;
//
//	//create Window and Attach Flash control
//	this->windowless = CreateWindowEx(0,0,0,WS_POPUP,0,0,this->_width,this->_height,0,0,0,0);
//	hr = CoCreateInstance(__uuidof(ShockwaveFlash),0,CLSCTX_ALL, __uuidof(IShockwaveFlash), (void **)&this->iflash);
//
//	//set up event notifications
//	hr = this->iflash->QueryInterface(IID_IConnectionPointContainer, (void**)&this->m_lpConCont);
//	if (FAILED(hr))
//		return -1;
//	hr = this->m_lpConCont->FindConnectionPoint(DIID__IShockwaveFlashEvents, &this->m_lpConPoint);
//	if (FAILED(hr))
//		return -1;
//	hr = this->m_lpConPoint->Advise((_IShockwaveFlashEvents *)this, &this->m_dwConPointID);
//	if (FAILED(hr))
//		return -1;
//
//	//We went through all this trouble, make the little prick give us the Alpha Channel
//	hr = this->iflash->put_WMode(L"transparent");
//	//hr = AtlAxAttachControl(this->iflash,this->windowless,0);
//	hr = AtlAxAttachControl(this->iflash,g_engine->getWindowHandle(),0);
//
//	//get the view object
//	hr = this->iflash->QueryInterface(__uuidof(IViewObject),(void **)&this->viewobject);
//
//	//create stream to Marshal view object into render thread
//	this->pStream = NULL;
//	hr = CoMarshalInterThreadInterfaceInStream(__uuidof(IViewObject), this->viewobject, &this->pStream);
//
//	//sanity check
//	this->RTviewobject = NULL;
//
//	//we want it to always loop
//	hr = this->iflash->put_Loop(true);
//
//	//load the movie
//	hr = this->iflash->put_Movie((BSTR)this->real_name);
//
//	//since these are always going to be local media,  force wait until ready state is loaded
//	//for(this->state = -1; (!hr) && (this->state != 4);)
//	//{
//	//	hr = this->iflash->get_ReadyState(&this->state);
//	//	if(this->state == 4)
//	//	{
//	//		//get the total frames of the SWF file
//	//		hr = this->iflash->get_TotalFrames(&this->totalframes);
//	//		break;
//	//	}
//	//	Sleep(0); //snooze
//	//}
//
//	hr = this->iflash->Play();
//	//this->flashThreadStatus = eRunning;
//
//	//our own little message loop.  The ActiveX container needs this to run outside of the main proc thread.
//	//MSG msg;
//	//while (GetMessage (&msg, 0, 0, 0))
//	//{	
//	//	//TO-Do mutex this!
//	//	if(eWaitingToStop == this->flashThreadStatus)
//	//	{
//	//		break;
//	//	}
//	//	else
//	//	{
//	//		TranslateMessage(&msg);
//	//		DispatchMessage (&msg);
//	//	}
//	//}
//
//	//this->flashThreadStatus = eFinished;
//	return 0;
//}

SwfContainer::~SwfContainer(void)
{
	//kill the memtexture
	memtex->Release();

	//release the viewobject and it's evil twin RTviewobject
	RTviewobject->Release();
	viewobject->Release();

	//unadvise the event sink
	m_lpConPoint->Unadvise(m_dwConPointID);

	//stop the flash thread
	/*flashThreadStatus = eWaitingToStop;
	while(flashThreadStatus == eWaitingToStop)
	{
		Sleep(10);
	}
	CloseHandle(hThread);*/
	iflash->Release();
}

//Draw the contents of the SFW hiden window onto a Render Target in the GPU (m_pOutTexture)
HRESULT SwfContainer::GetFrame(IDirect3DDevice9 * m_pDevice,IDirect3DTexture9 * m_pOutTexture)
{
	HRESULT hr = S_OK;

	//Don't bother if it's not fully loaded
	if(state != 4) return hr;

	//do we need to un-marshall our version of the view object?
	if(!RTviewobject)
	{
		CoGetInterfaceAndReleaseStream (pStream, __uuidof(IViewObject), (void**) &RTviewobject);
	}

	RECT rc={0,0,0,0};
	rc.right = _width;
	rc.bottom = _height;
	
	VARIANT_BOOL isplay;
	long fn = -1;

	iflash->IsPlaying(&isplay);
	iflash->CurrentFrame(&fn);

	//clear out the memtexture
	D3DLOCKED_RECT d3dlr;
	hr = memtex->LockRect(0,&d3dlr,0,D3DLOCK_DONOTWAIT);
	_int32 * buffer = (_int32 *)d3dlr.pBits;
	DWORD buffersize = _width * _height;
	for(int i = 1; i < buffersize; i++)
	{
		buffer[i] = (_int32)0x00000000;
	}
	memtex->UnlockRect(0);

	//draw frame onto surface of memtexture
	IDirect3DSurface9 * memsurface = NULL;
	memtex->GetSurfaceLevel(0,&memsurface);
	
	//Draw contents of hidden SWF window directly onto surface of in-memory texture
	HDC hdcCompatible;
	hr = memsurface->GetDC(&hdcCompatible);
	//Sanity check
	SetMapMode(hdcCompatible,MM_TEXT);

	if(!hr)
	{
		//draw the frame
		try
		{			
			hr = OleDraw(RTviewobject,DVASPECT_CONTENT,hdcCompatible,&rc);
		}
		catch(HRESULT hr1)
		{
			hr = hr1;
		}
		memsurface->ReleaseDC(hdcCompatible);
	}
	memsurface->Release();

	//upload mem-texture to render target on GPU
	memtex->AddDirtyRect(NULL);
	m_pDevice->UpdateTexture(memtex,m_pOutTexture);
	return hr;
}

//implement these if we want event feedback from Flash Scripting
//DShockwaveFlashEvents
HRESULT STDMETHODCALLTYPE SwfContainer::OnReadyStateChange(long newState)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SwfContainer::OnProgress(long percentDone)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SwfContainer::FSCommand(BSTR command, BSTR args)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SwfContainer::FlashCall(BSTR request)
{
	return S_OK;
}

//IDispatch Impl
HRESULT STDMETHODCALLTYPE SwfContainer::GetTypeInfoCount(UINT __RPC_FAR *pctinfo)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE SwfContainer::GetTypeInfo( 
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE SwfContainer::GetIDsOfNames( 
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
	/* [in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE SwfContainer::Invoke( 
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
	/* [out] */ VARIANT __RPC_FAR *pVarResult,
	/* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
	/* [out] */ UINT __RPC_FAR *puArgErr)
{
	return S_OK;
}

//IUnknown Impl
HRESULT STDMETHODCALLTYPE SwfContainer::QueryInterface(REFIID riid, void ** ppvObject)
{
	if (IsEqualGUID(riid, DIID__IShockwaveFlashEvents))
		*ppvObject = (void*)dynamic_cast<_IShockwaveFlashEvents *>(this);
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
	if (!(*ppvObject))
		return E_NOINTERFACE; //if dynamic_cast returned 0
	m_iRef++;
	return S_OK;
}

ULONG STDMETHODCALLTYPE SwfContainer::AddRef()
{
	m_iRef++;
	return m_iRef;
}

ULONG STDMETHODCALLTYPE SwfContainer::Release()
{
	m_iRef--;
	return m_iRef;
}