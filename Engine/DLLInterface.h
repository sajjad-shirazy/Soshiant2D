#pragma once
#include "Engine.h"

extern "C" {
	//Engine Functions ...	
	__declspec(dllexport) void InitEngine( HWND hWnd , int width,int height,int depth,bool fullscreen);
	__declspec(dllexport) void ResizeEngine(int width,int height);
	__declspec(dllexport) void Render();
	__declspec(dllexport) void CloseEngine();

	__declspec(dllexport) int  GetRealFPS();
	__declspec(dllexport) int  GetCoreFPS();

	__declspec(dllexport) void SetPause(bool choice);

	//Game Modifing Functions
	__declspec(dllexport) void SetBackColor(int R,int G, int B);
	//@@AddSprite returnd ID
	__declspec(dllexport) int  AddSprite(char* name,double x,double y,double scale,double rotation,int alpha,int row,int col,int totalFrame,int width,int height,int anim_timer,int isRev,double xmov,double ymov,int mov_timer,char* imagePath,bool isPhysical,bool isStatic,float mass,float inertia);
	//@@select by Mouse clicking
	__declspec(dllexport) int  SelectSprite(int x,int y);
	__declspec(dllexport) void SetSelectedSprite(int ID);
	__declspec(dllexport) bool IsSelectedSprite();
	__declspec(dllexport) void UpdateSprite(int ID,int x,int y);
	__declspec(dllexport) void ResetSprite(int ID,char* name,double x,double y,double scale,double rotation,int alpha,int width,int height,int row,int col,int totalFrame,int isRev,int anim_timer,double xmov,double ymov,int mov_timer , bool isPhysical , bool isStatic , float mass , float inertia);
	__declspec(dllexport) void ResetSpriteImage(int ID,char* imagePath);
	__declspec(dllexport) int  GetSpriteWidth(int ID);
	__declspec(dllexport) int  GetSpriteHeight(int ID);
	__declspec(dllexport) void DeleteSprite(int ID);


	//@@Layer Mangment Metods ....
	__declspec(dllexport) int  AddLayer(char* name,double z);
	__declspec(dllexport) void SetActivLayer(int ID);
	__declspec(dllexport) void SetFocusLayer(int ID);
	__declspec(dllexport) void SetPivotLayer(int ID);
	__declspec(dllexport) void ResetLayersPosition();
	__declspec(dllexport) void ResetLayers(char* name,double z);

	__declspec(dllexport) void UpdateEditorPositionBase(int x,int y);

	//Mouse Functions
	__declspec(dllexport) void EditorMouseDown(int x,int y);

	//Shape Functions
	__declspec(dllexport) void setActiveShape(int id);
	__declspec(dllexport) void setShapeVisible(int id , bool value);
	__declspec(dllexport) void RemoveShape(int id);
	__declspec(dllexport) void SwitchEditeMode(bool value);
	__declspec(dllexport) bool SelectPoint(int x,int y);
	__declspec(dllexport) void UpdatePoint(int x,int y);



	__declspec(dllexport) int  AddLineShape(float ax , float ay , float bx , float by ,float r);
	__declspec(dllexport) void ResetLineShape(float ax , float ay , float bx , float by ,float r);
	__declspec(dllexport) void GetLineProperties(int id , float* carry);


	__declspec(dllexport) int  AddCircleShape(float x , float y , float r );
	__declspec(dllexport) void ResetCircleShape(float x , float y , float r );
	__declspec(dllexport) void GetCircleProperties(int id , float* carry);


	//Particle System Methods
	__declspec(dllexport) int  AddParticleSystem(char* name,double x,double y,int count,int mnpl,int mxpl,int isRep,int scr,int scg,int scb,int sa,int ecr,int ecg,int ecb,int ea,float mnScale,float mxScale,float acc_x,float acc_y,float grv_x,float grv_y,float mnVel,float mxVel,char* imagePath);
	__declspec(dllexport) void ResetParticleSystem(int ID,char* name,double x,double y,int count,int mnpl,int mxpl,int isRep,int scr,int scg,int scb,int sa,int ecr,int ecg,int ecb,int ea,float mnScale,float mxScale,float acc_x,float acc_y,float grv_x,float grv_y,float mnVel,float mxVel,char* imagePath);
	__declspec(dllexport) void ResetParticleSystemImage(int ID,char* imagePath);
	
};