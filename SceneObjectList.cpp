#include "StdAfx.h"
#include "SceneObjectList.h"


SceneObjectList::SceneObjectList(void)
{
}


SceneObjectList::~SceneObjectList(void)
{
}

void SceneObjectList::addObject(SceneObject *pGOb)
{
		if(NULL==pGOb){return;}

	 L_SceneObject.push_back(pGOb);
}
int SceneObjectList::resetIterator( )
{

	iteratorTmp=L_SceneObject.begin();
	return 1;
}

SceneObject* SceneObjectList::nextObject()
{
	iteratorTmp++;
	if(iteratorTmp==L_SceneObject.end())
	{
		iteratorTmp=L_SceneObject.begin();
		return NULL;
	}

	

	return  (*iteratorTmp);
}
SceneObject* SceneObjectList::getObject()
{
	if(L_SceneObject.size()<1)
	{
		return NULL;
	}



	return  (*iteratorTmp);
	
}