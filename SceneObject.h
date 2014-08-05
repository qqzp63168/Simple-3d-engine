//场景中可见与不可见的对象

#pragma once
#include"ProjectHead.h"
#include"MessageAccepter.h"
#include"shap.h"
using namespace std;


class SceneObject :public MessageAccepter 
						  
{
public:
	int iNum;
	string sName ;
	int iType;
	string luapath;
	string luapathc;
	shap* pShap;
	D3DXMATRIXA16 mMat;//世界矩阵
	controlInfo m_CI;

	int getMessage(EngineMessage * pMs);
	int runObject();//执行对象的动作（带有尝试性，有可能会被修改）
	 strObjectInfo getObjectInfo();
	 int accepteMessage(EngineMessage *pM,int iAmount);
	string setname(string name);
	int setLocation(float x,float y,float z);
	float vx;
	float vy;
	float vz;
	SceneObject(void);
	~SceneObject(void);
	virtual void getControl(controlInfo CI);
};

