//信息类共用类型
#pragma once
#include<string>
#include<sstream>
#include<Windows.h>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace std;
//结构体定义

typedef struct//信息对象信息
{
	int type;//类型
}strObjectInfo;//描述对象与信息有关的信息（用于信息的接受与发送）

wstring s2ws(const string& s);

typedef struct 
{
	int up;
	int down;
	int right;
	int left;
	int space;
	int ctrl;
} controlInfo;