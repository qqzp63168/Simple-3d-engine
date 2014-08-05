#pragma once
#include<list>
#include"SceneObject.h"

using namespace std;

class SceneObjectList
{
public:
	list<SceneObject*> L_SceneObject;//�����еĶ�������
	list<SceneObject*>::iterator iteratorTmp;//��ʱ��iterator
	
	void addObject(SceneObject *pGOb);
	list<SceneObject*>::iterator deleteObject(int iNum);
	int resetIterator();
	SceneObject* nextObject();
	SceneObject* getObject();
	SceneObjectList(void);
	~SceneObjectList(void);
	
};

