//�����пɼ��벻�ɼ��Ķ���

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
	D3DXMATRIXA16 mMat;//�������
	controlInfo m_CI;

	int getMessage(EngineMessage * pMs);
	int runObject();//ִ�ж���Ķ��������г����ԣ��п��ܻᱻ�޸ģ�
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

