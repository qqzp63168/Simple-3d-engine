//���� 
//ѭ��ִ�ж��������������ı仯

#pragma once
#include"gameObject.h"
#include"SceneObjectList.h"
#include"SceneObjectList.h"
#include"ProjectHead.h"
#include"shap.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//d3d
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif




class GameScene
{
public:
	
	HDC Hwdc; //����dc���
	HWND Hwnd;//���ھ��
	
int rendercs; //������
	int render();
	int runScene();
	controlInfo CI;
	lua_State *pSLua;

	GameScene(void);
	GameScene(HWND hwnd);//hwnd ���ھ��
	~GameScene(void);
	static int CreateCTest(lua_State* L);
	void getControl(int iMes );
	void getControlDown(int iMes);
private:
	

};

