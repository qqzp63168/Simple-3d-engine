#include "StdAfx.h"
#include "SceneObject.h"

using namespace std;
SceneObject::SceneObject(void)
{
	iNum=0;
	sName="sourceObject";
	iType=0;
	luapath="luatest.lua";
	luapathc="lua//null.lua";
	pShap=NULL;
	D3DXMatrixTranslation(&mMat,0,0,0);
}

SceneObject::~SceneObject(void)
{
	delete pShap;
}
int SceneObject::runObject()
{

	return 1;
}
string SceneObject::setname(string name)
{
	sName=name;
	return name;
}
int SceneObject::setLocation(float x,float y,float z)
{
	D3DXMatrixTranslation(&mMat,x,y,z);
	return 1;
}
void SceneObject::getControl(controlInfo CI)
{
	m_CI=CI;
}