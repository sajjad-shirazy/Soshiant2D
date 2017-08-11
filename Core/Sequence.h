#include "Engine.h"

#pragma once

namespace Soshiant2D{
	class Manager;

	class Sequence
	{
	private :
		lua_State*		   luaState;
		
		Physics*           p_physics;
		std::list<Layer*>  p_layers;		
		std::list<Sprite*> p_keyDownEventFunctions;
		std::list<Sprite*> p_keyUpEventFunctions;
		std::list<Sprite*> p_onEnterFrameEventFunctions;
		std::list<Sprite*> p_onLoadEventFunctions;
		std::list<Sprite*> p_mouseMoveEventFunctions;
		std::list<Sprite*>::iterator itt;

		Layer*  p_focusLayer;
		Layer*  p_pivotLayer;
		Sprite* p_cameraLookAt;

		std::string name;
		Vector3 center;
		D3DCOLOR backColor;
		bool p_pauseMode;
		bool isPhysical;
		bool visible;

		void LoadFromFile(std::string path);
		void UpdateLayers();
		void DrawLayers();
		void DrowLines();
		void ParalaxScroling();
		void Release();

	public :	
		Layer*			   p_activeLayer;
		Library*		   _library;
		Manager*		   _parent;

		Audio *audio;

		Sequence(std::string fileName);
		~Sequence();

		void Update();
		void Render();		
		int Render2D_Start();
		int Render2D_Stop();
		void KeyDown(int e);
		void KeyUp(int e);
		void OnLoad();
		void MouseMove(int x,int y);

		Physics*	getPhysicsHandler(){ return this->p_physics; }
		bool isPaused() { return this->p_pauseMode; }
		void setPaused(bool value) { this->p_pauseMode = value; }

		bool isVisible() { return this->visible; }
		void setVisible(bool value) { this->visible = value; }

		void setName(std::string name){this->name = name;}
		std::string getName(){return this->name;}

		void SetBackColor(D3DCOLOR color){ this->backColor=color; }

		std::list<Layer*> getLayersList() { return p_layers; }
		long getLayersCount() { return (long)p_layers.size(); }
		Layer *findLayer(std::string name);
		Layer *findLayer(int ID);

		int addLayer(Layer* layer);

		void setActiveLayer(int id);
		Layer* getAvtiveLayer(){ return p_activeLayer; }

		void setFocusLayer(int id);
		Layer* getFocusLayer(){ return p_focusLayer; }

		void setPivotLayer(int id);
		Layer* getPivotLayer(){ return p_pivotLayer; }

		Layer* getZeroLayer(){std::list<Layer*>::iterator k = p_layers.begin();return (*k);}

		void AddKeyDownEventFunction(Sprite* sprite);
		void AddKeyUpEventFunction(Sprite* sprite);
		void AddOnEnterFrameEventFunction(Sprite* sprite);
		void AddOnLoadEventFunction(Sprite* sprite);
		void AddMouseMoveEventFunction(Sprite* sprite);
	};
}