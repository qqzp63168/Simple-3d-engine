#pragma once
#include<list>
#include"SceneObject.h"

using namespace std;

class SceneObjectList
{
public:
	list<SceneObject*> L_SceneObject;//场景中的对象链表
	list<SceneObject*>::iterator iteratorTmp;//临时的iterator
	
	void addObject(SceneObject *pGOb);
	list<SceneObject*>::iterator deleteObject(int iNum);
	int resetIterator();
	SceneObject* nextObject();
	SceneObject* getObject();
	SceneObjectList(void);
	~SceneObjectList(void);
	
};

